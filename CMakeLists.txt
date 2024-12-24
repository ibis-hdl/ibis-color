################################################################################
## IBIS/color project
##
## file: source/color/CMakeLists.txt
################################################################################

project(ibis_color
    VERSION "${ibis_version}"
    DESCRIPTION "${ibis_description}"
    HOMEPAGE_URL "${ibis_homepage}"
    LANGUAGES CXX
)
message(STATUS
    "project ${CMAKE_PROJECT_NAME}/${PROJECT_NAME} @v${ibis_version}: "
    "${ibis_description} [${ibis_homepage}]")


add_library(${PROJECT_NAME})
add_library(ibis::color ALIAS ${PROJECT_NAME})


target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ibis::util
        Boost::headers
)


target_sources(${PROJECT_NAME}
    PRIVATE
        src/color.cpp
        $<$<BOOL:${WIN32}>:src/color_platform_windows.cpp>
)


if(IBIS_ENABLE_CXXSTD_PCH)
    target_precompile_headers(${PROJECT_NAME} REUSE_FROM ibis_pch)
endif()


target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
