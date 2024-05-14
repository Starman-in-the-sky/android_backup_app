cmake_minimum_required(VERSION 3.8)
project("Phone_backup_app")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build/${CMAKE_BUILD_TYPE})
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/dep/imgui")
set(IMGUI_BACKENDS_DIR "${IMGUI_DIR}/backends")

file(GLOB IMGUI_SOURCES 
     "${IMGUI_DIR}/*.cpp"
     "${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp"
     "${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp"
)

add_executable(Phone_backup_app WIN32
     "src/main.cpp" 
     "src/mainwindow.h" 
     ${IMGUI_SOURCES}   
     "src/mainwindow.cpp" 
     "src/DevicePage.cpp" 
     "src/DevicePage.h"
     "src/HistoryPage.cpp"
     "src/HistoryPage.h"
     "src/SettingsPage.cpp"
     "src/SettingsPage.h"
     "src/utils/TextureLoader.cpp"
     "src/utils/TextureLoader.h"
     "src/ExplorerWindows.cpp"
     "src/ExplorerWindows.h"
)

target_include_directories(Phone_backup_app PRIVATE
    "${IMGUI_DIR}"
    "${IMGUI_BACKENDS_DIR}"
    "${CMAKE_SOURCE_DIR}/dep/stb"
    "${CMAKE_SOURCE_DIR}/src/utils"
)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(Phone_backup_app 
    PRIVATE 
    glfw
    OpenGL::GL
)
