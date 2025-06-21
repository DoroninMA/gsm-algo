cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME DataGenerator)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_subdirectory(${CMAKE_SOURCE_DIR}/libs/GsmCrypto)

#add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(${PROJECT_NAME} src/main.cpp)

# linking with libs
target_link_libraries(${PROJECT_NAME} PRIVATE
    gsmcrypto
    ioutils
    datagen
    statistics
)

target_include_directories(${PROJECT_NAME} PRIVATE
    inc/
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/Crypto/inc
)

#target_include_directories(${PROJECT_NAME} PRIVATE
#    ${CMAKE_SOURCE_DIR}/libs/GsmCrypto/inc
#)

#if(MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
#else()
#    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
#endif()