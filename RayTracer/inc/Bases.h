#ifndef BASES_H
#define BASES_H

#include <iomanip>
#include <iostream>

#include "Vectors.h"


// Parent class to all scene objects describing the position of an object in 3D.
struct Obj {
    FVector3 m_position;

    Obj()
        : m_position{ 0.0, 0.0, 0.0 } {
    }
    Obj( FVector3 pos )
        : m_position{ pos } {
    }
    Obj( Vector3 pos )
        : m_position{ FVector3( pos ) } {
    }
};

/* Color class that works with integers.
Initializes with R, G, B integer values or normalized float values. */
struct Color : protected Vector3 {
    int& r;
    int& g;
    int& b;

    Color( int in_r = 0, int in_g = 0, int in_b = 0 )
        : r{ x }, g{ y }, b{ z } {
        r = in_r;
        g = in_g;
        b = in_b;
    }
    Color( double in_r, double in_g, double in_b )
        : r{ x }, g{ y }, b{ z } {
        r = static_cast<int>(round( in_r * 255 ));
        g = static_cast<int>(round( in_g * 255 ));
        b = static_cast<int>(round( in_b * 255 ));
    }
};

// Easy printing of a Color (inline prevents linker error as this is in a header)
inline std::ostream& operator<<( std::ostream& os, const Color& color ) {
    os << std::setw( 3 ) << color.r << " " << std::setw( 3 ) << color.g << " " << std::setw( 3 ) << color.b;
    return os;
}

#endif // BASES_H