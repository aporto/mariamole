
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


# Find the QtWidgets library
find_package(Qt5Widgets)
include_directories(${Qt5Widgets_INCLUDE_DIRS})


find_package(Qt5Core)


## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -O2")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()



file(GLOB_RECURSE UI_FILES *.ui)
file(GLOB_RECURSE CODE_FILES about.cpp builder.cpp builderthread.cpp build_message_item.cpp buildwindow.cpp burnbootloader.cpp
		  qserialmonitor.cpp config.cpp editor.cpp editortab.cpp getuserstring.cpp launcher.cpp main.cpp mainwindow.cpp
                        message_handler.cpp mm_utils.cpp preferences.cpp project.cpp projectproperties.cpp qserialselector.cpp						   	      					        serialmonitor.cpp setworkspace.cpp wizard.cpp workspace.cpp)


qt5_wrap_ui(UI_HEADERS ${UI_FILES})
qt5_add_resources(RESOURCE_FILES mainwindow.qrc)

add_executable(${CMAKE_PROJECT_NAME} 
  ${UI_HEADERS}
  ${CODE_FILES}
  ${RESOURCE_FILES}
)


if(APPLE)
	set(LINK_LIBS "-L/Users/gduarte/Qt/5.3/clang_64/lib -F/Users/gduarte/Qt/5.3/clang_64/lib -framework QtWidgets -framework QtGui -framework QtCore -framework QtXml -framework QtSerialPort -lqscintilla2")
else()
	set(LINK_LIBS Qt5::Widgets Qt5Xml Qt5SerialPort qt5scintilla2)
endif(APPLE)


target_link_libraries(${CMAKE_PROJECT_NAME} ${LINK_LIBS})


install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/installer/common_files/config DESTINATION /etc/mariamole)




