cmake_minimum_required(VERSION 3.16)
project(json-c VERSION 0.1.0 LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(JSONC_BUILD_SHARED "Build shared library" ON)
option(JSONC_BUILD_STATIC "Build static library" ON)
option(JSONC_BUILD_TESTS "Build tests" ON)
option(JSONC_BUILD_EXAMPLES "Build examples" ON)
option(JSONC_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(JSONC_BUILD_DOCS "Build docs" OFF)
option(JSONC_BUILD_CPP "Build C++ wrapper" OFF)
option(JSONC_ENABLE_INSTALL "Enable install rules" ON)
option(JSONC_ENABLE_WARNINGS "Enable warnings" ON)
option(JSONC_ENABLE_THREADS "Enable thread support" OFF)
option(JSONC_BUILD_MINIMAL "Build minimal configuration" OFF)

set(JSONC_PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(JSONC_SOURCES src/jsonc.c)

if(JSONC_BUILD_SHARED)
    add_library(jsonc_shared SHARED ${JSONC_SOURCES})
    target_include_directories(jsonc_shared PUBLIC $<BUILD_INTERFACE:${JSONC_PUBLIC_INCLUDE_DIR}> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
    set_target_properties(jsonc_shared PROPERTIES OUTPUT_NAME jsonc)
    if(MSVC)
        set_target_properties(jsonc_shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
    if(JSONC_ENABLE_WARNINGS)
        target_compile_options(jsonc_shared PRIVATE $<$<C_COMPILER_ID:MSVC>:/W4> $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>)
    endif()
endif()

if(JSONC_BUILD_STATIC)
    add_library(jsonc_static STATIC ${JSONC_SOURCES})
    target_include_directories(jsonc_static PUBLIC $<BUILD_INTERFACE:${JSONC_PUBLIC_INCLUDE_DIR}> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
    set_target_properties(jsonc_static PROPERTIES OUTPUT_NAME jsonc-static)
    if(JSONC_ENABLE_WARNINGS)
        target_compile_options(jsonc_static PRIVATE $<$<C_COMPILER_ID:MSVC>:/W4> $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>)
    endif()
endif()

if(JSONC_BUILD_SHARED AND JSONC_BUILD_STATIC)
    add_library(jsonc::jsonc ALIAS jsonc_shared)
elseif(JSONC_BUILD_SHARED)
    add_library(jsonc::jsonc ALIAS jsonc_shared)
elseif(JSONC_BUILD_STATIC)
    add_library(jsonc::jsonc ALIAS jsonc_static)
endif()

if(JSONC_BUILD_TESTS)
    enable_testing()
    add_executable(jsonc_tests tests/test_jsonc.c)
    target_link_libraries(jsonc_tests PRIVATE jsonc::jsonc)
    target_include_directories(jsonc_tests PRIVATE ${JSONC_PUBLIC_INCLUDE_DIR})
    add_test(NAME jsonc_tests COMMAND jsonc_tests)
endif()

if(JSONC_BUILD_EXAMPLES)
    add_executable(jsonc_basic examples/basic.c)
    target_link_libraries(jsonc_basic PRIVATE jsonc::jsonc)
    target_include_directories(jsonc_basic PRIVATE ${JSONC_PUBLIC_INCLUDE_DIR})

    add_executable(jsonc_jsonl examples/jsonl.c)
    target_link_libraries(jsonc_jsonl PRIVATE jsonc::jsonc)
    target_include_directories(jsonc_jsonl PRIVATE ${JSONC_PUBLIC_INCLUDE_DIR})

    add_executable(jsonc_fileops examples/fileops.c)
    target_link_libraries(jsonc_fileops PRIVATE jsonc::jsonc)
    target_include_directories(jsonc_fileops PRIVATE ${JSONC_PUBLIC_INCLUDE_DIR})
endif()

if(JSONC_ENABLE_INSTALL)
    if(JSONC_BUILD_SHARED)
        install(TARGETS jsonc_shared EXPORT jsoncTargets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
    endif()
    if(JSONC_BUILD_STATIC)
        install(TARGETS jsonc_static EXPORT jsoncTargets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
    endif()
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/json-c-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/json-c-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/json-c-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/json-c)

    install(EXPORT jsoncTargets
        NAMESPACE jsonc::
        FILE json-c-targets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/json-c)

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/json-c-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/json-c-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/json-c)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/jsonc.pc.in ${CMAKE_CURRENT_BINARY_DIR}/jsonc.pc @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jsonc.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()
