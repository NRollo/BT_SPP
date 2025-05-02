// Project: ESP32 Bluetooth SPP Monitor
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_spp_api.h"

#define TAG "MAIN"

extern esp_err_t spp_server_init(void);

QueueHandle_t SPPconQueue;

void app_main(void)
{
    // Just to wait for a slow monitor to start (i.e. Platformio)
    vTaskDelay(pdMS_TO_TICKS(10000));

    SPPconQueue = xQueueCreate(2, sizeof(int));
    int fd = 0;

    if (spp_server_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "spp server init failed");
        return;
    }

    // Wait for the SPP connection to be established
    ESP_LOGI(TAG, "Waiting for SPP connection...");
    xQueueReceive(SPPconQueue, &fd, portMAX_DELAY);
   
    /* Other SW initialization / startup here (... after the SPP is connected) */
}
