#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define INDUCT_NUM 8U //�������
#define MAXSIZE 10U   //���ھ�ֵ�˲���������

#define OUTSIDE 50U        //��ʶ����ĵ�ѹֵ
#define DIR_U_LIMIT 220    //�����ƫ������
#define DIR_D_LIMIT (-220) //�����ƫ������

#define R_GAIN 1 //�ڲ��г����� 1
#define M_GAIN 1 //param.URt_k             //�м��г�����1
#define L_GAIN 1 //����г�����2
#define F_GAIN 7 //ǰ���г�����//3

typedef struct
{
    uint8 front; //�����ɼ��Ķ���
    uint8 rear;  //�����ɼ��Ķ�β

    uint16 Nor_min;      //���ڹ�һ���������С�ο�ֵ
    uint16 Nor_max;      //��е����ֵ
    uint16 Nor_MM_value; //���ֵ����Сֵ�Ĳ�

    uint8 Normalized_value;      //��һ���������
    uint8 Last_Normalized_value; //��ʷ��һ��������

    uint16 ad_mean_value;     //��ֵ
    uint16 ad_value[MAXSIZE]; //AD�ɼ���ԭʼ����,ʮ��������ƽ��
} Inductance, *pInductance;

extern Inductance Induct[INDUCT_NUM]; //����8�����

#define SCAN_NUM 2000 //ɨ�����
typedef struct
{
    uint8 finish_flag; //ɨ�������־
    uint16 times;      //ɨ�����
} SCAN;
extern SCAN Scan;

void AD_first_get();
void AD_sliding_get();
void AD_Scan();
void All_AD_init();
void Mean_Filter();
void Normalized();

#endif