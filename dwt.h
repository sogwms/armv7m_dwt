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

// register
#define ROM_TABLE_ENTRY             ((uint32_t)0xE00FF000)
#define DEMCR_REG_ADDR              ((uint32_t)0xE000EDFC)

#define DWT_CTRL_REG_ADDR           ((uint32_t)0xE0001000)
#define DWT_CYCCNT_REG_ADDR         ((uint32_t)0xE0001004)
#define DWT_CPICNT_REG_ADDR         ((uint32_t)0xE0001008)
#define DWT_EXCCNT_REG_ADDR         ((uint32_t)0xE000100C)
#define DWT_SLEEPCNT_REG_ADDR       ((uint32_t)0xE0001010)
#define DWT_LSUCNT_REG_ADDR         ((uint32_t)0xE0001014)
#define DWT_FOLDCNT_REG_ADDR        ((uint32_t)0xE0001018)
#define DWT_PCSR_REG_ADDR           ((uint32_t)0xE000101C)

#define DWT_COMP_0_REG_ADDR         ((uint32_t)0xE0001020)
#define DWT_MASK_0_REG_ADDR         ((uint32_t)0xE0001024)
#define DWT_FUNCTION_0_REG_ADDR     ((uint32_t)0xE0001028)

// mask
#define DEMCR_MASK_TRCENA           ((uint32_t)(0x01UL << 24))
#define DWT_MASK_CTRL_NUMCOMP       ((uint32_t)(0x0FUL << 28))
#define DWT_MASK_CTRL_NOCYCCNT      ((uint32_t)(0x01UL << 25))
#define DWT_MASK_CTRL_CYCCNTENA     ((uint32_t)(0x01UL << 0))
#define DWT_MASK_FUNCTION_CYCMATCH  ((uint32_t)(0x01UL << 7))
#define DWT_MASK_FUNCTION_MATCHED   ((uint32_t)(0x01UL << 24))

// offset
#define ROMDWT_OFFSET               (0x004)


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
