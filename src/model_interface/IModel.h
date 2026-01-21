#ifndef IMODEL_H
#define IMODEL_H

#include <string>

/**
 * @class IModel
 * @brief Interfaz abstracta para el motor de inteligencia artificial del transpilador.
 *
 * Esta clase define el contrato que debe cumplir cualquier "cerebro" de IA
 * utilizado por el transpilador TPTC. Desacopla la lógica de transpilación
 * principal del mecanismo específico de traducción de código.
 */
class IModel {
public:
    /**
     * @brief Destructor virtual para permitir la destrucción polimórfica.
     */
    virtual ~IModel() = default;

    /**
     * @brief Transpila una cadena de código fuente de JavaScript a C++.
     *
     * @param js_code El código JavaScript a transpilar.
     * @return std::string El código C++ generado.
     */
    virtual std::string transpile(const std::string& js_code) = 0;
};

#endif // IMODEL_H
