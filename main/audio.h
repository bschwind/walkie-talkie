#ifndef AUDIO_H
#define AUDIO_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/stream_buffer.h"

void init_audio(StreamBufferHandle_t mic_stream_buf, StreamBufferHandle_t network_stream_buf);

#endif
