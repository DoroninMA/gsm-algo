cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME gsmcrypto)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB gsm_crypto_src CONFIGURE_DEPENDS
    "src/*.c"
    "src/*.cpp"
    "src/Auth/*.cpp"
    "src/Encrypt/*.cpp"
    "src/utils/*.cpp"
    "src/utils/*.c"
)

add_library(${PROJECT_NAME} SHARED ${gsm_crypto_src})

target_include_directories(${PROJECT_NAME} PUBLIC
    inc
)
