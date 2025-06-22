#ifndef SIMPLE_APP_H
#define SIMPLE_APP_H
#pragma once

#include "cef_app.h"

// Implement application-level callbacks for the browser process.
class SimpleCefApp : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler{
public:
    SimpleCefApp();

    // CefApp methods:
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override{
        return this;
    }

    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override {
		return this;
    }

        // CefBrowserProcessHandler methods:
    virtual void OnContextInitialized() override;
    virtual void OnWebKitInitialized() override;

private:
    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(SimpleCefApp);
};

#endif
