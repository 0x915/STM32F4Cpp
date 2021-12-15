#include "main.h"

class HUART{
  private:
	UART_HandleTypeDef *UARTx;
  public:
	HUART(UART_HandleTypeDef *UARTxx) : UARTx(UARTxx){}
	
	void SeedByteString(const char str[]){
		for(uint16_t i = 0;str[i] != '\0';i++){
			uint8_t tmp[] = {str[i]};
			HAL_UART_Transmit(UARTx,tmp,1,100);
		}
	}
};

class HI2C{
  private:
	I2C_HandleTypeDef *I2Cx;
	uint8_t _MemSize;
	uint8_t buffer[2] = {0};
  public:
	HI2C(I2C_HandleTypeDef *I2Cxx,uint8_t MemSize = I2C_MEMADD_SIZE_8BIT) : I2Cx(I2Cxx){
		_MemSize = MemSize;
	}
	
	void Speed(uint32_t bps){
		I2Cx->Init.ClockSpeed = bps;
		if(HAL_I2C_Init(I2Cx) != HAL_OK) Error_Handler();
	}
	
	void Write(uint16_t slave,uint16_t reg,uint8_t data){
		HAL_I2C_Mem_Write(I2Cx,slave,reg,_MemSize,&data,1,10);
	}
	
	uint16_t Read(uint16_t slave,uint16_t mem,uint8_t size = 2){
		if(HAL_I2C_Mem_Read(I2Cx,slave,mem,8,buffer,size,10) == HAL_OK)
			return buffer[1] << 8 | buffer[0] << 0;
		else return 0xffff;
	}
	
	bool Check(uint8_t addr,uint8_t mem = 0x00,uint8_t data[8] = {}){
		if(HAL_I2C_Mem_Read(I2Cx,addr,mem,_MemSize,data,1,1) == HAL_OK) return true;
		else return false;
	}
	
	void GetI2CDevice(){}
};

class Delay{
  private:
	TIM_HandleTypeDef *TIMx;
  
  public:
	Delay(TIM_HandleTypeDef *TIMxx) : TIMx(TIMxx){}
	
	void us(uint16_t delay){
		uint16_t count = 0xffff - delay - 4;
		HAL_TIM_Base_Start(TIMx);
			__HAL_TIM_SetCounter(TIMx,count);
		while(count < 0xffff - 5) count = __HAL_TIM_GetCounter(TIMx);
		HAL_TIM_Base_Stop(TIMx);
	}
	
	void ms(uint16_t delay){
		for(int i = 0;i < delay;i++)
			us(1000);
	}
};

typedef enum{
	A00 = 1 << 8 | 0,A01,A02,A03,A04,A05,A06,A07,A08,A09,A10,A11,A12,A13,A14,A15,
	B00 = 2 << 8 | 0,B01,B02,B03,B04,B05,B06,B07,B08,B09,B10,B11,B12,B13,B14,B15,
	C00 = 3 << 8 | 0,C01,C02,C03,C04,C05,C06,C07,C08,C09,C10,C11,C12,C13,C14,C15,
	D00 = 4 << 8 | 0,D01,D02,D03,D04,D05,D06,D07,D08,D09,D10,D11,D12,D13,D14,D15,
	E00 = 5 << 8 | 0,E01,E02,E03,E04,E05,E06,E07,E08,E09,E10,E11,E12,E13,E14,E15,
}GPIO_Port;
typedef enum{
	ANALOG = GPIO_MODE_ANALOG,
	OUT_PP = GPIO_MODE_OUTPUT_PP,
	OUT_OD = GPIO_MODE_OUTPUT_OD,
	AF_PP = GPIO_MODE_AF_PP,
	AF_OD = GPIO_MODE_AF_OD,
	IN = GPIO_MODE_INPUT,
	IT_R = GPIO_MODE_IT_RISING,
	IT_F = GPIO_MODE_IT_FALLING,
	IT_RF = GPIO_MODE_IT_RISING_FALLING,
	EV_R = GPIO_MODE_EVT_RISING,
	EV_F = GPIO_MODE_EVT_FALLING,
	EV_RF = GPIO_MODE_EVT_RISING_FALLING,
	
}GPIO_Mode;
typedef enum{
	NO = GPIO_NOPULL,
	UP = GPIO_PULLUP,
	DOWN = GPIO_PULLDOWN,
}GPIO_Pull;
typedef enum{
	LOW = GPIO_SPEED_FREQ_LOW,
	MEDIUM = GPIO_SPEED_FREQ_MEDIUM,
	HIGH = GPIO_SPEED_FREQ_HIGH,
	SPEED = GPIO_SPEED_FREQ_VERY_HIGH,
}GPIO_Speed;

class GPIO{
  private:
	GPIO_TypeDef *_GPIOx;
	uint16_t _PINx;
	uint32_t _Mode;
	uint32_t _AF;
	uint32_t _Pull;
	uint32_t _Speed;
  
  public:
	GPIO(GPIO_Port Port,GPIO_Mode Mode,GPIO_Pull Pull,GPIO_Speed Speed,uint32_t AF = 0x00){
		switch(Port >> 8){
			case 1:
				_GPIOx = GPIOA;
				break;
			case 2:
				_GPIOx = GPIOB;
				break;
			case 3:
				_GPIOx = GPIOC;
				break;
			case 4:
				_GPIOx = GPIOD;
				break;
			case 5:
				_GPIOx = GPIOE;
				break;
		}
		switch(Port & 0xff){
			case 0:
				_PINx = GPIO_PIN_0;
				break;
			case 1:
				_PINx = GPIO_PIN_1;
				break;
			case 2:
				_PINx = GPIO_PIN_2;
				break;
			case 3:
				_PINx = GPIO_PIN_3;
				break;
			case 4:
				_PINx = GPIO_PIN_4;
				break;
			case 5:
				_PINx = GPIO_PIN_5;
				break;
			case 6:
				_PINx = GPIO_PIN_6;
				break;
			case 7:
				_PINx = GPIO_PIN_7;
				break;
			case 8:
				_PINx = GPIO_PIN_8;
				break;
			case 9:
				_PINx = GPIO_PIN_9;
				break;
			case 10:
				_PINx = GPIO_PIN_10;
				break;
			case 11:
				_PINx = GPIO_PIN_11;
				break;
			case 12:
				_PINx = GPIO_PIN_12;
				break;
			case 13:
				_PINx = GPIO_PIN_13;
				break;
			case 14:
				_PINx = GPIO_PIN_14;
				break;
			case 15:
				_PINx = GPIO_PIN_15;
				break;
			
		}
		_Mode = Mode;
		_Pull = Pull;
		_Speed = Speed;
		if(Mode == AF_PP || Mode == AF_OD) _AF = AF;
		Init();
		Write(false);
	}
	
	void Init(){
		if(_GPIOx == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
		else if(_GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
		else if(_GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
		else if(_GPIOx == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
		else if(_GPIOx == GPIOE) __HAL_RCC_GPIOH_CLK_ENABLE();
		else if(_GPIOx == GPIOH) __HAL_RCC_GPIOH_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = _PINx;
		GPIO_InitStruct.Mode = _Mode;
		GPIO_InitStruct.Pull = _Pull;
		GPIO_InitStruct.Speed = _Speed;
		if(_AF)GPIO_InitStruct.Alternate = _AF;
		HAL_GPIO_Init(_GPIOx,&GPIO_InitStruct);
	}
	
	void deInit(){
		HAL_GPIO_DeInit(_GPIOx,_PINx);
	}
	
	void Write(bool PinValue){
		if(PinValue)
			HAL_GPIO_WritePin(_GPIOx,_PINx,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(_GPIOx,_PINx,GPIO_PIN_RESET);
	}
	
	void Toggle(){
		HAL_GPIO_TogglePin(_GPIOx,_PINx);
	}
	
	GPIO_PinState Read(){
		return HAL_GPIO_ReadPin(_GPIOx,_PINx);
	}
};



