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


#include "resource.h"
#include <algorithm>

extern const Resource RES[];
extern const unsigned int NRES;

static bool operator<(const Resource &a, const Resource &b)
{
	return a.id < b.id;
}

const Resource *GetResource(unsigned int id)
{
	/* binary search into a sorted array */
	Resource ref;
	ref.id = id;
	const Resource *begin = RES;
	const Resource *end = begin + NRES;
	const Resource *res = std::lower_bound(begin, end, ref);
	return (res != end && res->id == id) ? res : nullptr;
}
