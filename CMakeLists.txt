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
set(NETWORK_PROJECT "${PROJECT_SOURCE_DIR}/libs/Network")
set(HOST_PROJECT "${PROJECT_SOURCE_DIR}/libs/Host")
set(MOBILE_PROJECT "${PROJECT_SOURCE_DIR}/libs/Mobile")
set(APP_PROJECT "${PROJECT_SOURCE_DIR}/apps/DataGenerator")
set(MOBILE_STATION_PROJECT "${PROJECT_SOURCE_DIR}/apps/MobileStation")

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/apps/")

add_subdirectory("${GSM_CRYPTO_PROJECT}")
add_subdirectory("${IO_UTILS_PROJECT}")
add_subdirectory("${DATA_GEN_PROJECT}")
add_subdirectory("${STATISTICS_PROJECT}")
add_subdirectory("${NETWORK_PROJECT}")
add_subdirectory("${HOST_PROJECT}")
add_subdirectory("${MOBILE_PROJECT}")
add_subdirectory("${APP_PROJECT}")
add_subdirectory("${MOBILE_STATION_PROJECT}")


target_link_libraries(DataGenerator
    PRIVATE
    gsmcrypto
    ioutils
    datagen
    statistics
    network
)

target_link_libraries(MobileStation
        PRIVATE
        gsmcrypto
        ioutils
        datagen
        mobile
        network
)

set_target_properties(DataGenerator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/DataGenerator"
)

set_target_properties(MobileStation PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/MobileStation"
)

set_target_properties(gsmcrypto PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)

set_target_properties(ioutils PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)

set_target_properties(datagen PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)

set_target_properties(statistics PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)

set_target_properties(network PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)
