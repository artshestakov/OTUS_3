cmake_minimum_required(VERSION 3.12)

project(homework_3)

set (CMAKE_CXX_STANDARD 17)

add_executable(homework_3
    src/main.cpp
	src/custom_list.h
	src/custom_allocator.h
	src/utils.h
)

target_include_directories(homework_3
    PRIVATE "${CMAKE_BINARY_DIR}"
)

install(TARGETS homework_3 RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_FILE_NAME "homework_3")
set(CPACK_PACKAGE_CONTACT art.shestakov@icloud.com)
include(CPack)
