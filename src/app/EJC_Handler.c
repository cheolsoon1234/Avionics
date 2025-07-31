//
// Created by cheolsoon on 25. 7. 24.
//

#include "EJC_Handler.h"

int32_t EJC_Init() {
    GPIO_Export(EJC_PIN);
    GPIO_Export(EJC_LED_PIN);
    usleep(1000);
    GPIO_Set_Direction(EJC_PIN, "out");
    GPIO_Set_Direction(EJC_LED_PIN, "out");
    return 1;
}

int32_t EJC_Close() {
    GPIO_Unexport(EJC_PIN);
    GPIO_Unexport(EJC_LED_PIN);
    LOG_WRITE_WITH_TIME("Ejection Pins Unexported");
    return 1; 
}

int32_t EJC_ARM() {
    EJC_ARM_FLAG = 1;
    GPIO_Write(EJC_LED_PIN, 1);
    LOG_WRITE_WITH_TIME("Parachute Ejection Armed");
    return EJC_ARM_FLAG;
}

int32_t EJC_DISARM() {
    EJC_ARM_FLAG = 0;
    GPIO_Write(EJC_LED_PIN, 0);
    LOG_WRITE_WITH_TIME("Parachute Ejection DisArmed");
    return EJC_ARM_FLAG;
}

int32_t EJC_Op() {

    if (EJC_ARM_FLAG) {
        LOG_WRITE_WITH_TIME("Parachute Ejection Operated");
        GPIO_Write(EJC_PIN, 1);
        EJC_FLAG = 1;
    } else {
        LOG_WRITE_WITH_TIME("EJC System is Not Armed");
    }
    
    return 0;
}

int32_t EJC_Op_Force() {

    LOG_WRITE_WITH_TIME("Parachute Ejection Operated By USER");
    GPIO_Write(EJC_PIN, 1);
    
    EJC_FLAG = 1;
    
    return 0;
}

