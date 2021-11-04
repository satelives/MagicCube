/**
 * @file    display.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-20
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/

#ifndef __DISPLAY_H
#define __DISPLAY_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"

/* Exported constants -------------------------------------------------------------------*/
#define LCD_BL_PIN 5
#define LCD_BL_PWM_CHANNEL 0

/* Exported types -----------------------------------------------------------------------*/
class Display
{
private:

public:
    void init();
    void process();
    void set_rotation(uint16_t rotation);
    // void setBackLight(float);
};

/* Exported variables -------------------------------------------------------------------*/
/* Exported functions -------------------------------------------------------------------*/


#endif