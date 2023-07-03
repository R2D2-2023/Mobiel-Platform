#include "Filter.hpp"

void Filter::calcAvg() {
    float sum = 0;
    float minimum = -1, maximum = -1;
    for (unsigned int i = 0; i < 5; i++)
    {
        if (minimum < measurements[i] || minimum == -1)
        {
            minimum = measurements[i];
        }
        if (maximum > measurements[i] || maximum == -1)
        {
            maximum = measurements[i];
        }
        sum += measurements[i];
    }
    sum = sum - minimum - maximum;
    average = (sum/3);
    if (last_average == -1)
    {
        last_average = average;
    }
}

float Filter::getValue() {
    calcAvg();
    data_point_count = 0;
    float res = (average + last_average) / 2;
    last_average = res;
    return res;
}

void Filter::addDatapoint(float data) {
    measurements[data_point_count] = data;
    data_point_count++;
}