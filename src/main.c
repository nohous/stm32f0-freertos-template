#include "stm32f0xx.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_gpio.h"
#include "FreeRTOS.h"
#include "task.h"

void SystemInit(void) 
{ 

}

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
	LL_GPIO_SetOutputPin(GPIOC, LED_STATUS_G);
}

void test_task(void* arg)
{
	for ( ; ; ) {
		LL_GPIO_SetOutputPin(GPIOC, LED_STATUS_R);
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

#if 0
void test_task2(void* arg)
{
	for ( ; ; ) {
		LL_GPIO_TogglePin(GPIOC, LED_STATUS_G);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
#endif

int main(void) 
{
	rcc_init();
	gpio_init();
	/* SysTick clock is HCLK = 48M, to get 1000 Hz, we divide by 6000 */
	//SysTick_Config(6000*8);

#if 1
	static StackType_t test_task_stack[128];
	static StaticTask_t test_task_buffer;
	static xTaskHandle test_task_handle;
	test_task_handle = xTaskCreateStatic(
		test_task, 
		"test_task", 
		sizeof(test_task_stack), 
		NULL, 
		2, 
		test_task_stack, 
		&test_task_buffer
	); 
//	vTaskResume(test_task_handle);
#endif
#if 0
	StackType_t test_task2_stack[256];
	StaticTask_t test_task2_buffer;
	xTaskHandle test_task2_handle;
	test_task2_handle = xTaskCreateStatic(
		test_task2, 
		"test_task", 
		sizeof(test_task2_stack), 
		NULL, 
		tskIDLE_PRIORITY, 
		test_task2_stack, 
		&test_task2_buffer
	); 
#endif
	vTaskStartScheduler();

	/* Dead code from here on */

	while (1) {
		volatile int i;
		for (i = 0; i < 100000; i++) {
			
		}
		//LL_GPIO_TogglePin(GPIOC, LED_STATUS_R);
	}
}

uint32_t SystemCoreClock = 48000000;

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
	static StaticTask_t xIdleTaskTCB;
	static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}