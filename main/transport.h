#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/stream_buffer.h"

void init_transport(StreamBufferHandle_t mic_stream_buf, StreamBufferHandle_t network_stream_buf);

#endif
