#ifndef __PID__H_
#define __PID__H_

#define TURN_ON  1
#define TURN_OFF 0

#define S_P 66
#define S_I 0
#define S_D 360

#define S_LIMIT 100
#define S_PIDMODE TURN_OFF     //������ַ��뿪��
#define S_DEAD_LIMIT 10        //������С
#define S_DEAD_SWITCH TURN_OFF //���������������

#define M_P 80 //17
#define M_I 8  //1
#define M_D 0
#define M_LIMIT 100
#define M_PIDMODE TURN_OFF     //������ַ��뿪��
#define M_DEAD_LIMIT 2         //������С
#define M_DEAD_SWITCH TURN_OFF //���������������

typedef struct
{
    int set_value;  //Ŀ��ֵ��������ٶ�       ���������ƫ������
    int prev_error; //Error[t-2]
    int last_error; //Error[t-1]
    int16 P;
    int16 I;
    int16 D;
    int32 sum_error; //�ۼ����,��������

    int last_out;
    uint16 error_limit;    //���л��ַ���ʱ��ƫ�����
    uint8 saturation_flag; //������ͱ�־
} PID, *pPID;

extern pPID Servo_PID; //Ŀ��ֵΪ����ƫ��
extern pPID Motor_PID; //Ŀ��ֵΪ����

typedef struct
{
    uint8 TH;    //�����������Ƶ���ֵ
    uint8 P_Th0; //����ϵ���ֶ���ֵ
    uint8 P_Th1;
    uint8 P_Th2;
    uint8 P_Th3;

    uint16 D_Th0; //΢��ϵ���ֶ���ֵ
    uint16 D_Th1;
    uint16 D_Th2;
    uint16 D_Th3;

    int16 P0;
    int16 D0;

    int16 P1;
    int16 D1;

    int16 P2;
    int16 D2;

    int16 P3;
    int16 D3;
} SecPID;
extern SecPID Servo_pid;

#define SEC_JUDGE(x, y)     (temp > Q->x##_Th##y)
#define SYMBOL(x)           do{x = Ecc_Car->Dir_Error >= 0 ? x : (-x);}while(0)

void All_PID_init();
int PID_Position(pPID Q, int input);
int PID_Increase(pPID Q, int input);
int Section_PID(SecPID *Q, pPID T, int16 input1, int16 input2);
void PID_Handler(uint8 S_frent, uint8 M_frent);

#endif