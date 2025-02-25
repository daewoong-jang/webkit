/*
 * Copyright (C) 2016 Devin Rousso <webkit@devinrousso.com>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

WI.CollectionContentView = class CollectionContentView extends WI.ContentView
{
    constructor(collection, contentViewConstructor, contentPlaceholderText)
    {
        console.assert(collection instanceof WI.Collection);

        super(collection);

        this.element.classList.add("collection");

        this._contentPlaceholder = new WI.TitleView(contentPlaceholderText || WI.CollectionContentView.titleForCollection(collection));
        this._contentViewConstructor = contentViewConstructor;
        this._contentViewMap = new Map;
        this._handleClickMap = new WeakMap;
        this._selectedItem = null;
        this._selectionEnabled = false;
    }

    static titleForCollection(collection)
    {
        switch (collection.typeVerifier) {
        case WI.Collection.TypeVerifier.Frame:
            return WI.UIString("Frames");
        case WI.Collection.TypeVerifier.Resource:
            return WI.UIString("Resources");
        case WI.Collection.TypeVerifier.Script:
            return WI.UIString("Scripts");
        case WI.Collection.TypeVerifier.CSSStyleSheet:
            return WI.UIString("Stylesheets");
        case WI.Collection.TypeVerifier.Canvas:
            return WI.UIString("Canvases");
        case WI.Collection.TypeVerifier.ShaderProgram:
            return WI.UIString("Shader Programs");
        }

        console.warn("No default title for Collection type verifier.", collection.typeVerifier);
        return WI.UIString("Collection");
    }

    // Public

    get supplementalRepresentedObjects()
    {
        if (this._selectedItem)
            return [this._selectedItem];
        return [];
    }

    get selectionEnabled()
    {
        return this._selectionEnabled;
    }

    set selectionEnabled(value)
    {
        if (this._selectionEnabled === value)
            return;

        this._selectionEnabled = value;
        if (!this._selectionEnabled)
            this._selectItem(null);
    }

    // Protected

    addContentViewForItem(item)
    {
        if (!this._contentViewConstructor)
            return;

        if (this._contentViewMap.has(item)) {
            console.assert(false, "Already added ContentView for item.", item);
            return;
        }

        if (this._contentPlaceholder.parentView)
            this.removeSubview(this._contentPlaceholder);

        let contentView = new this._contentViewConstructor(item);
        console.assert(contentView instanceof WI.ContentView);

        let handleClick = (event) => {
            if (event.button !== 0 || event.ctrlKey)
                return;

            if (this._selectionEnabled)
                this._selectItem(item);
            else
                WI.showRepresentedObject(item);
        };

        this._contentViewMap.set(item, contentView);
        this._handleClickMap.set(item, handleClick);
        contentView.element.addEventListener("click", handleClick);

        this.addSubview(contentView);
        this.contentViewAdded(contentView);
    }

    removeContentViewForItem(item)
    {
        if (!this._contentViewConstructor)
            return;

        let contentView = this._contentViewMap.get(item);
        console.assert(contentView);
        if (!contentView)
            return;

        this.removeSubview(contentView);
        this._contentViewMap.delete(item);
        this.contentViewRemoved(contentView);

        contentView.removeEventListener(null, null, this);

        let handleClick = this._handleClickMap.get(item);
        console.assert(handleClick);

        if (handleClick) {
            contentView.element.removeEventListener("click", handleClick);
            this._handleClickMap.delete(item);
        }

        if (this._selectedItem === item)
            this._selectItem(null);

        if (!this.subviews.length)
            this.addSubview(this._contentPlaceholder);
    }

    contentViewAdded(contentView)
    {
        // Implemented by subclasses.
    }

    contentViewRemoved(contentView)
    {
        // Implemented by subclasses.
    }

    initialLayout()
    {
        let items = this.representedObject.items;
        if (!items.size || !this._contentViewConstructor) {
            this.addSubview(this._contentPlaceholder);
            return;
        }

        for (let item of items)
            this.addContentViewForItem(item);
    }

    attached()
    {
        super.attached();

        this.representedObject.addEventListener(WI.Collection.Event.ItemAdded, this._handleItemAdded, this);
        this.representedObject.addEventListener(WI.Collection.Event.ItemRemoved, this._handleItemRemoved, this);

        for (let item of this._contentViewMap.keys()) {
            if (this.representedObject.items.has(item))
                continue;

            this.removeContentViewForItem(item);
            if (this._selectedItem === item)
                this._selectItem(null);
        }

        for (let item of this.representedObject.items) {
            if (!this._contentViewMap.has(item))
                this.addContentViewForItem(item);
        }
    }

    detached()
    {
        this.representedObject.removeEventListener(null, null, this);

        super.detached();
    }

     // Private

    _handleItemAdded(event)
    {
        let item = event.data.item;
        if (!item)
            return;

        this.addContentViewForItem(item);
    }

    _handleItemRemoved(event)
    {
        let item = event.data.item;
        if (!item)
            return;

        this.removeContentViewForItem(item);
    }

    _handleContentError(event)
    {
        if (event && event.target)
            this._removeContentViewForItem(event.target.representedObject);
    }

    _selectItem(item)
    {
        if (this._selectedItem === item)
            return;

        if (this._selectedItem) {
            let contentView = this._contentViewMap.get(this._selectedItem);
            console.assert(contentView, "Missing ContentView for deselected item.", this._selectedItem);
            contentView.element.classList.remove("selected");
        }

        this._selectedItem = item;

        if (this._selectedItem) {
            let selectedContentView = this._contentViewMap.get(this._selectedItem);
            console.assert(selectedContentView, "Missing ContentView for selected item.", this._selectedItem);
            selectedContentView.element.classList.add("selected");
        }

        this.dispatchEventToListeners(WI.ContentView.Event.SupplementalRepresentedObjectsDidChange);
    }
};
