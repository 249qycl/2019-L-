#include "common.h"
#include "include.h"

DEB deb_control;
float a;
/*��������ʾ����������*/
#define INV_PARAM0 (Ecc_Car->Error.now * 100)
#define INV_PARAM1 (Ecc_Car->Error.delta * 100)
#define INV_PARAM2 (Ecc_Car->Duty.servo)
#define INV_PARAM3 (0)
#define INV_PARAM4 (0)
#define INV_PARAM5 (0)
#define INV_PARAM6 (0)
#define INV_PARAM7 (0)
#define INV_PARAM8 (0)
#define INV_PARAM9 (0)
#define INV_PARAM10 (0)
#define INV_PARAM11 (0)
#define INV_PARAM12 (0)
#define INV_PARAM13 (0)
#define INV_PARAM14 (0)
#define INV_PARAM15 (0)
/*�����������������*/
#define VAR_PARAM0 a
#define VAR_PARAM1 a
#define VAR_PARAM2 a
#define VAR_PARAM3 a
#define VAR_PARAM4 a
#define VAR_PARAM5 a
#define VAR_PARAM6 a
#define VAR_PARAM7 a
#define VAR_PARAM8 a
#define VAR_PARAM9 a
#define VAR_PARAM10 a
#define VAR_PARAM11 a
#define VAR_PARAM12 a
#define VAR_PARAM13 a

static float INV_Var[20];
//С��ģʽ�洢�����λ����0��λ��
static union {
    char buff[4];
    float data;
} DATA;

#define my_putchar(temp) uart_putchar(VCAN_PORT, temp); //����ʵ�ʵĴ��ں����޸�,Ĭ��Ϊϵͳ��ʼ�˿�

/*����֪ͨ��λ���µ�һ�����ݿ�ʼ��Ҫ�������ݱ��뷢����*/
void Send_Begin()
{
    my_putchar(0x55);
    my_putchar(0xaa);
    my_putchar(0xae);
}
/*******************************************************
*
*          ���±�����Ϣ��ʹ������ʾ����ʱ�����
*
*******************************************************/
void Variable_update()
{
    INV_Var[0] = INV_PARAM0;
    INV_Var[1] = INV_PARAM1;
    INV_Var[2] = INV_PARAM2;
    INV_Var[3] = INV_PARAM3;
    INV_Var[4] = INV_PARAM4;
    INV_Var[5] = INV_PARAM5;
    INV_Var[6] = INV_PARAM6;
    INV_Var[7] = INV_PARAM7;
    INV_Var[8] = INV_PARAM8;
    INV_Var[9] = INV_PARAM9;
    INV_Var[10] = INV_PARAM10;
    INV_Var[11] = INV_PARAM11;
    INV_Var[12] = INV_PARAM12;
    INV_Var[13] = INV_PARAM13;
    INV_Var[14] = INV_PARAM14;
    INV_Var[15] = INV_PARAM15;
}

/******************************************************
*
*               Var_num:�����ͱ����ĸ���
*
******************************************************/
void Send_Variable(uint8 Var_num) //���ͱ�������λ��
{
    uint8 i, j;
    my_putchar(0x55);
    my_putchar(0xaa);
    my_putchar(0xad);
    my_putchar(Var_num);
    for (i = 0; i < Var_num; i++)
    {
        DATA.data = INV_Var[i];
        for (j = 0; j < 4; j++)
            my_putchar(DATA.buff[j]);
    }
    my_putchar(0x0d);
}

void Modify_Parameter(uint8 *buff)
{
    uint8 i, j;
    uint8 addr = 0, Parameter_num = 14; //14���ɸĲ���
                                        /* �޸Ĳ����б�*/
    for (i = 0; i < Parameter_num; i++)
    {
        for (j = 0; j < 4; j++)
        {
            DATA.buff[j] = *(uint8 *)(buff + addr);
            addr++;
        }
        switch (i)
        {
        case 0:
            VAR_PARAM0 = DATA.data;
            break;
        case 1:
            VAR_PARAM1 = DATA.data;
            break;
        case 2:
            VAR_PARAM2 = DATA.data;
            break;
        case 3:
            VAR_PARAM3 = DATA.data;
            break;
        case 4:
            VAR_PARAM4 = DATA.data;
            break;
        case 5:
            VAR_PARAM5 = DATA.data;
            break;
        case 6:
            VAR_PARAM6 = DATA.data;
            break;
        case 7:
            VAR_PARAM7 = DATA.data;
            break;
        case 8:
            VAR_PARAM8 = DATA.data;
            break;
        case 9:
            VAR_PARAM9 = DATA.data;
            break;
        case 10:
            VAR_PARAM10 = DATA.data;
            break;
        case 11:
            VAR_PARAM11 = DATA.data;
            break;
        case 12:
            VAR_PARAM12 = DATA.data;
            break;
        case 13:
            VAR_PARAM13 = DATA.data;
            break;
        default:
            break;
        }
    }
}
/*****************************************************
*
*         ���Ϳ��޸ĵı���ֵ����λ��      
*
*****************************************************/
void Send_Parameter()
{
    uint8 i, j;
    uint8 Parameter_num = 14; //14���ɸĲ���
    my_putchar(0x55);
    my_putchar(0xaa);
    my_putchar(0xab);
    my_putchar(Parameter_num);
    for (i = 0; i < Parameter_num; i++)
    {
        switch (i)
        {
        case 0:
            DATA.data = VAR_PARAM0;
            break;
        case 1:
            DATA.data = VAR_PARAM1;
            break;
        case 2:
            DATA.data = VAR_PARAM2;
            break;
        case 3:
            DATA.data = VAR_PARAM3;
            break;
        case 4:
            DATA.data = VAR_PARAM4;
            break;
        case 5:
            DATA.data = VAR_PARAM5;
            break;
        case 6:
            DATA.data = VAR_PARAM6;
            break;
        case 7:
            DATA.data = VAR_PARAM7;
            break;
        case 8:
            DATA.data = VAR_PARAM8;
            break;
        case 9:
            DATA.data = VAR_PARAM9;
            break;
        case 10:
            DATA.data = VAR_PARAM10;
            break;
        case 11:
            DATA.data = VAR_PARAM11;
            break;
        case 12:
            DATA.data = VAR_PARAM12;
            break;
        case 13:
            DATA.data = VAR_PARAM13;
            break;
        default:
            break;
        }
        for (j = 0; j < 4; j++)
            my_putchar(DATA.buff[j]);
    }
    my_putchar(0x0b); //֡β
}

void Uart_Handler()
{
    static uint8 recv;
    static uint8 data_cnt = 0;
    static uint8 Recv_Buff[100];
    static uint8 Data_Receiving = FALSE;
    if (uart_query(VCAN_PORT) == 1)
        uart_getchar(VCAN_PORT, (char *)(&recv)); //����ʵ�ʵĴ������޸�

    /**********�������ڽ���������λ���Ĳ�������*********/
    if (Data_Receiving)
    {
        if (data_cnt < 56)
        {
            Recv_Buff[data_cnt] = recv;
            data_cnt++;
        }
        else
        {
            data_cnt = 0; //�ﵽ֡��
            Data_Receiving = FALSE;
            if (recv == 0xab) //֡β
            {
                Modify_Parameter(Recv_Buff); //�޸Ĳ���
                Send_Parameter();            //�����ش���ȷ�ϲ����޸����,[����λ�����Ͳ���]
            }
        }
    }
    else
    {
        switch (recv) //�����ֽ���
        {
        case 0x30:
            deb_control.switch_1 = !deb_control.switch_1; //��ͣ����������
            break;

        case 0x31:
            deb_control.switch_2 = !deb_control.switch_2; //���Ϳ���������
            break;

        case 0x32:
            deb_control.switch_3 = !deb_control.switch_3; //��ȡ����������
            break;

        case 0x33:
            Data_Receiving = TRUE; //��������
            data_cnt = 0;
            break;

        case 0x34:
            deb_control.switch_4 = !deb_control.switch_4; //�������������
            break;

        case 0x35:
            deb_control.switch_5 = !deb_control.switch_5; //���濪��������
            break;
        default:
            break;
        }
    }
    uart_rx_irq_en(VCAN_PORT); //ʹ�ܴ��ڽ����жϣ���ֹ�������жϱ��ر�
}

void Host_hander(uint8 var_num)
{
    if (deb_control.switch_2 == 1)
    {
        Variable_update();
        Send_Variable(var_num);
    }
    if (deb_control.switch_3 == 1)
    {
        Send_Parameter();
        deb_control.switch_3 = 0;
    }
    if (deb_control.switch_4 == 1)
    {
        my_data_save(); //�浽flash
        deb_control.switch_4 = 0;
    }
    if (deb_control.switch_5 == 1) //��flash��������
    {
        my_data_load();
        deb_control.switch_5 = 0;
    }
}