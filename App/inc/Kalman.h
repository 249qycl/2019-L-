#ifndef __KALMAN_H__
#define __KALMAN_H__

//�����������˲�
typedef struct {
	float x;  // ϵͳ��״̬��
	float A;  // x(n)=A*x(n-1)+u(n),u(n)~N(0,q)
	float H;  // z(n)=H*x(n)+w(n),w(n)~N(0,r)
	float q;  // Ԥ���������Э����
	float r;  // ������������Э����
	float p;  // �������Э����
	float gain;//����������
}KalmanStructTypedef;

extern KalmanStructTypedef Err_Kalman;

void kalmanFilter_init(KalmanStructTypedef *kalmanFilter, float init_x, float init_p, float predict_q, float newMeasured_q);
float kalmanFilter_filter(KalmanStructTypedef *kalmanFilter, float newMeasured);


void Kalman_filter_init(KalmanStructTypedef *Q, int16 init_x, float init_p, float predict_q, float newMeasured_q);
int16 Kalman_filter(KalmanStructTypedef *Q,int16 newMeasured);
#endif