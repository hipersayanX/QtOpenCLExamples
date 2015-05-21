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
#include <CL/cl.hpp>

#define NNUMBERS (3 * 7 * 11 * 13 * 17 * 19 * 23)
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
    QFile kernelFile(":/vecsum.cl");
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

    // Create buffers
    QVector<cl_float> aNumbers(NNUMBERS);
    QVector<cl_float> bNumbers(NNUMBERS);
    QVector<cl_float> cNumbers(NNUMBERS);

    // Initialize input
    for (int i = 0; i < aNumbers.size(); i++) {
        aNumbers[i] = i % 128;
        bNumbers[i] = 256 - aNumbers[i];
    }

    cl::Buffer aBuffer(context,
                       CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
                       sizeof(cl_float) * aNumbers.size(),
                       aNumbers.data());

    cl::Buffer bBuffer(context,
                       CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
                       sizeof(cl_float) * bNumbers.size(),
                       bNumbers.data());

    cl::Buffer cBuffer(context,
                       CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
                       sizeof(cl_float) * cNumbers.size(),
                       cNumbers.data());

    cl::CommandQueue commandQueue(context, devices[0]);

    // Obtain a reference to the kernels
    cl::make_kernel<float, cl::Buffer &, cl::Buffer &, cl::Buffer &, int> vecsum(program, "vecsum");

    QElapsedTimer eTimer;
    eTimer.start();

    // serial vector sum
    for (int i = 0; i < NNUMBERS; i++)
        cNumbers[i] = 2.0 * aNumbers[i] + bNumbers[i];

    qDebug() << eTimer.elapsed();
//    qDebug() << cNumbers;

    eTimer.restart();

    int range;
    int groupSize = WORKGROUPSIZE;
    int oSize;

    calculateNDRange(NNUMBERS, groupSize,
                     &range, &groupSize, &oSize);

    // Apply kernel
    vecsum(cl::EnqueueArgs(commandQueue,
                           cl::NDRange(0),
                           cl::NDRange(range),
                           cl::NDRange(groupSize)),
           2.0, aBuffer, bBuffer, cBuffer, NNUMBERS);

    // Retrieve buffer
    commandQueue.enqueueReadBuffer(cBuffer,
                                   CL_TRUE,
                                   0,
                                   sizeof(cl_float) * cNumbers.size(),
                                   cNumbers.data());

    qDebug() << eTimer.elapsed();
//    qDebug() << cNumbers;

    return 0;
}
