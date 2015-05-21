/* QtOpenCLExamples, Qt 5 + OpenCL 1.2 usage examples.
 * Copyright (C) 2015  Gonzalo Exequiel Pedone
 *
 * QtOpenCLExamples is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QtOpenCLExamples is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QtOpenCLExamples. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email   : hipersayan DOT x AT gmail DOT com
 * Web-Site: http://github.com/hipersayanX/QtOpenCLExamples
 */

#include <QtCore>
#include <QImage>
#include <CL/cl.hpp>

#define WORKGROUPSIZE 32

inline void calculateNDRange(int size, int groupSize,
                             int *bSize, int *bGroupSize, int *oSize)
{
    if (size <= groupSize)
        groupSize = 1 << int(log(size) / log(2));
    else
        groupSize = 1 << int(log(groupSize) / log(2));

    if (size % groupSize)
        size = size + groupSize - size % groupSize;

    *bSize = size;
    *bGroupSize = groupSize;
    *oSize = size / groupSize;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Q_UNUSED(a)

    // Query platforms
    VECTOR_CLASS<cl::Platform> platforms;

    if (cl::Platform::get(&platforms) != CL_SUCCESS) {
        qDebug() << "Cann't query platforms";

        return -1;
    }

    if (platforms.size() < 1) {
        qDebug() << "No platforms available";

        return -1;
    }

    // Query devices
    cl::Platform platform = platforms[0];
    VECTOR_CLASS<cl::Device> devices;

    if (platform.getDevices(CL_DEVICE_TYPE_ALL, &devices) != CL_SUCCESS) {
        qDebug() << "No devices available";

       return -1;
    }

    // Create OpenCL context
    cl::Context context(devices);

    // Load OpenCL program
    QFile kernelFile(":/intimg.cl");
    kernelFile.open(QIODevice::ReadOnly | QIODevice::Text);
    cl::Program program(context, kernelFile.readAll().toStdString());
    kernelFile.close();

    // Compile kernels
    cl_int error = program.build();

    if (error != CL_SUCCESS) {
        cl::STRING_CLASS buildLog;
        program.getBuildInfo(devices[0], CL_PROGRAM_BUILD_LOG, &buildLog);
        qDebug() << buildLog.c_str();

        return -1;
    }

    QImage image(":/image.jpg");
    image = image.convertToFormat(QImage::Format_ARGB32);
    int videoArea = image.width() * image.height();

    // Create buffers
    QVector<cl_uint3> integral(videoArea);
    QVector<cl_ulong3> integral2(videoArea);

    cl::CommandQueue commandQueue(context, devices[0]);

    // Obtain a reference to the kernels
    cl::make_kernel<cl::Image2D &, cl::Buffer &, cl::Buffer &> horizontalIntegral(program, "horizontalIntegral");
    cl::make_kernel<int, int, cl::Buffer &, cl::Buffer &> verticalIntegral(program, "verticalIntegral");

    // Serial integral image calculation
    const QRgb *srcBits = (const QRgb *) image.constBits();

    quint32 *integralR = new quint32[videoArea];
    quint32 *integralG = new quint32[videoArea];
    quint32 *integralB = new quint32[videoArea];

    quint64 *integral2R = new quint64[videoArea];
    quint64 *integral2G = new quint64[videoArea];
    quint64 *integral2B = new quint64[videoArea];

    QElapsedTimer eTimer;
    eTimer.start();

    quint32 sumR = 0;
    quint32 sumG = 0;
    quint32 sumB = 0;

    quint64 sum2R = 0;
    quint64 sum2G = 0;
    quint64 sum2B = 0;

    for (int i = 0; i < image.width(); i++) {
        quint8 r = qRed(srcBits[i]);
        quint8 g = qGreen(srcBits[i]);
        quint8 b = qBlue(srcBits[i]);

        sumR += r;
        sumG += g;
        sumB += b;

        sum2R += r * r;
        sum2G += g * g;
        sum2B += b * b;

        integralR[i] = sumR;
        integralG[i] = sumG;
        integralB[i] = sumB;

        integral2R[i] = sum2R;
        integral2G[i] = sum2G;
        integral2B[i] = sum2B;
    }

    quint32 posPrev = 0;
    quint32 pos = image.width();

    for (int j = 1; j < image.height(); j++) {
        sumR = 0;
        sumG = 0;
        sumB = 0;

        sum2R = 0;
        sum2G = 0;
        sum2B = 0;

        for (int i = 0; i < image.width(); i++, posPrev++, pos++) {
            quint8 r = qRed(srcBits[pos]);
            quint8 g = qGreen(srcBits[pos]);
            quint8 b = qBlue(srcBits[pos]);

            sumR += r;
            sumG += g;
            sumB += b;

            sum2R += r * r;
            sum2G += g * g;
            sum2B += b * b;

            integralR[pos] = sumR + integralR[posPrev];
            integralG[pos] = sumG + integralG[posPrev];
            integralB[pos] = sumB + integralB[posPrev];

            integral2R[pos] = sum2R + integral2R[posPrev];
            integral2G[pos] = sum2G + integral2G[posPrev];
            integral2B[pos] = sum2B + integral2B[posPrev];
        }
    }

    qDebug() << "serial"
             << "R" << integralR[videoArea - 1]
             << "G" << integralG[videoArea - 1]
             << "B" << integralB[videoArea - 1]
             << "R2" << integral2R[videoArea - 1]
             << "G2" << integral2G[videoArea - 1]
             << "B2" << integral2B[videoArea - 1]
             << eTimer.elapsed();

    delete [] integralR;
    delete [] integralG;
    delete [] integralB;

    delete [] integral2R;
    delete [] integral2G;
    delete [] integral2B;

    image = image.convertToFormat(QImage::Format_RGBA8888);

    cl_int err;
    cl::Image2D imageBuffer(context,
                            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
                            cl::ImageFormat(CL_RGBA, CL_UNSIGNED_INT8),
                            image.width(),
                            image.height(),
                            image.bytesPerLine(),
                            image.bits(),
                            &err);

    cl::Buffer integralBuffer(context,
                              CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
                              sizeof(cl_uint3) * integral.size(),
                              integral.data());

    cl::Buffer integralBuffer2(context,
                              CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
                              sizeof(cl_ulong3) * integral2.size(),
                              integral2.data());

    eTimer.restart();

    int range;
    int groupSize = WORKGROUPSIZE;
    int oSize;

    // Apply kernels
    calculateNDRange(image.height(), groupSize,
                     &range, &groupSize, &oSize);

    horizontalIntegral(cl::EnqueueArgs(commandQueue,
                                       cl::NDRange(0),
                                       cl::NDRange(range),
                                       cl::NDRange(groupSize)),
                       imageBuffer, integralBuffer, integralBuffer2);

    calculateNDRange(image.width(), groupSize,
                     &range, &groupSize, &oSize);

    verticalIntegral(cl::EnqueueArgs(commandQueue,
                                     cl::NDRange(0),
                                     cl::NDRange(range),
                                     cl::NDRange(groupSize)),
                     image.width(), image.height(),
                     integralBuffer, integralBuffer2);

    // Retrieve buffers
    commandQueue.enqueueReadBuffer(integralBuffer,
                                   CL_TRUE,
                                   0,
                                   sizeof(cl_uint3) * integral.size(),
                                   integral.data());

    commandQueue.enqueueReadBuffer(integralBuffer2,
                                   CL_TRUE,
                                   0,
                                   sizeof(cl_ulong3) * integral2.size(),
                                   integral2.data());

    qDebug() << "paralell"
             << "R" << integral.last().s0
             << "G" << integral.last().s1
             << "B" << integral.last().s2
             << "R2" << integral2.last().s0
             << "G2" << integral2.last().s1
             << "B2" << integral2.last().s2
             << eTimer.elapsed();

    return 0;
}
