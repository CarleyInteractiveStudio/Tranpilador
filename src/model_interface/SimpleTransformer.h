#ifndef SIMPLETRANSFORMER_H
#define SIMPLETRANSFORMER_H

#include "model_interface/IModel.h"

/**
 * @class SimpleTransformer
 * @brief Una implementación inicial del motor de transpilación basada en reglas.
 *
 * Esta clase implementa la interfaz IModel y proporciona una traducción
 * directa y basada en reglas para un subconjunto simple de características de JavaScript.
 * Es el "cerebro" inicial del TPTC v0.1.
 */
class SimpleTransformer : public IModel {
public:
    /**
     * @brief Sobrescribe el método de transpilación de la interfaz IModel.
     *
     * @param js_code El código JavaScript a transpilar.
     * @return std::string El código C++ generado.
     */
    std::string transpile(const std::string& js_code) override;
};

#endif // SIMPLETRANSFORMER_H
