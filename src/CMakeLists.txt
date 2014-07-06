
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
include_directories(${Qt5Widgets_INCLUDE_DIRS})



## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -O2")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()



file(GLOB_RECURSE UI_FILES *.ui)
file(GLOB_RECURSE CODE_FILES builder.cpp build_message_item.cpp config.cpp editortab.cpp  mainwindow.cpp project.cpp setworkspace.cpp
                             workspace.cpp builderthread.cpp buildwindow.cpp editor.cpp  main.cpp message_handler.cpp projectproperties.cpp 
														    mm_utils.cpp wizard.cpp)


qt5_wrap_ui(UI_HEADERS ${UI_FILES})
qt5_add_resources(RESOURCE_FILES mainwindow.qrc)

add_executable(${CMAKE_PROJECT_NAME} 
  ${UI_HEADERS}
  ${CODE_FILES}
  ${RESOURCE_FILES}
)


target_link_libraries(${CMAKE_PROJECT_NAME}
  #Qt5::Widgets Qt5Xml qscintilla2
  Qt5::Widgets Qt5Xml Qt5SerialPort qt5scintilla2
)
