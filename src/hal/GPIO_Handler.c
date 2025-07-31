//
// Created by cheolsoon on 25. 7. 28.
//
#include "GPIO_Handler.h"

void GPIO_Export(int32_t gpio) {
    int32_t fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "[WARN] ERROR While Export GPIO %d\n", gpio);
        return;
    }

    char buffer[8];
    snprintf(buffer, sizeof(buffer), "%d", gpio);
    write(fd, buffer, strlen(buffer));
    close(fd);
    return;
}

void GPIO_Unexport(int32_t gpio) {
    int32_t fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "[WARN] ERROR While Unexport GPIO %d\n", gpio);
        return;
    }
    char buffer[8];
    snprintf(buffer, sizeof(buffer), "%d", gpio);
    write(fd, buffer, strlen(buffer));
    close(fd);
    return;
}

void GPIO_Set_Direction(int32_t gpio, const char* direction) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", gpio);

    int32_t fd = open(path, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "Direction open");
        return;
    }

    write(fd, direction, strlen(direction));
    close(fd);
    return;
}

void GPIO_Write(int32_t gpio, int32_t value) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);

    int32_t fd = open(path, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "Value open");
        return;
    }

    if (value)  write(fd, "1", 1);
    else        write(fd, "0", 1);

    close(fd);
    return;
}