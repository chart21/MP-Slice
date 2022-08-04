cmake_minimum_required(VERSION 3.13.4)

project(MP-Slice)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(src/circuitProcessor)
add_subdirectory(src/network)
add_subdirectory(src/players)
add_subdirectory(src/protocols)

target_link_directories(${PROJECT_NAME} PUBLIC src/circuitProcessor src/network src/protocols)

target_link_libraries(${PROJECT_NAME} circuitProcessor network protocols)




