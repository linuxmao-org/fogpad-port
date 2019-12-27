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

#include "Control.h"

void CControl::setValue(double value, NotifyMode notify)
{
    if (fValue == value)
        return;

    fValue = value;

    if (notify == kNotify)
    {
        for (CControlListener *cl : fListeners)
            cl->controlValueChanged(*this);
    }

    repaint();
}

void CControl::addListener(CControlListener *cl)
{
    DISTRHO_SAFE_ASSERT_RETURN(cl != nullptr, );

    fListeners.push_back(cl);
}

void CControl::removeListener(CControlListener *cl)
{
    auto it = std::find(fListeners.begin(), fListeners.end(), cl);

    DISTRHO_SAFE_ASSERT_RETURN(it != fListeners.end(), );

    fListeners.erase(it);
}

void CControl::beginChangeGesture()
{
    for (CControlListener *cl : fListeners)
        cl->controlBeganChangeGesture(*this);
}

void CControl::endChangeGesture()
{
    for (CControlListener *cl : fListeners)
        cl->controlEndedChangeGesture(*this);
}
