find_package(SDL2 REQUIRED)

add_executable(trexel_exec main.cpp)
target_include_directories(trexel_exec PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(trexel_exec PRIVATE SDL2::SDL2 SDL2::SDL2main)
