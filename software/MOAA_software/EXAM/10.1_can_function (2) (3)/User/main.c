
/*
 *@Note
 *GPIO routine:
 *PA0 push-pull output.
 *
 */

#include "debug.h"

#include "soft_spi_byMax.h"

#include "op_adc_byMax.h"













/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("GPIO Toggle TEST\r\n");



        Soft_SPI_Init(); // ³õÊ¼»¯ÈíSPIÒý½Å







            u16 ADC_val;

            uint16_t x_to_turn = 1;

            MX_Init();



            OPA1_Init();


            MOAA_GPIO_Init();

//            GPIOC->CFGLR = 0x00000003;
//            GPIOC->CFGHR = 0x00033333;
//
//            GPIOC->CFGLR = 0x33000000;
//            GPIOC->CFGHR = 0x00033333;

//            while(1){
//
//                for (uint16_t n =0x0001; n < 0x8000; n = n << 1){
//
//                    turn_off_Nth_sw(0xFFFF & n);
//
//                    Delay_Ms(20); // ÑÓ³Ù·½±ã¹Û²ì
//
//                    printf( "clam down! \r\n");
//                }
//            }








            uint8_t x_n = 2;

//            MOAA_GPIO_turn_n_on(x_to_turn << x_n);

            ADC_Channel3_Init();







            while(1){

                for(x_n = 0; x_n <= 0x0F ; x_n = x_n +1){


//                    x_n = 1;

                    turn_off_Nth_sw(0xFFFF & 0x0001<<x_n);
//                    printf( "SW turned \r\n");
                    Delay_Ms(5);




                    MOAA_GPIO_turn_n_on(x_to_turn << x_n);
//                    printf( "x_n is %d, gpio_turned \r\n", x_n);
                    Delay_Ms(5);









                    for(uint8_t y_n = 0 ; y_n <= 0x0F ; y_n += 1 ){


                        MX_turn_n_on(y_n);

                        Delay_Ms(1);

                        ADC_val = Get_ADC_Average( ADC_Channel_2, 1 );




//                        printf( "x_n is %d, y_n is %d, OPA_OUT=%04d\r\n", x_n, re_arrange_MX(y_n), ADC_val );

//                        ADC_val*=3;
                        printf( "x_n is %d, y_n is %d, OPA_OUT=%04d\r\n", x_n, y_n, ADC_val );


                        Delay_Us( 5 );

                        Delay_Ms(1);
                    }
                }

//                x_n = 2;
//
//
//                MOAA_GPIO_turn_n_on(x_to_turn << x_n);
//
//
//                for(uint8_t y_n = 0 ; y_n <= 0x0F ; y_n += 1 ){
//
//
//                    MX_turn_n_on(y_n);
//
//                    Delay_Ms(1);
//
//                    ADC_val = Get_ADC_Average( ADC_Channel_2, 1 );
//
//
//                    printf( "x_n is %d, y_n is %d, OPA_OUT=%04d\r\n", x_n, re_arrange_MX(y_n), ADC_val );
//
//
//                    Delay_Us( 5 );
//
//                    Delay_Ms(1);
//                }



        }




        MX_Init();

//        MX_turn_n_on(0x00);

        MX_turn_n_on(0x01);






//        u16 ADC_val;
//        OPA1_Init();
//        ADC_Channel3_Init();

//        while( 1 )
//        {
//
//                ADC_val = Get_ADC_Average( ADC_Channel_2, 2 );
//                printf( "OPA_OUT=%04d\r\n", ADC_val );
//                Delay_Us( 5 );
//
//        }







//            ========================TEST1============================================================


//    MOAA_GPIO_Init();
//
//    while(1){
//        for(uint16_t n = 1; n < 0x8000; n = n << 1){
//
//            MOAA_GPIO_turn_n_on(n);
//
//            Delay_Ms(10);
//        }
//
//    }
//
////
//
//
//    MX_Init();
//
//    while(1){
//
////        MX_turn_n_on(0x0F);
//
//        for(uint8_t n = 1; n <= 0x0F; n = n + 1)
//        {
//            Delay_Ms(20); // ÑÓ³Ù·½±ã¹Û²ì
//            MX_turn_n_on(n);
//        }
//
//    }







}
