set(MiniBrowser_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}/PackageHeaders"
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/Source"
    "${CMAKE_SOURCE_DIR}/Source/WTF"
)

set(MiniBrowser_SOURCES
    AppMain.cpp
    MiniBrowser.cpp
)

set(MiniBrowser_LIBRARIES
    WebKit
)

include_directories(${MiniBrowser_INCLUDE_DIRECTORIES})

if (ANDROID)
    add_custom_target(MiniBrowser ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_NDK_ABI_NAME}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/WebKit2Lib.jar" "${CMAKE_CURRENT_SOURCE_DIR}/libs/WebKit2Lib.jar"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libWebKit2.so" "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_NDK_ABI_NAME}/libWebKit2.so"
        COMMAND ${ANDROID_SDK_TOOL} update project --path ${CMAKE_CURRENT_SOURCE_DIR} --target android-${CMAKE_ANDROID_API} --name MiniBrowser
        COMMAND ${ANDROID_ANT_TOOL} debug && ${ANDROID_ANT_TOOL} release
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
elseif (WIN32)
    add_definitions(-D_UNICODE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB /SUBSYSTEM:CONSOLE")
    add_executable(MiniBrowser WIN32 ${MiniBrowser_SOURCES})
endif ()

add_dependencies(MiniBrowser WebKit)
if (NOT ANDROID)
    target_link_libraries(MiniBrowser ${MiniBrowser_LIBRARIES})
    set_target_properties(MiniBrowser PROPERTIES OUTPUT_NAME "MiniBrowser")
endif ()
