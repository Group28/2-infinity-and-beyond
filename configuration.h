
#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

// Constants

#define WHEEL_RADIUS 0.0417 // Wheel radius [m]

#define MOTOR_SAMPLE_FREQ 500.0 // Motor speed sample frequency [Hz]
#define SENSOR_SAMPLE_FREQ 1000.0 // Sensor sample frequency [Hz]

#define DEFAULT_PWM_FREQ 20000.0 // Default PWM switching frequency [Hz]
#define DEFAULT_MOTOR_MODE 0 // 0 = unipolar, 1 = bipolar

#define HALL_SENSOR_POLARITY 1 // Hall sensor polarity -1 = South, 1 = North, 0 = Both

#define MOTOR_KT 0.0076 // Motor Kt [Nm/A] 

// IO

// Light sensors
#define IO_SENSOR_0     GPIOC, LL_GPIO_PIN_10
#define IO_SENSOR_1     GPIOC, LL_GPIO_PIN_11
#define IO_SENSOR_2     GPIOC, LL_GPIO_PIN_12
#define IO_SENSOR_3     GPIOC, LL_GPIO_PIN_13
#define IO_SENSOR_4     GPIOC, LL_GPIO_PIN_14
#define IO_SENSOR_5     GPIOC, LL_GPIO_PIN_15
#define IO_ALL_SENSORS  LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15


// Shift register
#define IO_SR_CLK       GPIOB, LL_GPIO_PIN_13
#define IO_SR_DATA      GPIOB, LL_GPIO_PIN_14
#define IO_SR_LATCH     GPIOB, LL_GPIO_PIN_15

// Magnetic sensor
#define IO_MAGNETIC     GPIOB, LL_GPIO_PIN_1

// Current Sensing
#define IO_SENS_A_PLUS  GPIOA, LL_GPIO_PIN_4
#define IO_SENS_A_MINUS GPIOB, LL_GPIO_PIN_0
#define IO_SENS_B_PLUS  GPIOA, LL_GPIO_PIN_6
#define IO_SENS_B_MINUS GPIOA, LL_GPIO_PIN_7

// Battery voltage sensing
#define IO_MICROLAN     GPIOB, LL_GPIO_PIN_4

// Encoders
#define IO_ENC_1_CH_A   GPIOA, LL_GPIO_PIN_0
#define IO_ENC_1_CH_B   GPIOA, LL_GPIO_PIN_1
#define IO_ENC_2_CH_A   GPIOA, LL_GPIO_PIN_15
#define IO_ENC_2_CH_B   GPIOB, LL_GPIO_PIN_3

// Motor controlls
#define IO_MOTOR_EN     GPIOA, LL_GPIO_PIN_5

#define IO_MOTOR_1_BP   GPIOC, LL_GPIO_PIN_7
#define IO_MOTOR_1_DIR  GPIOB, LL_GPIO_PIN_5
#define IO_MOTOR_1_PWM  GPIOC, LL_GPIO_PIN_8
#define IO_TR_SERVO_1   GPIOA, LL_GPIO_PIN_8

#define IO_MOTOR_2_BP   GPIOB, LL_GPIO_PIN_10
#define IO_MOTOR_2_DIR  GPIOA, LL_GPIO_PIN_10
#define IO_MOTOR_2_PWM  GPIOC, LL_GPIO_PIN_9
#define IO_TR_SERVO_2   GPIOA, LL_GPIO_PIN_9

// ESP8266 Wifi serial

#define IO_ESP_TX       GPIOA, LL_GPIO_PIN_11
#define IO_ESP_RX       GPIOA, LL_GPIO_PIN_12

#define IO_USART2_TX    GPIOA, LL_GPIO_PIN_2
#define IO_USART2_RX    GPIOA, LL_GPIO_PIN_3

#define IO_I2C_SCL      GPIOB, LL_GPIO_PIN_8
#define IO_I2C_SDA      GPIOB, LL_GPIO_PIN_9

// LCD screen
#define IO_LCD_SCK      GPIOC, LL_GPIO_PIN_10
#define IO_LCD_RESET    GPIOC, LL_GPIO_PIN_11
#define IO_LCD_MOSI     GPIOC, LL_GPIO_PIN_12
#define IO_LCD_nCS      GPIOD, LL_GPIO_PIN_2
#define IO_LCD_A0       GPIOB, LL_GPIO_PIN_7

// Other IO
#define IO_SPEAKER      GPIOB, LL_GPIO_PIN_6
#define IO_CENTER_JOY   GPIOC, LL_GPIO_PIN_13


#endif
