#include "mbed.h"
#include "VL53L0X.h"



// Blinking rate in milliseconds
#define Delay 1000ms
#define HIGH_ACCURACY

Timer sensor_timer;
Timer led_timer;

I2C i2c(PA_8, PA_9);
VL53L0X sensor(i2c, sensor_timer);

PwmOut led1(D3);
PwmOut led2(D5);
PwmOut led3(D6);
AnalogIn potmeter(A0);
void object_detectie()
{
    int distance = sensor.readRangeSingleMillimeters();
    if (sensor.timeoutOccurred())
    {
        printf("TimeOut!\r\n");
    }
    else
    {
        printf("Afstand is: %d\n", distance);
    }
}

void update_leds()
{
    float value = potmeter.read();
    led1.write(value);
    led2.write(value / 2);
    led3.write(value / 2);
}

int main()
{
    printf("Start, Sensoren initieren\r\n");
    sensor.init();
    sensor.setTimeout(500);
    sensor.setMeasurementTimingBudget(200000);

    potmeter.set_reference_voltage(3.3);
    led1.write(0.0);
    led1.period_ms(1); 
    led2.write(0.0);
    led2.period_ms(1); 
    led3.write(0.0);
    led3.period_ms(1); 
    printf("Initiatie compleet!\r\n");
    
    sensor_timer.start();
    led_timer.start();

    while (true)
    {
        if (led_timer.elapsed_time() >= 1000ms)
        {
            update_leds();
            led_timer.reset();
        }

        if (sensor_timer.elapsed_time() >= 500ms)
        {
            object_detectie();
            sensor_timer.reset();
        }
    }
}



