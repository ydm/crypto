cmake_minimum_required (VERSION 3.0)
project (des)

add_executable(des src/main.c src/des.c src/image.c)
target_link_libraries(des m)
