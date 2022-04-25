#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "audio.h"
#include "transport.h"

static StreamBufferHandle_t mic_stream_buf;

void app_main(void) {
    mic_stream_buf = xStreamBufferCreate(4096, 1);

    init_transport(mic_stream_buf);
    init_audio(mic_stream_buf);

    while (true) {
        printf("Hello world!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
