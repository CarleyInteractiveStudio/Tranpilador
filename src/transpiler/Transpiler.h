#ifndef TRANSPILER_H
#define TRANSPILER_H

#include "model_interface/IModel.h"
#include <string>
#include <memory>

/**
 * @class Transpiler
 * @brief Orquesta el proceso de transpilación de archivos.
 *
 * Esta clase utiliza una implementación de la interfaz IModel para
 * leer un archivo de entrada de JavaScript, procesar su contenido
 * y escribir los archivos de salida C++ (.h y .cpp) correspondientes.
 */
class Transpiler {
public:
    /**
     * @brief Constructor que inyecta la dependencia del modelo de IA.
     * @param model Un puntero inteligente a un objeto que implementa la interfaz IModel.
     */
    explicit Transpiler(std::unique_ptr<IModel> model);

    /**
     * @brief Procesa un archivo JavaScript y genera los archivos C++ correspondientes.
     * @param input_filepath La ruta al archivo de entrada JavaScript.
     * @param output_dir La ruta al directorio de salida para los archivos C++.
     * @return bool Verdadero si la transpilación fue exitosa, falso en caso contrario.
     */
    bool transpile_project(const std::string& input_filepath, const std::string& output_dir);

private:
    std::unique_ptr<IModel> model_;
};

#endif // TRANSPILER_H
