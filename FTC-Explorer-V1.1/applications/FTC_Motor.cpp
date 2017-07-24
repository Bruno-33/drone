/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Motor.cpp
 * ����    �����������غ���
**********************************************************************************/
#include "FTC_Motor.h"

FTC_Motor motor;

void FTC_Motor::writeMotor(uint16_t throttle, int32_t pidTermRoll, int32_t pidTermPitch, int32_t pidTermYaw){
	static int mode = 0;//0 ң��    1һ�����    2 һ������     3�׷�     4��ʼ
	uint16_t tmp_throttle;
	if(rc.mymode==Normal){
			if(imu.Acc.z > 2*ACC_1G && ftc.f.ARMED  && mode==4){
				  mode=3;
			}
			else if(rc.rawData[THROTTLE] > RC_MINCHECK ){
					mode=0;
			}
	}
  else if(rc.mymode==Button_Fly){
			if(rc.rawData[THROTTLE] < RC_MINCHECK && mode==4){
				  mode=1;
			}
			else if(rc.rawData[THROTTLE] > RC_MINCHECK){
					mode=0; 
			}
	}
	else if(rc.mymode==Button_Land && mode == 4){
			mode=2;
		  tmp_throttle = throttle;
	}
	else if(rc.mymode == Button_Reset){
			mode=4;
	}
	
	
	//ÿ��100ms��clock_100ms�ͻ����50
	static uint16_t clock_100ms = 50;
	
	if(!(--clock_100ms)){
		clock_100ms = 50;
	}
	//
	switch(mode){
		case 1: throttle=1300;
		break;
		case 2: 
			//ÿ��100ms��throttle��10
			if(clock_100ms == 50 && tmp_throttle >= 1100){
					tmp_throttle = tmp_throttle - 10;
			}
			throttle = tmp_throttle;
	  break;
		case 3: throttle=1300;
		break;
		default: ;
	}
	motorPWM[2] = throttle - 0.5 * pidTermRoll + 0.866 *  pidTermPitch + pidTermYaw; 
	motorPWM[1] = throttle - 0.5 * pidTermRoll - 0.866 *  pidTermPitch + pidTermYaw; 
	motorPWM[0] = throttle + 0.5 * pidTermRoll + 0.866 *  pidTermPitch - pidTermYaw; 
	motorPWM[3] = throttle + 0.5 * pidTermRoll - 0.866 *  pidTermPitch - pidTermYaw; 
	motorPWM[5] = throttle - pidTermRoll - pidTermYaw;	
	motorPWM[4] = throttle + pidTermRoll + pidTermYaw;	
	
	int16_t maxMotor = motorPWM[0];
	for (u8 i = 1; i < MAXMOTORS; i++){
		if (motorPWM[i] > maxMotor)
					maxMotor = motorPWM[i];				
	}
	
	for (u8 i = 0; i < MAXMOTORS; i++) {
		if (maxMotor > MAXTHROTTLE)    
			motorPWM[i] -= maxMotor - MAXTHROTTLE;	
		//���Ƶ��PWM����С�����ֵ
		motorPWM[i] = constrain_uint16(motorPWM[i], MINTHROTTLE, MAXTHROTTLE);
	}

	//���δ�������򽫵���������Ϊ���rc.rawData[THROTTLE] < RC_MINCHECK
	if(!ftc.f.ARMED)	
		ResetPWM();
	 if(ftc.f.ARMED&&(mode==4||(mode==0&&rc.rawData[THROTTLE] < RC_MINCHECK)))
		   ResetPWM();
	static int16_t throw_throttle;
	pwm.SetPwm(motorPWM);
}

void FTC_Motor::getPWM(int16_t* pwm)
{
	*(pwm) = motorPWM[0];
	*(pwm+1) = motorPWM[1];
	*(pwm+2) = motorPWM[2];
	*(pwm+3) = motorPWM[3];
	*(pwm+4) = motorPWM[4];
	*(pwm+5) = motorPWM[5];	
}

void FTC_Motor::ResetPWM(void)
{
	for(u8 i=0; i< MAXMOTORS ; i++)
		motorPWM[i] = 1000;
}

/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
