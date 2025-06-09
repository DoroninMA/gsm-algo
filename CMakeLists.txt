cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("gsm-algo")

set(IO_UTILS_PROJECT "${PROJECT_SOURCE_DIR}/libs/IOUtils")
set(GSM_CRYPTO_PROJECT "${PROJECT_SOURCE_DIR}/libs/GsmCrypto")
set(DATA_GEN_PROJECT "${PROJECT_SOURCE_DIR}/libs/DataGen")
set(APP_PROJECT "${PROJECT_SOURCE_DIR}/app")

add_subdirectory ("${GSM_CRYPTO_PROJECT}")
add_subdirectory ("${IO_UTILS_PROJECT}")
add_subdirectory ("${DATA_GEN_PROJECT}")
add_subdirectory ("${APP_PROJECT}")


file(COPY "${CMAKE_BINARY_DIR}/libs/GsmCrypto/gsm_crypto.dll"
        DESTINATION "${CMAKE_BINARY_DIR}/app/")

file(COPY "${CMAKE_BINARY_DIR}/libs/IOUtils/io_utils.dll"
        DESTINATION "${CMAKE_BINARY_DIR}/app/")

file(COPY "${CMAKE_BINARY_DIR}/libs/DataGen/data_gen.dll"
        DESTINATION "${CMAKE_BINARY_DIR}/app/")
