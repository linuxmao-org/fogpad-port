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

#include "BitmapCache.h"
#include <unordered_map>
#include <mutex>
#include <assert.h>

typedef std::unordered_map<unsigned, cairo_surface_u> tBitmapCache;
static std::mutex bitmapCacheMutex;
static tBitmapCache bitmapCache;

cairo_surface_t *BitmapCache::load(unsigned id)
{
    std::lock_guard<std::mutex> lock(bitmapCacheMutex);

    tBitmapCache::iterator it = bitmapCache.find(id);
    if (it != bitmapCache.end())
        return it->second.get();

    cairo_surface_u image{cairo_image_surface_create_from_png_resource(id)};
    assert(image);

    cairo_surface_t *ret = image.get();
    bitmapCache[id] = std::move(image);
    return ret;
}

void BitmapCache::clear()
{
    std::lock_guard<std::mutex> lock(bitmapCacheMutex);

    bitmapCache.clear();
}
