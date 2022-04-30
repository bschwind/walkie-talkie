#include "transport.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "nvs_flash.h"

static uint8_t broadcast_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static uint8_t esp_now_send_buf[250];
static StreamBufferHandle_t network_stream_buf;

static void init_wifi() {
    esp_err_t err = esp_netif_init();
    if (err != ESP_OK) {
        printf("Error initializing netif\n");
    }

    err = esp_event_loop_create_default();
    if (err != ESP_OK) {
        printf("Error creating default event loop\n");
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    err = esp_wifi_init(&cfg);
    if (err != ESP_OK) {
        printf("Error initializing wifi\n");
    }

    err = esp_wifi_set_storage(WIFI_STORAGE_RAM);
    if (err != ESP_OK) {
        printf("Error setting wifi storage\n");
    }

    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK) {
        printf("Error setting wifi mode\n");
    }

    err = esp_wifi_start();
    if (err != ESP_OK) {
        printf("Error starting wifi\n");
    }

    // Enable long-range
    // esp_wifi_set_protocol(ESPNOW_WIFI_IF,
    // WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N|WIFI_PROTOCOL_LR);
}

static void recv_callback(const uint8_t* mac_addr, const uint8_t* data, int len) {
    // printf("ESP NOW RECV - %u bytes\n", len);

    xStreamBufferSend(network_stream_buf, data, len, portMAX_DELAY);
}

static void init_esp_now() {
    esp_err_t err = esp_now_init();
    if (err != ESP_OK) {
        printf("Error starting ESP NOW\n");
    }

    err = esp_now_register_recv_cb(recv_callback);
    if (err != ESP_OK) {
        printf("Error registering ESP NOW receiver callback\n");
    }

    esp_now_peer_info_t broadcast_peer = {
        .peer_addr = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .lmk = {0},
        .channel = 1, // ranges from 0-14
        .ifidx = ESP_IF_WIFI_STA,
        .encrypt = false,
        .priv = NULL,
    };

    err = esp_now_add_peer(&broadcast_peer);
    if (err != ESP_OK) {
        printf("Error adding ESP NOW peer\n");
    }
}

static void sender_task(void* task_param) {
    StreamBufferHandle_t mic_stream_buf = (StreamBufferHandle_t)task_param;

    while (true) {
        size_t num_bytes = xStreamBufferReceive(mic_stream_buf, (void*)esp_now_send_buf,
                                                sizeof(esp_now_send_buf), portMAX_DELAY);

        if (num_bytes > 0) {
            // printf("Received %u bytes\n", num_bytes);

            esp_err_t err = esp_now_send(broadcast_mac, esp_now_send_buf, sizeof(esp_now_send_buf));
            if (err != ESP_OK) {
                printf("Error sending ESP NOW packet: %x\n", err);
            }
        }
    }
}

static void init_non_volatile_storage() {
    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    if (err != ESP_OK) {
        printf("Error initializing NVS\n");
    }
}

void init_transport(StreamBufferHandle_t mic_stream_buf, StreamBufferHandle_t net_stream_buf) {
    printf("Init transport!\n");
    network_stream_buf = net_stream_buf;

    init_non_volatile_storage();
    init_wifi();
    init_esp_now();

    xTaskCreate(sender_task, "sender_task", 4096, (void*)mic_stream_buf, 10, NULL);
}
