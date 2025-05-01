// Project: ESP32 Bluetooth SPP Monitor
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_spp_api.h"

#define TAG "MAIN"

extern esp_err_t spp_server_init(void);
extern uint32_t ConHandle;

QueueHandle_t SPPconQueue;

static int app_printf(void *cookie, const char *data, int size)
{
    if (ConHandle != 0)
    {
        if (esp_spp_write(ConHandle, size, (uint8_t*) data) != ESP_OK)
        {
            esp_spp_write(ConHandle, 18, (uint8_t*) "data send failed\r\n");
            return -1;
        }
    }
    return size;
}

void app_main(void)
{
    SPPconQueue = xQueueCreate(2, sizeof(int));
    char *stdout_buf = (char *) malloc(128);
    int fd = 0;

    if (spp_server_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "spp server init failed");
        return;
    }

    xQueueReceive(SPPconQueue, &fd, portMAX_DELAY);

    // switch stdout to SPP via app_printf
    fclose(_GLOBAL_REENT->_stdout);
    _GLOBAL_REENT->_stdout = fwopen(NULL, &app_printf);
    setvbuf(_GLOBAL_REENT->_stdout, stdout_buf, _IOLBF, 128);
    // Log data will now be sent to the SPP connection
}
