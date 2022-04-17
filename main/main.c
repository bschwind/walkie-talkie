#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "transport.c"

static QueueHandle_t esp_now_message_queue;

void app_main(void) {
    esp_now_message_queue = xQueueCreate(32, sizeof(uint16_t));

    init_transport(esp_now_message_queue);

    while (true) {
        printf("Hello world!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
