
#include "op_adc_byMax.h"


//extern u16 ADC_val, i;


/*********************************************************************
 * @fn      OPA1_Init
 *
 * @brief   Initializes OPA1 collection.
 *
 * @return  none
 */
void OPA1_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    OPA_InitTypeDef  OPA_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init( GPIOB, &GPIO_InitStructure );

    OPA_InitStructure.OPA_NUM = OPA2;
    OPA_InitStructure.PSEL = CHP1;
    OPA_InitStructure.NSEL = CHN1;
    OPA_InitStructure.Mode = OUT_IO_OUT0;
    OPA_Init( &OPA_InitStructure );
    OPA_Cmd( OPA2, ENABLE );

}

/*********************************************************************
 * @fn      ADC_Channel3_Init
 *
 * @brief   Initializes ADC Channel3 collection.
 *
 * @return  none
 */
void ADC_Channel3_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    ADC_InitTypeDef ADC_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig( RCC_PCLK2_Div8 );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init( ADC1, &ADC_InitStructure );

    ADC_Cmd( ADC1, ENABLE );
    ADC1->CTLR1 |= ( 1 << 26 );  //buffer en
}


u16 Get_ADC_Val( u8 ch )
{
    u16 val;

    ADC_RegularChannelConfig( ADC1, ch, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd( ADC1, ENABLE );

    while( !ADC_GetFlagStatus( ADC1, ADC_FLAG_EOC ) );

    val = ADC_GetConversionValue( ADC1 );

    return val;
}

u16 Get_ADC_Average( u8 ch, u8 times )
{
    u32 temp_val = 0;
    u8 t;
    u16 val;

    for( t = 0; t < times; t++ ){
        temp_val += Get_ADC_Val( ch );
        Delay_Ms( 1 );
    }

    val = temp_val / times;

    return val;
}
