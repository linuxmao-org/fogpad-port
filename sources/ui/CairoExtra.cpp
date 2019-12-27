/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Jean Pierre Cimalando
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "CairoExtra.h"
#include "resource.h"
#include <unordered_map>

cairo_surface_t *
cairo_image_surface_create_from_png_data(const unsigned char *data, unsigned length)
{
    struct PngReaderData
    {
        const unsigned char *dataPtr;
        unsigned sizeLeft;
    };

    auto readSomePngData = [](void *closure,
                              unsigned char *data,
                              unsigned int length) noexcept -> cairo_status_t
                           {
                               PngReaderData &readerData = *reinterpret_cast<PngReaderData *>(closure);
                               if (readerData.sizeLeft < length)
                                   return CAIRO_STATUS_READ_ERROR;

                               memcpy(data, readerData.dataPtr, length);
                               readerData.dataPtr += length;
                               readerData.sizeLeft -= length;
                               return CAIRO_STATUS_SUCCESS;
                           };

    PngReaderData readerData;
    readerData.dataPtr = data;
    readerData.sizeLeft = length;
    return cairo_image_surface_create_from_png_stream(+readSomePngData, &readerData);
}

cairo_surface_t *
cairo_image_surface_create_from_png_resource(unsigned int id)
{
    const Resource *res = GetResource(id);

    if (!res || res->type != 'png ')
        return nullptr;

    return cairo_image_surface_create_from_png_data(res->data, res->size);
}

void
cairo_set_source_rgba32(cairo_t *cr, uint32_t color)
{
    double k = 1.0 / 255;
    double r = k * ((color >> 24) & 0xff);
    double g = k * ((color >> 16) & 0xff);
    double b = k * ((color >> 8) & 0xff);
    double a = k * ((color >> 0) & 0xff);
    cairo_set_source_rgba(cr, r, g, b, a);
}
