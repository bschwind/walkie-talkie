#include "audio.h"
#include "driver/i2s.h"
#include "soc/i2s_reg.h"

#define SAMPLE_RATE (16000)

#define I2S_PORT (I2S_NUM_0)
#define MIC_BITS_PER_SAMPLE (I2S_BITS_PER_SAMPLE_16BIT)
#define I2S_BCLK_PIN (GPIO_NUM_32)
#define I2S_WS_PIN (GPIO_NUM_25)
#define I2S_DATA_PIN (GPIO_NUM_33)

#define READ_BUF_SIZE_BYTES (250)

static uint8_t mic_read_buf[READ_BUF_SIZE_BYTES];

static void init_mic() {
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX,
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = MIC_BITS_PER_SAMPLE,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0,
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
        .data_in_num = I2S_DATA_PIN,
    };

    // Fixes for SPH0645 Mic
    // REG_SET_BIT(I2S_TIMING_REG(I2S_PORT), BIT(9));
    // REG_SET_BIT(I2S_CONF_REG(I2S_PORT), I2S_RX_MSB_SHIFT);

    err = i2s_set_pin(I2S_PORT, &pin_config);
    if (err != ESP_OK) {
        printf("Error setting I2S pins\n");
    }

    err = i2s_set_clk(I2S_PORT, SAMPLE_RATE, MIC_BITS_PER_SAMPLE, I2S_CHANNEL_MONO);
    if (err != ESP_OK) {
        printf("Error setting I2S clock\n");
    }
}

static void audio_capture_task(void* task_param) {
    StreamBufferHandle_t mic_stream_buf = (StreamBufferHandle_t)task_param;
    init_mic();

    size_t bytes_read = 0;
    TickType_t ticks_to_wait = 100;

    while (true) {
        i2s_read(I2S_PORT, (char*)mic_read_buf, READ_BUF_SIZE_BYTES, &bytes_read, ticks_to_wait);

        xStreamBufferSend(mic_stream_buf, mic_read_buf, READ_BUF_SIZE_BYTES, portMAX_DELAY);

        // printf("Mic bytes read: %i\n", bytes_read);
    }
}

void init_audio(StreamBufferHandle_t mic_stream_buf) {
    printf("Init audio!\n");

    xTaskCreate(audio_capture_task, "audio_capture_task", 4096, (void*)mic_stream_buf, 10, NULL);
}
