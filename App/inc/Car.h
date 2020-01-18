#ifndef __CAR_H__
#define __CAR_H__


typedef struct
{
	struct
	{
		uint8 servo;
		uint8 motor;
		uint8 move;			//���ڼ�¼���嵱ǰ��������ģʽ״��
	}Mode;
	
	struct
	{	
		uint8 protect:1;	//��������
		uint8 stop :1;		//����ͣ��
		uint8 outside :1;	//�����־
		uint8 buzzer :1;	//������
		uint8 hoare :1;		//����������
		uint8 calibrat:1;	//��ֵУ׼��־
		uint8 obstacle:2;	//�ϰ���
		uint8 laser1:1;		//����1
		uint8 laser2:1;		//����2
	}Flag;
   
	struct
	{
		int32 motor;
		int32 servo;
		int32 obstacle[3];
		float debug;
	}Duty;

	struct
	{
		int16 delta;	//ƫ��ı仯��
		int16 now;		//��ǰ����ƫ��  ��������Ϊ0  ������ƫΪ�� ��ƫΪ����
		int16 last;
	}Error;
	
	struct
	{
		uint8  state;
    	uint16 length;
    	uint32 time;
	}Distance;
	
	struct
	{
		uint8  finish; 		//ɨ�������־
    	uint16 times;      //ɨ�����
	}Scan;
	
	struct
	{	
		uint8  state;		
		uint16 threshold;		
		int32  ad_sum;//ˮƽ����ܺ�
		struct
		{
			int16 W[4];
			int16 Z[4];
			int16 N[4];
		}Debug;
	}Round;
	
	struct//�����Ԫ�ؽ��з�ʧ�ܱ�������
	{
		uint32 round;
		uint32 obstacle;
	}Protect;
	
	struct
	{
		int16  threshold;//���������Ϣǿ����ֵ		
	}Info;
	
	struct
	{
		int16 now;//��ǰ�ٶȲ���ֵ
		int16 standard;//�����ٶ�
		int16 round;//�����ٶ�
		int16 obstacle;//�����ٶ�
		int16 outside;//�����ٶ�
	}Speed;
	
	enum
	{
		Hz_50=50,
		Hz_100=100,
		Hz_200=200,
		Hz_250=250,
		Hz_300=300,
		S_FREQ=Hz_50,//���Ƶ��50Hz		
	}SERVO;
	enum
	{
		M_FREQ=13573,//���Ƶ�ʡ�13~17KHzΪ�ˣ��������е�г��Ƶ�ʳ�ͻ��
	}MOTOR;    
} Car, *pCar;

extern pCar Ecc_Car; //��������


/*******************************************************
*        ��ֵ1.4ms    ��ֵ1.8ms    ��ֵ1ms    Ƶ��Hz
*   duty    70          90           50        500 
*           56          72           40        400
*           35          45           25        250
*           28          36           20        200
*           14          18           10        100
*           7           9            5         50
*******************************************************/
//[B��300Hz:��37  �У�47    �ң�55]
//[L��50Hz����5.15  ��6.8 �ң�8.25]
#define L_VALUE 4.8f
#define M_VALUE 6.4f
#define R_VALUE 8.0f 



void Car_speed_get(pCar Car, uint8 T);
void Error_update(uint8 T);
void Car_init(void);
void Servo_set(float duty);
void Motor_set(float duty);
void Servo_move(uint8 mode);
void Motor_move(uint8 mode);
void Buzzer();
void Hoare_process();
void Car_Speed_Target();
#endif