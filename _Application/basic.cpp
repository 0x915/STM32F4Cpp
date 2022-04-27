#include "basic.h"
//#include "ssd1306.h"
//#include "MLX90614.h"
#include "main.hal.cpp"

static char StrCache[16] = {};
void main_(void){
	
	HSPI1::Init(A05,A06,A07,GPIO_AF5_SPI1);

	
	HI2C1::Init(B08,B09,GPIO_AF4_I2C1);
	HI2C I2Ch1(&HI2C1::I2C1Handle);
	
	HUART1::Init(B06,B07,GPIO_AF7_USART1);
	HUART Debug(&HUART1::UART1Handle);
	#define Print(str) Debug.SeedByteString(str)
	
	HTIM10::Init();
	Delay Time(&HTIM10::TIM10Handle);
	
	
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
	
//	OLED1306 TS(&HI2C1::I2C1Handle,0x78,8,8,128);
	
//	ST7735S LCD(&HSPI1::SPI1Handle,B01,B00,B02);
//
//	LCD.SelectRegion(0,0,127,127);
//	LCD.Data(0x0f);
	
//	TS.DrawString(Page1,Col1,h12,"1234567890");
//	TS.DrawString(Page3,Col1,h12,"F(xyz,ijk)!@#$%^&*");
//	TS.DrawString(Page5,Col1,h12,"<A>[B](C);");

//	MLX90614 DEV(&HI2C1::I2C1Handle,0x00);
//	float OBJ,AMB;
//	uint16_t ADDR = DEV.I2C_Read(0x0E|0x20,2);
//	sprintf(StrCache,"SMBus 0x%.4X",ADDR);
//	TS.DrawString(Page3,Col1,h12,StrCache);

//	MPU6050 AG(&HI2C1::I2C1Handle,0xD0);
	
	while(true){
		//LED13.Write(1);
		Time.ms(1000);
		LED13.Toggle();

//		AG.Update();
//		sprintf(StrCache,"T : %f ",AG.Temperature);
//		TS.DrawString(Page1,Col1,h12,StrCache);
//		sprintf(StrCache,"X : %x ",AG.GyrosX);
//		TS.DrawString(Page3,Col1,h12,StrCache);
//		sprintf(StrCache,"Y : %x ",AG.GyrosY);
//		TS.DrawString(Page5,Col1,h12,StrCache);
//		sprintf(StrCache,"Z : %x ",AG.GyrosZ);
//		TS.DrawString(Page7,Col1,h12,StrCache);



//		OBJ = DEV.GetTarget();
//		sprintf(StrCache,"OBJ : %.2lf C",OBJ);
//		TS.DrawString(Page7,Col1,h12,StrCache);
//		AMB = DEV.GetAmbient();
//		sprintf(StrCache,"AMB : %.2lf C",AMB);
//		TS.DrawString(Page5,Col1,h12,StrCache);
	}
}




