cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

# Vcpkg path. Not to set to envirment paths.
set(VCPKG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/vcpkg)

# Compile settings
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20) # C++20...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

# Build type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Update compiler settings.
if(MSVC)
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
	if (CMAKE_BUILD_TYPE EQUAL "Debug")
		set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
	else()
		set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "")
		# Release ビルドの時は"/MT" でビルド.
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	# Update if necessary
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
	# この指定が多分どのライブラリも使えるはず
	set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")
endif()

# Use vcpkg toolchain file
# 公式だとENV{} で環境変数のVCPKG を参照するが、プロジェクトのやつを参照したいのでそのまま参照.
if(DEFINED VCPKG_ROOT AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
	message(STATUS "Set CMAKE_TOOLCHAIN_FILE to vcpkg.cmake")
endif()
# 結局のところこっちじゃないとライブラリ見つけられなくてエラー吐く.
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# Set project settings
set(PROJECT_NAME tcp-echo-server)    # TODO:Project Name.
project(${PROJECT_NAME} VERSION  0.0.1)	# TODO:Project Version.

## project function set PROJECT_SOURCE_DIR to CMakefile.txt directory, so update path
set(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Boost settings.
if(NOT CMAKE_BUILD_TYPE EQUAL "Debug")
	set(Boost_USE_STATIC_LIBS        ON) # only find static libs
	set(Boost_USE_MULTITHREADED      ON)
	set(Boost_USE_STATIC_RUNTIME    OFF)
endif()

# Find libraries
# boost は最後に探しにいかないとCOMPONENTS に指定したやつが無視される(他のBoost を参照しているライブラリでFindBoost 呼ばれてBoost_LIBRARIES の設定上書きされてしまうような感じ？).
find_package(Boost COMPONENTS system thread REQUIRED)	# https://cmake.org/cmake/help/git-master/module/FindBoost.html
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

# Input files
file(GLOB_RECURSE CXX_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE H_FILES ${PROJECT_SOURCE_DIR}/*.h)

source_group("Header Files" FILES ${H_FILES})
source_group("Source Files" FILES ${CXX_FILES})

# Compile
add_executable(${PROJECT_NAME}
	${H_FILES}
	${CXX_FILES}
)

# Include
set(INCS
	${Boost_INCLUDE_DIRS}
    # ${OpenCV_INCLUDE_DIRS}
)
include_directories(${INCS})

# Link libraries
set(LIBS
	${Boost_LIBRARIES}
)

# include_directories(${Boost_INCLUDE_DIRS})

# static link
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_USER_PROPS ${VCPKG_ROOT}/scripts/buildsystems/msbuild/vcpkg.targets)
endif()
