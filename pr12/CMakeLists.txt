cmake_minimum_required(VERSION 3.25)
project(pr12_flor_mecanica)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(${PROJECT_NAME}
        igvMallaTriangulos.cpp
        igvMallaTriangulos.h
        igvEscena3D.cpp
        igvEscena3D.h
        igvInterfaz.cpp
        igvInterfaz.h
        pr1a.cpp
        igvCamara.cpp
        igvCamara.h
        igvPunto3D.cpp
        igvPunto3D.h)

# Buscar las librerías gráficas en el sistema
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Enlazar dependiendo del sistema operativo
if (APPLE)
    # Usa los frameworks nativos de macOS (para tu portátil)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL" "-framework GLUT")
else()
    # Usa las librerías estándar/Conan (para tu PC de Windows)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
endif()

if (LINUX)
    find_path(OPENGL_REGISTRY_INCLUDE_DIRS "GL/glcorearb.h")
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_REGISTRY_INCLUDE_DIRS})

    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

    find_package(GLUT REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE GLUT::GLUT)
endif ()

if (WIN32)
    find_package(opengl_system)
    target_link_libraries(${PROJECT_NAME} opengl::opengl)

    find_package(opengl-registry)
    target_link_libraries(${PROJECT_NAME} opengl-registry::opengl-registry)

    find_package(FreeGLUT)
    target_link_libraries(${PROJECT_NAME} FreeGLUT::freeglut_static)
endif ()
