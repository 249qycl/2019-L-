#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#define Time_Handler            \
    do                          \
    {                           \
        if (Ecc_Car->times > 0) \
            Ecc_Car->times--;   \
    } while (0) //�����ض�������ִ��ʱ�����

#define FLAGS_UPDATE       \
    do                     \
    {                      \
        Show.count++;      \
        Show.PIT_over = 1; \
    } while (0)

#define TFT_SCOPE                                                                                          \
    do                                                                                                     \
    {                                                                                                      \
        TFT_INPUT("��Ⱥ�:", (SUB0 * 100 / (Induct[2].Normalized_value + Induct[3].Normalized_value)), 0); \
        TFT_INPUT("��Ȼ�:", (SUB0 * 5000 / MUL0), 1);                                                     \
        TFT_INPUT("���2:", Induct[2].Normalized_value, 2);                                                \
        TFT_INPUT("���3:", Induct[3].Normalized_value, 3);                                                \
        TFT_INPUT("���4:", Induct[4].Normalized_value, 4);                                                \
    } while (0)

void PIT0_Handler();
#endif

/* //�ο����ӣ�Coertx-M4ϵ���ж�Ƕ�ס����ȼ�˵��������M0+ϵ��˵���� - ���ܳ������� - ɽ����̳ http://vcan123.com/forum.php?mod=viewthread&tid=959&ctid=1
NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);    //�������ȼ����飨���鷽ʽ4��,4bit ��ռ���ȼ�,û�������ȼ�(������0~15������)

NVIC_SetPriority(PORTA_IRQn,0);         //�������ȼ�    
VIC_SetPriority(PORTE_IRQn,1);         //�������ȼ�
���ȼ����飺���鷽ʽ0������4��bitΪ������������Ӧ���ȼ��������ж�֮�䲻�ܻ����ϣ�
            ���鷽ʽ1��1��bitλ����������ռ���ȼ���ʣ��3��bitλ����������Ӧ���ȼ���
            ���鷽ʽ2��2��bitλ����������ռ���ȼ���ʣ��2��bitλ����������Ӧ���ȼ���
            ���鷽ʽ3��3��bitλ����������ռ���ȼ���ʣ��1��bitλ����������Ӧ���ȼ���
            ���鷽ʽ4��4��bitλȫ����������ռ���ȼ���
��ռ���ȼ���ָ�жϴ�������жϵ����������ȼ��ߵĴ�����ȼ��͵��жϺ���
��Ӧ���ȼ��������ȼ�����ָ�������ȼ���ͬ���ж�ͬʱ����ʱ��ϵͳ���жϵ�ѡ�������� 
*/
