#pragma once
#include "cef_client.h"
#include "views/cef_browser_view.h"
#include <list>

class SimpleCefClient : public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler
{
public:
	explicit SimpleCefClient(bool use_views);
	~SimpleCefClient();

	static SimpleCefClient* GetInstance();

	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override
	{
		return this;
	}

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
	{
		return this;
	}

	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }

	// CefDisplayHandler的实现声明:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) override;
	// CefLifeSpanHandler的实现声明:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
	// CefLoadHandler的实现声明:
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) override;

	void CloseAllBrowsers(bool force_close); // 请求将所有的已经存在的浏览器窗体进行关闭
	bool IsClosing() const { return is_closing_; }

private:
	// 平台特定的标题修改
	// 当我们没有CEF的GUI视图框架的时候，就需要特定平台的标题修改实现
	// 例如，Windows中需要我们获取窗体句柄，调用Windows的API完成对该窗体的标题修改
	void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title);
	const bool use_views_; // 是否使用了CEF的GUI视图框架
	// List of existing browser windows. Only accessed on the CEF UI thread.
	typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
	BrowserList browser_list_;

	bool is_closing_;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(SimpleCefClient);
};
