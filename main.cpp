
// #include "mbed.h"
// #include "VL53L0X.h"

// #define HIGH_ACCURACY

// // ── Timers ────────────────────────────────────────────────────────
// Timer sensor_timer;
// Timer led_timer;
// Timer t1, t2, t3;

// // ── Status flags ──────────────────────────────────────────────────
// bool afgrond     = false;
// bool zwarte_lijn = false;

// // ── XSHUT pins ───────────────────────────────────────────────────
// DigitalOut xshut1(D8);
// DigitalOut xshut2(D9);
// DigitalOut xshut3(D10);

// // ── I2C bus + sensoren ───────────────────────────────────────────
// I2C i2c(D14, D15);  // SDA, SCL
// VL53L0X sensor1(i2c, t1);
// VL53L0X sensor2(i2c, t2);
// VL53L0X sensor3(i2c, t3);

// #define ADDR_SENSOR1 0x30
// #define ADDR_SENSOR2 0x31
// #define ADDR_SENSOR3 0x32

// // ── IR-sensoren afgrond ───────────────────────────────────────────
// // TODO: vervang D4 door de echte pinnen
// DigitalIn ir1(D4);
// DigitalIn ir2(D4);
// DigitalIn ir3(D4);
// DigitalIn ir4(D4);

// // ── IR-sensoren zwarte lijn ───────────────────────────────────────
// // TODO: vervang D4 door de echte pinnen
// DigitalIn ir5(D4);
// DigitalIn ir6(D4);
// DigitalIn ir7(D4);
// DigitalIn ir8(D4);

// // ── LEDs + potmeter ───────────────────────────────────────────────
// PwmOut led1(D3);
// PwmOut led2(D5);
// PwmOut led3(D6);
// AnalogIn potmeter(A0);

// // ─────────────────────────────────────────────────────────────────

// void init_sensors()
// {
//     // Zet alle sensoren in reset
//     xshut1 = 0;
//     xshut2 = 0;
//     xshut3 = 0;
//     ThisThread::sleep_for(10ms);

//     // Sensor 1
//     printf("Sensor1 wakker maken...\r\n");
//     xshut1 = 1;
//     ThisThread::sleep_for(10ms);
//     t1.start();
//     sensor1.init();
//     sensor1.setAddress(ADDR_SENSOR1);
//     sensor1.setTimeout(500);
//     sensor1.setMeasurementTimingBudget(200000);
//     printf("Sensor1 klaar op adres 0x%X\r\n", ADDR_SENSOR1);

//     // Sensor 2
//     printf("Sensor2 wakker maken...\r\n");
//     xshut2 = 1;
//     ThisThread::sleep_for(10ms);
//     t2.start();
//     sensor2.init();
//     sensor2.setAddress(ADDR_SENSOR2);
//     sensor2.setTimeout(500);
//     sensor2.setMeasurementTimingBudget(200000);
//     printf("Sensor2 klaar op adres 0x%X\r\n", ADDR_SENSOR2);

//     // Sensor 3
//     printf("Sensor3 wakker maken...\r\n");
//     xshut3 = 1;
//     ThisThread::sleep_for(10ms);
//     t3.start();
//     sensor3.init();
//     sensor3.setAddress(ADDR_SENSOR3);
//     sensor3.setTimeout(500);
//     sensor3.setMeasurementTimingBudget(200000);
//     printf("Sensor3 klaar op adres 0x%X\r\n", ADDR_SENSOR3);

//     printf("Alle sensoren geinitieerd!\r\n");
// }

// void object_detectie(VL53L0X &sensor, const char* name)
// {
//     int distance = sensor.readRangeSingleMillimeters();
//     if (sensor.timeoutOccurred())
//         printf("[%s] TimeOut!\r\n", name);
//     else
//         printf("[%s] Afstand: %d mm\r\n", name, distance);
// }

// void afgrond_detectie()
// {
//     afgrond = (ir1.read() == 1) || (ir2.read() == 1) ||
//               (ir3.read() == 1) || (ir4.read() == 1);
// }

// void zwarte_lijn_detectie()
// {
//     zwarte_lijn = (ir5.read() == 1) || (ir6.read() == 1) ||
//                   (ir7.read() == 1) || (ir8.read() == 1);
// }

// void update_leds()
// {
//     float value = potmeter.read();
//     led1.write(value);
//     led2.write(value / 2.0f);
//     led3.write(value / 2.0f);
// }

// int main()
// {
//     // LEDs initialiseren (period_ms VOOR write)
//     led1.period_ms(1);
//     led1.write(0.0f);
//     led2.period_ms(1);
//     led2.write(0.0f);
//     led3.period_ms(1);
//     led3.write(0.0f);

//     potmeter.set_reference_voltage(3.3f);

//     printf("Start, sensoren initieren...\r\n");
//     init_sensors();
//     printf("Initiatie compleet!\r\n");

//     sensor_timer.start();
//     led_timer.start();

//     while (true)
//     {
//         if (led_timer.elapsed_time() >= 1000ms)
//         {
//             update_leds();
//             led_timer.reset();
//         }

//         if (sensor_timer.elapsed_time() >= 500ms)
//         {
//             object_detectie(sensor1, "Sensor1");
//             object_detectie(sensor2, "Sensor2");
//             object_detectie(sensor3, "Sensor3");
//             afgrond_detectie();
//             zwarte_lijn_detectie();
//             sensor_timer.reset();
//         }
//     }
// }


#include <mbed.h>
#include <VL53L0X.h>

I2C i2c(D14, D15);
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
DigitalOut xshut1(D2);
DigitalOut xshut2(D3);
DigitalOut xshut3(D4);
DigitalOut xshut4(D5);

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
DigitalIn ir1(D4);
DigitalIn ir2(D4);
DigitalIn ir3(D4);
DigitalIn ir4(D4);

// ---- IR-sensoren zwarte lijn ----
DigitalIn ir5(D4);
DigitalIn ir6(D4);

//----RGB LEDS ----
PwmOut ledL_rood(PA_0);
PwmOut ledL_groen(PA_1);
PwmOut ledLF_rood(PA_2);
PwmOut ledLF_groen(PB_6);
PwmOut ledRF_rood(PB_7);
PwmOut ledRF_groen(PB_8);
PwmOut ledR_rood(PB_9);
PwmOut ledR_groen(D3); // D5, D6, D9-D12 pwm pins zijn vrij voor H-bridge. Overgebleven pins kunnen veranderd worden.

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
}

void draaiLinks()
{
    printf("draai links. Motor 1 uit of heel zacht(pwm) motor 2 volledig laten draaien\n");
}

void draaiRechts()
{
    printf("draai rechts. Motor 2 uit of heel zacht(pwm) motor 1 volledig laten draaien\n");
}

void stopMotoren()
{
    printf("Stop motoren\n");
}

void achteruitRijden()
{
    printf("Rijdt achteruit\n");
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

    voerRichtingUit(kiesRichting());
}

int main()
{
    printf("Starting VL53L0X sensors...\r\n");

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

        if (obstakelDetectie(distL, distLF, distRF, distR))
        {
            Richting richting = kiesRichting();

            if (richting == Links)
            {
                printf("Ga naar links\n");
            }

            if (richting == Rechts)
            {
                printf("Ga naar rechts\n");
            }
        }

        if (afgrond_detectie())
        {
            handleAfgrond();
        }

        if (sensorL.timeoutOccurred()||sensorLF.timeoutOccurred()||sensorRF.timeoutOccurred()||sensorR.timeoutOccurred())
        {
            printf("TIMEOUT!\r\n");
        }

        thread_sleep_for(500);
    }
}