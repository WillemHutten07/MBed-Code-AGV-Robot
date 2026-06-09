#include <mbed.h>
#include <VL53L0X.h>
#include <string>

I2C i2c(D14, D15);
BufferedSerial bluetooth(PA_11, PA_12, 9600);
Timer t1;

int linksCounter = 0;
int rechtsCounter = 0;
uint16_t obstakelAfstand = 200; //200 mm

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
DigitalIn ir1(D12);
DigitalIn ir2(PC_10);
DigitalIn ir3(A2);
DigitalIn ir4(A4);

// ---- IR-sensoren zwarte lijn ----
DigitalIn ir5(PB_1);
DigitalIn ir6(PB_2);

//----Motoren----
DigitalOut in1(D2);
DigitalOut in2(D4);
DigitalOut in3(D7);
DigitalOut in4(D8);

// DigitalOut ena(x); // Voor PWM

//----RGB LEDS ----PWM pins
PwmOut ledL_rood(D3);
PwmOut ledL_groen(D5);
PwmOut ledLF_rood(D6);
PwmOut ledLF_groen(D10);
PwmOut ledRF_rood(D11);
PwmOut ledRF_groen(A0);
PwmOut ledR_rood(A1);
PwmOut ledR_groen(A3);

void scanI2C()
{
    printf("Scanning I2C...\r\n");
    for (int addr = 1; addr < 127; addr++)
    {
        if (!i2c.write(addr << 1, nullptr, 0))
        {
            printf("Device found at 0x%02X\r\n", addr);
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
    printf("Rijdt vooruit\n");
        in1 = 1;
        in2 = 0;
        in3 = 1;
        in4 = 0;
}

void draaiLinks()
{
    printf("draai links. Motor 1 uit of heel zacht(pwm) motor 2 volledig laten draaien\n");
    in1 = 1;
    in2 = 0;
    in3 = 0;
    in4 = 0;
}

void draaiRechts()
{
    printf("draai rechts. Motor 2 uit of heel zacht(pwm) motor 1 volledig laten draaien\n");
    in1 = 0;
    in2 = 0;
    in3 = 1;
    in4 = 0;
}

void stopMotoren()
{
    printf("Stop motoren\n");
    in1 = 0;
    in2 = 0;
    in3 = 0;
    in4 = 0;
}

void achteruitRijden()
{
    printf("Rijdt achteruit\n");
    in1 = 0;
    in2 = 1;
    in3 = 0;
    in4 = 1;
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

void voerRichtingUit(Richting richting)
{
    if (richting == Links)
    {
        draaiLinks();
    }
    else
    {
        draaiRechts();
    }
}

void handleAfgrond()
{
    stopMotoren();
    achteruitRijden();
    thread_sleep_for(500);

    stopMotoren();
    thread_sleep_for(500);

    voerRichtingUit(kiesRichting());
}

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

    // Sensor Links
    xshut1 = 1;
    thread_sleep_for(10);
    sensorL.init();
    sensorL.setTimeout(500);
    sensorL.setAddress(SENSORL_ADDR);
    
    // Sensor links-voor
    xshut2 = 1;
    thread_sleep_for(10);
    sensorLF.init();
    sensorLF.setTimeout(500);
    sensorLF.setAddress(SENSORLF_ADDR);

    // Sensor Rechts-voor
    xshut3 = 1;
    thread_sleep_for(10);
    sensorRF.init();
    sensorRF.setTimeout(500);
    sensorRF.setAddress(SENSORRF_ADDR);
    thread_sleep_for(10);

    // Sensor rechts
    xshut4 = 1;
    thread_sleep_for(10);
    sensorR.init();
    sensorR.setTimeout(500);
    sensorR.setAddress(SENSORR_ADDR);
    thread_sleep_for(10);

    printf("Address sensor Links changed to 0x%02X\r\n", SENSORL_ADDR);
    printf("Address sensor Links-voor changed to 0x%02X\r\n", SENSORLF_ADDR);
    printf("Address sensor Rechts changed to 0x%02X\r\n", SENSORRF_ADDR);
    printf("Address sensor Rechts-voor changed to 0x%02X\r\n", SENSORR_ADDR);

    // Step 5: scan I2C bus (should show new address)
    scanI2C();

    char c;
    string received = "";
    Timer stringtimer;
    stringtimer.start();

    printf("Bluetooth HC-05 Ready\r\n");

    // Step 6: test reading from new address
    while (1)
    {
        uint16_t distL = sensorL.readRangeSingleMillimeters();
        uint16_t distLF = sensorLF.readRangeSingleMillimeters();
        uint16_t distRF = sensorRF.readRangeSingleMillimeters();
        uint16_t distR = sensorR.readRangeSingleMillimeters();

        printf("Afstand Links: %u mm @ address 0x%02X\r\n", distL, SENSORL_ADDR);
        printf("Afstand Links-Voor: %u mm @ address 0x%02X\r\n", distLF, SENSORLF_ADDR);
        printf("Afstand Rechts-Voohr: %u mm @ address 0x%02X\r\n", distRF, SENSORRF_ADDR);
        printf("Afstand Rects: %u mm @ address 0x%02X\r\n", distR, SENSORR_ADDR);

        if (afgrond_detectie())
        {
            handleAfgrond();
        }

        else if (obstakelDetectie(distL, distLF, distRF, distR))
        {
            if (distL < obstakelAfstand)
            {
                ledL_groen.write(1.0);
                ledL_rood.write(0.5);
            }
            else if (distR < obstakelAfstand)
            {
                ledR_groen.write(1.0);
                ledR_rood.write(0.5);
            }
            else if (distLF < obstakelAfstand || distRF < obstakelAfstand)
            {
                ledLF_groen.write(1.0);
                ledLF_rood.write(0.5);
                ledRF_groen.write(1.0);
                ledRF_rood.write(0.5);
            }

            voerRichtingUit(kiesRichting());
        }

        else
        {
            ledL_groen.write(0.5);
            ledL_rood.write(1.0);
            ledLF_groen.write(0.5);
            ledLF_rood.write(1.0);
            ledRF_groen.write(0.5);
            ledRF_rood.write(1.0);
            ledR_groen.write(0.5);
            ledR_rood.write(1.0);
            vooruitRijden();
        }

        if (bluetooth.readable())
        {
            if (bluetooth.read(&c, 1) == 1)
            {
                bluetooth.write(&c, 1);
                received += c;
                stringtimer.reset();  // reset timer on every new character
            }
        }

        // If we have data and no new characters for 50ms, process it
        if (received.length() > 0 && stringtimer.elapsed_time() > 50ms)
        {
            printf("Got: %s\r\n", received.c_str());

            if (received == "a")
            {
                printf("AGV Aan\r\n");
            }
            else if (received == "u")
            {
                printf("AGV Uit\r\n");
            }
            else if (received == "v")
            {
                printf("Vooruit\r\n");
            }
            else if (received == "l")
            {
                printf("Naar Links\r\n");
            }
            else if (received == "r")
            {
                printf("Naar Rechts\r\n");
            }
            else if (received == "s")
            {
                printf("Achteruit\r\n");
            }
            else if (received.substr(0, 4) == "LED_")
            {
                int value = stoi(received.substr(4));
                float brightness = value / 10.0f;
                printf("Value: %.2f\r\n", brightness);
            }

            received = "";  // clear for next message
        }    

        if (sensorL.timeoutOccurred()||sensorLF.timeoutOccurred()||sensorRF.timeoutOccurred()||sensorR.timeoutOccurred())
        {
            printf("TIMEOUT!\r\n");
            stopMotoren();
        }
        thread_sleep_for(3000);
    }
}

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
//         value = potmeter.read();

//         // led1_rood.write(value);
//         led1_groen.write(value);

//         // led2_rood.write(value);
//         led2_groen.write(value);
//         printf("Value: %f\n", value);
//         thread_sleep_for(500);
//     }
// }

// #include "mbed.h"

// // Pas de pinnen aan indien nodig
// DigitalOut in1(D2);
// DigitalOut in2(D3);
// DigitalOut in3(D4);
// DigitalOut in4(D5);
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

// DigitalOut xshut1(PC_6);
// DigitalOut xshut2(PC_8);
// DigitalOut xshut3(D4);
// DigitalOut xshut4(D5);


// int main()
// {
//     printf("initieren\n");
//     xshut1 = 1;
//     thread_sleep_for(10);
//     sensorL.init();
//     sensorL.setTimeout(500);
//     sensorL.setAddress(SENSORL_ADDR);
//     printf("Initieren gereed\n");

//     while (1)
//     {
//         uint16_t distL = sensorL.readRangeSingleMillimeters();
//         // uint16_t distLF = sensorLF.readRangeSingleMillimeters();
//         // uint16_t distRF = sensorRF.readRangeSingleMillimeters();
//         // uint16_t distR = sensorR.readRangeSingleMillimeters();

//         printf("Afstand Links: %u mm @ address 0x%02X\r\n", distL, SENSORL_ADDR);
//         // printf("Afstand Links-Voor: %u mm @ address 0x%02X\r\n", distLF, SENSORLF_ADDR);
//         // printf("Afstand Rechts-Voohr: %u mm @ address 0x%02X\r\n", distRF, SENSORRF_ADDR);
//         // printf("Afstand Rects: %u mm @ address 0x%02X\r\n", distR, SENSORR_ADDR);
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
