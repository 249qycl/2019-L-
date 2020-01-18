#include "common.h"
#include "include.h"

//      DELAY_MS(100);                      
void main()
{
    /*****************�жϳ�ʼ����************************/
    pit_init_ms(PIT0, 5); //��ʱ���ж�5ms
    ASSERT(enter_init());
    /*******************�ж����ȼ�������******************/
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);

    NVIC_SetPriority(UART3_RX_TX_IRQn, 0);
    NVIC_SetPriority(PIT0_IRQn, 1);
    /***************�ж�����װ�������������********************/
    set_vector_handler(UART3_RX_TX_VECTORn, Uart_Handler);
    set_vector_handler(PIT0_VECTORn, PIT0_Handler);
    led_init(LED0);    //��ʼ�� LED0���������ж��Ƿ������жϡ�
    All_PID_init();    //PID���ݳ�ʼ��
    my_flash_init();   //flash�洢���ݳ�ʼ��
    Car_init();        //�����ʼ��
    All_AD_init();     //AD�ɼ���ʼ��
    lcd_init(BLACK);   //��Ļ��ʼ��
    Boot_action();     //��������
    key_init(KEY_MAX); //��ʼ��ȫ�� ����
    Tree_init();       //��ϵ����ʼ��

    /********************�ж�ʹ����***********************/
    enable_irq(PIT0_IRQn);     //����ʱ���ж�
    uart_rx_irq_en(VCAN_PORT); //�����ڽ����ж�

    while (1) //ѭ���������������ݽ������������׿���
    {
        if (Scan.finish_flag == 1)
        {
            Show_Handler;
            key_IRQHandler(); //����ɨ��

            //#if   HOST_KEY==1
            //    Host_hander(10);
            //#elif HOST_KEY==2
            //    Host_Handler;
            //#endif
        }
    }
}