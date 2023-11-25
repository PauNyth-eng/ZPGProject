cmake_minimum_required(VERSION 3.16)
project(ZPGProject)

set(CMAKE_CXX_STANDARD 17)

link_libraries(-lglfw -lglut -lGL -lGLU -ldl -pthread -lassimp -lGLEW -lSOIL -lX11)

include_directories(/usr/include /usr/local/include include)


add_executable(ZPGProject
        main.cpp
        source/Engine.cpp
        source/Camera.cpp
        source/Model.cpp
        source/Object.cpp
        source/Observer.cpp
        source/Shader.cpp
        source/Scene.cpp
        source/ShaderManager.cpp
        source/TransComposite.cpp
        source/TransRotate.cpp
        source/TransScale.cpp
        source/TransTranslate.cpp
        include/Mouse.h
        source/Mouse.cpp
        main.cpp
        include/Light.h
        source/Light.cpp
        include/Skybox.h
        include/Texture.h
        source/Skybox.cpp
        source/Texture.cpp
)

file(COPY resources DESTINATION .)
