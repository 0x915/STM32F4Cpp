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
		GPIO *SCL = new GPIO(PortSCL,AF_OD,UP,LOW,AF);
		GPIO *SDA = new GPIO(PortSDA,AF_OD,UP,LOW,AF);
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

namespace HSPI1{
	SPI_HandleTypeDef SPI1Handle;
	void Init(GPIO_Port PortSCLK,GPIO_Port PortMISO,GPIO_Port PortMOSI,uint32_t AF){
		GPIO *SCLK = new GPIO(PortSCLK,AF_PP,NO,HIGH,AF);
		GPIO *MISO = new GPIO(PortMISO,AF_PP,NO,HIGH,AF);
		GPIO *MOSI = new GPIO(PortMOSI,AF_PP,NO,HIGH,AF);
		SPI1Handle.Instance = SPI1;
		SPI1Handle.Init.Mode = SPI_MODE_MASTER;
		SPI1Handle.Init.Direction = SPI_DIRECTION_2LINES;
		SPI1Handle.Init.DataSize = SPI_DATASIZE_8BIT;
		SPI1Handle.Init.CLKPolarity = SPI_POLARITY_LOW;
		SPI1Handle.Init.CLKPhase = SPI_PHASE_1EDGE;
		SPI1Handle.Init.NSS = SPI_NSS_SOFT;
		SPI1Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
		SPI1Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
		SPI1Handle.Init.TIMode = SPI_TIMODE_DISABLE;
		SPI1Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
		SPI1Handle.Init.CRCPolynomial = 10;
		if (HAL_SPI_Init(&SPI1Handle) != HAL_OK) Error_Handler();
		delete SCLK;
		delete MISO;
		delete MOSI;
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
