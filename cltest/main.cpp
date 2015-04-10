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

#include <QCoreApplication>
#include <QDebug>
#include <CL/cl.hpp>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Q_UNUSED(a)

    VECTOR_CLASS<cl::Platform> platforms;

    if (cl::Platform::get(&platforms) != CL_SUCCESS) {
        qDebug() << "Cann't query platforms";

        return -1;
    }

    if (platforms.size() < 1) {
        qDebug() << "No platforms available";

        return -1;
    }

    qDebug() << "Number of platforms:" << platforms.size();
    qDebug() << "";
    int i = 0;

    foreach (cl::Platform platform, platforms) {
        qDebug() << "Platform:" << i;

        cl::STRING_CLASS name;
        platform.getInfo(CL_PLATFORM_NAME, &name);
        qDebug() << "    Name:" << name.c_str();

        cl::STRING_CLASS vendor;
        platform.getInfo(CL_PLATFORM_VENDOR, &vendor);
        qDebug() << "    Vendor:" << vendor.c_str();

        cl::STRING_CLASS version;
        platform.getInfo(CL_PLATFORM_VERSION, &version);
        qDebug() << "    Version:" << version.c_str();

        cl::STRING_CLASS profile;
        platform.getInfo(CL_PLATFORM_PROFILE, &profile);
        qDebug() << "    Profile:" << profile.c_str();

        cl::STRING_CLASS extensions;
        platform.getInfo(CL_PLATFORM_EXTENSIONS, &extensions);
        qDebug() << "    Extensions:" << extensions.c_str();

        VECTOR_CLASS<cl::Device> devices;

        if (platform.getDevices(CL_DEVICE_TYPE_ALL, &devices) == CL_SUCCESS) {
            qDebug() << "    Number of devices:" << devices.size();
            qDebug() << "";

            for (uint device = 0; device < devices.size(); device++) {
                qDebug() << "        Device:" << device;

                cl_device_type type;
                devices[device].getInfo(CL_DEVICE_TYPE, &type);

                QStringList deviceTypes;

                if (type & CL_DEVICE_TYPE_DEFAULT)
                    deviceTypes << "CL_DEVICE_TYPE_DEFAULT";

                if (type & CL_DEVICE_TYPE_CPU)
                    deviceTypes << "CL_DEVICE_TYPE_CPU";

                if (type & CL_DEVICE_TYPE_GPU)
                    deviceTypes << "CL_DEVICE_TYPE_GPU";

                if (type & CL_DEVICE_TYPE_ACCELERATOR)
                    deviceTypes << "CL_DEVICE_TYPE_ACCELERATOR";

                if (type & CL_DEVICE_TYPE_CUSTOM)
                    deviceTypes << "CL_DEVICE_TYPE_CUSTOM";

                qDebug() << "            Type:" << deviceTypes;

                cl_uint vendorId = 0;
                devices[device].getInfo(CL_DEVICE_VENDOR_ID, &vendorId);
                qDebug() << "            Vendor ID:" << vendorId;

                cl_uint maxComputeUnits = 0;
                devices[device].getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &maxComputeUnits);
                qDebug() << "            CL_DEVICE_MAX_COMPUTE_UNITS:" << maxComputeUnits;

                cl_uint maxWorkItemDimensions = 0;
                devices[device].getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &maxWorkItemDimensions);
                qDebug() << "            CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:" << maxWorkItemDimensions;

                cl::size_t<3> maxWorkItemSizes;
                devices[device].getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &maxWorkItemSizes);

                QList<int> maxWorkItemSizesList;

                for (int i = 0; i < 3; i++)
                    maxWorkItemSizesList << maxWorkItemSizes[i];

                qDebug() << "            CL_DEVICE_MAX_WORK_ITEM_SIZES:" << maxWorkItemSizesList;

                size_t maxWorkGroupSize = 0;
                devices[device].getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &maxWorkGroupSize);
                qDebug() << "            CL_DEVICE_MAX_WORK_GROUP_SIZE:" << maxWorkGroupSize;

                cl_uint preferredVectorWidthChar = 0;
                devices[device].getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, &preferredVectorWidthChar);
                qDebug() << "            CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR:" << preferredVectorWidthChar;

                cl_uint preferredVectorWidthShort = 0;
                devices[device].getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, &preferredVectorWidthShort);
                qDebug() << "            CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT:" << preferredVectorWidthShort;

                cl_uint preferredVectorWidthInt = 0;
                devices[device].getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, &preferredVectorWidthInt);
                qDebug() << "            CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT:" << preferredVectorWidthInt;

                cl_uint preferredVectorWidthLong = 0;
                devices[device].getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, &preferredVectorWidthLong);
                qDebug() << "            CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG:" << preferredVectorWidthLong;

                cl_uint preferredVectorWidthFloat = 0;
                devices[device].getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, &preferredVectorWidthFloat);
                qDebug() << "            CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT:" << preferredVectorWidthFloat;

                cl_uint preferredVectorWidthDouble = 0;
                devices[device].getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, &preferredVectorWidthDouble);
                qDebug() << "            CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE:" << preferredVectorWidthDouble;

                cl_uint preferredVectorWidthHalf = 0;
                devices[device].getInfo(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, &preferredVectorWidthHalf);
                qDebug() << "            CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF:" << preferredVectorWidthHalf;

                cl_uint nativeVectorWidthChar = 0;
                devices[device].getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR, &nativeVectorWidthChar);
                qDebug() << "            CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR:" << nativeVectorWidthChar;

                cl_uint nativeVectorWidthShort = 0;
                devices[device].getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, &nativeVectorWidthShort);
                qDebug() << "            CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT:" << nativeVectorWidthShort;

                cl_uint nativeVectorWidthInt = 0;
                devices[device].getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, &nativeVectorWidthInt);
                qDebug() << "            CL_DEVICE_NATIVE_VECTOR_WIDTH_INT:" << nativeVectorWidthInt;

                cl_uint nativeVectorWidthLong = 0;
                devices[device].getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, &nativeVectorWidthLong);
                qDebug() << "            CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG:" << nativeVectorWidthLong;

                cl_uint nativeVectorWidthFloat = 0;
                devices[device].getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, &nativeVectorWidthFloat);
                qDebug() << "            CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT:" << nativeVectorWidthFloat;

                cl_uint nativeVectorWidthDouble = 0;
                devices[device].getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, &nativeVectorWidthDouble);
                qDebug() << "            CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE:" << nativeVectorWidthDouble;

                cl_uint nativeVectorWidthHalf = 0;
                devices[device].getInfo(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF, &nativeVectorWidthHalf);
                qDebug() << "            CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF:" << nativeVectorWidthHalf;

                cl_uint maxClockFrequency = 0;
                devices[device].getInfo(CL_DEVICE_MAX_CLOCK_FREQUENCY, &maxClockFrequency);
                qDebug() << "            CL_DEVICE_MAX_CLOCK_FREQUENCY:" << maxClockFrequency << "MHz";

                cl_uint addressBits = 0;
                devices[device].getInfo(CL_DEVICE_ADDRESS_BITS, &addressBits);
                qDebug() << "            CL_DEVICE_ADDRESS_BITS:" << addressBits;

                cl_ulong maxMemAllocSize = 0;
                devices[device].getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &maxMemAllocSize);
                qDebug() << "            CL_DEVICE_MAX_MEM_ALLOC_SIZE:" << maxMemAllocSize;

                cl_bool imageSupport = 0;
                devices[device].getInfo(CL_DEVICE_IMAGE_SUPPORT, &imageSupport);
                qDebug() << "            CL_DEVICE_IMAGE_SUPPORT:" << imageSupport;

                cl_uint maxReadImageArgs = 0;
                devices[device].getInfo(CL_DEVICE_MAX_READ_IMAGE_ARGS, &maxReadImageArgs);
                qDebug() << "            CL_DEVICE_MAX_READ_IMAGE_ARGS:" << maxReadImageArgs;

                cl_uint maxWriteImageArgs = 0;
                devices[device].getInfo(CL_DEVICE_MAX_WRITE_IMAGE_ARGS, &maxWriteImageArgs);
                qDebug() << "            CL_DEVICE_MAX_WRITE_IMAGE_ARGS:" << maxWriteImageArgs;

#ifdef CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS
                cl_uint maxReadWriteImageArgs = 0;
                devices[device].getInfo(CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS, &maxReadWriteImageArgs);
                qDebug() << "            CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS:" << maxReadWriteImageArgs;
#endif

                size_t Image2DMaxWidth = 0;
                devices[device].getInfo(CL_DEVICE_IMAGE2D_MAX_WIDTH, &Image2DMaxWidth);
                qDebug() << "            CL_DEVICE_IMAGE2D_MAX_WIDTH:" << Image2DMaxWidth;

                size_t Image2DMaxHeight = 0;
                devices[device].getInfo(CL_DEVICE_IMAGE2D_MAX_HEIGHT, &Image2DMaxHeight);
                qDebug() << "            CL_DEVICE_IMAGE2D_MAX_HEIGHT:" << Image2DMaxHeight;

                size_t Image3DMaxWidth = 0;
                devices[device].getInfo(CL_DEVICE_IMAGE3D_MAX_WIDTH, &Image3DMaxWidth);
                qDebug() << "            CL_DEVICE_IMAGE3D_MAX_WIDTH:" << Image3DMaxWidth;

                size_t Image3DMaxHeight = 0;
                devices[device].getInfo(CL_DEVICE_IMAGE3D_MAX_HEIGHT, &Image3DMaxHeight);
                qDebug() << "            CL_DEVICE_IMAGE3D_MAX_HEIGHT:" << Image3DMaxHeight;

                size_t Image3DMaxDepth = 0;
                devices[device].getInfo(CL_DEVICE_IMAGE3D_MAX_DEPTH, &Image3DMaxDepth);
                qDebug() << "            CL_DEVICE_IMAGE3D_MAX_DEPTH:" << Image3DMaxDepth;

                size_t ImageMaxBufferSize = 0;
                devices[device].getInfo(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, &ImageMaxBufferSize);
                qDebug() << "            CL_DEVICE_IMAGE_MAX_BUFFER_SIZE:" << ImageMaxBufferSize;

                size_t ImageMaxArraySize = 0;
                devices[device].getInfo(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE, &ImageMaxArraySize);
                qDebug() << "            CL_DEVICE_IMAGE_MAX_ARRAY_SIZE:" << ImageMaxArraySize;

                cl_uint maxSamplers = 0;
                devices[device].getInfo(CL_DEVICE_MAX_SAMPLERS, &maxSamplers);
                qDebug() << "            CL_DEVICE_MAX_SAMPLERS:" << maxSamplers;

                cl_uint imagePitchAlignment = 0;
                devices[device].getInfo(CL_DEVICE_IMAGE_PITCH_ALIGNMENT, &imagePitchAlignment);
                qDebug() << "            CL_DEVICE_IMAGE_PITCH_ALIGNMENT:" << imagePitchAlignment;

                cl_uint imageBaseAddressAlignment = 0;
                devices[device].getInfo(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT, &imageBaseAddressAlignment);
                qDebug() << "            CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT:" << imageBaseAddressAlignment;

#ifdef CL_DEVICE_MAX_PIPE_ARGS
                cl_uint maxPipeArgs = 0;
                devices[device].getInfo(CL_DEVICE_MAX_PIPE_ARGS, &maxPipeArgs);
                qDebug() << "            CL_DEVICE_MAX_PIPE_ARGS:" << maxPipeArgs;
#endif

#ifdef CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS
                cl_uint pipeMaxActiveReservations = 0;
                devices[device].getInfo(CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS, &pipeMaxActiveReservations);
                qDebug() << "            CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS:" << pipeMaxActiveReservations;
#endif

#ifdef CL_DEVICE_PIPE_MAX_PACKET_SIZE
                cl_uint pipeMaxPacketSize = 0;
                devices[device].getInfo(CL_DEVICE_PIPE_MAX_PACKET_SIZE, &pipeMaxPacketSize);
                qDebug() << "            CL_DEVICE_PIPE_MAX_PACKET_SIZE:" << pipeMaxPacketSize;
#endif

                size_t maxParameterSize = 0;
                devices[device].getInfo(CL_DEVICE_MAX_PARAMETER_SIZE, &maxParameterSize);
                qDebug() << "            CL_DEVICE_MAX_PARAMETER_SIZE:" << maxParameterSize;

                cl_uint memBaseAddrAlign = 0;
                devices[device].getInfo(CL_DEVICE_MEM_BASE_ADDR_ALIGN, &memBaseAddrAlign);
                qDebug() << "            CL_DEVICE_MEM_BASE_ADDR_ALIGN:" << memBaseAddrAlign;

                cl_device_fp_config simgleFpConfig = 0;
                devices[device].getInfo(CL_DEVICE_SINGLE_FP_CONFIG, &simgleFpConfig);

                QStringList simgleFpConfigList;

                if (simgleFpConfig & CL_FP_DENORM)
                    simgleFpConfigList << "CL_FP_DENORM";

                if (simgleFpConfig & CL_FP_INF_NAN)
                    simgleFpConfigList << "CL_FP_INF_NAN";

                if (simgleFpConfig & CL_FP_ROUND_TO_NEAREST)
                    simgleFpConfigList << "CL_FP_ROUND_TO_NEAREST";

                if (simgleFpConfig & CL_FP_ROUND_TO_ZERO)
                    simgleFpConfigList << "CL_FP_ROUND_TO_ZERO";

                if (simgleFpConfig & CL_FP_ROUND_TO_INF)
                    simgleFpConfigList << "CL_FP_ROUND_TO_INF";

                if (simgleFpConfig & CL_FP_FMA)
                    simgleFpConfigList << "CL_FP_FMA";

                if (simgleFpConfig & CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT)
                    simgleFpConfigList << "CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT";

                if (simgleFpConfig & CL_FP_SOFT_FLOAT)
                    simgleFpConfigList << "CL_FP_SOFT_FLOAT";

                qDebug() << "            CL_DEVICE_SINGLE_FP_CONFIG:" << simgleFpConfigList;

                cl_device_fp_config doubleFpConfig = 0;
                devices[device].getInfo(CL_DEVICE_DOUBLE_FP_CONFIG, &doubleFpConfig);

                QStringList doubleFpConfigList;

                if (doubleFpConfig & CL_FP_DENORM)
                    doubleFpConfigList << "CL_FP_DENORM";

                if (doubleFpConfig & CL_FP_INF_NAN)
                    doubleFpConfigList << "CL_FP_INF_NAN";

                if (doubleFpConfig & CL_FP_ROUND_TO_NEAREST)
                    doubleFpConfigList << "CL_FP_ROUND_TO_NEAREST";

                if (doubleFpConfig & CL_FP_ROUND_TO_ZERO)
                    doubleFpConfigList << "CL_FP_ROUND_TO_ZERO";

                if (doubleFpConfig & CL_FP_ROUND_TO_INF)
                    doubleFpConfigList << "CL_FP_ROUND_TO_INF";

                if (doubleFpConfig & CL_FP_FMA)
                    doubleFpConfigList << "CL_FP_FMA";

                if (doubleFpConfig & CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT)
                    doubleFpConfigList << "CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT";

                if (doubleFpConfig & CL_FP_SOFT_FLOAT)
                    doubleFpConfigList << "CL_FP_SOFT_FLOAT";

                qDebug() << "            CL_DEVICE_DOUBLE_FP_CONFIG:" << doubleFpConfigList;

                cl_device_mem_cache_type globalMemCacheType = 0;
                devices[device].getInfo(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, &globalMemCacheType);

                QMap<cl_device_mem_cache_type, QString> memCacheTypeToStr;
                memCacheTypeToStr[CL_NONE] = "CL_NONE";
                memCacheTypeToStr[CL_READ_ONLY_CACHE] = "CL_READ_ONLY_CACHE";
                memCacheTypeToStr[CL_READ_WRITE_CACHE] = "CL_READ_WRITE_CACHE";

                qDebug() << "            CL_DEVICE_GLOBAL_MEM_CACHE_TYPE:" << memCacheTypeToStr[globalMemCacheType];

                cl_uint globalMemCachelineSize = 0;
                devices[device].getInfo(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, &globalMemCachelineSize);
                qDebug() << "            CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE:" << globalMemCachelineSize;

                cl_ulong globalMemCacheSize = 0;
                devices[device].getInfo(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, &globalMemCacheSize);
                qDebug() << "            CL_DEVICE_GLOBAL_MEM_CACHE_SIZE:" << globalMemCacheSize;

                cl_ulong globalMemSize = 0;
                devices[device].getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &globalMemSize);
                qDebug() << "            CL_DEVICE_GLOBAL_MEM_SIZE:" << globalMemSize;

                cl_ulong maxConstantBufferSize = 0;
                devices[device].getInfo(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, &maxConstantBufferSize);
                qDebug() << "            CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:" << maxConstantBufferSize;

                cl_uint maxConstantArgs = 0;
                devices[device].getInfo(CL_DEVICE_MAX_CONSTANT_ARGS, &maxConstantArgs);
                qDebug() << "            CL_DEVICE_MAX_CONSTANT_ARGS:" << maxConstantArgs;

#ifdef CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE
                size_t maxGlobalVariableSize = 0;
                devices[device].getInfo(CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE, &maxGlobalVariableSize);
                qDebug() << "            CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE:" << maxGlobalVariableSize;
#endif

#ifdef CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE
                size_t globalVariablePreferredTotalSize = 0;
                devices[device].getInfo(CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE, &globalVariablePreferredTotalSize);
                qDebug() << "            CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE:" << globalVariablePreferredTotalSize;
#endif

                cl_device_mem_cache_type localMemType = 0;
                devices[device].getInfo(CL_DEVICE_LOCAL_MEM_TYPE, &localMemType);

                QMap<cl_device_local_mem_type, QString> localMemTypeToStr;
                localMemTypeToStr[CL_NONE] = "CL_NONE";
                localMemTypeToStr[CL_LOCAL] = "CL_LOCAL";
                localMemTypeToStr[CL_GLOBAL] = "CL_GLOBAL";

                qDebug() << "            CL_DEVICE_LOCAL_MEM_TYPE:" << localMemTypeToStr[localMemType];

                cl_ulong localMemSize = 0;
                devices[device].getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &localMemSize);
                qDebug() << "            CL_DEVICE_LOCAL_MEM_SIZE:" << localMemSize;

                cl_bool errorCorrectionSupport = false;
                devices[device].getInfo(CL_DEVICE_ERROR_CORRECTION_SUPPORT, &errorCorrectionSupport);
                qDebug() << "            CL_DEVICE_ERROR_CORRECTION_SUPPORT:" << errorCorrectionSupport;

                size_t profilingTimerResolution = 0;
                devices[device].getInfo(CL_DEVICE_PROFILING_TIMER_RESOLUTION, &profilingTimerResolution);
                qDebug() << "            CL_DEVICE_PROFILING_TIMER_RESOLUTION:" << profilingTimerResolution;

                cl_bool endianLittle = false;
                devices[device].getInfo(CL_DEVICE_ENDIAN_LITTLE, &endianLittle);
                qDebug() << "            CL_DEVICE_ENDIAN_LITTLE:" << endianLittle;

                cl_bool available = false;
                devices[device].getInfo(CL_DEVICE_AVAILABLE, &available);
                qDebug() << "            CL_DEVICE_AVAILABLE:" << available;

                cl_bool compilerAvailable = false;
                devices[device].getInfo(CL_DEVICE_COMPILER_AVAILABLE, &compilerAvailable);
                qDebug() << "            CL_DEVICE_COMPILER_AVAILABLE:" << compilerAvailable;

                cl_bool linkerAvailable = false;
                devices[device].getInfo(CL_DEVICE_LINKER_AVAILABLE, &linkerAvailable);
                qDebug() << "            CL_DEVICE_LINKER_AVAILABLE:" << linkerAvailable;

                cl_device_exec_capabilities executionCapabilities = 0;
                devices[device].getInfo(CL_DEVICE_EXECUTION_CAPABILITIES, &executionCapabilities);

                QStringList executionCapabilitiesList;

                if (executionCapabilities & CL_EXEC_KERNEL)
                    executionCapabilitiesList << "CL_EXEC_KERNEL";

                if (executionCapabilities & CL_EXEC_NATIVE_KERNEL)
                    executionCapabilitiesList << "CL_EXEC_NATIVE_KERNEL";

                qDebug() << "            CL_DEVICE_EXECUTION_CAPABILITIES:" << executionCapabilitiesList;

#ifdef CL_DEVICE_QUEUE_ON_HOST_PROPERTIES
                cl_command_queue_properties queueOnHostProperties = 0;
                devices[device].getInfo(CL_DEVICE_QUEUE_ON_HOST_PROPERTIES, &queueOnHostProperties);

                QStringList queueOnHostPropertiesList;

                if (queueOnHostProperties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)
                    queueOnHostPropertiesList << "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE";

                if (queueOnHostProperties & CL_QUEUE_PROFILING_ENABLE)
                    queueOnHostPropertiesList << "CL_QUEUE_PROFILING_ENABLE";

                qDebug() << "            CL_DEVICE_QUEUE_ON_HOST_PROPERTIES:" << queueOnHostPropertiesList;
#endif

#ifdef CL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES
                cl_command_queue_properties queueOnDeviceProperties = 0;
                devices[device].getInfo(CL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES, &queueOnDeviceProperties);

                QStringList queueOnDevicePropertiesList;

                if (queueOnDeviceProperties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)
                    queueOnDevicePropertiesList << "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE";

                if (queueOnDeviceProperties & CL_QUEUE_PROFILING_ENABLE)
                    queueOnDevicePropertiesList << "CL_QUEUE_PROFILING_ENABLE";

                qDebug() << "            CL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES:" << queueOnDevicePropertiesList;
#endif

#ifdef CL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE
                cl_uint queueOnDevicePreferredSize = 0;
                devices[device].getInfo(CL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE, &queueOnDevicePreferredSize);
                qDebug() << "            CL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE:" << queueOnDevicePreferredSize;
#endif

#ifdef CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE
                cl_uint queueOnDeviceMaxSize = 0;
                devices[device].getInfo(CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE, &queueOnDeviceMaxSize);
                qDebug() << "            CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE:" << queueOnDeviceMaxSize;
#endif

#ifdef CL_DEVICE_MAX_ON_DEVICE_QUEUES
                cl_uint maxOnDeviceQueues = 0;
                devices[device].getInfo(CL_DEVICE_MAX_ON_DEVICE_QUEUES, &maxOnDeviceQueues);
                qDebug() << "            CL_DEVICE_MAX_ON_DEVICE_QUEUES:" << maxOnDeviceQueues;
#endif

#ifdef CL_DEVICE_MAX_ON_DEVICE_EVENTS
                cl_uint maxOnDeviceEvents = 0;
                devices[device].getInfo(CL_DEVICE_MAX_ON_DEVICE_EVENTS, &maxOnDeviceEvents);
                qDebug() << "            CL_DEVICE_MAX_ON_DEVICE_EVENTS:" << maxOnDeviceEvents;
#endif

                cl::STRING_CLASS builtInKernels;
                platform.getInfo(CL_DEVICE_BUILT_IN_KERNELS, &builtInKernels);
                qDebug() << "            CL_DEVICE_BUILT_IN_KERNELS:" << builtInKernels.c_str();

                cl::STRING_CLASS deviceName;
                platform.getInfo(CL_DEVICE_NAME, &deviceName);
                qDebug() << "            CL_DEVICE_NAME:" << deviceName.c_str();

                cl::STRING_CLASS deviceVendor;
                platform.getInfo(CL_DEVICE_VENDOR, &deviceVendor);
                qDebug() << "            CL_DEVICE_VENDOR:" << deviceVendor.c_str();

                cl::STRING_CLASS driverVersion;
                platform.getInfo(CL_DRIVER_VERSION, &driverVersion);
                qDebug() << "            CL_DRIVER_VERSION:" << driverVersion.c_str();

                cl::STRING_CLASS deviceProfile;
                platform.getInfo(CL_DEVICE_PROFILE, &deviceProfile);
                qDebug() << "            CL_DEVICE_PROFILE:" << deviceProfile.c_str();

                cl::STRING_CLASS deviceVersion;
                platform.getInfo(CL_DEVICE_VERSION, &deviceVersion);
                qDebug() << "            CL_DEVICE_VERSION:" << deviceVersion.c_str();

                cl::STRING_CLASS openCLVersion;
                platform.getInfo(CL_DEVICE_OPENCL_C_VERSION, &openCLVersion);
                qDebug() << "            CL_DEVICE_OPENCL_C_VERSION:" << openCLVersion.c_str();

                cl::STRING_CLASS deviceExtensions;
                platform.getInfo(CL_DEVICE_EXTENSIONS, &deviceExtensions);
                qDebug() << "            CL_DEVICE_EXTENSIONS:" << deviceExtensions.c_str();

                size_t PrintfBufferSize = 0;
                devices[device].getInfo(CL_DEVICE_PRINTF_BUFFER_SIZE, &PrintfBufferSize);
                qDebug() << "            CL_DEVICE_PRINTF_BUFFER_SIZE:" << PrintfBufferSize;

                cl_bool preferredInteropUserSync = false;
                devices[device].getInfo(CL_DEVICE_PREFERRED_INTEROP_USER_SYNC, &preferredInteropUserSync);
                qDebug() << "            CL_DEVICE_PREFERRED_INTEROP_USER_SYNC:" << preferredInteropUserSync;

                cl_uint partitionMaxSubDevices = 0;
                devices[device].getInfo(CL_DEVICE_PARTITION_MAX_SUB_DEVICES, &partitionMaxSubDevices);
                qDebug() << "            CL_DEVICE_PARTITION_MAX_SUB_DEVICES:" << partitionMaxSubDevices;

                VECTOR_CLASS<cl_device_partition_property> partitionProperties;
                devices[device].getInfo(CL_DEVICE_PARTITION_PROPERTIES, &partitionProperties);

                QMap<cl_device_partition_property, QString> partitionPropertyToStr;
                partitionPropertyToStr[CL_DEVICE_PARTITION_EQUALLY] = "CL_DEVICE_PARTITION_EQUALLY";
                partitionPropertyToStr[CL_DEVICE_PARTITION_BY_COUNTS] = "CL_DEVICE_PARTITION_BY_COUNTS";
                partitionPropertyToStr[CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN] = "CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN";

                QStringList partitionPropertiesList;

                foreach (cl_device_partition_property partitionProperty, partitionProperties)
                    partitionPropertiesList << partitionPropertyToStr[partitionProperty];

                qDebug() << "            CL_DEVICE_PARTITION_PROPERTIES:" << partitionPropertiesList;

                cl_device_affinity_domain PartitionAffinityDomain = 0;
                devices[device].getInfo(CL_DEVICE_PARTITION_AFFINITY_DOMAIN, &PartitionAffinityDomain);

                QStringList PartitionAffinityDomainList;

                if (PartitionAffinityDomain & CL_DEVICE_AFFINITY_DOMAIN_NUMA)
                    PartitionAffinityDomainList << "CL_DEVICE_AFFINITY_DOMAIN_NUMA";

                if (PartitionAffinityDomain & CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE)
                    PartitionAffinityDomainList << "CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE";

                if (PartitionAffinityDomain & CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE)
                    PartitionAffinityDomainList << "CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE";

                if (PartitionAffinityDomain & CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE)
                    PartitionAffinityDomainList << "CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE";

                if (PartitionAffinityDomain & CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE)
                    PartitionAffinityDomainList << "CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE";

                if (PartitionAffinityDomain & CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE)
                    PartitionAffinityDomainList << "CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE";

                qDebug() << "            CL_DEVICE_PARTITION_AFFINITY_DOMAIN:" << PartitionAffinityDomainList;

                VECTOR_CLASS<cl_device_partition_property> partitionType;
                devices[device].getInfo(CL_DEVICE_PARTITION_TYPE, &partitionType);

                QMap<cl_device_partition_property, QString> partitionTypeToStr;
                partitionTypeToStr[CL_DEVICE_AFFINITY_DOMAIN_NUMA] = "CL_DEVICE_AFFINITY_DOMAIN_NUMA";
                partitionTypeToStr[CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE] = "CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE";
                partitionTypeToStr[CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE] = "CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE";
                partitionTypeToStr[CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE] = "CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE";
                partitionTypeToStr[CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE] = "CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE";

                QStringList partitionTypeList;

                foreach (cl_device_partition_property partitionProperty, partitionType)
                    partitionTypeList << partitionTypeToStr[partitionProperty];

                qDebug() << "            CL_DEVICE_PARTITION_TYPE:" << partitionTypeList;

                cl_uint referenceCount = 0;
                devices[device].getInfo(CL_DEVICE_REFERENCE_COUNT, &referenceCount);
                qDebug() << "            CL_DEVICE_REFERENCE_COUNT:" << referenceCount;

#ifdef cl_device_svm_capabilities
                cl_device_svm_capabilities svmCapabilities = 0;
                devices[device].getInfo(CL_DEVICE_SVM_CAPABILITIES, &svmCapabilities);

                QStringList svmCapabilitiesList;

                if (svmCapabilities & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER)
                    svmCapabilitiesList << "CL_DEVICE_SVM_COARSE_GRAIN_BUFFER";

                if (svmCapabilities & CL_DEVICE_SVM_FINE_GRAIN_BUFFER)
                    svmCapabilitiesList << "CL_DEVICE_SVM_FINE_GRAIN_BUFFER";

                if (svmCapabilities & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM)
                    svmCapabilitiesList << "CL_DEVICE_SVM_FINE_GRAIN_SYSTEM";

                if (svmCapabilities & CL_DEVICE_SVM_ATOMICS)
                    svmCapabilitiesList << "CL_DEVICE_SVM_ATOMICS";

                qDebug() << "            CL_DEVICE_SVM_CAPABILITIES:" << svmCapabilitiesList;
#endif

#ifdef CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT
                cl_uint preferredPlatformAtomicAlignment = 0;
                devices[device].getInfo(CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT, &preferredPlatformAtomicAlignment);
                qDebug() << "            CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT:" << preferredPlatformAtomicAlignment;
#endif

#ifdef CL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT
                cl_uint preferredGlobalAtomicAlignment = 0;
                devices[device].getInfo(CL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT, &preferredGlobalAtomicAlignment);
                qDebug() << "            CL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT:" << preferredGlobalAtomicAlignment;
#endif

#ifdef CL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT
                cl_uint preferredLocalAtomicAlignment = 0;
                devices[device].getInfo(CL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT, &preferredLocalAtomicAlignment);
                qDebug() << "            CL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT:" << preferredLocalAtomicAlignment;
#endif

                qDebug() << "";
                qDebug() << "            Supported image formats:";

                QMap<cl_mem_object_type, QString> objectTypes;
                objectTypes[CL_MEM_OBJECT_IMAGE2D] = "CL_MEM_OBJECT_IMAGE2D";
                objectTypes[CL_MEM_OBJECT_IMAGE3D] = "CL_MEM_OBJECT_IMAGE3D";

                cl::Context context(devices[device]);

                QMap<cl_channel_order, QString> channelOrderToStr;
                channelOrderToStr[CL_R] = "CL_R";
                channelOrderToStr[CL_A] = "CL_A";
                channelOrderToStr[CL_RG] = "CL_RG";
                channelOrderToStr[CL_RA] = "CL_RA";
                channelOrderToStr[CL_RGB] = "CL_RGB";
                channelOrderToStr[CL_RGBA] = "CL_RGBA";
                channelOrderToStr[CL_BGRA] = "CL_BGRA";
                channelOrderToStr[CL_ARGB] = "CL_ARGB";
                channelOrderToStr[CL_INTENSITY] = "CL_INTENSITY";
                channelOrderToStr[CL_LUMINANCE] = "CL_LUMINANCE";
                channelOrderToStr[CL_Rx] = "CL_Rx";
                channelOrderToStr[CL_RGx] = "CL_RGx";
                channelOrderToStr[CL_RGBx] = "CL_RGBx";
                channelOrderToStr[CL_DEPTH] = "CL_DEPTH";
                channelOrderToStr[CL_DEPTH_STENCIL] = "CL_DEPTH_STENCIL";

                QMap<cl_channel_type, QString> channelTypeToStr;
                channelTypeToStr[CL_SNORM_INT8] = "CL_SNORM_INT8";
                channelTypeToStr[CL_SNORM_INT16] = "CL_SNORM_INT16";
                channelTypeToStr[CL_UNORM_INT8] = "CL_UNORM_INT8";
                channelTypeToStr[CL_UNORM_INT16] = "CL_UNORM_INT16";
                channelTypeToStr[CL_UNORM_SHORT_565] = "CL_UNORM_SHORT_565";
                channelTypeToStr[CL_UNORM_SHORT_555] = "CL_UNORM_SHORT_555";
                channelTypeToStr[CL_UNORM_INT_101010] = "CL_UNORM_INT_101010";
                channelTypeToStr[CL_SIGNED_INT8] = "CL_SIGNED_INT8";
                channelTypeToStr[CL_SIGNED_INT16] = "CL_SIGNED_INT16";
                channelTypeToStr[CL_SIGNED_INT32] = "CL_SIGNED_INT32";
                channelTypeToStr[CL_UNSIGNED_INT8] = "CL_UNSIGNED_INT8";
                channelTypeToStr[CL_UNSIGNED_INT16] = "CL_UNSIGNED_INT16";
                channelTypeToStr[CL_UNSIGNED_INT32] = "CL_UNSIGNED_INT32";
                channelTypeToStr[CL_HALF_FLOAT] = "CL_HALF_FLOAT";
                channelTypeToStr[CL_FLOAT] = "CL_FLOAT";
                channelTypeToStr[CL_UNORM_INT24] = "CL_UNORM_INT24";

                foreach (cl_mem_object_type objectType, objectTypes.keys()) {
                    qDebug() << QString("                %1:").arg(objectTypes[objectType]).toStdString().c_str();
                    VECTOR_CLASS<cl::ImageFormat> supportedImageFormats;

                    context.getSupportedImageFormats(CL_MEM_READ_WRITE,  objectType, &supportedImageFormats);

                    foreach (cl::ImageFormat imageFormat, supportedImageFormats)
                        qDebug() << "                    "
                                 << channelOrderToStr[imageFormat.image_channel_order]
                                 << channelTypeToStr[imageFormat.image_channel_data_type];
                }
            }
        }

        qDebug() << "";

        i++;
    }

    return 0;
}
