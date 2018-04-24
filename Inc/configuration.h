#include "main.h"

#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

/* Constants ------------------------------------------------------------------------------------------ //
// 
// Default constant values for buggy functions
*/

#define WHEEL_RADIUS 						0.0417 	// Wheel radius [m]
#define ENCODER_TICKS_PER_REV 	1024 		// Counter ticks per revolution 

#define MOTOR_SAMPLE_FREQ 			20.0 	// Motor speed sample frequency [Hz]
#define SENSOR_SAMPLE_FREQ 			1000.0 	// Sensor sample frequency [Hz]

#define DEFAULT_PWM_FREQ 				20000.0 // Default PWM switching frequency [Hz]
#define DEFAULT_MOTOR_MODE 			0 			// 0 = unipolar, 1 = bipolar

#define HALL_SENSOR_POLARITY 		1 			// Hall sensor polarity -1 = South, 1 = North, 0 = Both

#define MOTOR_KT 								0.0076 	// Motor Kt [Nm/A] 


#define ROTATION_SPEED          3

#define LEFT_MOTOR_PID_VALUES		{0.25, 0.005, 0.8} // Default PID values for left motor
#define RIGHT_MOTOR_PID_VALUES	{0.2, 0.003, 0.8} // Default PID values for right motor

#define LS_WEIGHTS							{-1.5, 1.5, -0.9, 0.9, -1, 1} // Sensor weights
                                   

#define IR_SENSOR_COUNT					6 // Number of infrared sensors

// ADC configuration
#define ADC_CHANNEL_COUNT 9    // Number of ADC channels to sample
#define ADC_RESOLUTION LL_ADC_RESOLUTION_12B // ADC resolution (using 12bits)
#define ADC_SENSOR_SAMPLETIME LL_ADC_SAMPLINGTIME_480CYCLES    // Sample time for Light sensors 
#define ADC_MAGNETIC_SAMPLETIME LL_ADC_SAMPLINGTIME_480CYCLES  // Sample time for Magnetic sensor
#define ADC_SENS_SAMPLETIME LL_ADC_SAMPLINGTIME_480CYCLES      // Sample time for current sensing sensors


#define MAGNET_NORTH_THRESHOLD 2482   // Upper threshold for north magnetic field detection
#define MAGNET_SOUTH_THRESHOLD 1675   // Lower threshold for south magnetic field detection
/* Communication --------------------------------------------------------------------------------------- //
//
// Toggles communication channels
*/

#define USB_USART 							1 			// Set to 1 to enable, 0 to disable USB serail communication
#define ESP_USART 							1 			// Set to 1 to enable, 0 to disable Wifi serial communication

#define USART_BAUDRATE 					115200 	// Default baud rade used for USART communication
#define USART_BUFFER_SIZE 			128    // Size of USART buffer in bytes

#define LCD_BUFFER_SIZE 				512    // Size of LCD buffer - determined by size of LCD

/* IO -------------------------------------------------------------------------------------------------- //
//
// IO map of ports and pins to functions
*/

// Light sensors
#define IO_SENSOR_0     GPIOC, LL_GPIO_PIN_0
#define IO_SENSOR_1     GPIOC, LL_GPIO_PIN_1
#define IO_SENSOR_2     GPIOC, LL_GPIO_PIN_2
#define IO_SENSOR_3     GPIOC, LL_GPIO_PIN_3
#define IO_SENSOR_4     GPIOC, LL_GPIO_PIN_4
#define IO_SENSOR_5     GPIOC, LL_GPIO_PIN_5
#define IO_ALL_SENSORS  LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5

#define ADC_CH_SENSOR_0 LL_ADC_CHANNEL_10
#define ADC_CH_SENSOR_1 LL_ADC_CHANNEL_11
#define ADC_CH_SENSOR_2 LL_ADC_CHANNEL_12
#define ADC_CH_SENSOR_3 LL_ADC_CHANNEL_13
#define ADC_CH_SENSOR_4 LL_ADC_CHANNEL_14
#define ADC_CH_SENSOR_5 LL_ADC_CHANNEL_15


// Shift register
#define IO_SR_CLK       GPIOB, LL_GPIO_PIN_13
#define IO_SR_LATCH      GPIOB, LL_GPIO_PIN_14
#define IO_SR_DATA     GPIOB, LL_GPIO_PIN_15

// Magnetic sensor
#define IO_MAGNETIC     GPIOB, LL_GPIO_PIN_1
#define ADC_CH_MAGNETIC LL_ADC_CHANNEL_9

// Current Sensing
//#define IO_SENS_A_PLUS  GPIOA, LL_GPIO_PIN_4
//#define IO_SENS_A_MINUS GPIOB, LL_GPIO_PIN_0
//#define IO_SENS_B_PLUS  GPIOA, LL_GPIO_PIN_6
//#define IO_SENS_B_MINUS GPIOA, LL_GPIO_PIN_7
#define IO_SENS_A 				GPIOA, LL_GPIO_PIN_6
#define IO_SENS_B 				GPIOA, LL_GPIO_PIN_7


//#define ADC_CH_A_PLUS   LL_ADC_CHANNEL_4
//#define ADC_CH_A_MINUS  LL_ADC_CHANNEL_8
//#define ADC_CH_B_PLUS   LL_ADC_CHANNEL_6
//#define ADC_CH_B_MINUS  LL_ADC_CHANNEL_7
#define ADC_CH_A			    LL_ADC_CHANNEL_6
#define ADC_CH_B					LL_ADC_CHANNEL_7

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

// USB Serial
#define IO_USART2_TX    GPIOA, LL_GPIO_PIN_2
#define IO_USART2_RX    GPIOA, LL_GPIO_PIN_3

// I2C channel
#define IO_I2C_SCL      GPIOB, LL_GPIO_PIN_8
#define IO_I2C_SDA      GPIOB, LL_GPIO_PIN_9

// LCD screen

/* SPI3 -> temporarily disabled for testing */
#define IO_LCD_SCK      GPIOC, LL_GPIO_PIN_10
#define IO_LCD_MISO     GPIOC, LL_GPIO_PIN_11
#define IO_LCD_MOSI     GPIOC, LL_GPIO_PIN_12
#define IO_LCD_nCS      GPIOD, LL_GPIO_PIN_2
#define IO_LCD_A0       GPIOB, LL_GPIO_PIN_7
#define IO_LCD_RESET    GPIOB, LL_GPIO_PIN_12


/*
#define IO_LCD_SCK      GPIOA, LL_GPIO_PIN_5
#define IO_LCD_MISO     GPIOC, LL_GPIO_PIN_11
#define IO_LCD_MOSI     GPIOA, LL_GPIO_PIN_7
#define IO_LCD_nCS      GPIOB, LL_GPIO_PIN_6
#define IO_LCD_A0       GPIOA, LL_GPIO_PIN_8
#define IO_LCD_RESET    GPIOA, LL_GPIO_PIN_6
*/
// Other IO
#define IO_SPEAKER      GPIOB, LL_GPIO_PIN_6
#define IO_CENTER_JOY   GPIOC, LL_GPIO_PIN_13
#define IO_LED_BLUE			GPIOA, LL_GPIO_PIN_9

#endif
