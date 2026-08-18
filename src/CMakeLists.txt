# 1. Look for a SDL2 package
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

add_executable(trexel_exec
    main.cpp
    engine.cpp
)

# Link to actual SDL2 library
target_link_libraries(trexel_exec PRIVATE SDL2::SDL2)
target_link_libraries(trexel_exec PRIVATE imgui)

# Include header files
target_include_directories(trexel_exec PRIVATE ${CMAKE_SOURCE_DIR}/include/trexel)
