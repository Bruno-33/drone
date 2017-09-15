/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Motor.cpp
 * 描述    ：电机控制相关函数
**********************************************************************************/
#include "FTC_Motor.h"

FTC_Motor motor;
	static int mode = 4;//0 遥控    1一键起飞    2 一键翻滚     3抛飞     4初始   5中间状态
	static uint16_t tmp_throttle = 0;
	static uint16_t clock_100ms = 50;
	static uint16_t clock_1000ms = 400;
	static uint16_t turn=0;
	static uint16_t flag=0;
	static uint16_t degree=0;
	uint16_t last=0;
void FTC_Motor::writeMotor(uint16_t throttle, int32_t pidTermRoll, int32_t pidTermPitch, int32_t pidTermYaw){
	if(rc.mymode==Normal){
			if(imu.Acc.z > 2*ACC_1G && ftc.f.ARMED  && mode==4){
				  mode=3;
				  tmp_throttle = 1500; 
				  clock_100ms=200;
			}
			else if(rc.rawData[THROTTLE] > RC_MINCHECK ){
					mode=0;
			}
	}
  else if(rc.mymode==Button_Fly){
			if(mode==4){
				  mode=1;
				  tmp_throttle = 1600;
				  clock_100ms=70;
			}
			else if(rc.rawData[THROTTLE] > RC_MINCHECK ){
					mode=0;
			}
	}
	else if(rc.mymode==Button_turn && mode == 0&&flag==0){
//		  flag=1;
//			mode=2;
//		  turn=20;
//			if(mode==4){
//				  mode=1;
//				  tmp_throttle = 1500; 
//				  clock_100ms=200;
//			}
//			else if(rc.rawData[THROTTLE] > RC_MINCHECK ){
//					mode=0;
//			}
	}
	else if(rc.mymode == Button_Reset){
			mode=4;
	}
	//每过100ms，clock_100ms就会等于50
	if(!(--clock_100ms)){
		clock_100ms = 50;
	}
	//
	if(mode == 5){
			clock_1000ms--;
			if(clock_1000ms == 0){
					flag=1;
				  mode=2;
					turn=20;
				  degree=0;
			}
	}
	switch(mode){
		case 0 : 
				break;
		case 1:			
			//care
			   //	tmp_throttle = 1000;
			
		if(clock_100ms == 50 && tmp_throttle >= 1100){
						tmp_throttle = tmp_throttle - 20;
				}
				if(tmp_throttle <= 1300){
						tmp_throttle = 1000;
				}
				throttle = tmp_throttle;
				break;
		case 2: throttle=2000;break;
//			//每过100ms，throttle减10
//			if(clock_100ms == 50 && tmp_throttle >= 1100){
//					tmp_throttle = tmp_throttle - 10;
//			}
//			throttle = tmp_throttle;
//	  break;
		case 3: 
				if(clock_100ms == 50 && tmp_throttle >= 1100){
						tmp_throttle = tmp_throttle - 13;
				}
				if(tmp_throttle <= 1300){
						tmp_throttle = 1000;
				}
				throttle = tmp_throttle;
		break;
		case 5:
				throttle = 1500;
		break;
		default: break;
	}
	motorPWM[2] = throttle - 0.5 * pidTermRoll + 0.866 *  pidTermPitch + pidTermYaw; 
	motorPWM[1] = throttle - 0.5 * pidTermRoll - 0.866 *  pidTermPitch + pidTermYaw; 
	motorPWM[0] = throttle + 0.5 * pidTermRoll + 0.866 *  pidTermPitch - pidTermYaw; 
	motorPWM[3] = throttle + 0.5 * pidTermRoll - 0.866 *  pidTermPitch - pidTermYaw; 
	motorPWM[5] = throttle - pidTermRoll - pidTermYaw;	
	motorPWM[4] = throttle + pidTermRoll + pidTermYaw;	
	if(mode==2){
			motorPWM[2] = throttle;
	    motorPWM[1] = throttle;
		  motorPWM[0] = throttle;
		  motorPWM[3] = throttle;
		  motorPWM[5] = throttle;
		  motorPWM[4] = throttle;
	}
	int16_t maxMotor = motorPWM[0];
	for (u8 i = 1; i < MAXMOTORS; i++){
		if (motorPWM[i] > maxMotor)
					maxMotor = motorPWM[i];				
	}
	
	for (u8 i = 0; i < MAXMOTORS; i++) {
		if (maxMotor > MAXTHROTTLE)    
			motorPWM[i] -= maxMotor - MAXTHROTTLE;	
		//限制电机PWM的最小和最大值
		motorPWM[i] = constrain_uint16(motorPWM[i], MINTHROTTLE, MAXTHROTTLE);
	}
	 last=imu.Gyro.y;
   if(mode==2){
		  degree+=(imu.Gyro.y+last)*0.002; 
	    motorPWM[1]=0;
		  motorPWM[3]=0;
		  //if(!turn) throttle=1000,mode=1;
		  if(degree>=145) mode=1;
		  // turn--;
	 }
	//如果未解锁，则将电机输出设置为最低rc.rawData[THROTTLE] < RC_MINCHECK
	if(!ftc.f.ARMED||throttle==1000)	
		ResetPWM();
	 if(ftc.f.ARMED&&(mode==4||(mode==0&&rc.rawData[THROTTLE] < RC_MINCHECK)))
		   ResetPWM();
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
