
/*************  ����֧���빺��˵��    **************
��Ʒ��ҳ��http://tw51.haohaodada.com
�Ա�����������51���ɹ���Ŀǰ������99Ԫ����������׼����149Ԫ
����֧��QQȺһ��1138055784
******************************************/

#include "adc.h"
#include "sys.h"

uint8  setbit = 0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC��ʼ��
//  @param      adcn            ѡ��ADCͨ��
//  @param      speed      		ADCʱ��Ƶ��
//  @return     void
//  Sample usage:               adc_init(ADC_P10,ADC_SYSclk_DIV_2);//��ʼ��P1.0ΪADC����,ADCʱ��Ƶ�ʣ�SYSclk/2
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADC_Name adcn, ADC_CLK speed, ADC_bit sbi)
{
	setbit = sbi;
	ADC_CONTR |= 1 << 7;	//1 ���� ADC ��Դ
	if (adcn > 15) 
	{
		adcn = adcn - 16;
		//IO����Ҫ����Ϊ��������
		P3M0 &= ~(1 << (adcn & 0x07));
		P3M1 |= (1 << (adcn & 0x07));


	}
	else {
		if ((adcn >> 3) == 1) //P0.0
		{
			//IO����Ҫ����Ϊ��������
			P0M0 &= ~(1 << (adcn & 0x07));
			P0M1 |= (1 << (adcn & 0x07));
		}
		else if ((adcn >> 3) == 0) //P1.0	
		{
			//IO����Ҫ����Ϊ��������
			P1M0 &= ~(1 << (adcn & 0x07));
			P1M1 |= (1 << (adcn & 0x07));
		}
	}

	ADCCFG |= speed & 0x0F;	//ADCʱ��Ƶ��SYSclk/2/speed&0x0F;

	ADCCFG |= 1 << 5;		//ת������Ҷ��롣 ADC_RES �������ĸ� 2 λ�� ADC_RESL �������ĵ� 8 λ��

}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADCת��һ��
//  @param      adcn            ѡ��ADCͨ��
//  @param      resolution      �ֱ���
//  @return     void
//  Sample usage:               adc_convert(ADC_P10, ADC_10BIT);
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_read(ADC_Name adcn)
{
	uint16 adc_value;
	if (adcn > 15)adcn = adcn - 8;

	ADC_CONTR &= (0xF0);	//���ADC_CHS[3:0] �� ADC ģ��ͨ��ѡ��λ
	ADC_CONTR |= adcn;

	ADC_CONTR |= 0x40;  // ���� AD ת��
	while (!(ADC_CONTR & 0x20));  // ��ѯ ADC ��ɱ�־
	ADC_CONTR &= ~0x20;  // ����ɱ�־


	adc_value = ADC_RES;  //�洢 ADC �� 10 λ����ĸ� 2 λ
	adc_value <<= 8;
	adc_value |= ADC_RESL;  //�洢 ADC �� 10 λ����ĵ� 8 λ

	ADC_RES = 0;
	ADC_RESL = 0;

	adc_value >>= setbit;//ȡ����λ


	return adc_value;
}

