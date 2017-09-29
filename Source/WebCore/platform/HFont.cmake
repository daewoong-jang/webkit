list(APPEND WebCore_INCLUDE_DIRECTORIES
    "${WEBCORE_DIR}/ForwardingHeaders"
    "${WEBCORE_DIR}/platform/win"
    "${WEBCORE_DIR}/platform/graphics/win"
    "${THIRDPARTY_DIR}/ANGLE/include"
    "${THIRDPARTY_DIR}/ANGLE/include/EGL"
)

list(APPEND WebCore_SOURCES
    platform/graphics/FontPlatformData.cpp

    platform/graphics/harfbuzz/ComplexTextControllerHarfBuzz.cpp

    platform/graphics/opentype/OpenTypeUtilities.cpp

    platform/graphics/win/FontCacheWin.cpp
    platform/graphics/win/FontCustomPlatformDataCairo.cpp
    platform/graphics/win/FontPlatformDataCairoWin.cpp
    platform/graphics/win/FontPlatformDataWin.cpp
    platform/graphics/win/FontWin.cpp
    platform/graphics/win/GlyphPageTreeNodeCairoWin.cpp
    platform/graphics/win/SimpleFontDataCairoWin.cpp
    platform/graphics/win/SimpleFontDataWin.cpp
    platform/graphics/win/UniscribeController.cpp

    platform/win/GDIUtilities.cpp
)
