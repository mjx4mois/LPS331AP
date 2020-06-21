/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: pressure & temperature sensor
     File Name		: EXAMPLE_LPS331AP.c
     Function		: EXAMPLE_LPS331AP
     Create Date	: 2017/10/31
---------------------------------------------------------------------- */

#ifndef __LPS331AP_EXAMPLE__
#define __LPS331AP_EXAMPLE__

#include <mega32a.h>
#include <stdio.h>
#include <stdlib.h>
#include <delay.h>
#include <math.h>
#include <alcd.h>
#include <i2c.h>
#include "SENSOR_LPS331AP.h"
#include "Porting_Layer.h"

void EXAMPLE_LPS331AP(void);


void EXAMPLE_LPS331AP(void)
{
	CHAR8S status = 0;
	FLOAT pressure_data,temperature_data;
	INT32U data[3]={0};
	INT32U lcd_char_data[2][6]={0}; 

	
		/* let I2C bus release */
	
		status = LPS331AP_SET_INIT();
		if(status !=0)
		{
			printf("LPS331AP initial fail %d\r\n",status);	
		}
		else
		{
			printf("LPS331AP initial success\r\n");	
		}
						
		while(1)
		{
		


			/* get the temperature data & pressure data */
			status = LPS331AP_GET_DATA(&pressure_data,&temperature_data);
			
			if(status !=0)
			{
				printf("get LPS331AP data fail\r\n");
			}
			else
			{
				printf("pressure data = %f mbar[hPa]\r\n",pressure_data);
				printf("temperature data = %f C\r\n",temperature_data);
			}



			/* --------- Temperature bolck --------- */
			if(temperature_data<0)/* temperature < 0 */
			{				
				/* Temperautre */
				lcd_char_data[0][0] = (INT32U)(abs(temperature_data/100))%10;		/* use abs() */
				lcd_char_data[0][1] = (INT32U)(abs(temperature_data/10))%10;		/* use abs() */
				lcd_char_data[0][2] = (INT32U)(abs(temperature_data))%10;			/* use abs() */  
				lcd_char_data[0][3] = (INT32U)(abs(temperature_data*10))%10;		/* use abs() */   
				lcd_char_data[0][4] = (INT32U)(abs(temperature_data*100))%10; 		/* use abs() */  		
			}
			else /* temperature >= 0 */
			{
				/* Temperautre */
				lcd_char_data[0][0] = (INT32U)(temperature_data/100)%10;     
				lcd_char_data[0][1] = (INT32U)(temperature_data/10)%10;
				lcd_char_data[0][2] = (INT32U)(temperature_data)%10;  
				lcd_char_data[0][3] = (INT32U)(temperature_data*10)%10;   
				lcd_char_data[0][4] = (INT32U)(temperature_data*100)%10;   						
			}
				


			/* SHOW Temperautre */                    
	            	lcd_gotoxy(0,0);
	            	lcd_putsf("T:");     
	            	if(temperature_data<0)
	            	{
	            		lcd_putchar(45);		/* LCD show "-"  mean negative */
			}                             
	           	else
			{
				lcd_putchar(32);		/* LCD show " "  mean positive */
			}   
					
			/* show Temperautre data on LCD */
			lcd_putchar(48+lcd_char_data[0][0]);
			lcd_putchar(48+lcd_char_data[0][1]);
			lcd_putchar(48+lcd_char_data[0][2]);
			lcd_putchar(46);    /* print "."  */					
			lcd_putchar(48+lcd_char_data[0][3]);
			lcd_putchar(48+lcd_char_data[0][4]);
			lcd_putsf("C");
			/* --------- Temperature bolck --------- */



			/* ---------Pressure bolck --------- */
			{
				/* Pressure */
				lcd_char_data[0][0] = (INT32U)(pressure_data/1000)%10;     
				lcd_char_data[0][1] = (INT32U)(pressure_data/100)%10;
				lcd_char_data[0][2] = (INT32U)(pressure_data/10)%10;
				lcd_char_data[0][3] = (INT32U)(pressure_data)%10;  
				lcd_char_data[0][4] = (INT32U)(pressure_data*10)%10;   
				lcd_char_data[0][5] = (INT32U)(pressure_data*100)%10;   						
			}
			

			/* SHOW Pressure */                    
	            	lcd_gotoxy(0,1);
	            	lcd_putsf("P:");     

			/* show Temperautre data on LCD */
			lcd_putchar(48+lcd_char_data[0][0]);
			lcd_putchar(48+lcd_char_data[0][1]);
			lcd_putchar(48+lcd_char_data[0][2]);
			lcd_putchar(48+lcd_char_data[0][3]);
			lcd_putchar(46);    /* print "."  */					
			lcd_putchar(48+lcd_char_data[0][4]);
			lcd_putchar(48+lcd_char_data[0][5]);
			lcd_putsf("mbar");
			lcd_gotoxy(9,2);
			lcd_putsf("hPa");			
			/* --------- Temperature bolck --------- */


			/* --------- Show ID bolck --------- */
			lcd_gotoxy(0,3);
			lcd_putsf("LPS331AP");
			/* --------- Show ID bolck --------- */


			/* tiny delay */
			delay_ms(100);

			
		}
		
}

#endif		//#ifndef __LPS331AP_EXAMPLE__
