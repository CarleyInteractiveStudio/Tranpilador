#include "model_interface/SimpleTransformer.h"
#include <iostream>
#include <string>
#include <cassert>
#include <vector>

// Función auxiliar para verificar que el código generado contenga todas las subcadenas esperadas.
void assert_contains(const std::string& generated_code, const std::vector<std::string>& expected_substrings) {
    for (const auto& sub : expected_substrings) {
        assert(generated_code.find(sub) != std::string::npos);
    }
}

void test_multi_statement_transpilation() {
    std::cout << "Ejecutando prueba: test_multi_statement_transpilation..." << std::endl;

    // 1. Arrange: Crear el modelo y el código de entrada con múltiples sentencias
    auto model = SimpleTransformer();
    std::string js_code = R"(
        // Declaración de variables
        let version = 1;
        const name = "TPTC";

        // Declaración de función
        function get_version() {
            return version;
        }
    )";

    // El orden no importa, solo que las traducciones estén presentes.
    std::vector<std::string> expected_substrings = {
        "int version = 1;",
        "const std::string name = \"TPTC\";",
        "int get_version() {\n  return version;\n}"
    };

    // 2. Act: Transpilar el bloque de código
    std::string actual_cpp_code = model.transpile(js_code);

    // 3. Assert: Verificar que todas las traducciones esperadas están en la salida
    assert_contains(actual_cpp_code, expected_substrings);

    std::cout << "Prueba superada." << std::endl;
}

int main() {
    test_multi_statement_transpilation();
    return 0;
}
