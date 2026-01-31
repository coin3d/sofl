add_subdirectory(simple)

add_executable(xpm_loader xpm_loader.cpp)
target_link_libraries(xpm_loader fltk::fltk fltk::gl SoFl)

add_executable(buttons_with_image buttons_with_image.cpp)
target_link_libraries(buttons_with_image fltk::fltk fltk::gl SoFl)

add_executable(flflex flflex.cpp)
target_link_libraries(flflex fltk::fltk)
