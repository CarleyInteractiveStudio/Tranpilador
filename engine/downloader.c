#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void download_from_github(const char* url, const char* dest) {
    char cmd[512];
    printf("[Downloader] Cloning from GitHub: %s\n", url);
    sprintf(cmd, "git clone --depth 1 %s %s", url, dest);
    system(cmd);
}

void download_from_drive(const char* url, const char* dest) {
    printf("[Downloader] Handling Google Drive link...\n");
    // Lógica para extraer ID y usar curl para descargar el zip
    char cmd[512];
    sprintf(cmd, "curl -L -o %s/project.zip '%s'", dest, url);
    system(cmd);
}

void download_generic(const char* url, const char* dest) {
    char cmd[512];
    printf("[Downloader] Downloading from URL: %s\n", url);
    sprintf(cmd, "mkdir -p %s && curl -L -o %s/index.html %s", dest, dest, url);
    system(cmd);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Uso: %s <url> <dest_folder>\n", argv[0]);
        return 1;
    }
    const char* url = argv[1];
    const char* dest = argv[2];

    if (strstr(url, "github.com")) {
        download_from_github(url, dest);
    } else if (strstr(url, "drive.google.com")) {
        download_from_drive(url, dest);
    } else {
        download_generic(url, dest);
    }
    return 0;
}
