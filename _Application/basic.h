#include "main.h"

class HUART{
  private:
	UART_HandleTypeDef UARTx;
  public:
	HUART(const UART_HandleTypeDef *UARTxx): UARTx(*UARTxx){}
	void SeedByteString(const char str[]){
		for(uint16_t i = 0;str[i] != '\0';i++){
			uint8_t tmp[] = {str[i]};
			HAL_UART_Transmit(&UARTx,tmp,1,100);
		}
	}
};

class HI2C{
  private:
	I2C_HandleTypeDef I2Cx;
	uint8_t _MemSize;
  public:
	HI2C(const I2C_HandleTypeDef *I2Cxx,uint8_t MemSize = I2C_MEMADD_SIZE_8BIT) : I2Cx(*I2Cxx){
		_MemSize = MemSize;
	}
	
	void Write(uint16_t slave,uint16_t reg,uint8_t data){
		HAL_I2C_Mem_Write(&I2Cx,slave,reg,_MemSize,&data,1,10);
	}
	
	uint8_t Read(uint16_t slave,uint16_t mem){
		uint8_t data;
		HAL_I2C_Mem_Read(&I2Cx,slave,mem,8,&data,8,10);
		return data;
	}
	
	bool Check(uint8_t addr,uint8_t mem = 0x00,uint8_t data[8] = {}){
		if(HAL_I2C_Mem_Read(&I2Cx,addr,mem,_MemSize,data,1,1) == HAL_OK) return true;
		else return false;
	}
	
	void GetI2CDevice(){}
};

class Delay{
  private:
	TIM_HandleTypeDef TIMx;
  
  public:
	Delay(const TIM_HandleTypeDef *TIMxx) : TIMx(*TIMxx){}
	
	void us(uint16_t delay){
		uint16_t count = 0xffff - delay - 4;
		HAL_TIM_Base_Start(&TIMx);
			__HAL_TIM_SetCounter(&TIMx,count);
		while(count < 0xffff - 5) count = __HAL_TIM_GetCounter(&TIMx);
		HAL_TIM_Base_Stop(&TIMx);
	}
	
	void ms(uint16_t delay){
		for(int i = 0;i < delay;i++)
			us(1000);
	}
};

class GPIO{
  private:
	GPIO_TypeDef *_GPIOx;
	uint16_t _PINx;
	
	uint32_t _Mode;
	uint32_t _AF;
	
	uint32_t _Pull;
	uint32_t _Speed;
  
  public:
	GPIO(GPIO_TypeDef *GPIOx,uint16_t Pin,uint32_t Mode,uint32_t Pull,uint32_t Speed,uint32_t AF = 0x00){
		_GPIOx = GPIOx;
		_PINx = Pin;
		_Mode = Mode;
		_Pull = Pull;
		_Speed = Speed;
		if(Mode == GPIO_MODE_AF_PP || Mode == GPIO_MODE_AF_OD) _AF = AF;
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
		GPIO_InitStruct.Alternate = _AF;
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
