#include "stm32f0xx.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_gpio.h"

void SystemInit(void) { }

void rcc_init(void) 
{
	/* TODO: enable clock security subsystem*/
	LL_RCC_HSI_Enable();
	while (!LL_RCC_HSI_IsReady());

	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI);
	
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1 | LL_RCC_USART2_CLKSOURCE_PCLK1);

	LL_RCC_HSE_Enable();
	while (!LL_RCC_HSE_IsReady());

	LL_RCC_PLL_Disable();
	while (LL_RCC_PLL_IsReady());
	
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLL_MUL_6, LL_RCC_PREDIV_DIV_1);
	
	LL_RCC_PLL_Enable();
	while (!LL_RCC_PLL_IsReady());
	
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

	LL_RCC_HSI_Disable();
	while (LL_RCC_HSI_IsReady());
}

void usart1_init()
{
	
}

#define LED_STATUS_R LL_GPIO_PIN_2
#define LED_STATUS_G LL_GPIO_PIN_1
#define LED_STATUS_B LL_GPIO_PIN_0
#define LED_STATUS_PORT C

GPIO_TypeDef gpio_port_c;

void gpio_init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	LL_GPIO_InitTypeDef init = { 0 };
	init.Pin = LED_STATUS_R | LED_STATUS_G | LED_STATUS_B;
	init.Mode = LL_GPIO_MODE_OUTPUT;
	init.Speed = LL_GPIO_SPEED_FREQ_LOW;
	init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;

	LL_GPIO_Init(GPIOC, &init);
	LL_GPIO_ResetOutputPin(GPIOC, LED_STATUS_B | LED_STATUS_G | LED_STATUS_R);
	LL_GPIO_SetOutputPin(GPIOC, 0);
}

int main(void) 
{
	rcc_init();
	gpio_init();
	/* SysTick clock is 48M / 8, to get 1000 Hz, we divide by 6000 */
	SysTick_Config(6000*8);

	while (1) {
		volatile int i;
		for (i = 0; i < 100000; i++) {
			
		}
		//LL_GPIO_TogglePin(GPIOC, LED_STATUS_R);
	}
}

void SysTick_Handler(void)
{
	volatile static int i = 0;
	if (++i == 1000) {
		i = 0;
		LL_GPIO_SetOutputPin(GPIOC, LED_STATUS_B);
	}
	if (i == 10) {
		LL_GPIO_ResetOutputPin(GPIOC, LED_STATUS_B);
	}
}