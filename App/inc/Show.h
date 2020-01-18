#ifndef __SHOW_H__
#define __SHOW_H__
/*��320x��240*/

#define BACK_C BLACK
#define FONT_C YELLOW

typedef struct
{
    uint8 freq;							//ˢ��Ƶ��
	uint8 state:2;						//��ʾ״̬
    uint8 PIT_over; 					//��ʱ���жϽ�����־
	
    char   strbuff[30]; 					//�ַ�������	
	uint8  seat[30];                 	//��ɫλ�ñ�
	uint16 color[3];					//��ɫ��

	struct
	{
		char  chose;
		float gear;
	}Gear;	
} SHOW;
extern SHOW Show;

typedef struct
{
	struct
	{
		uint8 ok:1;		//�޸�����
		uint8 wr:1;		//0��ʾ�ɶ���1��ʾ��д�����ڱ����޸ģ�
		uint8 state:1;  //�Ӵ���״̬1���ڣ�0����
		uint8 only:1;	////���ײ����ֹ������Ѱ,������ʾ��
	}Flag;
	    
	struct
	{
		uint8 up;	//�Ӵ����ʲ㼶����
		uint8 down;//�Ӵ����ʲ㼶����
		char  now;//��ǰ�㼶
	}Grade;
	
	uint8 show_mode; //��ʾ����
    char  temp;       //�����ı���
    char  parents;
    char  child;
} Node, *pNode;

extern Node Tree[7];		

typedef struct
{
    struct
    {
        uint16 min[INDUCT_NUM];   //��й�һ����С�ο�ֵ
        uint16 range[INDUCT_NUM]; //��й�һ����ֵ
    } Nor;

    struct
    {
        int16 P;
        int16 I;
        int16 D;
        int32 duty;
    } Motor;
    struct
    {
        int16 P;
        int16 I;
        int16 D;
    } Servo, S_Kp, S_Kd;

    uint16 threshold;
    int32 password;
} Data, *pData;

#define PASSWORD 0x01912		//���ڼ���flash�е������Ƿ���Ч

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


static inline void SEAT(uint8 i)
{
	for (uint8 k = Tree[i].Grade.down; k < (Tree[i].Grade.up + 1); k++)            
	{                                                                      
		Show.seat[k] = (k == Tree[i].Grade.now )? (1==Tree[i].Flag.ok ? 2 : 1) : 0; 
	}    	
}

static inline void GEAR_SHOW(char gear_chose)
{
	switch (gear_chose)                                                
	{                                                                  
	case 0:                                                            
	    lcd_STR_ench(0, 30, Show.color[Show.seat[0]], BACK_C, "������λ:x0.001");
	    break;                                                         
	case 1:                                                            
	    lcd_STR_ench(0, 30, Show.color[Show.seat[0]], BACK_C, "������λ:x 0.01");
	    break;                                                         
	case 2:                                                            
	    lcd_STR_ench(0, 30, Show.color[Show.seat[0]], BACK_C, "������λ:x  0.1");
	    break;                                                         
	case 3:                                                            
	    lcd_STR_ench(0, 30, Show.color[Show.seat[0]], BACK_C, "������λ:x    1");
	    break;                                                         
	case 4:                                                            
	    lcd_STR_ench(0, 30, Show.color[Show.seat[0]], BACK_C, "������λ:x   10");
	    break;                                                         
	case 5:                                                            
	    lcd_STR_ench(0, 30, Show.color[Show.seat[0]], BACK_C, "������λ:x  100");
	    break;                                                         
	case 6:                                                            
	    lcd_STR_ench(0, 30, Show.color[Show.seat[0]], BACK_C, "������λ:x 1000");
	    break;                                                         
	}                                                                   	
}


#define SHOW_1_0                                                                                                               \
    do                                                                                                                         \
    {                                                                                                                          \
        uint16 temp = 0;                                                                                                       \
        for (uint8 k = 0; k < 6; k++)                                                                                          \
        {                                                                                                                      \
            lcd_STR_ench(0, temp, FONT_C, BACK_C, "���%d delta:%d", k, Induct[k].Nor.range);                                  \
            lcd_STR_ench(140, temp, FONT_C, BACK_C, "Norֵ:%3d", Induct[k].Nor.now);                                           \
            temp += 20;                                                                                                        \
        }                                                                                                                      \
        lcd_STR_ench(0, 120, FONT_C, BACK_C, "ˮƽ:%4d ȫ��:%4d", ADD0, ADD3);                                                 \
        lcd_STR_ench(0, 140, FONT_C, BACK_C, "DW:%4d DZ:%4d DN:%4d", SUB0, SUB1, SUB2);                                        \
        lcd_STR_ench(0, 160, FONT_C, BACK_C, "EW:%4d EZ:%4d EN:%4d", WZN.W, WZN.Z, WZN.N);                                     \
        lcd_STR_ench(0, 180, FONT_C, BACK_C, "KW:%4d KZ:%4d KN:%4d", DIR_gain.k0, DIR_gain.k1, DIR_gain.k2);                   \
        lcd_STR_ench(0, 200, FONT_C, BACK_C, "����ƫ��:%4d", Ecc_Car->Error.now);                                              \
        lcd_STR_ench(0, 220, FONT_C, BACK_C, "��׼��ֵ:%3d  ������ֵ:%3d", Ecc_Car->Info.threshold, Ecc_Car->Round.threshold); \
        lcd_STR_ench(0, 240, FONT_C, BACK_C, "ˮƽ����ܺ�:%4d", Ecc_Car->Round.ad_sum);                                       \
        lcd_STR_ench(0, 260, FONT_C, BACK_C, "����:%3d  ���ϱ�־:%d", Ecc_Car->Distance.length, Ecc_Car->Flag.obstacle);                \
        lcd_STR_ench(0, 280, FONT_C, BACK_C, "������־:%2d  �����־:%d", Ecc_Car->Round.state, Ecc_Car->Flag.outside);        \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����", 0);                                                                     \
    } while (0)
		
static inline void SHOW_0(void)
{
	lcd_STR_ench(0, 0, FONT_C, BACK_C, "��ҳ�棺");            
	lcd_STR_ench(85, 30, Show.color[Show.seat[1]], BACK_C, "�����Ϣ");  
	lcd_STR_ench(85, 50, Show.color[Show.seat[2]], BACK_C, "������Ϣ");  
	lcd_STR_ench(85, 66, Show.color[Show.seat[3]], BACK_C, "PID������"); 
	lcd_STR_ench(0, 304, FONT_C, BACK_C, "ȷ��");              
	lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");            
}
	
		
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
                lcd_STR_ench(160, 200 + i * 20, Show.color[Show.seat[i + 10]], BLACK, "DIV:%5d", Scope.divide[i]);                                   \
                lcd_STR_ench(200 + i * 8, 180, Show.color[Show.seat[i + 5]], BLACK, "%c", "OI"[Scope.state[i]]);                                     \
            }                                                                                                                              \
            lcd_STR_ench(0, 140, Show.color[Show.seat[0]], BACK_C, "ʾ��������:%c", "OI"[Scope.key]);                                                \
            lcd_STR_ench(0, 160, Show.color[Show.seat[1]], BACK_C, "���:%3d", Scope.cursor_seat);                                                   \
            lcd_STR_ench(0, 180, Show.color[Show.seat[2]], BACK_C, "��ͣ:%c", "OI"[Scope.stop]);                                                     \
            lcd_STR_ench(160, 140, Show.color[Show.seat[3]], BACK_C, "STEP:%2d", Scope.step);                                                        \
            lcd_STR_ench(160, 160, Show.color[Show.seat[4]], BACK_C, "AUTO:%c", "OI"[Scope.auto_switch]);                                            \
        }                                                                                                                                  \
        else                                                                                                                               \
        {                                                                                                                                  \
            if (Scope.re_window == 1)                                                                                                      \
            {                                                                                                                              \
                Scope.re_window = 0;                                                                                                       \
                lcd_clear(BACK_C);                                                                                                         \
            }                                                                                                                              \
            lcd_STR_ench(0, 0, FONT_C, BACK_C, "������Ϣ:");                                                                               \
            lcd_STR_ench(0, 20, FONT_C, BACK_C, "����ƫ��:%4d", Ecc_Car->Error.now);                                                       \
            lcd_STR_ench(0, 40, FONT_C, BACK_C, "����    :%5d", Ecc_Car->Speed.now);                                                       \
            lcd_STR_ench(0, 60, FONT_C, BACK_C, "�������:%4d", Ecc_Car->Duty.servo / 100);                                                \
            lcd_STR_ench(0, 80, FONT_C, BACK_C, "���ռ�ձ�:%4d", Ecc_Car->Duty.motor);                                                    \
            lcd_STR_ench(0, 120, Show.color[Show.seat[0]], BACK_C, "ʾ��������:%c", "OI"[Scope.key]);                                                \
            lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");                                                                                \
        }                                                                                                                                  \
    } while (0)

#define SHOW_3                                                       \
    do                                                               \
    {                                                                \
        lcd_STR_ench(0, 0, FONT_C, BACK_C, "PIDѡ��:");              \
        lcd_STR_ench(75, 30, Show.color[Show.seat[4]], BACK_C, "���PID����"); \
        lcd_STR_ench(75, 50, Show.color[Show.seat[5]], BACK_C, "���PID����"); \
        lcd_STR_ench(75, 70, Show.color[Show.seat[6]], BACK_C, "��������   "); \
        lcd_STR_ench(0, 304, FONT_C, BACK_C, "ȷ��");                \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");              \
    } while (0) //ȷ�� ������ʾ

//��PID������ʾ
#define SHOW_4                                                                                \
    do                                                                                        \
    {                                                                                         \
        lcd_STR_ench(0, 0, FONT_C, BACK_C, "���PID:");                                       \
        lcd_STR_ench(120, 0, FONT_C, BACK_C, "�������:%4d", Ecc_Car->Duty.servo / 100);      \
        lcd_STR_ench(0, 20, Show.color[Show.seat[0]], BACK_C, "ͣ����־:%c", "OI"[Ecc_Car->Flag.stop]); \
        lcd_STR_ench(0, 40, Show.color[Show.seat[1]], BACK_C, "S_P:%3d", Servo_PID->P);                 \
        lcd_STR_ench(0, 60, Show.color[Show.seat[2]], BACK_C, "Kp_P:%3d", Adapt_S_Kp.P);                \
        lcd_STR_ench(80, 60, Show.color[Show.seat[3]], BACK_C, "Kp_I:%3d", Adapt_S_Kp.I);               \
        lcd_STR_ench(160, 60, Show.color[Show.seat[4]], BACK_C, "Kp_D:%3d", Adapt_S_Kp.D);              \
        lcd_STR_ench(0, 80, Show.color[Show.seat[5]], BACK_C, "S_D:%3d", Servo_PID->D);                 \
        lcd_STR_ench(0, 100, Show.color[Show.seat[6]], BACK_C, "Kd_P:%3d", Adapt_S_Kd.P);               \
        lcd_STR_ench(80, 100, Show.color[Show.seat[7]], BACK_C, "Kd_I:%3d", Adapt_S_Kd.I);              \
        lcd_STR_ench(160, 100, Show.color[Show.seat[8]], BACK_C, "Kd_D:%3d", Adapt_S_Kd.D);             \
        lcd_STR_ench(0, 304, FONT_C, BACK_C, "ȷ��");                                         \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");                                       \
    } while (0) //ȷ�� ������ʾ

//��PID������ʾ
#define SHOW_5                                                                              \
    do                                                                                      \
    {                                                                                       \
        lcd_STR_ench(0, 0, FONT_C, BACK_C, "���PID:");                                     \
        lcd_STR_ench(0, 20, Show.color[Show.seat[0]], BACK_C, "�������:%4d", Ecc_Car->Duty.motor);   \
        lcd_STR_ench(0, 40, Show.color[Show.seat[1]], BACK_C, "���P:%4d", Motor_PID->P);             \
        lcd_STR_ench(0, 60, Show.color[Show.seat[2]], BACK_C, "���I:%4d", Motor_PID->I);             \
        lcd_STR_ench(0, 80, Show.color[Show.seat[3]], BACK_C, "���D:%4d", Motor_PID->D);             \
        lcd_STR_ench(0, 100, Show.color[Show.seat[4]], BACK_C, "Ŀ�공��:%4d", Motor_PID->set_value); \
        lcd_STR_ench(0, 120, Show.color[Show.seat[5]], BACK_C, "Kp_P:%3d", Adapt_M_Kp.P);             \
        lcd_STR_ench(0, 140, Show.color[Show.seat[6]], BACK_C, "Kp_I:%3d", Adapt_M_Kp.I);             \
        lcd_STR_ench(0, 160, Show.color[Show.seat[7]], BACK_C, "Kp_D:%3d", Adapt_M_Kp.D);             \
        lcd_STR_ench(120, 100, FONT_C, BACK_C, "ʵ�ʳ���:%3d", Ecc_Car->Speed.now);         \
        lcd_STR_ench(0, 304, FONT_C, BACK_C, "ȷ��");                                       \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");                                     \
    } while (0) //ȷ�� ������ʾ

//��PID������ʾ
#define SHOW_6                                                                                    \
    do                                                                                            \
    {                                                                                             \
        lcd_STR_ench(0, 0, FONT_C, BACK_C, "��������:");                                          \
        GEAR_SHOW(Show.Gear.chose);                                                                                \
        lcd_STR_ench(0, 50, Show.color[Show.seat[1]], BACK_C, "��������:%.3f", Ecc_Car->Duty.debug);        \
        lcd_STR_ench(0, 70, Show.color[Show.seat[2]], BACK_C, "��ֵУ׼:%c", "OI"[Ecc_Car->Flag.calibrat]); \
        lcd_STR_ench(0, 90, FONT_C, BACK_C, "��������");                                          \
        lcd_STR_ench(0, 130, GBLUE, BACK_C, "W");                                                 \
        lcd_STR_ench(0, 150, GBLUE, BACK_C, "Z");                                                 \
        lcd_STR_ench(0, 170, GBLUE, BACK_C, "N");                                                 \
        lcd_STR_ench(20, 110, GBLUE, BACK_C, "0");                                                \
        lcd_STR_ench(60, 110, GBLUE, BACK_C, "1");                                                \
        lcd_STR_ench(100, 110, GBLUE, BACK_C, "2");                                               \
        lcd_STR_ench(140, 110, GBLUE, BACK_C, "3");                                               \
        lcd_STR_ench(180, 110, GBLUE, BACK_C, "4");                                               \
        lcd_STR_ench(220, 110, GBLUE, BACK_C, "5");                                               \
        lcd_STR_ench(20, 130, Show.color[Show.seat[3]], BACK_C, "%2d", Ecc_Car->Round.Debug.W[0]);          \
        lcd_STR_ench(20, 150, Show.color[Show.seat[4]], BACK_C, "%2d", Ecc_Car->Round.Debug.Z[0]);          \
        lcd_STR_ench(20, 170, Show.color[Show.seat[5]], BACK_C, "%2d", Ecc_Car->Round.Debug.N[0]);          \
        lcd_STR_ench(40, 130, Show.color[Show.seat[6]], BACK_C, "%3d", Ecc_Car->Round.Debug.W[1]);          \
        lcd_STR_ench(40, 150, Show.color[Show.seat[7]], BACK_C, "%3d", Ecc_Car->Round.Debug.Z[1]);          \
        lcd_STR_ench(40, 170, Show.color[Show.seat[8]], BACK_C, "%3d", Ecc_Car->Round.Debug.N[1]);          \
        lcd_STR_ench(80, 130, Show.color[Show.seat[9]], BACK_C, "%3d", Ecc_Car->Round.Debug.W[2]);          \
        lcd_STR_ench(80, 150, Show.color[Show.seat[10]], BACK_C, "%3d", Ecc_Car->Round.Debug.Z[2]);         \
        lcd_STR_ench(80, 170, Show.color[Show.seat[11]], BACK_C, "%3d", Ecc_Car->Round.Debug.N[2]);         \
        lcd_STR_ench(120, 130, Show.color[Show.seat[12]], BACK_C, "%3d", Ecc_Car->Round.Debug.W[3]);        \
        lcd_STR_ench(120, 150, Show.color[Show.seat[13]], BACK_C, "%3d", Ecc_Car->Round.Debug.Z[3]);        \
        lcd_STR_ench(120, 170, Show.color[Show.seat[14]], BACK_C, "%3d", Ecc_Car->Round.Debug.N[3]);        \
        lcd_STR_ench(0, 190,  Show.color[Show.seat[15]], BACK_C, "��:%5d", Ecc_Car->Duty.obstacle[0]);       \
        lcd_STR_ench(80, 190, Show.color[Show.seat[16]], BACK_C, "��:%5d", Ecc_Car->Duty.obstacle[1]);      \
        lcd_STR_ench(160,190, Show.color[Show.seat[17]], BACK_C, "��:%5d", Ecc_Car->Duty.obstacle[2]);     \
        lcd_STR_ench(0, 210, Show.color[Show.seat[18]], BACK_C, "�����ٶ�:%3d", Ecc_Car->Speed.obstacle);   \
        lcd_STR_ench(0, 230, Show.color[Show.seat[19]], BACK_C, "�����ٶ�:%3d", Ecc_Car->Speed.round);      \
        lcd_STR_ench(0, 250, Show.color[Show.seat[20]], BACK_C, "�����ٶ�:%3d", Ecc_Car->Speed.outside);    \
        lcd_STR_ench(0, 270, Show.color[Show.seat[21]], BACK_C, "��׼�ٶ�:%3d", Ecc_Car->Speed.standard);   \
        lcd_STR_ench(0, 304, FONT_C, BACK_C, "ȷ��");                                             \
        lcd_STR_ench(208, 304, FONT_C, BACK_C, "����");                                           \
    } while (0) //ȷ�� ������ʾ

#define Show_Handler                                               \
    do                                                             \
    {                                                              \
        key_num_get();                                             \
        if (Show.freq > 2 && 0==Show.state && 1==Show.PIT_over ) \
        {                                                          \
            All_show();                                            \
            Show.freq = 0;                                        \
        }                                                          \
        else if (1==Show.state )                  \
        {                                                          \
            Show.state = 2;                                         \
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
/*Flash�洢�����*/
void my_flash_init();
void my_data_save();
void my_data_load();
#endif