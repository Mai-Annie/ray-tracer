#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

class interval {
    public:
        double min, max;

        interval() : min(+infinity), max(-infinity) {} // default constructor creates an empty interval
        interval(double min, double max) : min(min), max(max) {} // constructor with specified min and max values

        double size() const {
            return max - min; // returns the size of the interval
        }

        bool contains(double x) const {
            return x >= min && x <= max; // returns true if x is within the interval, false otherwise
        }

        bool surrounds(double x) const {
            return x > min && x < max; // returns true if x is strictly within the interval, false otherwise
        }

        double clamp(double x) const {
            if (x < min) return min; // if x is less than the minimum, return the minimum
            if (x > max) return max; // if x is greater than the maximum, return the maximum
            return x; // if x is within the interval, return x
        }

        static const interval empty, universe; // static constants representing the empty interval and the universe interval
 };

const interval interval::empty = interval(+infinity, -infinity); // the empty interval has min greater than max
const interval interval::universe = interval(-infinity, +infinity); // the universe interval has min less than max and covers all real numbers

#endif