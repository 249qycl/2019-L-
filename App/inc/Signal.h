#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define INDUCT_NUM 6U 		//�������
#define MAXSIZE   10U   	//���ھ�ֵ�˲���������

typedef struct
{
    uint8 front; //�����ɼ��Ķ���
    uint8 rear;  //�����ɼ��Ķ�β

	struct
	{
		uint16 min;//���ڹ�һ���������С�ο�ֵ
		uint16 max;//��е����ֵ
		uint16 range;//���ֵ����Сֵ�ļ���
		uint8  now;//��һ���������
		uint8 last;//��ʷ��һ��������
	}Nor;

	struct
	{
		uint16 average;//��ֵ
		uint16 data[MAXSIZE];//AD�ɼ���ԭʼ����,ʮ��������ƽ��
	}AD;
} Inductance, *pInductance;

extern Inductance Induct[INDUCT_NUM]; //����8�����
                    
#define SCAN_NUM 2000 //ɨ�����

void AD_first_get();
void AD_sliding_get();
void AD_Scan();
void All_AD_init();
void Mean_Filter();
void Normalized();

#endif