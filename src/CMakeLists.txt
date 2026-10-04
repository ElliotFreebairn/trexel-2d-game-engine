# 1. Look for a SDL2 package
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

add_executable(trexel_exec
    main.cpp
    engine.cpp
    sdl_context.cpp
    game_renderer.cpp
    ui_manager.cpp
    game.cpp
    game_object.cpp
    texture_manager.cpp
    sprite.cpp
    scene.cpp
    scene_manager.cpp
    asset_manager.cpp
    mydefines.cpp
    game_object_registry.cpp
    scene_serializer.cpp
)

# Link to actual SDL2 library
target_link_libraries(trexel_exec PRIVATE SDL2::SDL2)
target_link_libraries(trexel_exec PRIVATE SDL2_image::SDL2_image)
target_link_libraries(trexel_exec PRIVATE imgui)
target_link_libraries(trexel_exec PRIVATE glm::glm)
target_link_libraries(trexel_exec PRIVATE nlohmann_json::nlohmann_json)

# Include header files
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine)
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine/trexel)
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine/editor)

add_subdirectory(trexel)
add_subdirectory(editor)
