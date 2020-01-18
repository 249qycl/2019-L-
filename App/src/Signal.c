#include "common.h"
#include "include.h"

//SCAN Scan;                     //������ʼɨ��
Inductance Induct[INDUCT_NUM]; //�����ж���
ADCn_Ch_e ADC_Channel[INDUCT_NUM] = {
    ADC0_SE8,
    ADC0_SE9,
    ADC0_SE10,
    ADC0_SE11,
    ADC0_SE12,
    ADC0_SE13,
}; //AD�ɼ�ͨ��ADC0_SE17, ADC0_SE18

/*****************************************************
*
*       ��ADCͨ������г�ʼ״̬��Ϣ��������ݽ��г�ʼ��
*
******************************************************/
void ADC_init()
{
    for (uint8 j = 0; j < INDUCT_NUM; j++) //�����Ϣ�洢�ռ��ʼ��
    {
        adc_init(ADC_Channel[j]);
        Induct[j] = (Inductance){
            .AD.data = {0},
            .AD.average = 0,
            .front = 0,
            .rear = 0,
            .Nor.last = 0,
            .Nor.now = 0};
    }
    DIR_gain = (DIR_ERR){L_GAIN, M_GAIN, R_GAIN};
}
/***************************************
*
*               ��ֵ�˲�
*
***************************************/
void Mean_Filter1()
{
    for (uint8 j = 0; j < INDUCT_NUM; j++)
    {
        uint32 sum = 0;
        for (uint8 i = 0; i < MAXSIZE; i++)
        {
            sum += Induct[j].AD.data[i]; //���ƾ�ֵ
        }
        Induct[j].AD.average = sum / 10;
    }
}

void Mean_Filter()
{
    for (uint8 i = 0; i < INDUCT_NUM; i++)
    {
        int32 sum = 0;
        for (uint8 j = MAXSIZE; j > 0; j--)
            sum += Induct[i].AD.data[(Induct[i].rear + j) % MAXSIZE] * j;
        Induct[i].AD.average = sum / 55;
    }
}
/*******************************************
*
*       �״�AD�ɼ����òɼ������������ռ�
*
*******************************************/
static void AD_first_get()
{
    for (uint8 i = 0; i < MAXSIZE; i++)
    {
        for (uint8 j = 0; j < INDUCT_NUM; j++)
        {
            Induct[j].AD.data[i] = adc_once(ADC_Channel[j], ADC_10bit);
        }
        DELAY_MS(5);
    }
    Mean_Filter();
    for (uint8 i = 0; i < INDUCT_NUM; i++)
    {
        Induct[i].Nor.min = Induct[i].AD.average;
        Induct[i].Nor.max = Induct[i].AD.average;
    }
}

/************************************************
*
*       �������У����л����ɼ�
*
************************************************/
void AD_sliding_get()
{
    for (uint8 i = 0; i < INDUCT_NUM; i++)
    {
        Induct[i].AD.data[Induct[i].rear] = adc_once(ADC_Channel[i], ADC_10bit); //�ɼ������ķ���ֵ
        Induct[i].front = (Induct[i].front + 1) % MAXSIZE;
        Induct[i].rear = (Induct[i].rear + 1) % MAXSIZE;
    }
}
/***************************************
*
*       �������ڹ�һ���������Сֵ
*       Q:��Ӧ��еĵ�ַ
***************************************/
static void Get_min_max(pInductance Induct)
{
    if (Induct->AD.average > Induct->Nor.max)
        Induct->Nor.max = Induct->AD.average;
    else if (Induct->AD.average < Induct->Nor.min)
        Induct->Nor.min = Induct->AD.average;
}
/**********************************************
*
*       AD��ֵɨ�衾���ж��е��á�
*
**********************************************/
void AD_Scan()
{
    AD_sliding_get();
    Mean_Filter();
    for (uint8 i = 0; i < INDUCT_NUM; i++) //��ֵɨ��
    {
        Get_min_max(&Induct[i]);
        Induct[i].Nor.range = Induct[i].Nor.max - Induct[i].Nor.min;
    }
    Ecc_Car->Scan.times--;
    if (0 == Ecc_Car->Scan.times)
        Ecc_Car->Scan.finish = 1;
}

/****************************************
*
*       AD���й��ܳ�ʼ��
*
****************************************/
void All_AD_init()
{
    ADC_init();
    AD_first_get();
}

/****************************************************
*
*               ��һ������
*
****************************************************/
void Normalized()
{
    for (uint8 i = 0; i < INDUCT_NUM; i++)
    {
        int temp = (Induct[i].AD.average - Induct[i].Nor.min) * 100 / Induct[i].Nor.range;
        Induct[i].Nor.now = RANGE(temp, 255, 0);
    }
}
