/*********************** (C) COPYRIGHT  2017 DOC-AI  **************************

  * Copyright Wuhan DOC-AI Technology Inc.

  * File name   : SysWork.h
  * Version     : V0.1
  * Description : 血凝仪主控板工作程序
  * Date        : 2017年9月22日
  * Author      : XYD
  * Check       :  
*******************************************************************************/
#ifndef _SYSWORK_H_
#define _SYSWORK_H_


/*********************Hardware File include*********************/
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "USART_Configuration.h"
#include "MNTC_CanProtocol.h"
#include "DS18B20_V0.1.h"
#include "DW_Agreement.h"
#include "Barcode_V0.1.h"
#include "MNTC_CanConfig.h"

/*********************User File include*********************/
#include "17003UI.h"
#include "SysWork.h"
#include "Scope_SendData.h"
#include "Calibration.h"
#include "Delay.h"
#include "TIM.h"
/******************Macro definition********************/
//板卡ID，不同项目自行进行分配
#define CAN_MASTER_ID   0X01            //主板ID
#define CAN_Z_AXIS_ID   0X02            //Z轴板ID
#define CAN_X_AXIS_ID   0X03            //X轴板ID
#define CAN_Y_AXIS_ID   0X04            //Y轴板ID
#define CAN_PLUNGER_ID  0X05            //柱塞泵板ID
#define CAN_ADC_ID      0XF6            //ADC板ID
#define CAN_TEMP_ID     0XF7            //温控板板ID
#define CAN_PRP1_ID			0XF8						//蠕动泵1ID
#define CAN_PRP2_ID			0XF9						//蠕动泵2ID



#define MAX_X_POS       330.0               //X轴最大行程330mm
#define MAX_Y_POS       170.0               //Y轴最大行程170mm
#define MAX_Z_POS       33.0                //运动中Z轴最大行程33mm
#define SAFE_Z_POS      0                   //Z轴安全行程，移动中保证不撞针


//样本位坐标信息
#define SAMPLE_SPACE_X        25.5                 //相邻样本架X方向间距
#define SAMPLE_SPACE_Y        16                 //同一个样本架上相邻样本位Y方向的中心距

//供杯位坐标信息

#define CUP_SPACE_X           17                      //相邻供杯位X方向间距
#define CUP_SPACE_Y           13.0                      //相邻供杯位Y方向间距
#define CUP_RACK_SPACE_Y      21.0                      //两个供杯位在Y方向上的间距
#define CUP_NUM_X             8                         //一个供杯架X方向上9个供杯位
#define CUP_NUM_Y             5                         //一个供杯架Y方向上5个供杯位

//试剂位坐标信息
#define REAGENT_SPACE1_X       35.0                //相邻大号试剂位X方向间距
#define REAGENT_RACK_SPACE_X   -2.0               //第一个大号试剂位和第一个小号试剂位X方向间距
#define REAGENT_RACK_SPACE_Y   32.0                //第一个大号试剂位和第一个小号试剂位Y方向间距
#define REAGENT_SPACE2_X      26.0                //相邻小号试剂位X方向间距
#define REAGENT_SPACE2_Y      26.0                //相邻小号试剂位Y方向间距
#define REAGENT_NUM_X        4                   //小号试剂位X方向个数
#define REAGENT_NUM_Y        4                   //小号试剂位Y方向个数

//检测位坐标信息
#define DETECT_SPACE_X    16.8              //相邻检测位X方向间距
#define DETECT_SPACE_Y    0.0               //相邻检测位Y方向间距


//模块间相对距离
#define DETECT_BLOCK_X    53.9              	//相邻检测块X方向间距
#define DETECT_BLOCK_Y    0.0               //相邻检测块Y方向间距

#define DETECT_BLOCK45_X    48              	//检测块4、5X方向间距
#define DETECT_BLOCK45_Y    0.0               //检测块4、5Y方向间距

#define DETECT_BLOCK56_X    36              	//检测块4、5X方向间距
#define DETECT_BLOCK56_Y    0.0               //检测块4、5Y方向间距

#define DETECT_TO_CUP_X   59								//第一个检测位和第一个供杯位X方向间距
#define DETECT_TO_CUP_Y   42							//第一个检测位和第一个供杯位Y方向间距

#define DETECT_TO_SAMPLE_X   0  							//第一个检测位和第一个样本位X方向间距
#define DETECT_TO_SAMPLE_Y   31						//第一个检测位和第一个样本位Y方向间距

#define DETECT_TO_REAGENT_X   234							//第一个检测位和第一个试剂位X方向间距
#define DETECT_TO_REAGENT_Y   55						//第一个检测位和第一个试剂位Y方向间距

#define DETECT_TO_WASH_X   200							//第一个检测位和清洗位X方向间距
#define DETECT_TO_WASH_Y   69					//第一个检测位和清洗位Y方向间距



//
#define GET_CUP_Y_DIST    16              	//取杯时Y后退的距离
#define DISCARD_CUP_Y_DIST    17              	//丢杯时Y后退的距离
#define HAVE_CUP_Z_POS    20                //取杯后保证不撞的Z轴坐标

#define GET_CUP_CUP_Z_POS     -14.5        //取供杯位空杯准备动作时Z轴坐标           
#define GET_DETECT_CUP_Z_POS  -14.5         //取检测位杯准备动作时Z轴坐标
#define DISCARD_CUP_Z_POS     -14.5           //丢杯位下降后的Z坐标



#define SAMPLE_RACK_NUM   2                 //样本架个数
#define SINGLE_RACK_NUM   9                 //单个样本架样本个数
#define SAMPLE_NUM        (SAMPLE_RACK_NUM*SINGLE_RACK_NUM)//样本数
#define MAX_TEST_LIST_NUM 12                 //一个样本最多能够做的项目个数
#define DETECT_SEAT_NUM   12                 //检测位个数
#define REAGENT_NUM       19                //试剂个数
#define CUP_NUM           90                //反应杯个数


//测试项目状态
#define ITEM_READY_FINISH            0      //准备
#define ITEM_SAMPLE_FINISH           1      //样本加入完成
#define ITEM_INCUBATE_FINISH         2      //孵育
#define ITEM_ADD_R1_FINISH           3      //R1试剂加样完成
#define ITEM_R1_TEST_FINISH          4      //R1试剂测试完成
#define ITEM_ADD_R2_FINISH           5      //R2试剂加样完成
#define ITEM_TEST_FINISH             6      //R2反应完成=反应完成
#define ITEM_DISCARD_FINISH          7      //丢杯完成

//试剂类型信息
#define PT_REAGENT        1                 //PT试剂
#define APTT_REAGENT      2                 //APTT试剂
#define TT_REAGENT        3                 //TT试剂
#define FIB_REAGENT       4                 //FIB试剂
#define OVB_REAGENT       5                 //OVB缓冲液
#define DD_R1_REAGENT     6                 //DD测试R1试剂
#define DD_R2_REAGENT     7                 //DD测试R2试剂
#define FDP_R1_REAGENT    8                 //FDP R1
#define FDP_R2_REAGENT    9                 //FDP R2
#define HEPARIN_REAGENT   10                //肝素试剂
#define AT3_R1_REAGENT    11                //AT3的R1试剂
#define AT3_R2_REAGENT    12                //AT3的R2试剂
#define CaCl2_REAGENT     13                //CaCl2试剂
#define CLEAN_REAGENT     14                //清洗液
//检测结果
#define PT_S_RESULT         0               //PT(S)
#define PT_INR_RESULT       1               //PT(INR)
#define PT_PERCENT_RESULT   2               //PT(%)
#define APTT_S_RESULT       3               //APTT(S)
#define TT_S_RESULT         4               //TT(S)
#define FIB_S_RESULT        5               //FIB(S)
#define FIB_GL_RESULT       6               //FIB(g/L)
#define DD_OD_RESULT        7               //DD(OD)
#define DD_GL_RESULT        8               //DD(g/L)
#define FDP_OD_RESULT       9               //FDP(OD)
#define FDP_GL_RESULT       10              //FDP(g/L)
#define AT3_OD_RESULT       11              //AT3(OD)
#define AT3_PERCENT_RESULT  12              //AT3(%)

//样本位状态
#define SAMPLE_GRAY       0                 //灰--未选定/已完成
#define SAMPLE_YELLOW     1                 //黄--选定/测试中
#define SAMPLE_GREEN      2                 //绿--设置完成/反应完成
#define SAMPLE_RED        3                 //红-- 

//供杯位状态
#define CUP_GRAY          0                 //灰--没有反应杯
#define CUP_GREEN         1                 //绿--有反应杯

//试剂位状态
#define REAGENT_GRAY      0                 //灰--未定义试剂
#define REAGENT_YELLOW    1                 //黄--试剂量少
#define REAGENT_GREEN     2                 //绿--试剂量充足
#define REAGENT_RED       3                 //红--试剂量预警
#define REAGENT_ENOUGH    20                //剩余试剂量>=REAGENT_ENOUGH-- 试剂量充足
#define REAGENT_ALARM     10                //剩余试剂量>=REAGENT_ALARM-- 试剂量少


//测试项目信息
#define PT_TEST           1                 //PT测试
#define APTT_TEST         2                 //APTT测试
#define TT_TEST           3                 //TT测试
#define FIB_TEST          4                 //FIB测试
#define AT3_TEST          5                 //AT3测试
#define FDP_TEST          6                 //FDP测试
#define DD_TEST           7                 //DD测试

//通道类型
#define SCATTERING_CH     0X00              //散射通道
#define TRANSMISSION_CH   0X01              //透射通道

//算法类型
#define THRESHOLD_METHOD    0X00            //阈值法
#define DERIVATIVE_1_METHOD 0X01            //一阶法          
#define DERIVATIVE_2_METHOD 0X02            //二阶法
#define MEDIAN_METHOD       0X03            //中值法


//CAN协议用户自定义段
#define CAN_CMD_USER_TESTSS     0X00        //检测启停控制指令
#define CAN_CMD_USER_STOP_TEST  0X07        //停止所有测试
#define CAN_CMD_USER_MIX        0X08        //混匀命令
#define CAN_CMD_USER_LEVEL      0X09        //定位到液面
#define CAN_CMD_USER_PTSET      0X10        //PT检测方法设置
#define CAN_CMD_USER_APTTSET    0X11        //APTT检测方法设置
#define CAN_CMD_USER_TTSET      0X12        //TT检测方法设置
#define CAN_CMD_USER_FIBSET     0X13        //FIB检测方法设置
#define CAN_CMD_USER_AT3SET     0X14        //AT3检测方法设置
#define CAN_CMD_USER_FDPSET     0X15        //FDP检测方法设置 
#define CAN_CMD_USER_DDPSET     0X16        //DD检测方法设置
#define CAN_CMD_USER_HELLO      0X17        //握手指令
#define CAN_CMD_USER_SELF_TEST  0X18        //自检指令

#define CAN_DATA_USER_RESULT    0X00        //检测完成，检测结果回传
#define CAN_DATA_USER_TEMP    	0X01        //温度数据回传


#define LED1_ON           GPIO_ResetBits(GPIOD,GPIO_Pin_3)
#define LED1_OFF          GPIO_SetBits(GPIOD,GPIO_Pin_3)

#define LED2_ON           GPIO_ResetBits(GPIOD,GPIO_Pin_4)
#define LED2_OFF          GPIO_SetBits(GPIOD,GPIO_Pin_4)

#define PUMP_OFF          GPIO_SetBits(GPIOD,GPIO_Pin_13)
#define PUMP_ON           GPIO_ResetBits(GPIOD,GPIO_Pin_13)

#define VALVE_OFF         GPIO_SetBits(GPIOD,GPIO_Pin_12)
#define VALVE_ON          GPIO_ResetBits(GPIOD,GPIO_Pin_12)

/*********Enumeration variable type definition********/




/**************Structure type definition**************/

//针对主控板所需的关于轴的信息定义的结构体
//未作特别说明时，结构体中数组元素的分配按XYZ的顺序进行
typedef struct _AxisStruct                    
{
  int   finish;                              //指令完成标志位，0-- 未完成，1-- 完成
  float speed;                               //mm/s,该数据通过CAN总线下发时要
  float position;                            //最近一次指令所指定的轴坐标信息,该数据通过CAN总线下发时要x10
}AxisStruct;

typedef struct _TestItemInfoStruct     //测试项目信息结构体
{
  int type;                        //测试项目类型，详见宏定义
  int detect_num;                  //所使用的检测位编号，从0开始计数
  int item_state;                  //测试项目状态,详见宏定义
	float item_result;
  float dilution;                  //稀释系数
}TestItemInfoStruct;

typedef struct _SampleInfoStruct   //sample information样本信息
{
  int rack_num;                    //所属样本架架号，
  int seat_num;                    //在对应样本架上样本编号，1~9
  char sample_code[12];            //样本编号，默认eg：201710190101
  int state;                       //样本位状态0灰--未选定/测试完成  1黄--编辑中/反应中 2绿--编辑完成/等待反应 3红--
  float result[13];                //测试结果，对应项目的结果放在数据对应位置
  TestItemInfoStruct item_list[MAX_TEST_LIST_NUM];    //测试列表，最多选择12个测试项目，允许重复测试项目,项目定义见SysWork文件宏定义
}SampleInfoStruct;

typedef struct _ReagentInfoStruct  //试剂信息结构体
{
  int  type;                        //实际类型
  int  single_cnt;                  //单瓶余量
  int  batch_cnt;                   //批次余量
  char batch_code[10];              //批号
  char date[12];                    //有效期，以天计算
  int  isi;                         //国际敏感度指数，专用于计算PT的INR值
}ReagentInfoStruct;

typedef struct _TestMethodInfoStruct  //测试方法结构体
{
  int ch_type;                     //通道类型，取值见宏定义
  int calcu_type;                  //算法类型，取值见宏定义
  int reference[2];                //参考值,[0]--参考值下限 [1]--参考值上限
  int sample_vol;                  //样本体积ul
  int incubate_time;               //孵育时间
  int r1_type;                     //试剂1类型
  int r1_vol;                      //试剂1体积
  int r2_type;                     //试剂2类型
  int r2_vol;                      //试剂2体积
  int max_time;                    //最大检测时间
}TestMethodInfoStruct;


//软件定时器结构体类型定义
typedef struct _SoftTimerStruct
{
	char enable;													//软件定时器使能标志位，1-- 使能  2--失能
	int  now;															//当前已计时间  时间=now*时基
	int  set;															//设定时间			时间=set*时基
}SoftTimerStruct;

//队列结构体
typedef struct _QueueStruct
{
  int front;                            //队首
  int rear;                             //队尾
  int max_size;                         //队列大小
  int *data;                            //队列
}QueueStruct;

/*************External variable declaration************/
extern MNTC_CanProtocolTypeDef CanCom;                          //CAN通信结构体
extern DW_Struct  LCD;	                                        //显示屏结构体								
extern int MaxRackNum;                                          //当前最大样本架数，默认2，更换新的样本架+1
extern SampleInfoStruct Sample[];                               //样本信息，最多100个样本
extern SampleInfoStruct EditSample[];       
extern ReagentInfoStruct Reagent[];                             //试剂信息，最多19个试剂
extern int CupSeat[];                                           //供杯位信息，灰0--没有杯，绿1--有杯
extern int RunState;                                            //设备运行状态，0--准备，1--运行中
extern TestMethodInfoStruct TestMethod[];                       //测试方法存储，[0]不使用,PT_TEST,APTT_TEST....
extern SoftTimerStruct DetectDelay[];                           //各检测块延时
extern int DetectSeatState[]; 
extern CalibrationStruct Calibration[4];                        //定标结构体     
extern CalibrationStruct EditCalibration;                       //编辑中的定标数据

extern QueueStruct Err;                                         //错误队列
extern BarcodeTypeDef Barcode;                                  //条码结构体
extern ReagentInfoStruct EditReagent;                           //编辑中的试剂信息缓存

extern float WashPosBase[];                                     //清洗位基地址
extern float SamplePosBase[];                                   //样本架基地址，第一个样本位坐标
extern float CupPosBase[];                                      //供杯位基地址，左上角第一个供杯位
extern float ReagentPosBase[];                                  //试剂位基地址，第一个试剂位坐标
extern float DetectPosBase[];                                   //检测位基地址
extern float DiscardPosBase[];                                  //丢杯位基地址

extern AxisStruct Axis[4];
extern int Pump[2];																						 //蠕动泵运动状态 0-- 未完成 1-- 完成
extern int NowPageNum;                                         //当前页面地址
extern int ReturnPagNum;                                       //返回的页面地址

extern int TestCnt;
extern int Hour,Minute,Second;
extern int LabviewFlag;
extern int LabviewBuf[20];

extern int FirstColNum;//结果显示列表左右滑动
extern int FirstRowNum;//结果显示列表上下滑动

extern int Temp1;
extern int Temp2;

/*****************Function declaration**************/
//组合功能实现
void DetectProcess(void);
void TestMethodInit(void);                                        //测试方法初始化
void SeatInfoInit(void);                                          //各种位信息初始化
void GetSample2Detect(int sample_cnt,int item_cnt,int detect_cnt);//取指定样本放入指定检测位用于指定检测项目
void AddR1ToSample(int sample_cnt,int item_cnt);                  //向指定样本的指定项目加入该项目所使用的R1
void AddR2ToSample(int sample_cnt,int item_cnt);                  //向指定样本的指定项目加入该项目所使用的R2
void WashNeedle(void);                                            //清洗进样针
void DiscardCup(int detect_num);                                  //从指定检测位丢杯
void JudgeDiscardCup(void);                                       //插入加样动作后的一个丢杯判断，专用

//用户自定义命令实现
void CAN_Mix(int time);                                               //混匀
void CAN_StartTest(int test_type,int detect_num);                     //通知AD板启动一项检测
void CAN_SetTestMethod(int test_type,TestMethodInfoStruct *method);   //测试方法设置
void CAN_Hello(int board_id);                                         //握手
void CAN_SelfTest(int board_id);                                      //自检
void PID_Decode(MNTC_CanProtocolTypeDef *buf);

//基本功能实现
void SampleInfoInit(void);                                        //初始化样本位信息
void GetSample(int rack_num,int sample_num,int volume);           //运动到指定样本位，取一定量样本
void RunAndAddSample(int vol,int detect_cnt);                     //边移动，边向当前抓取的空杯加样本
void GetCupFromEmpty(int cup_num);                                //运动到指定供杯位，抓取一个空杯
void PutCupFromEmpty(int cup_num);																//运动到指定供杯位，放置一个反应杯
void GetReagent(int reagent_num,int volume);                      //运动到指定试剂位，取一定量试剂
void GetCupFromDetect(int detect_num);                            //从检测位夹杯
void PutCup2Detect(int detect_num);                               //放置杯子到检测位
void PutReagent2Detect(int detect_num,int volume);                //向检测位加试剂

void WhileDelay_ms(int ms);                                       //while循环实现的非精确延时
void CAN_Decode(MNTC_CanProtocolTypeDef *buf);                    //CAN通信解码
void ADC_Decode(MNTC_CanProtocolTypeDef *buf);                    //ADC板相关的CAN数据解码子函数
void LabviewDecod(void);
void ScanerDecode(BarcodeTypeDef *buf);
void AxisRun2Pos(float x,float y,float z);                        //三轴运行到指定位置
void AxisRun2Zero(void);                                          //三轴精确回零
void AxisSetSpeed(int x_speed,int y_speed,int z_speed);           //三轴速度设置
void PlungerDraw(int vol,int run_time,int delay_time);            //柱塞泵抽取
void PlungerPush(int vol,int run_time,int delay_time);            //柱塞泵注射
void Plunger2Zero(void);                                          //柱塞泵回零
void PlungerWash(int vol,int cnt,int delay_time);                 //柱塞泵冲洗

void GetSamplePos(float *x_pos,float *y_pos,int rack_num,int sample_num);//计算输出指定样本位坐标
void GetCupPos(float *x_pos,float *y_pos,int cup_num);                   //计算输出指定供杯位坐标
void GetReagnetPos(float *x_pos,float *y_pos,int reagent_num);           //计算输出指定试剂位坐标
void GetDetectPos(float *x_pos,float *y_pos,int detect_num);             //计算输出指定检测位坐标
void TIM_TIM5_Configuration(void);                                       //1S周期中断
void TIM_TIM5_Open(void);                                                //开定时器                                      
void TIM_TIM5_Closed(void);                                              //关定时器
void LED_Init(void);                                                     //LED初始化
void StartSoftTimer(SoftTimerStruct *timer,int t);                       //启动软件定时器，先进行清零 
void ClearSoftTimer(SoftTimerStruct *timer);                             //清零软件定时器

void CreatQueue(QueueStruct *que,int max_size);                          //创建循环队列
int FullQueue(QueueStruct *que);                                         //队列判满，1--已满 0--未满
int EmptyQueue(QueueStruct *que);                                        //循环队列判空，1--空 0--非空
int InQueue(QueueStruct *que,int data);                                  //循环队列入队，1--入队成功，0--已满
int OutQueue(QueueStruct *que,int *data);                                //循环队列出队，1--出队成功，0--队列为空
void DiscardCup1(int detect_num,int cup_num);


void TesttProcess(void);
void PT_Test(int sample_num,int item_num);
void APTT_Test(int sample_num,int item_num);
void TT_Test(int sample_num,int item_num);
void FIB_Test(int sample_num,int item_num);
void AT3_Test(int sample_num,int item_num);

 #endif
/**************** (C) COPYRIGHT 2017 DOC-AI *****END OF FILE*******************/
