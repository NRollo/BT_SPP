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
    SPPconQueue = xQueueCreate(2, sizeof(int));
    int fd = 0;

    if (spp_server_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "spp server init failed");
        return;
    }

    xQueueReceive(SPPconQueue, &fd, portMAX_DELAY);
    ESP_LOGI(TAG, "stdout now switched to SPP connection");    
}
