#include "test.h"

int txid;
int num1;
int num2;
int num3;
int Speed;
int Increment;
int panduan;

/*运行*/

void run(int id, int speed, int position)
{
    // CAN_frame_t rx_frame;
    CAN_frame_t tx_frame;
    tx_frame.FIR.B.FF = CAN_frame_std;
    tx_frame.FIR.B.RTR =CAN_no_RTR;
    tx_frame.MsgID = 0x140 + id;
    tx_frame.FIR.B.DLC = 8;
    tx_frame.data.u8[0] = 0xA4;
    tx_frame.data.u8[1] = 0x00;
    int32_t *p1 = (int32_t *)&tx_frame.data.u8[2];
    int32_t *p2 = (int32_t *)&tx_frame.data.u8[4];
    *p1 = (int32_t)speed;
    *p2 = (int32_t)100 * position;
    ESP32Can.CANWriteFrame(&tx_frame);
}

/*发送指令读取电机运行时的数据
 */
int read_rd(int id, int suoyin)
{

    CAN_frame_t tx_frame;
    int rxdata;
    tx_frame.FIR.B.FF = CAN_frame_std;
    tx_frame.FIR.B.RTR =CAN_no_RTR;
    tx_frame.MsgID = 0x140 + id;
    tx_frame.FIR.B.DLC = 8;
    tx_frame.data.u8[0] = suoyin;
    tx_frame.data.u8[1] = 0x00;
    tx_frame.data.u8[2] = 0x00;
    tx_frame.data.u8[3] = 0x00;
    tx_frame.data.u8[4] = 0x00;
    tx_frame.data.u8[5] = 0x00;
    tx_frame.data.u8[6] = 0x00;
    tx_frame.data.u8[7] = 0x00;
    ESP32Can.CANWriteFrame(&tx_frame);
    unsigned long currentMillis = millis();
    CAN_frame_t rx_frame;
    while (1)
    {
        xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS);
        if (tx_frame.MsgID == rx_frame.MsgID)
        {
            rxdata = *(int *)&rx_frame.data.u8[1];
            break;
        }
    }
    return rxdata;
}
