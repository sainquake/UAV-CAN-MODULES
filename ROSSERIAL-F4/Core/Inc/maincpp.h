/*
 * maincpp.h
 *
 *  Created on: Jul 15, 2021
 *      Author: Ehsan Shaghaei
 */


#define RosserialUart USART3   // choose your USART you prefer to utilize for ROSSERIAL



#ifndef INC_MAINCPP_H_
#define INC_MAINCPP_H_

#ifdef __cplusplus
extern "C"{
#endif

void setup();
void loop();


#ifdef __cplusplus
}
#endif


#endif /* INC_MAINCPP_H_ */
