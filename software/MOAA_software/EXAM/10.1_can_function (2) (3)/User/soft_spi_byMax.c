// soft_spi.c
#include "soft_spi_byMax.h"

//// should i do this?
//#include "ch32v20x.h"

// SPI 引脚定义，根据实际连接修改
// at GPIO_B
#define SCLK_PIN GPIO_Pin_13 // SCLK 引脚
#define DIN_PIN  GPIO_Pin_15  // DIN 引脚
#define SYNC_PIN GPIO_Pin_12  // SYNC 引脚

#define SW_RESET_PIN GPIO_Pin_4  // SYNC 引脚

// 软SPI操作函数
void Soft_SPI_Init(void) {

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//
//    GPIOB->CFGLR = 0x33333333;

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = SCLK_PIN | DIN_PIN | SYNC_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


//    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = SW_RESET_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIOA->BSHR |= SW_RESET_PIN;
}

void Soft_SPI_Start(void) {

    GPIOB->BCR |= SYNC_PIN;
}

void Soft_SPI_Stop(void) {
//    GPIO_SetBits(GPIOC, SYNC_PIN); // SYNC置高更新开关状态

      //1
    GPIOB->BSHR |= SYNC_PIN;
}

void Soft_SPI_Write(uint16_t data) {
    for (int i = 0; i < 16; i++) {
        // 发送最高位
        if (data & 0x8000) {
//            GPIO_SetBits(GPIOC, DIN_PIN);

              //1
            GPIOB->BSHR |= DIN_PIN;

            Delay_Us(100);

        } else {

            GPIOB->BCR |= DIN_PIN;

            Delay_Us(100);

        }
        data <<= 1;

        // 产生时钟上升沿
        GPIOB->BSHR |= SCLK_PIN;

        Delay_Us(100);

        // 产生时钟下降沿
        GPIOB->BCR |= SCLK_PIN;

        Delay_Us(100);
    }
}

void writeToADG714(uint8_t chip1Data, uint8_t chip2Data) {
    uint16_t combinedData = (chip1Data << 8) | chip2Data; // 高8位给第一个芯片，低8位给第二个芯片
    Soft_SPI_Start();  // SYNC置低开始数据传输
    Soft_SPI_Write(combinedData); // 发送16位数据
    Soft_SPI_Stop();   // SYNC置高更新开关状态
}

//eg: n = 4, means turn off the third sw
void turn_off_Nth_sw(uint16_t n){
//    for (uint8_t i = 0; i < 16; i++) {

        uint8_t chip1Data = 0x00; // 第一个芯片的状态
        uint8_t chip2Data = 0x00; // 第二个芯片的状态

//        chip1Data |= n & 0xFF; // 0xFF是低八位掩码
//        chip2Data |= (n >> 8) & 0xFF;

        chip1Data |= (n >> 8) & 0xFF;
        chip2Data |=  n & 0xFF;

        // 打开目标开关
        writeToADG714(~chip1Data, ~chip2Data);

        // 延迟一段时间以保持开关状态
        Delay_Ms(5); // 延迟方便观察

}


// at GPIOC

#define MX_SW_1 GPIO_Pin_0 //
#define MX_SW_2 GPIO_Pin_1  //
#define MX_SW_3 GPIO_Pin_2  //
#define MX_SW_4 GPIO_Pin_3  //

void MX_Init(void) {

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = MX_SW_1 | MX_SW_2 | MX_SW_3 | MX_SW_4;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //notice this point!
    GPIOC->CFGLR = 0x33333333;

}



//0000 0000 0000 0001 ==> 0000
//0000 0000 0000 0010 ==> 0001
//0000 0000 0000 0100 ==> 0010
//0000 0000 0000 1000 ==> 0011
//
//0000 0000 0001 0000 ==> 0100
//0000 0000 0010 0000 ==> 0101
//0000 0000 0100 0000 ==> 0110
//0000 0000 1000 0000 ==> 0111
//
//0000 0001 0000 0000 ==> 1000
//0000 0010 0000 0000 ==> 1001
//0000 0100 0000 0000 ==> 1010
//0000 1000 0000 0000 ==> 1011
//
//0001 0000 0000 0000 ==> 1100
//0010 0000 0000 0000 ==> 1101
//0100 0000 0000 0000 ==> 1110
//1000 0000 0000 0000 ==> 1111
//
//
//c 语言中 前面是 uint16_t 类型的变量，后面是四位的数字，如何完成从前面到后面的转换？

//uint16_t numbers[] = {
//    0x0001, 0x0002, 0x0004, 0x0008,
//    0x0010, 0x0020, 0x0040, 0x0080,
//    0x0100, 0x0200, 0x0400, 0x0800,
//    0x1000, 0x2000, 0x4000, 0x8000
//};
//
//n

uint8_t re_arrange_MX(uint8_t n){


//    if(n == 0) {return 1;}
//
//    if(n == 1) {return 3;}
//
//    if(n == 2) {return 5;}
//
//    if(n == 3) {return 7;}

    if( n <= 7 ) {return n*2 + 1;}




    if( n == 8) {return 14;}

    if(n == 9) {return 12;}

    if(n == 10) {return 10;}

    if(n == 11) {return 8;}

    if(n == 12) {return 6;}

    if(n == 13) {return 4;}

    if(n == 14) {return 2;}

    if(n == 15) {return 0;}

    return n;



}

void MX_turn_n_on(uint8_t n){

//    n = re_arrange_MX(n);


    if(n & 0x01){
        GPIOC->BSHR |= MX_SW_1;
    }else{
        GPIOC->BCR |= MX_SW_1;
    }

    if(n & 0x02){
        GPIOC->BSHR |= MX_SW_2;
    }else{
        GPIOC->BCR |= MX_SW_2;
    }

    if(n & 0x04){
        GPIOC->BSHR |= MX_SW_3;
    }else{
        GPIOC->BCR |= MX_SW_3;
    }

    if(n & 0x08){
        GPIOC->BSHR |= MX_SW_4;
    }else{
        GPIOC->BCR |= MX_SW_4;
    }



//    while(1){
//
//        GPIOA->BCR |= GPIO_Pin_4;
//
//        GPIOC->BSHR |= MX_SW_1;
//        GPIOC->BSHR |= MX_SW_2;
//        GPIOC->BSHR |= MX_SW_3;
//        GPIOC->BSHR |= MX_SW_4;
//
//        printf("BSHR\r\n");
//
//        Delay_Ms(2);
//
////        GPIOC->BCR |= MX_SW_1;
////        GPIOC->BCR |= MX_SW_2;
////        GPIOC->BCR |= MX_SW_3;
////        GPIOC->BCR |= MX_SW_4;
////
////        printf("BCR\r\n");
////
//
////        Delay_Ms(2);
//    }
}


//====================================================================================================

//PB

#define S1 GPIO_Pin_0
#define S2 GPIO_Pin_1
#define S3 GPIO_Pin_3
#define S4 GPIO_Pin_5
#define S5 GPIO_Pin_8
#define S6 GPIO_Pin_9
#define S7 GPIO_Pin_10
#define S8 GPIO_Pin_11
#define S9 GPIO_Pin_14

//PC

#define S10 GPIO_Pin_6
#define S11 GPIO_Pin_7
#define S12 GPIO_Pin_8
#define S13 GPIO_Pin_9
#define S14 GPIO_Pin_10
#define S15 GPIO_Pin_11
#define S16 GPIO_Pin_12

// 用于保存所有需要设置的引脚
#define PB_PINS (S1 | S2 | S3 | S4 | S5 | S6 | S7 | S8 | S9)
#define PC_PINS (S10 | S11 | S12 | S13 | S14 | S15 | S16)



//====================================================================================================


void MOAA_GPIO_Init(void) {

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//
//    GPIOB->CFGLR = 0x33333333;

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = S1 | S2 | S3 | S4 | S5 | S6 | S7 | S8 | S9 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIOC->CFGLR = 0x33333333;
    GPIOC->CFGHR = 0x33333333;

}


//void MOAA_GPIO_Init(void) {
//
//
////    GPIO_InitTypeDef GPIO_InitStructure = {0};
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
//
//    GPIOB->CFGLR = 0x00000000;
//    GPIOB->CFGHR = 0x00000000;
//
//    GPIOC->CFGLR = 0x00000000;
//    GPIOC->CFGHR = 0x00000000;
//
//    GPIOB->CFGLR = 0x33;
//
//}


void MOAA_GPIO_turn_n_on(uint16_t n) {
    // 遍历每个引脚，如果 n 对应的位为 1，则打开对应的引脚

//    while(1){
//        GPIO_SetBits(GPIOB, S1);
//    }

    if (n & 0x0001) {
        // 打开第1个开关 S1
        GPIO_SetBits(GPIOB, S1);
    } else {
        // 关闭第1个开关 S1
        GPIO_ResetBits(GPIOB, S1);
    }

    if (n & 0x0002) {
        GPIO_SetBits(GPIOB, S2);
    } else {
        GPIO_ResetBits(GPIOB, S2);
    }

    if (n & 0x0004) {
        GPIO_SetBits(GPIOB, S3);
    } else {
        GPIO_ResetBits(GPIOB, S3);
    }

    if (n & 0x0008) {
        GPIO_SetBits(GPIOB, S4);
    } else {
        GPIO_ResetBits(GPIOB, S4);
    }

    if (n & 0x0010) {
        GPIO_SetBits(GPIOB, S5);
    } else {
        GPIO_ResetBits(GPIOB, S5);
    }

    if (n & 0x0020) {
        GPIO_SetBits(GPIOB, S6);
    } else {
        GPIO_ResetBits(GPIOB, S6);
    }

    if (n & 0x0040) {
        GPIO_SetBits(GPIOB, S7);
    } else {
        GPIO_ResetBits(GPIOB, S7);
    }

    if (n & 0x0080) {
        GPIO_SetBits(GPIOB, S8);
    } else {
        GPIO_ResetBits(GPIOB, S8);
    }

    if (n & 0x0100) {
        GPIO_SetBits(GPIOB, S9);
    } else {
        GPIO_ResetBits(GPIOB, S9);
    }

    if (n & 0x0200) {
        GPIO_SetBits(GPIOC, S10);
    } else {
        GPIO_ResetBits(GPIOC, S10);
    }

    if (n & 0x0400) {
        GPIO_SetBits(GPIOC, S11);
    } else {
        GPIO_ResetBits(GPIOC, S11);
    }

    if (n & 0x0800) {
        GPIO_SetBits(GPIOC, S12);
    } else {
        GPIO_ResetBits(GPIOC, S12);
    }

    if (n & 0x1000) {
        GPIO_SetBits(GPIOC, S13);
    } else {
        GPIO_ResetBits(GPIOC, S13);
    }

    if (n & 0x2000) {
        GPIO_SetBits(GPIOC, S14);
    } else {
        GPIO_ResetBits(GPIOC, S14);
    }

    if (n & 0x4000) {
        GPIO_SetBits(GPIOC, S15);
    } else {
        GPIO_ResetBits(GPIOC, S15);
    }

    if (n & 0x8000) {
        GPIO_SetBits(GPIOC, S16);
    } else {
        GPIO_ResetBits(GPIOC, S16);
    }
}

//    while(1){
//        // 设置PB和PC的引脚为高电平
//         GPIOB->BSHR |= PB_PINS;  // 设置PB的所有引脚为高电平
//         GPIOC->BSHR |= PC_PINS;  // 设置PC的所有引脚为高电平
//
//         Delay_Ms(200);
//
//         // 设置PB和PC的引脚为高电平
//          GPIOB->BCR |= PB_PINS;  // 设置PB的所有引脚为高电平
//          GPIOC->BCR |= PC_PINS;  // 设置PC的所有引脚为高电平
//
//          Delay_Ms(200);
//    }




