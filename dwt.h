/*
 * Copyright (c) 2019, sogwyms@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-24     sogwms       the first version
 */
#ifndef __DWT_H__
#define __DWT_H__

#include <stdint.h>

float       dwt_set_frequency(uint32_t frquency);
uint32_t    dwt_tick_to_ns(uint32_t tick);
uint32_t    dwt_get_count(void);
void        dwt_delay(uint32_t tick);
void        dwt_udelay(uint32_t us);
void        dwt_mdelay(uint32_t ms);
void        dwt_measure_begin(void);
uint32_t    dwt_measure_end(void);
void        dwt_enable_cyccnt(void);
void        dwt_disable_cyccnt(void);
int         dwt_init(uint32_t frquency);

#endif // __DWT_H__
