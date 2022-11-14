get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)

file(GLOB SOURCES thread_pool/*.cpp)

add_library(fibers STATIC ${SOURCES})
target_include_directories(fibers PUBLIC ${LIB_INCLUDE_PATH})
target_link_libraries(fibers PUBLIC pthread)
