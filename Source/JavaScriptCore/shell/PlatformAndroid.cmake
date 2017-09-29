if (WIN32)
    add_definitions(-DWTF_PLATFORM_ANDROID=1 -DJSStringRefCF_h)

    include(PlatformWin.cmake)
    
    target_link_libraries(jscLib ${WEBKIT_SYSTEM_LIBRARIES})
    target_link_libraries(testRegExpLib ${WEBKIT_SYSTEM_LIBRARIES})
    target_link_libraries(testapiLib ${WEBKIT_SYSTEM_LIBRARIES})
    target_link_libraries(testmasmLib ${WEBKIT_SYSTEM_LIBRARIES})
endif ()
