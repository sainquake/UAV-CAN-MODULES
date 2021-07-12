/**
 * @file leds_rgb.c
 * @author d.ponomarev
 * @date March 02, 2021
 * @note
 * WS2812B datasheet: https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf
 * Required config for timer:
 * - timer freq = 72 000 000
 * - timer prescaller = 0
 * - pwm period or 1 bit = 90 timer ticks = 1.25 us:
 * -    25 ticks = 0.347 us
 * -    65 ticks = 0.903 us
 * - 1 led = 24 bits = 30 us
 * - 3 led = 72 bits = 90 us
 */

#include "leds_rgb.h"

#include <stdbool.h>            // For bool


#define BUF_OFFSET          4
#define BITS_PER_SHADE      8
#define PWM_PERIOD_LOW      25
#define PWM_PERIOD_HIGH     65
#define MAX_BUF_SIZE        2 * BUF_OFFSET + MAX_NUM_OF_LEDS * SHADES_PER_LED * BITS_PER_SHADE

static uint8_t LEDS_NUM = 0;
static uint16_t BUF_SIZE = 0;
static uint16_t crc_values[MAX_BUF_SIZE] = {0x00};
static TIM_HandleTypeDef* timer = NULL;
static uint32_t timerChannel = 0;


uint8_t rgbLedsInit(uint8_t numberOfLeds, TIM_HandleTypeDef* timerPtr, uint32_t channel){
    if(numberOfLeds > MAX_NUM_OF_LEDS || timerPtr == NULL){
        LEDS_NUM = 0;
        BUF_SIZE = 0;
        return STATUS_ERROR;
    }
    LEDS_NUM = numberOfLeds;
    BUF_SIZE = 2 * BUF_OFFSET + numberOfLeds * SHADES_PER_LED * BITS_PER_SHADE;
    timer = timerPtr;
    timerChannel = channel;
    return STATUS_OK;
}

/**
  * @note hack:
  * - sometimes RED is PURPLE, it probably means that the first bit was ignored
  * - sometimes BLUS is Yellow, it probably means that the first bit was ignored too
  * - So, we increase buffer size from necessary 96 to 104, by adding 4 bytes at the start and
  * at the end of buffer filled by zeros.
  * This increases stability of leds.
  */
void rgbLedsMapColorToPwm(const Leds_Color_t* ledsColor){
    const uint16_t SHADES_AMOUNT = LEDS_NUM * SHADES_PER_LED;
    for(size_t shade_num = 0; shade_num < SHADES_AMOUNT; shade_num++){
        for(size_t bit_num = 0; bit_num < BITS_PER_SHADE; bit_num++){
            bool is_bit_high = ledsColor->shades[shade_num] >> (7 - bit_num) & 0x01;
            uint16_t crc_value = (is_bit_high) ? PWM_PERIOD_HIGH : PWM_PERIOD_LOW;
            crc_values[BUF_OFFSET + shade_num * BITS_PER_SHADE + bit_num] =  crc_value;
        }
    }
}
void rgbLedsStart(){
    if(timer != NULL){
        HAL_TIM_PWM_Start_DMA(timer, timerChannel, (uint32_t*)crc_values, BUF_SIZE);
    }
}
//uint8_t setColor(Leds_Color_t* ledsColor,uint16_t index,  uint32_t RGB)
//{
//	if(index>MAX_NUM_OF_LEDS-1){
//		return STATUS_ERROR;
//	}
//	ledsColor[index].colors->shades.red   = (uint8_t)((0xff0000 & RGB)>>16);
//	ledsColor[index].colors->shades.green = (uint8_t)((0xff0000 & RGB)>>8);
//	ledsColor[index].colors->shades.blue = (uint8_t)((0xff0000 & RGB));
//	rgbLedsStart();
//	return STATUS_OK;
//}


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
    // is it neccessary in normal (not circular) mode?
    if(htim != NULL && htim == timer){
        HAL_TIM_PWM_Stop_DMA(timer, timerChannel);
    }
}
