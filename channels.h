#pragma once

#include <stdint.h>

uint32_t thread_add_channel(uint32_t thread_id);

void tread_read_channel(uint32_t thread_id, uint8_t channel,void *buffer,
                        uint32_t  buffer_size);

void thread_send_to_channel(uint32_t thread_id, uint8_t channel, void *buffer,
                            uint32_t buffer_size);
