/*
 * MIT License
 *
 * Copyright (c) 2017 David Antliff
 * Copyright (c) 2017 Chris Morgan <chmorgan@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file
 * @brief Interface definitions for ESP32 RMT driver used to communicate with devices
 *        on the One Wire Bus.
 *
 * This is the recommended driver.
 */

#pragma once
#ifndef OWB_RMT_H
#define OWB_RMT_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"
#include "driver/rmt.h"

#ifdef __cplusplus
extern "C" {
#endif




#define OWB_RMT_TIMING_STANDARD {   \
        .durationReset = 480,       \
        .durationW1Low = 6,         \
        .durationW1High = 64,       \
        .durationW0Low = 60,        \
        .durationW0High = 10,       \
        .durationSample = 15-2,     \
        .durationRXIdle = 72        \
    }

#define OWB_RMT_TIMING_OVERDRIVE {  \
        .durationReset = 70,        \
        .durationW1Low = 1,         \
        .durationW1High = 8,        \
        .durationW0Low = 8,         \
        .durationW0High = 3,        \
        .durationSample = 3,        \
        .durationRXIdle = 13        \
    }

typedef struct
{
    uint32_t durationReset; // Duration of RESET low pulse

    uint32_t durationW1Low; // Duration of W1 low pulse
    uint32_t durationW1High; // Duration of W1 high pulse

    uint32_t durationW0Low; // Duration of W0 low pulse
    uint32_t durationW0High; // Duration of W0 high pulse

    uint32_t durationSample; // TODO: What?

    uint32_t durationRXIdle; // Must be larger than any duration occurring during write slots
} owb_rmt_timing_t;

/**
 * @brief RMT driver information
 */
typedef struct
{
  int tx_channel;     ///< RMT channel to use for TX
  int rx_channel;     ///< RMT channel to use for RX
  RingbufHandle_t rb; ///< Ring buffer handle
  int gpio;           ///< OneWireBus GPIO
  OneWireBus bus;     ///< OneWireBus instance
  owb_rmt_timing_t timing;
} owb_rmt_driver_info;

/**
 * @brief Initialise the RMT driver.
 * @param[in] info Pointer to an uninitialized owb_rmt_driver_info structure.
 *                 Note: the structure must remain in scope for the lifetime of this component.
 * @param[in] gpio_num The GPIO number to use as the One Wire bus data line.
 * @param[in] tx_channel The RMT channel to use for transmitting data to bus devices.
 * @param[in] rx_channel the RMT channel to use for receiving data from bus devices.
 * @return OneWireBus *, pass this into the other OneWireBus public API functions
 */
OneWireBus* owb_rmt_initialize(owb_rmt_driver_info * info, gpio_num_t gpio_num,
                               rmt_channel_t tx_channel, rmt_channel_t rx_channel,
                               owb_rmt_timing_t timing);

#ifdef __cplusplus
}
#endif

#endif // OWB_RMT_H
