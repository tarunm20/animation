#pragma once

#include "core.h"

enum class Axis {
    X, Y, Z
};

class DOF {
private:
    Axis axis;
    float value;
    float max;
    float min;

public:
    DOF(Axis axis);
    ~DOF();

    void SetValue(float newValue);
    void SetMinMax(float minVal, float max);
    float getValue() const;
    Axis getAxis() const;
};