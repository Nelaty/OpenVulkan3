cmake_minimum_required(VERSION 3.16)
#include("cmake/HunterGate.cmake")

#HunterGate(
#        URL "https://github.com/cpp-pm/hunter/archive/v0.23.296.tar.gz"
#        SHA1 "232f5022ee1d45955a7e8d3e1720f31bac1bb534"
#)
project(OpenVulkan3)

set(PROJECT_SHORT_NAME OVU)


###########################################################
# CMAKE GENERAL CONFIG
###########################################################
set(BUILD_SYSTEM_APPLE 0)
set(BUILD_SYSTEM_WINDOWS 0)
set(BUILD_SYSTEM_LINUX 0)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    set(BUILD_SYSTEM_WINDOWS 1)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "APPLE")
    set(BUILD_SYSTEM_APPLE 1)
else()
    set(BUILD_SYSTEM_LINUX 1)
endif()

###########################################################
# USER OPTIONS
###########################################################
set(OVU_BUILD_SAMPLES 1)
set(OVU_BUILD_TESTS 1)

###########################################################
# DEVELOPER OPTIONS
###########################################################
set(OVU_WARNINGS_ALL 1)
set(OVU_WARNINGS_ANNOYING 0)
set(OVU_WARNINGS_AS_ERRORS 0)

###########################################################
# CONFIGURATION FILE
###########################################################
set(OVU_VERSION_MAJOR "0")
set(OVU_VERSION_MINOR "0")
set(OVU_VERSION_PATCH "1")

set(OVU_CONFIG_PATH "${CMAKE_SOURCE_DIR}/ovu/include/ovu/config/Ovu3Config.h")
configure_file("${OVU_CONFIG_PATH}.in"
        "${OVU_CONFIG_PATH}"
        @ONLY)

###########################################################
# COMPILER CONFIGURATION
###########################################################
if(${BUILD_SYSTEM_WINDOWS})
    if(${OVU_WARNINGS_ALL})
        set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} /W3")
        if(${OVU_WARNINGS_ANNOYING})
            set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} /W4")
        else()
            set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS}")
        endif()
        if(${OVU_WARNINGS_AS_ERRORS})
            set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} /WX")
        endif()
    endif()
elseif(${BUILD_SYSTEM_APPLE})
    # TBD
else()
    if(${OVU_WARNINGS_ALL})
        set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -Wall -Wpedantic")
        if(${OVU_WARNINGS_ANNOYING})
            set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -Wextra")
        else()
            set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
        endif()
        if(${OVU_WARNINGS_AS_ERRORS})
            set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -Werror")
        endif()
    endif()
endif()

set(CMAKE_CXX_STANDARD 20)

###########################################################
# OUTPUT PATHS
###########################################################

set(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/bin)
set(${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/bin)
set(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/lib)


###########################################################
# GLOBAL PATHS
###########################################################
set(OVU_RESOURCE_DIR ${CMAKE_SOURCE_DIR}/res)
set(OVU_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ovu/include/)



###########################################################
# DEPENDENCIES
###########################################################
# Hunter config

#hunter_add_package(glm)
#find_package(Clara REQUIRED)

#hunter_add_package(spdlog)
#find_package(spdlog REQUIRED)

#hunter_add_package(glfw)
#find_package(glfw3 REQUIRED)

add_subdirectory(deps)

find_package(Vulkan REQUIRED)
set(Vulkan_LIBRARIES Vulkan::Vulkan)


# Dependency variables
set(OVU_DEPENDENCY_INCLUDE_DIR ${VULKAN_INCLUDE_DIR})
set(OVU_COMMON_LIBRARIES
        glm
        fmt
        spdlog
        #json
        #Clara
        )

if(${BUILD_SYSTEM_WINDOWS})
    set(OVU_DEPENDENCY_LIBRARIES
            glfw3
            ${Vulkan_LIBRARIES}
            ${OVU_COMMON_LIBRARIES}
            )
elseif(${BUILD_SYSTEM_APPLE})
    set(OVU_DEPENDENCY_LIBRARIES
            glfw
            ${Vulkan_LIBRARIES}
            ${OVU_COMMON_LIBRARIES}
            )
else()
    set(OVU_DEPENDENCY_LIBRARIES
            glfw
            dl
            pthread
            X11
            Xxf86vm
            Xrandr
            Xi
            ${Vulkan_LIBRARIES}
            ${OVU_COMMON_LIBRARIES}
            )
endif()

###########################################################
# LIBRARY DEPENDENCIES
###########################################################
set(OVU_ALL_INCLUDES ${OVU_INCLUDE_DIR} ${OVU_DEPENDENCY_INCLUDE_DIR})
set(OVU_ALL_LIBRARIES ${OVU_DEPENDENCY_LIBRARIES} ${PROJECT_NAME})


###########################################################
# SUBPROJECTS
###########################################################
add_subdirectory(ovu)
if(${OVU_BUILD_SAMPLES})
    add_subdirectory(example)
endif()

###########################################################
# TESTING
###########################################################
if(${OVU_BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
endif()
