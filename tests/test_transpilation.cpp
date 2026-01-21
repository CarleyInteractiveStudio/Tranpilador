#include "model_interface/SimpleTransformer.h"
#include <iostream>
#include <string>
#include <cassert>

// Función de prueba simple
void test_simple_variable_transpilation() {
    std::cout << "Ejecutando prueba: test_simple_variable_transpilation..." << std::endl;

    // 1. Arrange: Crear el modelo y el código de entrada
    auto model = SimpleTransformer();
    std::string js_code = "let version = 1;";
    std::string expected_cpp_code = "int version = 1;";

    // 2. Act: Transpilar el código
    std::string actual_cpp_code = model.transpile(js_code);

    // 3. Assert: Verificar el resultado
    assert(actual_cpp_code == expected_cpp_code);

    std::cout << "Prueba superada." << std::endl;
}

int main() {
    test_simple_variable_transpilation();
    return 0;
}
