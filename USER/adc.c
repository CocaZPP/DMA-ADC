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
GPIO_Init(GPIOA,&GPIO_InitTStruct);  	//  A5  A4  A6ģ������

ADC_CommonInitTStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled; //DMAģʽ��ֹ
ADC_CommonInitTStruct.ADC_Mode=ADC_Mode_Independent; // ADC����ģʽ
ADC_CommonInitTStruct.ADC_Prescaler=ADC_Prescaler_Div4;  // ��Ƶϵ��
ADC_CommonInitTStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;//���������׶ε���ʱ������
ADC_CommonInit(&ADC_CommonInitTStruct);     //ADCʱ��ģʽ

ADC_InitStruct.ADC_ContinuousConvMode=ENABLE;	//����ת��
ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;   //�Ҷ���
ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
ADC_InitStruct.ADC_NbrOfConversion=3;               // 2��ת���ڹ���������
ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;  // 12λת��ģʽ
ADC_InitStruct.ADC_ScanConvMode=ENABLE;       //ɨ��ģʽ
ADC_Init(ADC1,&ADC_InitStruct);         //����
//	ADC_Cmd(ADC1,ENABLE);     // ʹ��ADC 
}

void INit_dma(void)
	{
DMA_InitTypeDef       DMA_InitStructure;   
Init_adc();
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  //����ʱ��  
DMA_InitStructure.DMA_Channel = DMA_Channel_0; //����DMAͨ��   
DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_BASE+0x4C;; //����ĵ�ַ
DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)data2;  //�����ڴ��ַ
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //���赽�ڴ� 
DMA_InitStructure.DMA_BufferSize = 3; //��3��ת��ͨ����ADC�ɼ�3�� 
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ����
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //�ڴ��ַ����
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;  	//���ݿ��Ϊ32λ	ֻ�������Ų����λ//������ݿ��Ϊ16λ��U32data2�ĳ�u16
DMA_InitStructure.DMA_Mode =  DMA_Mode_Circular   ;  //ѭ��ת��  
DMA_InitStructure.DMA_Priority = DMA_Priority_High;  //DMA���ȼ�
DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable ;           
DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  
DMA_InitStructure.DMA_MemoryBurst =  DMA_MemoryBurst_INC4 ;  //�ڴ��ַ������ʽ
DMA_InitStructure.DMA_PeripheralBurst =  0 ; //�����ڴ�������ʽ
DMA_Init(DMA2_Stream0, &DMA_InitStructure);    //����
DMA_Cmd(DMA2_Stream0, ENABLE);               //ʹ��DMA 
// ת�����ȼ�
ADC_RegularChannelConfig(ADC1,4, 1, ADC_SampleTime_144Cycles );  
ADC_RegularChannelConfig(ADC1,5, 2, ADC_SampleTime_144Cycles );  
    ADC_RegularChannelConfig(ADC1,6, 3, ADC_SampleTime_144Cycles );   
ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);  //Դ���ݱ仯ʱ����DMA����
ADC_DMACmd(ADC1, ENABLE);  //ʹ��ADC1��DMA����
ADC_ContinuousModeCmd(ADC1, ENABLE);       // adc����ת��ģʽ
ADC_Cmd(ADC1,ENABLE);    	 // ʹ��ADC1
ADC_SoftwareStartConv(ADC1);               //����ת������ͨ��
}
	
