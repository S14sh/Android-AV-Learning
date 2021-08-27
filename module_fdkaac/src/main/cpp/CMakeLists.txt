# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("android_av_learning")

set(LIBS_DIR ${CMAKE_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI})

FILE(GLOB NATIVE_FDKAAC_LIB_CPP ${CMAKE_SOURCE_DIR}/interface/*.cpp)
FILE(GLOB NATIVE_FDKAAC_LIB_H ${CMAKE_SOURCE_DIR}/interface/*.h)
FILE(GLOB ADTS_HEAD_CPP ${CMAKE_SOURCE_DIR}/adtshead/*.cpp)
FILE(GLOB ADTS_HEAD_H ${CMAKE_SOURCE_DIR}/adtshead/*.h)
FILE(GLOB AAC_ENCODER_CPP ${CMAKE_SOURCE_DIR}/aacencoder/*.cpp)
FILE(GLOB AAC_ENCODER_H ${CMAKE_SOURCE_DIR}/aacencoder/*.h)
FILE(GLOB AAC_DECODER_CPP ${CMAKE_SOURCE_DIR}/aacdecoder/*.cpp)
FILE(GLOB AAC_DECODER_H ${CMAKE_SOURCE_DIR}/aacdecoder/*.h)

include_directories(${CMAKE_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI}/include/)
include_directories(${CMAKE_SOURCE_DIR}/adtshead/)
include_directories(${CMAKE_SOURCE_DIR}/aacencoder/)
include_directories(${CMAKE_SOURCE_DIR}/aacdecoder/)
#include_directories(${NATIVE_FDKAAC_LIB_H})
#include_directories(${ADTS_HEAD_H})
#include_directories(${AAC_ENCODER_H})
#include_directories(${AAC_DECODER_H})

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-fdkaac-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${NATIVE_FDKAAC_LIB_CPP}
        ${ADTS_HEAD_CPP}
        ${AAC_ENCODER_CPP}
        ${AAC_DECODER_CPP}
        )


add_library(fdk-aac STATIC IMPORTED)
set_target_properties(fdk-aac PROPERTIES IMPORTED_LOCATION ${LIBS_DIR}/libfdk-aac.a)

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

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-fdkaac-lib

        fdk-aac

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
