#pragma once
class Point {
private:
    double x;
    double y;

public:
    Point(double xVal, double yVal) {
        x = xVal;
        y = yVal;
    }

    Point() {
        x = 0;
        y = 0;
    }

    double get_x() {
        return x;
    }

    void set_x(double value) {
        x = value;
    }

    double get_y() {
        return y;
    }

    void set_y(double value) {
        y = value;
    }
};
