#include "transpiler/Transpiler.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

// Función auxiliar para extraer el nombre base de un archivo sin la extensión.
static std::string get_base_name(const std::string& path) {
    return fs::path(path).stem().string();
}

Transpiler::Transpiler(std::unique_ptr<IModel> model) : model_(std::move(model)) {
    if (!model_) {
        throw std::invalid_argument("El modelo de IA no puede ser nulo.");
    }
}

bool Transpiler::transpile_project(const std::string& input_filepath, const std::string& output_dir) {
    // 1. Leer el archivo de entrada
    std::ifstream input_file(input_filepath);
    if (!input_file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de entrada: " << input_filepath << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << input_file.rdbuf();
    std::string js_code = buffer.str();
    input_file.close();

    if (js_code.empty()) {
        std::cerr << "Advertencia: El archivo de entrada está vacío: " << input_filepath << std::endl;
        return true; // No es un error, simplemente no hay nada que transpilar.
    }

    // 2. Invocar el modelo de IA para transpilar el código
    std::string cpp_code = model_->transpile(js_code);

    // 3. Determinar las rutas de salida
    std::string base_name = get_base_name(input_filepath);
    fs::path output_dir_path(output_dir);
    fs::path header_path = output_dir_path / (base_name + ".h");
    fs::path source_path = output_dir_path / (base_name + ".cpp");

    // 4. Escribir el archivo de implementación (.cpp)
    std::ofstream source_file(source_path);
    if (!source_file.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de salida: " << source_path << std::endl;
        return false;
    }
    source_file << cpp_code;
    source_file.close();

    // 5. Escribir el archivo de cabecera (.h) (vacío por ahora)
    std::ofstream header_file(header_path);
    if (!header_file.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de salida: " << header_path << std::endl;
        return false;
    }
    header_file << "// Archivo de cabecera generado por TPTC para " << base_name << ".cpp\n";
    header_file.close();

    std::cout << "Transpilación completada: " << source_path << " y " << header_path << " generados." << std::endl;

    return true;
}
