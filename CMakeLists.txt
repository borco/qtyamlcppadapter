set(LIB_NAME QtYamlCppAdapter)

set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "disable shared libs")

qt_add_library(${LIB_NAME}
    STATIC
    src/qtyamlcppadapter/yamlhelper.cpp
    src/qtyamlcppadapter/yamlhelper.h
)

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(${LIB_NAME} PUBLIC
    Qt6::Core
    yaml-cpp
    iborcocpputils
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/yaml-cpp)
