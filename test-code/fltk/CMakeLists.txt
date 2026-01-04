add_subdirectory(simple)

add_executable(xpm_loader xpm_loader.cpp)
target_link_libraries(xpm_loader fltk::fltk fltk::gl SoFl)
