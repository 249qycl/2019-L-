#include "common.h"
#include "include.h"

SHOW         Show;
KEY_MSG_t    keymsg;                    //������Ϣ����
flash_data_t data;                      //flash��ʼ����Ϣ
Data         md;                        //flash����

uint8 seat[20];                 //��ɫλ�ñ�
uint16 Color[3]={FONT_C,GREEN,RED}; //��ɫ��

uint8 key_num;

float Gear= 1.0;                 //��λ
char  gear_chose=3;             //��λѡ��

Node Tree[7];

#define EVENT1          {Servo_pid.P0+=Tree[i].temp*Gear;}
#define EVENT2          {Servo_pid.D0+=Tree[i].temp*Gear;}
#define EVENT3          {Servo_pid.P1+=Tree[i].temp*Gear;}
#define EVENT4          {Servo_pid.D1+=Tree[i].temp*Gear;}
#define EVENT5          {Servo_pid.P2+=Tree[i].temp*Gear;}
#define EVENT6          {Servo_pid.D2+=Tree[i].temp*Gear;}
#define EVENT7          {Servo_pid.P_Th0+=Tree[i].temp*Gear;}
#define EVENT8          {Servo_pid.P_Th1+=Tree[i].temp*Gear;}
#define EVENT9          {Servo_pid.D_Th0+=Tree[i].temp*Gear;}
#define EVENT10         {Servo_pid.D_Th1+=Tree[i].temp*Gear;}
#define EVENT11         {Servo_pid.TH   +=Tree[i].temp*Gear;}
#define EVENT12         {}
                         
#define EVENT13         {Motor_PID->P+=Tree[i].temp*Gear;     }
#define EVENT14         {Motor_PID->I+=Tree[i].temp*Gear;     }
#define EVENT15         {Motor_PID->D+=Tree[i].temp*Gear;     }
#define EVENT16         {Motor_PID->set_value+=Tree[i].temp*Gear;}
#define EVENT17         {}
#define EVENT18         {}
#define EVENT19         {}
#define EVENT20         {}
#define EVENT21         {}
#define EVENT22         {}
#define EVENT23         {}
#define EVENT24         {}

#define EVENT25         {gear_chose+=Tree[i].temp;gear_chose=RANGE(gear_chose,5,0);gear_select(gear_chose);}
#define EVENT26         {Ecc_Car->M_Duty+=Tree[i].temp*Gear;}
#define EVENT27         {param.Rt_k +=Tree[i].temp;}
#define EVENT28         {param.IRt_k+=Tree[i].temp;}
#define EVENT29         {param.ORt_k+=Tree[i].temp;}
#define EVENT30         {param.URt_k+=Tree[i].temp;}
#define EVENT31         {param.S_duty+=Tree[i].temp*Gear;}
#define EVENT32         {Show.key+=Tree[i].temp;}
#define EVENT33         {}
#define EVENT34         {}
#define EVENT35         {}
#define EVENT36         {}

#define SCOPE_DIV(x)    do{Scope.divide[x]=Scope.gear[x]>5?(Scope.gear[x]>10?(Scope.gear[x]>15?(Scope.gear[x]>20?60000:((Scope.gear[x]-15)*10000)):((Scope.gear[x]-10)*2000)):((Scope.gear[x]-5)*200)):(Scope.gear[x]*20);}while(0) 


#define CAR_EVENT0      {Scope.key=Tree[i].temp!=0?(!Scope.key):Scope.key;}
#define CAR_EVENT1      {Scope.cursor_seat+=Tree[i].temp*Scope.step;Scope.cursor_seat=RANGE(Scope.cursor_seat,238,1);}
#define CAR_EVENT2      {Scope.stop=Tree[i].temp!=0?(!Scope.stop):Scope.stop;}
#define CAR_EVENT3      {Scope.step+=Tree[i].temp;}
#define CAR_EVENT4      {Scope.auto_switch=Tree[i].temp!=0?(!Scope.auto_switch):Scope.auto_switch;}
#define CAR_EVENT5      {Scope.state[0]=Tree[i].temp!=0?(!Scope.state[0]):Scope.state[0];}
#define CAR_EVENT6      {Scope.state[1]=Tree[i].temp!=0?(!Scope.state[1]):Scope.state[1];}
#define CAR_EVENT7      {Scope.state[2]=Tree[i].temp!=0?(!Scope.state[2]):Scope.state[2];}
#define CAR_EVENT8      {Scope.state[3]=Tree[i].temp!=0?(!Scope.state[3]):Scope.state[3];}
#define CAR_EVENT9      {Scope.state[4]=Tree[i].temp!=0?(!Scope.state[4]):Scope.state[4];}
#define CAR_EVENT10     {Scope.gear[0]+=Tree[i].temp;Scope.gear[0]=RANGE(Scope.gear[0],20,1);SCOPE_DIV(0);}
#define CAR_EVENT11     {Scope.gear[1]+=Tree[i].temp;Scope.gear[1]=RANGE(Scope.gear[1],20,1);SCOPE_DIV(1);}
#define CAR_EVENT12     {Scope.gear[2]+=Tree[i].temp;Scope.gear[2]=RANGE(Scope.gear[2],20,1);SCOPE_DIV(2);}
#define CAR_EVENT13     {Scope.gear[3]+=Tree[i].temp;Scope.gear[3]=RANGE(Scope.gear[3],20,1);SCOPE_DIV(3);}
#define CAR_EVENT14     {Scope.gear[4]+=Tree[i].temp;Scope.gear[4]=RANGE(Scope.gear[4],20,1);SCOPE_DIV(4);}
#define CAR_EVENT15     {}


/*********************************************************
*
*       �����ڸ�����ϵ�����Ĺ�ϵ�����г�ʼ��
*
*********************************************************/
void Tree_init()
{
    /*���Ӵ�������ϵ����*/
    Tree[0].parents = -1;
    Tree[1].parents = 0;//induct
    Tree[2].parents = 0;//other
    Tree[3].parents = 0;//pid
    Tree[4].parents = 3;//1
    Tree[5].parents = 3;//2
    Tree[6].parents = 3;//3

    /*�Ӵ�ɸѡ���޷�����*/
    Tree[0].gra_h = 3;
    Tree[0].gra_l = 1;
    Tree[0].grade = 1;

    Tree[2].gra_h = 15;
    Tree[2].gra_l = 0;
    Tree[2].grade = 0;
    
    Tree[3].gra_h = 6;
    Tree[3].gra_l = 4;
    Tree[3].grade = 4;

    Tree[4].gra_h = 11;
    Tree[4].gra_l = 0;
    Tree[4].grade = 0;

    Tree[5].gra_h = 11;
    Tree[5].gra_l = 0;
    Tree[5].grade = 0;

    Tree[6].gra_h = 11;
    Tree[6].gra_l = 0;
    Tree[6].grade = 0;

    /*���ױ�������*/
    Tree[0].only_show = 0;
    Tree[1].only_show = 1;
    Tree[2].only_show = 0;
    Tree[3].only_show = 0;
    Tree[4].only_show = 1;
    Tree[5].only_show = 1;
    Tree[6].only_show = 1;
    /*��ʾ������*/
    Tree[0].show_mode = 0;
    Tree[1].show_mode = 1;//�����Ϣչʾ
    Tree[2].show_mode = 2;//������Ϣչʾ
    Tree[3].show_mode = 3;//PID������Ϣչʾ
    Tree[4].show_mode = 4;//��һ��PID
    Tree[5].show_mode = 5;//�ڶ���PID
    Tree[6].show_mode = 6;//������PID

    /*�Ƿ���б����޸�Ȩ��*/
    Tree[0].wr_flag = 0;//ֻ�ɶ�
    Tree[1].wr_flag = 0;//ֻ��,������ʾ�ã�����Ӧkey==3��
    Tree[2].wr_flag = 1;//��0��
    Tree[3].wr_flag = 0;//ֻ����������ʾ�ã�����Ӧkey==3��
    Tree[4].wr_flag = 1;
    Tree[5].wr_flag = 1;
    Tree[6].wr_flag = 1;

    for (uint8 i = 0; i < 7; i++)
    {
        Tree[i].temp  = 0;
        Tree[i].child =Tree[i].gra_l;
        Tree[i].ok_flag = 0;
        Tree[0].c_state = 0;//�Ӵ�����״̬
    }
}
/**************************************************
*
*       ���ڷ��ȵ�ѡ��
*
**************************************************/
void gear_select(char gear)
{
    switch(gear)
    {
    case 0:Gear=0.001;break;
    case 1:Gear=0.01 ;break;
    case 2:Gear=0.1  ;break;
    case 3:Gear=1    ;break;
    case 4:Gear=10   ;break;
    case 5:Gear=100  ;break;
    }
}
/***************************************************
*
*       ������������������ײ���Ӵ�
*
***************************************************/
uint8 find()
{
        uint8 i = 0;
        while (Tree[i].c_state == 1)
        {
                if (Tree[i].only_show == 1)
                        break;
                i = Tree[i].child;
        }
        return i;
}
/*********************************************************
*
*               ����Ϣ�����ж�ȡ������Ϣ��key_num
*
*********************************************************/
void key_num_get()
{
    if(get_key_msg(&keymsg) == 1)
    {
        if(keymsg.status==1)//�̰�
        {
            switch(keymsg.key)
            {
            case 0:key_num=1;break;
            case 1:key_num=2;break;
            case 2:key_num=3;break;
            case 3:key_num=4;break;
            }
            keymsg.status=KEY_DOWN;
        }
    }
    if(key_num!=0)
        Show.key=0;
}
/*************************************************
*
*       ��������������Ҫ���ڵı���ֵ���и���
*
*************************************************/
void key_process(uint8 i,uint8 key)
{
    uint8 event;
    key_num=0;
    switch (key)
    {
        case 1:
                if (Tree[i].wr_flag == 1 && Tree[i].ok_flag == 1)
                    Tree[i].temp++;
                else
                    Tree[i].grade--;
                break;
                
        case 2:
                if (Tree[i].wr_flag == 1 && Tree[i].ok_flag == 1)
                    Tree[i].temp--;
                else
                    Tree[i].grade++;
                break;
                
        case 3:
                if (Tree[i].wr_flag == 1&& Tree[i].ok_flag ==0)
                    Tree[i].ok_flag = 1;
                else
                {
                    if(i==2&&Scope.key==1)
                        break;
                    Tree[i].c_state = 1;
                    Tree[i].child   = Tree[i].grade;//ͨ��grade������Ӧ�ӿռ�
                }
                break;
                
        case 4: 
                if(Tree[i].ok_flag==1)
                   Tree[i].ok_flag=0;
                else
                {
                    if(i==2&&Scope.key==1)
                        break;
                    Tree[Tree[i].parents].c_state = 0;
                    Tree[i].grade   = Tree[i].gra_l;
                    Tree[i].ok_flag = 0;
                }
                if(i==0)
                {
                    my_data_save();//д��flash����
                    DELAY_MS(1);
                    lcd_clear(BLACK);
                    lcd_STR_ench(85,150,YELLOW,BLACK,"д��ɹ���");
                }
                break;
    }
    /*�޷�*/
    Tree[i].grade = RANGE(Tree[i].grade,Tree[i].gra_h,Tree[i].gra_l);
    if (i < 7 && i>3)//��дҳ������ݸ���
    {
        event = (i - 4) * 12 + Tree[i].grade+1;
        switch (event)
        {
        case 1 :EVENT1; break;
        case 2 :EVENT2; break;
        case 3 :EVENT3; break;
        case 4 :EVENT4; break;
        case 5 :EVENT5; break;
        case 6 :EVENT6; break;
        case 7 :EVENT7; break;
        case 8 :EVENT8; break;
        case 9 :EVENT9; break;
        case 10:EVENT10;break;
        case 11:EVENT11;break;
        case 12:EVENT12;break;
        case 13:EVENT13;break;
        case 14:EVENT14;break;
        case 15:EVENT15;break;
        case 16:EVENT16;break;
        case 17:EVENT17;break;
        case 18:EVENT18;break;
        case 19:EVENT19;break;
        case 20:EVENT20;break;
        case 21:EVENT21;break;
        case 22:EVENT22;break;
        case 23:EVENT23;break;
        case 24:EVENT24;break;
        case 25:EVENT25;break;
        case 26:EVENT26;break;
        case 27:EVENT27;break;
        case 28:EVENT28;break;
        case 29:EVENT29;break;
        case 30:EVENT30;break;
        case 31:EVENT31;break;
        case 32:EVENT32;break;
        case 33:EVENT33;break;
        case 34:EVENT34;break;
        case 35:EVENT35;break;
        case 36:EVENT36;break;
        }         
    }
    else if(i==2)
    {
        switch(Tree[i].grade)
        {
        case 0 :CAR_EVENT0 ;break;
        case 1 :CAR_EVENT1 ;break;
        case 2 :CAR_EVENT2 ;break;
        case 3 :CAR_EVENT3 ;break;
        case 4 :CAR_EVENT4 ;break;
        case 5 :CAR_EVENT5 ;break;
        case 6 :CAR_EVENT6 ;break;
        case 7 :CAR_EVENT7 ;break;
        case 8 :CAR_EVENT8 ;break;
        case 9 :CAR_EVENT9 ;break;
        case 10:CAR_EVENT10;break;
        case 11:CAR_EVENT11;break;
        case 12:CAR_EVENT12;break;
        case 13:CAR_EVENT13;break;
        case 14:CAR_EVENT14;break;
        case 15:CAR_EVENT15;break;
        }
    }
    /*�м��������*/
    for (uint8 j = 0; j < 7; j++)
        Tree[j].temp = 0;
}

/*****************************************************
*
*       �˻�����������ʾ����      
*       
*****************************************************/
void key_show(uint8 i)
{    
    static uint8 last_mode=0;

    switch(Tree[i].show_mode)
    {
        case 0: CLEAR;  //������ʾmain page  
                SEAT;                         
                SHOW_0;
                break;
                
        case 1: CLEAR;
                SHOW_1_0;//��ʾ�����Ϣ
                break;
                
        case 2: CLEAR;
                SEAT;
                SHOW_2;//��ʾ������Ϣ
                break;
                
        case 3: CLEAR;
                SEAT;
                SHOW_3;
                break;
                
        case 4: CLEAR;
                SEAT;
                SHOW_4;
                break;
                
        case 5: CLEAR;
                SEAT;
                SHOW_5;
                break;
                
        case 6: CLEAR;                        
                SEAT;                     
                SHOW_6;
                break;
    }
}
/*************************************************
*
*       �˻�����ִ�в��ֺ�����װ
*
*************************************************/
void All_show()
{
    uint8 temp=find();
    key_process(temp,key_num);
    key_show(temp);
}
/***************************************************
*
*       �������漰��������
*
***************************************************/
void Boot_action()
{
    uint8 temp0,temp1=0,temp2=0,temp3=0,boot=0;
    uint16 times=0;
    
    gpio_init(PTD11, GPI, 0);
    port_init_NoALT(PTD11, PULLUP);//���ָ��ò��䣬�����ı�����ѡ��
    gpio_init(PTD12, GPI, 0);
    port_init_NoALT(PTD12, PULLUP);//���ָ��ò��䣬�����ı�����ѡ��
    lcd_STR_ench(0,0,YELLOW,BLACK,"�Ƿ������ʷ������");
    lcd_STR_ench(0,40,YELLOW,BLACK,"����ѡ�񣬽���3����Զ�ȷ��");
    lcd_STR_ench(0,304,YELLOW,BLACK,"ȷ��");
    lcd_STR_ench(208,304,YELLOW,BLACK,"����");
    while(1)
    {
        temp0 = gpio_get (PTD11);
        temp1 = temp0==0?(temp1+1):0;
        temp0 = gpio_get (PTD12);
        temp2 = temp0==0?(temp2+1):0;
        if(temp1>10)
        {
            boot=1;
            break;
        }      
        if(temp2>10)
        {
            boot=0;
            break;
        }       
        DELAY_MS(5);
        
        times++;
        if(times>0&&times<=200)
            lcd_STR_ench(70,130,YELLOW,BLACK,"      3��       ");
        else if(times>200&&times<=400)
            lcd_STR_ench(70,130,YELLOW,BLACK,"      2��       ");
        else if(times>400&&times<=600)
            lcd_STR_ench(70,130,YELLOW,BLACK,"      1��       ");
        else if(times>600)//3����Զ�����
        {
            boot=1;
            led(LED0,LED_ON);
            break;
        }
    }
    
    lcd_clear(BLACK);
    
    if(boot==1)
    {
        lcd_STR_ench(50,50,YELLOW,BLACK,"׼�����ز���������");
        my_data_load();//����flash����
        DELAY_MS(50);
        lcd_STR_ench(50,50,YELLOW,BLACK,"���������С�����");
        DELAY_MS(500);
        lcd_STR_ench(50,50,YELLOW,BLACK,"�������سɹ���   ");
        temp3=1;
        DELAY_MS(1500);
    }
    if(temp3==0)
    {
        lcd_STR_ench(50,50,YELLOW,BLACK,"������ʼ����ɨ��");
        DELAY_MS(1000);
        lcd_STR_ench(70,130,YELLOW,BLACK,"      3��       ");
        DELAY_MS(1000);
        lcd_STR_ench(70,130,YELLOW,BLACK,"      2��       ");
        DELAY_MS(1000);
        lcd_STR_ench(70,130,YELLOW,BLACK,"      1��       ");
        DELAY_MS(1000);
    }
    lcd_clear(BLACK);    
}
/***********************************************************
*
*       �Լ���ʹ�õ�flash�ռ���г�ʼ��
*
************************************************************/
void my_flash_init()
{
    
    data.sectornum_start  = FLASH_SECTOR_NUM - 2;     //��ʼ����      ������3����������Ϊ�������
    data.sectornum_end    = FLASH_SECTOR_NUM - 1;     //��������
    data.data_addr        = &md;                          //���ݵĵ�ַ
    data.data_size        = sizeof(md);                  //���ݵĴ�С
    
    flash_data_init(&data);//��ʼ��  
}
/*****************************************************
*
*       ��Ҫ���������д��flash
*
*****************************************************/
void my_data_save()
{
    md.sP0=Servo_pid.P0;
    md.sD0=Servo_pid.D0;    
    md.sP1=Servo_pid.P1;
    md.sD1=Servo_pid.D1;
    md.sP2=Servo_pid.P2;
    md.sD2=Servo_pid.D2;
    md.sP3=Servo_pid.P3;
    md.sD3=Servo_pid.D3;
    
    md.mP=Motor_PID->P;
    md.mI=Motor_PID->I;
    md.mD=Motor_PID->D;
    
    md.cM_Duty=Ecc_Car->M_Duty;
    
    for(uint8 i=0;i<INDUCT_NUM;i++)
    {
        md.nor_min[i]=Induct[i].Nor_min;
        md.nor_MM_value[i]=Induct[i].Nor_MM_value;
    }
    flash_data_save(&data);//д������
}
/****************************************************
*
*       ��flash�ж�ȡ���������
*
****************************************************/
void my_data_load()
{
    flash_data_load(&data);//��������   
    Servo_pid.P0=md.sP0;
    Servo_pid.D0=md.sD0;    
    Servo_pid.P1=md.sP1;
    Servo_pid.D1=md.sD1;
    Servo_pid.P2=md.sP2;
    Servo_pid.D2=md.sD2;
    Servo_pid.P3=md.sP3;
    Servo_pid.D3=md.sD3;
    
    Motor_PID->P=md.mP;
    Motor_PID->I=md.mI;
    Motor_PID->D=md.mD;
    
    Ecc_Car->M_Duty=md.cM_Duty;
    
    for(uint8 i=0;i<INDUCT_NUM;i++)
    {
        Induct[i].Nor_min=md.nor_min[i];
        Induct[i].Nor_MM_value=md.nor_MM_value[i];
    }
    Scan.finish_flag=1;
}