cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME ioutils)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB io_utils_src CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/Csv/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${io_utils_src})

target_include_directories(${PROJECT_NAME} PUBLIC
    inc
)
