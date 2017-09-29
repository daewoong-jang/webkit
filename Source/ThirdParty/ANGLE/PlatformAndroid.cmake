if (WIN32)
    include(PlatformWin.cmake)
    
    list(APPEND ANGLEGLESv2_LIBRARIES
        ${WEBKIT_SYSTEM_LIBRARIES}
    )
endif ()

include_directories(BEFORE
    include
    include/KHR
)
