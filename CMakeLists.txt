cmake_minimum_required(VERSION 3.14)

project(TPTC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Definir una "biblioteca" para el núcleo del transpilador.
# Esto nos permite agrupar las fuentes y sus propiedades.
add_library(tptc_lib
    src/transpiler/Transpiler.cpp
    src/model_interface/SimpleTransformer.cpp
)

# Especificar que tptc_lib necesita acceso a los headers en `src`
target_include_directories(tptc_lib PUBLIC src)

# Crear el ejecutable principal y vincularlo con la biblioteca
add_executable(tptc src/main.cpp)
target_link_libraries(tptc PRIVATE tptc_lib)

# --- Sección de Pruebas ---
enable_testing()

# Crear el ejecutable para las pruebas
add_executable(run_tests tests/test_transpilation.cpp)

# El ejecutable de pruebas también necesita la lógica del transpilador y sus includes.
target_link_libraries(run_tests PRIVATE tptc_lib)

# Añadir la prueba al conjunto de CTest
add_test(NAME simple_transpilation_test COMMAND run_tests)
