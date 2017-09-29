list(APPEND PAL_SOURCES
    system/ClockGeneric.cpp
    system/Sound.cpp

    text/KillRing.cpp
)

if (ANDROID)
    list(APPEND PAL_SOURCES
        crypto/android/CryptoDigestAndroid.cpp
    )
elseif (WIN32)
    list(APPEND PAL_SOURCES
        crypto/win/CryptoDigestWin.cpp
    )
endif ()
