/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: pressure & temperature sensor
     File Name		: SENSOR_LPS331AP.h
     Function		: SENSOR_LPS331AP
     Create Date	: 2017/10/31
---------------------------------------------------------------------- */
#ifndef __LPS331AP_HEADER__
#define __LPS331AP_HEADER__


/*Note : CS pin must connect to high , enable  I2C .*/
/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/
/* LPS331AP SLAVE ADDRESS*/
	#define LPS331AP_ADDR_PIN 	 				(0)		/*PIN7(ADDR) connect  "0" GND or "1" Vcc*/
#if (LPS331AP_ADDR_PIN == 1)
	#define LPS331AP_SLAVE_ADDRESS 			(0xBA)
#else if (LPS331AP_ADDR_PIN == 0)
	#define LPS331AP_SLAVE_ADDRESS 			(0xB8)
#endif
/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/



/* ------------- DEFINE LPS331AP REGISTER  ------------*/
/* REG 0x11 ~ 0x1F , 0x2D ~ 0x2F  		Reserved (Do not modify)*/
/* REG 0x30[REG_AMP_CTRL] 				Partially Reserved*/
/*------------------------------------------------*/
#define LPS331AP_REG_REF_P_XL					(0x08)		/*Read/Write*/
/* The lower part of the reference pressure , def:0x0 */
/* The full value is   REF_P_XL   +   REF_P_H   +   REF_P_L   and is represented as 2・s complement.*/
/*------------------------------------------------*/
#define LPS331AP_REG_REF_P_L					(0x09)		/*Read/Write*/
/* This register contains the middle part of the reference pressure , def:0x0 */
/* The full value is   REF_P_XL   +   REF_P_H   +   REF_P_L   and is represented as 2・s complement.*/
/*------------------------------------------------*/
#define LPS331AP_REG_REF_P_H					(0x0A)		/*Read/Write*/
/* This register contains the higher part of the reference pressure , def:0x0 */
/* The full value is   REF_P_XL   +   REF_P_H   +   REF_P_L   and is represented as 2・s complement.*/
/*------------------------------------------------*/
#define LPS331AP_REG_WHO_AM_I				(0x0F)		/*Read only*/	/*value must be : 0xBB*/
/* for LPS331AP ID, 0xBB.*/
/*------------------------------------------------*/
#define LPS331AP_REG_RES_CONF					(0x10)		/*Read/Write*/
/*
	Pressure resolution mode , *more detail see sepc.  , def:0x7A 
	bit 3 ~ 0 set Pressure resolution configuration  , internal average .
	bit 4 ~ 6 set Temperature resolution configuration , internal average .
	-> usually set 0x7A to this REG.
*/
/*------------------------------------------------*/
#define LPS331AP_REG_CTRL1						(0x20)		/*Read/Write*/
/*
	bit 7 [PD]  def:0  power down control. "0"-> power-down mode  "1"-> active mode  
	bit 6~4 [ODR2, ODR1, ODR0]  def:0  , output data rate selection.
		ODR2  ODR1	 ODR0 		Pressure output data rate		Temperature output data rate
		0 		0 		0 			One shot 						One shot
		0		0 		1 			 1 Hz 							1 Hz
		0 		1 		0 			 7 Hz 							1 Hz
		0 		1 		1 			12.5 Hz 							1 Hz 
		1		0 		0 			25 Hz 							1 Hz
		1 		0 		1 			7 Hz 							7 Hz
		1 		1 		0 			12.5 Hz 							12.5 Hz
		1 		1 		1 			25 Hz 							25 Hz
*/
/*--------------------------------------------------------------*/
/*
	bit 3 [DIFF_EN]  def:0, Interrupt circuit enable. "0" -> interrupt generation disabled ; "1" -> interrupt circuit enabled.
	bit 2 [BDU]  def:0, block data update. "0" ->continuous update; "1" -> output registers not updated until MSB and LSB reading
	bit 1 [DELTA_EN]  def:0 , delta pressure enable. "0" -> disable  ;  "1" -> delta pressure registers enabled.
	bit 0 [SIM] def:0 ,   "0" ->4-wire interface;  "1" -> 3-wire interface.
	 ** ODR2- ODR1 - ODR0 bits allow to change the output data rates of pressure and temperature samples.
*/
/*------------------------------------------------*/
#define LPS331AP_REG_CTRL2						(0x21)		/*Read/Write*/
/*
	*more detail see sepc. 
	bit 7 [BOOT] def:0   Reboot memory content.   "0" -> normal mode ;  "1" -> reboot memory content.
	bit 6~3 RESERVED
	bit 2 [SWRESET] def: 0  , Software reset.   "0" -> normal mode ;   "1" -> software reset
	The device is reset to the power on configuration if the SWRESET bit is set to ．1・ and BOOT is set to ．1・.
	bit 1 [AUTO_ZERO] def: 0  , Autozero enable.   "0" -> normal mode;   "1" -> autozero enable
	bit 0 [ONE_SHOT] def: 0  , One shot enable.   "0" ->waiting for start of conversion ;   "1" ->start for a new dataset 
	start a new conversion when ODR1-ODR0 bits in CTRL_REG1 are set to "000"
	when ONE_SHOT bit is set to ．1・. At the end of conversion the new data are available in the output registers, 
	the REG_STAUS[0] and REG_STAUS[1] bits are set to ．1・ and the ONE_SHOT bit comes back to ．0・ by hardware.
*/
/*------------------------------------------------*/
#define LPS331AP_REG_CTRL3						(0x22)		/*Read/Write*/
/*
	bit 7 [INT_H_L] def:0 Interrupt active high, low.    "0" ->  active high ;   "1" -> active low 
	bit 6 [PP_OD] def:0  Push-pull/open drain selection on interrupt pads.  "0" -> push-pull ;   "1" -> open drain 
	bit 5~3 [INT2_S3, INT2_S2, INT2_S1] def:0   data signal on INT2 pad control bits. see below table!
	bit 2~0 [INT1_S3, INT1_S2, INT1_S1] def:0   data signal on INT1 pad control bits. see below table!
	
		INT1(2)_S3		INT1(2)_S2 		INT1(2)S1 		INT1(2) pin
			0 				0 				0 			GND
			0 				0 				1 			Pressure high (P_high)
			0 				1	 			0 			Pressure low (P_low)
			0 				1 				1 			P_low OR P_high
			1				0 				0 			Data ready
			1 				0 				1 			Reserved
			1 				1 				0 			Reserved
	 		1 				1 				1 			Tri-state
*/
/*------------------------------------------------*/
#define LPS331AP_REG_INT_CFG					(0x23)		/*Read/Write*/
/*
	bit 7~3 RESERVED
	bit 2 [LIR] def:0   Latch Interrupt request into INT_SOURCE register    
					"0" ->interrupt request not latched  ;   "1" -> interrupt request latched 
	bit 1 [PL_E] def:0   Enable interrupt generation on differential pressure low event.  "0" -> disable interrupt request ; 
					"1" -> enable interrupt request on measured differential pressure value lower than preset threshold
	bit 0 [PH_E] def:0  Enable interrupt generation on differential pressure high event.  "0" -> disable interrupt request;  
					"1" -> enable interrupt request on measured differential pressure value higher than preset threshold
*/
/*------------------------------------------------*/
#define LPS331AP_REG_INT_SOURCE				(0x24)		/*Read only*/		 
/*
	bit 7~3 RESERVED
	bit 2 [IA] def:0   Interrupt Active. 			"0" -> no interrupt has been generated  ;	"1" -> one or more interrupt events have been generated
	bit 1 [PL] def:0   Differential pressure Low.		"0" ->  no interrupt has been generated ;	"1" -> Low differential pressure event has occurred
	bit 0 [PH] def:0	Differential pressure High.	"0" ->  no interrupt has been generated;	"1" -> High differential pressure event has occurred
*/
/*------------------------------------------------*/
#define LPS331AP_REG_THS_P_LOW				(0x25)		/*Read/Write*/
/*
	This register contains the low part of threshold value for pressure interrupt generation.
	The complete threshold value is given by THS_P_H & THS_P_L and is expressed as unsigned number.
*/
/*------------------------------------------------*/
#define LPS331AP_REG_THS_P_HIGH				(0x26)		/*Read/Write*/
/*
	This register contains the high part of the threshold value for pressure interrupt generation.
	The complete threshold value is given by THS_P_H & THS_P_L and is expressed as unsigned number. P_ths(mbar)=(THS_P_H & THS_P_L)[dec]/16.
*/
/*------------------------------------------------*/
#define LPS331AP_REG_STATUS						(0x27)		/*Read only*/
/*
	The content of this register is updated every ODR cycle, regardless of BDU value in CTRL_REG1.
	bit 5 [P_OR] def:0  Pressure data overrun		"0" -> no overrun has occurred ;		"1" ->A new data for pressure has overwritten the previous one
	bit 4 [T_OR] def:0  Temperature data overrun	"0" -> no overrun has occurred ;		"1" -> A new data for temperature has overwritten the previous one
	bit 1 [P_DA] def:0  Pressure data available.		"0" -> Pressure data not ready ;		"1" -> new data for pressure is available
	bit 0 [T_DA] def:0  Temperature data available.	"0" -> Temperature data not ready;	"1" -> new data for temperature is available
*/
/*------------------------------------------------*/
#define LPS331AP_REG_PRESS_POUT_XL_REH	(0x28)		/*Read only*/
/*Pressure data (LSB)*/
/*------------------------------------------------*/
#define LPS331AP_REG_PRESS_OUT_L				(0x29)		/*Read only*/
/*Pressure data*/
/*------------------------------------------------*/
#define LPS331AP_REG_PRESS_OUT_H				(0x2A)		/*Read only*/
/*Pressure data (MSB)*/
/*------------------------------------------------*/
#define LPS331AP_REG_TEMP_OUT_L				(0x2B)		/*Read only*/
/*Temperature data (LSB)*/
/*------------------------------------------------*/
#define LPS331AP_REG_TEMP_OUT_H				(0x2C)		/*Read only*/
/*Temperature data (MSB)*/
/*******************************************************************************/
/******* Temperature output data: T(degC) = 42.5 + (Temp_OUTH & TEMP_OUT_L)[dec]/480*/
/*******************************************************************************/
/*------------------------------------------------*/
//#define LPS331AP_REG_AMP_CTRL					0x2D		/*Read/Write*/
/*don't use */
/*------------------------------------------------*/
#define LPS331AP_REG_DELTA_PRESS_XL			(0x3C)
#define LPS331AP_REG_DELTA_PRESS_L			(0x3D)
#define LPS331AP_REG_DELTA_PRESS_H			(0x3E)
/* Delta pressure register for One Point calibration*/
/* ------------- DEFINE LPS331AP REGISTER   ------------*/


/* ------------- DEFINE LPS331AP Parameter -------------*/
/*------------------------------------------*/
/*
	or set output rate   ,  ** default : 0x00
	** function parameter : LPS331AP_SET_REG_CTRL1 use.
						LPS331AP_GET_DATA use.
*/
#define LPS331AP_OUTPUT_RATE_ONE_SHOT	 		(0x00)	
#define LPS331AP_OUTPUT_RATE_1HZ_1HZ			(0x10)	/*pressure 1HZ,  temperature1HZ  */
#define LPS331AP_OUTPUT_RATE_7HZ_1HZ			(0x20)	/*pressure 7HZ,  temperature1HZ  */
#define LPS331AP_OUTPUT_RATE_12_5HZ_1HZ		(0x30)	/*pressure 12.5HZ,  temperature1HZ  */
#define LPS331AP_OUTPUT_RATE_25HZ_1HZ			(0x40)	/*pressure 25HZ,  temperature1HZ  */
#define LPS331AP_OUTPUT_RATE_7HZ_7HZ			(0x50)	/*pressure 7HZ,  temperature7HZ  */
#define LPS331AP_OUTPUT_RATE_12_5HZ_12_5HZ	(0x60)	/*pressure 12.5HZ,  temperature12.5HZ  */
#define LPS331AP_OUTPUT_RATE_25HZ_25HZ		(0x70)	/*pressure 25HZ,  temperature25HZ  */

#define LPS331AP_POWER_DOWN						(0x00)	/*bit7*/
#define LPS331AP_ACTIVE								(0x80)	/*bit7*/

/*This feature avoids reading LSB and MSB related to different samples.*/
#define LPS331AP_DBU									(0x04)	/* always  open */

/*------------------------------------------*/
/* for set the resolution [internal average]*/
/** function parameter : LPS331AP_SET_NOISE_ADJ use.*/
#define LPS331AP_PRESSURE_NOISE    					(0x0A<<0)		/*default 0x0A (max 0x0f)*/
#define LPS331AP_TEMPERATURE_NOISE 				(0x07<<4)		/*default 0x07 (max 0x07)*/
/*------------------------------------------*/
/** function parameter : LPS331AP_RESET use.*/
#define LPS331AP_SW_RESET 							(0x84)			/*set bit7 & 2*/
/*------------------------------------------*/
/* function parameter : */
#define LPS331AP_AUTO_ZERO  						(0x02)		/*enable auto zero*/
/*------------------------------------------*/
/** function parameter : LPS331AP_GET_DATA use.*/
#define LP331AP_ONE_SHOT_MODE						(0x01)
/*------------------------------------------*/

/*------------------------------------------*/
/*INETERRUPT PIN STATUS*/
#define LP331AP_INT_H_L								(0x00)		/*BIT7 (0: active high; 1: active low)*/
#define LP331AP_INT_OD								(0x40)		/*BIT6 (0: push-pull; 1: open drain)*/

#define LP331AP_INT_PIN_STATUS_GND 				(0x00)  
#define LP331AP_INT_PIN_STATUS_P_HIGH			(0x01)		/*pressure high*/
#define LP331AP_INT_PIN_STATUS_P_LOW			(0x02)		/*pressure low*/
#define LP331AP_INT_PIN_STATUS_P_LOW_HIGH	(0x03)		/*pressure low or high*/	
#define LP331AP_INT_PIN_STATUS_DATA_READY	(0x04)		/*data ready*/
#define LP331AP_INT_PIN_STATUS_TRI_STATE		(0x07)		

/*PIN INT 1*/
#define LP331AP_INT_PIN1 		(LP331AP_INT_PIN_STATUS_GND)
/*PIN INT 2*/
#define LP331AP_INT_PIN2 		(LP331AP_INT_PIN_STATUS_GND)
/*------------------------------------------*/
//INETERRUPT configuration
// ** set only mode
#define LP331AP_INT_CFG_LIR							(0x01)		/*Latch Interrupt request into INT_SOURCE register.*/
#define LP331AP_INT_CFG_PL_E						(0x02)		/*Enable interrupt generation on differential pressure low event.*/
#define LP331AP_INT_CFG_PH_E						(0x04)		/*Enable interrupt generation on differential pressure high event.*/
#define LP331AP_INT_CFG_NONE						(0x00)		/*NO any EVENT!*/
/* ------------- DEFINE LPS331AP Parameter -------------*/



/*************************************************************************************************************************************/
/*************************************************************************************************************************************/
/*SET CONFIG Parameter*/
/*
	set  REG_CTRL1 
*/
#define LPS331AP_SET_REGCTRL1		(LPS331AP_ACTIVE  | LPS331AP_OUTPUT_RATE_ONE_SHOT)
//#define LPS331AP_SET_REGCTRL1		(LPS331AP_ACTIVE | LPS331AP_OUTPUT_RATE_12_5HZ_12_5HZ)

/*
	set  LPS331AP_REG_RES_CONF
*/
#define LPS331AP_RESOLUTION 		(LPS331AP_PRESSURE_NOISE | LPS331AP_TEMPERATURE_NOISE)


/*
	set  LPS331AP_REG_CTRL3
*/
#define LP331AP_INT_REG3				(LP331AP_INT_PIN1 |(LP331AP_INT_PIN2<<3) |LP331AP_INT_OD |LP331AP_INT_H_L)


/*
	set LPS331AP_REG_INT_CFG
*/
#define LP331AP_INT_CFG				(LP331AP_INT_CFG_NONE)
/*************************************************************************************************************************************/
/*************************************************************************************************************************************/


/********************************************** SYSTEM **************************************************/
/*--------------------------------------------------------------------------------------------------*/
/* initial LPS331AP */
CHAR8S LPS331AP_SET_INIT(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* read LPS3311AP id  ; id = 0xBB */
CHAR8S LPS331AP_GET_ID(CHAR8U *id);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	check LPS331AP Pressure data & Temperature data already?
	if return -1 -> read the register fail [I2S fail]
	if status return 0x01 -> Temperature data ready
	if status return 0x02 -> Pressure data ready

	The content of this register is updated every ODR cycle, regardless of BDU value in CTRL_REG1.
	bit 5 [P_OR] def:0  Pressure data overrun  		"0" 	-> no overrun has occurred ;	"1" -> A new data for pressure has overwritten the previous one
	bit 4 [T_OR] def:0  Temperature data overrun	"0" 	-> no overrun has occurred ;	"1" -> A new data for temperature has overwritten the previous one
	bit 1 [P_DA] def:0  Pressure data available.		"0" 	-> Pressure data not ready ;		"1" -> new data for pressure is available
	bit 0 [T_DA] def:0  Temperature data available.	"0" 	-> Temperature data not ready; 	"1" -> new data for temperature is available

*/
CHAR8S LPS331AP_GET_DATA_READY(CHAR8U *status);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	 set [PD],[ODR2, ODR1, ODR0],[BDU]
*/
CHAR8S LPS331AP_SET_REG_CTRL1(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* set the pressure internal average & temperature internal average*/
/* LPS331AP_REG_RES_CONF : default 0x7A*/
CHAR8S LPS331AP_SET_NOISE_ADJ(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* Software Reset the LPS331AP */
CHAR8S LPS331AP_SET_RESET(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* Get Pressure & Temperature data */
CHAR8S LPS331AP_GET_DATA(FLOAT *pressure,FLOAT *temperature);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* to calculate the Pressure data & Temperature data*/
CHAR8S LPS331AP_GET_CALCULATE(INT32U P_DATA,INT16U T_DATA,FLOAT *P_RESULT , FLOAT *T_RESULT);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*set INTERRUPT PIN status*/
CHAR8S LPS331AP_SET_INT_PIN_STATUS(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	get INTERRUPT status
	if status = 0x01   -> Interrupt Active.  1: one or more interrupt events have been generated).
	if status = 0x02   ->Differential pressure Low.[compare with REG:0x25~0x26 ]   1: Low differential pressure event has occurred
	if status = 0x04   ->Differential pressure High.[compare with REG:0x25~0x26 ]   1: High differential pressure event has occurred).
*/
CHAR8S LPS331AP_GET_INT_STATUS(CHAR8U *status);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* set Interrupt pin configuration  [set INT interrput event type]*/
CHAR8S LPS331AP_SET_INT_CFG(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	set the interrupt  Threshold pressure value
	The complete threshold value is given by THS_P_H & THS_P_L and is expressed as "unsigned number". 
	P_ths(mbar)=(THS_P_H & THS_P_L)[dec]/16.
*/
CHAR8S LPS331AP_SET_INT_THD_PRESSURE(INT16U THD);
/*--------------------------------------------------------------------------------------------------*/
/********************************************** SYSTEM **************************************************/
#endif		/*#ifndef __LPS331AP_HEADER__*/