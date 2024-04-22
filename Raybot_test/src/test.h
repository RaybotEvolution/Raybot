#ifndef __TEST_H__
#define __TEST_H__
#include <Arduino.h>
#include "config.h" //引脚配置
#include <HardwareSerial.h>
#include <ESP32CAN.h>
#include <CAN_config.h>
#include <SPI.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int txid;
extern int num1;
extern int num2;
extern int num3;
extern int Speed;
extern int Increment;
extern int panduan;

void run(int, int, int); // 运行
int read_rd(int, int);   // 读取位置

#endif