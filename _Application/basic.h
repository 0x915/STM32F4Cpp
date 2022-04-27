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
	
	void Write(uint16_t slave,uint16_t reg,uint8_t *data){
		HAL_I2C_Mem_Write(I2Cx,slave,reg,_MemSize,data,1,10);
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

class HSPI{
  private:
	SPI_HandleTypeDef *SPIx;
	uint8_t Buffer;
  public:
	HSPI(SPI_HandleTypeDef *SPIxx) : SPIx(SPIxx){}
	
	void Transmit(uint8_t *data,uint16_t size=1){
		HAL_SPI_Transmit(SPIx,data,size,35);
	}
	
	uint8_t Receive(uint8_t data){
		if(HAL_SPI_TransmitReceive(SPIx,&data,&Buffer,1,35) == HAL_OK)
			return Buffer;
		else return 0xff;
	}
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

typedef enum{
	FS8KHz = 0,
	FS4KHz = 1,
	FS2KHz = 3,
	FS1KHz = 7,
	FS500Hz = 15,
	FS250Hz = 31,
	FS125Hz = 63,
	FS100Hz = 79,
}MPU6050FS;
typedef enum{
	A2G,A4G,A8G,A16G,
}MPU6050ARange;
typedef enum{
	G250DPS,G500DPS,G1000DPS,G2000DPS,
}MPU6050GRange;

class MPU6050{
	#define I2CBusCLK6050 400000
  private:
	I2C_HandleTypeDef *I2Cx;
	uint8_t addr;
	uint8_t buffer[2] = {0};
  public:
	int16_t AccelX = 0,AccelY = 0,AccelZ = 0;
	int16_t GyrosX = 0,GyrosY = 0,GyrosZ = 0;
	float Temperature = 0.0;
	
	MPU6050(I2C_HandleTypeDef *i2cxx,uint8_t addr = 0xD0) : I2Cx(i2cxx),addr(addr){
		Init();
	}
	
	void Init(){
		I2C_Write(0x6B,0x80);       // 复位
		I2C_Write(0x6B,0x00);       // 唤醒
		I2C_Write(0X19,FS100Hz);     // 采样率
		I2C_Write(0x1C,A2G);       // 加速度量程
		I2C_Write(0x1B,G2000DPS);   // 陀螺仪量程
		I2C_Write(0X23,0x00);       // FIFO
		I2C_Write(0X38,0x00);       // Interrupt
		I2C_Write(0x6C,0x00);       // 使能
	}
	
	void CheckBusSpeed(){
		if(I2Cx->Init.ClockSpeed != I2CBusCLK6050){
			I2Cx->Init.ClockSpeed = I2CBusCLK6050;
			if(HAL_I2C_Init(I2Cx) != HAL_OK) Error_Handler();
		}
	};
	
	void I2C_Write(uint8_t cmd,uint8_t data){
		//CheckBusSpeed();
		HAL_I2C_Mem_Write(I2Cx,addr,cmd,I2C_MEMADD_SIZE_8BIT,&data,1,1000);
	}
	
	uint16_t I2C_Read(uint8_t mem,uint8_t size = 2){
		//CheckBusSpeed();
		if(HAL_I2C_Mem_Read(I2Cx,addr,mem,I2C_MEMADD_SIZE_8BIT,buffer,size,35) == HAL_OK){
			return buffer[1] << 8 | buffer[0] << 0;
		}else return 0xffff;
	}
	
	void Update(){
//		AccelX = I2C_Read(0x3B);
//		AccelY = I2C_Read(0x3D);
//		AccelZ = I2C_Read(0x3F);
		Temperature = I2C_Read(0x41) / 340.0 + 36.53;
		GyrosX += 1;//I2C_Read(0x43);
		GyrosY += 1;//I2C_Read(0x45);
		GyrosZ += 1;//I2C_Read(0x47);
	}
};

class ST7735S{
  private:
	SPI_HandleTypeDef *SPIx;
	GPIO *CS;
	GPIO *DC;
	GPIO *RE;
	uint8_t Buffer[2] = {0xaa};
  public:
	ST7735S(SPI_HandleTypeDef *SPIxx,GPIO_Port PortCS,GPIO_Port PortDC,GPIO_Port PortRE) : SPIx(SPIxx){
		CS = new GPIO(PortCS,OUT_PP,NO,SPEED);
		DC = new GPIO(PortDC,OUT_PP,NO,SPEED);
		RE = new GPIO(PortRE,OUT_PP,NO,SPEED);
		Init();
		//Init();
	}
	
	void Init(){
		RE->Write(1);
		Command(0x11);
		Command(0xB1);
		Data(0x01);
		Data(0x2C);
		Data(0x2D);
		Command(0xB2);
		Data(0x01);
		Data(0x2C);
		Data(0x2D);
		Command(0xB3);
		Data(0x01);
		Data(0x2C);
		Data(0x2D);
		Data(0x01);
		Data(0x2C);
		Data(0x2D);
		Command(0xB4);  //Column inversion
		Data(0x07);
		Command(0xC0);  //ST7735R Power Sequence
		Data(0xA2);
		Data(0x02);
		Data(0x84);
		Command(0xC1);
		Data(0xC5);
		Command(0xC2);
		Data(0x0A);
		Data(0x00);
		Command(0xC3);
		Data(0x8A);
		Data(0x2A);
		Command(0xC4);
		Data(0x8A);
		Data(0xEE);
		Command(0xC5);  //VCOM
		Data(0x0E);
		Command(0x36);  //MX, MY, RGB mode
		Data(0xC8);
		Command(0xe0);  //ST7735R Gamma Sequence
		Data(0x0f);
		Data(0x1a);
		Data(0x0f);
		Data(0x18);
		Data(0x2f);
		Data(0x28);
		Data(0x20);
		Data(0x22);
		Data(0x1f);
		Data(0x1b);
		Data(0x23);
		Data(0x37);
		Data(0x00);
		Data(0x07);
		Data(0x02);
		Data(0x10);
		Command(0xe1);
		Data(0x0f);
		Data(0x1b);
		Data(0x0f);
		Data(0x17);
		Data(0x33);
		Data(0x2c);
		Data(0x29);
		Data(0x2e);
		Data(0x30);
		Data(0x30);
		Data(0x39);
		Data(0x3f);
		Data(0x00);
		Data(0x07);
		Data(0x03);
		Data(0x10);
		Command(0x2a);
		Data(0x00);
		Data(0x00);
		Data(0x00);
		Data(0x7f);
		Command(0x2b);
		Data(0x00);
		Data(0x00);
		Data(0x00);
		Data(0x9f);
		Command(0xF0);  //Enable test command
		Data(0x01);
		Command(0xF6);  //Disable ram power save mode
		Data(0x00);
		Command(0x3A);  //65k mode
		Data(0x05);
		Command(0x29);  //Display on
	}
	
	void Command(uint8_t cmd){
		CS->Write(0);
		DC->Write(0);
		HAL_SPI_Transmit(SPIx,&cmd,1,35);
		CS->Write(1);
	}
	
	void Data(uint16_t data,uint8_t byte=2){
		Buffer[0] = (data >> 8) & 0xff;
		Buffer[1] = (data >> 0) & 0xff;
		CS->Write(0);
		DC->Write(1);
		HAL_SPI_Transmit(SPIx,Buffer,byte,35);
		CS->Write(1);
	}
	
	void SelectRegion(uint16_t X0,uint16_t Y0,uint16_t X1,uint16_t Y1){
		Command(0x2a);
		Data(X0);
		Data(Y0);
		Command(0x2b);
		Data(X1);
		Data(Y1);
		Command(0x2c);
	}
	
	void DrawString(){
	
	}
};
