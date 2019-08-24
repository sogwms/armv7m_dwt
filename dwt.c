/*
 * Copyright (c) 2019, sogwyms@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-24     sogwms       the first version
 */

// TODO
// support COMPARATOR0

#include <rtthread.h>
#include "dwt.h"

#define DBG_SECTION_NAME    "dwt"
#define DBG_LEVEL           DBG_LOG
#include <rtdbg.h>

#define UINT32_MAX_VALUE    (0xFFFFFFFFUL)

static float tick_to_ns = 0.0f;
static uint32_t tick_measure;

rt_inline uint32_t get_diff(uint32_t t1, uint32_t t2)
{
    int64_t t = t2 - t1;
    if (t < 0)
        t += UINT32_MAX_VALUE;
    return t;
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

void dwt_enable_cyccnt(void)
{
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

int dwt_init(uint32_t frquency)
{
    uint32_t tmp;

    if (!(*(uint32_t *)(ROM_TABLE_ENTRY + ROMDWT_OFFSET) & 1))
    {
        LOG_E("DWT is not present!");
        return RT_ERROR;
    }
    LOG_I("DWT is present");

    // Global enable for DWT and ITM features
    *(uint32_t *)DEMCR_REG_ADDR |= DEMCR_MASK_TRCENA;
    LOG_I("Enable DWT and ITM units");

    tmp = *(uint32_t *)(DWT_CTRL_REG_ADDR);

    if (tmp & DWT_MASK_CTRL_NOCYCCNT)
    {
        LOG_E("Cycle counter is not supported!");
        return RT_ERROR;
    }

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

    // Enable DWT count
    dwt_enable_cyccnt();

    return RT_EOK;
}
