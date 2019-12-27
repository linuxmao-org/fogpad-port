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
#include "Control.h"

class Knob final : public CControl
{
public:
    explicit Knob(Widget *group);

    void setValue(double value, NotifyMode notify = kNotify) override;

    void setValueBounds(double v1, double v2);
    void setLogarithmic(bool log);
    void setNumSteps(unsigned numSteps);

    bool onMouse(const MouseEvent &event) override;
    bool onMotion(const MotionEvent &event) override;
    bool onScroll(const ScrollEvent &event) override;
    void onDisplay() override;

private:
    double clampToBounds(double value);

    double valueForRatio(double ratio) const;
    double ratioForValue(double value) const;

private:
    double fValueBound1 = 0, fValueBound2 = 1;
    bool fIsLogarithmic = false;
    unsigned fNumSteps = 100;
    double fAngleMin = -2.3561945;
    double fAngleMax = +2.3561945;
    bool fIsDragging = false;
};
