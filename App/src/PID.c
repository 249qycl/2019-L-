#include "common.h"
#include "include.h"

pPID Servo_PID; //�������PID������
pPID Motor_PID; //�������PID������

/************************************************************
*	���ã�PID���������ʼ��
*	limit�����ַ����ƫ�����
*	saturation�����ֱ���ֵ
*************************************************************/
pPID PID_create(int16 p, int16 i, int16 d)
{
    pPID P = (pPID)malloc(sizeof(PID));
    P->set_value = 0;
    P->prev_error = 0;
    P->last_error = 0;
    P->sum_error = 0;
    P->P = p;
    P->I = i;
    P->D = d;
    return P;
}

/***********************************************************
* 
*       PID������ʼ��      
*
************************************************************/
void All_PID_init()
{
    Servo_PID = PID_create(S_P, S_I, S_D);
    Motor_PID = PID_create(M_P, M_I, M_D);
}
/************************************************************
*	����:		λ��ʽPD�����������ַ����͡�
*	Q:			׼������Ķ���
*	input��		����������ֵ����������
*	out:		���ֵ
*************************************************************/
int PID_Position(pPID Q, int input)
{
    int iError;
    int output;

    iError = 0 - input;
    output = Q->P * iError + Q->D * (iError - Q->last_error);
    Q->last_error = iError;
    return output; //�������,��׼����������PWMռ�ձ�
}
/***************************************************************
*	����:		����ʽPI�����������ַ����͡�
*	Q:		׼������Ķ���
*	input��		����������ֵ����������
*	Increase_out: ���������
****************************************************************/
int PID_Increase(pPID Car, int input) //����ʽPID
{
    int32 iError;
    int32 increase_out;

    iError = Car->set_value - input;

    increase_out = Car->P * (iError - Car->last_error) + Car->I * iError;

    Car->last_error = iError;
    return increase_out;
}

/*************************************************************
*
*				�Բ�������PID���ڡ�����ӦPID��
*	�ٶ�	Kp_P	Kp_I	Kp_D	Kd_P	Kd_I	Kd_D
*	360		5		18		0		2		10		0
*	400		7		11		0		3		9		0
*************************************************************/
PID Adapt_S_Kp = {.P = 7,
                  .I = 11,
                  .D = 0,
                  .last_error = 0}; //10000==1

PID Adapt_S_Kd = {.P = 3,
                  .I = 9,
                  .D = 0,
                  .last_error = 0}; //10000==1

PID Adapt_M_Kp = {.P = 3,
                  .I = 50,
                  .D = 0,
                  .last_error = 0}; //10000==1

int32 PID_Adapt(PID *sprt, int32 NowPiont)
{
    int32 iError; //��ǰ���
    int32 Actual; //���ó���ʵ�����ֵ

    iError = NowPiont; // ���㵱ǰ���

    Actual = sprt->P * (iError * iError) // ��̬����������ƽ����ϵ
             + sprt->I * 10000           // �˴����ǡ����֡������Ƕ�̬������һ����Сֵ
             + sprt->D * (iError - sprt->last_error);

    sprt->last_error = iError; //�����ϴ����

    return Actual / 10000;
}

/**************************************************************
*
*               PID������
*
**************************************************************/

void PID_Handler(uint8 Servo_T, uint8 Motor_T)
{
    static uint8 Servo_time = 0, Motor_time = 0;
    if (Servo_time > Servo_T)
    {
        Servo_PID->P = PID_Adapt(&Adapt_S_Kp, Ecc_Car->Error.now);
        Servo_PID->D = PID_Adapt(&Adapt_S_Kd, Ecc_Car->Error.delta);
        Ecc_Car->Duty.servo = -Ecc_Car->Error.now * Servo_PID->P +
                              Ecc_Car->Error.delta * Servo_PID->D;
        Ecc_Car->Duty.servo = RANGE(Ecc_Car->Duty.servo, 10000, -10000);
        Ecc_Car->Mode.servo = 0;
        Servo_time = 0;
    }

    if (Motor_time > Motor_T)
    {
        if (0 != Ecc_Car->Flag.obstacle)
            Motor_PID->set_value = Ecc_Car->Speed.obstacle; //�����ٶ�Ecc_Car->Speed.standard
        else if (0 != Ecc_Car->Round.state)
            Motor_PID->set_value = Ecc_Car->Speed.round;
        else if (0 != Ecc_Car->Flag.outside)
            Motor_PID->set_value = Ecc_Car->Speed.outside;
        else
            Motor_PID->set_value = Ecc_Car->Speed.standard;

        Motor_PID->set_value = RANGE(Motor_PID->set_value, 1000, 200);
        Motor_PID->P = PID_Adapt(&Adapt_M_Kp, Ecc_Car->Speed.now);
        Ecc_Car->Duty.motor += PID_Increase(Motor_PID, Ecc_Car->Speed.now);
        /****************************����޷��뱣��*******************************************/
        if (Ecc_Car->Speed.now > 200)
            Ecc_Car->Duty.motor = RANGE(Ecc_Car->Duty.motor, 100000, 0);
        else
            Ecc_Car->Duty.motor = RANGE(Ecc_Car->Duty.motor, 40000, 0);
        if (Ecc_Car->Speed.now > 100 && 0 == Ecc_Car->Flag.protect)
            Ecc_Car->Flag.protect = 1;
        else if (1 == Ecc_Car->Flag.protect && Ecc_Car->Speed.now < 20)
        {
            Ecc_Car->Flag.protect = 0;
            Ecc_Car->Flag.stop = 1;
        }

        /***********************************************************************/

        Motor_time = 0;
    }

    Servo_time++;
    Motor_time++;
}