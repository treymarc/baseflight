#include "board.h"



#ifndef SOFT_I2C

#define I2C_SHORT_TIMEOUT             ((uint32_t)0x1000)
#define I2C_LONG_TIMEOUT             ((uint32_t)(10 * I2C_SHORT_TIMEOUT))

#define I2C1_SCL_GPIO        GPIOA
#define I2C1_SCL_PIN         GPIO_Pin_9
#define I2C1_SCL_PIN_SOURCE  GPIO_PinSource9
#define I2C1_SCL_CLK_SOURCE  RCC_AHBPeriph_GPIOA
#define I2C1_SDA_GPIO        GPIOA
#define I2C1_SDA_PIN         GPIO_Pin_10
#define I2C1_SDA_PIN_SOURCE  GPIO_PinSource10
#define I2C1_SDA_CLK_SOURCE  RCC_AHBPeriph_GPIOA

#define I2C2_SCL_GPIO        GPIOA
#define I2C2_SCL_PIN         GPIO_Pin_9
#define I2C2_SCL_PIN_SOURCE  GPIO_PinSource9
#define I2C2_SCL_CLK_SOURCE  RCC_AHBPeriph_GPIOA
#define I2C2_SDA_GPIO        GPIOA
#define I2C2_SDA_PIN         GPIO_Pin_10
#define I2C2_SDA_PIN_SOURCE  GPIO_PinSource10
#define I2C2_SDA_CLK_SOURCE  RCC_AHBPeriph_GPIOA

static uint32_t i2cTimeout;

static volatile uint16_t i2c1ErrorCount = 0;
static volatile uint16_t i2c2ErrorCount = 0;

///////////////////////////////////////////////////////////////////////////////
// I2C TimeoutUserCallback
///////////////////////////////////////////////////////////////////////////////

bool i2cTimeoutUserCallback(I2C_TypeDef *I2Cx)
{
    if (I2Cx == I2C1) {
        i2c1ErrorCount++;
    } else {
        i2c2ErrorCount++;
    }
    return false;
}
//
//static void i2cUnstick(I2C_TypeDef *I2Cx)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//
//    uint8_t i;
//
//    ///////////////////////////////////
//
//    if (I2Cx == I2C1) {
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//
//        GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN;
//        GPIO_Init(I2C1_SCL_GPIO, &GPIO_InitStructure);
//
//        GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
//        GPIO_Init(I2C1_SDA_GPIO, &GPIO_InitStructure);
//
//        GPIO_SetBits(I2C1_SCL_GPIO, I2C1_SCL_PIN);
//        GPIO_SetBits(I2C1_SDA_GPIO, I2C1_SDA_PIN);
//
//        for (i = 0; i < 8; i++) {
//            while (!GPIO_ReadInputDataBit(I2C1_SCL_GPIO, I2C1_SCL_PIN)) // Wait for any clock stretching to finish
//                delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//
//            // Pull low
//            GPIO_ResetBits(I2C1_SCL_GPIO, I2C1_SCL_PIN); // Set bus low
//            delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//            // Release high again
//            GPIO_SetBits(I2C1_SCL_GPIO, I2C1_SCL_PIN); // Set bus high
//            delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//        }
//
//        // Generate a start then stop condition
//
//        GPIO_ResetBits(I2C1_SDA_GPIO, I2C1_SDA_PIN); // Set bus data low
//        delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//        GPIO_ResetBits(I2C1_SCL_GPIO, I2C1_SCL_PIN); // Set bus scl low
//        delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//        GPIO_SetBits(I2C1_SCL_GPIO, I2C1_SCL_PIN); // Set bus scl high
//        delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//        GPIO_SetBits(I2C1_SDA_GPIO, I2C1_SDA_PIN); // Set bus sda high
//        delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//    }
//
/////////////////////////////////////
//
//    if (I2Cx == I2C2) {
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//
//        GPIO_InitStructure.GPIO_Pin = I2C2_SCL_PIN;
//        GPIO_Init(I2C2_SCL_GPIO, &GPIO_InitStructure);
//
//        GPIO_InitStructure.GPIO_Pin = I2C2_SDA_PIN;
//        GPIO_Init(I2C2_SDA_GPIO, &GPIO_InitStructure);
//
//        GPIO_SetBits(I2C2_SCL_GPIO, I2C2_SCL_PIN);
//        GPIO_SetBits(I2C2_SDA_GPIO, I2C2_SDA_PIN);
//
//        for (i = 0; i < 8; i++) {
//            while (!GPIO_ReadInputDataBit(I2C2_SCL_GPIO, I2C2_SCL_PIN)) // Wait for any clock stretching to finish
//                delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//
//            // Pull low
//            GPIO_ResetBits(I2C2_SCL_GPIO, I2C2_SCL_PIN); // Set bus low
//            delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//            // Release high again
//            GPIO_SetBits(I2C2_SCL_GPIO, I2C2_SCL_PIN); // Set bus high
//            delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//        }
//
//        // Generate a start then stop condition
//
//        GPIO_ResetBits(I2C2_SDA_GPIO, I2C2_SDA_PIN); // Set bus data low
//        delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//        GPIO_ResetBits(I2C2_SCL_GPIO, I2C2_SCL_PIN); // Set bus scl low
//        delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//        GPIO_SetBits(I2C2_SCL_GPIO, I2C2_SCL_PIN); // Set bus scl high
//        delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//        GPIO_SetBits(I2C2_SDA_GPIO, I2C2_SDA_PIN); // Set bus sda high
//        delayMicroseconds(3); // 2.5 would be 400 kHz, 3 is 333.33333 kHz
//    }
//
//    ///////////////////////////////////
//}

void i2cInitPort(I2C_TypeDef *I2Cx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    if (I2Cx == I2C1) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        RCC_AHBPeriphClockCmd(I2C1_SCL_CLK_SOURCE | I2C1_SDA_CLK_SOURCE, ENABLE);
        RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);

//        i2cUnstick(I2Cx);                                         // Clock out stuff to make sure slaves arent stuck

        GPIO_PinAFConfig(I2C1_SCL_GPIO, I2C1_SCL_PIN_SOURCE, GPIO_AF_4);
        GPIO_PinAFConfig(I2C1_SDA_GPIO, I2C1_SDA_PIN_SOURCE, GPIO_AF_4);

        GPIO_StructInit(&GPIO_InitStructure);
        I2C_StructInit(&I2C_InitStructure);

        // Init pins

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

        GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN;
        GPIO_Init(I2C1_SCL_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
        GPIO_Init(I2C1_SDA_GPIO, &GPIO_InitStructure);

        I2C_StructInit(&I2C_InitStructure);

        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
        I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
        I2C_InitStructure.I2C_DigitalFilter = 0x00;
        I2C_InitStructure.I2C_OwnAddress1 = 0x00;
        I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
        I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_InitStructure.I2C_Timing = 0x00E0257A; // 400 Khz, 72Mhz Clock, Analog Filter Delay ON, Rise 100, Fall 10.
        //I2C_InitStructure.I2C_Timing              = 0x8000050B;

        I2C_Init(I2C1, &I2C_InitStructure);

        I2C_Cmd(I2C1, ENABLE);

    } else if (I2Cx == I2C2) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
        RCC_AHBPeriphClockCmd(I2C2_SCL_CLK_SOURCE | I2C2_SDA_CLK_SOURCE, ENABLE);
        RCC_I2CCLKConfig(RCC_I2C2CLK_SYSCLK);

//        i2cUnstick(I2Cx);                                         // Clock out stuff to make sure slaves arent stuck

        GPIO_PinAFConfig(I2C2_SCL_GPIO, I2C2_SCL_PIN_SOURCE, GPIO_AF_4);
        GPIO_PinAFConfig(I2C2_SDA_GPIO, I2C2_SDA_PIN_SOURCE, GPIO_AF_4);

        GPIO_StructInit(&GPIO_InitStructure);
        I2C_DeInit(I2Cx);
        I2C_StructInit(&I2C_InitStructure);

        // Init pins
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

        GPIO_InitStructure.GPIO_Pin = I2C2_SCL_PIN;
        GPIO_Init(I2C2_SCL_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = I2C2_SDA_PIN;
        GPIO_Init(I2C2_SDA_GPIO, &GPIO_InitStructure);

        I2C_StructInit(&I2C_InitStructure);

        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
        I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
        I2C_InitStructure.I2C_DigitalFilter = 0x00;
        I2C_InitStructure.I2C_OwnAddress1 = 0x00;
        I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
        I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_InitStructure.I2C_Timing = 0x00E0257A;
        //0x00E0257A; // 400 Khz, 72Mhz Clock, Analog Filter Delay ON, Rise 100, Fall 10.
        //I2C_InitStructure.I2C_Timing              = 0x8000050B;

        I2C_Init(I2C2, &I2C_InitStructure);

        I2C_Cmd(I2C2, ENABLE);
    }
}

void i2cInit(I2C_TypeDef *I2C)
{
    i2cInitPort(I2C);
}

uint16_t i2cGetErrorCounter(I2C_TypeDef* I2Cx)
{
    if (I2Cx == I2C1) {
        return i2c1ErrorCount;
    } else {
        return i2c2ErrorCount;
    }
}

bool i2cWrite(I2C_TypeDef* I2Cx, uint8_t addr_, uint8_t reg, uint8_t data)
{

    /* Test on BUSY Flag */
    i2cTimeout = I2C_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_ISR_BUSY) != RESET) {
        if ((i2cTimeout--) == 0) {
            return i2cTimeoutUserCallback(I2Cx);
        }
    }

    /* Configure slave address, nbytes, reload, end mode and start or stop generation */
    I2C_TransferHandling(I2Cx, addr_, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);

    /* Wait until TXIS flag is set */
    i2cTimeout = I2C_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS) == RESET) {
        if ((i2cTimeout--) == 0) {
            return i2cTimeoutUserCallback(I2Cx);
        }
    }

    /* Send Register address */
    I2C_SendData(I2Cx, (uint8_t)reg);

    /* Wait until TCR flag is set */
    i2cTimeout = I2C_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_ISR_TCR) == RESET) {
        if ((i2cTimeout--) == 0) {
            return i2cTimeoutUserCallback(I2Cx);
        }
    }

    /* Configure slave address, nbytes, reload, end mode and start or stop generation */
    I2C_TransferHandling(I2Cx, addr_, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);

    /* Wait until TXIS flag is set */
    i2cTimeout = I2C_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS) == RESET) {
        if ((i2cTimeout--) == 0) {
            return i2cTimeoutUserCallback(I2Cx);
        }
    }

    /* Write data to TXDR */
    I2C_SendData(I2Cx, data);

    /* Wait until STOPF flag is set */
    i2cTimeout = I2C_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_ISR_STOPF) == RESET) {
        if ((i2cTimeout--) == 0) {
            return i2cTimeoutUserCallback(I2Cx);
        }
    }

    /* Clear STOPF flag */
    I2C_ClearFlag(I2Cx, I2C_ICR_STOPCF);

    return true;
}

bool i2cRead(I2C_TypeDef* I2Cx, uint8_t addr_, uint8_t reg, uint8_t len, uint8_t* buf)
{
    /* Test on BUSY Flag */
    i2cTimeout = I2C_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_ISR_BUSY) != RESET) {
        if ((i2cTimeout--) == 0) {
            return i2cTimeoutUserCallback(I2Cx);
        }
    }

    /* Configure slave address, nbytes, reload, end mode and start or stop generation */
    I2C_TransferHandling(I2Cx, addr_, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

    /* Wait until TXIS flag is set */
    i2cTimeout = I2C_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS) == RESET) {
        if ((i2cTimeout--) == 0) {
            return i2cTimeoutUserCallback(I2Cx);
        }
    }

    if (len > 1) {
        reg |= 0x80;
    }

    /* Send Register address */
    I2C_SendData(I2Cx, (uint8_t)reg);

    /* Wait until TC flag is set */
    i2cTimeout = I2C_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_ISR_TC) == RESET) {
        if ((i2cTimeout--) == 0) {
            return i2cTimeoutUserCallback(I2Cx);
        }
    }

    /* Configure slave address, nbytes, reload, end mode and start or stop generation */
    I2C_TransferHandling(I2Cx, addr_, len, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

    /* Wait until all data are received */
    while (len) {
        /* Wait until RXNE flag is set */
        i2cTimeout = I2C_LONG_TIMEOUT;
        while (I2C_GetFlagStatus(I2Cx, I2C_ISR_RXNE) == RESET) {
            if ((i2cTimeout--) == 0) {
                return i2cTimeoutUserCallback(I2Cx);
            }
        }

        /* Read data from RXDR */
        *buf = I2C_ReceiveData(I2Cx);
        /* Point to the next location where the byte read will be saved */
        buf++;

        /* Decrement the read bytes counter */
        len--;
    }

    /* Wait until STOPF flag is set */
    i2cTimeout = I2C_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2Cx, I2C_ISR_STOPF) == RESET) {
        if ((i2cTimeout--) == 0) {
            return i2cTimeoutUserCallback(I2Cx);
        }
    }

    /* Clear STOPF flag */
    I2C_ClearFlag(I2Cx, I2C_ICR_STOPCF);

    /* If all operations OK */
    return true;
}

#endif
