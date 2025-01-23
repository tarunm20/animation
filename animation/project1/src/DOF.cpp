#include "DOF.h"



DOF::DOF(Axis axis) : axis(axis){
    this->min = -100000;
    this->max = 100000;
    this->value = 0;
}

DOF::~DOF() {

}

void DOF::SetValue(float newValue) {
    float* newVal = new float(newValue);
    if (newValue > this->max) {
        this->value = max;
    } else if (newValue < this->min) {
        this->value = min;
    } else {
        this->value = *newVal;
    }
}

void DOF::SetMinMax(float min, float max) {
    this->min = min;
    this->max = max;
}

float DOF::getValue() const {
    return this->value;
}

Axis DOF::getAxis() const {
    return this->axis;
}