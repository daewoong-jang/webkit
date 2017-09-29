/*
 * Copyright (C) 2015 NAVER Corp. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RenderThemeAndroid_h
#define RenderThemeAndroid_h

#include "PaintInfo.h"
#include "RenderTheme.h"

namespace WebCore {

struct ThemeData {
    ThemeData() :m_part(0), m_state(0), m_classicState(0) {}
    ThemeData(int part, int state)
        : m_part(part)
        , m_state(state)
        , m_classicState(0)
    { }

    unsigned m_part;
    unsigned m_state;
    unsigned m_classicState;
};

class RenderThemeAndroid final: public RenderTheme {
public:
    friend NeverDestroyed<RenderThemeAndroid>;
    virtual ~RenderThemeAndroid();

    virtual bool stateChanged(const RenderObject&, ControlStates::States) const override;

    virtual String extraDefaultStyleSheet() override;
    virtual String extraQuirksStyleSheet() override;

    // A method asking if the theme's controls actually care about redrawing when hovered.
    virtual bool supportsHover(const RenderStyle&) const override;

    virtual Color platformActiveSelectionBackgroundColor() const override;
    virtual Color platformInactiveSelectionBackgroundColor() const override;
    virtual Color platformActiveSelectionForegroundColor() const override;
    virtual Color platformInactiveSelectionForegroundColor() const override;

    virtual Color platformActiveTextSearchHighlightColor() const override;
    virtual Color platformInactiveTextSearchHighlightColor() const override;

    virtual Color platformFocusRingColor() const override;

    virtual Color systemColor(CSSValueID) const override;

    virtual bool paintCheckbox(const RenderObject& o, const PaintInfo& i, const IntRect& r) override;
    virtual void setCheckboxSize(RenderStyle&) const override;

    virtual bool paintRadio(const RenderObject& o, const PaintInfo& i, const IntRect& r) override;
    virtual void setRadioSize(RenderStyle& style) const override;

    virtual bool paintButton(const RenderObject&, const PaintInfo&, const IntRect&) override;

    virtual void adjustInnerSpinButtonStyle(StyleResolver&, RenderStyle&, const Element*) const override;
    virtual bool paintInnerSpinButton(const RenderObject&, const PaintInfo&, const IntRect&) override;

    virtual bool paintTextField(const RenderObject&, const PaintInfo&, const FloatRect&) override;

    virtual bool paintTextArea(const RenderObject& o, const PaintInfo& i, const FloatRect& r) override;

    virtual void adjustMenuListStyle(StyleResolver&, RenderStyle&, const Element*) const override;
    virtual bool paintMenuList(const RenderObject&, const PaintInfo&, const FloatRect&) override;
    virtual void adjustMenuListButtonStyle(StyleResolver&, RenderStyle&, const Element*) const override;

    virtual bool paintMenuListButtonDecorations(const RenderBox&, const PaintInfo&, const FloatRect&) override;

    virtual bool paintSliderTrack(const RenderObject&, const PaintInfo&, const IntRect&) override;
    virtual bool paintSliderThumb(const RenderObject&, const PaintInfo&, const IntRect&) override;
    virtual void adjustSliderThumbSize(RenderStyle&, const Element*) const override;

    virtual bool popupOptionSupportsTextIndent() const override { return true; }

    virtual void adjustSearchFieldStyle(StyleResolver&, RenderStyle&, const Element*) const override;
    virtual bool paintSearchField(const RenderObject&, const PaintInfo&, const IntRect&) override;

    virtual void adjustSearchFieldCancelButtonStyle(StyleResolver&, RenderStyle&, const Element*) const override;
    virtual bool paintSearchFieldCancelButton(const RenderBox&, const PaintInfo&, const IntRect&) override;

    virtual void adjustSearchFieldDecorationPartStyle(StyleResolver&, RenderStyle&, const Element*) const override;
    virtual bool paintSearchFieldDecorationPart(const RenderObject&, const PaintInfo&, const IntRect&) override { return false; }

    virtual void adjustSearchFieldResultsDecorationPartStyle(StyleResolver&, RenderStyle&, const Element*) const override;
    virtual bool paintSearchFieldResultsDecorationPart(const RenderBox&, const PaintInfo&, const IntRect&) override;

    virtual void adjustSearchFieldResultsButtonStyle(StyleResolver&, RenderStyle&, const Element*) const override;
    virtual bool paintSearchFieldResultsButton(const RenderBox&, const PaintInfo&, const IntRect&) override;

    virtual void themeChanged() override;

    virtual void adjustButtonStyle(StyleResolver&, RenderStyle&, const Element*) const override;
    virtual void adjustTextFieldStyle(StyleResolver&, RenderStyle&, const Element*) const override { }
    virtual void adjustTextAreaStyle(StyleResolver&, RenderStyle&, const Element*) const override { }

    static void setWebKitIsBeingUnloaded();

    virtual bool supportsFocusRing(const RenderStyle&) const override;

#if ENABLE(VIDEO)
    virtual String mediaControlsStyleSheet() override;
    virtual String mediaControlsScript() override;
#endif

    virtual void adjustProgressBarStyle(StyleResolver&, RenderStyle&, const Element*) const override;
    virtual bool paintProgressBar(const RenderObject&, const PaintInfo&, const IntRect&) override;
    virtual Seconds animationRepeatIntervalForProgressBar(RenderProgress&) const override;
    virtual double animationDurationForProgressBar(RenderProgress&) const override;

    virtual bool supportsDataListUI(const AtomicString&) const override;

#if ENABLE(DATALIST_ELEMENT)
    virtual IntSize sliderTickSize() const override;
    virtual int sliderTickOffsetFromTrackCenter() const override;
    virtual LayoutUnit sliderTickSnappingThreshold() const override;
#endif

    bool paintCombo(const RenderObject&, const PaintInfo&, const FloatRect&);

private:
    enum ControlSubPart {
        None,
        SpinButtonDown,
        SpinButtonUp,
        MenulistDropdownButton,
        MenulistLeft,
        MenulistCenter,
        MenulistRight,
        ButtonLeft,
        ButtonCenter,
        ButtonRight,
    };

    RenderThemeAndroid();

    // System fonts.
    virtual void updateCachedSystemFontDescription(CSSValueID, FontCascadeDescription&) const override;

    void addIntrinsicMargins(RenderStyle&) const;
    void close();

    String m_mediaControlsScript;
    String m_mediaControlsStyleSheet;
};

};

#endif
