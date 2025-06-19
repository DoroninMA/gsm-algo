cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME mobile)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB mobile_src CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${mobile_src})

target_include_directories(${PROJECT_NAME} PUBLIC
    inc
)

target_link_libraries(${PROJECT_NAME} PRIVATE network)
target_link_libraries(${PROJECT_NAME} PRIVATE gsm_crypto)
