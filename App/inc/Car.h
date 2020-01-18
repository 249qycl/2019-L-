#ifndef __CAR_H__
#define __CAR_H__

extern uint8  get_times;
typedef struct 
{
    uint8   mv_mode;            //���ڼ�¼���嵱ǰ��������ģʽ״�����γɱ�����������
    uint8   Servo_mode;
    uint8   Motor_mode;
    
    uint8   outside_flag;       //�����־
     
    uint16  times;
    int32   M_Duty;
    int32   S_Duty;
    
    int16   Delta_err;          //ƫ��ı仯��
    int16   Dir_Error;          //����ƫ��  ��������Ϊ0  ������ƫΪ�� ��ƫΪ����
    int16   Last_Error;
    
    int16   DIR_k;              //����仯��б��
  
    int16   speed;              //����
}Car,*pCar;

extern pCar Ecc_Car;//��������

typedef struct
{
    uint8   Rt_k;               //������ֱ�������ǿ�ȡ�70��
    uint8   IRt_k;              //��������ʱ��ֱ�������Ч��
    char    ORt_k;              //����ʱ��ֱ�������Ч��
    uint8   URt_k;              //������ֱ�������Ч��
    
    uint8   in_round;           //������־
    int     All_ad_sum;         //ˮƽ����ܺ�
    float   S_duty;             //������Բ���
    
    int     D_adj;
    int     P_adj;
}ADJ;

extern ADJ param;


#define CAR_SPEED_HIGH          500
#define CAR_SPEED_LOW           200


/*******************************************************
*        ��ֵ1.4ms    ��ֵ1.8ms    ��ֵ1ms    Ƶ��Hz
*   duty    70          90           50        500 
*           56          72           40        400
*           35          45           25        250
*           28          36           20        200
*           14          18           10        100
*           7           9            5         50
*******************************************************/
#define Hz_50           50
#define Hz_100          100
#define Hz_200          200
#define Hz_250          250
#define Hz_300			300

#define M_FRENT         13000      //���Ƶ�ʡ�13~17KHzΪ�ˣ��������е�г��Ƶ�ʳ�ͻ��

#define B_CAR           0
#define L_CAR           1
#define CAR_MODEL       1         //��ģ�ͺ�L��B

//[B��300Hz:��37  �У�47    �ң�55]  
//[L��50Hz����5.15  ��6.8 �ң�8.25]
#define S_FRENT         Hz_300      //���Ƶ��50Hz                  
#define L_VALUE         37 
#define M_VALUE         47 
#define R_VALUE         55         

void Car_speed_get(pCar Q,uint8 frequency);
void Error_update(uint8 frequency);
void Car_init(void);
void Servo_set(float duty);
void Motor_set(float duty);
void Servo_move(uint8 mode);
void Motor_move(uint8 mode);
void Car_Speed_Target();
void DIR_error_slope();
void Death_limit();
uint8 Special_action(uint8 dir,uint8 speed1,uint8 speed2,uint8 add_time,uint8 sub_time);
#endif