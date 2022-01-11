﻿//
//  CefViewBrowserHandler.h
//  CefView
//
//  Created by Sheen Tian on 2020/6/11.
//

#ifndef CefViewBrowserHandler_h
#define CefViewBrowserHandler_h
#pragma once

#pragma region std_headers
#include <list>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#pragma endregion std_headers

#pragma region cef_headers
#include <include/cef_client.h>
#include <include/wrapper/cef_message_router.h>
#include <include/wrapper/cef_resource_manager.h>
#pragma endregion cef_headers

#include <CefViewBrowserClientDelegate.h>
#include <CefViewQueryHandler.h>

class CefViewBrowserClient
  : public CefClient
  , public CefContextMenuHandler
  , public CefDisplayHandler
  , public CefDragHandler
  , public CefJSDialogHandler
  , public CefFocusHandler
  , public CefKeyboardHandler
  , public CefLifeSpanHandler
  , public CefLoadHandler
  , public CefRequestHandler
  , public CefResourceRequestHandler
{
public:
  /// <summary>
  ///
  /// </summary>
  enum
  {
    MAIN_FRAME = (int64_t)0,
    ALL_FRAMES = (int64_t)-1,
  };

  ///  /// <summary>
  ///
  /// </summary>
  /// <param name="delegate"></param>
  CefViewBrowserClient(CefViewBrowserClientDelegateInterface::RefPtr delegate);

  /// <summary>
  ///
  /// </summary>
  ~CefViewBrowserClient();

  //////////////////////////////////////////////////////////////////////////
  // CefClient methods:
  virtual CefRefPtr<CefDragHandler> GetDragHandler() override { return this; }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }

  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message) override;

  // CefContextMenuHandler methods
  virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefContextMenuParams> params,
                                   CefRefPtr<CefMenuModel> model) override;
  virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefContextMenuParams> params,
                                    int command_id,
                                    EventFlags event_flags) override;

  // CefDisplayHandler methods
  virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) override;
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
  virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                cef_log_severity_t level,
                                const CefString& message,
                                const CefString& source,
                                int line) override;

  // CefDragHandler methods
  virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefDragData> dragData,
                           CefDragHandler::DragOperationsMask mask) override;

  virtual void OnDraggableRegionsChanged(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         const std::vector<CefDraggableRegion>& regions) override;

  // CefJSDialogHandler methods
  virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                          const CefString& origin_url,
                          JSDialogType dialog_type,
                          const CefString& message_text,
                          const CefString& default_prompt_text,
                          CefRefPtr<CefJSDialogCallback> callback,
                          bool& suppress_message) override;

  virtual bool OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
                                    const CefString& message_text,
                                    bool is_reload,
                                    CefRefPtr<CefJSDialogCallback> callback) override;
  virtual void OnResetDialogState(CefRefPtr<CefBrowser> browser) override;

  // CefFocusHandler methods
  void OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) override;
  bool OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source) override;

  // CefKeyboardHandler methods
  virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                             const CefKeyEvent& event,
                             CefEventHandle os_event,
                             bool* is_keyboard_shortcut) override;

  // CefLifeSpanHandler methods:
  virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& target_url,
                             const CefString& target_frame_name,
                             CefLifeSpanHandler::WindowOpenDisposition target_disposition,
                             bool user_gesture,
                             const CefPopupFeatures& popupFeatures,
                             CefWindowInfo& windowInfo,
                             CefRefPtr<CefClient>& client,
                             CefBrowserSettings& settings,
                             CefRefPtr<CefDictionaryValue>& extra_info,
                             bool* no_javascript_access) override;
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  // CefLoadHandler methods
  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                    bool isLoading,
                                    bool canGoBack,
                                    bool canGoForward) override;
  virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           TransitionType transition_type) override;
  virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) override;

  // CefRequestHandler methods
  virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefRequest> request,
                              bool user_gesture,
                              bool is_redirect) override;

  virtual bool OnOpenURLFromTab(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                const CefString& target_url,
                                CefRequestHandler::WindowOpenDisposition target_disposition,
                                bool user_gesture) override;

  virtual CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(CefRefPtr<CefBrowser> browser,
                                                                         CefRefPtr<CefFrame> frame,
                                                                         CefRefPtr<CefRequest> request,
                                                                         bool is_navigation,
                                                                         bool is_download,
                                                                         const CefString& request_initiator,
                                                                         bool& disable_default_handling) override;

  virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                              const CefString& origin_url,
                              int64 new_size,
                              CefRefPtr<CefRequestCallback> callback) override;

  virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) override;

  // CefResourceRequestHandler
  virtual ReturnValue OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                           CefRefPtr<CefFrame> frame,
                                           CefRefPtr<CefRequest> request,
                                           CefRefPtr<CefRequestCallback> callback) override;

  virtual CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser,
                                                           CefRefPtr<CefFrame> frame,
                                                           CefRefPtr<CefRequest> request) override;

  virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefRequest> request,
                                   bool& allow_os_execution) override;

  //////////////////////////////////////////////////////////////////////////

  void AddLocalDirectoryResourceProvider(const std::string& dir_path, const std::string& url, int priority = 0);

  void AddArchiveResourceProvider(const std::string& archive_path,
                                  const std::string& url,
                                  const std::string& password,
                                  int priority = 0);

  // Request that all existing browser windows close.
  void CloseAllBrowsers(bool force_close);

  bool TriggerEvent(CefRefPtr<CefBrowser> browser, const int64_t frame_id, const CefRefPtr<CefProcessMessage> msg);

  bool ResponseQuery(const int64_t query, bool success, const CefString& response, int error);

  bool DispatchNotifyRequest(CefRefPtr<CefBrowser> browser,
                             CefProcessId source_process,
                             CefRefPtr<CefProcessMessage> message);

  void NotifyTakeFocus(CefRefPtr<CefBrowser> browser, bool next);

  void NotifyDragRegion(CefRefPtr<CefBrowser> browser, const std::vector<CefDraggableRegion> regions);

private:
  bool is_closing_;
  bool initial_navigation_;

  std::unordered_map<int, CefRefPtr<CefBrowser>> browser_map_;
  CefViewBrowserClientDelegateInterface::WeakPtr client_delegate_;

  // message router
  CefMessageRouterConfig message_router_config_;
  CefRefPtr<CefViewQueryHandler> cefquery_handler_;
  CefRefPtr<CefMessageRouterBrowserSide> message_router_;

  // resource manager
  CefRefPtr<CefResourceManager> resource_manager_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(CefViewBrowserClient);
};
#endif