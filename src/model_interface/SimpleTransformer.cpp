#include "model_interface/SimpleTransformer.h"
#include <regex>
#include <string>

// Función auxiliar para eliminar espacios en blanco al inicio y final de una cadena.
static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Implementación de la lógica de transpilación basada en reglas.
 *
 * Utiliza expresiones regulares para encontrar y reemplazar patrones de código JS.
 */
std::string SimpleTransformer::transpile(const std::string& js_code) {
    std::string trimmed_js = trim(js_code);
    std::string cpp_code;

    // Regla 1: Declaración de variable numérica (ej: let name = 10;)
    std::regex num_var_decl_regex("^(let|const)\\s+([a-zA-Z_][a-zA-Z0-9_]*)\\s*=\\s*([0-9]+(?:\\.[0-9]+)?);");
    std::smatch match;

    if (std::regex_search(trimmed_js, match, num_var_decl_regex)) {
        std::string keyword = match[1];
        std::string var_name = match[2];
        std::string value = match[3];
        std::string type = (value.find('.') != std::string::npos) ? "double" : "int";

        cpp_code = (keyword == "const" ? "const " : "") + type + " " + var_name + " = " + value + ";";
        return cpp_code;
    }

    // Regla 2: Declaración de variable de cadena (ej: const name = "TPTC";)
    std::regex str_var_decl_regex("^(let|const)\\s+([a-zA-Z_][a-zA-Z0-9_]*)\\s*=\\s*\"([^\"]*)\";");
    if (std::regex_search(trimmed_js, match, str_var_decl_regex)) {
        std::string keyword = match[1];
        std::string var_name = match[2];
        std::string value = match[3];

        cpp_code = "#include <string>\n\n";
        cpp_code += (keyword == "const" ? "const " : "") + std::string("std::string ") + var_name + " = \"" + value + "\";";
        return cpp_code;
    }

    // Código de respaldo si no se reconoce el patrón
    cpp_code = "// TPTC: No se pudo transpilar el siguiente código JavaScript:\n";
    cpp_code += "/*\n" + js_code + "\n*/";
    return cpp_code;
}
