#ifndef __FUZZY_H__
#define __FUZZY_H__

#define DIR_RANGE       200     //����ƫ��ķ�Χ
#define SERVO_RANGE     200     //�������ķ�Χ
extern float Motor_duty[7];
extern const float RuleDuty[7][7];

#define NB      0
#define NM      1
#define NS      2
#define ZE      3
#define PS      4
#define PM      5
#define PB      6

float fuzzy(float range,float e,float ec,const float (*Rule)[7],float *object);

#endif