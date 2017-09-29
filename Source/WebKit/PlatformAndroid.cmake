list(APPEND WebKit_SOURCES
    ${WEBCORE_DIR}/platform/graphics/android/MediaPlayerPrivateAndroid.cpp

    NetworkProcess/CustomProtocols/curl/LegacyCustomProtocolManagerCurl.cpp

    NetworkProcess/Downloads/android/DownloadAndroid.cpp

    NetworkProcess/curl/NetworkProcessCurl.cpp
    NetworkProcess/curl/NetworkProcessMainCurl.cpp
    NetworkProcess/curl/RemoteNetworkingContextCurl.cpp

    Platform/IPC/unix/AttachmentUnix.cpp
    Platform/IPC/unix/ConnectionUnix.cpp

    Platform/android/LoggingAndroid.cpp
    Platform/android/ModuleAndroid.cpp
    Platform/android/SharedMemoryAndroid.cpp

    Platform/classifier/ResourceLoadStatisticsClassifier.cpp

    Shared/API/c/cairo/WKImageCairo.cpp

    Shared/Authentication/curl/AuthenticationManagerCurl.cpp

    Shared/CoordinatedGraphics/CoordinatedBackingStore.cpp
    Shared/CoordinatedGraphics/CoordinatedGraphicsScene.cpp
    Shared/CoordinatedGraphics/SimpleViewportController.cpp

    Shared/CoordinatedGraphics/coordinatedcompositor/CoordinatedGraphicsArgumentCoders.cpp
    Shared/CoordinatedGraphics/coordinatedcompositor/SharedCoordinatedSurface.cpp

    Shared/android/NativeWebEvent.cpp
    Shared/android/ProcessExecutablePathAndroid.cpp

    Shared/cairo/ShareableBitmapCairo.cpp

    Shared/curl/WebCoreArgumentCodersCurl.cpp

    Shared/unix/ChildProcessMain.cpp

    UIProcess/AcceleratedDrawingAreaProxy.cpp
    UIProcess/BackingStore.cpp
    UIProcess/DefaultUndoController.cpp
    UIProcess/DrawingAreaProxyImpl.cpp
    UIProcess/LegacySessionStateCodingNone.cpp
    UIProcess/WebResourceLoadStatisticsStore.cpp
    UIProcess/WebResourceLoadStatisticsTelemetry.cpp
    UIProcess/WebTextChecker.cpp
    UIProcess/WebTextCheckerClient.cpp

    UIProcess/API/APIPageConfiguration.cpp

    UIProcess/API/android/APIWebsiteDataStoreAndroid.cpp

    UIProcess/Automation/cairo/WebAutomationSessionCairo.cpp

    UIProcess/CoordinatedGraphics/CoordinatedCompositor.cpp
    UIProcess/CoordinatedGraphics/CoordinatedCompositorHost.cpp

    UIProcess/Launcher/android/ProcessLauncherAndroid.cpp
    UIProcess/Launcher/android/ProcessLauncherStrategy.cpp

    UIProcess/WebStorage/StorageManager.cpp

    UIProcess/WebsiteData/android/WebsiteDataStoreAndroid.cpp

    UIProcess/WebsiteData/unix/WebsiteDataStoreUnix.cpp

    UIProcess/android/DLLMain.cpp
    UIProcess/android/DownloadClient.cpp
    UIProcess/android/JNIMain.cpp
    UIProcess/android/NavigationClient.cpp
    UIProcess/android/PageClientImpl.cpp
    UIProcess/android/TextCheckerAndroid.cpp
    UIProcess/android/UIClient.cpp
    UIProcess/android/ViewController.cpp
    UIProcess/android/WebContextMenuProxyAndroid.cpp
    UIProcess/android/WebInspectorProxyAndroid.cpp
    UIProcess/android/WebPageProxyAndroid.cpp
    UIProcess/android/WebPopupMenuProxyAndroid.cpp
    UIProcess/android/WebPreferencesAndroid.cpp
    UIProcess/android/WebProcessPoolAndroid.cpp

    UIProcess/cairo/BackingStoreCairo.cpp

    WebProcess/Cookies/curl/WebCookieManagerCurl.cpp

    WebProcess/InjectedBundle/posix/InjectedBundlePOSIX.cpp

    WebProcess/MediaCache/WebMediaKeyStorageManager.cpp

    WebProcess/WebCoreSupport/WebPasteboardOverrides.cpp

    WebProcess/WebCoreSupport/android/WebContextMenuClientAndroid.cpp
    WebProcess/WebCoreSupport/android/WebErrorsAndroid.cpp
    WebProcess/WebCoreSupport/android/WebPopupMenuAndroid.cpp

    WebProcess/WebCoreSupport/curl/WebFrameNetworkingContext.cpp

    WebProcess/WebPage/AcceleratedDrawingArea.cpp
    WebProcess/WebPage/AcceleratedSurface.cpp
    WebProcess/WebPage/DrawingAreaImpl.cpp

    WebProcess/WebPage/CoordinatedGraphics/AreaAllocator.cpp
    WebProcess/WebPage/CoordinatedGraphics/CompositingCoordinator.cpp
    WebProcess/WebPage/CoordinatedGraphics/CoordinatedCompositorHostProxy.cpp
    WebProcess/WebPage/CoordinatedGraphics/CoordinatedLayerTreeHost.cpp
    WebProcess/WebPage/CoordinatedGraphics/ThreadedCoordinatedLayerTreeHost.cpp
    WebProcess/WebPage/CoordinatedGraphics/UpdateAtlas.cpp

    WebProcess/WebPage/android/AcceleratedSurfaceAndroid.cpp
    WebProcess/WebPage/android/WebInspectorAndroid.cpp
    WebProcess/WebPage/android/WebInspectorUIAndroid.cpp
    WebProcess/WebPage/android/WebPageAndroid.cpp

    WebProcess/android/WebProcessAndroid.cpp
    WebProcess/android/WebProcessMainAndroid.cpp
)

list(REMOVE_ITEM WebKit_SOURCES
    UIProcess/WebTextCheckerClient.cpp

    UIProcess/API/C/WKGrammarDetail.cpp
)

list(APPEND WebKit_MESSAGES_IN_FILES
    UIProcess/CoordinatedGraphics/CoordinatedCompositorHost.messages.in

    WebProcess/WebPage/CoordinatedGraphics/CoordinatedCompositorHostProxy.messages.in
)

list(APPEND WebKit_INCLUDE_DIRECTORIES
    "${DERIVED_SOURCES_DIR}/Interfaces/WebKit"
    "${DERIVED_SOURCES_DIR}/Interfaces/WebCore"
    "${DERIVED_SOURCES_DIR}/Interfaces/WTF"
    "${WEBKIT_DIR}/DatabaseProcess/unix"
    "${WEBKIT_DIR}/NetworkProcess/CustomProtocols/curl"
    "${WEBKIT_DIR}/NetworkProcess/Downloads/curl"
    "${WEBKIT_DIR}/NetworkProcess/android"
    "${WEBKIT_DIR}/NetworkProcess/curl"
    "${WEBKIT_DIR}/NetworkProcess/unix"
    "${WEBKIT_DIR}/Platform/IPC/unix"
    "${WEBKIT_DIR}/Platform/android"
    "${WEBKIT_DIR}/Platform/classifier"
    "${WEBKIT_DIR}/Shared/API/android"
    "${WEBKIT_DIR}/Shared/API/c/android"
    "${WEBKIT_DIR}/Shared/API"
    "${WEBKIT_DIR}/Shared/CoordinatedGraphics"
    "${WEBKIT_DIR}/Shared/CoordinatedGraphics/coordinatedcompositor"
    "${WEBKIT_DIR}/Shared/CoordinatedGraphics/threadedcompositor"
    "${WEBKIT_DIR}/Shared/android"
    "${WEBKIT_DIR}/Shared/curl"
    "${WEBKIT_DIR}/Shared/unix"
    "${WEBKIT_DIR}/UIProcess/API/android"
    "${WEBKIT_DIR}/UIProcess/API"
    "${WEBKIT_DIR}/UIProcess/API/C/cairo"
    "${WEBKIT_DIR}/UIProcess/API/C/android"
    "${WEBKIT_DIR}/UIProcess/API/C/curl"
    "${WEBKIT_DIR}/UIProcess/API/cpp/android"
    "${WEBKIT_DIR}/UIProcess/CoordinatedGraphics"
    "${WEBKIT_DIR}/UIProcess/Launcher/android"
    "${WEBKIT_DIR}/UIProcess/Network/CustomProtocols/curl"
    "${WEBKIT_DIR}/UIProcess/android"
    "${WEBKIT_DIR}/UIProcess/linux"
    "${WEBKIT_DIR}/UIProcess/curl"
    "${WEBKIT_DIR}/WebProcess/android"
    "${WEBKIT_DIR}/WebProcess/curl"
    "${WEBKIT_DIR}/WebProcess/unix"
    "${WEBKIT_DIR}/WebProcess/InjectedBundle/API/android"
    "${WEBKIT_DIR}/WebProcess/WebCoreSupport/android"
    "${WEBKIT_DIR}/WebProcess/WebCoreSupport/curl"
    "${WEBKIT_DIR}/WebProcess/WebPage/CoordinatedGraphics"
    "${WEBKIT_DIR}/android"
    "${WEBKIT_DIR}"
)

# This is needed because of a naming conflict with DiagnosticLoggingClient.h.
# FIXME: Rename one of the DiagnosticLoggingClient headers.
list(REMOVE_ITEM WebKit_INCLUDE_DIRECTORIES
    "${WEBCORE_DIR}/page"
)
list(APPEND WebKit_INCLUDE_DIRECTORIES
    "${WEBCORE_DIR}/page"
)

if (MSVC)
    set(WebKit_LIBRARIES
        PRIVATE WebCore WebCoreDerivedSources
    )
else ()
    set(WebKit_LIBRARIES
        PRIVATE -Wl,--start-group WebCore WebCoreDerivedSources -Wl,--end-group
    )
endif ()

list(APPEND WebKit_LIBRARIES
    PUBLIC  WTF
    PUBLIC  JavaScriptCore

    PRIVATE ${ANGLE_LIBRARIES}

    PUBLIC  ${ANDROID_LIBRARIES}
    PRIVATE ${CAIRO_LIBRARIES}
    PRIVATE ${CURL_LIBRARIES}
    PRIVATE cutils
    PRIVATE dl
    PRIVATE giflib
    PRIVATE hyphenation
    PRIVATE jpeg
    PRIVATE log
    PRIVATE pixman
    PRIVATE png
    PRIVATE safeint
    PRIVATE sqlite
    PRIVATE webp
    PRIVATE xml2
    PRIVATE xslt
    PRIVATE ${ZLIB_LIBRARIES}

    PUBLIC  ${WEBKIT_SYSTEM_LIBRARIES}
)

list(APPEND WebProcess_SOURCES
    WebProcess/EntryPoint/unix/WebProcessMain.cpp
)

list(APPEND NetworkProcess_SOURCES
    NetworkProcess/EntryPoint/unix/NetworkProcessMain.cpp
)

list(APPEND DatabaseProcess_SOURCES
    DatabaseProcess/EntryPoint/unix/DatabaseProcessMain.cpp
)

set(WebKit_ANDROID_SOURCES
    ${WEBCORE_DIR}/platform/graphics/android/SurfaceTextureFactory.java

    Shared/API/android/AWKCallback.java
    Shared/API/android/AWKData.java
    Shared/API/android/AWKError.java
    Shared/API/android/AWKFunction.java
    Shared/API/android/AWKSecurityOrigin.java
    Shared/API/android/AWKURL.java
    Shared/API/android/AWKURLRequest.java
    Shared/API/android/AWKURLResponse.java

    UIProcess/API/android/AWKBackForwardList.java
    UIProcess/API/android/AWKBackForwardListItem.java
    UIProcess/API/android/AWKDownload.java
    UIProcess/API/android/AWKDownloadDelegate.java
    UIProcess/API/android/AWKFrameInfo.java
    UIProcess/API/android/AWKNavigation.java
    UIProcess/API/android/AWKNavigationAction.java
    UIProcess/API/android/AWKNavigationDelegate.java
    UIProcess/API/android/AWKNavigationResponse.java
    UIProcess/API/android/AWKOpenPanelParameters.java
    UIProcess/API/android/AWKPreferences.java
    UIProcess/API/android/AWKPreviewActionItem.java
    UIProcess/API/android/AWKPreviewElementInfo.java
    UIProcess/API/android/AWKProcessPool.java
    UIProcess/API/android/AWKProcessPoolConfiguration.java
    UIProcess/API/android/AWKScriptMessage.java
    UIProcess/API/android/AWKScriptMessageHandler.java
    UIProcess/API/android/AWKUIDelegate.java
    UIProcess/API/android/AWKUserContentController.java
    UIProcess/API/android/AWKUserScript.java
    UIProcess/API/android/AWKUserStyleSheet.java
    UIProcess/API/android/AWKView.java
    UIProcess/API/android/AWKWebContent.java
    UIProcess/API/android/AWKWebContentConfiguration.java
    UIProcess/API/android/AWKWebsiteDataRecord.java
    UIProcess/API/android/AWKWebsiteDataStore.java
    UIProcess/API/android/AWKWindowFeatures.java

    UIProcess/Launcher/android/MessageChannel.java
    UIProcess/Launcher/android/MessageHost.java
    UIProcess/Launcher/android/Messages.java
    UIProcess/Launcher/android/ProcessLauncherBackend.java
    UIProcess/Launcher/android/WebKitProcess.java
    UIProcess/Launcher/android/WebKitProcessLauncher.java
    UIProcess/Launcher/android/WebKitWebProcess1.java
    UIProcess/Launcher/android/WebKitWebProcess2.java
    UIProcess/Launcher/android/WebKitWebProcess3.java
    UIProcess/Launcher/android/WebKitWebProcess4.java
    UIProcess/Launcher/android/WebKitWebProcess5.java
    UIProcess/Launcher/android/WebKitWebProcess6.java
    UIProcess/Launcher/android/WebKitWebProcess7.java
    UIProcess/Launcher/android/WebKitWebProcess8.java

    UIProcess/android/API.java
    UIProcess/android/AbstractMethod.java
    UIProcess/android/AccessedByNative.java
    UIProcess/android/CallByReference.java
    UIProcess/android/CalledByNative.java
    UIProcess/android/CompositionClause.java
    UIProcess/android/ContainerView.java
    UIProcess/android/LogHelper.java
    UIProcess/android/MotionDetector.java
    UIProcess/android/NativeConstructor.java
    UIProcess/android/NativeDestructor.java
    UIProcess/android/NativeExportMacro.java
    UIProcess/android/NativeNamespace.java
    UIProcess/android/NativeObjectField.java
    UIProcess/android/SurfaceTextureFactoryHost.java
    UIProcess/android/VirtualKeyMap.java
    UIProcess/android/WKPrivate.java
    UIProcess/android/WebEditable.java
    UIProcess/android/WebEditableContext.java
    UIProcess/android/WebEditableInputConnection.java
    UIProcess/android/WebEventEnumerations.java
    UIProcess/android/WebProcessContext.java
    UIProcess/android/WebRenderContext.java
    UIProcess/android/WebRenderContextList.java
    UIProcess/android/WebRenderer.java
    UIProcess/android/WebView.java
    UIProcess/android/WebViewContext.java
    UIProcess/android/WebViewPrivate.java
)

add_definitions(-DBUILDING_ANDROID_PACKAGE -DANDROID_EXTERN=WTF_EXPORT)

add_custom_target(GenerateForwardingHeadersForWebKit
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${WEBKIT_DIR} --output ${DERIVED_SOURCES_DIR}/ForwardingHeaders --platform android --platform CoordinatedGraphics --platform curl
)

set(WEBKIT_EXTRA_DEPENDENCIES
    GenerateForwardingHeadersForWebKit
)

set_target_properties(GenerateForwardingHeadersForWebKit PROPERTIES FOLDER "WebKit")
add_dependencies(GenerateForwardingHeadersForWebKit WebCore)

set(DISABLE_WEBKIT_PROCESS_EXECUTABLES TRUE)

WEBKIT_ANDROID_FRAMEWORK(WebKit)

if (ANDROID)
    get_property(WEBKITLIB_SOURCES GLOBAL PROPERTY WEBKIT_ANDROID_FRAMEWORK_SOURCES)
    add_jar(WebKitLib
        SOURCES ${WEBKITLIB_SOURCES}
        OUTPUT_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )
endif ()
