#ifndef HARDWARE_API_H
#define HARDWARE_API_H

#include <stdbool.h>

// Microfono
void hw_mic_start_record();
void hw_mic_stop_record();

// Galeria / Camara
void hw_camera_open();
void hw_gallery_pick_image();

// Sistema de Archivos
void hw_fs_write_file(const char* path, const char* data);
char* hw_fs_read_file(const char* path);

// Estado de Red
bool hw_net_is_connected();

#endif
