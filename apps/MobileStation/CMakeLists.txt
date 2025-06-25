cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME MobileStation)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB app_src CONFIGURE_DEPENDS
    "src/*.cpp"
)

file(GLOB app_ui CONFIGURE_DEPENDS
    "ui/*.ui"
)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.cpp
    src/DualStreamBuf.cpp


    inc/mainwindow.h
    ui/mainwindow.ui
    resources/resources.qrc
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    gsmcrypto
    ioutils
    datagen
    mobile
    network
)

target_include_directories(${PROJECT_NAME} PRIVATE
    inc/
)
