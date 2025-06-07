cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME app)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_subdirectory(${CMAKE_SOURCE_DIR}/libs/crypto)

#add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(app src/main.cpp)

# linking with libs
#set(LIBS_ROOT ${CMAKE_BINARY_DIR}/libs)
#set(GSM_CRYPTO ${LIBS_ROOT}/crypto/gsm_crypto)
#set(GSM_CRYPTO ${LIBS_ROOT}/crypto/gsm_crypto.lib)
set(GSM_CRYPTO gsm_crypto)
#target_link_libraries(${PROJECT_NAME} PRIVATE ${GSM_CRYPTO})
target_link_libraries(app PRIVATE ${GSM_CRYPTO})

#target_include_directories(${PROJECT_NAME} PRIVATE
#    ${CMAKE_SOURCE_DIR}/libs/crypto/inc
#)

#if(MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
#else()
#    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
#endif()