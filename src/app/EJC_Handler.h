//
// Created by cheolsoon on 25. 7. 24.
//
#ifndef EJC_H
#define EJC_H

#include <stdint.h>

#include "../app/Log.h"

#include "../common/BaseDef.h"
#include "../common/Time.h"

#include "../hal/GPIO_Handler.h"

int32_t EJC_Init();
int32_t EJC_ARM();
int32_t EJC_DISARM();
int32_t EJC_Op();
int32_t EJC_Op_Force();
int32_t EJC_Close();

#endif // EJC_H