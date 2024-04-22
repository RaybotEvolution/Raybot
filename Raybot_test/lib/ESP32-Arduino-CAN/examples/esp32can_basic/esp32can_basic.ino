/*
 * @Author: your name
 * @Date: 2021-10-06 11:26:15
 * @LastEditTime: 2021-11-11 17:43:55
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \ESP32-Arduino-CAN\examples\esp32can_basic\esp32can_basic.ino
 */
#include <Arduino.h>
#include <ESP32CAN.h>
#include <CAN_config.h>

CAN_device_t CAN_cfg;             // CAN Config
unsigned long previousMillis = 0; // will store last time a CAN Message was send
const int interval = 1000;        // interval at which send CAN Messages (milliseconds)
const int rx_queue_size = 10;     // Receive Queue size

void setup()
{

  pinMode(16, OUTPUT);
  digitalWrite(16, 1);

  pinMode(23, OUTPUT);
  digitalWrite(23, 0);

  Serial.begin(115200);
  Serial.println("Basic Demo - ESP32-Arduino-CAN");
  CAN_cfg.speed = CAN_SPEED_1000KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_27;
  CAN_cfg.rx_pin_id = GPIO_NUM_26;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // Init CAN Module
  ESP32Can.CANInit();

  Serial.print("CAN SPEED :");
  Serial.println(CAN_cfg.speed);
}

void loop()
{

  CAN_frame_t rx_frame;

  unsigned long currentMillis = millis();

  // Receive next CAN frame from queue
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE)
  {

    if (rx_frame.FIR.B.FF == CAN_frame_std)
    {
      printf("New standard frame");
    }
    else
    {
      printf("New extended frame");
    }

    if (rx_frame.FIR.B.RTR == CAN_RTR)
    {
      printf(" RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID, rx_frame.FIR.B.DLC);
    }
    else
    {
      printf(" from 0x%08X, DLC %d, Data ", rx_frame.MsgID, rx_frame.FIR.B.DLC);
      for (int i = 0; i < rx_frame.FIR.B.DLC; i++)
      {
        printf("0x%02X ", rx_frame.data.u8[i]);
      }
      printf("\n");
    }
  }
  // Send CAN Message
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    CAN_frame_t tx_frame;
    tx_frame.FIR.B.FF = CAN_frame_std;
    tx_frame.MsgID = 0x001;
    tx_frame.FIR.B.DLC = 8;
    tx_frame.data.u8[0] = 0x00;
    tx_frame.data.u8[1] = 0x01;
    tx_frame.data.u8[2] = 0x02;
    tx_frame.data.u8[3] = 0x03;
    tx_frame.data.u8[4] = 0x04;
    tx_frame.data.u8[5] = 0x05;
    tx_frame.data.u8[6] = 0x06;
    tx_frame.data.u8[7] = 0x07;

    ESP32Can.CANWriteFrame(&tx_frame);
    Serial.println("CAN");
  }
}
