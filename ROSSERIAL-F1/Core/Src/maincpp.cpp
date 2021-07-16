/*
 * maincpp.cpp
 *
 *  Created on: Jul 16, 2021
 *      Author: Ehsan Shaghaei
 *      github.com/ehsan2754
 */


#include <maincpp.h>
#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle nh;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);
char hello[] = "Hello world!";

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->reset_rbuf();
}

void setup(void)
{
  nh.initNode();
  nh.advertise(chatter);
}

void loop(void)
{
  HAL_Delay(500);
  HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_4);

  str_msg.data = hello;
  chatter.publish(&str_msg);
  nh.spinOnce();

  HAL_Delay(500);
}
