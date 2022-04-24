#include "audio.h"
#include "driver/i2s.h"

#define SAMPLE_BITS (24)
#define SAMPLE_RATE (48000)

#define I2S_PORT (0)
#define I2S_BCLK_PIN (GPIO_NUM_4)
#define I2S_WS_PIN (GPIO_NUM_5)
#define I2S_DO_PIN (GPIO_NUM_18)

#define READ_BUF_SIZE_BYTES ((SAMPLE_BITS / 8) * 4096)

static uint8_t mic_read_buf[READ_BUF_SIZE_BYTES];

static void init_mic() {
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM,
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2,
        .dma_buf_count = 8,
        .dma_buf_len = 200,
        .use_apll = 0,
    };

    esp_err_t err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);

    if (err != ESP_OK) {
        printf("Error initializing I2S Mic\n");
    }

    i2s_pin_config_t pin_config = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = I2S_BCLK_PIN,
        .ws_io_num = I2S_WS_PIN,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_DO_PIN,
    };

    err = i2s_set_pin(I2S_PORT, &pin_config);

    if (err != ESP_OK) {
        printf("Error setting I2S pins\n");
    }

    err = i2s_set_clk(I2S_PORT, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_24BIT, I2S_CHANNEL_MONO);

    if (err != ESP_OK) {
        printf("Error setting I2S clock\n");
    }
}

static void audio_capture_task() {
    init_mic();

    size_t bytes_read = 0;
    TickType_t ticks_to_wait = 100;

    while (true) {
        i2s_read(I2S_PORT, (char*)mic_read_buf, READ_BUF_SIZE_BYTES, &bytes_read, ticks_to_wait);

        printf("Mic bytes read: %i\n", bytes_read);
    }
}

void init_audio() {
    printf("Init audio!\n");

    xTaskCreate(audio_capture_task, "audio_capture_task", 4096, NULL, 10, NULL);
}
