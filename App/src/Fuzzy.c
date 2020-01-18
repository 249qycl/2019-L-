#include "common.h"
#include "include.h"
/*************************************************************************
*                       ��������������⡿
*
*                                     ƫ������EC
*                 NB      NM      NS      ZE      PS      PM      PB
*              -----------------------------------------------------
*          NB  .  PB      PB      PM      PM      PS      ZE      ZE
*          NM  .  PB      PB      PM      PS      PS      ZE      NS
*          NS  .  PM      PM      PM      PS      ZE      NS      NS
* ƫ��E    ZE  .  PM      PM      PS      ZO      NS      NM      NM
*          PS  .  PS      PS      ZE      NS      NS      NM      NM
*          PM  .  PS      ZE      NS      NM      NM      NM      NB
*          PB  .  ZE      ZE      NM      NM      NM      NB      NB
*************************************************************************/
const float RuleKp[7][7] = {
    PB, PB, PM, PM, PS, ZE, ZE,
    PB, PB, PM, PS, PS, ZE, NS,
    PM, PM, PM, PS, ZE, NS, NS,
    PM, PM, PS, ZE, NS, NM, NM,
    PS, PS, ZE, NS, NS, NM, NM,
    PS, ZE, NS, NM, NM, NM, NB,
    ZE, ZE, NM, NM, NM, NB, NB};
/*************************************************************************
*                       �����ֲ�������⡿
*
*                                     ƫ������EC
*                 NB      NM      NS      ZE      PS      PM      PB
*              -----------------------------------------------------
*          NB  .  NB      NB      NM      NM      NS      ZE      ZE
*          NM  .  NB      NB      NM      NS      NS      ZE      ZE
*          NS  .  NB      NM      NS      NS      ZE      PS      PS
* ƫ��E    ZE  .  NM      NM      NS      ZE      PS      PM      PM
*          PS  .  NM      NS      ZE      PS      PS      PM      PB
*          PM  .  ZE      ZE      PS      PS      PM      PB      PB
*          PB  .  ZE      ZE      PS      PM      PM      PB      PB
*************************************************************************/
const float RuleKi[7][7] = {
    NB, NB, NM, NM, NS, ZE, ZE,
    NB, NB, NM, NS, NS, ZE, ZE,
    NB, NM, NS, NS, ZE, PS, PS,
    NM, NM, NS, ZE, PS, PM, PM,
    NM, NS, ZE, PS, PS, PM, PB,
    ZE, ZE, PS, PS, PM, PB, PB,
    ZE, ZE, PS, PM, PM, PB, PB};
/*************************************************************************
*                       ��΢�ֲ�������⡿
*
*                                     ƫ������EC
*                 NB      NM      NS      ZE      PS      PM      PB
*              -----------------------------------------------------
*          NB  .  PS      NS      NB      NB      NB      NM      PS
*          NM  .  PS      NS      NB      NM      NM      NS      ZE
*          NS  .  ZE      NS      NM      NM      NS      NS      ZE
* ƫ��E    ZE  .  ZE      NS      NS      NS      NS      NS      ZE
*          PS  .  ZE      ZE      ZE      ZE      ZE      ZE      ZE
*          PM  .  PB      NS      PS      PS      PS      PS      PB
*          PB  .  PB      PM      PM      PM      PS      PS      PB
*************************************************************************/
const float RuleKd[7][7] = {
    PS, NS, NB, NB, NB, NM, PS,
    PS, NS, NB, NM, NM, NS, ZE,
    ZE, NS, NM, NM, NS, NS, ZE,
    ZE, NS, NS, NS, NS, NS, ZE,
    ZE, ZE, ZE, ZE, ZE, ZE, ZE,
    PB, NS, PS, PS, PS, PS, PB,
    PB, PM, PM, PM, PS, PS, PB};
/*************************************************************************
*                       ��ִ�������֪ʶ�⡿(�α��ϵ�,Ӧ��ʱ���ݾ����������)
*
*                                     ƫ������EC
*                 NB      NM      NS      ZE      PS      PM      PB
*              -----------------------------------------------------
*          NB  .  NB      NB      NM      NM      NS      ZE      ZE
*          NM  .  NB      NB      NM      NM      NS      ZE      ZE
*          NS  .  NB      NB      NM      NS      ZE      PM      PM
* ƫ��E    ZE  .  NB      NB      NM      ZE      PM      PB      PB
*          PS  .  NM      NM      ZE      PS      PM      PB      PB
*          PM  .  ZE      ZE      PS      PM      PM      PB      PB
*          PB  .  ZE      ZE      PS      PM      PM      PB      PB
*************************************************************************/
float Servo_duty[7] = {-100, -67, -34, 0, 34, 67, 100}; //�������
const float RuleOut[7][7] = {
    NB, NB, NM, NM, NS, ZE, ZE,
    NB, NB, NM, NM, NS, ZE, ZE,
    NB, NB, NM, NS, ZE, PM, PM,
    NB, NB, NM, ZE, PM, PB, PB,
    NM, NM, ZE, PS, PM, PB, PB,
    ZE, ZE, PS, PM, PM, PB, PB,
    ZE, ZE, PS, PM, PM, PB, PB};
/*************************************************************************
*                       �����ռ�ձ����֪ʶ�⡿
*
*                                     ƫ������EC
*                 NB      NM      NS      ZE      PS      PM      PB
*              -----------------------------------------------------
*          NB  .  NB      NB      NM      NM      NS      ZE      ZE
*          NM  .  NB      NB      NM      NM      NS      ZE      ZE
*          NS  .  NB      NB      NM      NS      ZE      PM      PM
* ƫ��E    ZE  .  NB      NB      NM      ZE      PM      PB      PB
*          PS  .  NM      NM      ZE      PS      PM      PB      PB
*          PM  .  ZE      ZE      PS      PM      PM      PB      PB
*          PB  .  ZE      ZE      PS      PM      PM      PB      PB
*************************************************************************/
float Motor_duty[7] = {0, 150, 250, 350, 450, 550, 650}; //���ռ�ձ��������
const float RuleDuty[7][7] = {
    NB, NB, NM, NB, NB, NB, NB,
    NB, NB, NB, NB, NB, NB, NB,
    NS, NS, NS, NM, NS, NS, NS,
    NS, NS, ZE, PS, ZE, NS, NS, //ze
    NS, NS, NS, NM, NS, NS, NS,
    NB, NB, NB, NB, NB, NB, NB,
    NB, NB, NB, NB, NB, NB, NB};
/*********************************************************
*
*       1��������������Ⱥ�������ֵ
*       2������������ֵ���ó���Ӧ�ļ������������ֵ
*       3���Եõ���������ֵ�����Ӧ�������Ƚ������ķ�����
*
*********************************************************/

float S_Kd[7] = {0};
float S_Kp[7] = {0};
/*********************************************
*
*       ģ��������
*       Rule:�����
*       range=maxmum-minmum
*       e :ƫ��
*       ec:ƫ������
*       object:�������
*********************************************/
float fuzzy(float range, float e, float ec, const float (*Rule)[7], float *object) /*����ǰ�ȶ�ƫ�ƫ����������*/
{

        float e_temp, ec_temp;     //����������ֵ����Ԥ�����м����
        float e_L_temp, ec_L_temp; //������
        float e_R_temp, ec_R_temp;

        int e_L_Index, ec_L_Index; //����������
        int e_R_Index, ec_R_Index;
        float out; //��ģ��������ֵ
        /*������������*/
        e = 6.0 * e / range;   //ƫ��ͳһ��-6~6֮�����
        ec = 3.0 * ec / range; //ƫ������ͳһ��-6~6֮�����
        /*ƫ������������ֵ����Ԥ����*/
        e_temp = e > 6.0 ? 0.0 : (e < -6.0 ? 0.0 : (e >= 0.0 ? (e >= 4.0 ? 5.0 : (e >= 2.0 ? 3.0 : 1.0)) : (e >= -2.0 ? -1.0 : (e >= -4.0 ? -3.0 : (e >= -6.0 ? -5.0 : 0.0)))));
        /*ƫ������������ֵ����ӳ��*/
        e_L_Index = (int)((e_temp - 1) / 2 + 3); //[-6,6] ---ӳ��---> [0,6]
        e_R_Index = (int)((e_temp + 1) / 2 + 3);
        /*��ƫ�������ֵ��������*/
        e_L_temp = e_temp == 0.0 ? 0.0 : (((e_temp + 1) - e) / 2);
        e_R_temp = e_temp == 0.0 ? 0.0 : ((e - (e_temp - 1)) / 2);
        /*ƫ����������������ֵ����Ԥ����*/
        ec_temp = ec > 6.0 ? 0.0 : (ec < -6.0 ? 0.0 : (ec >= 0.0 ? (ec >= 4.0 ? 5.0 : (ec >= 2.0 ? 3.0 : 1.0)) : (ec >= -2.0 ? -1.0 : (ec >= -4.0 ? -3.0 : (ec >= -6.0 ? -5.0 : 0.0)))));
        /*ƫ������������ֵ����ӳ��*/
        ec_L_Index = (int)((ec_temp - 1) / 2 + 3); //[-6,6] ---ӳ��---> [0,6]
        ec_R_Index = (int)((ec_temp + 1) / 2 + 3);
        /*��ƫ������������ֵ��������*/
        ec_L_temp = ec_temp == 0.0 ? 0.0 : (((ec_temp + 1) - ec) / 2);
        ec_R_temp = ec_temp == 0.0 ? 0.0 : ((ec - (ec_temp - 1)) / 2);
        /************************************���ķ���ģ��*******************************************/

#if 1 //����±�
        out = e_L_temp * (ec_L_temp * object[(uint8)Rule[e_L_Index][ec_L_Index]] + ec_R_temp * object[(uint8)Rule[e_L_Index][ec_R_Index]]) 
            + e_R_temp * (ec_L_temp * object[(uint8)Rule[e_R_Index][ec_L_Index]] + ec_R_temp * object[(uint8)Rule[e_R_Index][ec_R_Index]]);
#else //���ֵ
        out = e_L_temp * (ec_L_temp * Rule[e_L_Index][ec_L_Index] + ec_R_temp * Rule[e_L_Index][ec_R_Index]) + e_R_temp * (ec_L_temp * Rule[e_R_Index][ec_L_Index] + ec_R_temp * Rule[e_R_Index][ec_R_Index]);
#endif
        return out;
}