#include "common.h"
#include "include.h"
#include "24TFT_Font.h"
/***************************************************************************************
STM32����ƽ̨����:
�����壺����ԭ��MiniSTM32������
MCU ��STM32_F103_RBT6
���� ��12MHZ
��Ƶ ��72MHZ
����˵��:
//-------------------------------------------------------------------------------------
#define LCD_CTRL   	  	GPIOB		//����TFT���ݶ˿�
#define LCD_LED        	GPIO_Pin_9  //PB9 ������TFT -LED
#define LCD_RS         	GPIO_Pin_10	//PB10������TFT --RS
#define LCD_CS        	GPIO_Pin_11 //PB11 ������TFT --CS
#define LCD_RST     	  GPIO_Pin_12	//PB12������TFT --RST
#define LCD_SCL        	GPIO_Pin_13	//PB13������TFT -- CLK
#define LCD_SDA        	GPIO_Pin_15	//PB15������TFT - SDI
//VCC:���Խ�5VҲ���Խ�3.3V
//LED:���Խ�5VҲ���Խ�3.3V����ʹ���������IO����(�ߵ�ƽʹ��)
//GND���ӵ�Դ��
//˵��������Ҫ��������ռ��IO�����Խ�LCD_CS�ӵأ�LCD_LED��3.3V��LCD_RST������Ƭ����λ�ˣ�
//�������ͷ�3������IO
//�ӿڶ�����Lcd_Driver.h�ڶ��壬
//������IO�ӷ������������ʵ�ʽ����޸���ӦIO��ʼ��LCD_GPIO_Init()
//-----------------------------------------------------------------------------------------
���̹���˵����
1.	��ˢ������
2.	Ӣ����ʾ����ʾ��
3.	������ʾ����ʾ��
4.	�����������ʾʾ��
5.	ͼƬ��ʾʾ��
6.	2D�����˵�ʾ��
7.	������֧�ֺ���/�����л�(������USE_HORIZONTAL,���Lcd_Driver.h)
8.	������֧�����ģ��SPI/Ӳ��SPI�л�(������USE_HARDWARE_SPI,���Lcd_Driver.h)


ILI9341��Ļ��С��320x240

**********************************************************************************************/


//---------------------------------function----------------------------------------------------//

/****************************************************************************
* ��    �ƣ�void LCD_GPIO_Init(void)
* ��    �ܣ�STM32_ģ��SPI���õ���GPIO��ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ������ʼ��ģ��SPI���õ�GPIO
****************************************************************************/
void LCD24_GPIO_Init(void)
{
  
gpio_init(PTD0, GPO, 0);
gpio_init(PTD1, GPO, 0);
gpio_init(PTD3, GPO, 0);
gpio_init(PTD5, GPO, 0);  

//  spi_init(SPI0,SPIn_PCS0, MASTER,10*1000*1000);
}
/****************************************************************************
* ��    �ƣ�void  SPIv_WriteData(u8 Data)
* ��    �ܣ�STM32_ģ��SPIдһ���ֽ����ݵײ㺯��
* ��ڲ�����Data
* ���ڲ�������
* ˵    ����STM32_ģ��SPI��дһ���ֽ����ݵײ㺯��
****************************************************************************/
void  SPIv_WriteData(uint8 Data)
{
  
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
                    gpio_set(PTD3,1); //�������
                else 
                    gpio_set(PTD3,0);	   
      gpio_set(PTD5,0);       
      gpio_set(PTD5,1);
      Data<<=1; 
	}

//  spi_init(SPI0,SPIn_PCS0, MASTER,10*1000*1000);
}

/****************************************************************************
* ��    �ƣ�Lcd_WriteIndex(u8 Index)
* ��    �ܣ���Һ����дһ��8λָ��
* ��ڲ�����Index   �Ĵ�����ַ
* ���ڲ�������
* ˵    ��������ǰ����ѡ�п��������ڲ�����
****************************************************************************/
void Lcd24_WriteIndex(uint8 Index)
{
   //LCD_CS_CLR;
   gpio_set(PTD1,0);
   SPIv_WriteData(Index);
   //LCD_CS_SET;
}

/****************************************************************************
* ��    �ƣ�Lcd_WriteData(u8 Data)
* ��    �ܣ���Һ����дһ��8λ����
* ��ڲ�����dat     �Ĵ�������
* ���ڲ�������
* ˵    �����������ָ����ַд�����ݣ��ڲ�����
****************************************************************************/
void Lcd24_WriteData(uint8 Data)
{
   //LCD_CS_CLR;
   gpio_set(PTD1,1);
   SPIv_WriteData(Data);
   //LCD_CS_SET;
}

/****************************************************************************
* ��    �ƣ�void LCD_WriteReg(u8 Index,u16 Data)
* ��    �ܣ�д�Ĵ�������
* ��ڲ�����Index,Data
* ���ڲ�������
* ˵    ����������Ϊ��Ϻ�������Index��ַ�ļĴ���д��Dataֵ
****************************************************************************/
void LCD24_WriteReg(uint8 Index,uint16 Data)
{
	Lcd24_WriteIndex(Index);
  	Lcd24_WriteData_16Bit(Data);
}

/****************************************************************************
* ��    �ƣ�void Lcd_WriteData_16Bit(u16 Data)
* ��    �ܣ���Һ����дһ��16λ����
* ��ڲ�����Data
* ���ڲ�������
* ˵    �����������ָ����ַд��һ��16λ����
****************************************************************************/
void Lcd24_WriteData_16Bit(uint16 Data)
{	
	Lcd24_WriteData(Data>>8);
	Lcd24_WriteData(Data);	
}

/****************************************************************************
* ��    �ƣ�void Lcd_Reset(void)
* ��    �ܣ�Һ��Ӳ��λ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����Һ����ʼ��ǰ��ִ��һ�θ�λ����
****************************************************************************/
void Lcd24_Reset(void)
{
	LCD_RST_CLR;
	DELAY_MS(100);//��ʱ100ms
	LCD_RST_SET;
	DELAY_MS(50);
}
/****************************************************************************
* ��    �ƣ�void Lcd_Init(void)
* ��    �ܣ�Һ����ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����Һ����ʼ��_ILI9225_176X220
****************************************************************************/
void Lcd24_Init(void)
{	

	LCD24_GPIO_Init();
	Lcd24_Reset(); //Reset before LCD Init.

	//2.2inch TM2.2-G2.2 Init 20171020 
	Lcd24_WriteIndex(0x11);  
	Lcd24_WriteData(0x00); 

	Lcd24_WriteIndex(0xCF);  
	Lcd24_WriteData(0X00); 
	Lcd24_WriteData(0XC1); 
	Lcd24_WriteData(0X30);

	Lcd24_WriteIndex(0xED);  
	Lcd24_WriteData(0X64); 
	Lcd24_WriteData(0X03); 
	Lcd24_WriteData(0X12);
	Lcd24_WriteData(0X81);

	Lcd24_WriteIndex(0xE8);  
	Lcd24_WriteData(0X85); 
	Lcd24_WriteData(0X11); 
	Lcd24_WriteData(0X78);

	Lcd24_WriteIndex(0xF6);  
	Lcd24_WriteData(0X01); 
	Lcd24_WriteData(0X30); 
	Lcd24_WriteData(0X00);

	Lcd24_WriteIndex(0xCB);  
	Lcd24_WriteData(0X39); 
	Lcd24_WriteData(0X2C); 
	Lcd24_WriteData(0X00);
	Lcd24_WriteData(0X34);
	Lcd24_WriteData(0X05);
           
	Lcd24_WriteIndex(0xF7);  
	Lcd24_WriteData(0X20); 
           
	Lcd24_WriteIndex(0xEA);  
	Lcd24_WriteData(0X00); 
	Lcd24_WriteData(0X00); 
           
	Lcd24_WriteIndex(0xC0);  
	Lcd24_WriteData(0X20); 
           
	Lcd24_WriteIndex(0xC1);  
	Lcd24_WriteData(0X11); 
           
	Lcd24_WriteIndex(0xC5);  
	Lcd24_WriteData(0X31); 
	Lcd24_WriteData(0X3C); 
           
	Lcd24_WriteIndex(0xC7);  
	Lcd24_WriteData(0XA9); 
           
	Lcd24_WriteIndex(0x3A);  
	Lcd24_WriteData(0X55); 
	   
        Lcd24_WriteIndex(0x36);  
	#if USE_HORIZONTAL
		 Lcd24_WriteData(0xE8);//��������
	#else
		 Lcd24_WriteData(0x48);//�������� 
	#endif

	Lcd24_WriteIndex(0xB1);  
	Lcd24_WriteData(0X00); 
	Lcd24_WriteData(0X18); 

	Lcd24_WriteIndex(0xB4);  
	Lcd24_WriteData(0X00); 
	Lcd24_WriteData(0X00); 

	Lcd24_WriteIndex(0xF2);  
	Lcd24_WriteData(0X00); 

	Lcd24_WriteIndex(0x26);  
	Lcd24_WriteData(0X01); 

	Lcd24_WriteIndex(0xE0);  
	Lcd24_WriteData(0X0F); 
	Lcd24_WriteData(0X17); 
	Lcd24_WriteData(0X14); 
	Lcd24_WriteData(0X09); 
	Lcd24_WriteData(0X0C); 
	Lcd24_WriteData(0X06); 
	Lcd24_WriteData(0X43); 
	Lcd24_WriteData(0X75); 
	Lcd24_WriteData(0X36); 
	Lcd24_WriteData(0X08); 
	Lcd24_WriteData(0X13); 
	Lcd24_WriteData(0X05); 
	Lcd24_WriteData(0X10); 
	Lcd24_WriteData(0X0B); 
	Lcd24_WriteData(0X08); 


	Lcd24_WriteIndex(0xE1);  
	Lcd24_WriteData(0X00); 
	Lcd24_WriteData(0X1F); 
	Lcd24_WriteData(0X23); 
	Lcd24_WriteData(0X03); 
	Lcd24_WriteData(0X0E); 
	Lcd24_WriteData(0X04); 
	Lcd24_WriteData(0X39); 
	Lcd24_WriteData(0X25); 
	Lcd24_WriteData(0X4D); 
	Lcd24_WriteData(0X06); 
	Lcd24_WriteData(0X0D); 
	Lcd24_WriteData(0X0B); 
	Lcd24_WriteData(0X33); 
	Lcd24_WriteData(0X37); 
	Lcd24_WriteData(0X0F); 

	Lcd24_WriteIndex(0x29); 
        Lcd24_Clear(_WHITE);
}



/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void Lcd24_SetXY(uint16 Xpos, uint16 Ypos)
{	
	Lcd24_WriteIndex(0x2A);
	Lcd24_WriteData_16Bit(Xpos);
	Lcd24_WriteIndex(0x2B);
	Lcd24_WriteData_16Bit(Ypos);
	Lcd24_WriteIndex(0x2c);	
} 
/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
//������ʾ����
void Lcd24_SetRegion(uint16 xStar, uint16 yStar,uint16 xEnd,uint16 yEnd)
{
	Lcd24_WriteIndex(0x2A);
	Lcd24_WriteData_16Bit(xStar);
	Lcd24_WriteData_16Bit(xEnd);
	Lcd24_WriteIndex(0x2B);
	Lcd24_WriteData_16Bit(yStar);
	Lcd24_WriteData_16Bit(yEnd);
	Lcd24_WriteIndex(0x2c);
}

	
/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�����xy�������ɫ����
����ֵ����
*************************************************/
void Gui_DrawPoint(uint16 x,uint16 y,uint16 Data)
{
	Lcd24_SetXY(x,y);
	Lcd24_WriteData_16Bit(Data);

}    

/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�����xy�������ɫ����
����ֵ����
*************************************************/
void DrawRectangle(uint16 x,uint16 y,uint16 W,uint16 H,uint16 Data)
{
  uint16 i,j;
  for(i=0;i<H;i++)
  {
    for(j=0;j<W;j++)
    {
      Gui_DrawPoint(x+j,y+i,Data);
    }
  }  
}    

/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void Lcd24_Clear(uint16 Color)               
{	
   unsigned int i;
   Lcd24_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
	 //LCD_CS_CLR;
   LCD_RS_SET;	
   for(i=0;i<X_MAX_PIXEL*Y_MAX_PIXEL;i++)
   {	
	  	//Lcd_WriteData_16Bit(Color);
		SPIv_WriteData(Color>>8);
		SPIv_WriteData(Color); 
   }   
		//LCD_CS_SET;
}



//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
uint16 LCD_BGR2RGB(uint16 c)
{
  uint16  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);

}




void Gui_Circle(uint16 X,uint16 Y,uint16 R,uint16 fc) //����һ��Բ  fcΪ��ɫ����
{//Bresenham�㷨 
    unsigned short  a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc);     //        7 
        Gui_DrawPoint(X-a,Y+b,fc);     //        6 
        Gui_DrawPoint(X+a,Y-b,fc);     //        2 
        Gui_DrawPoint(X-a,Y-b,fc);     //        3 
        Gui_DrawPoint(X+b,Y+a,fc);     //        8 
        Gui_DrawPoint(X-b,Y+a,fc);     //        5 
        Gui_DrawPoint(X+b,Y-a,fc);     //        1 
        Gui_DrawPoint(X-b,Y-a,fc);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y-b,fc); 
        Gui_DrawPoint(X-a,Y-b,fc); 
        Gui_DrawPoint(X+b,Y+a,fc); 
        Gui_DrawPoint(X-b,Y+a,fc); 
        Gui_DrawPoint(X+b,Y-a,fc); 
        Gui_DrawPoint(X-b,Y-a,fc); 
    } 
	
} 
//���ߺ�����ʹ��Bresenham �����㷨
void Gui_DrawLine(uint16 x0, uint16 y0,uint16 x1, uint16 y1,uint16 Color) //��б��  
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	


	Lcd24_SetXY(x0,y0);
	dx = x1-x0;//����x����
	dy = y1-y0;//����y����

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x�������y���룬��ôÿ��x����ֻ��һ���㣬ÿ��y���������ɸ���
	{//���ߵĵ�������x���룬��x���������
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//Ҫ���ĵ������ᳬ��x����
		{
			//����
			Gui_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //�Ƿ���Ҫ����y����ֵ
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//����y����ֵ
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x����ֵÿ�λ���󶼵���1
		} // end for
	} // end if |slope| <= 1
	else//y�����x�ᣬ��ÿ��y����ֻ��һ���㣬x�����ɸ���
	{//��y��Ϊ��������
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			Gui_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}



void Gui_box(uint16 x, uint16 y, uint16 w, uint16 h,uint16 bc)//bc������ɫ
{
	Gui_DrawLine(x,y,x+w,y,0xEF7D);
	Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
	Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
	Gui_DrawLine(x,y,x,y+h,0xEF7D);
        Gui_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}
void Gui_box2(uint16 x,uint16 y,uint16 w,uint16 h, uint8 mode)
{
	if (mode==0)	{
		Gui_DrawLine(x,y,x+w,y,0xEF7D);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
		Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
		Gui_DrawLine(x,y,x,y+h,0xEF7D);
		}
	if (mode==1)	{
		Gui_DrawLine(x,y,x+w,y,0x2965);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
		Gui_DrawLine(x,y+h,x+w,y+h,0xEF7D);
		Gui_DrawLine(x,y,x,y+h,0x2965);
	}
	if (mode==2)	{
		Gui_DrawLine(x,y,x+w,y,0xffff);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
		Gui_DrawLine(x,y+h,x+w,y+h,0xffff);
		Gui_DrawLine(x,y,x,y+h,0xffff);
	}
}


/**************************************************************************************
��������: ����Ļ��ʾһ͹��İ�ť��
��    ��: uint16 x1,y1,x2,y2 ��ť�����ϽǺ����½�����
��    ��: ��
**************************************************************************************/
void DisplayButtonDown(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, _GRAY2);  //H
	Gui_DrawLine(x1+1,y1+1,x2,y1+1, _GRAY1);  //H
	Gui_DrawLine(x1,  y1,  x1,y2, _GRAY2);  //V
	Gui_DrawLine(x1+1,y1+1,x1+1,y2, _GRAY1);  //V
	Gui_DrawLine(x1,  y2,  x2,y2, _WHITE);  //H
	Gui_DrawLine(x2,  y1,  x2,y2, _WHITE);  //V
}

/**************************************************************************************
��������: ����Ļ��ʾһ���µİ�ť��
��    ��: uint16 x1,y1,x2,y2 ��ť�����ϽǺ����½�����
��    ��: ��
**************************************************************************************/
void DisplayButtonUp(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, _WHITE); //H
	Gui_DrawLine(x1,  y1,  x1,y2, _WHITE); //V
	
	Gui_DrawLine(x1+1,y2-1,x2,y2-1, _GRAY1);  //H
	Gui_DrawLine(x1,  y2,  x2,y2, _GRAY2);  //H
	Gui_DrawLine(x2-1,y1+1,x2-1,y2, _GRAY1);  //V
    Gui_DrawLine(x2  ,y1  ,x2,y2, _GRAY2); //V
}

/*��ʾ����Ϊ  16  ������ Ӣ�� ���� �ַ�*/
void Gui_DrawFont_GBK16(uint16 x, uint16 y, uint16 fc, uint16 bc, uint8 *s)  //fc������ɫ  bc������ɫ
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;

	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
	
			    for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
					{
				    	if(asc16[k*16+i]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
						}
					}
				x+=8;
			}
			s++;
		}
			
		else 
		{
		

			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		} 
		
	}
}

void Gui_DrawFont_GBK24(uint16 x, uint16 y, uint16 fc, uint16 bc, uint8 *s)//fc������ɫ  bc������ɫ
{
unsigned char i,j;
unsigned short k;

while(*s) 
{
if( *s < 0x80 ) 
{
	k=*s;
	if (k>32) k-=32; else k=0;

        for(i=0;i<16;i++)
	for(j=0;j<8;j++) 
		{
	    	if(asc16[k*16+i]&(0x80>>j))	
			Gui_DrawPoint(x+j,y+i,fc);
			else 
			{
				if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
			}
		}
	s++;x+=8;
}
else 
{

	for (k=0;k<hz24_num;k++) 
	{
	  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
	  { 
		    for(i=0;i<24;i++)
		    {
				for(j=0;j<8;j++) 
					{
				    	if(hz24[k].Msk[i*3]&(0x80>>j))
						Gui_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
						}
					}
				for(j=0;j<8;j++) 
					{
				    	if(hz24[k].Msk[i*3+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
						else {
							if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
						}
					}
				for(j=0;j<8;j++) 
					{
				    	if(hz24[k].Msk[i*3+2]&(0x80>>j))	
						Gui_DrawPoint(x+j+16,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j+16,y+i,bc);
						}
					}
		    }
	  }
	}
	s+=2;x+=24;
}
}
}
void Gui_DrawFont_Num32(uint16 x, uint16 y, uint16 fc, uint16 bc, uint16 num)//fc������ɫ  bc������ɫ
{
	unsigned char i,j,k,c;
	//lcd_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
//	w=w/8;

    for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++) 
		{
			c=*(sz32+num*32*4+i*4+j);
			for (k=0;k<8;k++)	
			{
	
		    	if(c&(0x80>>k))	Gui_DrawPoint(x+j*8+k,y+i,fc);
				else {
					if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
				}
			}
		}
	}
}


