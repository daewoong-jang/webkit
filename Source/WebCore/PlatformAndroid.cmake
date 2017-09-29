include(platform/Cairo.cmake)
include(platform/Curl.cmake)
include(platform/ImageDecoders.cmake)
include(platform/TextureMapper.cmake)

if (USE_FREETYPE)
    include(platform/FreeType.cmake)
elseif (WIN32)
    include(platform/HFont.cmake)
endif ()

list(APPEND WebCore_INCLUDE_DIRECTORIES
    "${DERIVED_SOURCES_DIR}/Interfaces/WebCore"
    "${DERIVED_SOURCES_JAVASCRIPTCORE_DIR}"
    "${DERIVED_SOURCES_JAVASCRIPTCORE_DIR}/inspector"
    "${THIRDPARTY_DIR}/ANGLE"
    "${THIRDPARTY_DIR}/ANGLE/include/KHR"
    "${WEBCORE_DIR}/page/android"
    "${WEBCORE_DIR}/platform/android"
    "${WEBCORE_DIR}/platform/graphics/android"
    "${WEBCORE_DIR}/platform/graphics/egl"
    "${WEBCORE_DIR}/platform/graphics/opengl"
    "${WEBCORE_DIR}/platform/graphics/surfaces"
    "${WEBCORE_DIR}/platform/graphics/surfaces/android"
    "${WEBCORE_DIR}/platform/graphics/surfaces/egl"
    "${WEBCORE_DIR}/platform/network/android"
    "${ANDROID_SYSROOT}/usr/include/EGL"
)

list(APPEND WebCore_SOURCES
    editing/android/EditorAndroid.cpp

    page/PageDebuggable.cpp

    page/android/EventHandlerAndroid.cpp
    page/android/SettingsAndroid.cpp

    platform/Cursor.cpp
    platform/LocalizedStrings.cpp

    platform/android/CursorAndroid.cpp
    platform/android/ErrorsAndroid.cpp
    platform/android/EventLoopAndroid.cpp
    platform/android/FileSystemAndroid.cpp
    platform/android/ImageAndroid.cpp
    platform/android/KeyboardUtilitiesAndroid.cpp
    platform/android/KeyedDecoderAndroid.cpp
    platform/android/KeyedEncoderAndroid.cpp
    platform/android/KeyedVariant.cpp
    platform/android/LocalizedStringsAndroid.cpp
    platform/android/LoggingAndroid.cpp
    platform/android/MIMETypeRegistryAndroid.cpp
    platform/android/MainThreadSharedTimerAndroid.cpp
    platform/android/PasteboardAndroid.cpp
    platform/android/PlatformKeyboardEventAndroid.cpp
    platform/android/PlatformScreenAndroid.cpp
    platform/android/ScrollbarThemeAndroid.cpp
    platform/android/SSLKeyGeneratorAndroid.cpp
    platform/android/TemporaryLinkStubs.cpp
    platform/android/UserAgentAndroid.cpp
    platform/android/WidgetAndroid.cpp

    platform/graphics/GLContext.cpp
    platform/graphics/GraphicsContext3DPrivate.cpp
    platform/graphics/ImageSource.cpp
    platform/graphics/OpenGLShims.cpp
    platform/graphics/PlatformDisplay.cpp
    platform/graphics/WOFFFileFormat.cpp

    platform/graphics/android/GLContextEGLAndroid.cpp

    platform/graphics/cpu/arm/filters/FELightingNEON.cpp

    platform/graphics/egl/GLContextEGL.cpp

    platform/graphics/gpu/Texture.cpp
    platform/graphics/gpu/TilingData.cpp

    platform/graphics/opengl/Extensions3DOpenGL.cpp
    platform/graphics/opengl/Extensions3DOpenGLCommon.cpp
    platform/graphics/opengl/Extensions3DOpenGLES.cpp
    platform/graphics/opengl/GraphicsContext3DOpenGLCommon.cpp
    platform/graphics/opengl/GraphicsContext3DOpenGLES.cpp
    platform/graphics/opengl/TemporaryOpenGLSetting.cpp

    platform/image-encoders/JPEGImageEncoder.cpp

    platform/network/NetworkStorageSessionStub.cpp
    platform/network/SynchronousLoaderClient.cpp

    platform/network/curl/NetworkStorageSessionCurl.cpp

    platform/posix/FileSystemPOSIX.cpp
    platform/posix/SharedBufferPOSIX.cpp

    platform/text/Hyphenation.cpp
    platform/text/LocaleICU.cpp

    rendering/RenderThemeAndroid.cpp
)

list(APPEND WebCore_USER_AGENT_STYLE_SHEETS
    ${WEBCORE_DIR}/Modules/mediacontrols/mediaControlsBase.css
)

set(WebCore_USER_AGENT_SCRIPTS
    ${WEBCORE_DIR}/English.lproj/mediaControlsLocalizedStrings.js
    ${WEBCORE_DIR}/Modules/mediacontrols/mediaControlsBase.js
)

add_custom_command(
    OUTPUT ${DERIVED_SOURCES_WEBCORE_DIR}/WebKitVersion.h
    MAIN_DEPENDENCY ${WEBKITLEGACY_DIR}/scripts/generate-webkitversion.pl
    DEPENDS ${WEBKITLEGACY_DIR}/mac/Configurations/Version.xcconfig
    COMMAND ${PERL_EXECUTABLE} ${WEBKITLEGACY_DIR}/scripts/generate-webkitversion.pl --config ${WEBKITLEGACY_DIR}/mac/Configurations/Version.xcconfig --outputDir ${DERIVED_SOURCES_WEBCORE_DIR}
    VERBATIM)
list(APPEND WebCore_SOURCES ${DERIVED_SOURCES_WEBCORE_DIR}/WebKitVersion.h)

set(WebCore_USER_AGENT_SCRIPTS_DEPENDENCIES ${WEBCORE_DIR}/rendering/RenderThemeAndroid.cpp)

set(WebCore_OUTPUT_NAME
    WebCore${DEBUG_SUFFIX}
)

set(WebCore_ANDROID_SOURCES
    platform/android/WebCorePlatform.java

    platform/graphics/android/WebMediaPlayer.java
)

WEBKIT_ANDROID_FRAMEWORK(WebCore)
