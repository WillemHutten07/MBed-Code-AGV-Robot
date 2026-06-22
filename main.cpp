// #include <mbed.h>
// #include <VL53L0X.h>
// #include <string>

// I2C i2c(D14, D15);
// BufferedSerial bluetooth(PA_11, PA_12, 9600);
// Timer t1;

// int linksCounter = 0;
// int rechtsCounter = 0;
// uint16_t obstakelAfstand = 300; //200 mm
// float brightness = 0.5;

// enum Richting
// {
//     Links,
//     Rechts
// };

// //Reset pins op VL53L0X ToF sensor
// DigitalOut xshut1(PC_6);
// DigitalOut xshut2(PC_8);
// DigitalOut xshut3(PC_5);
// DigitalOut xshut4(PC_9);

// // Default en nieuwe address
// #define DEFAULT_ADDR    0x29
// #define SENSORL_ADDR    0x30
// #define SENSORLF_ADDR   0x31
// #define SENSORRF_ADDR   0x32
// #define SENSORR_ADDR    0x33

// // #define HIGH_ACCURACY
// #define LONG_RANGE

// //---- ToF Afstandssensoren ----
// VL53L0X sensorL(i2c, t1);
// VL53L0X sensorLF(i2c, t1);
// VL53L0X sensorRF(i2c, t1);
// VL53L0X sensorR(i2c, t1);

// // ---- IR-sensoren afgrond ----
// DigitalIn ir1(D12, PullDown); // L
// DigitalIn ir2(PC_10, PullDown); // LF
// DigitalIn ir3(A2, PullDown);  // RF
// DigitalIn ir4(A4, PullDown); // R

// // ---- IR-sensoren zwarte lijn ----
// DigitalIn ir5(PB_1, PullDown); // LF
// DigitalIn ir6(PB_2, PullDown); // RF

// //----Motoren----
// DigitalOut in1(D2);
// DigitalOut in2(D4);
// DigitalOut in3(D7);
// DigitalOut in4(D8);

// // DigitalOut ena(x); // Voor PWM

// //----RGB LEDS ----PWM pins
// PwmOut ledL_rood(D3); // D3
// PwmOut ledL_groen(D5); // D5
// PwmOut ledLF_rood(D6); // D6
// PwmOut ledLF_groen(D10); // D10F
// PwmOut ledRF_rood(D11); // D11
// PwmOut ledRF_groen(A0); // A0
// PwmOut ledR_rood(A1); // A1
// PwmOut ledR_groen(A3); // A3

// void scanI2C()
// {
//     printf("Scanning I2C...\r\n");
//     for (int addr = 1; addr < 127; addr++)
//     {
//         if (!i2c.write(addr << 1, nullptr, 0))
//         {
//             printf("Device found at 0x%02X\r\n", addr);
//         }
//     }
// }

// // ----Detectie----
// bool obstakelDetectie(uint16_t L, uint16_t LF, uint16_t RF, uint16_t R)
// {
//     return (L < obstakelAfstand ||LF < obstakelAfstand ||RF < obstakelAfstand ||R < obstakelAfstand);
// }

// bool afgrond_detectie()
// {
//     return (ir1.read() == 1) || (ir2.read() == 1) || (ir3.read() == 1) || (ir4.read() == 1);
// }

// bool zwarte_lijn_detectie()
// {
//     return (ir5.read() == 1) || (ir6.read() == 1);
// }

// // ----Motoren----

// void vooruitRijden()
// {
//     printf("Rijdt vooruit\n");
//         in1 = 0; // Achteruit
//         in2 = 1; // Vooruit L
//         in3 = 1; // Vooruit R
//         in4 = 0; // Achteruit
// }

// void draaiLinks()
// {
//     printf("draai links. Motor 1 uit of heel zacht(pwm) motor 2 volledig laten draaien\n");
//     in1 = 0;
//     in2 = 1;
//     in3 = 0;
//     in4 = 1;
// }

// void draaiRechts()
// {
//     printf("draai rechts. Motor 2 uit of heel zacht(pwm) motor 1 volledig laten draaien\n");
//     in1 = 1;
//     in2 = 0;
//     in3 = 1;
//     in4 = 0;
// }

// void stopMotoren()
// {
//     printf("Stop motoren\n");
//     in1 = 0;
//     in2 = 0;
//     in3 = 0;
//     in4 = 0;
// }

// void achteruitRijden()
// {
//     printf("Rijdt achteruit\n");
//     in1 = 1;
//     in2 = 0;
//     in3 = 0;
//     in4 = 1;
// }

// Richting kiesRichting()
// {
//     if (linksCounter > rechtsCounter)
//     {
//         rechtsCounter++;
//         return Rechts;
//     }

//     else if (rechtsCounter > linksCounter)
//     {
//         linksCounter++;
//         return Links;
//     }
//     //Standaardrichting wanneer counters gelijk zijn
//     else 
//     {
//         linksCounter++;
//         return Links;
//     }
// }

// void voerRichtingUit(Richting richting)
// {
//     if (richting == Links)
//     {
//         draaiLinks();
//     }
//     else
//     {
//         draaiRechts();
//     }
// }

// void handleAfgrond()
// {
//     stopMotoren();
//     thread_sleep_for(100);
//     achteruitRijden();
//     thread_sleep_for(5000);

//     stopMotoren();
//     thread_sleep_for(500);

//     voerRichtingUit(kiesRichting());
// }

//   bool sensor_L = false;
//   bool sensor_LF = false;
//   bool sensor_RF = false;
//   bool sensor_R = false;

// int main()
// {
//     printf("VL53L0X sensors initieren...\r\n");

//     xshut1 = 0;
//     xshut2 = 0;
//     xshut3 = 0;
//     xshut4 = 0;

//     ledL_rood.period_ms(1);
//     ledLF_rood.period_ms(1);
//     ledRF_rood.period_ms(1);
//     ledR_rood.period_ms(1);
//     ledL_groen.period_ms(1);
//     ledLF_groen.period_ms(1);
//     ledRF_groen.period_ms(1);
//     ledR_groen.period_ms(1);
    
//     thread_sleep_for(50);
//     printf("Beginnen met sensor 1: \r\n");

//     // Sensor Links
//     xshut1 = 1;
//     thread_sleep_for(10);
//     printf("X-shut 1 veranderd\r\n");
  

//     if (sensorL.init())
//     {
//     sensor_L = true;
//     sensorL.setTimeout(500);
//     sensorL.setAddress(SENSORL_ADDR);
//     printf("Address sensor Links changed to 0x%02X\r\n", SENSORL_ADDR);
//     }
//     else if (!sensor_L)
//     {
//         printf("Kon linker sensor niet initieren\n");
//     }
    
//     // Sensor links-voor
//     xshut2 = 1;
//     thread_sleep_for(10);
    
//     if (sensorLF.init())
//     {
//     sensor_LF = true;
//     sensorLF.setTimeout(500);
//     sensorLF.setAddress(SENSORLF_ADDR);
//     printf("Address sensor Links-voor changed to 0x%02X\r\n", SENSORLF_ADDR);
//     }
//     else if (!sensor_LF) 
//     {
//     printf("Kon sensor Links-voor niet initieren\n");
//     }

//     // Sensor Rechts-voor
//     xshut3 = 1;
//     thread_sleep_for(10);

//     if (sensorRF.init())
//     {
//     sensor_RF = true;
//     sensorRF.setTimeout(500);
//     sensorRF.setAddress(SENSORRF_ADDR);
//     thread_sleep_for(10);
//     printf("Address sensor Rechts changed to 0x%02X\r\n", SENSORRF_ADDR);
//     }
//     else if (!sensor_RF)
//     {
//     printf("Kon sensor Rechts-voor niet initieren\n");
//     }

//     // // Sensor rechts
//     xshut4 = 1;
//     thread_sleep_for(10);

//     if (sensorR.init())
//     {
//     sensor_R = true;
//     sensorR.setTimeout(500);
//     sensorR.setAddress(SENSORR_ADDR);
//     thread_sleep_for(10);
//     printf("Address sensor Rechts-voor changed to 0x%02X\r\n", SENSORR_ADDR);
//     }
//     else if(!sensor_R) 
//     {
//     printf("kon sensor Rechts niet initieren\n");
//     }

//     scanI2C();

//     char c;
//     string received = "";
//     Timer stringtimer;
//     stringtimer.start();

//     printf("Bluetooth HC-05 Ready\r\n");

//     while (1)
//     {
//         ledL_groen.write(brightness);
//         ledLF_groen.write(brightness);
//         ledRF_groen.write(brightness);
//         ledR_groen.write(brightness);
        
//         ledL_rood.write(1.0);
//         ledLF_rood.write(1.0);
//         ledRF_rood.write(1.0);
//         ledR_rood.write(1.0);

//         uint16_t distL = sensorL.readRangeSingleMillimeters();
//         uint16_t distLF = sensorLF.readRangeSingleMillimeters();
//         uint16_t distRF = sensorRF.readRangeSingleMillimeters();
//         uint16_t distR = sensorR.readRangeSingleMillimeters();

//         // printf("Afstand Links: %u mm @ address 0x%02X\r\n", distL, SENSORL_ADDR);
//         // printf("Afstand Links-Voor: %u mm @ address 0x%02X\r\n", distLF, SENSORLF_ADDR);
//         // printf("Afstand Rechts-Voohr: %u mm @ address 0x%02X\r\n", distRF, SENSORRF_ADDR);
//         // printf("Afstand Rects: %u mm @ address 0x%02X\r\n", distR, SENSORR_ADDR);
//         printf("Afstand Links: %u mm Links-Voor: %u mm Rechts-voor: %u mm Rechts: %u mm \r\n", distL, distLF, distRF, distR);

//        if (afgrond_detectie())
//        {
//            handleAfgrond();
//        }

//         else if (obstakelDetectie(distL, distLF, distRF, distR))
//         {
//             if (distL < obstakelAfstand)
//             {
//                 ledL_groen.write(1.0);
//                 ledL_rood.write(brightness);
//             }
//             if (distR < obstakelAfstand)
//             {
//                 ledR_groen.write(1.0);
//                 ledR_rood.write(brightness);
//             }
//             if (distLF < obstakelAfstand || distRF < obstakelAfstand)
//             {
//                 ledLF_groen.write(1.0);
//                 ledLF_rood.write(brightness);
//                 ledRF_groen.write(1.0);
//                 ledRF_rood.write(brightness);
//             }

//             // voerRichtingUit(kiesRichting());
//             Richting richting = kiesRichting();

//             while (obstakelDetectie(sensorL.readRangeSingleMillimeters(), sensorLF.readRangeSingleMillimeters(), sensorRF.readRangeSingleMillimeters(), sensorR.readRangeSingleMillimeters()))
//             {
//             voerRichtingUit(richting);
//             thread_sleep_for(10);
//             }
//         }

//         else
//         {
//             ledL_groen.write(brightness);
//             ledL_rood.write(1.0);
//             ledLF_groen.write(brightness);
//             ledLF_rood.write(1.0);
//             ledRF_groen.write(brightness);
//             ledRF_rood.write(1.0);
//             ledR_groen.write(brightness);
//             ledR_rood.write(1.0);
//             vooruitRijden();
//         }

//         if (bluetooth.readable())
//         {
//             if (bluetooth.read(&c, 1) == 1)
//             {
//                 bluetooth.write(&c, 1);
//                 received += c;
//                 stringtimer.reset();  // reset timer on every new character
//             }
//         }

//         // If we have data and no new characters for 50ms, process it
//         if (received.length() > 0 && stringtimer.elapsed_time() > 50ms)
//         {
//             printf("Got: %s\r\n", received.c_str());

//             if (received == "a")
//             {
//                 printf("AGV Aan\r\n");
//             }
//             else if (received == "u")
//             {
//                 printf("AGV Uit\r\n");
//             }
//             else if (received == "v")
//             {
//                 printf("Vooruit\r\n");
//                 vooruitRijden();
//                 thread_sleep_for(1000);
//                 stopMotoren();
//             }
//             else if (received == "l")
//             {
//                 printf("Naar Links\r\n");
//                 draaiLinks();
//                 thread_sleep_for(1000);
//                 stopMotoren();
//             }
//             else if (received == "r")
//             {
//                 printf("Naar Rechts\r\n");
//                 draaiRechts();
//                 thread_sleep_for(1000);
//                 stopMotoren();
//             }
//             else if (received == "s")
//             {
//                 printf("Achteruit\r\n");
//                 achteruitRijden();
//                 thread_sleep_for(1000);
//                 stopMotoren();
//             }
//             else if (received.substr(0, 4) == "LED_")
//             {
//                 int value = stoi(received.substr(4));
//                 brightness = value / 10.0f;
//                 printf("Value: %.2f\r\n", brightness);
//             }

//             received = "";  // clear for next message
//         }    

//         if (sensorL.timeoutOccurred()||sensorLF.timeoutOccurred()||sensorRF.timeoutOccurred()||sensorR.timeoutOccurred())
//         {
//             printf("TIMEOUT!\r\n");
//             stopMotoren();
//         }
//         thread_sleep_for(10);
//     }
// }

// ----LED Test----

// #include <mbed.h>
// #include <VL53L0X.h>

// PwmOut led1_rood(D3);
// PwmOut led1_groen(D5);
// PwmOut led2_rood(D6);
// PwmOut led2_groen(D9);

// AnalogIn potmeter(A0);


// float value = 0;

// int main()
// {
//     led1_rood.period_ms(1);
//     led1_groen.period_ms(1);
//     led2_rood.period_ms(1);
//     led2_groen.period_ms(1);

//     potmeter.set_reference_voltage(3.3f);

//     led1_rood = true;
//     led1_groen = true;
//     led2_rood = true;
//     led2_groen = true;


//     while (true)
//     {
//         // value = potmeter.read();

//         // led1_rood.write(value);
//         led1_groen.write(1.0);
//         led1_rood.write(0.0);
//         printf("Zou nu rood moeten zijn\n");
//         thread_sleep_for(3000);

//         led1_rood.write(1.0);
//         led1_groen.write(0.0);
//         printf("Zou nu groen moeten zijn\n");
//         thread_sleep_for(500);
//     }
// }

// #include "mbed.h"

// // Pas de pinnen aan indien nodig
// DigitalOut in1(D2);
// DigitalOut in2(D4);
// DigitalOut in3(D7);
// DigitalOut in4(D8);
// // DigitalOut ena(D5);

// int main()
// {
//     // ena = true;

//     while (true)
//     {
//         // Vooruit
//         in1 = 1;
//         in2 = 0;
//         in3 = 1;
//         in4 = 0;
//         ThisThread::sleep_for(20s);

//         // Stop
//         in1 = 0;
//         in2 = 0;
//         in3 = 0;
//         in4 = 0;
//         ThisThread::sleep_for(1s);

//         // Achteruit
//         in1 = 0;
//         in2 = 1;
//         in3 = 0;
//         in4 = 0;
//         ThisThread::sleep_for(20s);

//         // Stop
//         in1 = 0;
//         in2 = 0;
//         in3 = 0;
//         in4 = 0;
//         ThisThread::sleep_for(1s);
//     }
// }
// #include <mbed.h>
// #include <VL53L0X.h>

// I2C i2c(D14, D15);
// Timer t1;

// VL53L0X sensorL(i2c, t1);
// VL53L0X sensorLF(i2c, t1);
// VL53L0X sensorRF(i2c, t1);
// VL53L0X sensorR(i2c, t1);

// #define DEFAULT_ADDR    0x29
// #define SENSORL_ADDR    0x30
// #define SENSORLF_ADDR   0x31
// #define SENSORRF_ADDR   0x32
// #define SENSORR_ADDR    0x33

// // #define HIGH_ACCURACY
// #define LONG_RANGE

// DigitalOut xshut1(D3);
// DigitalOut xshut2(D4);
// DigitalOut xshut3(D5);
// DigitalOut xshut4(D6);


// int main()
// {
//     xshut1 = 0;
//     xshut2 = 0;
//     xshut3 = 0;
//     xshut4 = 0;

//     printf("initieren\n");
//     xshut1 = 1;
//     thread_sleep_for(100);
//     sensorL.init();
//     sensorL.setTimeout(500);
//     sensorL.setAddress(SENSORL_ADDR);
//     printf("Initieren sensor1 gereed\n");

//     thread_sleep_for(100);
//     xshut2 = 1;
//     thread_sleep_for(100);
//     sensorLF.init();
//     sensorLF.setTimeout(500);
//     sensorLF.setAddress(SENSORLF_ADDR);
//     printf("Initieren sensor2 gereed\n");
    
//     thread_sleep_for(100);
//     xshut3 = 1;
//     thread_sleep_for(100);
//     sensorRF.init();
//     sensorRF.setTimeout(500);
//     sensorRF.setAddress(SENSORRF_ADDR);
//     printf("Initieren sensor3 gereed\n");

//     thread_sleep_for(100);
//     xshut4 = 1;
//     thread_sleep_for(100);
//     sensorR.init();
//     sensorR.setTimeout(500);
//     sensorR.setAddress(SENSORR_ADDR);
//     printf("Initieren sensor4 gereed\n");

//     thread_sleep_for(100);

//     while (1)
//     {
//         uint16_t distL = sensorL.readRangeSingleMillimeters();
//         uint16_t distLF = sensorLF.readRangeSingleMillimeters();
//         uint16_t distRF = sensorRF.readRangeSingleMillimeters();
//         uint16_t distR = sensorR.readRangeSingleMillimeters();

//         printf("Afstand Links: %u mm @ address 0x%02X\r\n", distL, SENSORL_ADDR);
//         printf("Afstand Links-Voor: %u mm @ address 0x%02X\r\n", distLF, SENSORLF_ADDR);
//         printf("Afstand Rechts-Voor: %u mm @ address 0x%02X\r\n", distRF, SENSORRF_ADDR);
//         printf("Afstand Rechts: %u mm @ address 0x%02X\r\n", distR, SENSORR_ADDR);
//         thread_sleep_for(500);
//     }
// }

// #include "mbed.h"
// #include <string>

// BufferedSerial bluetooth(PA_11, PA_12, 9600);
// DigitalOut led(LED1);

// int main()
// {
//     char c;
//     string received = "";
//     Timer stringtimer;
//     stringtimer.start();

//     printf("Bluetooth HC-05 Ready\r\n");

//     while (true)
//     {
//         if (bluetooth.readable())
//         {
//             if (bluetooth.read(&c, 1) == 1)
//             {
//                 bluetooth.write(&c, 1);
//                 received += c;
//                 stringtimer.reset();  // reset timer on every new character
//             }
//         }

//         // If we have data and no new characters for 50ms, process it
//         if (received.length() > 0 && stringtimer.elapsed_time() > 50ms)
//         {
//             printf("Got: %s\r\n", received.c_str());

//             if (received == "a")
//             {
//                 led = 1;
//                 printf("AGV Aan\r\n");
//             }
//             else if (received == "u")
//             {
//                 led = 0;
//                 printf("AGV Uit\r\n");
//             }
//             else if (received == "v")
//             {
//                 printf("Vooruit\r\n");
//             }
//             else if (received == "l")
//             {
//                 printf("Naar Links\r\n");
//             }
//             else if (received == "r")
//             {
//                 printf("Naar Rechts\r\n");
//             }
//             else if (received == "s")
//             {
//                 printf("Achteruit\r\n");
//             }
//             else if (received.substr(0, 4) == "LED_")
//             {
//                 int value = stoi(received.substr(4));
//                 float brightness = value / 10.0f;
//                 printf("Value: %.2f\r\n", brightness);
//             }

//             received = "";  // clear for next message
//         }
//     }
// }

// ----IR Test ----
// #include <mbed.h>

// DigitalIn ir1(D12, PullDown); // L
// DigitalIn ir2(PC_10, PullDown); // LF
// DigitalIn ir3(PC_11, PullDown);  // RF
// DigitalIn ir4(A4, PullDown); // R

// // ---- IR-sensoren zwarte lijn ----
// DigitalIn ir5(PB_1); // LF
// DigitalIn ir6(PB_2); // RF

// int main()
// {
//     while (true)
//     {
//     int afgrondL = ir1.read();
//     int afgrondLF = ir2.read();
//     int afgrondRF= ir3.read();
//     int afgrondR = ir4.read();
//     // int zwartelijn1= ir5.read();
//     // int zwartelijn2 = ir6.read();

//     printf("Afgrond L: %d\n", afgrondL);
//     printf("Afgrond LF: %d\n", afgrondLF);
//     printf("Afgrond RF: %d\n", afgrondRF);
//     printf("Afgrond R: %d\n", afgrondR);
//     // printf("zwarte lijn 1: %d\n", zwartelijn1);
//     // printf("zwarte lijn 2: %d\n", zwartelijn2);
//     thread_sleep_for(500);
//     }
// }


//--------EINDCODE--------


#include <cstdint>
#include <mbed.h>
#include <VL53L0X.h>
#include <string>

I2C i2c(D14, D15);
BufferedSerial bluetooth(PA_11, PA_12, 9600);
Timer t1;

int linksCounter = 0;
int rechtsCounter = 0;
uint16_t obstakelAfstand = 350; //200 mm
float brightness = 0.5;
Timer zwartTimer;
bool zwarte_lijn_cooldown = false;

enum Richting
{
    Links,
    Rechts
};

//Reset pins op VL53L0X ToF sensor
DigitalOut xshut1(PC_6);
DigitalOut xshut2(PC_8);
DigitalOut xshut3(PC_5);
DigitalOut xshut4(PC_9);

// Default en nieuwe address
#define DEFAULT_ADDR    0x29
#define SENSORL_ADDR    0x30
#define SENSORLF_ADDR   0x31
#define SENSORRF_ADDR   0x32
#define SENSORR_ADDR    0x33

// #define HIGH_ACCURACY
#define LONG_RANGE

//---- ToF Afstandssensoren ----
VL53L0X sensorL(i2c, t1);
VL53L0X sensorLF(i2c, t1);
VL53L0X sensorRF(i2c, t1);
VL53L0X sensorR(i2c, t1);

// ---- IR-sensoren afgrond ----
DigitalIn ir1(D12, PullDown); // L
DigitalIn ir2(PC_10, PullDown); // LF
DigitalIn ir3(PC_11, PullDown);  // RF
DigitalIn ir4(A4, PullDown); // R

// ---- IR-sensoren zwarte lijn ----
DigitalIn ir5(PB_1, PullDown); // LF
DigitalIn ir6(PB_2, PullDown); // RF

//----Motoren----
DigitalOut in1(D2);
DigitalOut in2(D4);
DigitalOut in3(D7);
DigitalOut in4(D8);

// DigitalOut ena(x); // Voor PWM

//----RGB LEDS----PWM pins
PwmOut ledL_rood(D9); // D3
PwmOut ledL_groen(D5); // D5
PwmOut ledLF_rood(D6); // D6
PwmOut ledLF_groen(D10); // D10F
PwmOut ledRF_rood(D11); // D11
PwmOut ledRF_groen(A0); // A0
PwmOut ledR_rood(A1); // A1
PwmOut ledR_groen(A3); // A3

//----Aan/Uit knop----
InterruptIn t1_knop(D3); //Pull-down knop
bool agv_aan = false;
Timer debounce;
bool gestart = false;

void knop_ingedrukt()
{
    if (debounce.elapsed_time() < 200ms) return;

    agv_aan = !agv_aan;
    debounce.reset();
}

void scanI2C()
{
    printf("I2C Scannen...\r\n");
    for (int addr = 1; addr < 127; addr++)
    {
        if (!i2c.write(addr << 1, nullptr, 0))
        {
            printf("Sensor gevonden: 0x%02X\r\n", addr);
        }
    }
}

// ----Detectie----
bool obstakelDetectie(uint16_t L, uint16_t LF, uint16_t RF, uint16_t R)
{
    return (L < obstakelAfstand ||LF < obstakelAfstand ||RF < obstakelAfstand ||R < obstakelAfstand);
}

bool afgrond_detectie()
{
    return (ir1.read() == 1) || (ir2.read() == 1) || (ir3.read() == 1) || (ir4.read() == 1);
}

bool zwarte_lijn_detectie()
{
    return (ir5.read() == 1) || (ir6.read() == 1);
}

// ----Motoren----

void vooruitRijden()
{
    // printf("Rijdt vooruit\n");
        in1 = 0; // Achteruit
        in2 = 1; // Vooruit L
        in3 = 1; // Vooruit R
        in4 = 0; // Achteruit
}

void achteruitRijden()
{
    // printf("Rijdt achteruit\n");
        in1 = 1; // Achteruit
        in2 = 0; // Vooruit L
        in3 = 0; // Vooruit R
        in4 = 1; // Achteruit
}

void draaiLinks()
{
    // printf("draai links. Motor 1 achteruit motor 2 vooruit\n");
    in1 = 0;
    in2 = 1;
    in3 = 0;
    in4 = 1;
}

void draaiRechts()
{
    // printf("draai rechts. Motor 2 achteruit motor 1 vooruit\n");
    in1 = 1;
    in2 = 0;
    in3 = 1;
    in4 = 0;
}

void stopMotoren()
{
    // printf("Stop motoren\n");
    in1 = 0;
    in2 = 0;
    in3 = 0;
    in4 = 0;
}

Richting kiesRichting()
{
    if (linksCounter > rechtsCounter)
    {
        rechtsCounter++;
        return Rechts;
    }

    else if (rechtsCounter > linksCounter)
    {
        linksCounter++;
        return Links;
    }
    //Standaardrichting wanneer counters gelijk zijn
    else 
    {
        linksCounter++;
        return Links;
    }
}

void updateLEDs(uint16_t distL, uint16_t distLF, uint16_t distRF, uint16_t distR)
{
    if (distL < obstakelAfstand)
    {
        ledL_groen.write(1.0);
        ledL_rood.write(brightness);
    }

    else
    {
        ledL_groen.write(brightness);
        ledL_rood.write(1.0);
    }

    if (distR < obstakelAfstand)
    {
        ledR_groen.write(1.0);
        ledR_rood.write(brightness);
    }

    else
    {
        ledR_groen.write(brightness);
        ledR_rood.write(1.0);
    }

    if (distLF < obstakelAfstand)
    {
        ledLF_groen.write(1.0);
        ledLF_rood.write(brightness);
    }

    else
    {
        ledLF_groen.write(brightness);
        ledLF_rood.write(1.0);
    }

    if (distRF < obstakelAfstand)
    {
        ledRF_groen.write(1.0);
        ledRF_rood.write(brightness);
    }

    else 
    {
        ledRF_groen.write(brightness);
        ledRF_rood.write(1.0);
    }
}

  bool sensor_L = false;
  bool sensor_LF = false;
  bool sensor_RF = false;
  bool sensor_R = false;

  enum agvState
  {
      Vooruit_Rijden,
      Achteruit_Rijden,
      Stoppen,
      Draai_Links,
      Draai_Rechts,
      Draai_Links_Afgrond,
      Draai_Rechts_Afgrond,
      Omdraaien
  };

agvState state = Vooruit_Rijden;
Timer stateTimer;

int main()
{
    printf("VL53L0X sensors initieren...\r\n");

    xshut1 = 0;
    xshut2 = 0;
    xshut3 = 0;
    xshut4 = 0;

    ledL_rood.period_ms(1);
    ledLF_rood.period_ms(1);
    ledRF_rood.period_ms(1);
    ledR_rood.period_ms(1);
    ledL_groen.period_ms(1);
    ledLF_groen.period_ms(1);
    ledRF_groen.period_ms(1);
    ledR_groen.period_ms(1);
    
    thread_sleep_for(50);
    printf("Beginnen met sensor 1: \r\n");

    // Sensor Links
    xshut1 = 1;
    thread_sleep_for(10);
    printf("X-shut 1 veranderd\r\n");
  

    if (sensorL.init())
    {
    sensor_L = true;
    sensorL.setTimeout(500);
    sensorL.setAddress(SENSORL_ADDR);
    printf("Address sensor Links changed to 0x%02X\r\n", SENSORL_ADDR);
    }
    else if (!sensor_L)
    {
        printf("Kon linker sensor niet initieren\n");
    }   
    
    // Sensor links-voor
    xshut2 = 1;
    thread_sleep_for(10);
    
    if (sensorLF.init())
    {
    sensor_LF = true;
    sensorLF.setTimeout(500);
    sensorLF.setAddress(SENSORLF_ADDR);
    printf("Address sensor Links-voor changed to 0x%02X\r\n", SENSORLF_ADDR);
    }
    else if (!sensor_LF) 
    {
    printf("Kon sensor Links-voor niet initieren\n");
    }

    // Sensor Rechts-voor
    xshut3 = 1;
    thread_sleep_for(10);

    if (sensorRF.init())
    {
    sensor_RF = true;
    sensorRF.setTimeout(500);
    sensorRF.setAddress(SENSORRF_ADDR);
    thread_sleep_for(10);
    printf("Address sensor Rechts changed to 0x%02X\r\n", SENSORRF_ADDR);
    }
    else if (!sensor_RF)
    {
    printf("Kon sensor Rechts-voor niet initieren\n");
    }

    // // Sensor rechts
    xshut4 = 1;
    thread_sleep_for(10);

    if (sensorR.init())
    {
    sensor_R = true;
    sensorR.setTimeout(500);
    sensorR.setAddress(SENSORR_ADDR);
    thread_sleep_for(10);
    printf("Address sensor Rechts-voor changed to 0x%02X\r\n", SENSORR_ADDR);
    }
    else if(!sensor_R) 
    {
    printf("kon sensor Rechts niet initieren\n");
    }

    scanI2C();

    char c;
    string received = "";
    Timer stringtimer;
    stringtimer.start();

    printf("Bluetooth HC-05 Ready\r\n");

    stateTimer.start();
    zwartTimer.start();
    debounce.start();
    t1_knop.rise(&knop_ingedrukt);


    while (true)
    {   


        if (bluetooth.readable())
        {
            if (bluetooth.read(&c, 1) == 1)
            {
                bluetooth.write(&c, 1);
                received += c;
                stringtimer.reset(); 
            }
        }

        if (received.length() > 0 && stringtimer.elapsed_time() > 50ms)
        {
            printf("Got: %s\r\n", received.c_str());

            if (received == "a")
            {
                printf("AGV Aan\r\n");
                agv_aan = true;
            }
            else if (received == "u")
            {
                printf("AGV Uit\r\n");
                agv_aan = false;
            }
            else if (received == "v")
            {
                printf("Vooruit\r\n");
                vooruitRijden();
            }
            else if (received == "l")
            {
                printf("Naar Links\r\n");
                draaiLinks();

            }
            else if (received == "r")
            {
                printf("Naar Rechts\r\n");
                draaiRechts();

            }
            else if (received == "s")
            {
                printf("Achteruit\r\n");
                achteruitRijden();

            }
            else if (received.substr(0, 4) == "LED_")
            {
                int value = stoi(received.substr(4));
                brightness = value / 10.0f;
                printf("Value: %.2f\r\n", brightness);
            }

            received = ""; 
        }

        if (!agv_aan)
        {
            stopMotoren();
            linksCounter = 0;  // Counters resetten
            rechtsCounter = 0; // Counters resetten
            state = Vooruit_Rijden;
            stateTimer.reset();
            continue;
        }

        if (agv_aan == true)
        {
            // ledL_groen.write(brightness);
            // ledLF_groen.write(brightness);
            // ledRF_groen.write(brightness);
            // ledR_groen.write(brightness);
            
            // ledL_rood.write(1.0);
            // ledLF_rood.write(1.0);
            // ledRF_rood.write(1.0);
            // ledR_rood.write(1.0);

            uint16_t distL = sensorL.readRangeSingleMillimeters();
            uint16_t distLF = sensorLF.readRangeSingleMillimeters();
            uint16_t distRF = sensorRF.readRangeSingleMillimeters();
            uint16_t distR = sensorR.readRangeSingleMillimeters();

            // printf("Afstand Links: %u mm Links-Voor: %u mm Rechts-voor: %u mm Rechts: %u mm \r\n", distL, distLF, distRF, distR);

            bool zwart = zwarte_lijn_detectie();

            if (zwarte_lijn_cooldown && zwartTimer.elapsed_time() > 10s)
            {
                zwarte_lijn_cooldown = false;
            }

            switch (state) 
            {
            case Vooruit_Rijden:

                vooruitRijden();

                updateLEDs(distL, distLF, distRF, distR);

                if (afgrond_detectie())
                {
                    state = Stoppen;
                    stateTimer.reset();
                }

                else if (obstakelDetectie(distL, distLF, distRF, distR))
                {
                    Richting richting = kiesRichting();

                    if (richting == Links)
                    {
                        state = Draai_Links;
                        stateTimer.reset();
                    }

                    else if (richting == Rechts)
                    {
                        state = Draai_Rechts;
                        stateTimer.reset();
                    }
                }

// Om ervoor te zorgen dat de agv niet eindeloos rondjes draait bij zwartelijn detectie
                else if (!zwarte_lijn_cooldown && zwart)
                {
                    state = Omdraaien;
                    stateTimer.reset();

                    zwartTimer.reset();
                    zwarte_lijn_cooldown = true;
                }

                break;

            case Stoppen:

                stopMotoren();

                if (stateTimer.elapsed_time() > 500ms)
                {
                    if (afgrond_detectie())
                    {
                        state = Achteruit_Rijden;
                        stateTimer.reset();
                    }
                    else
                    {
                        Richting richting = kiesRichting();

                        if (richting == Links)
                        {
                            state = Draai_Links;
                            stateTimer.reset();
                        }

                        else if (richting == Rechts)
                        {
                            state = Draai_Rechts;
                            stateTimer.reset();
                        }
                    }
                }
                break;

            case Achteruit_Rijden:

                if (afgrond_detectie())
                {
                    achteruitRijden();
                    ledL_groen.write(1.0);
                    ledL_rood.write(brightness);
                    ledLF_groen.write(1.0);
                    ledLF_rood.write(brightness);
                    ledRF_groen.write(1.0);
                    ledRF_rood.write(brightness);
                    ledR_groen.write(1.0);
                    ledR_rood.write(brightness);
                }

                //else if(!afgrond_detectie())
                else
                {
                    ledL_groen.write(brightness);
                    ledL_rood.write(1.0);
                    ledLF_groen.write(brightness);
                    ledLF_rood.write(1.0);
                    ledRF_groen.write(brightness);
                    ledRF_rood.write(1.0);
                    ledR_groen.write(brightness);
                    ledR_rood.write(1.0);

                    Richting richting = kiesRichting();
                    if (richting == Links)
                    {
                        state = Draai_Links_Afgrond;
                        stateTimer.reset();
                    }

                    else if (richting == Rechts)
                    {
                        state = Draai_Rechts_Afgrond;
                        stateTimer.reset();
                    }
                }

                break;

            case Draai_Links:

                draaiLinks();

                updateLEDs(distL, distLF, distRF, distR);

                if (!obstakelDetectie(distL, distLF, distRF, distR))
                {
                    state = Vooruit_Rijden;
                    stateTimer.reset();
                }
                break;

            case Draai_Rechts:

                draaiRechts();

                updateLEDs(distL, distLF, distRF, distR);

                if (!obstakelDetectie(distL, distLF, distRF, distR))
                {
                    state = Vooruit_Rijden;
                    stateTimer.reset();
                }
                break;

            case Draai_Links_Afgrond:

                draaiLinks();

                if (stateTimer.elapsed_time() >= 18000ms)
                {
                    state = Vooruit_Rijden;
                    stateTimer.reset();
                }

                break;

            case Draai_Rechts_Afgrond:

                draaiRechts();

                if (stateTimer.elapsed_time() >= 1800ms)
                {
                    state = Vooruit_Rijden;
                    stateTimer.reset();
                }

                break;

            case Omdraaien:

                draaiRechts();

                if (stateTimer.elapsed_time() >= 3000ms) // Benodigde tijd voor de AGV om 180 graden te draaien
                {

                    zwarte_lijn_cooldown = true;
                    zwartTimer.reset();

                    state = Vooruit_Rijden;
                    stateTimer.reset();
                }
            }
        }

        if (sensorL.timeoutOccurred()||sensorLF.timeoutOccurred()||sensorRF.timeoutOccurred()||sensorR.timeoutOccurred())
        {
            printf("TIMEOUT!\r\n");
            agv_aan = false;
        }
    }
}