/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2018, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#if DEVICE_LPTICKER

/***********************************************************************/
/* lpticker_lptim config is 1 in json config file                      */
/* LPTICKER is based on LPTIM feature from ST drivers. RTC is not used */
#if MBED_CONF_TARGET_LPTICKER_LPTIM

#include "lp_ticker_api.h"
#include "mbed_error.h"
#include "mbed_power_mgmt.h"
#include "platform/mbed_critical.h"
#include <stdbool.h>

/*  lpticker delay is for using C++ Low Power Ticker wrapper,
 *  which introduces extra delays. We rather want to use the
 *  low level implementation from this file */
#if defined(LPTICKER_DELAY_TICKS) && (LPTICKER_DELAY_TICKS > 0)
#warning "lpticker_delay_ticks usage not recommended"
#endif

#define LP_TIMER_WRAP(val) (val & 0xFFFF)
/* Safe guard is the number of ticks between the current tick and the next
 * tick we want to program an interrupt for. Programing an interrupt in
 * between is unreliable */
#define LP_TIMER_SAFE_GUARD 5


LPTIM_HandleTypeDef LptimHandle;

const ticker_info_t *lp_ticker_get_info()
{
    static const ticker_info_t info = {
#if MBED_CONF_TARGET_LSE_AVAILABLE
        LSE_VALUE / MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK,
#else
        LSI_VALUE / MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK,
#endif
        16
    };
    return &info;
}

volatile uint8_t  lp_Fired = 0;
/*  Flag and stored counter to handle delayed programing at low level */
volatile bool lp_delayed_prog = false;
volatile bool lp_cmpok = false;
volatile timestamp_t lp_delayed_counter = 0;
volatile bool sleep_manager_locked = false;

static int LPTICKER_inited = 0;
static void LPTIM1_IRQHandler(void);

void lp_ticker_init(void)
{
    /* Check if LPTIM is already configured */
    if (LPTICKER_inited) {
        lp_ticker_disable_interrupt();
        return;
    }
    LPTICKER_inited = 1;

    RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

#if MBED_CONF_TARGET_LSE_AVAILABLE

    /* Enable LSE clock */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    /* Select the LSE clock as LPTIM peripheral clock */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
#if (TARGET_STM32L0)
    RCC_PeriphCLKInitStruct.LptimClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
#else
    RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
#endif

#else /* MBED_CONF_TARGET_LSE_AVAILABLE */

    /* Enable LSI clock */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    /* Select the LSI clock as LPTIM peripheral clock */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
#if (TARGET_STM32L0)
    RCC_PeriphCLKInitStruct.LptimClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
#else
    RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
#endif

#endif /* MBED_CONF_TARGET_LSE_AVAILABLE */

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error("HAL_RCC_OscConfig ERROR\n");
        return;
    }

    if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK) {
        error("HAL_RCCEx_PeriphCLKConfig ERROR\n");
        return;
    }

    __HAL_RCC_LPTIM1_CLK_ENABLE();
    __HAL_RCC_LPTIM1_FORCE_RESET();
    __HAL_RCC_LPTIM1_RELEASE_RESET();

    /* Initialize the LPTIM peripheral */
    LptimHandle.Instance = LPTIM1;
    LptimHandle.State = HAL_LPTIM_STATE_RESET;
    LptimHandle.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
#if defined(MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK)
#if (MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK == 4)
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV4;
#elif (MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK == 2)
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV2;
#else
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
#endif
#else
    LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
#endif /* MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK */

    LptimHandle.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
#if defined (LPTIM_ACTIVEEDGE_FALLING)
    LptimHandle.Init.Trigger.ActiveEdge = LPTIM_ACTIVEEDGE_FALLING;
#endif
#if defined (LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION)
    LptimHandle.Init.Trigger.SampleTime = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;
#endif

    LptimHandle.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
    LptimHandle.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
    LptimHandle.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
#if defined (LPTIM_INPUT1SOURCE_GPIO) /* STM32L4 */
    LptimHandle.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
    LptimHandle.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
#endif /* LPTIM_INPUT1SOURCE_GPIO */

    if (HAL_LPTIM_Init(&LptimHandle) != HAL_OK) {
        error("HAL_LPTIM_Init ERROR\n");
        return;
    }

    NVIC_SetVector(LPTIM1_IRQn, (uint32_t)LPTIM1_IRQHandler);

#if defined (__HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_IT)
    /* EXTI lines are not configured by default */
    __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_IT();
    __HAL_LPTIM_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE();
#endif

    __HAL_LPTIM_ENABLE_IT(&LptimHandle, LPTIM_IT_CMPM);
    __HAL_LPTIM_ENABLE_IT(&LptimHandle, LPTIM_IT_CMPOK);
    HAL_LPTIM_Counter_Start(&LptimHandle, 0xFFFF);

    /* Need to write a compare value in order to get LPTIM_FLAG_CMPOK in set_interrupt */
    __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK);
    __HAL_LPTIM_COMPARE_SET(&LptimHandle, 0);
    while (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK) == RESET) {
    }
    __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK);

    /* Init is called with Interrupts disabled, so the CMPOK interrupt
     * will not be handled. Let's mark it is now safe to write to LP counter */
    lp_cmpok = true;
}

static void LPTIM1_IRQHandler(void)
{
    core_util_critical_section_enter();

    if (lp_Fired) {
        lp_Fired = 0;
        /* We're already in handler and interrupt might be pending,
         * so clear the flag, to avoid calling irq_handler twice */
        __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPM);
        lp_ticker_irq_handler();
    }

    /* Compare match interrupt */
    if (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPM) != RESET) {
        if (__HAL_LPTIM_GET_IT_SOURCE(&LptimHandle, LPTIM_IT_CMPM) != RESET) {
            /* Clear Compare match flag */
            __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPM);
            lp_ticker_irq_handler();
        }
    }

    if (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK) != RESET) {
        if (__HAL_LPTIM_GET_IT_SOURCE(&LptimHandle, LPTIM_IT_CMPOK) != RESET) {
            __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK);
            lp_cmpok = true;
            if(sleep_manager_locked) {
                sleep_manager_unlock_deep_sleep();
                sleep_manager_locked = false;
            }
            if(lp_delayed_prog) {
                lp_ticker_set_interrupt(lp_delayed_counter);
                lp_delayed_prog = false;
            }
        }
    }


#if defined (__HAL_LPTIM_WAKEUPTIMER_EXTI_CLEAR_FLAG)
    /* EXTI lines are not configured by default */
    __HAL_LPTIM_WAKEUPTIMER_EXTI_CLEAR_FLAG();
#endif
    core_util_critical_section_exit();
}

uint32_t lp_ticker_read(void)
{
    uint32_t lp_time = LPTIM1->CNT;
    /* Reading the LPTIM_CNT register may return unreliable values.
    It is necessary to perform two consecutive read accesses and verify that the two returned values are identical */
    while (lp_time != LPTIM1->CNT) {
        lp_time = LPTIM1->CNT;
    }
    return lp_time;
}

/*  This function should always be called from critical section */
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    core_util_critical_section_enter();

    /* Always store the last requested timestamp */
    lp_delayed_counter = timestamp;
    NVIC_EnableIRQ(LPTIM1_IRQn);

    /* CMPOK is set by hardware to inform application that the APB bus write operation to the
     * LPTIM_CMP register has been successfully completed.
     * Any successive write before the CMPOK flag be set, will lead to unpredictable results
     * We need to prevent to set a new comparator value before CMPOK flag is set by HW */
    if (lp_cmpok == false) {
        /* if this is not safe to write, then delay the programing to the
         * time when CMPOK interrupt will trigger */
        lp_delayed_prog = true;
    } else {
        timestamp_t last_read_counter = lp_ticker_read();
        lp_ticker_clear_interrupt();

        /*  HW is not able to trig a very short term interrupt, that is
         *  not less than few ticks away (LP_TIMER_SAFE_GUARD). So let's make sure it'
         *  s at least current tick + LP_TIMER_SAFE_GUARD */
        for(uint8_t i = 0; i < LP_TIMER_SAFE_GUARD; i++) {
            if (LP_TIMER_WRAP(last_read_counter  + i) == timestamp) {
                timestamp = LP_TIMER_WRAP(timestamp + LP_TIMER_SAFE_GUARD);
            }
        }
        /* Then check if this target timestamp is not in the past, or close to wrap-around
         * Let's assume last_read_counter = 0xFFFC, and we want to program timestamp = 0x100
         * The interrupt will not fire before the CMPOK flag is OK, so there are 2 cases:
         * in case CMPOK flag is set by HW after or at wrap-around, then this will fire only @0x100
         * in case CMPOK flag is set before, it will indeed fire early, as for the wrap-around case.
         * But that will take at least 3 cycles and the interrupt fires at the end of a cycle.
         * In our case 0xFFFC + 3 => at the transition between 0xFFFF and 0.
         * If last_read_counter was 0xFFFB, it should be at the transition between 0xFFFE and 0xFFFF.
         * There might be crossing cases where it would also fire @ 0xFFFE, but by the time we read the counter,
         * it may already have moved to the next one, so for now we've taken this as margin of error.
         */
         if((timestamp < last_read_counter) && (last_read_counter <= (0xFFFF - LP_TIMER_SAFE_GUARD))) {
            /*  Workaround, because limitation */
            __HAL_LPTIM_COMPARE_SET(&LptimHandle, ~0);
        } else {
            /*  It is safe to write */
            __HAL_LPTIM_COMPARE_SET(&LptimHandle, timestamp);
        }

        /* We just programed the CMP so we'll need to wait for cmpok before
         * next programing */
        lp_cmpok = false;
        /*  Prevent from sleeping after compare register was set as we need CMPOK
         *  interrupt to fire (in ~3x30us cycles) before we can safely enter deep sleep mode */
        if(!sleep_manager_locked) {
            sleep_manager_lock_deep_sleep();
            sleep_manager_locked = true;
        }
    }
    core_util_critical_section_exit();
}

void lp_ticker_fire_interrupt(void)
{
    core_util_critical_section_enter();
    lp_Fired = 1;
    /* In case we fire interrupt now, then cancel pending programing */
    lp_delayed_prog = false;
    NVIC_SetPendingIRQ(LPTIM1_IRQn);
    NVIC_EnableIRQ(LPTIM1_IRQn);
    core_util_critical_section_exit();
}

void lp_ticker_disable_interrupt(void)
{
    core_util_critical_section_enter();

    if(!lp_cmpok) {
        while (__HAL_LPTIM_GET_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK) == RESET) {
        }
        __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPOK);
        lp_cmpok = true;
    }
    /*  now that CMPOK is set, allow deep sleep again */
    if(sleep_manager_locked) {
        sleep_manager_unlock_deep_sleep();
        sleep_manager_locked = false;
    }
    lp_delayed_prog = false;
    lp_Fired = 0;
    NVIC_DisableIRQ(LPTIM1_IRQn);
    NVIC_ClearPendingIRQ(LPTIM1_IRQn);

    core_util_critical_section_exit();
}

void lp_ticker_clear_interrupt(void)
{
    core_util_critical_section_enter();
    __HAL_LPTIM_CLEAR_FLAG(&LptimHandle, LPTIM_FLAG_CMPM);
    NVIC_ClearPendingIRQ(LPTIM1_IRQn);
    core_util_critical_section_exit();
}

void lp_ticker_free(void)
{
    lp_ticker_disable_interrupt();
}

/*****************************************************************/
/* lpticker_lptim config is 0 or not defined in json config file */
/* LPTICKER is based on RTC wake up feature from ST drivers      */
#else /* MBED_CONF_TARGET_LPTICKER_LPTIM */

#include "rtc_api_hal.h"

const ticker_info_t *lp_ticker_get_info()
{
    static const ticker_info_t info = {
        RTC_CLOCK / 4, // RTC_WAKEUPCLOCK_RTCCLK_DIV4
        32
    };
    return &info;
}

void lp_ticker_init(void)
{
    rtc_init();
    lp_ticker_disable_interrupt();
}

uint32_t lp_ticker_read(void)
{
    return rtc_read_lp();
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    rtc_set_wake_up_timer(timestamp);
}

void lp_ticker_fire_interrupt(void)
{
    rtc_fire_interrupt();
}

void lp_ticker_disable_interrupt(void)
{
    rtc_deactivate_wake_up_timer();
}

void lp_ticker_clear_interrupt(void)
{
    lp_ticker_disable_interrupt();
}

void lp_ticker_free(void)
{
    lp_ticker_disable_interrupt();
}

#endif /* MBED_CONF_TARGET_LPTICKER_LPTIM */

#endif /* DEVICE_LPTICKER */
