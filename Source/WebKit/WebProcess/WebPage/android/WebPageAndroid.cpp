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
#include "WebPage.h"

#include "DataReference.h"
#include "EditorState.h"
#include "HitTestResult.h"
#include "Logging.h"
#include "PrintInfo.h"
#include "WebEvent.h"
#include "WebFrame.h"
#include "WebPageProxyMessages.h"
#include "WindowsKeyboardCodes.h"
#include <WebCore/BackForwardController.h>
#include <WebCore/Document.h>
#include <WebCore/Editor.h>
#include <WebCore/EventHandler.h>
#include <WebCore/EventNames.h>
#include <WebCore/FocusController.h>
#include <WebCore/Frame.h>
#include <WebCore/FrameSelection.h>
#include <WebCore/FrameView.h>
#include <WebCore/KeyboardEvent.h>
#include <WebCore/KeyboardUtilitiesAndroid.h>
#include <WebCore/NotImplemented.h>
#include <WebCore/Page.h>
#include <WebCore/PlatformContextCairo.h>
#include <WebCore/PrintContext.h>
#include <WebCore/RenderBox.h>
#include <WebCore/Settings.h>
#include <WebCore/TextIterator.h>

#include <cairo-pdf.h>

using namespace WebCore;

namespace WebKit {

void WebPage::platformInitialize()
{
    notImplemented();
}

void WebPage::platformDetach()
{
    notImplemented();
}

void WebPage::platformEditorState(WebCore::Frame& frame, EditorState& result, IncludePostLayoutDataHint) const
{
    const VisibleSelection& selection = frame.selection().selection();
    if (frame.editor().hasComposition()) {
        RefPtr<Range> compositionRange = frame.editor().compositionRange();
        result.compositionRangeStart = compositionRange->startOffset();
        result.compositionRangeEnd = compositionRange->endOffset();
    }
    FrameView* view = frame.view();
    if (selection.isCaret()) {
        result.caretRectAtStart = view->contentsToRootView(frame.selection().absoluteCaretBounds());
        result.caretRectAtEnd = result.caretRectAtStart;
        RefPtr<Range> selectedRange = selection.toNormalizedRange();
        result.selectionRangeStart = selectedRange->startOffset();
        result.selectionRangeEnd = selectedRange->endOffset();
    } else if (selection.isRange()) {
        result.caretRectAtStart = view->contentsToRootView(VisiblePosition(selection.start()).absoluteCaretBounds());
        result.caretRectAtEnd = view->contentsToRootView(VisiblePosition(selection.end()).absoluteCaretBounds());
        RefPtr<Range> selectedRange = selection.toNormalizedRange();
        result.selectionRangeStart = selectedRange->startOffset();
        result.selectionRangeEnd = selectedRange->endOffset();
    }
    if (selection.isContentEditable()) {
        Element* root = selection.rootEditableElement();
        if (root)
            result.editableText = root->innerText();
    }
}

#if HAVE(ACCESSIBILITY)
void WebPage::updateAccessibilityTree()
{
    if (!m_accessibilityObject)
        return;

    webPageAccessibilityObjectRefresh(m_accessibilityObject.get());
}
#endif

void WebPage::platformPreferencesDidChange(const WebPreferencesStore&)
{
    notImplemented();
}

String WebPage::platformUserAgent(const WebCore::URL&) const
{
    notImplemented();
    return String();
}

static inline void scroll(Page* page, ScrollDirection direction, ScrollGranularity granularity)
{
    page->focusController().focusedOrMainFrame().eventHandler().scrollRecursively(direction, granularity);
}

bool WebPage::performDefaultBehaviorForKeyEvent(const WebKeyboardEvent& keyboardEvent)
{
    if (keyboardEvent.type() != WebEvent::KeyDown && keyboardEvent.type() != WebEvent::RawKeyDown)
        return false;

    Frame& frame = m_page->focusController().focusedOrMainFrame();
    const VisibleSelection& selection = frame.selection().selection();
    if (selection.isContentEditable())
        return false;

    switch (keyboardEvent.windowsVirtualKeyCode()) {
    case VK_BACK:
        if (keyboardEvent.shiftKey())
            m_page->backForward().goForward();
        else
            m_page->backForward().goBack();
        break;
    case VK_SPACE: {
        Element* element = frame.document()->focusedElement();
        if (element && element->isFormControlElement())
            return false;
        scroll(m_page.get(), keyboardEvent.shiftKey() ? ScrollUp : ScrollDown, ScrollByPage);
        break;
    }
    case VK_LEFT:
        scroll(m_page.get(), ScrollLeft, ScrollByLine);
        break;
    case VK_RIGHT:
        scroll(m_page.get(), ScrollRight, ScrollByLine);
        break;
    case VK_UP:
        scroll(m_page.get(), ScrollUp, ScrollByLine);
        break;
    case VK_DOWN:
        scroll(m_page.get(), ScrollDown, ScrollByLine);
        break;
    case VK_HOME:
        scroll(m_page.get(), ScrollUp, ScrollByDocument);
        break;
    case VK_END:
        scroll(m_page.get(), ScrollDown, ScrollByDocument);
        break;
    case VK_PRIOR:
        scroll(m_page.get(), ScrollUp, ScrollByPage);
        break;
    case VK_NEXT:
        scroll(m_page.get(), ScrollDown, ScrollByPage);
        break;
    default:
        return false;
    }

    return true;
}

bool WebPage::platformHasLocalDataForURL(const URL&)
{
    notImplemented();
    return false;
}

String WebPage::cachedResponseMIMETypeForURL(const URL&)
{
    notImplemented();
    return String();
}

bool WebPage::platformCanHandleRequest(const ResourceRequest&)
{
    notImplemented();
    return true;
}

String WebPage::cachedSuggestedFilenameForURL(const URL&)
{
    notImplemented();
    return String();
}

RefPtr<SharedBuffer> WebPage::cachedResponseDataForURL(const URL&)
{
    notImplemented();
    return 0;
}

const char* WebPage::interpretKeyEvent(const KeyboardEvent* event)
{
    ASSERT(event->type() == eventNames().keydownEvent || event->type() == eventNames().keypressEvent);

    if (event->type() == eventNames().keydownEvent)
        return getKeyDownCommandName(event);

    return getKeyPressCommandName(event);
}

static Frame* targetFrameForEditing(WebPage* page)
{
    Frame& frame = page->corePage()->focusController().focusedOrMainFrame();

    Editor& editor = frame.editor();
    if (!editor.canEdit())
        LOG(TextInput, "targetFrameForEditing, but canEdit flag is false");

    if (editor.hasComposition()) {
        // We should verify the parent node of this IME composition node are
        // editable because JavaScript may delete a parent node of the composition
        // node. In this case, WebKit crashes while deleting texts from the parent
        // node, which doesn't exist any longer.
        if (RefPtr<Range> range = editor.compositionRange()) {
            Node& node = range->startContainer();
            if (!node.isContentEditable())
                return 0;
        }
    }

    return &frame;
}

void WebPage::setComposition(const WTF::String& text, const WTF::Vector<WebCore::CompositionUnderline>& underlines, uint64_t selectionStart, uint64_t selectionEnd, uint64_t replacementStart, uint64_t replacementEnd)
{
    Frame* targetFrame = targetFrameForEditing(this);
    if (!targetFrame || !targetFrame->selection().selection().isContentEditable())
        return;

    Editor& editor = targetFrame->editor();
    editor.setIgnoreSelectionChanges(true);

    uint64_t replacementLength = replacementEnd - replacementStart;
    if (replacementLength > 0) {
        // The layout needs to be uptodate before setting a selection
        targetFrame->document()->updateLayout();

        Element* scope = targetFrame->selection().selection().rootEditableElement();
        RefPtr<Range> replacementRange = TextIterator::rangeFromLocationAndLength(scope, replacementStart, replacementLength);
        targetFrame->selection().setSelection(VisibleSelection(*replacementRange, SEL_DEFAULT_AFFINITY));
    }

    editor.setComposition(text, underlines, selectionStart, selectionEnd);
    editor.setIgnoreSelectionChanges(false);
    editor.respondToChangedSelection(targetFrame->selection().selection(), 0);
}

void WebPage::confirmComposition(const String& text)
{
    Frame* targetFrame = targetFrameForEditing(this);
    if (!targetFrame)
        return;

    Editor& editor = targetFrame->editor();
    editor.setIgnoreSelectionChanges(true);
    editor.confirmComposition(text);
    editor.setIgnoreSelectionChanges(false);
    editor.respondToChangedSelection(targetFrame->selection().selection(), 0);
}

void WebPage::finishComposition()
{
    Frame* targetFrame = targetFrameForEditing(this);
    if (!targetFrame)
        return;

    Editor& editor = targetFrame->editor();
    editor.setIgnoreSelectionChanges(true);
    editor.confirmComposition();
    editor.setIgnoreSelectionChanges(false);
    editor.respondToChangedSelection(targetFrame->selection().selection(), 0);
}

void WebPage::cancelComposition()
{
    Frame* targetFrame = targetFrameForEditing(this);
    if (!targetFrame)
        return;

    Editor& editor = targetFrame->editor();
    editor.setIgnoreSelectionChanges(true);
    editor.cancelComposition();
    editor.setIgnoreSelectionChanges(false);
    editor.respondToChangedSelection(targetFrame->selection().selection(), 0);
}

void WebPage::drawRectToImage(uint64_t frameID, const PrintInfo&, const WebCore::IntRect&, const WebCore::IntSize&, CallbackID callbackID)
{
    send(Messages::WebPageProxy::VoidCallback(callbackID));
}

static cairo_status_t writePdfDataToBuffer(void* closure, unsigned char* data, unsigned int length)
{
    SharedBuffer* buffer = reinterpret_cast<SharedBuffer*>(closure);
    if (!buffer)
        return CAIRO_STATUS_WRITE_ERROR;

    buffer->append(reinterpret_cast<const char*>(data), length);

    return CAIRO_STATUS_SUCCESS;
}

void WebPage::drawPagesToPDF(uint64_t frameID, const PrintInfo& printInfo, uint32_t first, uint32_t count, CallbackID callbackID)
{
    RefPtr<SharedBuffer> buffer = SharedBuffer::create();

    RefPtr<cairo_surface_t> cairoPdfSurface = adoptRef(cairo_pdf_surface_create_for_stream((cairo_write_func_t)writePdfDataToBuffer, buffer.get(), printInfo.availablePaperWidth, printInfo.availablePaperHeight));
    RefPtr<cairo_t> cairoContext = adoptRef(cairo_create(cairoPdfSurface.get()));
  
    size_t pageCount = m_printContext->pageCount();
    for (uint32_t page = first; page < first + count; ++page) {
        if (page >= pageCount)
            break;

        WebCore::PlatformContextCairo platformContext(cairoContext.get());
        WebCore::GraphicsContext graphicsContext(&platformContext);

        m_printContext->spoolPage(graphicsContext, page, printInfo.availablePaperWidth);

        cairo_surface_show_page(cairoPdfSurface.get());
    }

    cairo_surface_finish(cairoPdfSurface.get());

    send(Messages::WebPageProxy::DataCallback(IPC::DataReference(reinterpret_cast<const uint8_t*>(buffer->data()), buffer->size()), callbackID));

    endPrinting();
}

} // namespace WebKit
