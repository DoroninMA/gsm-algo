cmake_minimum_required(VERSION 3.28)

project(network LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found")
endif()



file(GLOB mobile_src CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/Level2/*.cpp"
    "src/Level3/*.cpp"
    "src/Level3/CcMessage/*.cpp"
    "src/Level3/MmMessage/*.cpp"
)

add_library(network SHARED ${mobile_src})

target_include_directories(${PROJECT_NAME} PUBLIC
    inc
)
