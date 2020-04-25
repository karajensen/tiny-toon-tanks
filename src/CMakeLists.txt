cmake_minimum_required(VERSION 3.17)
project(TinyToonTanks)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC_LIST
    Application.cpp
    Application.h
    Bullet.cpp
    Bullet.h
    BulletManager.cpp
    BulletManager.h
    Camera.cpp
    Camera.h
    CollisionEvent.h
    CollisionManager.cpp
    CollisionManager.h
    Common.h
    Conversions.h
    DataIDs.h
    Enemy.cpp
    Enemy.h
    Game.cpp
    Game.h
    GameBuilder.cpp
    GameBuilder.h
    GameData.h
    Glcommon.h
    GlmHelper.h
    Gui.cpp
    Gui.h
    Input.cpp
    Input.h
    Light.cpp
    Light.h
    Logger.h
    main.cpp
    Mesh.cpp
    Mesh.h
    MeshFile.cpp
    MeshFile.h
    OpenGL.cpp
    OpenGL.h
    OutputHelper.h
    PhysicsEngine.cpp
    PhysicsEngine.h
    Player.cpp
    Player.h
    Postprocessing.cpp
    Postprocessing.h
    Quad.cpp
    Quad.h
    RandomGenerator.cpp
    RandomGenerator.h
    Rendertarget.cpp
    Rendertarget.h
    Scene.cpp
    Scene.h
    SceneBuilder.cpp
    SceneBuilder.h
    SceneData.h
    Shader.cpp
    Shader.h
    SoundEngine.cpp
    SoundEngine.h
    Tank.cpp
    Tank.h
    TankManager.cpp
    TankManager.h
    Texture.cpp
    Texture.h
    Timer.cpp
    Timer.h
    ToonText.cpp
    ToonText.h
    Tweaker.cpp
    Tweaker.h
)

set(OPENGL_LIST
    opengl/GL.h
    opengl/gl_core_4_4.c
    opengl/gl_core_4_4.h
)

add_executable(TinyToonTanks ${SRC_LIST} ${OPENGL_LIST})

target_link_libraries(TinyToonTanks ${CMAKE_CURRENT_SOURCE_DIR}/assimp/assimp.lib)
target_link_libraries(TinyToonTanks ${CMAKE_CURRENT_SOURCE_DIR}/anttweakbar/AntTweakBar.lib)
target_link_libraries(TinyToonTanks ${CMAKE_CURRENT_SOURCE_DIR}/fmod/fmod_vc.lib)
target_link_libraries(TinyToonTanks ${CMAKE_CURRENT_SOURCE_DIR}/fmod/fmodL_vc.lib)
target_link_libraries(TinyToonTanks ${CMAKE_CURRENT_SOURCE_DIR}/glfw/glfw3.lib)
target_link_libraries(TinyToonTanks ${CMAKE_CURRENT_SOURCE_DIR}/opengl/OpenGL32.lib)
target_link_libraries(TinyToonTanks debug ${CMAKE_CURRENT_SOURCE_DIR}/bullet/debug/Bullet.lib)
target_link_libraries(TinyToonTanks optimized ${CMAKE_CURRENT_SOURCE_DIR}/bullet/release/Bullet.lib)
target_link_libraries(TinyToonTanks debug ${CMAKE_CURRENT_SOURCE_DIR}/soil/debug/soil.lib)
target_link_libraries(TinyToonTanks optimized ${CMAKE_CURRENT_SOURCE_DIR}/soil/release/soil.lib)

source_group("OpenGL" FILES ${OPENGL_LIST})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bin/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})