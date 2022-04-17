#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

void init_transport(QueueHandle_t message_queue);

#endif
