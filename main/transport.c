#include "transport.h"

static void receiver_task() {
    while (true) {
        printf("Receive\n");
        vTaskDelay(500 / portTICK_RATE_MS);
    }
}

void init_transport(QueueHandle_t message_queue) {
    printf("Init transport!\n");

    xTaskCreate(receiver_task, "receiver_task", 4096, NULL, 10, NULL);
}
