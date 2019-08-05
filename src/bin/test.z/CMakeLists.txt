cmake_minimum_required(VERSION 3.0)

project(test.z.example C)
add_executable(${PROJECT_NAME} example.c)
# set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-D_FILE_OFFSET_BITS=64")
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(${PROJECT_NAME} z)

project(test.z.infcover)
add_executable(${PROJECT_NAME} infcover.c)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(${PROJECT_NAME} z)

project(test.z.minigzip)
add_executable(${PROJECT_NAME} minigzip.c)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(${PROJECT_NAME} z)