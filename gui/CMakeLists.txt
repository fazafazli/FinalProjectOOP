cmake_minimum_required(VERSION 3.16)
project(GudangGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Class Barang dan Gudang diambil dari folder di atas, sama dengan versi console
add_executable(gudang_gui
    main_gui.cpp
    JendelaUtama.cpp
    ../Barang.cpp
    ../Gudang.cpp
)
target_include_directories(gudang_gui PRIVATE ..)
target_link_libraries(gudang_gui PRIVATE Qt6::Widgets)
