# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
set(APP_STL "c++_shared")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall")
set(GLM_HOME "/usr/include/GL/glm")

include_directories(${GLM_HOME})


add_library(
        # Sets the name of the library.
        lesson-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        # common
        # src/main/cpp/AbstractSurfaceRendererNative.h
        # src/main/cpp/AbstractSurfaceRendererNative.cpp

        # graphics
        src/main/cpp/graphics/GLincludes.h
        src/main/cpp/graphics/GLUtils.h
        src/main/cpp/graphics/GLUtils.cpp
        src/main/cpp/graphics/Matrix.h
        src/main/cpp/graphics/Matrix.cpp
        src/main/cpp/graphics/Camera.h
        src/main/cpp/graphics/Camera.cpp
        src/main/cpp/graphics/ShaderClass.java.h
        src/main/cpp/graphics/ShaderClass.java.cpp
        src/main/cpp/graphics/PointLight.h
        src/main/cpp/graphics/PointLight.cpp
        src/main/cpp/graphics/Material.h
        src/main/cpp/graphics/Material.cpp
        src/main/cpp/graphics/Model.h
        src/main/cpp/graphics/Model.cpp
        src/main/cpp/graphics/transform.h

        # utils
        src/main/cpp/utils/Logger.h
        #src/main/cpp/utils/Logger.cpp
        src/main/cpp/utils/Scene.cpp
        src/main/cpp/utils/Scene.h

        #quantized color
        src/main/cpp/QuantizedColor/QuantizedColorNative.cpp
        src/main/cpp/QuantizedColor/QuantizedColorNative.h

        #bricks
        src/main/cpp/BricksShader/BricksShaderNativeRenderer.cpp
        src/main/cpp/BricksShader/BricksShaderNativeRenderer.h

        #Julia
        src/main/cpp/JuliaSet/JuliaSetRenderer.cpp
        src/main/cpp/JuliaSet/JuliaSetRenderer.h

        #polyhedrons
        src/main/cpp/Polyhedrons/PolyhedronsRenderer.cpp
        src/main/cpp/Polyhedrons/PolyhedronsRenderer.h
        #src/main/cpp/Polyhedrons/Polyhedron.h
        #src/main/cpp/Polyhedrons/Polyhedron.cpp
        #src/main/cpp/Polyhedrons/Tetrahedron.h
        #src/main/cpp/Polyhedrons/Tetrahedron.cpp
        #src/main/cpp/Polyhedrons/TestCube.h
        #src/main/cpp/Polyhedrons/TestCube.cpp
        #src/main/cpp/Polyhedrons/Cube.h
        #src/main/cpp/Polyhedrons/Cube.cpp
        #src/main/cpp/Polyhedrons/Octahedron.h
        #src/main/cpp/Polyhedrons/Octahedron.cpp
        #src/main/cpp/Polyhedrons/Dodecahedron.h
        #src/main/cpp/Polyhedrons/Dodecahedron.cpp
        #src/main/cpp/Polyhedrons/Icosahedron.h
        #src/main/cpp/Polyhedrons/Icosahedron.cpp

        #src/main/cpp/graphics/PerlinNoiseGenerator.h
        #src/main/cpp/graphics/PerlinNoiseGenerator.cpp
)

include_directories(src/main/cpp/)


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(log-lib log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        lesson-lib
        EGL
        GLESv2
        GLESv3
        android
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} )