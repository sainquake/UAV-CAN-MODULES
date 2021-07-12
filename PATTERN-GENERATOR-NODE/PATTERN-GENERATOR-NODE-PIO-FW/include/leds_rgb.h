/**
 * @file leds_rgb.h
 * @author d.ponomarev
 * @date March 02, 2021
 */

#ifndef INC_LEDS_RGB_H_
#define INC_LEDS_RGB_H_

#include <stm32f1xx_hal.h>      // For TIM_HandleTypeDef
#include <stdint.h>             // For sizes

#define MAX_NUM_OF_LEDS     80
#define SHADES_PER_LED      3
#define STATUS_ERROR                -1
#define STATUS_OK                   0

typedef union{
    struct{
        uint8_t green;
        uint8_t red;
        uint8_t blue;
    }shades;
    uint8_t raw[SHADES_PER_LED];
}Color_t;

typedef union{
    Color_t colors[MAX_NUM_OF_LEDS];
    uint8_t shades[MAX_NUM_OF_LEDS * SHADES_PER_LED];
}Leds_Color_t;

uint8_t rgbLedsInit(uint8_t numberOfLeds, TIM_HandleTypeDef* timerPtr, uint32_t channel);
void rgbLedsMapColorToPwm(const Leds_Color_t* ledsColor);
void rgbLedsStart();
//uint8_t setColor(Leds_Color_t* ledsColor,uint16_t index,  uint32_t RGB);

#endif /* INC_LEDS_RGB_H_ */
