#include "transpiler/Transpiler.h"
#include "model_interface/SimpleTransformer.h"
#include <iostream>
#include <memory>

void print_usage() {
    std::cout << "Uso: tptc <archivo_entrada.js> -o <directorio_salida>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        print_usage();
        return 1;
    }

    std::string input_filepath = argv[1];
    std::string option = argv[2];
    std::string output_dir = argv[3];

    if (option != "-o") {
        print_usage();
        return 1;
    }

    try {
        // 1. Instanciar el motor de IA
        auto model = std::make_unique<SimpleTransformer>();

        // 2. Instanciar el transpilador con el motor
        Transpiler transpiler(std::move(model));

        // 3. Ejecutar la transpilación
        bool success = transpiler.transpile_project(input_filepath, output_dir);

        if (success) {
            std::cout << "TPTC ha finalizado exitosamente." << std::endl;
            return 0;
        } else {
            std::cerr << "TPTC ha encontrado errores durante la transpilación." << std::endl;
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "Se ha producido una excepción: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
