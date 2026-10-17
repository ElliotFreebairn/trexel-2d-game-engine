target_sources(trexel_exec PRIVATE
    asset_manager.cpp
    game.cpp
    game_object.cpp
    game_renderer.cpp
    object_factory.cpp
    scene.cpp
    scene_manager.cpp
    scene_serializer.cpp
    texture_manager.cpp
    asset_json.cpp
)

add_subdirectory(trexel)
