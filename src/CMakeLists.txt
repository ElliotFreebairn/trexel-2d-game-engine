# 1. Look for a SDL2 package
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

add_executable(trexel_exec
    main.cpp
    engine.cpp
)

# Link to actual SDL2 library
target_link_libraries(trexel_exec PRIVATE SDL2::SDL2)
target_link_libraries(trexel_exec PRIVATE SDL2_image::SDL2_image)
target_link_libraries(trexel_exec PRIVATE imgui)
target_link_libraries(trexel_exec PRIVATE glm::glm)
target_link_libraries(trexel_exec PRIVATE nlohmann_json::nlohmann_json)

# Include header files
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine)
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine/games)
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine/games/trexel)
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine/interfaces)
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine/sprite)
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine/utilities)
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine/editor)
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel_engine/ecs)

add_subdirectory(games)
add_subdirectory(editor)
add_subdirectory(sprite)
add_subdirectory(utilities)
add_subdirectory(ecs)
