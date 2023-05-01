# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("clog")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        clog

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/clog.cpp

        src/main/cpp/jni/managed_jnienv.h
        src/main/cpp/jni/managed_jnienv.cc
        src/main/cpp/jni/comm_function.h
        src/main/cpp/jni/comm_function.cc
        src/main/cpp/jni/scoped_jstring.h
        src/main/cpp/jni/scoped_jstring.cc

        src/main/cpp/ptrbuffer.h
        src/main/cpp/ptrbuffer.cc
        src/main/cpp/autobuffer.h
        src/main/cpp/autobuffer.cc
        src/main/cpp/log_base_buffer.h
        src/main/cpp/log_base_buffer.cc
        src/main/cpp/log_crypt.h
        src/main/cpp/log_crypt.cc
        src/main/cpp/log_magic_num.h

        src/main/cpp/appender.h
        src/main/cpp/appender.cpp

        src/main/cpp/compiler_util.h
        src/main/cpp/xloggerbase.h
        src/main/cpp/xloggerbase.c
        src/main/cpp/xlogger_interface.h
        src/main/cpp/xlogger_interface.cpp
        src/main/cpp/xlogger_category.h
        src/main/cpp/xlogger_category.cc

        src/main/cpp/mapped_file.h
        src/main/cpp/mapped_flle.cpp
        )

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
        clog

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})