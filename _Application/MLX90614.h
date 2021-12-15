class MLX90614{
	#define RAMAccess90614 0x00
	#define ROMAccess90614 0x20
	#define WRAccess90614 0xBE<<8
	#define SMBusCLK90614 100000
  private:
	I2C_HandleTypeDef *I2Cx;
	uint8_t addr;
	uint8_t buffer[2] = {0};
  public:
	MLX90614(I2C_HandleTypeDef *I2Cxx,uint8_t addr) : I2Cx(I2Cxx),addr(addr){
		// MLX90614.h E (-40～C...85～C) K (-40～C＃125～C) SF (TO-39)
		// (1) Supply Voltage\Accuracy: A - 5V / B - 3V / C - Reserved  / D - 3V medical accuracy
		// (2) Number of thermopiles:   A 每 single zone / B 每 dual zone / C 每 gradient compensated*
		// (3) Package options: A 每 Standard package / B 每 Reserved / C 每 35～ FOV / D/E 每 Reserved
		//     F 每 10～ FOV / G 每 Reserved / H 每 12～ FOV (refractive lens) / I 每 5～ FOV / K 每 13～FOV
	}
	
	void CheckBusSpeed(){
		if(I2Cx->Init.ClockSpeed != SMBusCLK90614){
			I2Cx->Init.ClockSpeed = SMBusCLK90614;
			if(HAL_I2C_Init(I2Cx) != HAL_OK) Error_Handler();
		}
	};
	
	uint16_t I2C_Read(uint8_t mem,uint8_t size=2){
		CheckBusSpeed();
		if(HAL_I2C_Mem_Read(I2Cx,addr,mem,1,buffer,size,35) == HAL_OK)
			return buffer[1] << 8 | buffer[0] << 0;
		else return 0xffff;
	}

	float GetTarget(void){
		#define MIN90614OBJ 0x27AD // -70.00～C
		#define MAX90614OBJ 0x7FFF // 382.19～C
		float temp_float;
		uint16_t temp_hex = I2C_Read(0x07 | RAMAccess90614);
		if(temp_hex <= MIN90614OBJ)
			temp_float = -70.0;
		else if(temp_hex >= MAX90614OBJ)
			temp_float = +382.19;
		else temp_float = temp_hex * 0.02 - 273.15;
		return temp_float;
	}
	
	float GetAmbient(void){
		#define MIN90614AMB 0x2DE4 // -38.2～C
		#define MAX90614AMB 0x4DC4 // +125～C
		float temp_float;
		uint16_t temp_hex = I2C_Read(0x06 | RAMAccess90614);
		if(temp_hex <= MIN90614AMB)
			temp_float = -38.2;
		else if(temp_hex >= MAX90614AMB)
			temp_float = +125;
		else temp_float = temp_hex * 0.02 - 273.15;
		return temp_float;
	}
};
