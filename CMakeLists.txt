cmake_minimum_required(VERSION 3.26)
cmake_policy(SET CMP0141 NEW)

# ------------------
if (MSVC)
    add_compile_options("$<$<CONFIG:Debug>:/JMC>")

    add_link_options("/CETCOMPAT")
    add_compile_options(
        "$<$<COMPILE_LANGUAGE:CXX,C>:/bigobj>"
        "$<$<COMPILE_LANGUAGE:CXX,C>:/MP>"
        "$<$<COMPILE_LANGUAGE:CXX,C>:/we4996;/we4099>"
        "$<$<COMPILE_LANGUAGE:CXX,C>:/utf-8>"
        "$<$<COMPILE_LANGUAGE:CXX,C>:/Zc:preprocessor>"
        "$<$<COMPILE_LANGUAGE:CXX,C>:/EHsc>"
        "$<$<COMPILE_LANGUAGE:CXX,C>:/Zc:__cplusplus>"
    )
    string(REPLACE "/Ob1" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "/Ob2" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    add_compile_options("$<$<NOT:$<CONFIG:Debug>>:/Ob3;/Zc:inline>")
    
    add_link_options("$<$<CONFIG:Release>:/DEBUG;/OPT:REF;/OPT:ICF>")
    add_compile_options("/arch:AVX")
endif()

# ----------------
if (WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX -DUNICODE -D_UNICODE -D_CRT_SECURE_NO_WARNINGS)
endif()

# ----------------------
if(NOT DEFINED ENV{VCPKG_ROOT})
    set(ENV{VCPKG_ROOT} ${CMAKE_CURRENT_LIST_DIR}/deps/vcpkg)
endif()
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

include(GoogleTest)
enable_testing()

project(jaxaoc2025 LANGUAGES C CXX VERSION 0.1.0)

find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(GEOS CONFIG REQUIRED)

file(GLOB_RECURSE RAWR_SOURCES CONFIGURE_DEPENDS "day*.cpp")

add_executable(${PROJECT_NAME} ${RAWR_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
# --------------------
set_target_properties(${PROJECT_NAME}
                        PROPERTIES
                            CXX_STANDARD 20
                            CXX_STANDARD_REQUIRED ON
                            CXX_EXTENSIONS OFF
                            INSTALL_RPATH_USE_LINK_PATH TRUE
                            POSITION_INDEPENDENT_CODE ON
                            CXX_VISIBILITY_PRESET hidden
                            COMPILE_WARNING_AS_ERROR ON)
if(NOT DEFINED CMAKE_MSVC_DEBUG_INFORMATION_FORMAT)
    set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_DEBUG_INFORMATION_FORMAT ProgramDatabase)
endif()
if(NOT DEFINED CMAKE_INTERPROCEDURAL_OPTIMIZATION)
    set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
endif()
#------
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(${PROJECT_NAME} PRIVATE GEOS::geos)
#gtest_discover_tests(${PROJECT_NAME} DISCOVERY_TIMEOUT 30)
