#include "common.h"
#include "include.h"


void main()
{
    Vector_handler_init();		//�ж�����	
    led_init(LED0);    			//��ʼ�� LED0���������ж��Ƿ������жϡ�
	led_init(LED3);    			//��ʼ�� LED3
	led_init(LED2);    			//��ʼ�� LED2
	Distance_init();		   	//�����ϳ�ʼ��
    All_PID_init();    			//PID���ݳ�ʼ��
    my_flash_init();   			//flash�洢���ݳ�ʼ��
    Car_init();        			//�����ʼ��
    All_AD_init();     			//AD�ɼ���ʼ��
    lcd_init(BLACK);   			//��Ļ��ʼ��
    Boot_action();     			//��������
    key_init(KEY_MAX); 			//��ʼ��ȫ�� ����
    Tree_init();       			//��ϵ����ʼ�� 
	Vector_enable_init();		//�ж�ʹ��
    while (1)                  
    {
        if (1==Ecc_Car->Scan.finish )
        {
            Show_Handler;
            key_IRQHandler(); //����ɨ��
			Distance_measure();
            //#if   HOST_KEY==1
            //    Host_hander(10);
            //#elif HOST_KEY==2
            //    Host_Handler;
            //#endif
        }
    }
}