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

__const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE
                            | CLK_ADDRESS_CLAMP_TO_EDGE
                            | CLK_FILTER_NEAREST;

__kernel void edges(__read_only image2d_t imageIn,
                    __write_only image2d_t imageOut)
{
    size_t x = get_global_id(0);
    size_t y = get_global_id(1);

    if (x >= get_image_width(imageIn)
        || y >= get_image_height(imageIn))
        return;

    uint4 gx = read_imageui(imageIn, sampler, (int2)(x + 1, y - 1))
              + 2 * read_imageui(imageIn, sampler, (int2)(x + 1, y))
              + read_imageui(imageIn, sampler, (int2)(x + 1, y + 1))
              - read_imageui(imageIn, sampler, (int2)(x - 1, y - 1))
              - 2 * read_imageui(imageIn, sampler, (int2)(x - 1, y))
              - read_imageui(imageIn, sampler, (int2)(x - 1, y + 1));

    uint4 gy = read_imageui(imageIn, sampler, (int2)(x - 1, y + 1))
              + 2 * read_imageui(imageIn, sampler, (int2)(x, y + 1))
              + read_imageui(imageIn, sampler, (int2)(x + 1, y + 1))
              - read_imageui(imageIn, sampler, (int2)(x - 1, y - 1))
              - 2 * read_imageui(imageIn, sampler, (int2)(x, y - 1))
              - read_imageui(imageIn, sampler, (int2)(x + 1, y - 1));

    uint4 pixel = convert_uint4(sqrt(convert_float4(gx * gx + gy * gy)));
    pixel = clamp(pixel, (uint4)(0), (uint4)(255));
//    printf("r = %d, g = %d, b = %d, a = %d\n", pixel.s0, pixel.s1, pixel.s2, pixel.s3);
    pixel.s3 = 255;
    write_imageui(imageOut, (int2)(x, y), pixel);
}
