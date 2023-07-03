#ifndef Filter_HPP
#define Filter_HPP
#include <Arduino.h>

     /**
     * @brief 
     * class for filtering plausible faulty data
     */
class Filter
{
private:
    /**
     * @brief 
     * Funtion that calculates the average
     */
    void calcAvg();
    /**
     * @brief 
     * The average
     */
    float average;
      /**
     * @brief 
     * The last average
     */
    float last_average = -1;
       /**
     * @brief 
     * Array with 5 measurements
     */
    float measurements[5];
       /**
     * @brief 
     * The amount of data point
     */
    unsigned int data_point_count = 0;
   
public:

       /**
     * @brief 
     * This function calculates the datapoint 
     */
    float getValue();

       /**
     * @brief 
     * Adds the datapoint to measurements
     * @param
     * The data get value returned
     */
    void addDatapoint(float data);
};

#endif //Filter_HPP
