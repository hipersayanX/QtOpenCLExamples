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

#define WORKGROUPSIZE 16

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
    QFile kernelFile(":/edges.cl");
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

    QImage imageIn("image.jpg");
    imageIn = imageIn.convertToFormat(QImage::Format_ARGB32);

    // Convert to gray scale
    for (int y = 0; y < imageIn.height(); y++) {
        QRgb *line = (QRgb *) imageIn.scanLine(y);

        for (int x = 0; x < imageIn.width(); x++) {
            int r = qGray(line[x]);
            int g = r;
            int b = r;
            line[x] = qRgb(r, g, b);
        }
    }

    QImage imageOut(imageIn.size(), imageIn.format());

    cl::CommandQueue commandQueue(context, devices[0]);

    // Obtain a reference to the kernel
    cl::make_kernel<cl::Image2D &, cl::Image2D &> edges(program, "edges");

    QElapsedTimer eTimer;
    eTimer.start();

    // Serial edge detector
    for (int y = 0; y < imageIn.height(); y++) {
        QRgb *iLine = (QRgb *) imageIn.scanLine(y);

        QRgb *iLine_m1 = (y < 1)?
                             iLine:
                             (QRgb *) imageIn.scanLine(y - 1);

        QRgb *iLine_p1 = (y >= imageIn.height())?
                             iLine:
                             (QRgb *) imageIn.scanLine(y + 1);

        QRgb *oLine = (QRgb *) imageOut.scanLine(y);

        for (int x = 0; x < imageIn.width(); x++) {
            int x_m = (x < 1)? x: x - 1;
            int x_p = (x >= imageIn.width())? x: x + 1;

            // Sobel operator
            int grayX = qRed(iLine_p1[x_m])
                      + 2 * qRed(iLine_p1[x])
                      + qRed(iLine_p1[x_p])
                      - qRed(iLine_m1[x_m])
                      - 2 * qRed(iLine_m1[x])
                      - qRed(iLine_m1[x_p]);

            int grayY = qRed(iLine_m1[x_p])
                      + 2 * qRed(iLine[x_p])
                      + qRed(iLine_p1[x_p])
                      - qRed(iLine_m1[x_m])
                      - 2 * qRed(iLine[x_m])
                      - qRed(iLine_p1[x_m]);

            int gray = sqrt(grayX * grayX + grayY * grayY);
            gray = qBound(0, gray, 255);

            oLine[x] = qRgb(gray, gray, gray);
        }
    }

    qDebug() << "serial" << eTimer.elapsed();

    imageOut.save("edges-serial.jpg");
    imageIn = imageIn.convertToFormat(QImage::Format_RGBA8888);

    // Create image buffers
    cl::Image2D clImageIn(context,
                          CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
                          cl::ImageFormat(CL_RGBA, CL_UNSIGNED_INT8),
                          imageIn.width(),
                          imageIn.height(),
                          imageIn.bytesPerLine(),
                          imageIn.bits());

    cl::Image2D clImageOut(context,
                           CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
                           cl::ImageFormat(CL_RGBA, CL_UNSIGNED_INT8),
                           imageOut.width(),
                           imageOut.height(),
                           imageOut.bytesPerLine(),
                           imageOut.bits());

    eTimer.restart();

    int rangeX;
    int rangeY;
    int groupSizeX;
    int groupSizeY;
    int oSize;

    // Calgulate range values
    calculateNDRange(imageIn.width(), WORKGROUPSIZE,
                     &rangeX, &groupSizeX, &oSize);

    calculateNDRange(imageIn.height(), WORKGROUPSIZE,
                     &rangeY, &groupSizeY, &oSize);

    // Apply kernel
    edges(cl::EnqueueArgs(commandQueue,
                          cl::NDRange(0, 0),
                          cl::NDRange(rangeX, rangeY),
                          cl::NDRange(groupSizeX, groupSizeY)),
          clImageIn, clImageOut);

    cl::size_t<3> origin;
    origin[0] = 0;
    origin[1] = 0;
    origin[2] = 0;

    cl::size_t<3> region;
    region[0] = imageOut.width();
    region[1] = imageOut.height();
    region[2] = 1;

    // Retrieve output image
    commandQueue.enqueueReadImage(clImageOut, CL_TRUE, origin, region, 0, 0, imageOut.bits());

    qDebug() << "paralell" << eTimer.elapsed();

    imageOut.save("edges-paralell.jpg");

    return 0;
}
