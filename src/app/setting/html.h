/**
 * @file    html.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-10-25
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/

#ifndef __HTML_H
#define __HTML_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"

/* Exported constants -------------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------------------*/
extern String webpage;
extern String webpage_header;
extern String webpage_footer;

/* Exported functions -------------------------------------------------------------------*/
void init_page_header();
void init_page_footer();
String get_wifi_page();
String get_reboot_page();

#endif