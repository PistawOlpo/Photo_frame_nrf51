/**
 * Copyright (c) 2014 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/** @file
 *
 * @defgroup blinky_example_main main.c
 * @{
 * @ingroup blinky_example
 * @brief Blinky Example Application main file.
 *
 * This file contains the source code for a sample application to blink LEDs.
 *
 */
#define NRF_LOG_DEFAULT_LEVEL 4
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#include "nrf_log_ctrl.h"
#include "nrf_log.h"

void foo(void)
{ 
 char string_on_stack[] = "stack";
 //nrf_log_push() copies the string into the logger buffer and returns address from the logger buffer
 NRF_LOG_INFO("%s",nrf_log_push(string_on_stack));
}

/**void send_func( uint32_t length)
{
 NRF_LOG_INFO("sending data\r\n");
 NRF_LOG_DEBUG("%d bytes, data:\r\n", length);
 //NRF_LOG_HEXDUMP_DEBUG(p_data, length);
 if (true)
 { 
 NRF_LOG_ERROR("Phy is busy\r\n"); 
 }
}**/
/**
 * @brief Function for application main entry.
 */
int main(void)
{
    /* Configure board. */
    bsp_board_leds_init();
    
    uint32_t err_code;
    // Initialize module
    err_code = NRF_LOG_INIT(NULL);
    if (err_code != NRF_SUCCESS)
    {
    // Module initialization failed. Take corrective action.
    return 0;
    }

    while (true)
    {
        for (int i = 0; i < LEDS_NUMBER; i++)
        {
            bsp_board_led_invert(i);
            nrf_delay_ms(500);
            if (!NRF_LOG_PROCESS()) {
                NRF_LOG_FLUSH() ;
            }

            NRF_LOG_INFO("sending data\r\n");
            NRF_LOG_DEBUG("%d bytes, data:\r\n", 10);
            NRF_LOG_HEXDUMP_DEBUG("piotrstawciki", 10);

            


          //  foo();
        }
    }
}

/**
 *@}
 **/
