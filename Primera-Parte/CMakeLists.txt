cmake_minimum_required(VERSION 3.7)
project(MiProyecto CXX)

# Buscar el paquete de Python
find_package(Python REQUIRED COMPONENTS Interpreter Development)

# Agregar los archivos fuente de tu proyecto
set(SOURCES
    main.cpp
    forPostfix.h
    forPythonGraphs.h
)

# Agregar un ejecutable usando los archivos fuente enumerados
add_executable(${PROJECT_NAME} ${SOURCES})
target_sources(${PROJECT_NAME} PRIVATE forPostfix.cpp forPythonGraphs.cpp)

# Incluir directorios de Python
target_include_directories(${PROJECT_NAME} PRIVATE ${Python_INCLUDE_DIRS})

# Vincular con las bibliotecas de Python
target_link_libraries(${PROJECT_NAME} PRIVATE ${Python_LIBRARIES})
