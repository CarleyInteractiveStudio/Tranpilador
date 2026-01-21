# TPTC - Transpilador de Código

## Visión del Proyecto

TPTC (Transpilador de Código) es un proyecto ambicioso para crear no solo un transpilador, sino un verdadero **ingeniero de software artificial**. El objetivo final es desarrollar un modelo de IA avanzado, construido desde cero en C++, capaz de comprender, traducir y generar proyectos de software complejos.

A diferencia de las herramientas existentes, TPTC está diseñado para:
- **Ser 100% Original:** El núcleo del sistema será un modelo de lenguaje propio, entrenado específicamente para la traducción de código y la ingeniería de software.
- **Tomar Decisiones Inteligentes:** El modelo podrá reflexionar sobre los requisitos, optimizar el código generado y añadir comentarios explicativos de forma autónoma.
- **Generar Proyectos Completos:** Más allá de la traducción, TPTC será capaz de generar la estructura de un proyecto, los archivos de construcción y la lógica de la aplicación a partir de una descripción de alto nivel.

## Estrategia y Hoja de Ruta

Abordaremos este desafío en fases iterativas para asegurar un progreso tangible.

### Fase 1: TPTC v0.1 (Versión Actual)

El objetivo de esta fase es construir el **esqueleto de la aplicación transpiladora** en C++.

1.  **Estructura del Proyecto:** Se utiliza CMake para gestionar la compilación de un proyecto modular y escalable.
2.  **Interfaz de IA:** Se define una interfaz de C++ (`IModel`) que desacopla la lógica del transpilador del "cerebro" de IA. Esto nos permite empezar con una implementación simple y reemplazarla en el futuro sin rediseñar la aplicación.
3.  **"Pequeño Transformer" Basado en Reglas:** La primera implementación de `IModel` será un motor de traducción basado en reglas, capaz de manejar un subconjunto fundamental de JavaScript (variables, funciones, tipos primitivos).

Esta fase nos permitirá tener una herramienta funcional de línea de comandos que puede traducir archivos JavaScript simples, validando así la arquitectura general del sistema.

### Fases Futuras

- **Desarrollo del Modelo de IA:** Entrenamiento y desarrollo del modelo de lenguaje de código en C++.
- **Integración del Modelo:** Reemplazo del "Pequeño Transformer" por el modelo de IA real.
- **Librerías de Compatibilidad:** Creación de librerías C++ para emular APIs específicas de entornos JavaScript (como el DOM del navegador o APIs de Node.js).
- **Interfaz de Conversación:** Habilitar la interacción con el modelo para solicitar la generación de código o hacer preguntas.

---
*Este proyecto está siendo desarrollado por Jules, un ingeniero de software de IA.*
