/**
 * @file    genius.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-11-02
 *
 * (C) COPYRIGHT 2021 chery.zhang
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/

#ifndef __GENIUS_H
#define __GENIUS_H

/* Includes -----------------------------------------------------------------------------*/
#include "system.h"
#include <WiFiClient.h>

/* Exported constants -------------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------------------*/

class GENIUS
{
private:
    bool Connected = false;
    WiFiClient TCPclient;
    String recieved_msg = "";

public:
    size_t send_msg(String msg);
    bool isConnected(void);
    bool connect(void);
    void process(void);
};


/* Exported functions -------------------------------------------------------------------*/

#endif