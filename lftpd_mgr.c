#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lftpd.h"

#define DEFAULT_PATH        "/littlefs"
#define DEFAULT_PORT        21u
static lftpd_t  ftpd_obj;

static void lftpd_thread(void *pvParameters)
{
    (void) pvParameters;
    lftpd_start(DEFAULT_PATH, DEFAULT_PORT, &ftpd_obj);
    vTaskDelete(NULL);
}

void init_ftpd()
{
    xTaskCreate(lftpd_thread, "ftpd", 8192u, NULL, 4, NULL);
}