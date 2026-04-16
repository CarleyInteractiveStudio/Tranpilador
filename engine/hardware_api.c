#include "hardware_api.h"
#include <stdio.h>
#include <stdlib.h>

void hw_mic_start_record() { printf("[Hardware] Iniciando grabacion de microfono nativo...\n"); }
void hw_mic_stop_record() { printf("[Hardware] Deteniendo grabacion de microfono.\n"); }

void hw_camera_open() { printf("[Hardware] Abriendo camara del dispositivo...\n"); }
void hw_gallery_pick_image() { printf("[Hardware] Abriendo galeria de imagenes...\n"); }

void hw_fs_write_file(const char* path, const char* data) {
    printf("[Hardware] Escribiendo en archivo nativo: %s\n", path);
    FILE* f = fopen(path, "w");
    if (f) { fputs(data, f); fclose(f); }
}

char* hw_fs_read_file(const char* path) {
    printf("[Hardware] Leyendo archivo nativo: %s\n", path);
    return NULL;
}

bool hw_net_is_connected() {
    printf("[Hardware] Verificando estado de conexion a Internet...\n");
    return true;
}
