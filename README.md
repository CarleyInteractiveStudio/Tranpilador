# TPTC - Transpilador de Código Nativo Pro (v2.0)

Esta versión introduce la integración con parsers profesionales de HTML/CSS, permitiendo convertir sitios web completos y complejos en aplicaciones nativas puras.

## Novedades v2.0: El "Navegador Nativo"
- **Parser Profesional (Lexbor):** Integración de un motor de análisis de alto rendimiento que captura el DOM y CSSOM completos.
- **Soporte de Estilos Expandido:** Soporte para clases CSS, transformaciones, bordes complejos y resolución de la cascada.
- **API DOM Nativa:** Funciones en C para interactuar con el DOM (`querySelector`, `animate`) integradas con el compilador de JS.
- **Cero Errores en Proyectos Grandes:** Gracias al análisis profesional de HTML5/CSS3, la fidelidad de la transpilación es máxima.

## Arquitectura Avanzada
- `engine/`: Motor gráfico v2.0 con soporte para transformaciones y DOM API.
- `compiler/parser_pro.c`: Nuevo núcleo de transpilación profesional.

## Cómo usar
El sistema ahora detecta automáticamente la complejidad del proyecto y utiliza el motor de análisis profesional para asegurar que cada etiqueta y estilo se convierta exactamente a su equivalente en C nativo.
