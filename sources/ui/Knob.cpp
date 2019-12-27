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

#include "Knob.h"
#include "CairoExtra.h"
#include "Cairo.hpp"
#include "Window.hpp"
#include <complex>

typedef std::complex<double> cdouble;

///
Knob::Knob(Widget *group)
    : CControl(group)
{
    setSize(30, 30);
}

void Knob::setValue(double value, NotifyMode notify)
{
    CControl::setValue(clampToBounds(value), notify);
}

void Knob::setValueBounds(double v1, double v2)
{
    fValueBound1 = v1;
    fValueBound2 = v2;
    setValue(getValue());
}

void Knob::setLogarithmic(bool log)
{
    if (fIsLogarithmic == log)
        return;

    fIsLogarithmic = log;
    repaint();
}

void Knob::setNumSteps(unsigned numSteps)
{
    fNumSteps = numSteps;
}

bool Knob::onMouse(const MouseEvent &event)
{
    DGL::Size<uint> wsize = getSize();
    DGL::Point<int> mpos = event.pos;

    if (!fIsDragging && event.press && event.button == 1)
    {
        bool insideX = mpos.getX() >= 0 && (unsigned)mpos.getX() < wsize.getWidth();
        bool insideY = mpos.getY() >= 0 && (unsigned)mpos.getY() < wsize.getHeight();

        if (!insideX || !insideY)
            return false;

        fIsDragging = true;
        beginChangeGesture();

        return true;
    }
    else if (fIsDragging && !event.press && event.button == 1)
    {
        fIsDragging = false;
        endChangeGesture();
        return true;
    }

    return false;
}

bool Knob::onMotion(const MotionEvent &event)
{
    DGL::Size<uint> wsize = getSize();
    DGL::Point<int> mpos = event.pos;

    if (fIsDragging) {
        double dx = mpos.getX() - 0.5 * getWidth();
        double dy = mpos.getY() - 0.5 * getHeight();
        if (dx * dx + dy * dy > 100.0) {
            double angle = std::atan2(dx, -dy);
            angle = std::max(angle, fAngleMin);
            angle = std::min(angle, fAngleMax);
            double fill = (angle - fAngleMin) / (fAngleMax - fAngleMin);
            setValue(valueForRatio(fill));
        }
        return true;
    }

    return false;
}

bool Knob::onScroll(const ScrollEvent &event)
{
    DGL::Size<uint> wsize = getSize();
    DGL::Point<int> mpos = event.pos;

    bool inside = mpos.getX() >= 0 && mpos.getY() >= 0 && (unsigned)mpos.getX() < wsize.getWidth() &&
                  (unsigned)mpos.getY() < wsize.getHeight();

    if (inside) {
        double amount = event.delta.getY() - event.delta.getX();

        double fill = ratioForValue(getValue()) + amount / fNumSteps;
        if (fill < 0) fill = 0;
        if (fill > 1) fill = 1;

        setValue(valueForRatio(fill));

        return true;
    }

    return false;
}

void Knob::onDisplay()
{
    cairo_t *cr = getParentWindow().getGraphicsContext().cairo;
    int w = getWidth();
    int h = getHeight();

    double fill = ratioForValue(getValue());

    cairo_save(cr);

    double xc = 0.5 * w;
    double yc = 0.5 * h;
    double rad = 0.9 * ((xc < yc) ? xc : yc);
    double trackw = 6.0;
    double btnrad = 6.0;

    double a1 = fAngleMin - M_PI / 2.0;
    double a2 = fAngleMax - M_PI / 2.0;
    double a = a1 + fill * (a2 - a1);

    cairo_set_line_width(cr, trackw);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

    cairo_new_path(cr);
    cairo_arc(cr, xc, yc, rad, a1, a2);
    cairo_set_source_rgba32(cr, 0x8a8a8aff);
    cairo_stroke(cr);

    cairo_new_path(cr);
    cairo_arc(cr, xc, yc, rad, a1, a);
    cairo_set_source_rgba32(cr, 0xffffffff);
    cairo_stroke(cr);

    cdouble btncoord = std::polar(rad, a) + cdouble{xc, yc};
    double btnx = btncoord.real();
    double btny = btncoord.imag();

    cairo_set_line_width(cr, 1.0);

    cairo_new_path(cr);
    cairo_arc(cr, btnx, btny, btnrad, 0.0, 2.0 * M_PI);
    cairo_set_source_rgba32(cr, 0xffffffff);
    cairo_fill_preserve(cr);
    cairo_set_source_rgba32(cr, 0x8a8a8aff);
    cairo_stroke(cr);

    cairo_restore(cr);
}

double Knob::clampToBounds(double value)
{
    double vmin = fValueBound1;
    double vmax = fValueBound2;
    if (vmin > vmax)
        std::swap(vmin, vmax);

    value = (value < vmin) ? vmin : value;
    value = (value > vmax) ? vmax : value;
    return value;
}

double Knob::valueForRatio(double ratio) const
{
    double v1 = fValueBound1;
    double v2 = fValueBound2;
    if (!fIsLogarithmic)
        return v1 + ratio * (v2 - v1);
    else
        return v1 * pow(v2 / v1, ratio);
}

double Knob::ratioForValue(double value) const
{
    double v1 = fValueBound1;
    double v2 = fValueBound2;
    if (!fIsLogarithmic)
        return (value - v1) / (v2 - v1);
    else
        return log(value / v1) / log(v2 / v1);
}
