#include <adc.h>
#include <stm32f4xx_adc.h>
#include <stm32f4xx.h>

void Init_adc(void)
{
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonInitTStruct;
GPIO_InitTypeDef GPIO_InitTStruct;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
GPIO_InitTStruct.GPIO_Mode=GPIO_Mode_AN;        
GPIO_InitTStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_6;
GPIO_InitTStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
GPIO_Init(GPIOA,&GPIO_InitTStruct);  	//  A5  A4  A6模拟输入

ADC_CommonInitTStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled; //DMA模式禁止
ADC_CommonInitTStruct.ADC_Mode=ADC_Mode_Independent; // ADC独立模式
ADC_CommonInitTStruct.ADC_Prescaler=ADC_Prescaler_Div4;  // 分频系数
ADC_CommonInitTStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;//两个采样阶段的延时周期数
ADC_CommonInit(&ADC_CommonInitTStruct);     //ADC时钟模式

ADC_InitStruct.ADC_ContinuousConvMode=ENABLE;	//连续转换
ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;   //右对齐
ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
ADC_InitStruct.ADC_NbrOfConversion=3;               // 2个转换在规则序列中
ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;  // 12位转换模式
ADC_InitStruct.ADC_ScanConvMode=ENABLE;       //扫描模式
ADC_Init(ADC1,&ADC_InitStruct);         //配置
//	ADC_Cmd(ADC1,ENABLE);     // 使能ADC 
}

void INit_dma(void)
	{
DMA_InitTypeDef       DMA_InitStructure;   
Init_adc();
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  //外设时钟  
DMA_InitStructure.DMA_Channel = DMA_Channel_0; //设置DMA通道   
DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_BASE+0x4C;; //外设的地址
DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)data2;  //数据内存地址
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //外设到内存 
DMA_InitStructure.DMA_BufferSize = 3; //有3个转换通道，ADC采集3次 
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址不变
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //内存地址增长
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为16位
DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;  	//数据宽度为32位	只有这样才不会错位//如果数据宽度为16位将U32data2改成u16
DMA_InitStructure.DMA_Mode =  DMA_Mode_Circular   ;  //循环转换  
DMA_InitStructure.DMA_Priority = DMA_Priority_High;  //DMA优先级
DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable ;           
DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  
DMA_InitStructure.DMA_MemoryBurst =  DMA_MemoryBurst_INC4 ;  //内存地址增长方式
DMA_InitStructure.DMA_PeripheralBurst =  0 ; //外设内存增长方式
DMA_Init(DMA2_Stream0, &DMA_InitStructure);    //配置
DMA_Cmd(DMA2_Stream0, ENABLE);               //使能DMA 
// 转换优先级
ADC_RegularChannelConfig(ADC1,4, 1, ADC_SampleTime_144Cycles );  
ADC_RegularChannelConfig(ADC1,5, 2, ADC_SampleTime_144Cycles );  
    ADC_RegularChannelConfig(ADC1,6, 3, ADC_SampleTime_144Cycles );   
ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);  //源数据变化时开启DMA传输
ADC_DMACmd(ADC1, ENABLE);  //使能ADC1的DMA功能
ADC_ContinuousModeCmd(ADC1, ENABLE);       // adc连续转换模式
ADC_Cmd(ADC1,ENABLE);    	 // 使能ADC1
ADC_SoftwareStartConv(ADC1);               //开启转换规则通道
}
	
