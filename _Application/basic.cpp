#include "basic.h"
#include "ssd1306.h"

namespace HTIM10{
	TIM_HandleTypeDef TIM10Handle;
	#define TIM10CLK 100
	
	void Init(void){
		__HAL_RCC_TIM10_CLK_ENABLE();
		TIM10Handle.Instance = TIM10;
		TIM10Handle.Init.Prescaler = TIM10CLK - 1;
		TIM10Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
		TIM10Handle.Init.Period = 1;
		TIM10Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		TIM10Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		if(HAL_TIM_Base_Init(&TIM10Handle) != HAL_OK) Error_Handler();
	}
}
namespace HI2C1{
	I2C_HandleTypeDef I2C1Handle;
	
	void IO(){
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
		HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	}
	
	void Init(){
		IO();
		__HAL_RCC_I2C1_CLK_ENABLE();
		I2C1Handle.Instance = I2C1;
		I2C1Handle.Init.ClockSpeed = 400000;
		I2C1Handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
		I2C1Handle.Init.OwnAddress1 = 0;
		I2C1Handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		I2C1Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		I2C1Handle.Init.OwnAddress2 = 0;
		I2C1Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		I2C1Handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
		if(HAL_I2C_Init(&I2C1Handle) != HAL_OK) Error_Handler();
	}
}
namespace HUART1{
	UART_HandleTypeDef UART1Handle;
	
	void IO(){
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	}
	
	void Init(void){
		IO();
		__HAL_RCC_USART1_CLK_ENABLE();
		UART1Handle.Instance = USART1;
		UART1Handle.Init.BaudRate = 115200;
		UART1Handle.Init.WordLength = UART_WORDLENGTH_8B;
		UART1Handle.Init.StopBits = UART_STOPBITS_1;
		UART1Handle.Init.Parity = UART_PARITY_NONE;
		UART1Handle.Init.Mode = UART_MODE_TX_RX;
		UART1Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		UART1Handle.Init.OverSampling = UART_OVERSAMPLING_16;
		if(HAL_UART_Init(&UART1Handle) != HAL_OK) Error_Handler();
	}
}

void main_(void){
	
	HI2C1::Init();
	HUART1::Init();
	HTIM10::Init();
	
	HUART Debug(&HUART1::UART1Handle);
	#define Print(str) Debug.SeedByteString(str)
	
	Delay Time(&HTIM10::TIM10Handle);
	HI2C I2Ch1(&HI2C1::I2C1Handle);
	
	// ------------------ Print I2C device ---- Start ----------
	Print(" #### Online I2C device(7b address) #### \n ");
	for(uint8_t addr = 0;addr < 0xff;addr++){
		if(addr > 0x01 && addr % 2 == 0){
			if(I2Ch1.Check(addr)){
				char addr_str[6] = {};
				sprintf(addr_str,"0x%02X ",addr);
				Print(addr_str);
			}else
				Print("---- ");
			if(addr == 0xFE) Print("---- \n");
			if(addr % 16 == 0) Print("\n ");
		}
	}
	Print("\n");
	// ------------------ Print I2C device ----- End -----------

	GPIO LED13(GPIOC,GPIO_PIN_13,GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_HIGH);
	LED13.Write(0);
	
	OLED1306 TS(&HI2C1::I2C1Handle,0x78,8,8,128);
	
	TS.DrawString(Page1,Col1,h12,"1234567890");
	TS.DrawString(Page3,Col1,h12,"F(xyz,ijk)");
	TS.DrawString(Page5,Col1,h12,"<A>[B](C);");
	TS.DrawString(Page7,Col1,h12,"~!@#$%^&&*");
	
	while(true){
		Time.ms(1000);
		LED13.Toggle();
	}
}




