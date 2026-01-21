#include "model_interface/SimpleTransformer.h"
#include <iostream>
#include <string>
#include <cassert>

void test_simple_variable_transpilation() {
    std::cout << "Ejecutando prueba: test_simple_variable_transpilation..." << std::endl;
    auto model = SimpleTransformer();
    std::string js_code = "let version = 1;";
    std::string expected_cpp_code = "int version = 1;";
    std::string actual_cpp_code = model.transpile(js_code);
    assert(actual_cpp_code == expected_cpp_code);
    std::cout << "Prueba superada." << std::endl;
}

void test_simple_string_transpilation() {
    std::cout << "Ejecutando prueba: test_simple_string_transpilation..." << std::endl;
    auto model = SimpleTransformer();
    std::string js_code = "const name = \"TPTC\";";
    std::string expected_cpp_code = "#include <string>\n\nconst std::string name = \"TPTC\";";
    std::string actual_cpp_code = model.transpile(js_code);
    assert(actual_cpp_code == expected_cpp_code);
    std::cout << "Prueba superada." << std::endl;
}

void test_simple_function_transpilation() {
    std::cout << "Ejecutando prueba: test_simple_function_transpilation..." << std::endl;
    auto model = SimpleTransformer();
    std::string js_code = "function add(a, b) { return a + b; }";
    std::string expected_cpp_code = "int add(int a, int b) {\n  return a + b;\n}";
    std::string actual_cpp_code = model.transpile(js_code);
    assert(actual_cpp_code == expected_cpp_code);
    std::cout << "Prueba superada." << std::endl;
}

int main() {
    test_simple_variable_transpilation();
    test_simple_string_transpilation();
    test_simple_function_transpilation();
    return 0;
}
