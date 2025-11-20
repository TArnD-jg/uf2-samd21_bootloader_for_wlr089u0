#include "uf2.h"
volatile bool g_interrupt_enabled = true;

// SAML21 starts at 1MHz by default.
uint32_t current_cpu_frequency_MHz = 1;
void system_init(void) {
    __disable_irq();
    NVMCTRL->CTRLB.bit.RWS=2;
    __enable_irq();
    OSCCTRL->DFLLCTRL.reg = 0U ;
    while((OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY) != OSCCTRL_STATUS_DFLLRDY)
    {
    }
    uint8_t calibCoarse = (uint8_t)(((*(uint32_t*)0x00806020U) >> 26U ) & 0x3fU);
    OSCCTRL->DFLLVAL.reg= OSCCTRL_DFLLVAL_COARSE((uint32_t)calibCoarse) | OSCCTRL_DFLLVAL_FINE(512U);
    while((OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY) != OSCCTRL_STATUS_DFLLRDY)
    {
    }
    OSCCTRL->DFLLCTRL.reg = OSCCTRL_DFLLCTRL_ENABLE ;
    while((OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY) != OSCCTRL_STATUS_DFLLRDY)
    {
    }
    GCLK->GENCTRL[0].reg = GCLK_GENCTRL_DIV(1U) | GCLK_GENCTRL_SRC(7U) | GCLK_GENCTRL_OE | GCLK_GENCTRL_GENEN;

    while((GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL0) == GCLK_SYNCBUSY_GENCTRL0)
    {
    }
    SysTick_Config(1000);

    // Uncomment these two lines to output GCLK0 on the SWCLK pin.
    // PORT->Group[0].PINCFG[30].bit.PMUXEN = 1;
    // Set the port mux mask for odd processor pin numbers, PA30 = 30 is even number, PMUXE = PMUX Even
    // PORT->Group[0].PMUX[30 / 2].reg |= PORT_PMUX_PMUXE_H;
    current_cpu_frequency_MHz = 48;
}

void SysTick_Handler(void) { LED_TICK(); }
