cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("gsm-algo")

set(IO_UTILS_PROJECT "${PROJECT_SOURCE_DIR}/libs/IOUtils")
set(GSM_CRYPTO_PROJECT "${PROJECT_SOURCE_DIR}/libs/GsmCrypto")
set(DATA_GEN_PROJECT "${PROJECT_SOURCE_DIR}/libs/DataGen")
set(STATISTICS_PROJECT "${PROJECT_SOURCE_DIR}/libs/Statistics")
set(APP_PROJECT "${PROJECT_SOURCE_DIR}/app")

add_subdirectory ("${GSM_CRYPTO_PROJECT}")
add_subdirectory ("${IO_UTILS_PROJECT}")
add_subdirectory ("${DATA_GEN_PROJECT}")
add_subdirectory ("${STATISTICS_PROJECT}")
add_subdirectory ("${APP_PROJECT}")


target_link_libraries(app
    PRIVATE
    gsm_crypto
    io_utils
    data_gen
    statistics
)


set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app"
)

set_target_properties(gsm_crypto PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app"
)

set_target_properties(io_utils PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app"
)

set_target_properties(data_gen PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app"
)

set_target_properties(statistics PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/app"
)
