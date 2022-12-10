#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0

static int addr = 0x18;

// Initialise Accelerometer Function
void accel_init(void){
    // Check to see if connection is correct
    sleep_ms(1000); // Add a short delay to help BNO005 boot up
    uint8_t reg = 0x06;
    uint8_t chipID[2];
    i2c_write_blocking(I2C_PORT, addr, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, addr, chipID, 2, false);

    if(chipID[1] != 0x54){
        while(1){
            printf("Chip ID Not Correct - Check Connection!");
            sleep_ms(5000);
        }
    }
}

int main(void){
    stdio_init_all(); // Initialise STD I/O for printing over serial

    // Configure the I2C Communication
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    // Call accelerometer initialisation function
    accel_init();

    uint8_t data[2]; // Store data from the 6 acceleration registers
    float f_tempC; // Float type of acceleration data
    uint8_t val = 0x05; // Start register address

    // Infinite Loop
    while(1){
        i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
        i2c_read_blocking(I2C_PORT, addr, data, 2, false);

        printf("%x %x\n", data[0], data[1]);
        float Temperature;
        data[0] = data[0] & 0x1F;
        if ((data[0] & 0x10) == 0x10) { // tA < 0C
            printf("Less than 0 calc\n");
            Temperature = 256 - (((float)data[0]) * 16 + (((float)data[1])/16));
        } else {
            printf("More than 0 calc\n");
            float a = ((float)data[0]);
            float b = ((float)data[1]);
            printf("%f %f\n",a,b);
            Temperature = (a * 16.0f) + (b/16.0f);
        }
        printf("%6.2f \n", Temperature);

        // Print to serial monitor
        // printf("X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_accelX, f_accelY, f_accelZ);
        sleep_ms(300);
    }
}