cmake_minimum_required(VERSION 3.16)

# Project
project(BrsCanFlasher CXX)

file(GLOB_RECURSE HEADER_FILES "./include/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "./src/*.cpp")

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "./include")

# Compile
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Nana
find_package(unofficial-nana CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::nana::nana)

# Vector.XlApi
target_include_directories(${PROJECT_NAME} PRIVATE "./dep/VectorXlApi/include")
target_link_libraries(${PROJECT_NAME} PRIVATE ${BRS_PROJ_PATH}"dep/VectorXlApi/lib/vxlapi64.lib")
message(${BRS_PROJ_PATH})
