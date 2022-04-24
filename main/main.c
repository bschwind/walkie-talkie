#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "audio.h"
#include "transport.h"

static QueueHandle_t esp_now_message_queue;

void app_main(void) {
    // TODO(bschwind) - Use a stream buffer instead with xStreamBufferCreate.
    esp_now_message_queue = xQueueCreate(32, sizeof(uint16_t));

    init_transport(esp_now_message_queue);
    init_audio(esp_now_message_queue);

    while (true) {
        printf("Hello world!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
