cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME MobileStation)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE
    gsmcrypto
    ioutils
    datagen
    statistics
)

target_include_directories(${PROJECT_NAME} PRIVATE
    inc/
)
