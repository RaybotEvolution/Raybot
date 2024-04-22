#include "test.h"
#include <DNSServer.h>
#include <ESPUI.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

#if defined(ESP32)
#include <WiFi.h>
#else
// esp8266

#ifndef MMU_IRAM_HEAP
#warning Try MMU option '2nd heap shared' in 'tools' IDE menu (cf. https://arduino-esp8266.readthedocs.io/en/latest/mmu.html#option-summary)
#warning use decorators: { HeapSelectIram doAllocationsInIRAM; ESPUI.addControl(...) ... } (cf. https://arduino-esp8266.readthedocs.io/en/latest/mmu.html#how-to-select-heap)
#warning then check http://<ip>/heap
#endif // MMU_IRAM_HEAP
#ifndef DEBUG_ESP_OOM
#error on ESP8266 and ESPUI, you must define OOM debug option when developping
#endif
#endif

const char *ssid = "2024";
const char *password = "12345678";
const char *hostname = "espressif";

uint16_t status;
uint16_t button1;
uint16_t button2;
uint16_t button3;
uint16_t button4;
uint16_t button5;
uint16_t button6;
uint16_t button7;
uint16_t button8;
uint16_t millisLabelId;
uint16_t switchOne;

void numberCall_1(Control *sender_1, int type_1)
{
    Serial.println(sender_1->value);
    Speed = sender_1->value.toInt();
}
void numberCall_2(Control *sender_2, int type_2)
{
    Serial.println(sender_2->value);
    Increment = sender_2->value.toInt();
}
void numberCall_3(Control *sender_3, int type_3)
{
    Serial.println(sender_3->value);
    Increment = sender_3->value.toInt();
}

void buttonCallback_1(Control *sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        Serial.println("Button_1 DOWN");
        break;

    case B_UP:
        Serial.println("Button_1 UP");
        panduan = 1;
        break;
    }
}

void buttonCallback_2(Control *sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        Serial.println("Button_2 DOWN");
        break;

    case B_UP:
        Serial.println("Button_2 UP");
        panduan = 2;
        break;
    }
}

void buttonCallback_3(Control *sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        Serial.println("Button_3 DOWN");
        break;

    case B_UP:
        Serial.println("Button_3 UP");
        panduan = 3;
        break;
    }
}
void buttonCallback_4(Control *sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        Serial.println("Button_4 DOWN");
        break;

    case B_UP:
        Serial.println("Button_4 UP");
        panduan = 4;
        break;
    }
}
void buttonCallback_5(Control *sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        Serial.println("Button_5 DOWN");
        break;

    case B_UP:
        Serial.println("Button_5 UP");
        panduan = 5;
        break;
    }
}
void buttonCallback_6(Control *sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        Serial.println("Button_6 DOWN");
        break;

    case B_UP:
        Serial.println("Button_6 UP");
        panduan = 6;
        break;
    }
}
void buttonCallback_7(Control *sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        Serial.println("Button_5 DOWN");
        break;

    case B_UP:
        Serial.println("Button_5 UP");
        panduan = 7;
        break;
    }
}void buttonCallback_8(Control *sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        Serial.println("Button_5 DOWN");
        break;

    case B_UP:
        Serial.println("Button_5 UP");
        panduan = 8;
        break;
    }
}

CAN_device_t CAN_cfg;             // CAN Config (CAN配置)
unsigned long previousMillis = 0; // will store last time a CAN Message was send (将存储最后一次发送的CAN消息)
const int interval = 10;          // interval at which send CAN Messages (milliseconds) (发送CAN消息的时间间隔(毫秒))
const int rx_queue_size = 10;     // Receive Queue size (接收队列大小)

void setup()
{
    pinMode(PIN_5V_EN, OUTPUT);
    digitalWrite(PIN_5V_EN, HIGH);

    pinMode(CAN_SE_PIN, OUTPUT);
    digitalWrite(CAN_SE_PIN, LOW);

    // 初始化IO32，开启引脚的输出模式
    pinMode(32, OUTPUT);
    digitalWrite(32, HIGH);

    ESPUI.setVerbosity(Verbosity::VerboseJSON);
    Serial.begin(115200);
    CAN_cfg.speed = CAN_SPEED_1000KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_27;
    CAN_cfg.rx_pin_id = GPIO_NUM_26;
    CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
    // Init CAN Module (初始化CAN模块)
    ESP32Can.CANInit();

#if defined(ESP32)
    WiFi.setHostname(hostname);
#else
    WiFi.hostname(hostname);
#endif

    // try to connect to existing network
    WiFi.begin(ssid, password);
    Serial.print("\n\nTry to connect to existing network");

    {
        uint8_t timeout = 10;

        // Wait for connection, 5s timeout
        do
        {
            delay(500);
            Serial.print(".");
            timeout--;
        } while (timeout && WiFi.status() != WL_CONNECTED);

        // not connected -> create hotspot
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.print("\n\nCreating hotspot");

            WiFi.mode(WIFI_AP);
            delay(100);
            WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
#if defined(ESP32)
            uint32_t chipid = 0;
            for (int i = 0; i < 17; i = i + 8)
            {
                chipid |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
            }
#else
            uint32_t chipid = ESP.getChipId();
#endif
            char ap_ssid[25];
            snprintf(ap_ssid, 26, "ESPUI-%08X", chipid);
            WiFi.softAP(ap_ssid);

            timeout = 5;

            do
            {
                delay(500);
                Serial.print(".");
                timeout--;
            } while (timeout);
        }
    }

    dnsServer.start(DNS_PORT, "*", apIP);

    Serial.println("\n\nWiFi parameters:");
    Serial.print("Mode: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
    Serial.print("IP address: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());
#ifdef ESP8266
    {
        HeapSelectIram doAllocationsInIRAM;
#endif

        ESPUI.addControl(ControlType::Number, "速度:", "0", ControlColor::Sunflower, Control::noParent, &numberCall_1);
        ESPUI.addControl(ControlType::Number, "增量:", "0", ControlColor::Dark, Control::noParent, &numberCall_2);
        button1 = ESPUI.addControl(
            ControlType::Button, " ", "正180", ControlColor::Peterriver, Control::noParent, &buttonCallback_1);
        button2 = ESPUI.addControl(
            ControlType::Button, " ", "负180", ControlColor::Peterriver, Control::noParent, &buttonCallback_2);
        button3 = ESPUI.addControl(
            ControlType::Button, " ", "归0", ControlColor::Peterriver, Control::noParent, &buttonCallback_3);
        button4 = ESPUI.addControl(
            ControlType::Button, " ", "低30", ControlColor::Peterriver, Control::noParent, &buttonCallback_4);
        button5 = ESPUI.addControl(
            ControlType::Button, " ", "抬30", ControlColor::Peterriver, Control::noParent, &buttonCallback_5);
        button6 = ESPUI.addControl(
            ControlType::Button, " ", "左30", ControlColor::Peterriver, Control::noParent, &buttonCallback_6);
        button7 = ESPUI.addControl(
            ControlType::Button, " ", "右30", ControlColor::Peterriver, Control::noParent, &buttonCallback_7);
        button8 = ESPUI.addControl(
            ControlType::Button, " ", "归零", ControlColor::Peterriver, Control::noParent, &buttonCallback_8);

        ESPUI.begin("Demo", "demo", "123456");

#ifdef ESP8266
    } // HeapSelectIram
#endif

    // put your setup code here, to run once: (将您的设置代码放在此处，运行一次：)

    num1 = 0.01 * read_rd(1, 0x92); // 读取1轴位置
    delay(100);
    run(1, 200, num1);
    delay(100);
    num2 = 0.01 * read_rd(2, 0x92); // 读取2轴位置
    delay(100);
    run(2, 200, num2);
    delay(100);
    num3 = 0.01 * read_rd(3, 0x92); // 读取3轴位置
    delay(100);
    run(3, 200, num3);
    delay(1500);
    panduan = -1;
}

void loop()
{
    // put your setup code here, to loop run: (将您的设置代码放在此处，循环运行:)
    dnsServer.processNextRequest();
    switch (panduan)
    {  
    case 1:
        run(1, 500, num1 + 1080); // 正转到180
        delay(100);
        panduan = -1;
        break;
    case 2:
        run(1, 500, num1 - 1080); // 反转到180
        delay(100);
        panduan = -1;
        break;
    case 3:
        run(1, 500, num1); // 转到0
        delay(100);
        panduan = -1;
        break;
      
    case 4:
        run(2, 500, num2 + 180); // 低30
        delay(100);
        panduan = -1;
        break;
       
    case 5:
        run(2, 500, num2); // 转到0
        delay(100);
        panduan = -1;
        break;
     
     case 6:
        run(3, 500, num3 +180); // 转到0
        delay(100);
        panduan = -1;
        break;
       
    case 7:
    
        run(3, 500, num3- 180); // 转到0
        delay(100);
        panduan = -1;
        break;

    case 8:
        run(3, 500, num3); // 转到0
        delay(100);
        panduan = -1;
        break;
    }
}
