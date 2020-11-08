# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
set(APP_STL "c++_shared")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -Wall")
set(GLM_HOME "/usr/include/glm")

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
        src/main/cpp/graphics/PerlinNoiseGenerator.h
        src/main/cpp/graphics/PerlinNoiseGenerator.cpp
        src/main/cpp/graphics/lights/PointLight.h
        src/main/cpp/graphics/lights/PointLight.cpp
        src/main/cpp/graphics/Material.h
        src/main/cpp/graphics/Material.cpp
        #src/main/cpp/graphics/Models/ModelNode.h
        #src/main/cpp/graphics/Models/ModelNode.cpp
        #src/main/cpp/graphics/Models/Model.h
        #src/main/cpp/graphics/Models/Model.cpp
        src/main/cpp/graphics/Models/HardCodedModel.h
        src/main/cpp/graphics/Models/HardCodedModel.cpp
        src/main/cpp/graphics/transform.h
        src/main/cpp/graphics/VertexLayout.h
        src/main/cpp/graphics/VertexLayout.cpp
        src/main/cpp/graphics/Geometry.h
        src/main/cpp/graphics/Geometry.cpp

        # utils
        src/main/cpp/utils/Logger.h
        #src/main/cpp/utils/Logger.cpp
        src/main/cpp/utils/Scene.cpp
        src/main/cpp/utils/Scene.h

        #bricks
        src/main/cpp/BricksShader/BricksShaderNativeRenderer.cpp
        src/main/cpp/BricksShader/BricksShaderNativeRenderer.h

        #Mandelbrot
        src/main/cpp/MandelbrotSet/MandelbrotSetRenderer.cpp
        src/main/cpp/MandelbrotSet/MandelbrotSetRenderer.h

        #polyhedrons
        src/main/cpp/Polyhedrons/PolyhedronsRenderer.cpp
        src/main/cpp/Polyhedrons/PolyhedronsRenderer.h
        src/main/cpp/graphics/Models/Platonic/Polyhedron.h
        src/main/cpp/graphics/Models/Platonic/Polyhedron.cpp
        src/main/cpp/graphics/Models/Platonic/Tetrahedron.h
        src/main/cpp/graphics/Models/Platonic/Tetrahedron.cpp
        src/main/cpp/graphics/Models/Platonic/TestCube.h
        src/main/cpp/graphics/Models/Platonic/TestCube.cpp
        src/main/cpp/graphics/Models/Platonic/Octahedron.h
        src/main/cpp/graphics/Models/Platonic/Octahedron.cpp
        src/main/cpp/graphics/Models/Platonic/Dodecahedron.h
        src/main/cpp/graphics/Models/Platonic/Dodecahedron.cpp
        src/main/cpp/graphics/Models/Platonic/Icosahedron.h
        src/main/cpp/graphics/Models/Platonic/Icosahedron.cpp

        #Geodetic
        src/main/cpp/geodeticBodies/GeodeticBodiesRenderer.h
        src/main/cpp/geodeticBodies/GeodeticBodiesRenderer.cpp
        src/main/cpp/graphics/Models/Geodetic/Cylinder.h
        src/main/cpp/graphics/Models/Geodetic/Cylinder.cpp
        src/main/cpp/graphics/Models/Geodetic/Sphere.h
        src/main/cpp/graphics/Models/Geodetic/Sphere.cpp
        src/main/cpp/graphics/Models/Geodetic/Torus.h
        src/main/cpp/graphics/Models/Geodetic/Torus.cpp
        src/main/cpp/graphics/Models/Geodetic/Cone.h
        src/main/cpp/graphics/Models/Geodetic/Cone.cpp



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