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

#pragma once
#include "Cairo.hpp"
#include <memory>

struct cairo_surface_deleter;
typedef std::unique_ptr<_cairo_surface, cairo_surface_deleter> cairo_surface_u;

cairo_surface_t *
cairo_image_surface_create_from_png_data(const unsigned char *data, unsigned length);

cairo_surface_t *
cairo_image_surface_create_from_png_resource(unsigned int id);

void cairo_set_source_rgba32(cairo_t *cr, uint32_t color);

#include "CairoExtra.tcc"

