#include "common.h"
#include "include.h"

pCar Ecc_Car; //��������
/***********************************************
*
*       ����������������Ϣ�ı������г�ʼ������
*
***********************************************/
pCar Car_create()
{
    pCar P = (pCar)malloc(sizeof(Car));

    P->Flag.protect = 0;
    P->Flag.outside = 0;
    P->Flag.calibrat = 0;
    P->Flag.laser1 = 0;
    P->Flag.laser2 = 0;
    P->Flag.obstacle = 0;
    P->Flag.buzzer = 0;
    P->Flag.stop = 0;
    P->Flag.hoare = 0;

    P->Duty.servo = 0;
    P->Duty.motor = 0;
    P->Duty.debug = 1;

    P->Mode.move = 0;
    P->Mode.servo = 0;
    P->Mode.motor = 0;

    P->Error.now = 0;
    P->Error.last = 0;
    P->Error.delta = 0;

    P->Protect.round = 0;
    P->Protect.obstacle = 0;

    P->Info.threshold = 100;

    P->Speed.now = 0;
    P->Speed.standard = 200; //300
    P->Speed.round = 200;
    P->Speed.obstacle = 200;
    P->Speed.outside = 200;

    P->Duty.obstacle[0] = 8000;
    P->Duty.obstacle[1] = -3000;
    P->Duty.obstacle[2] = -10000;

	P->Scan.finish=0;
	P->Scan.times=SCAN_NUM;
	
    P->Round.state = 0;
    P->Round.threshold = 420;
    P->Round.ad_sum = 0;

    P->Round.Debug.W[0] = 1; //2  0  2
    P->Round.Debug.Z[0] = 0; //[���뻷Ť��]
    P->Round.Debug.N[0] = 1;

    P->Round.Debug.W[1] = 0;
    P->Round.Debug.Z[1] = 2; //��
    P->Round.Debug.N[1] = 0;

    P->Round.Debug.W[2] = 5;  //5//2
    P->Round.Debug.Z[2] = 10; //��ǿ���ڵ�����
    P->Round.Debug.N[2] = 1;  //1

    P->Round.Debug.W[3] = 1; //101
    P->Round.Debug.Z[3] = 0;
    P->Round.Debug.N[3] = 1;

    return P;
}

/******************************************************
*
*       ��ȡ�������ķ���ֵ
*
******************************************************/
void Car_speed_get(pCar Car, uint8 T)
{
#define SPEED_NUM 5

    static int16 queue[SPEED_NUM] = {0};
    static uint8 rear = 0, sample_times = 0;
    int32 sum = 0;

    if (sample_times > T)
    {
        int16 val = ftm_quad_get(FTM1); //��ȡFTM �������� ��������(������ʾ������)
        ftm_quad_clean(FTM1);           //������������¿�ʼ

        rear = (rear + 1) % SPEED_NUM;
        queue[rear] = val;
        for (uint8 i = SPEED_NUM; i > 0; i--)
        {
            sum += queue[(rear + i) % SPEED_NUM] * i; //(i+1)
        }
        Car->Speed.now = sum / 150; //���ƾ�ֵ�˲����ֵ��С10��[210]
        sample_times = 0;
    }
    sample_times++;

#undef SPEED_NUM
}

/*************************************************
*
*       ���ڸ�����ʷ����ƫ����Ϣ����ƫ����΢�֡�
*       T
*
*************************************************/
void Error_update(uint8 T)
{
    static uint8 times = 0;
    if (times++ > T && Ecc_Car->Flag.outside == 0)
    {
        times = 0;
        Ecc_Car->Error.delta = Ecc_Car->Error.last - Ecc_Car->Error.now;
        Ecc_Car->Error.last = Ecc_Car->Error.now;
    }
}

/*****************************************************
*
*       ��ʼ����������λ�ü���Ӧ����ģ��
*
*****************************************************/
void Car_init()
{
    Ecc_Car = Car_create();
    gpio_init(PTE10, GPO, 1);//������
    /*ģ��0��ͨ��2  3*/
    ftm_pwm_init(FTM0, FTM_CH2, M_FREQ, 0.0f, FTM0_CH2_PIN); //�����ʼ��Ƶ��Ϊ10k ��ռ�ձ�Ϊ 00.0%
    ftm_pwm_init(FTM0, FTM_CH3, M_FREQ, 0.0f, FTM0_CH3_PIN); //�����ʼ��Ƶ��Ϊ10k ��ռ�ձ�Ϊ 00.0%
    /*FTM1 ���������ʼ��*/
    ftm_quad_init(FTM1, FTM_PS_1, FTM1_QDPHA_PIN, FTM1_QDPHB_PIN);
    /*ģ��1��ͨ��0*/
    ftm_pwm_init(FTM2, FTM_CH0, S_FREQ, M_VALUE, FTM2_CH0_PIN); //L�������ʼ��50Hz,��������λ��
}

/************************************************************
*
*					����������
*
************************************************************/
void Buzzer()
{
    static uint8 times = 0;
    if (1 == Ecc_Car->Flag.buzzer && times < 120)
    {
        gpio_set(PTE10, 0);
        times++;
    }
    else
    {
        gpio_set(PTE10, 1);
        times = 0;
        Ecc_Car->Flag.buzzer = 0;
    }
/****************��ȡ�����״̬**********************/
    if (0 == gpio_get(LASER_PORT1))
    {
        Ecc_Car->Flag.laser2 = 1;
        led(LED3, LED_ON);
    }
    else
    {
        led(LED3, LED_OFF);
        Ecc_Car->Flag.laser2 = 0;
    }

    if (0 == gpio_get(LASER_PORT2))
    {
        led(LED2, LED_ON);
        Ecc_Car->Flag.laser1 = 1;
    }
    else
    {
        led(LED2, LED_OFF);
        Ecc_Car->Flag.laser1 = 0;
    }
}

/****************************************************
*
*       ���ö����λ��
*       ��Ϊ�Ҵ�ǣ���Ϊ����
****************************************************/
void Servo_set(float duty) //-100~100
{
    duty = duty > 0 ? (duty / 100 * (R_VALUE - M_VALUE) + M_VALUE) : (duty / 100 * (M_VALUE - L_VALUE) + M_VALUE);
    duty = duty < L_VALUE ? L_VALUE : (duty > R_VALUE ? R_VALUE : duty);
    ftm_pwm_duty(FTM2, FTM_CH0, duty);
}
/*****************************************************
*
*       �����Ӧģʽ�˶�
*
******************************************************/
void Servo_move(uint8 mode)
{

    switch (mode)
    {
    case 0:
        Servo_set(Ecc_Car->Duty.servo / 100);
        break; //ִ�����Զ���ת
    case 3:
        Servo_set(-100);
        break; //�����
    case 4:
        Servo_set(100);
        break; //�Ҵ���
    case 5:
        break;

    default:
        Servo_set(Ecc_Car->Duty.servo / 100);
        break;
    }
}
/*******************************************************
*
*       �����ռ�ձ�����
*
*******************************************************/
void Motor_set(float duty)
{
    duty = RANGE(duty, 1000, -1000);
    duty /= 10;

    if (duty > 0)
    {
        ftm_pwm_duty(FTM0, FTM_CH2, duty); //����ռ�ձ�
        ftm_pwm_duty(FTM0, FTM_CH3, 0);
    }
    else
    {
        ftm_pwm_duty(FTM0, FTM_CH2, 0); //����ռ�ձ�
        ftm_pwm_duty(FTM0, FTM_CH3, -duty);
    }
}
/********************************************************
*
*       �������Ӧ��ģʽ�˶�
*
********************************************************/
void Motor_move(uint8 mode)
{

    if (1 == Ecc_Car->Flag.stop)
        mode = 9;

    switch (mode)
    {
    case 0:
        Motor_set((float)Ecc_Car->Duty.motor / 100);
        break; //ִ�����Զ���ת
    case 1:
        Motor_set(150);
        break; //�������ٶ�
    case 3:
        Motor_set(0);
        break; //��ɲ
    case 4:
        Motor_set(1000);
        break; //ȫ��
    case 9:
        Motor_set(0);
        break; //ͣ����ɲ��
    default:
        Motor_set(Ecc_Car->Duty.motor);
        break; //ִ�����Զ���ת
    }
}
/*************************************************************
*
*						����ͣ�����
*
*************************************************************/
void Hoare_process()
{
    static uint8 times = 0, re_judge = 0;
    if (1 == Ecc_Car->Flag.hoare && 1 == gpio_get(HOARE_PORT))
        times++;
    else
    {
        times = 0;
        Ecc_Car->Flag.hoare = 0;
    }
    if (times > 1 && 0 == re_judge)
    {
        Ecc_Car->Flag.buzzer = 1;
        Ecc_Car->Flag.stop = 1;
        re_judge = 1;
        times = 0;
    }
    if (0 == gpio_get(HOARE_PORT))
        re_judge = 0;
}
