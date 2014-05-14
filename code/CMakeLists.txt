cmake_minimum_required(VERSION 2.8.11)

project(mariamole)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

include_directories(${Qt5Widgets_INCLUDE_DIRS})



## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-fPIC -O2")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()



file(GLOB_RECURSE UI_FILES *.ui)
file(GLOB_RECURSE CODE_FILES *.cpp)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})
qt5_add_resources(RESOURCE_FILES mainwindow.qrc)

add_executable(${CMAKE_PROJECT_NAME} 
  ${UI_HEADERS}
  ${CODE_FILES}
  ${RESOURCE_FILES}
)


target_link_libraries(${CMAKE_PROJECT_NAME}
  Qt5::Widgets
)
