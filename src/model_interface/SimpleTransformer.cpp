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

#include <sstream>
#include <vector>

// Estructura para mantener una regla de transpilación
struct TranspilationRule {
    std::regex pattern;
    std::function<std::string(const std::smatch&)> replacer;
};

// Colección de reglas
static const std::vector<TranspilationRule> rules = {
    // Regla para números
    {
        std::regex("^(let|const)\\s+([a-zA-Z_][a-zA-Z0-9_]*)\\s*=\\s*([0-9]+(?:\\.[0-9]+)?);"),
        [](const std::smatch& match) {
            std::string keyword = match[1];
            std::string var_name = match[2];
            std::string value = match[3];
            std::string type = (value.find('.') != std::string::npos) ? "double" : "int";
            return (keyword == "const" ? "const " : "") + type + " " + var_name + " = " + value + ";";
        }
    },
    // Regla para cadenas
    {
        std::regex("^(let|const)\\s+([a-zA-Z_][a-zA-Z0-9_]*)\\s*=\\s*\"([^\"]*)\";"),
        [](const std::smatch& match) {
            std::string keyword = match[1];
            std::string var_name = match[2];
            std::string value = match[3];
            return (keyword == "const" ? "const " : "") + std::string("std::string ") + var_name + " = \"" + value + "\";";
        }
    },
    // Regla para funciones
    {
        std::regex("function\\s+([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\(([^)]*)\\)\\s*\\{\\s*return\\s+([^;]+);\\s*\\}"),
        [](const std::smatch& match) {
            std::string func_name = match[1];
            std::string params_str = match[2];
            std::string return_expr = match[3];

            std::string cpp_params;
            std::regex param_regex("([a-zA-Z_][a-zA-Z0-9_]*)");
            auto words_begin = std::sregex_iterator(params_str.begin(), params_str.end(), param_regex);
            auto words_end = std::sregex_iterator();
            for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                if (!cpp_params.empty()) {
                    cpp_params += ", ";
                }
                cpp_params += "int " + (*i).str();
            }

            return "int " + func_name + "(" + cpp_params + ") {\n  return " + return_expr + ";\n}";
        }
    }
};


/**
 * @brief Implementación de la lógica de transpilación basada en reglas.
 *
 * Procesa el código como un flujo, buscando sentencias completas para transpilar.
 */
std::string SimpleTransformer::transpile(const std::string& js_code) {
    std::stringstream result_stream;
    std::string remaining_code = js_code;
    bool needs_string_include = false;

    while (true) {
        // 1. Limpiar espacios en blanco, saltos de línea y comentarios al principio.
        remaining_code = trim(remaining_code);
        if (remaining_code.empty()) {
            break;
        }

        // Ignorar comentarios de una línea
        if (remaining_code.rfind("//", 0) == 0) {
            size_t newline_pos = remaining_code.find('\n');
            if (newline_pos != std::string::npos) {
                remaining_code = remaining_code.substr(newline_pos + 1);
            } else {
                remaining_code.clear();
            }
            continue;
        }

        // 2. Intentar hacer coincidir una regla con el inicio del código limpio.
        bool matched_in_iteration = false;
        for (const auto& rule : rules) {
            std::smatch match;
            if (std::regex_search(remaining_code, match, rule.pattern, std::regex_constants::match_continuous)) {
                std::string replacement = rule.replacer(match);
                result_stream << replacement << "\n\n";

                if (replacement.find("std::string") != std::string::npos) {
                    needs_string_include = true;
                }

                remaining_code = match.suffix().str();
                matched_in_iteration = true;
                break;
            }
        }

        // 3. Si ninguna regla coincide, reportar y terminar.
        if (!matched_in_iteration) {
            result_stream << "// TPTC: No se pudo transpilar el resto del código:\n/*\n" << remaining_code << "\n*/\n";
            break;
        }
    }

    std::string final_code = result_stream.str();
    if (needs_string_include) {
        return "#include <string>\n\n" + final_code;
    }

    return final_code;
}
