#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sanitize(char* str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == ';' || str[i] == '&' || str[i] == '|' || str[i] == '`' || str[i] == '$') str[i] = '_';
    }
}

int main(int argc, char** argv) {
    if (argc < 3) return 1;
    char url[512], dest[256], cmd[1024];
    strncpy(url, argv[1], 511);
    strncpy(dest, argv[2], 255);
    sanitize(url); sanitize(dest);

    if (strstr(url, "github.com")) {
        sprintf(cmd, "git clone --depth 1 https://github.com/%s %s", strstr(url, "github.com") + 11, dest);
    } else {
        sprintf(cmd, "mkdir -p %s && curl -L %s -o %s/index.html", dest, url, dest);
    }
    printf("[Downloader] Ejecutando: %s\n", cmd);
    return system(cmd);
}
