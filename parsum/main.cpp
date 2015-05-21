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
    QFile kernelFile(":/parsum.cl");
    kernelFile.open(QIODevice::ReadOnly | QIODevice::Text);
    cl::Program program(context, kernelFile.readAll().toStdString());
    kernelFile.close();

    // Compile kernels
    cl_int error = program.build(QString("-D WORKGROUPSIZE=%1")
                                 .arg(WORKGROUPSIZE)
                                 .toStdString()
                                 .c_str());

    if (error != CL_SUCCESS) {
        cl::STRING_CLASS buildLog;
        program.getBuildInfo(devices[0], CL_PROGRAM_BUILD_LOG, &buildLog);
        qDebug() << buildLog.c_str();

        return -1;
    }

    // Create buffers
    QVector<cl_int> inputNumbers(NNUMBERS);
    QVector<cl_int> outputNumbers(NNUMBERS);

    // Initialize input
    for (int i = 0; i < inputNumbers.size(); i++)
        inputNumbers[i] = i % 128;

    cl::Buffer inputBuffer(context,
                           CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
                           sizeof(cl_int) * inputNumbers.size(),
                           inputNumbers.data());

    cl::Buffer outputBuffer(context,
                            CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
                            sizeof(cl_int) * outputNumbers.size(),
                            outputNumbers.data());

    cl::CommandQueue commandQueue(context, devices[0]);

    // Obtain a reference to the kernels
    cl::make_kernel<cl::Buffer &, cl::Buffer &, int> parsum(program, "parsum");

    QElapsedTimer eTimer;
    eTimer.start();
    cl_int sum = 0;

    // Serial prefix sum
    for (int i = 0; i < inputNumbers.size(); i++)
        sum += inputNumbers[i];

    qDebug() << sum << eTimer.elapsed();

    eTimer.restart();

    int inputSize = inputNumbers.size();
    int groupSize = WORKGROUPSIZE;
    int oSize;
    int range;

    // Parallel sum
    forever {
        calculateNDRange(inputSize, groupSize,
                         &range, &groupSize, &oSize);

        // Apply kernel
        parsum(cl::EnqueueArgs(commandQueue,
                               cl::NDRange(0),
                               cl::NDRange(range),
                               cl::NDRange(groupSize)),
               inputBuffer, outputBuffer, inputSize);

        if (oSize < 2)
            break;

        inputSize = oSize;
        cl::Buffer tmpBuffer(inputBuffer);
        inputBuffer = outputBuffer;
        outputBuffer = tmpBuffer;
    }

    // Retrieve buffer
    commandQueue.enqueueReadBuffer(outputBuffer,
                                   CL_TRUE,
                                   0,
                                   sizeof(cl_int) * outputNumbers.size(),
                                   outputNumbers.data());

    qDebug() << outputNumbers[0] << eTimer.elapsed();

    return 0;
}
