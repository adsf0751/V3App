#define _TIMER_NEXSYS_1_        TIMER_ID_1
#define _TIMER_NEXSYS_2_        TIMER_ID_2
#define _TIMER_NEXSYS_3_        TIMER_ID_3
#define _TIMER_NEXSYS_4_        TIMER_ID_4
#define TIMER_ID_1              0
#define TIMER_ID_2              1
#define TIMER_ID_3              2
#define TIMER_ID_4              3

int inDISP_PutGraphic(char *, int , int );
int inPRINT_Buffer_GetHeight(unsigned char* , int* );
int inDISP_Timer_Start(int , long );
int inTimerGet(int);
int inTimerStart(int , long );