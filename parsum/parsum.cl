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

__kernel void parsum(__global int *inputBuffer,
                     __global int *outputBuffer,
                     int size)
{
    size_t globalId = get_global_id(0);
    size_t localId = get_local_id(0);

    // Create temporal buffers.
    __local int buffer[WORKGROUPSIZE];

    // Initialize buffers.
    buffer[localId] = inputBuffer[globalId];
    barrier(CLK_LOCAL_MEM_FENCE);

    for (int offset = get_local_size(0) >> 1; offset > 0; offset >>= 1) {
        if (localId < offset
            && globalId + offset < size)
            buffer[localId] += buffer[localId + offset];

        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Write result to the output buffer.
    if (localId == 0)
        outputBuffer[get_group_id(0)] = buffer[0];
}
