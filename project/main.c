#include "main.h"
// Noi khai bao hang so
#define     LED     PORTD
#define     ON      1
#define     OFF     0

#define PORTD_OUT   PORTD
#define TRISD_OUT   TRISD

void init_output(void);
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);
void Test_KeyMatrix();
//Chuong trinh Password Door
unsigned char arrayMapOfNumber [16] = {1,2,3,'A',4,5,6,'B',
                                       7,8,9,'C','*',0,'E','D'};
unsigned char arrayMapOfPassword [5][4]= {
  {1,2,3,4},
  {2,7,8,9},
  {3,3,3,3},
  {4,8,6,8},
  {5,'A','B','C'},
};

unsigned char isButtonNumber();
unsigned char numberValue;
unsigned char indexOfNumber = 0;
unsigned char isButtonEnter();
void Phase1_GreenOn();
void Phase1_GreenOff();
void Phase1_YellowOn();
void Phase1_YellowOff();
void Phase1_RedOn();
void Phase1_RedOff();

void Phase2_GreenOn();
void Phase2_GreenOff();
void Phase2_YellowOn();
void Phase2_YellowOff();
void Phase2_RedOn();
void Phase2_RedOff();

#define     INIT_SYSTEM         255
#define     PHASE1_GREEN        0
#define     PHASE1_YELLOW       1
#define     PHASE2_GREEN        2
#define     PHASE2_YELLOW       3
#define     WAIT                4
#define     SLOWDOWN            5
#define     SET_TIME_GREEN_1    6
#define     SET_TIME_GREEN_2    7
#define     SET_TIME_YELLOW_1   8
#define     SET_TIME_YELLOW_2   9
#define     Pedestrian_1_wait   10
#define     Pedestrian_2_wait   11
#define     Pedestrian_1_go     12
#define     Pedestrian_2_go     13


unsigned char statusOfLight = INIT_SYSTEM;
unsigned char timeOfLight = 0;
unsigned char timeOfGreen1 = 30;
unsigned char timeOfGreen2 = 30;
unsigned char timeOfYellow1 = 3;
unsigned char timeOfYellow2 = 3;
unsigned char cntOfLight = 0;
unsigned char timedelay = 0;
unsigned char arrsave[2];
unsigned char temp = 0;
unsigned char  cnt = 0;
void AppTrafficLight();

void main(void)
{
	//unsigned int k = 0;
	init_system();
    //delay_ms(1000);
	while (1)
	{
            while (!flag_timer3);
            flag_timer3 = 0;
            scan_key_matrix(); // 8 button
            AppTrafficLight();
            DisplayLcdScreenOld(); //Output process 14ms vs 10ms with no timer
            
	}
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_output(void)
{
   TRISD_OUT = 0x00;
    PORTD_OUT = 0x00;
}

void init_system(void)
{
    init_output();
    lcd_init();
    init_key_matrix();
    init_interrupt();
    lcd_clear();
    LcdClearS();
    delay_ms(500);
    init_timer0(4695);//dinh thoi 1ms sai so 1%
    init_timer3(46950);//dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
    //PORTAbits.RA0 = 1;
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT | arrayMapOfOutput[index];
	}


}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT & ~arrayMapOfOutput[index];
	}
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int k;
	for (k=0;k<14 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}

void Phase1_GreenOn()
{
    OpenOutput(0);
}
void Phase1_GreenOff()
{
    CloseOutput(0);
}

void Phase1_YellowOn()
{
    OpenOutput(4);
}
void Phase1_YellowOff()
{
    CloseOutput(4);
}

void Phase1_RedOn()
{
    OpenOutput(6);
}
void Phase1_RedOff()
{
    CloseOutput(6);
}

void Phase2_GreenOn()
{
    OpenOutput(1);
}
void Phase2_GreenOff()
{
    CloseOutput(1);
}

void Phase2_YellowOn()
{
    OpenOutput(5);
}
void Phase2_YellowOff()
{
    CloseOutput(5);
}

void Phase2_RedOn()
{
    OpenOutput(7);
}
void Phase2_RedOff()
{
    CloseOutput(7);
}

void AppTrafficLight()
{
    switch (statusOfLight)
    {
        case INIT_SYSTEM:
            LcdPrintStringS(0,0,"1810226");
            LcdPrintStringS(1,0,"Traffic Light");
            Phase1_GreenOff();
            Phase1_YellowOff();
            Phase2_GreenOff();
            Phase2_YellowOff();
            Phase1_RedOff();
            Phase2_RedOff();
            if(isButtonNumber()){
                if(numberValue == 'B'){
                    LcdClearS();
                    statusOfLight = SET_TIME_GREEN_1;
                }
            }
            if(isButtonNumber()){
                if(numberValue == 'C'){
                    LcdClearS();
                    Phase1_GreenOff();
                    Phase1_YellowOff();
                    Phase2_GreenOff();
                    Phase2_YellowOff();
                    Phase1_RedOff();
                    Phase2_RedOff();
                    statusOfLight = SLOWDOWN;
                }
            }
            if(isButtonNumber()){
                if(numberValue == 'A'){
                    LcdClearS();
                    statusOfLight = PHASE1_GREEN;
                } 
            }            
            break;
        case SET_TIME_GREEN_1:
            LcdPrintStringS(0,0,"SET TIME GREEN 1");
            LcdPrintStringS(1,0,"(10~99)");
            if(isButtonNumber()){
                LcdPrintNumS(1,indexOfNumber + 14,numberValue);
                arrsave [indexOfNumber] = numberValue;
                indexOfNumber++;
            }
            timeOfGreen1 = (arrsave [0]*10 + arrsave [1]);
            if(indexOfNumber == 2){
                temp++;
                if(temp > 10){
                    LcdClearS();
                    temp = 0;
                    indexOfNumber = 0;
                    statusOfLight = SET_TIME_YELLOW_1;
                }
            }
            break;
        case SET_TIME_YELLOW_1:            
            LcdPrintStringS(0,0,"SET TIME YELLOW1");
            LcdPrintStringS(1,0,"(1~9)");
            if(isButtonNumber()){
                LcdPrintNumS(1,indexOfNumber + 15,numberValue);
                arrsave [indexOfNumber] = numberValue;
                indexOfNumber++;
            }
            timeOfYellow1 = (arrsave [0]);
            if(indexOfNumber == 1){
                temp++;
                if(temp > 10){
                    LcdClearS();
                    indexOfNumber = 0;
                    statusOfLight = SET_TIME_GREEN_2;
                    temp = 0;
                }
            }
            break;
        case SET_TIME_GREEN_2:            
            LcdPrintStringS(0,0,"SET TIME GREEN 2");
            LcdPrintStringS(1,0,"(10~99)");
            if(isButtonNumber()){
                LcdPrintNumS(1,indexOfNumber + 14,numberValue);
                arrsave [indexOfNumber] = numberValue;
                indexOfNumber++;
            }
            timeOfGreen2 = (arrsave[0]*10 + arrsave[1]);
            if(indexOfNumber == 2){
                temp++;
                if(temp > 13){
                    LcdClearS();
                    temp = 0;
                    indexOfNumber = 0;
                    statusOfLight = SET_TIME_YELLOW_2;
                }               
            }
            break;
        case SET_TIME_YELLOW_2:            
            LcdPrintStringS(0,0,"SET TIME YELLOW2");
            LcdPrintStringS(1,0,"(1~9)");
            if(isButtonNumber()){
                LcdPrintNumS(1,indexOfNumber + 15,numberValue);
                arrsave [indexOfNumber] = numberValue;
                indexOfNumber++;
            }
            timeOfYellow2 = arrsave[0];
            if(indexOfNumber == 1){
                temp++;
                if(temp > 13){
                    LcdClearS();
                    temp = 0;
                    indexOfNumber = 0;
                    statusOfLight = PHASE1_GREEN;
                }               
            }
            break;
        case SLOWDOWN:
            LcdPrintStringS(0,0,"SLOW DOWN !!!   ");
            LcdPrintStringS(1,0,"Press */D to Go");
            cntOfLight++;
            if(cntOfLight % 20 == 0){
                timedelay++;
                Phase1_YellowOn();
                Phase2_YellowOn();
            }
            if (timedelay % 2 == 0){
                Phase1_YellowOff();
                Phase2_YellowOff();
            }
            if(isButtonNumber()){
                if(numberValue == 'C'){
                    LcdClearS();
                    timedelay = 0;
                    cnt = 0;
                    cntOfLight = 0;
                    statusOfLight = INIT_SYSTEM;
                }                
            }
            if(isButtonNumber()){
                if(numberValue == '*'){
                    LcdClearS();
                    cnt = 0;
                    timedelay = 0;
                    cntOfLight = 0;
                    statusOfLight = Pedestrian_1_wait;
                }                
            }
            if(isButtonNumber()){
                if(numberValue == 'D'){
                    LcdClearS();
                    Phase2_YellowOff();
                    timedelay = 0;
                    cntOfLight = 0;
                    statusOfLight = Pedestrian_2_wait;
                }                
            }
            break;
        case Pedestrian_1_wait:
            Phase1_YellowOn();
            timeOfLight = 5;
            cntOfLight++;
            if((cntOfLight + 1) % 20 == 0){
                cnt++;
                Phase2_YellowOn();
                LcdPrintStringS(0,0,"Waiting...      ");
                LcdPrintNumS(1,15,timeOfLight + 1 - cnt);
            }
            if(cnt % 2 == 0){
                Phase2_YellowOff();
            }
            if(cnt > timeOfLight){
                LcdClearS();
                statusOfLight = Pedestrian_1_go;
                cntOfLight = 0;
                cnt = 0;
            }
            break;
        case Pedestrian_1_go:
            timeOfLight = 30;
            Phase1_RedOn();
            Phase1_YellowOff();
            cntOfLight++;
            if((cntOfLight + 1) % 20 == 0){
                cnt++;
                Phase2_YellowOn();
                LcdPrintStringS(0,0,"Pedestrian 1 Go");
                LcdPrintStringS(1,14,"  ");
                LcdPrintNumS(1,14,timeOfLight + 1 - cnt);
            }
            if(cnt % 2 == 0){
                Phase2_YellowOff();
            }
            if(cnt > timeOfLight){
                LcdClearS();
                Phase1_RedOff();
                statusOfLight = SLOWDOWN;
                cntOfLight = 0;
                cnt = 0;
                temp = 0;
            }
            break;
        case Pedestrian_2_wait:
            Phase2_YellowOn();
            timeOfLight = 5;
            cntOfLight++;
            if((cntOfLight + 1) % 20 == 0){
                cnt++;
                Phase1_YellowOn();
                LcdPrintStringS(0,0,"Waiting...      ");
                LcdPrintNumS(1,15,timeOfLight + 1 - cnt);
            }
            if(cnt % 2 == 0){
                Phase1_YellowOff();
            }
            if(cnt > timeOfLight){
                LcdClearS();
                statusOfLight = Pedestrian_2_go;
                cntOfLight = 0;
                cnt = 0;
            }
            break;
         case Pedestrian_2_go:
            Phase2_RedOn();
            Phase2_YellowOff();
            timeOfLight = 30;
            cntOfLight++;
            if((cntOfLight + 1) % 20 == 0){
                cnt++;
                Phase1_YellowOn();
                LcdPrintStringS(0,0,"Pedestrian 2 Go");
                LcdPrintStringS(1,14,"  ");
                LcdPrintNumS(1,14,timeOfLight + 1 - cnt);
            }
            if(cnt % 2 == 0){
                Phase1_YellowOff();
            }
            if(cnt > timeOfLight){
                LcdClearS();
                Phase2_RedOff();
                statusOfLight = SLOWDOWN;
                cntOfLight = 0;
                cnt = 0;
            }
            break;
        case PHASE1_GREEN:
            timeOfLight = timeOfGreen1;
            Phase1_RedOff();
            Phase1_GreenOn();
            Phase2_RedOn();
            Phase2_YellowOff();
            cntOfLight++;
            if((cntOfLight + 1) % 20 == 0){
                cnt++;
                LcdPrintStringS(0,0,"1_Green");
                LcdPrintStringS(0,14,"  ");
                LcdPrintNumS(0,14,timeOfLight + 1 - cnt);
                LcdPrintStringS(1,0,"2_Red");
                LcdPrintStringS(1,14,"  ");
                LcdPrintNumS(1,14,timeOfLight + 1 + timeOfYellow1 - cnt);
            }
            
            if(cnt > timeOfLight){
                LcdPrintStringS(0,0,"                ");
                LcdPrintStringS(1,0,"                ");
                statusOfLight = PHASE1_YELLOW;
                cntOfLight = 0;
                cnt = 0;
            }
            if(isButtonNumber()){
                if(numberValue == 'C'){
                    LcdClearS();
                    cnt = 0;
                    cntOfLight = 0;
                    Phase1_GreenOff();
                    Phase1_YellowOff();
                    Phase2_GreenOff();
                    Phase2_YellowOff();
                    Phase1_RedOff();
                    Phase2_RedOff();
                    statusOfLight = SLOWDOWN;
                }
            }
            break;
        case PHASE1_YELLOW:
            Phase1_GreenOff();
            Phase1_YellowOn();
            timeOfLight = timeOfYellow1;           
            cntOfLight++;
            if((cntOfLight + 1) % 20 == 0){
                cnt++;
                LcdPrintStringS(0,0,"1_Yellow");
                LcdPrintStringS(0,14,"  ");
                LcdPrintNumS(0,14,timeOfLight + 1 - cnt);
                LcdPrintStringS(1,0,"2_Red");
                LcdPrintStringS(1,14,"  ");
                LcdPrintNumS(1,14,timeOfLight + 1 - cnt);
            }
            if (cnt > timeOfLight){
                LcdClearS();
                statusOfLight = PHASE2_GREEN;
                cntOfLight = 0;
                cnt = 0;
            }
            if(isButtonNumber()){
                if(numberValue == 'C'){
                    LcdClearS();
                    cnt = 0;
                    cntOfLight = 0;
                    Phase1_GreenOff();
                    Phase1_YellowOff();
                    Phase2_GreenOff();
                    Phase2_YellowOff();
                    Phase1_RedOff();
                    Phase2_RedOff();
                    statusOfLight = SLOWDOWN;
                }
            }
            break;
        case PHASE2_GREEN:
            timeOfLight = timeOfGreen2;
            Phase1_YellowOff();
            Phase1_RedOn();
            Phase2_RedOff();
            Phase2_GreenOn();
            cntOfLight++;
            if((cntOfLight + 1) % 20 == 0){
                cnt++;
                LcdPrintStringS(0,0,"1_Red");
                LcdPrintStringS(0,14,"  ");
                LcdPrintNumS(0,14,timeOfLight + 1 + timeOfYellow2 - cnt);
                LcdPrintStringS(1,0,"2_Green");
                LcdPrintStringS(1,14,"  ");
                LcdPrintNumS(1,14,timeOfLight + 1 - cnt);
            }
            if (cnt > timeOfLight){
                LcdClearS();
                statusOfLight = PHASE2_YELLOW;
                cntOfLight = 0;
                cnt = 0;
            }
            if(isButtonNumber()){
                if(numberValue == 'C'){
                    LcdClearS();
                    cnt = 0;
                    cntOfLight = 0;
                    Phase1_GreenOff();
                    Phase1_YellowOff();
                    Phase2_GreenOff();
                    Phase2_YellowOff();
                    Phase1_RedOff();
                    Phase2_RedOff();
                    statusOfLight = SLOWDOWN;
                }
            }
            break;
        case PHASE2_YELLOW:
            Phase2_GreenOff();
            Phase2_YellowOn();
            timeOfLight = timeOfYellow2;
            cntOfLight++;
            if((cntOfLight + 1) % 20 == 0){
                cnt++;
                LcdPrintStringS(0,0,"1_Red");
                LcdPrintStringS(0,14,"  ");
                LcdPrintNumS(0,14,timeOfLight + 1 - cnt);
                LcdPrintStringS(1,0,"2_Yellow");
                LcdPrintStringS(1,14,"  ");
                LcdPrintNumS(1,14,timeOfLight + 1 - cnt);
            }
            if (cnt > timeOfLight){
                LcdClearS();
                statusOfLight = PHASE1_GREEN;
                cntOfLight = 0;
                cnt = 0;
            }
            if(isButtonNumber()){
                if(numberValue == 'C'){
                    cnt = 0;
                    cntOfLight = 0;
                    LcdClearS();
                    Phase1_GreenOff();
                    Phase1_YellowOff();
                    Phase2_GreenOff();
                    Phase2_YellowOff();
                    Phase1_RedOff();
                    Phase2_RedOff();
                    statusOfLight = SLOWDOWN;
                }
            }
            break;
    }
}

unsigned char isButtonNumber()
{
    unsigned char i;
    for (i = 0; i<=15; i++)
        if (key_code[i] == 1)
        {
            numberValue = arrayMapOfNumber[i];
            return 1;
        }
    return 0;
}

unsigned char isButtonEnter()
{
    if (key_code[12] == 1)
        return 1;
    else
        return 0;
}