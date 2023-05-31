cmake_minimum_required(VERSION 3.26)
project(aaa VERSION 0.1.0)

add_executable(${PROJECT_NAME} src/main.cpp src/rectadj.cpp )

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED 20)

find_package(OpenMP)
add_subdirectory(lib/fmt)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PUBLIC lib/fmt/include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE src
    PRIVATE lib/fmt/src
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} 
        PUBLIC fmt 
        PUBLIC OpenMP::OpenMP_CXX 
    )
else()
    target_link_libraries(${PROJECT_NAME}
        PUBLIC fmt
    )
endif()