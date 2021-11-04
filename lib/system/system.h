/**
 * @file    system.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-08-27
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/
#ifndef __SYSTEM_H
#define __SYSTEM_H

/* Includes -----------------------------------------------------------------------------*/
// #include "stdio.h"
// #include "stdint.h"
// #include "stdlib.h"
// #include "stdbool.h"
// #include "stdarg.h"
// #include "string.h"

#include "gt_define.h"
#include <Arduino.h>

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported constants -------------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------------------*/
extern volatile gt_system_t gt_system;

/* Exported functions -------------------------------------------------------------------*/

int32_t gt_vsnprintf(char *buf, uint32_t size, const char *fmt, va_list args);
int32_t gt_snprintf(char *buf, uint32_t size, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
