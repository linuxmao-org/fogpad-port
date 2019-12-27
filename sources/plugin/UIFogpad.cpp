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

#include "UIFogpad.hpp"
#include "SharedFogpad.hpp"
#include "Window.hpp"
#include "paramids.h"
#include "ui/BitmapCache.h"
#include "ui/CheckBox.h"
#include "ui/Knob.h"

namespace Igorski {

// -----------------------------------------------------------------------
// Init / Deinit

UIFogpad::UIFogpad()
    : UI(930, 530)
{
    fKnobById = new Knob *[kNumParameters]{};
    fCheckBoxById = new CheckBox *[kNumParameters]{};

    fParameterRanges = new ParameterRangesSimple[kNumParameters];

    for (unsigned i = 0; i < kNumParameters; ++i) {
        Parameter param;
        SharedFogpad::InitParameter(i, param);
        fParameterRanges[i] = ParameterRangesSimple{param.ranges.def, param.ranges.min, param.ranges.max};
    }

    // MIST
    createKnob(kReverbSizeId, 20, 105, 60, 60);
    createKnob(kReverbWidthId, 110, 105, 60, 60);
    createCheckBox(kReverbFreezeId, 224, 138, 11, 11);

    // HUE
    createKnob(kFilterCutoffId, 303, 105, 60, 60);
    //createKnob(kFilterResonanceId, 393, 105, 60, 60);
    createKnob(kLFOFilterId, 393, 105, 60, 60);
    createKnob(kLFOFilterDepthId, 483, 105, 60, 60);

    // MURK
    createKnob(kReverbPlaybackRateId, 630, 105, 60, 60);
    createKnob(kDecimatorId, 720, 105, 60, 60);

    // BOTHER
    createKnob(kBitResolutionId, 20, 360, 60, 60, kControlInverted);
    createKnob(kLFOBitResolutionId, 110, 360, 60, 60);
    createKnob(kLFOBitResolutionDepthId, 200, 360, 60, 60);
    createCheckBox(kBitResolutionChainId, 380, 300, 11, 11);

    // COMMINGLE
    createKnob(kReverbDryMixId, 455, 360, 60, 60);
    createKnob(kReverbWetMixId, 545, 360, 60, 60);

    for (unsigned i = 0; i < kNumParameters; ++i) {
        ParameterRangesSimple range = fParameterRanges[i];
        setParameterValue(i, range.def);
    }
}

UIFogpad::~UIFogpad() {
    while (!fSubwidgets.empty()) {
        delete fSubwidgets.back();
        fSubwidgets.pop_back();
    }

    delete[] fKnobById;
    delete[] fCheckBoxById;

    delete[] fParameterRanges;
}

// -----------------------------------------------------------------------
// DSP/Plugin callbacks

/**
  A parameter has changed on the plugin side.
  This is called by the host to inform the UI about parameter changes.
*/
void UIFogpad::parameterChanged(uint32_t index, float value) {
    DISTRHO_SAFE_ASSERT_RETURN(index < kNumParameters, );

    if (Knob *ctl = fKnobById[index]) {
        ctl->setValue(value, CControl::kDoNotNotify);
    }
    if (CheckBox *ctl = fCheckBoxById[index]) {
        ctl->setValue(value, CControl::kDoNotNotify);
    }
}

/**
  Optional callback to inform the UI about a sample rate change on the plugin side.
*/
void UIFogpad::sampleRateChanged(double newSampleRate) {
    (void)newSampleRate;
}

// -----------------------------------------------------------------------
// Widget callbacks


/**
  A function called to draw the view contents.
*/
void UIFogpad::onDisplay() {

    cairo_t* cr = getParentWindow().getGraphicsContext().cairo;

    cairo_set_line_width(cr, 1.0);

    cairo_surface_t *bg = BitmapCache::load(150);
    cairo_set_source_surface(cr, bg, 0, 0);
    cairo_paint(cr);
}


// -----------------------------------------------------------------------

void UIFogpad::createKnob(int id, int x, int y, int w, int h, int flags)
{
    Knob *ctl = new Knob(this);
    fSubwidgets.push_back(ctl);

    ctl->setTag(id);
    fKnobById[id] = ctl;

    ParameterRangesSimple range = fParameterRanges[id];
    if ((flags & kControlLogarithmic) != 0)
        ctl->setLogarithmic(true);
    if ((flags & kControlInverted) == 0)
        ctl->setValueBounds(range.min, range.max);
    else
        ctl->setValueBounds(range.max, range.min);

    ctl->addListener(this);
    ctl->setAbsolutePos(x, y);
    ctl->setSize(w, h);
}

void UIFogpad::createCheckBox(int id, int x, int y, int w, int h, int flags)
{
    CheckBox *ctl = new CheckBox(this);
    fSubwidgets.push_back(ctl);

    ctl->setTag(id);
    fCheckBoxById[id] = ctl;

    (void)flags;

    ParameterRangesSimple range = fParameterRanges[id];
    ctl->setValueBounds(range.min, range.max);

    ctl->addListener(this);
    ctl->setAbsolutePos(x, y);
    ctl->setSize(w, h);
}

// -----------------------------------------------------------------------

void UIFogpad::controlValueChanged(CControl &ctl)
{
    int id = ctl.getTag();
    if (id >= 0 && id < kNumParameters)
        setParameterValue(id, ctl.getValue());
}

void UIFogpad::controlBeganChangeGesture(CControl &ctl)
{
    int id = ctl.getTag();
    if (id >= 0 && id < kNumParameters)
        this->editParameter(id, true);
}

void UIFogpad::controlEndedChangeGesture(CControl &ctl)
{
    int id = ctl.getTag();
    if (id >= 0 && id < kNumParameters)
        this->editParameter(id, false);
}

// -----------------------------------------------------------------------

}

// -----------------------------------------------------------------------

UI* DISTRHO::createUI() {
    return new Igorski::UIFogpad;
}
