/*
 * Copyright (C) 2013 NAVER Corp. All rights reserved.
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
#include "ScrollbarThemeAndroid.h"

#include "GraphicsContext.h"
#include "PlatformMouseEvent.h"
#include "Scrollbar.h"
#include "TransformationMatrix.h"

#include <algorithm>

namespace WebCore {

#if OS(WINDOWS)
static const bool s_useOverlayScrollbars = false;
static const int s_scrollbarThickness = 16;
static const int s_scrollbarMargin = 2;
#else
static const bool s_useOverlayScrollbars = true;
static const int s_scrollbarThickness = 7;
static const int s_scrollbarMargin = 2;
#endif

static const float s_arrowSize = 7;

ScrollbarTheme& ScrollbarTheme::nativeTheme()
{
    static ScrollbarThemeAndroid theme;
    return theme;
}

int ScrollbarThemeAndroid::scrollbarThickness(ScrollbarControlSize, ScrollbarExpansionState)
{
    return s_scrollbarThickness;
}

bool ScrollbarThemeAndroid::usesOverlayScrollbars() const
{
    return s_useOverlayScrollbars;
}


bool ScrollbarThemeAndroid::hasButtons(Scrollbar&)
{
    return !s_useOverlayScrollbars;
}

bool ScrollbarThemeAndroid::hasThumb(Scrollbar& scrollbar)
{
    return thumbLength(scrollbar) > 0;
}

IntRect ScrollbarThemeAndroid::backButtonRect(Scrollbar& scrollbar, ScrollbarPart, bool)
{
    if (s_useOverlayScrollbars)
        return IntRect();

    IntRect rect = IntRect(scrollbar.frameRect().location(), IntSize(s_scrollbarThickness, s_scrollbarThickness));
    rect.inflate(-s_scrollbarMargin);
    return rect;
}

IntRect ScrollbarThemeAndroid::forwardButtonRect(Scrollbar& scrollbar, ScrollbarPart, bool)
{
    if (s_useOverlayScrollbars)
        return IntRect();

    IntRect rect;
    if (scrollbar.orientation() == HorizontalScrollbar)
        rect = IntRect(scrollbar.frameRect().maxX() - s_scrollbarThickness, scrollbar.frameRect().y(), s_scrollbarThickness, s_scrollbarThickness);
    else
        rect = IntRect(scrollbar.frameRect().x(), scrollbar.frameRect().maxY() - s_scrollbarThickness, s_scrollbarThickness, s_scrollbarThickness);

    rect.inflate(-s_scrollbarMargin);
    return rect;
}

IntRect ScrollbarThemeAndroid::trackRect(Scrollbar& scrollbar, bool)
{
    IntRect rect = scrollbar.frameRect();

    int buttonSize = 0;
    if (!s_useOverlayScrollbars)
        buttonSize = s_scrollbarThickness;

    if (scrollbar.orientation() == HorizontalScrollbar) {
        return IntRect(rect.x() + buttonSize,
            rect.y() + s_scrollbarMargin,
            rect.width() - buttonSize * 2,
            rect.height() - s_scrollbarMargin * 2);
    }

    return IntRect(rect.x() + s_scrollbarMargin,
        rect.y() + buttonSize,
        rect.width() - s_scrollbarMargin * 2,
        rect.height() - buttonSize * 2);
}

void ScrollbarThemeAndroid::paintScrollbarBackground(GraphicsContext& context, Scrollbar& scrollbar)
{
    if (s_useOverlayScrollbars)
        return;

    IntRect rect = scrollbar.frameRect();
    context.fillRect(rect, Color(240, 240, 240));
}

void ScrollbarThemeAndroid::paintTrackBackground(GraphicsContext&, Scrollbar&, const IntRect&)
{
}

void ScrollbarThemeAndroid::paintTrackPiece(GraphicsContext&, Scrollbar&, const IntRect&, ScrollbarPart)
{
}

void ScrollbarThemeAndroid::paintButton(GraphicsContext& context, Scrollbar& scrollbar, const IntRect& rect, ScrollbarPart currentPart)
{
    if (s_useOverlayScrollbars)
        return;

    if (currentPart == BackButtonEndPart || currentPart == ForwardButtonEndPart)
        return;

    context.save();

    Color arrowColor;
    bool isButtonActive = false;
    if ((currentPart == BackButtonStartPart && scrollbar.currentPos())
        || (currentPart == ForwardButtonStartPart && scrollbar.currentPos() != scrollbar.maximum())) {
        arrowColor = Color(80, 80, 80);
        isButtonActive = true;
    } else
        arrowColor = Color(163, 163, 163);

    FloatPoint arrowPoint(
        rect.x() + (rect.width() - s_arrowSize) / 2,
        rect.y() + (rect.height() - s_arrowSize) / 2);

    if (isButtonActive && scrollbar.hoveredPart() == currentPart) {
        Color backgroundColor;

        if (scrollbar.pressedPart() == currentPart)
            backgroundColor = Color(141, 141, 142);
        else
            backgroundColor = Color(170, 170, 171);

        context.fillRect(rect, backgroundColor);
    }

    // Draw arrow
    Path arrowPath;
    if (scrollbar.orientation() == HorizontalScrollbar) {
        if (currentPart == BackButtonStartPart) {
            arrowPath.moveTo(FloatPoint(arrowPoint.x() + 2, arrowPoint.y() + s_arrowSize / 2));
            arrowPath.addLineTo(FloatPoint(arrowPoint.x() + s_arrowSize, arrowPoint.y()));
            arrowPath.addLineTo(FloatPoint(arrowPoint.x() + s_arrowSize, arrowPoint.y() + s_arrowSize));
        }
        else {
            arrowPath.moveTo(FloatPoint(arrowPoint.x(), arrowPoint.y()));
            arrowPath.addLineTo(FloatPoint(arrowPoint.x(), arrowPoint.y() + s_arrowSize));
            arrowPath.addLineTo(FloatPoint(arrowPoint.x() + s_arrowSize - 2, arrowPoint.y() + s_arrowSize / 2));
        }
    } else {
        if (currentPart == BackButtonStartPart) {
            arrowPath.moveTo(FloatPoint(arrowPoint.x() + s_arrowSize / 2, arrowPoint.y() + 2));
            arrowPath.addLineTo(FloatPoint(arrowPoint.x(), arrowPoint.y() + s_arrowSize));
            arrowPath.addLineTo(FloatPoint(arrowPoint.x() + s_arrowSize, arrowPoint.y() + s_arrowSize));
        }
        else {
            arrowPath.moveTo(FloatPoint(arrowPoint.x(), arrowPoint.y()));
            arrowPath.addLineTo(FloatPoint(arrowPoint.x() + s_arrowSize, arrowPoint.y()));
            arrowPath.addLineTo(FloatPoint(arrowPoint.x() + s_arrowSize / 2, arrowPoint.y() + s_arrowSize - 2));
        }
    }

    context.setFillColor(arrowColor);
    context.fillPath(arrowPath);

    context.restore();
}

void ScrollbarThemeAndroid::paintThumb(GraphicsContext& context, Scrollbar& scrollbar, const IntRect& rect)
{
    IntRect thumbRect = rect;
    if (scrollbar.orientation() == HorizontalScrollbar)
        thumbRect.inflateY(-s_scrollbarMargin);
    else
        thumbRect.inflateX(-s_scrollbarMargin);

    if (s_useOverlayScrollbars) {
        context.fillRect(thumbRect, Color(128, 128, 128, 128));
        return;
    }

    if (scrollbar.hoveredPart() == ThumbPart) {
        if (scrollbar.pressedPart() == ThumbPart)
            context.fillRect(thumbRect, Color(141, 141, 142));
        else
            context.fillRect(thumbRect, Color(170, 170, 171));
    } else
        context.fillRect(thumbRect, Color(188, 188, 188));
}

} // namespace WebCore
