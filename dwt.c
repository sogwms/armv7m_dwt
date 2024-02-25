/*
 * Copyright (c) 2019, sogwyms@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-24     sogwms       the first version
 * 2024-02-25     sogwms       optimize internal implementations
 */

// TODO
// support COMPARATOR0

// Note: The DWT cycle counter is a 32-bit counter which increase one on each processor clock cycle. On overflow, it wraps to zero.
// Cons: The counter can't work when the processor is halted in Debug state.

#include <rtthread.h>
#include "dwt.h"

#define DBG_SECTION_NAME    "dwt"
#define DBG_LEVEL           DBG_LOG
#include <rtdbg.h>

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

static float tick_to_ns = 0.0f;
static uint32_t tick_measure;

rt_inline uint32_t get_diff(uint32_t t1, uint32_t t2)
{
    return (uint32_t)((int32_t)t2 - (int32_t)t1);
}

float dwt_set_frequency(uint32_t frquency)
{
    tick_to_ns = 1000 * 1000 * 1000 / frquency;

    return tick_to_ns;
}

uint32_t dwt_tick_to_ns(uint32_t tick)
{
    return (uint32_t)(tick_to_ns * tick);
}

uint32_t dwt_get_count(void)
{
    return *((uint32_t *)DWT_CYCCNT_REG_ADDR);
}

static void dwt_set_count(uint32_t val)
{
    return *((uint32_t *)DWT_CYCCNT_REG_ADDR) = val;
}

void dwt_enable_cyccnt(void)
{
    // Global enable for DWT and ITM features
    *(uint32_t *)DEMCR_REG_ADDR |= DEMCR_MASK_TRCENA;

    *(uint32_t *)DWT_CTRL_REG_ADDR |= DWT_MASK_CTRL_CYCCNTENA;
    LOG_I("Cycle counter enabled");
}

void dwt_disable_cyccnt(void)
{
    *(uint32_t *)DWT_CTRL_REG_ADDR &= ~DWT_MASK_CTRL_CYCCNTENA;
    LOG_I("Cycle counter disabled");
}

void dwt_delay(uint32_t tick)
{
    uint32_t t = dwt_get_count();
    while(get_diff(t, dwt_get_count()) < tick);
}

void dwt_udelay(uint32_t us)
{
    dwt_delay(us * 1000 / tick_to_ns);
}

void dwt_mdelay(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        dwt_udelay(1000);
    }
}

void dwt_measure_begin(void)
{
    tick_measure = dwt_get_count();
}

uint32_t dwt_measure_end(void)
{
    return get_diff(tick_measure, dwt_get_count());
}

static int has_dwt(void) 
{
    if (!(*(uint32_t *)(ROM_TABLE_ENTRY + ROMDWT_OFFSET) & 1))
    {
        return 0;
    }

    return 1;
}

static int has_cycle_counter(void)
{
    uint32_t tmp = *(uint32_t *)(DWT_CTRL_REG_ADDR);

    if (tmp & DWT_MASK_CTRL_NOCYCCNT)
    {
        return 0;
    }

    return 1;
}

int dwt_init(uint32_t frquency)
{
    if (!(has_dwt() && has_cycle_counter())) {
        LOG_E("The chip doesn't support needed features (DWT or DWT cycle-counter)");
        return RT_ERROR;
    }
    LOG_I("The chip supports needed features");

    if (frquency == 0)
    {
        LOG_E("Frequency can't be zero");
        return RT_ERROR;
    }

    dwt_set_frequency(frquency);
    LOG_I("The 1 tick is approximately equal to %u ns", (int)tick_to_ns);

    // // check comparator0
    // if (tmp & DWT_MASK_CTRL_NUMCOMP)
    // {
    //     LOG_I("Comparator is supported");
    // }

    // Disable DWT counter And then clear counter value (to be zero)
    dwt_disable_cyccnt();
    dwt_set_count(0);

    // Enable DWT count
    dwt_enable_cyccnt();

    return RT_EOK;
}
