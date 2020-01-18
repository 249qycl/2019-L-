#ifndef __SHOW_H__
#define __SHOW_H__
/*��320x��240*/
//extern uint8 show_frequency,show_flag,Timer_over;
extern uint8 show_switch;

#define BACK_C BLACK
#define FONT_C YELLOW

typedef struct
{
    uint8 count;
    uint8 flag;
    uint8 PIT_over; //��ʱ���жϽ�����־
    uint8 key;
    char strbuff[30]; //�ַ�������
} SHOW;
extern SHOW Show;

typedef struct
{
    uint8 ok_flag;   //�޸�����
    uint8 wr_flag;   //0��ʾ�ɶ���1��ʾ��д�����ڱ����޸ģ�
    uint8 c_state;   //�Ӵ���״̬1���ڣ�0����
    uint8 only_show; //���ײ����ֹ������Ѱ
    uint8 show_mode; //��ʾ����
    uint8 gra_h;     //�Ӵ����ʷ�Χ
    uint8 gra_l;     //�Ӵ����ʷ�Χ
    char grade;      //�ݽ��Ĳ㼶
    char temp;       //�����ı���
    char parents;
    char child;
} Node, *pNode;

typedef struct
{
    uint16 nor_min[INDUCT_NUM];      //��й�һ����С�ο�ֵ
    uint16 nor_MM_value[INDUCT_NUM]; //��й�һ����ֵ

    int16 sP0;
    int16 sP1;
    int16 sP2;
    int16 sP3;
    int16 sD0;
    int16 sD1;
    int16 sD2;
    int16 sD3;

    int16 mP;
    int16 mI;
    int16 mD;

    int32 cM_Duty;
} Data, *pData;

#define lcd_STR_ench(sx, sy, Fc, Bc, ...)                              \
    do                                                                 \
    {                                                                  \
        sprintf(Show.strbuff, ##__VA_ARGS__);                          \
        lcd_str_ench((Site_t){sx, sy}, (uint8 *)Show.strbuff, Fc, Bc); \
    } while (0)

#define CLEAR                               \
    do                                      \
    {                                       \
        if (last_mode != Tree[i].show_mode) \
        {                                   \
            lcd_clear(BACK_C);              \
            last_mode = Tree[i].show_mode;  \
        }                                   \
    } while (0)

#define SEAT                                                                   \
    do                                                                         \
    {                                                                          \
        for (uint8 k = Tree[i].gra_l; k < (Tree[i].gra_h + 1); k++)            \
        {                                                                      \
            seat[k] = k == Tree[i].grade ? (Tree[i].ok_flag == 1 ? 2 : 1) : 0; \
        }                                                                      \
    } while (0)

#define GEAR_SHOW                                                           \
    do                                                                      \
    {                                                                       \
        switch (gear_chose)                                                 \
        {                                                                   \
        case 0:                                                             \
            lcd_STR_ench(0, 30, Color[seat[0]], BACK_C, "������λ:x0.001"); \
            break;                                                          \
        case 1:                                                             \
            lcd_STR_ench(0, 30, Color[seat[0]], BACK_C, "������λ:x 0.01"); \
            break;                                                          \
        case 2:                                                             \
            lcd_STR_ench(0, 30, Color[seat[0]], BACK_C, "������λ:x  0.1"); \
            break;                                                          \
        case 3:                                                             \
            lcd_STR_ench(0, 30, Color[seat[0]], BACK_C, "������λ:x    1"); \
            break;                                                          \
        case 4:                                                             \
            lcd_STR_ench(0, 30, Color[seat[0]], BACK_C, "������λ:x   10"); \
            break;                                                          \
        case 5:                                                             \
            lcd_STR_ench(0, 30, Color[seat[0]], BACK_C, "������λ:x  100"); \
            break;                                                          \
        }                                                                   \
    } while (0)

#define SHOW_1_0                                                                                 \
    do                                                                                           \
    {                                                                                            \
        uint16 temp = 0;                                                                         \
        for (uint8 k = 0; k < 8; k++)                                                            \
        {                                                                                        \
            lcd_STR_ench(0, temp, FONT_C, BACK_C, "���%d delta:%d", k, Induct[k].Nor_MM_value); \
            lcd_STR_ench(140, temp, FONT_C, BACK_C, "Norֵ:%3d", Induct[k].Normalized_value);    \
            temp += 20;                                                                          \
        }                                                                                        \
        lcd_STR_ench(0, 210, FONT_C, BACK_C, "ˮƽ����ܺ�:%4d", param.All_ad_sum);              \
        lcd_STR_ench(0, 230, FONT_C, BACK_C, "����ƫ��:%4d", Ecc_Car->Dir_Error);                \
        lcd_STR_ench(0, 250, FONT_C, BACK_C, "��:%2d", DIR_gain.k0);                             \
        lcd_STR_ench(60, 250, FONT_C, BACK_C, "��:%3d", DIR_gain.k1);                            \
        lcd_STR_ench(120, 250, FONT_C, BACK_C, "��:%2d", DIR_gain.k2);                           \
        lcd_STR_ench(180, 250, FONT_C, BACK_C, "ǰ:%3d", DIR_gain.k3);                           \
        lcd_STR_ench(0, 270, FONT_C, BACK_C, "������־:%2d", param.in_round);                    \
        lcd_STR_ench(100, 270, FONT_C, BACK_C, "�����־:%d", Ecc_Car->outside_flag);            \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����", 0);                                       \
    } while (0)

#define SHOW_0                                                     \
    do                                                             \
    {                                                              \
        lcd_STR_ench(0, 0, FONT_C, BACK_C, "��ҳ�棺");            \
        lcd_STR_ench(85, 30, Color[seat[1]], BACK_C, "�����Ϣ");  \
        lcd_STR_ench(85, 50, Color[seat[2]], BACK_C, "������Ϣ");  \
        lcd_STR_ench(85, 66, Color[seat[3]], BACK_C, "PID������"); \
        lcd_STR_ench(0, 304, FONT_C, BACK_C, "ȷ��");              \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");            \
    } while (0)

#define SHOW_2                                                                                                                             \
    do                                                                                                                                     \
    {                                                                                                                                      \
        if (Scope.key == 1)                                                                                                                \
        {                                                                                                                                  \
            Scope_show();                                                                                                                  \
            lcd_STR_ench(160, 180, FONT_C, BLACK, "״̬:");                                                                                \
            for (uint8 i = 0; i < SCOPE_VAR_NUM; i++)                                                                                      \
            {                                                                                                                              \
                lcd_STR_ench(0, 200 + i * 20, Scope.color[i], BLACK, "%s%4d", Scope.var_name[i], Scope.cursor_data[Scope.cursor_seat][i]); \
                lcd_STR_ench(160, 200 + i * 20, Color[seat[i + 10]], BLACK, "DIV:%5d", Scope.divide[i]);                                   \
                lcd_STR_ench(200 + i * 8, 180, Color[seat[i + 5]], BLACK, "%c", "OI"[Scope.state[i]]);                                     \
            }                                                                                                                              \
            lcd_STR_ench(0, 140, Color[seat[0]], BACK_C, "ʾ��������:%c", "OI"[Scope.key]);                                                \
            lcd_STR_ench(0, 160, Color[seat[1]], BACK_C, "���:%3d", Scope.cursor_seat);                                                   \
            lcd_STR_ench(0, 180, Color[seat[2]], BACK_C, "��ͣ:%c", "OI"[Scope.stop]);                                                     \
            lcd_STR_ench(160, 140, Color[seat[3]], BACK_C, "STEP:%2d", Scope.step);                                                        \
            lcd_STR_ench(160, 160, Color[seat[4]], BACK_C, "AUTO:%c", "OI"[Scope.auto_switch]);                                            \
        }                                                                                                                                  \
        else                                                                                                                               \
        {                                                                                                                                  \
            if (Scope.re_window == 1)                                                                                                      \
            {                                                                                                                              \
                Scope.re_window = 0;                                                                                                       \
                lcd_clear(BACK_C);                                                                                                         \
            }                                                                                                                              \
            lcd_STR_ench(0, 0, FONT_C, BACK_C, "������Ϣ:");                                                                               \
            lcd_STR_ench(0, 20, FONT_C, BACK_C, "����ƫ��:%3d", Ecc_Car->Dir_Error);                                                       \
            lcd_STR_ench(0, 40, FONT_C, BACK_C, "����    :%5d", Ecc_Car->speed);                                                           \
            lcd_STR_ench(0, 60, FONT_C, BACK_C, "�������:%4d", Ecc_Car->S_Duty / 100);                                                    \
            lcd_STR_ench(0, 80, FONT_C, BACK_C, "���ռ�ձ�:%4d", Ecc_Car->M_Duty);                                                        \
            lcd_STR_ench(0, 120, Color[seat[0]], BACK_C, "ʾ��������:%c", "OI"[Scope.key]);                                                \
            lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");                                                                                \
        }                                                                                                                                  \
    } while (0)

#define SHOW_3                                                       \
    do                                                               \
    {                                                                \
        lcd_STR_ench(0, 0, FONT_C, BACK_C, "PIDѡ��:");              \
        lcd_STR_ench(75, 30, Color[seat[4]], BACK_C, "���PID����"); \
        lcd_STR_ench(75, 50, Color[seat[5]], BACK_C, "���PID����"); \
        lcd_STR_ench(75, 70, Color[seat[6]], BACK_C, "��������   "); \
        lcd_STR_ench(0, 304, FONT_C, BACK_C, "ȷ��");                \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");              \
    } while (0) //ȷ�� ������ʾ

//��PID������ʾ
#define SHOW_4                                                                        \
    do                                                                                \
    {                                                                                 \
        lcd_STR_ench(0, 0, FONT_C, BACK_C, "���PID��");                              \
        lcd_STR_ench(0, 20, FONT_C, BACK_C, "Error < P_TH0:");                        \
        lcd_STR_ench(0, 40, Color[seat[0]], BACK_C, "P0:%4d", Servo_pid.P0);          \
        lcd_STR_ench(0, 60, Color[seat[1]], BACK_C, "D0:%4d", Servo_pid.D0);          \
        lcd_STR_ench(0, 80, FONT_C, BACK_C, "P_TH0 < Error < P_TH1:");                \
        lcd_STR_ench(0, 100, Color[seat[2]], BACK_C, "P1:%4d", Servo_pid.P1);         \
        lcd_STR_ench(0, 120, Color[seat[3]], BACK_C, "D1:%4d", Servo_pid.D1);         \
        lcd_STR_ench(0, 140, FONT_C, BACK_C, "P_TH1 < Error < BangBang: ");           \
        lcd_STR_ench(0, 160, Color[seat[4]], BACK_C, "P2:%4d ", Servo_pid.P2);        \
        lcd_STR_ench(0, 180, Color[seat[5]], BACK_C, "D2:%4d ", Servo_pid.D2);        \
        lcd_STR_ench(0, 220, Color[seat[6]], BACK_C, "P_TH0:%3d", Servo_pid.P_Th0);   \
        lcd_STR_ench(100, 220, Color[seat[7]], BACK_C, "P_TH1:%3d", Servo_pid.P_Th1); \
        lcd_STR_ench(0, 240, Color[seat[8]], BACK_C, "D_TH0:%3d", Servo_pid.D_Th0);   \
        lcd_STR_ench(100, 240, Color[seat[9]], BACK_C, "D_TH1:%3d", Servo_pid.D_Th1); \
        lcd_STR_ench(0, 260, Color[seat[10]], BACK_C, "BangBang:%3d", Servo_pid.TH);  \
        lcd_STR_ench(0, 304, FONT_C, BACK_C, "ȷ��");                                 \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");                               \
    } while (0) //ȷ�� ������ʾ

//��PID������ʾ
#define SHOW_5                                                                             \
    do                                                                                     \
    {                                                                                      \
        lcd_STR_ench(0, 0, FONT_C, BACK_C, "���PID:");                                    \
        lcd_STR_ench(0, 30, Color[seat[0]], BACK_C, "���P:%4d", Motor_PID->P);            \
        lcd_STR_ench(0, 50, Color[seat[1]], BACK_C, "���I:%4d", Motor_PID->I);            \
        lcd_STR_ench(0, 70, Color[seat[2]], BACK_C, "���D:%4d", Motor_PID->D);            \
        lcd_STR_ench(0, 90, Color[seat[3]], BACK_C, "Ŀ�공��:%4d", Motor_PID->set_value); \
        lcd_STR_ench(0, 304, FONT_C, BACK_C, "ȷ��");                                      \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");                                    \
    } while (0) //ȷ�� ������ʾ

//��PID������ʾ
#define SHOW_6                                                                          \
    do                                                                                  \
    {                                                                                   \
        lcd_STR_ench(0, 0, FONT_C, BACK_C, "��������:");                                \
        GEAR_SHOW;                                                                      \
        lcd_STR_ench(0, 50, Color[seat[1]], BACK_C, "���ռ�ձ�:%4d", Ecc_Car->M_Duty); \
        lcd_STR_ench(0, 70, Color[seat[2]], BACK_C, "����Rt:%2d", param.Rt_k);          \
        lcd_STR_ench(80, 70, Color[seat[3]], BACK_C, "����Rt:%2d", param.IRt_k);        \
        lcd_STR_ench(160, 70, Color[seat[4]], BACK_C, "����Rt:%2d", param.ORt_k);       \
        lcd_STR_ench(0, 90, Color[seat[5]], BACK_C, "����Rt:%2d", param.URt_k);         \
        lcd_STR_ench(0, 110, Color[seat[6]], BACK_C, "�������:%.3f", param.S_duty);    \
        lcd_STR_ench(0, 130, Color[seat[7]], BACK_C, "��ʾ������:%d", Show.key);        \
        lcd_STR_ench(0, 304, FONT_C, BACK_C, "ȷ��");                                   \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");                                 \
    } while (0) //ȷ�� ������ʾ

#define Show_Handler                                               \
    do                                                             \
    {                                                              \
        key_num_get();                                             \
        if (Show.count > 2 && Show.key == 0 && Show.PIT_over == 1) \
        {                                                          \
            All_show();                                            \
            Show.flag = 0;                                         \
            Show.count = 0;                                        \
        }                                                          \
        else if (Show.flag == 0 && Show.key == 1)                  \
        {                                                          \
            Show.flag = 1;                                         \
            lcd_clear(BLACK);                                      \
        }                                                          \
        else                                                       \
            Show.PIT_over = 0;                                     \
    } while (0) //��ʾˢ��Ƶ�ʼ�������Ϣ�������

void All_show();
void Tree_init();
void gear_select(char gear);
void key_num_get();
void key_process(uint8 i, uint8 key);
void Boot_action();
void my_flash_init();
void my_data_save();
void my_data_load();
#endif