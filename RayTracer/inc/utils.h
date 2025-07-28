#ifndef UTILS_H
#define UTILS_H

// Return the normalized value based on a current and a max value.
template <typename T>
double normalize( T val, T max ) {
    return static_cast<double>(val) / max;
}

#endif // UTILS_H