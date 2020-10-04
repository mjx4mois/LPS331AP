/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: pressure & temperature sensor
     File Name		: SENSOR_LPS331AP.c
     Function		: SENSOR_LPS331AP
     Create Date	: 2017/10/31
---------------------------------------------------------------------- */

#include <stdio.h>
#include <math.h>
#include <delay.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_LPS331AP.h>


#define LPS331_DEBUG			(1)		/* set "1" to printf debug message */


/********************************************* SYSTEM *************************************************/
/*--------------------------------------------------------------------------------------------------*/
/* initial LPS331AP */
CHAR8S LPS331AP_SET_INIT(void)
{
	CHAR8S status = 0;
	CHAR8U id=0;
	CHAR8U read_data;

	
		/* read LPS331AP ID */
		status =LPS331AP_GET_ID(&id);
		if(status !=0) 
		{
			return -1;	/*read fail*/
		}
		else
		{
			printf("LPS331AP id = 0x%X\r\n",id); /* LPS331AP ID must be 0xBB */
		}


		/* set LPS331AP resolution & noise (LPS331AP internal average data) */		
		status = LPS331AP_SET_NOISE_ADJ();
		if(status !=0)
		{
			return -3; /* fail*/
		}
		

		/* set data rate & power active & DBU */	
		status = LPS331AP_SET_REG_CTRL1();
		if(status !=0) 
		{
			return -2; /* fail*/
		}
				
 
		
		/*--------- INTERRUPT INITAIL --------*/

		/* set the interrupt pin status */
		status = LPS331AP_SET_INT_PIN_STATUS();
		if(status !=0)
		{
			return -5;/* fail*/
		}

		/* set the interrupt config */
		status = LPS331AP_SET_INT_CFG();
		if(status !=0) 
		{
			return -6; /* fail*/
		}

		/* set the interrupt threshold */		
		status = LPS331AP_SET_INT_THD_PRESSURE(1000);	/*SET 1000mbar */
		if(status !=0)
		{
			return -7; /* fail*/
		}
		/*--------- INTERRUPT INITAIL --------*/

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*read LPS3311AP id  ; id = 0xBB*/
CHAR8S LPS331AP_GET_ID(CHAR8U *id)
{
	CHAR8U read_data ;
	CHAR8S status = 0;

		/* read LPS331AP ID */
		status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_WHO_AM_I,&read_data);
		if(status !=1) 
		{
			return -1;		/*read fail*/
		}
		
		*id = read_data;	/*read success*/
		return 0;

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	check LPS331AP Pressure data & Temperature data already?
	if return -1 -> read the register fail [I2S fail]
	if status return 0x01 -> Temperature data ready
	if status return 0x02 -> Pressure data ready

	The content of this register is updated every ODR cycle, regardless of BDU value in CTRL_REG1.
	bit 5 [P_OR] def:0  Pressure data overrun   		"0" -> no overrun has occurred ;   	"1" ->A new data for pressure has overwritten the previous one
	bit 4 [T_OR] def:0  Temperature data overrun		"0" -> no overrun has occurred ;  	"1" -> A new data for temperature has overwritten the previous one
	bit 1 [P_DA] def:0  Pressure data available.			"0" -> Pressure data not ready ;   	"1" -> new data for pressure is available
	bit 0 [T_DA] def:0  Temperature data available.		"0" -> Temperature data not ready;   	"1" -> new data for temperature is available
*/
CHAR8S LPS331AP_GET_DATA_READY(CHAR8U *status)
{
	CHAR8S status_function = 0;
	CHAR8U temp_data=0;	
		
        	i2c_stop_hang();
		i2c_stop_hang();
		
		status_function = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_STATUS,&temp_data);
		if(status_function!=1) 
		{
			return -1;	/* read fail*/
		}

		/*return status*/
		*status = temp_data ; 
		return 0;		
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* set [PD],[ODR2, ODR1, ODR0],[BDU]*/
CHAR8S LPS331AP_SET_REG_CTRL1(void)
{
	CHAR8S status = 0;
	CHAR8U read_data =0;
	
		/* write the LPS331AP_SPEED_SET into LPS331AP_REG_CTRL1 register */
		status = i2c_write_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_CTRL1,LPS331AP_SET_REGCTRL1);
		if(status !=1)
		{
			printf("write LPS331AP_REG_CTRL1 fail\r\n");
			return -1;	/*write fail*/
		}
		else
		{
			printf("write LPS331AP_REG_CTRL1 success\r\n");
		}


#if 0
		/*enable auto zero mode*/
		status = i2c_write_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_CTRL2,LPS331AP_AUTO_ZERO);
		if(status !=1) 
		{
			return -1; 	/*write fail*/
		}
#endif		


		return 0;	/*write success*/

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* set the resolution */
/* set the pressure internal average & temperature internal average*/
/* LPS331AP_REG_RES_CONF : default 0x7A*/
CHAR8S LPS331AP_SET_NOISE_ADJ(void)
{
	CHAR8S status = 0;
	
#if LPS331_DEBUG
		printf("Set resolution reg[0x%x]=0x%x\r\n",LPS331AP_REG_RES_CONF,LPS331AP_RESOLUTION);
#endif

		/* set LPS331AP noise (LPS331AP internal average data) */
		status = i2c_write_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_RES_CONF,LPS331AP_RESOLUTION);
		if(status !=1) 
		{
			return -1;	/*write fail*/
		}
		else
		{
			return 0;	/*write success*/
		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
//Software Reset the LPS331AP */
CHAR8S LPS331AP_SET_RESET(void)
{
	CHAR8S status = 0;
	
		/* set LPS331AP software reset */	
		status = i2c_write_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_CTRL2,LPS331AP_SW_RESET);
		if(status !=1)
		{
			return -1;/* write fail*/
		}
		else
		{
			return 0;/*write success*/
		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*get Pressure & Temperature data */
CHAR8S LPS331AP_GET_DATA(FLOAT *pressure,FLOAT *temperature)
{
	CHAR8U read_data,write_data ;
	CHAR8S status = 0;
	CHAR8U cnt,timeout = 1000;
	CHAR8U pressure_temp_raw_data[5]={0};
	CHAR8U rdata=0;
	INT32S P_RAW;
	INT16S T_RAW;
	FLOAT P_RESULT,T_RESULT;

		
		/*check outupt speed rate , LPS331AP_SPEED_SET = ONE SHOT , SET ONE SHOT MODE */
		//	printf("LPS331AP_SET_REGCTRL11111111 %x\r\n,",(LPS331AP_SET_REGCTRL1 & 0x70) );
		if((LPS331AP_SET_REGCTRL1 & 0x70) == 0)/*check bit6~4 = '000' is ONE SHOT mode in LPS331AP_REG_CTRL1 */
		{
		
			printf("LPS331AP_GET_DATA -> one shot mode\r\n");

			i2c_stop_hang();
			
			/*first read  */
			status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_CTRL2,&read_data);
			if(status !=1)
			{
				return -1;	/*read fail */
			}

			i2c_stop_hang();


#if LPS331_DEBUG
			printf("write LPS331AP_GET_DATA reg[0x%x]=0x%x\r\n",LPS331AP_REG_CTRL2,LP331AP_ONE_SHOT_MODE); 
#endif
			
			status = i2c_write_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_CTRL2,LP331AP_ONE_SHOT_MODE);/* write_data must be 0x01 to trigger ONE SHOT mode */
			if(status !=1) 
			{
				return -1; 	/*write fail*/
			}


		}
		else
		{	
			printf("LPS331AP_GET_DATA -> continue mode ,see LPS331AP_SET_REGCTRL1\r\n");
		}


		/****** wait data ready ******/
		for(cnt = 0 ;cnt<=timeout;cnt++)
		{

			i2c_stop_hang();
			i2c_stop_hang();
			
			/******* need to check again *******/
			/* double check ? */	
			status = LPS331AP_GET_DATA_READY(&read_data);
			if(status !=0) 
			{
				printf("Get status fail \r\n");
				return -1; 	/*write fail*/
			}
			
#if LPS331_DEBUG			
			printf("LPS331AP_REG_STATUS = [0x%x]\r\n",read_data);
#endif			

			/*
			bit 5 [P_OR] def:0  Pressure data overrun   		"0" -> no overrun has occurred ;   	"1" ->A new data for pressure has overwritten the previous one
			bit 4 [T_OR] def:0  Temperature data overrun		"0" -> no overrun has occurred ;  	"1" -> A new data for temperature has overwritten the previous one
			bit 1 [P_DA] def:0  Pressure data available.			"0" -> Pressure data not ready ;   	"1" -> new data for pressure is available
			bit 0 [T_DA] def:0  Temperature data available.		"0" -> Temperature data not ready;   	"1" -> new data for temperature is available
			*/
			if((read_data & 0x03) == 0x03)/* check bit 1 & bit 0 */
			{
				printf("Temp & Press data ready! 0x%x\r\n",read_data);
				break; 	/* check Pressure & Temperaute ok */
			}

			/* if use one shot mode */
			if((LPS331AP_SET_REGCTRL1 & 0x70) == 0)/*check bit6~4 = '000' is ONE SHOT mode in LPS331AP_REG_CTRL1 */
			{		
				/* read LPS331AP_REG_CTRL2 to check */
				status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_CTRL2,&read_data);
				if(read_data == 0x00)
				{
					break;
				}
			}
			
			delay_ms(1);
		}


		if(cnt>=1000) 
		{
		
#if LPS331_DEBUG			
			printf("LPS331AP wait data timeout \r\n");
#endif		
			return -2;	/*timeout*/
		}


		i2c_stop_hang();
		i2c_stop_hang();

		
		/*READ pressure & temperature data*/
		/* must be one byte read . if use muti-read , the data will read error */
#if 0
		/* OR 0x80 -> enable multi-read */
		status = i2c_read_n_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_PRESS_POUT_XL_REH | 0x80,5,&pressure_temp_raw_data[0]);

#else
		/* read temperature */
		{
			status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_TEMP_OUT_L,&rdata);
			if(status !=1) 
			{
				return -1;	/*read fail*/
			}
			pressure_temp_raw_data[3]=rdata;	/*write data*/

			//delay_ms(1);
			
			status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_TEMP_OUT_H,&rdata);
			if(status !=1) 
			{
				return -1;	/*read fail*/
			}
			pressure_temp_raw_data[4]=rdata;	/*write data*/	
			
			//delay_ms(1);
		}

		/* read pressure */	
		{	
			status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_PRESS_POUT_XL_REH ,&rdata);

			if(status !=1) 
				
			{
				return -1;	/*read fail*/
			}			
			pressure_temp_raw_data[0]=rdata;	/*write data*/

			//delay_ms(1);

			status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_PRESS_OUT_L,&rdata);
			if(status !=1) 
			{
				return -1;	/*read fail*/
			}
			pressure_temp_raw_data[1]=rdata;	/*write data*/

			//delay_ms(1);
				
			status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_PRESS_OUT_H,&rdata);
			if(status !=1) 
			{
				return -1;	/*read fail*/
			}
			pressure_temp_raw_data[2]=rdata;	/*write data*/

		}
#endif

#if 0

		/* read pressure */	
		{	
			status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_REF_P_XL,&rdata);
			if(status !=1) 
			{
				return -1;	/*read fail*/
			}			
			printf("LPS331AP_REG_REF_P_XL  0x%lx \r\n",rdata);

			//delay_ms(1);
			
			status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_REF_P_L,&rdata);
			if(status !=1) 
			{
				return -1;	/*read fail*/
			}
		
			printf("LPS331AP_REG_REF_P_L  0x%lx \r\n",rdata);
			//delay_ms(1);
			
			status = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_REF_P_H,&rdata);
			if(status !=1) 
			{
				return -1;	/*read fail*/
			}
			printf("LPS331AP_REG_REF_P_H  0x%lx \r\n",rdata);
		}
#endif

		
		/* turn value */
		P_RAW = (INT32S)( ((INT32U)pressure_temp_raw_data[2]<<16) | ((INT32U)pressure_temp_raw_data[1]<<8) |((INT32U)pressure_temp_raw_data[0]<<0) ) ;//& 0xffffff;/*mask 24bit*/
		T_RAW = (INT16S)( ((INT16U)pressure_temp_raw_data[4]<<8) |((INT16U)pressure_temp_raw_data[3]<<0)) & 0xffff;/*mask 16bit*/


#if LPS331_DEBUG
		for(cnt = 0 ;cnt<5;cnt++)
		{
			printf("pressure_temp_raw_data [%d]= 0x%X[%d]\r\n",cnt,pressure_temp_raw_data[cnt],pressure_temp_raw_data[cnt]);
		}
		printf("P_RAW = 0x%lx , T_RAW = 0x%lx \r\n",P_RAW,T_RAW);
#endif

		/* calculate */
		LPS331AP_GET_CALCULATE(P_RAW,T_RAW,&P_RESULT,&T_RESULT);

		/* GET VALUE */
		*pressure = P_RESULT;		/* final pressure data*/
		*temperature = T_RESULT;	/* final temperature data*/
		
		return 0;		
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*to calculate the Pressure data & Temperature data*/
CHAR8S LPS331AP_GET_CALCULATE(INT32U P_DATA,INT16U T_DATA,FLOAT *P_RESULT , FLOAT *T_RESULT)
{

	FLOAT temp_data;
	INT32U temp_data2;
	INT16U temp_data3;


	/* check the calculate correct or not */
	#if 0
		//P_DATA = 0x3FF58D; /*postivie*/ /* result :1023.3 mbar*/
		P_DATA = 0xF21EA4; /*negative*/ /* result :-222.08mbar*/
		//T_DATA = 0x6BD0 ;/*postivie*/ /* result :100 C*/
		T_DATA = 0xE07C; /*negative*/ /* result :25.7 C*/
	#endif 

	
#if LPS331_DEBUG
		printf("P_DATA sign 23bit = 0x%lX \r\n",(P_DATA&0x800000));
#endif	


		/*calculate pressure data*/
		/*check negative or positive [bit24]*/
		if(P_DATA & 0x800000) 
		{
			/*negative*/
			temp_data2 = ((~P_DATA) & 0xffffff);	/* mask 24bit*/
			temp_data2+=1;
			temp_data = -(FLOAT)temp_data2 ;
			temp_data = temp_data/4096 ;
		}
		else
		{
			/*postivie*/
			
			temp_data = (FLOAT)P_DATA;
			printf(" temp_data %lf\r\n",P_DATA);
			temp_data = temp_data /4096 ;
		}
		
		*P_RESULT = temp_data;

		/*calcluate temperature data*/
		/*check negative or positive [bit15]*/
		if(T_DATA & 0x8000) 
		{
			/*negative*/
			temp_data2 = ((~T_DATA) & 0xffff);	/* mask 16bit*/
			temp_data2+=1;
			temp_data = -(FLOAT)temp_data2 ;
			temp_data = temp_data /480 + 42.5 ;
		}
		else
		{
			/*postivie*/
			temp_data = (FLOAT)T_DATA;
			temp_data = temp_data /480 + 42.5 ;
		}

		*T_RESULT = temp_data;
		
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*set INTERRUPT PIN status */
CHAR8S LPS331AP_SET_INT_PIN_STATUS(void)
{
	CHAR8U read_data,write_data ;
	CHAR8S status = 0;

		/* set the interrupt pin status */
		status = i2c_write_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_CTRL3,LP331AP_INT_REG3);
		if(status !=1) 
		{
			return -1; /*write fail*/
		}
		else
		{
			return 0; /*write success*/
		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	get INTERRUPT status
	if status = 0x01   -> Interrupt Active.  1: one or more interrupt events have been generated).
	if status = 0x02   ->Differential pressure Low.[compare with REG:0x25~0x26 ]   1: Low differential pressure event has occurred
	if status = 0x04   ->Differential pressure High.[compare with REG:0x25~0x26 ]   1: High differential pressure event has occurred).
*/
CHAR8S LPS331AP_GET_INT_STATUS(CHAR8U *status)
{
	CHAR8S status_function = 0;
	CHAR8U read_data=0;

		/*get the LPS331AP_REG_INT_SOURCE (Interrupt) status */
		status_function = i2c_read_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_INT_SOURCE,&read_data);
		if(status_function !=1) 
		{
			return -1; /*read fail*/
		}
		
		*status = read_data;
		return 0;	/*read success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*set Interrupt pin configuration  [set INT interrput event type]*/
CHAR8S LPS331AP_SET_INT_CFG(void)
{
	CHAR8U read_data,write_data ;
	CHAR8S status = 0;

		/* set the INT interrupt event into LPS331AP_REG_INT_CFG register */
		status = i2c_write_1_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_INT_CFG,LP331AP_INT_CFG);
		if(status !=1) 
		{
			return -1; /*write fail*/
		}
		else
		{
			return 0; /*write success*/
		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*set the interrupt  Threshold pressure value*/
/*The complete threshold value is given by THS_P_H & THS_P_L and is expressed as "unsigned number". */
/*P_ths(mbar)=(THS_P_H & THS_P_L)[dec]/16.*/
CHAR8S LPS331AP_SET_INT_THD_PRESSURE(INT16U THD)
{
	CHAR8S status = 0;
	CHAR8U ths_p[2]={0};
	INT16U temp_data;

		if(THD>10000) 
		{
			return -1 ; /*error  THD*/
		}
		
		/*P_ths(mbar)=(THS_P_H & THS_P_L)[dec]/16.*/
		temp_data = THD<<4;

		/*THS_P_H*/
		ths_p[1] = temp_data & 0xff00;
		ths_p[1] = ths_p[1]>>8;
		
		/*THS_P_L*/
		ths_p[0] = temp_data & 0xff;

		/* write the interrupt threshold value into LPS331AP_REG_THS_P_LOW*/
		status = i2c_write_n_byte_data(LPS331AP_SLAVE_ADDRESS,LPS331AP_REG_THS_P_LOW,2,&ths_p[0]);
		if(status !=1)
		{
			return -1; /* write fail*/
		}
}
/*--------------------------------------------------------------------------------------------------*/
/********************************************** SYSTEM **************************************************/
