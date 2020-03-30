cmake_minimum_required(VERSION 3.1)

project(project-mediator-wrapper)


include_directories("mysql-conn-cpp/include/")
link_directories("mysql-conn-cpp/lib/")

include_directories("boost_1_72_0/")

include_directories("rapidxml-1.13/")

include_directories("json_lib/include/")

include_directories("./include/")

set(EXECUTABLE_OUTPUT_PATH bin)
file(GLOB_RECURSE SOURCES_FILES "src/*")

add_executable(app ${SOURCES_FILES})
target_link_libraries(
  app
  mysqlcppconn
)
