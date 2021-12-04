project(CG-UFABC-FPS3D)
add_executable(${PROJECT_NAME} main.cpp model.cpp openglwindow.cpp
                               camera.cpp targets.cpp)
enable_abcg(${PROJECT_NAME})
