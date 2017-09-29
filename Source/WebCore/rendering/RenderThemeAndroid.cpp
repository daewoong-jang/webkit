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

#include "config.h"
#include "RenderThemeAndroid.h"

#include "Color.h"
#include "Element.h"
#include "FloatRoundedRect.h"
#include "GraphicsContext.h"
#include "HTMLNames.h"
#include "HTMLOptionElement.h"
#include "HTMLSelectElement.h"
#include "HTMLInputElement.h"
#include "InputTypeNames.h"
#include "NotImplemented.h"
#include "Node.h"
#include "RenderObject.h"
#include "RenderSlider.h"
#include "SliderThumbElement.h"
#include "UserAgentStyleSheets.h"

#if ENABLE(MEDIA_CONTROLS_SCRIPT)
#include "UserAgentScripts.h"
#endif

namespace WebCore {

// This is the color of selection in a textfield.  It was computed from
// frameworks/base/core/res/res/values/colors.xml, which uses #9983CC39
// (decimal a = 153, r = 131, g = 204, b = 57)
// for all four highlighted text values. Blending this with white yields:
// R = (131 * 153 + 255 * (255 - 153)) / 255  -> 180.6
// G = (204 * 153 + 255 * (255 - 153)) / 255  -> 224.4
// B = ( 57 * 153 + 255 * (255 - 153)) / 255  -> 136.2

const RGBA32 selectionColor = makeRGB(181, 224, 136);

// Colors copied from the holo resources
const RGBA32 defaultBgColor = makeRGBA(204, 204, 204, 197);
const RGBA32 defaultBgBright = makeRGBA(213, 213, 213, 221);
const RGBA32 defaultBgDark = makeRGBA(92, 92, 92, 160);
const RGBA32 defaultBgMedium = makeRGBA(132, 132, 132, 111);
const RGBA32 defaultFgColor = makeRGBA(101, 101, 101, 225);
const RGBA32 defaultCheckColor = makeRGBA(0, 153, 204, 255);
const RGBA32 defaultCheckColorShadow = makeRGBA(29, 123, 154, 192);

const RGBA32 disabledBgColor = makeRGBA(205, 205, 205, 107);
const RGBA32 disabledBgBright = makeRGBA(213, 213, 213, 133);
const RGBA32 disabledBgDark = makeRGBA(92, 92, 92, 96);
const RGBA32 disabledBgMedium = makeRGBA(132, 132, 132, 111);
const RGBA32 disabledFgColor = makeRGBA(61, 61, 61, 68);
const RGBA32 disabledCheckColor = makeRGBA(61, 61, 61, 128);
const RGBA32 disabledCheckColorShadow = disabledCheckColor;

const static int defaultToggleSize = 13;
const static int focusRingBorderForInput = 4;
const static int focusRingBorderForButton = 2;

const int paddingButton = 2;
const int cornerButton = 2;

// scale factors for various resolutions
const float scaleFactor[4] = {
    1.0f, // medium res
    1.5f, // high res
    2.0f  // extra high res
};

// Draws a nice, mitered line.
// This is a partial copy from RenderObject::drawLineForBoxSide
static void drawLineForBoxSide(GraphicsContext& context, int x1, int y1,
                               int x2, int y2, BoxSide side, Color color,
                               int adjacentWidth1, int adjacentWidth2)
{
    static const bool antialias = false;
    context.setFillColor(color);
    if (!adjacentWidth1 && !adjacentWidth2) {
        // Turn off antialiasing to match the behavior of drawConvexPolygon();
        // this matters for rects in transformed contexts.
        bool wasAntialiased = context.shouldAntialias();
        context.setShouldAntialias(antialias);
        context.drawRect(IntRect(x1, y1, x2 - x1, y2 - y1));
        context.setShouldAntialias(wasAntialiased);
        return;
    }
    FloatPoint quad[4];
    switch (side) {
        case BSTop:
            quad[0] = FloatPoint(x1 + std::max(-adjacentWidth1, 0), y1);
            quad[1] = FloatPoint(x1 + std::max(adjacentWidth1, 0), y2);
            quad[2] = FloatPoint(x2 - std::max(adjacentWidth2, 0), y2);
            quad[3] = FloatPoint(x2 - std::max(-adjacentWidth2, 0), y1);
            break;
        case BSBottom:
            quad[0] = FloatPoint(x1 + std::max(adjacentWidth1, 0), y1);
            quad[1] = FloatPoint(x1 + std::max(-adjacentWidth1, 0), y2);
            quad[2] = FloatPoint(x2 - std::max(-adjacentWidth2, 0), y2);
            quad[3] = FloatPoint(x2 - std::max(adjacentWidth2, 0), y1);
            break;
        case BSLeft:
            quad[0] = FloatPoint(x1, y1 + std::max(-adjacentWidth1, 0));
            quad[1] = FloatPoint(x1, y2 - std::max(-adjacentWidth2, 0));
            quad[2] = FloatPoint(x2, y2 - std::max(adjacentWidth2, 0));
            quad[3] = FloatPoint(x2, y1 + std::max(adjacentWidth1, 0));
            break;
        case BSRight:
            quad[0] = FloatPoint(x1, y1 + std::max(adjacentWidth1, 0));
            quad[1] = FloatPoint(x1, y2 - std::max(adjacentWidth2, 0));
            quad[2] = FloatPoint(x2, y2 - std::max(-adjacentWidth2, 0));
            quad[3] = FloatPoint(x2, y1 + std::max(-adjacentWidth1, 0));
            break;
    }

    Vector<FloatPoint> points;
    points.append(quad, 4);
    context.strokePath(Path::polygonPathFromPoints(points));
}

enum WidgetType {
    CheckBox,
    Radio
};

static void setToggleSize(WidgetType, RenderStyle& style)
{
    // If the width and height are both specified, then we have nothing to do.
    if (!style.width().isIntrinsicOrAuto() && !style.height().isAuto())
        return;

    style.setWidth(Length(defaultToggleSize, Fixed));
    style.setHeight(Length(defaultToggleSize, Fixed));
}

static Path getCheckMarkPath(const IntRect& rect)
{
    const int shortLineDelta = static_cast<int>(defaultToggleSize / 6);
    const int longLineDelta = static_cast<int>(defaultToggleSize / 4);

    const FloatPoint centerPosition(rect.center().x(), rect.center().y());

    Vector<FloatPoint> vertexes;
    vertexes.append(FloatPoint(rect.maxX() - shortLineDelta, rect.y()));
    vertexes.append(FloatPoint(rect.maxX(), rect.y() + longLineDelta));
    vertexes.append(FloatPoint(centerPosition.x(), centerPosition.y() + longLineDelta));
    vertexes.append(FloatPoint(centerPosition.x() - longLineDelta, centerPosition.y()));
    vertexes.append(FloatPoint(centerPosition.x() - shortLineDelta, centerPosition.y() - shortLineDelta));

    Path paths;
    paths.moveTo(FloatPoint(centerPosition.x(), centerPosition.y()));
    for (unsigned int i = 0; i < vertexes.size(); ++i)
        paths.addLineTo(vertexes[i]);

    paths.closeSubpath();
    return paths;
}

static bool paintToggle(const RenderObject& obj, WidgetType widgetType, const PaintInfo& info, const IntRect& rect)
{
    if (!is<Element>(*obj.node()))
        return false;

    Element& element = downcast<Element>(*obj.node());
    Color borderColor = defaultFgColor;
    Color checkColor = defaultCheckColor;
    Color checkColorShadow = defaultCheckColorShadow;
    if (element.isDisabledFormControl()) {
        borderColor = disabledFgColor;
        checkColor = disabledCheckColor;
        checkColorShadow = disabledCheckColorShadow;
    }

    if (rect.isEmpty())
        return false;
    IntRect borderRect(rect);

    GraphicsContext& context = info.context();
    context.save();
    context.setStrokeStyle(SolidStroke);
    context.setStrokeColor(borderColor);
    context.setStrokeThickness(1);
    context.setFillColor(Color::transparent);
    context.setShadow(FloatSize(), 1.0f, borderColor);
    switch (widgetType) {
    case CheckBox:
        // Set defaultToggleSize because other ports don't apply checkbox size defined by html content as below,
        // <input type="checkbox" style="min-height:20px;"/>
        borderRect.setWidth(defaultToggleSize);
        borderRect.setHeight(defaultToggleSize);
        context.drawRect(borderRect);
        break;
    case Radio:
        context.drawEllipse(borderRect);
        break;
    default:
        ASSERT_NOT_REACHED();
        return false;
    }

    if (downcast<HTMLInputElement>(element).checked()) {
        context.setFillColor(checkColor);
        context.setStrokeColor(Color::transparent);
        context.setShadow(FloatSize(), 2, checkColorShadow);
        switch (widgetType) {
        case CheckBox:
            context.fillPath(getCheckMarkPath(borderRect));
            break;
        case Radio:
            borderRect.inflate(-3);
            context.drawEllipse(borderRect);
            break;
        default:
            ASSERT_NOT_REACHED();
            return false;
        }
    }

    if (obj.theme().isFocused(obj))
        context.drawFocusRing(Vector<FloatRect>(1, rect), focusRingBorderForInput, 0, obj.theme().platformFocusRingColor());

    context.restore();

    return true;
}

static bool supportsFocus(ControlPart appearance)
{
    switch (appearance) {
    case PushButtonPart:
    case ButtonPart:
    case DefaultButtonPart:
    case RadioPart:
    case CheckboxPart:
        return true;
    case TextFieldPart:
    case TextAreaPart:
    case MenulistButtonPart:
    case ListboxPart:
        return false;
    default:
        return false;
    }
}

RenderTheme& RenderTheme::singleton()
{
    static NeverDestroyed<RenderThemeAndroid> theme;
    return theme;
}

RenderThemeAndroid::RenderThemeAndroid()
{
}

RenderThemeAndroid::~RenderThemeAndroid()
{
}

void RenderThemeAndroid::close()
{
}

bool RenderThemeAndroid::stateChanged(const RenderObject& obj, ControlStates::States state) const
{
    if (ControlStates::CheckedState == state) {
        obj.repaint();
        return true;
    }
    return false;
}

Color RenderThemeAndroid::platformActiveSelectionBackgroundColor() const
{
    return Color(selectionColor);
}

Color RenderThemeAndroid::platformActiveTextSearchHighlightColor() const
{
    return Color(0x00, 0x99, 0xcc, 0x99); // HOLO_DARK
}

Color RenderThemeAndroid::platformInactiveTextSearchHighlightColor() const
{
    return Color(0x33, 0xb5, 0xe5, 0x66); // HOLO_LIGHT
}

void RenderThemeAndroid::addIntrinsicMargins(RenderStyle&) const
{
}

void RenderThemeAndroid::adjustButtonStyle(StyleResolver&, RenderStyle&, const Element*) const
{
}

#if ENABLE(VIDEO)
String RenderThemeAndroid::mediaControlsStyleSheet()
{
#if ENABLE(MEDIA_CONTROLS_SCRIPT)
    return ASCIILiteral(mediaControlsBaseUserAgentStyleSheet);
#else
    return String();
#endif
}

String RenderThemeAndroid::mediaControlsScript()
{
#if ENABLE(MEDIA_CONTROLS_SCRIPT)
    StringBuilder scriptBuilder;
    scriptBuilder.append(mediaControlsLocalizedStringsJavaScript, sizeof(mediaControlsLocalizedStringsJavaScript));
    scriptBuilder.append(mediaControlsBaseJavaScript, sizeof(mediaControlsBaseJavaScript));
    return scriptBuilder.toString();
#else
    return String();
#endif
}
#endif

bool RenderThemeAndroid::paintCheckbox(const RenderObject& obj, const PaintInfo& info, const IntRect& rect)
{
    return paintToggle(obj, CheckBox, info, rect);
}

bool RenderThemeAndroid::paintButton(const RenderObject& obj, const PaintInfo& info, const IntRect& rect)
{
    // If it is a disabled button, simply paint it to the master picture.
    Node* node = obj.node();
    Element* formControlElement = static_cast<Element*>(node);
    if (formControlElement) {
        GraphicsContext& context = info.context();
        IntRect innerrect = IntRect(rect.x() + paddingButton, rect.y() + paddingButton,
                rect.width() - 2 * paddingButton, rect.height() - 2 * paddingButton);
        IntSize cornerrect = IntSize(cornerButton, cornerButton);
        Color bg, bright, dark, medium;
        if (!formControlElement->isDisabledFormControl()) {
            bg = Color(defaultBgColor);
            bright = Color(defaultBgBright);
            dark = Color(defaultBgDark);
            medium = Color(defaultBgMedium);
        } else {
            bg = Color(disabledBgColor);
            bright = Color(disabledBgBright);
            dark = Color(disabledBgDark);
            medium = Color(disabledBgMedium);
        }
        context.save();
        if (isFocused(obj)) {
            IntRect focusRingRect = IntRect(rect.x() - focusRingBorderForButton, rect.y() - focusRingBorderForButton,
                rect.width() + 2 * focusRingBorderForButton, rect.height() + 2 * focusRingBorderForButton);
            drawLineForBoxSide(context, focusRingRect.x(), focusRingRect.y(), focusRingRect.maxX(), rect.y(),
                BSTop, platformFocusRingColor(), focusRingBorderForButton, focusRingBorderForButton);
            drawLineForBoxSide(context, focusRingRect.x(), focusRingRect.y(), rect.x(), focusRingRect.maxY(),
                BSLeft, platformFocusRingColor(), focusRingBorderForButton, focusRingBorderForButton);
            drawLineForBoxSide(context, rect.maxX(), focusRingRect.y(), focusRingRect.maxX(), focusRingRect.maxY(),
                BSRight, platformFocusRingColor(), focusRingBorderForButton, focusRingBorderForButton);
            drawLineForBoxSide(context, focusRingRect.x(), rect.maxY(), focusRingRect.maxX(), focusRingRect.maxY(),
                BSBottom, platformFocusRingColor(), focusRingBorderForButton, focusRingBorderForButton);
        }
        FloatRoundedRect border(rect, cornerrect, cornerrect, cornerrect, cornerrect);
        context.clipRoundedRect(border);
        context.setStrokeStyle(NoStroke);
        drawLineForBoxSide(context, rect.x(), rect.y(), rect.maxX(), innerrect.y(),
                            BSTop, bright, paddingButton, paddingButton);
        drawLineForBoxSide(context, rect.x(), rect.y(), innerrect.x(), rect.maxY(),
                            BSLeft, medium, paddingButton, paddingButton);
        drawLineForBoxSide(context, innerrect.maxX(), rect.y(), rect.maxX(), rect.maxY(),
                            BSRight, medium, paddingButton, paddingButton);
        drawLineForBoxSide(context, rect.x(), innerrect.maxY(), rect.maxX(), rect.maxY(),
                            BSBottom, dark, paddingButton, paddingButton);
        context.fillRect(innerrect, bg);
        context.restore();
    }


    // We always return false so we do not request to be redrawn.
    return false;
}

bool RenderThemeAndroid::paintRadio(const RenderObject& obj, const PaintInfo& info, const IntRect& rect)
{
    return paintToggle(obj, Radio, info, rect);
}

void RenderThemeAndroid::setCheckboxSize(RenderStyle& style) const
{
    setToggleSize(CheckBox, style);
}

void RenderThemeAndroid::setRadioSize(RenderStyle& style) const
{
    setToggleSize(Radio, style);
}

bool RenderThemeAndroid::paintTextField(const RenderObject&, const PaintInfo&, const FloatRect&)
{
    return true;
}

bool RenderThemeAndroid::paintTextArea(const RenderObject& obj, const PaintInfo& info, const FloatRect& rect)
{
    if (obj.isMenuList())
        paintCombo(obj, info, rect);
    return true;
}

void RenderThemeAndroid::adjustSearchFieldStyle(StyleResolver&, RenderStyle& style, const Element*) const
{
    addIntrinsicMargins(style);
}

bool RenderThemeAndroid::paintSearchField(const RenderObject&, const PaintInfo&, const IntRect&)
{
    return true;
}

static void adjustMenuListStyleCommon(RenderStyle& style)
{
    // Added to make room for our arrow and make the touch target less cramped.
    const int padding = (int)(scaleFactor[1/*RenderSkinAndroid::DrawableResolution()*/] + 0.5f);
    style.setPaddingLeft(Length(padding,Fixed));
    style.setPaddingTop(Length(padding, Fixed));
    style.setPaddingBottom(Length(padding, Fixed));
    // allocate height as arrow size
    int arrow = std::max(18, style.fontMetrics().height() + 2 * padding);
    style.setPaddingRight(Length(arrow, Fixed));
    style.setMinHeight(Length(arrow, Fixed));
    style.setHeight(Length(arrow, Fixed));
}

void RenderThemeAndroid::adjustMenuListStyle(StyleResolver&, RenderStyle& style, const Element*) const
{
    adjustMenuListStyleCommon(style);
    addIntrinsicMargins(style);
}

bool RenderThemeAndroid::paintCombo(const RenderObject& obj, const PaintInfo& info, const FloatRect& rect)
{
  if (!obj.style().visitedDependentColor(CSSPropertyBackgroundColor).alpha())
        return true;
    Node* node = obj.node();
    Element* element = static_cast<Element*>(node);
    if (element) {
        GraphicsContext& context = info.context();
        context.save();
        if (element->isDisabledFormControl())
            context.setAlpha(0.5f);
        IntRect bounds = IntRect(rect.x(), rect.y(), rect.width(), rect.height());
        // paint bg color
        const RenderStyle* style = &obj.style();
        context.setFillColor(style->visitedDependentColor(CSSPropertyBackgroundColor));
        context.fillRect(FloatRect(bounds));
        // copied form the original RenderSkinCombo:
        // If this is an appearance where RenderTheme::paint returns true
        // without doing anything, this means that
        // RenderBox::PaintBoxDecorationWithSize will end up painting the
        // border, so we shouldn't paint a border here.
        if (style->appearance() != MenulistButtonPart &&
                style->appearance() != ListboxPart &&
                style->appearance() != TextFieldPart &&
                style->appearance() != TextAreaPart) {
            const int arrowSize = bounds.height();
            // dropdown button bg
            context.setFillColor(Color(defaultBgColor));
            context.fillRect(FloatRect(bounds.maxX() - arrowSize + 0.5f, bounds.y() + .5f,
                    arrowSize - 1, bounds.height() - 1));
            // outline
            context.setStrokeStyle(SolidStroke);
            context.setStrokeThickness(1.0f);
            context.setStrokeColor(Color(defaultBgDark));
            context.strokeRect(bounds, 1.0f);
            // arrow
            context.setFillColor(Color(defaultFgColor));
            Path tri = Path();
            tri.clear();
            const float aw = arrowSize - 10;
            FloatPoint br = FloatPoint(bounds.maxX() - 4, bounds.maxY() - 4);
            tri.moveTo(br);
            tri.addLineTo(FloatPoint(br.x() - aw, br.y()));
            tri.addLineTo(FloatPoint(br.x(), br.y() - aw));
            context.fillPath(tri);
        }
        context.restore();
    }
    return false;
}

void RenderThemeAndroid::updateCachedSystemFontDescription(CSSValueID, FontCascadeDescription&) const
{
    notImplemented();
}

bool RenderThemeAndroid::paintMenuList(const RenderObject& obj, const PaintInfo& info, const FloatRect& rect)
{
    return paintCombo(obj, info, rect);
}

void RenderThemeAndroid::adjustMenuListButtonStyle(StyleResolver&,
        RenderStyle& style, const Element*) const
{
    // Copied from RenderThemeSafari.
    const float baseFontSize = 11.0f;
    const int baseBorderRadius = 5;
    float fontScale = style.computedFontPixelSize() / baseFontSize;

    style.resetPadding();
    style.setBorderRadius(IntSize(int(baseBorderRadius + fontScale - 1), int(baseBorderRadius + fontScale - 1))); // FIXME: Round up?

    const int minHeight = 15;
    style.setMinHeight(Length(minHeight, Fixed));
    
    style.setLineHeight(RenderStyle::initialLineHeight());
    // Found these padding numbers by trial and error.
    const int padding = 4;
    style.setPaddingTop(Length(padding, Fixed));
    style.setPaddingLeft(Length(padding, Fixed));
    adjustMenuListStyleCommon(style);
}

bool RenderThemeAndroid::paintMenuListButtonDecorations(const RenderBox&, const PaintInfo&, const FloatRect&)
{
    notImplemented();
    return false;
}

void RenderThemeAndroid::adjustInnerSpinButtonStyle(StyleResolver&, RenderStyle&, const Element*) const
{
    notImplemented();
}

bool RenderThemeAndroid::paintInnerSpinButton(const RenderObject&, const PaintInfo&, const IntRect&)
{
    notImplemented();
    return false;
}

bool RenderThemeAndroid::paintSliderTrack(const RenderObject&, const PaintInfo&, const IntRect&)
{
    notImplemented();
    return false;
}

bool RenderThemeAndroid::paintSliderThumb(const RenderObject&, const PaintInfo&, const IntRect&)
{
    notImplemented();
    return false;
}

void RenderThemeAndroid::adjustSliderThumbSize(RenderStyle&, const Element*) const
{
    notImplemented();
}

Color RenderThemeAndroid::platformFocusRingColor() const
{
    static Color focusRingColor(0x33, 0xB5, 0xE5, 0x66);
    return focusRingColor;
}

bool RenderThemeAndroid::supportsFocusRing(const RenderStyle& style) const
{
    return supportsFocus(style.appearance());
}

String RenderThemeAndroid::extraDefaultStyleSheet()
{
    notImplemented();
    return String();
}

String RenderThemeAndroid::extraQuirksStyleSheet()
{
    notImplemented();
    return String();
}

void RenderThemeAndroid::themeChanged()
{
    notImplemented();
}

bool RenderThemeAndroid::supportsHover(const RenderStyle&) const
{
    notImplemented();
    return false;
}

Color RenderThemeAndroid::systemColor(CSSValueID) const
{
    notImplemented();
    return Color();
}

Color RenderThemeAndroid::platformInactiveSelectionBackgroundColor() const
{
    notImplemented();
    return Color();
}

Color RenderThemeAndroid::platformActiveSelectionForegroundColor() const
{
    notImplemented();
    return Color();
}

Color RenderThemeAndroid::platformInactiveSelectionForegroundColor() const
{
    notImplemented();
    return Color();
}

void RenderThemeAndroid::adjustProgressBarStyle(StyleResolver&, RenderStyle&, const Element*) const
{
    notImplemented();
}

bool RenderThemeAndroid::paintProgressBar(const RenderObject&, const PaintInfo&, const IntRect&)
{
    notImplemented();
    return false;
}

Seconds RenderThemeAndroid::animationRepeatIntervalForProgressBar(RenderProgress&) const
{
    notImplemented();
    return Seconds();
}

double RenderThemeAndroid::animationDurationForProgressBar(RenderProgress&) const
{
    notImplemented();
    return 0.0;
}

void RenderThemeAndroid::adjustSearchFieldCancelButtonStyle(StyleResolver&, RenderStyle&, const Element*) const
{
    notImplemented();
}

void RenderThemeAndroid::adjustSearchFieldDecorationPartStyle(StyleResolver&, RenderStyle&, const Element*) const
{
    notImplemented();
}

void RenderThemeAndroid::adjustSearchFieldResultsDecorationPartStyle(StyleResolver&, RenderStyle&, const Element*) const
{
    notImplemented();
}

bool RenderThemeAndroid::paintSearchFieldResultsDecorationPart(const RenderBox&, const PaintInfo&, const IntRect&)
{
    notImplemented();
    return false;
}

bool RenderThemeAndroid::paintSearchFieldCancelButton(const RenderBox&, const PaintInfo&, const IntRect&)
{
    notImplemented();
    return false;
}

void RenderThemeAndroid::adjustSearchFieldResultsButtonStyle(StyleResolver&, RenderStyle&, const Element*) const
{
    notImplemented();
}

bool RenderThemeAndroid::paintSearchFieldResultsButton(const RenderBox&, const PaintInfo&, const IntRect&)
{
    notImplemented();
    return false;
}

#if ENABLE(DATALIST_ELEMENT)
IntSize RenderThemeAndroid::sliderTickSize() const
{
    return IntSize(1, 6);
}

int RenderThemeAndroid::sliderTickOffsetFromTrackCenter() const
{
    static const int sliderTickOffset = -12;

    return sliderTickOffset;
}

LayoutUnit RenderThemeAndroid::sliderTickSnappingThreshold() const
{
    // The same threshold value as the Chromium port.
    return 5;
}
#endif

bool RenderThemeAndroid::supportsDataListUI(const AtomicString& type) const
{
#if ENABLE(DATALIST_ELEMENT)
    return type == InputTypeNames::range();
#else
    UNUSED_PARAM(type);
    return false;
#endif
}


} // namespace WebCore
