#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <Arduino.h>
#include <Adafruit_SCD30.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <sps30.h>
#include "INA226.h"
#include <Filter.hpp>


/**
 * @brief 
 * This the class Sensors. In this class sets up the sensors and gets the measurements.
 * It also send the measured values to the serial port.
 */
class Sensors {
private:
	/**
     * @brief 
     * Adafruit_BME280 object
     */
	Adafruit_BME280 bme; 

	/**
     * @brief 
     * INNA266 object
     */
	INA226 INA = INA226(0x40);

	/**
     * @brief 
     * Adafruit_SCD30 object
     */

	Adafruit_SCD30 scd30;
	
    /**
     * @brief 
     * The temprature in degrees celsius 
     */
	unsigned int temperature;
	 /**
     * @brief 
     * The relative humidity in %
     */
	unsigned int humidity;
	 /**
     * @brief 
     * The pressure in HPa
     */
	unsigned int pressure;
	 /**
     * @brief 
     * The carbondioxide levels in parts per milion
     */
	unsigned int co2;
   /**
     * @brief 
     * The voltage in volt.
     */
	unsigned int voltage;
	 /**
     * @brief 
     * The particulate matter 1.0
     */
	unsigned int pm1;
	 /**
     * @brief 
     * The particulate matter 2.5
     */
	unsigned int pm25;
		 /**
     * @brief 
     * The particulate matter 10
     */
	unsigned int pm10;
	 /**
     * @brief 
     * The X location as given by position
     */
     String loc_x = "0";
	 /**
     * @brief 
     * The Y location as given by position
     */
     String loc_y = "0";
	 /**
     * @brief 
     * Bool to indicate if a location has been measured yet. 
     */
     bool loc_measured = true;

    //Error bytes
    uint8_t check_values_byte_0;
    uint8_t check_values_byte_1;
  
	/**
     * @brief 
     * Filter object to filter temperature
     */
	Filter temp_filter = Filter();
	/**
     * @brief 
     * Filter object to filter relative humidity
     */
	Filter hum_filter = Filter();
	/**
     * @brief 
     * Filter object to filter carbondioxide levels
     */
	Filter co2_filter = Filter();
	/**
     * @brief 
     * Filter object to filter pressure
     */
	Filter pres_filter = Filter();
	/**
     * @brief 
     * Filter object to filter voltage
     */
	Filter volt_filter = Filter();
	/**
     * @brief 
     * Filter object to filter  The particulate matter 1.0
     */
	Filter pm1_filter = Filter();
	/**
     * @brief 
     * Filter object to filter  The particulate matter 2.5
     */
	Filter pm25_filter = Filter();
	/**
     * @brief 
     * Filter object to filter  The particulate matter 10
     */
	Filter pm10_filter = Filter();
	
	void getValues();
    /**
     * @brief 
     * Function to send the measured values
     */
	void sendValues();
	void checkValues();
	void sendErrorByte();


public:
	/**
     * @brief 
     * Construct a new Sensor object
     */
	Sensors();
	/**
     * @brief 
     * Funtion that sets up the sensors and returns int to indicate if a sensor won't set up.
     */
	void setUpSensors();
	/**
     * @brief 
     * Funtion that does the measurements
     */
	void doMeasurements();
     /**
     * @brief 
     * Funtion sends live location
     */
     void sendLiveLocationValues();
};
#endif //SENSORS_HPP
