
typedef enum{
	Page1 = 0,Page2,Page3,Page4,Page5,Page6,Page7,Page8
}pointer_page;

typedef enum{
	Col1 = 0,Col2,Col3,Col4,Col5,Col6,Col7,Col8,Col9,Col10,Col11,Col12,Col13,Col14,Col15,Col16,
	Col17,Col18,Col19,Col20,Col21,Col22,Col23,Col24,Col25,Col26,Col27,Col28,Col29,Col30,Col31,Col32,
	Col33,Col34,Col35,Col36,Col37,Col38,Col39,Col40,Col41,Col42,Col43,Col44,Col45,Col46,Col47,Col48,
	Col49,Col50,Col51,Col52,Col53,Col54,Col55,Col56,Col57,Col58,Col59,Col60,Col61,Col62,Col63,Col64,
	Col65,Col66,Col67,Col68,Col69,Col70,Col71,Col72,Col73,Col74,Col75,Col76,Col77,Col78,Col79,Col80,
	Col81,Col82,Col83,Col84,Col85,Col86,Col87,Col88,Col89,Col90,Col91,Col92,Col93,Col94,Col95,Col96,
	Col97,Col98,Col99,Col100,Col101,Col102,Col103,Col104,Col105,Col106,Col107,Col108,Col109,Col110,Col111,Col112,
	Col113,Col114,Col115,Col116,Col117,Col118,Col119,Col120,Col121,Col122,Col123,Col124,Col125,Col126,Col127,Col128
}pointer_col;

#include "font.h"

#define F8L6 H8W6
#define F12L8 H16W8
#define F16L10 H16W10

typedef enum{
	h8 = 8,h12 = 12,h16 = 16
}fonts;

class OLED1306{
	#define I2CBusCLK1306 400000
	#define CONFIG 0x00
	#define DRAW 0x40
  private:
	I2C_HandleTypeDef *I2Cx;
	uint8_t Address;
	uint8_t MaxPages;
	uint8_t PageLines;
	uint8_t MaxColumns;
  public:
	OLED1306(I2C_HandleTypeDef *i2cxx,uint8_t addr,uint8_t page,uint8_t line,uint8_t col) : I2Cx(i2cxx){
		Address = addr;
		MaxPages = page - 1;
		PageLines = line - 1;
		MaxColumns = col - 1;
		Init();
	}
	
	void CheckBusSpeed(){
		if(I2Cx->Init.ClockSpeed != I2CBusCLK1306){
			I2Cx->Init.ClockSpeed = I2CBusCLK1306;
			if(HAL_I2C_Init(I2Cx) != HAL_OK) Error_Handler();
		}
	};
	
	void I2C_Write(uint8_t cmd,uint8_t data){
		//CheckBusSpeed();
		HAL_I2C_Mem_Write(I2Cx,Address,cmd,I2C_MEMADD_SIZE_8BIT,&data,1,1000);
	}
	
	void Init(){
		I2C_Write(CONFIG,0xAE);  //设置屏幕关闭
		
		I2C_Write(CONFIG,0x20);  //设置内存地址模式
		I2C_Write(CONFIG,0x00);  //水平00b 垂直01b 页模式10b
		
		I2C_Write(CONFIG,0x81);  //设置对比度
		I2C_Write(CONFIG,0xff);  //最小00h 最大FFh
		
		I2C_Write(CONFIG,0xc8);  //列扫描方向
		
		I2C_Write(CONFIG,0xa1);  //设置段重映射
		
		I2C_Write(CONFIG,0xa8);  //设置复用率(1-64)
		I2C_Write(CONFIG,0x3f);  //
		
		I2C_Write(CONFIG,0xd3);  //设置显示偏移
		I2C_Write(CONFIG,0x00);  //
		
		I2C_Write(CONFIG,0xd5);  //设置显示时钟分频值/震荡频率
		I2C_Write(CONFIG,0x80);  //
		
		I2C_Write(CONFIG,0xd8);  //关闭区域颜色模式
		I2C_Write(CONFIG,0x05);  //
		
		I2C_Write(CONFIG,0xd9);  //设置预充电期 Set Pre-Charge Period
		I2C_Write(CONFIG,0xf1);  //
		
		I2C_Write(CONFIG,0xda);  //设置com引脚配置
		I2C_Write(CONFIG,0x12);  //
		
		I2C_Write(CONFIG,0xdb);  //设置vcomh set Vcomh
		I2C_Write(CONFIG,0x30);  //
		
		I2C_Write(CONFIG,0x8d);  //设置电源泵启用 set charge pump enable
		I2C_Write(CONFIG,0x14);  //
		
		I2C_Write(CONFIG,0xa4);  //设置全局显示	bit0，1白，0黑
		
		I2C_Write(CONFIG,0xa6);  //设置正常/反转显示(A6h/A7h)
		
		FillScreen(0x00);
		I2C_Write(CONFIG,0xAF);  //设置屏幕开启
	}
	
	void MovePointer(uint8_t page,uint8_t column){
		I2C_Write(CONFIG,0x22);      // PageReg
		I2C_Write(CONFIG,page);             // Page0
		I2C_Write(CONFIG,MaxPages);         // Page7
		I2C_Write(CONFIG,0x21);      // ColumnReg
		I2C_Write(CONFIG,column);           // Col000
		I2C_Write(CONFIG,MaxColumns);       // Col127
	}
	
	void FillScreen(uint8_t pbyte){
		MovePointer(0,0);
		for(int i = 0;i < (MaxPages + 1) * (MaxColumns + 1);i++){
			I2C_Write(DRAW,pbyte);
		}
	}
	
	void DrawBlock(uint8_t page,uint8_t col,uint8_t width,uint8_t pbyte = 0x00){
		MovePointer(page,col);
		for(int i = 0;i < width;i++){
			I2C_Write(DRAW,pbyte);
		}
	}
	
	void DrawString(uint8_t page,uint8_t col,fonts font,const char *str){
		uint8_t L8,H8,CW,PP = page,PC = col;
		for(int i = 0;str[i] > 0x00;i++){
			if(str[i] >= 0x20 && str[i] <= 0x7e){
				if(font == h8){
					CW = F12L8[str[i] - 0x20][8];
					MovePointer(PP,PC);
					for(int p = 0;p < CW;p++){
						L8 = F8L6[str[i] - 0x20][p];
						PC += 1;
						I2C_Write(DRAW,L8);
					}
				}
				if(font == h12){
					CW = F12L8[str[i] - 0x20][8];
					MovePointer(PP,PC);
					for(int p = 0;p < CW;p++){
						H8 = (F12L8[str[i] - 0x20][p]) >> 0 & 0xff;
						PC += 1;
						I2C_Write(DRAW,H8);
					}
					MovePointer(PP + 1,PC - CW);
					for(int p = 0;p < CW;p++){
						L8 = (F12L8[str[i] - 0x20][p]) >> 8 & 0xff;
						I2C_Write(DRAW,L8);
					}
				}
				if(font == h16){
					CW = F12L8[str[i] - 0x20][8];
					MovePointer(PP,PC);
					for(int p = 0;p < CW;p++){
						H8 = (F16L10[str[i] - 0x20][p]) >> 0 & 0xff;
						PC += 1;
						I2C_Write(DRAW,H8);
					}
					MovePointer(PP + 1,PC - CW);
					for(int p = 0;p < CW;p++){
						L8 = (F16L10[str[i] - 0x20][p]) >> 8 & 0xff;
						I2C_Write(DRAW,L8);
					}
				}
			}
		}
	}
	
};
