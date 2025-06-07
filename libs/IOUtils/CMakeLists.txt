cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME io_utils)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB gsm_crypto_src CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/Csv/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${gsm_crypto_src})

target_include_directories(${PROJECT_NAME} PUBLIC
    inc
)
