#include "common.h"
#include "include.h"


void kalmanFilter_init(KalmanStructTypedef *kalmanFilter, float init_x, float init_p, float predict_q, float newMeasured_q)
{
	kalmanFilter->x = init_x;//�������ĳ�ʼֵ��������ֵһ�������ֵ
	kalmanFilter->p = init_p;//����״̬����ֵ���ķ���ĳ�ʼֵ����ҪΪ0���ⲻ��
	kalmanFilter->A = 1;
	kalmanFilter->H = 1;
	kalmanFilter->q = predict_q;//Ԥ�⣨���̣��������� Ӱ���������ʣ����Ը���ʵ���������
	kalmanFilter->r = newMeasured_q;//�������۲⣩�������� ����ͨ��ʵ���ֶλ��
}

/**
 *@function: - �������˲���
 *@kalmanFilter:�������ṹ��
 *@newMeasured������ֵ
 *�����˲����ֵ
 */
float kalmanFilter_filter(KalmanStructTypedef *kalmanFilter, float newMeasured)
{
	/* Predict */
	kalmanFilter->x = kalmanFilter->A * kalmanFilter->x;//%x�������������һ��ʱ���ĺ������ֵ��������Ϣ����
	kalmanFilter->p = kalmanFilter->A * kalmanFilter->A * kalmanFilter->p + kalmanFilter->q;  /*������������� p(n|n-1)=A^2*p(n-1|n-1)+q */

	/* Correct */
	kalmanFilter->gain = kalmanFilter->p * kalmanFilter->H / (kalmanFilter->p * kalmanFilter->H * kalmanFilter->H + kalmanFilter->r);
	kalmanFilter->x = kalmanFilter->x + kalmanFilter->gain * (newMeasured - kalmanFilter->H * kalmanFilter->x);//���ò������Ϣ���ƶ�x(t)�Ĺ��ƣ�����������ƣ����ֵҲ�������
	kalmanFilter->p = (1 - kalmanFilter->gain * kalmanFilter->H) * kalmanFilter->p;//%������������

	return kalmanFilter->x;
}

/***********��������������************/
KalmanStructTypedef Err_Kalman;
void Kalman_filter_init(KalmanStructTypedef *Q, int16 init_x, float init_p, float predict_q, float newMeasured_q)
{
    Q->x=init_x;
    Q->p=init_p;
    Q->q=predict_q;
    Q->r=newMeasured_q;
}

int16 Kalman_filter(KalmanStructTypedef *Q,int16 newMeasured)
{
    Q->p=Q->p+Q->q;
    Q->gain=Q->p/(Q->p+Q->r);
    Q->x=Q->x+Q->gain*(newMeasured-Q->x);
    Q->p=(1-Q->gain)*Q->p;
	
    return (int16)Q->x;
}