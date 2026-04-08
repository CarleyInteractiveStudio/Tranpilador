# TPTC - Transpilador de Código Nativo Pro (v0.5)

Este sistema convierte aplicaciones web (HTML/JS/CSS) en binarios nativos de alto rendimiento.

## Novedades v0.5
- **Flexbox Layout:** Motor nativo con soporte para `display: flex` y `flex-direction`.
- **Soporte de Red:** Nueva API nativa para `fetch()`, permitiendo conectar las apps a internet.
- **Preparación Android:** Estructura base de proyecto JNI para exportación a APK.
- **Transpilador v2:** Parser mejorado para manejar divs, botones y lógica de red.

## Componentes
- `android_project/`: Estructura base para empaquetado móvil.
- `engine/`: Motor C con soporte Flexbox y Networking.
- `compiler/`: Transpilador avanzado con soporte para APIs web modernas.

## Uso
1. Ejecuta `make` para construir el IDE.
2. Escribe una app con Flexbox y Fetch.
3. Exporta a Nativo o prepara tu proyecto para Android.
