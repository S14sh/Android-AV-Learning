# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("android_av_learning")


FILE(GLOB NATIVE_FFMPEG_CPP ${CMAKE_SOURCE_DIR}/interface/*.cpp)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-ffmpeg-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${NATIVE_FFMPEG_CPP})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

include_directories(${CMAKE_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI}/include/)

set(LIBS_DIR ${CMAKE_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI})

add_library(avutil
        SHARED
        IMPORTED)
set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libavutil.so)

add_library(swresample
        SHARED
        IMPORTED)
set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libswresample.so)

add_library(swscale
        SHARED
        IMPORTED)
set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libswscale.so)

add_library(avcodec
        SHARED
        IMPORTED)
set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libavcodec.so)

add_library(avformat
        SHARED
        IMPORTED)
set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libavformat.so)

add_library(avfilter
        SHARED
        IMPORTED)
set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libavfilter.so)

add_library(postproc
        SHARED
        IMPORTED)
set_target_properties(postproc
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libpostproc.so)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-ffmpeg-lib

        avformat avcodec avfilter avutil swresample swscale postproc

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})