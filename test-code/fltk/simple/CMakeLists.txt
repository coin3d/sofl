
find_package(GLEW)

if(GLEW_FOUND)
    add_executable(OpenGL3test OpenGL3test.cxx)
    target_link_libraries(OpenGL3test fltk::fltk fltk::gl GLEW::glew)
endif (GLEW_FOUND)

add_executable(fltkWithOpenInventorWindow fltkWithOpenInventorWindow.cpp)
target_link_libraries(fltkWithOpenInventorWindow SoFl)
