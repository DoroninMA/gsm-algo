cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("gsm-algo")

set(APP_PROJECT "${PROJECT_SOURCE_DIR}/app")
set(CRYPTO_PROJECT "${PROJECT_SOURCE_DIR}/libs/crypto")

add_subdirectory ("${CRYPTO_PROJECT}")
add_subdirectory ("${APP_PROJECT}")

set(crypto_name "$<TARGET_FILE:${gsm_crypto}>")

file(COPY "${CMAKE_BINARY_DIR}/libs/crypto/gsm_crypto.dll"
        DESTINATION "${CMAKE_BINARY_DIR}/app/")
