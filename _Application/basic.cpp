#include "basic.h"
#include "ssd1306.h"
#include "MLX90614.h"

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

//namespace SMBUS1{
//	SMBUS_HandleTypeDef SMB1Handle;
//
//	void Init(GPIO_Port PortSCL,GPIO_Port PortSDA,GPIO_Port PortAlert,uint32_t AF){
//		GPIO *SCL = new GPIO(PortSCL,AF_OD,NO,LOW,AF);
//		GPIO *SDA = new GPIO(PortSDA,AF_OD,NO,LOW,AF);
//		GPIO *SMBA = new GPIO(PortAlert,AF_OD,NO,LOW,AF);
//		__HAL_RCC_I2C1_CLK_ENABLE();
//		SMB1Handle.Instance = I2C1;
//		SMB1Handle.Mode = HAL_SMBUS_MODE_MASTER;
//		SMB1Handle.Init.ClockSpeed = 100000;
//
//	}
//}

namespace HI2C1{
	I2C_HandleTypeDef I2C1Handle;
	
	void Init(GPIO_Port PortSCL,GPIO_Port PortSDA,uint32_t AF){
		GPIO *SCL = new GPIO(PortSCL,AF_OD,NO,LOW,AF);
		GPIO *SDA = new GPIO(PortSDA,AF_OD,NO,LOW,AF);
		__HAL_RCC_I2C1_CLK_ENABLE();
		I2C1Handle.Instance = I2C1;
		I2C1Handle.Mode = HAL_I2C_MODE_MASTER;
		I2C1Handle.Init.ClockSpeed = 400000;
		I2C1Handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
		I2C1Handle.Init.OwnAddress1 = 0;
		I2C1Handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		I2C1Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		I2C1Handle.Init.OwnAddress2 = 0;
		I2C1Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		I2C1Handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
		if(HAL_I2C_Init(&I2C1Handle) != HAL_OK) Error_Handler();
		delete SCL;
		delete SDA;
	}
}

namespace HUART1{
	UART_HandleTypeDef UART1Handle;
	
	void Init(GPIO_Port PortTX,GPIO_Port PortRX,uint32_t AF){
		GPIO *TX = new GPIO(PortTX,AF_PP,NO,LOW,AF);
		GPIO *RX = new GPIO(PortRX,AF_PP,NO,LOW,AF);
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
		delete TX;
		delete RX;
	}
}

static char StrCache[16] = {};
void main_(void){
	
	HI2C1::Init(B08,B09,GPIO_AF4_I2C1);
	HUART1::Init(B06,B07,GPIO_AF7_USART1);
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
	
	GPIO LED13(C13,OUT_PP,NO,SPEED);
	LED13.Write(0);
	
	OLED1306 TS(&HI2C1::I2C1Handle,0x78,8,8,128);

//	TS.DrawString(Page1,Col1,h12,"1234567890");
//	TS.DrawString(Page3,Col1,h12,"F(xyz,ijk)!@#$%^&*");
//	TS.DrawString(Page5,Col1,h12,"<A>[B](C);");

//	MLX90614 DEV(&HI2C1::I2C1Handle,0x00);
//	float OBJ,AMB;
//	uint16_t ADDR = DEV.I2C_Read(0x0E|0x20,2);
//	sprintf(StrCache,"SMBus 0x%.4X",ADDR);
//	TS.DrawString(Page3,Col1,h12,StrCache);
	
	while(true){
//		LED13.Write(1);
		Time.ms(1000);
		LED13.Toggle();
		
//		OBJ = DEV.GetTarget();
//		sprintf(StrCache,"OBJ : %.2lf C",OBJ);
//		TS.DrawString(Page7,Col1,h12,StrCache);
//		AMB = DEV.GetAmbient();
//		sprintf(StrCache,"AMB : %.2lf C",AMB);
//		TS.DrawString(Page5,Col1,h12,StrCache);
	}
}




