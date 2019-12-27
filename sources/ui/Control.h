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
#include "Widget.hpp"
#include <vector>
#include <cstdint>

class CControlListener;

class CControl : public Widget
{
public:
    CControl(Widget *group) : Widget(group) {}
    virtual ~CControl() {}

    intptr_t getTag() const noexcept { return fTag; }
    void setTag(intptr_t tag) noexcept { fTag = tag; }

    enum NotifyMode
    {
        kNotify,
        kDoNotNotify,
    };

    double getValue() const { return fValue; }
    virtual void setValue(double value, NotifyMode notify = kNotify);

    void addListener(CControlListener *cl);
    void removeListener(CControlListener *cl);

protected:
    void beginChangeGesture();
    void endChangeGesture();

private:
    double fValue = 0;
    intptr_t fTag = 0;
    std::vector<CControlListener *> fListeners;
};

class CControlListener
{
public:
    virtual ~CControlListener() {}

    virtual void controlValueChanged(CControl &) {};
    virtual void controlBeganChangeGesture(CControl &) {};
    virtual void controlEndedChangeGesture(CControl &) {};
};
