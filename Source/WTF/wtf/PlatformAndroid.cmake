list(APPEND WTF_SOURCES
    UniStdExtras.cpp

    android/DispatchQueue.cpp
    android/MemoryFootprintAndroid.cpp
    android/MemoryPressureHandlerAndroid.cpp
    android/RunLoopAndroid.cpp
    android/ThreadLoop.cpp
    android/WorkQueueAndroid.cpp

    text/android/TextBreakIteratorInternalICU.cpp
)

if (WIN32)
    list(APPEND WTF_SOURCES
        win/CPUTimeWin.cpp
        win/LanguageWin.cpp
        win/MemoryFootprintWin.cpp
    )
else ()
    list(APPEND WTF_SOURCES
        unix/CPUTimeUnix.cpp
        unix/LanguageUnix.cpp
    )
endif ()

set(WTF_LIBRARIES
    PUBLIC ${WTF_LIBRARIES}
)

list(APPEND WTF_LIBRARIES
    PRIVATE ${ANDROID_LIBRARIES}
    PRIVATE log
    PRIVATE safeint
    PRIVATE ${ZLIB_LIBRARIES}
    PRIVATE ${WEBKIT_SYSTEM_LIBRARIES}
)

list(APPEND WTF_INCLUDE_DIRECTORIES
    "${WTF_DIR}/wtf/android"
    "${DERIVED_SOURCES_DIR}/WTF"
    "${DERIVED_SOURCES_DIR}/Interfaces/WTF"
)

set(WTF_ANDROID_SOURCES
    android/WTFMainThread.java
    android/ThreadLoopBackend.java
)

WEBKIT_ANDROID_FRAMEWORK(WTF)
