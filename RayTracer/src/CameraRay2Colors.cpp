#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>


constexpr int vertsInTriangle = 3;

// Vector2 structure supporting addition with another Vector2.
struct Vector2 {
    int x, y;

    Vector2 operator+( const Vector2& val ) const {
        return Vector2( x + val.x, y + val.y );
    }
};

/* FVector2 structure using doubles supporting addition with another FVector2 and
a scalar. Initializes using doubles or a Vector2. */
struct FVector2 {
    double x, y;

    FVector2( double in_x = 0, double in_y = 0 )
        : x{ in_x }, y{ in_y } {
    }
    FVector2( Vector2 vec )
        : x{ static_cast<double>(vec.x) }, y{ static_cast<double>(vec.y) } {
    }

    FVector2 operator+( const double val ) const {
        return FVector2( x + val, y + val );
    }

    FVector2 operator+( const FVector2& val ) const {
        return FVector2( x + val.x, y + val.y );
    }
};

// Vector3 structure supporting addition with another Vector3 and += addition.
struct Vector3 {
    int x, y, z;

    Vector3 operator+( const Vector3& other ) const {
        return { x + other.x, y + other.y, z + other.z };
    };

    Vector3 operator+=( const Vector3& other ) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    };
};

/* FVector3 structure using doubles supporting addition with another FVector3 and
+= addition. Initializes using doubles or a Vector2 and a double. */
struct FVector3 {
    double x, y, z;

    FVector3( double in_x = 0.0, double in_y = 0.0, double in_z = 0.0 )
        : x{ in_x }, y{ in_y }, z{ in_z } {
    }
    FVector3( const FVector2& vec, double in_z = 0.0 )
        : x{ vec.x }, y{ vec.y }, z{ in_z } {
    }
    FVector3( const Vector3& vec )
        : x{ static_cast<double>(vec.x) },
        y{ static_cast<double>(vec.y) },
        z{ static_cast<double>(vec.z) } {
    }

    FVector3 operator+( const FVector3& other ) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    FVector3 operator+=( const FVector3& other ) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    };
};

// Parent class to all scene objects describing the position of an object in 3D.
struct Obj {
    FVector3 m_position;
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

// Camera Image Plane object with a given resolution and position.
struct ImagePlane : Obj {
    FVector2 resolution;
    ImagePlane( int width, int height, Vector3 pos = { 0, 0, -1 } )
        : resolution{ static_cast<double>(width), static_cast<double>(height) }, Obj( pos ) {
    }
};

/* Camera class with an Image Plane, position, orientation and children. Any child object
can move relative to the parent Camera. Supports move to an absolute location or relative
to current location. Both move functions move the children as well. */
struct Camera : public Obj {
    ImagePlane m_imgPlane;
    FVector3 m_direction;
    std::vector<Obj> m_children;

    Camera( ImagePlane imgPlane, FVector3 direction, FVector3 pos = { 0, 0, 0 } )
        : Obj( pos ), m_imgPlane{ imgPlane }, m_direction{ direction } {
        m_children.reserve( 1 );
        m_children.push_back( m_imgPlane );
    }

    void move( const FVector3& abs_pos ) {
        std::vector<FVector3> childrenRelPos;
        childrenRelPos.reserve( m_children.size() );

        for ( Obj& child : m_children ) {
            childrenRelPos.push_back( m_position + child.m_position );
        }

        m_position = abs_pos;

        for ( size_t i{}; i < m_children.size(); ++i ) {
            m_children[i].m_position = abs_pos + childrenRelPos[i];
        }
    }

    void moveRel( const FVector3& rel_pos ) {
        m_position += rel_pos;

        for ( Obj& child : m_children ) {
            child.m_position += rel_pos;
        }
    }
};

// Triangle class with position vertices
class Triangle : public Obj {
public:
private:
    /* v0, v1, v2 are the indices of this array. The order matters
    for cross product and Triangle normal vector calculation. */
    FVector3 verts[vertsInTriangle];
};

// Return the normalized value based on a current and a max value.
template <typename T>
double normalize( T val, T max ) {
    return val / max;
}

// Convert 2D world coordinates to raster coordinates.
FVector2 world2RasterSpace( const FVector2& w_coords, const ImagePlane& imgPlane ) {
    return { w_coords.x - (imgPlane.resolution.x / 2),
             w_coords.y - (imgPlane.resolution.y / 2) };
}

// Convert raster coordinates to 2D world coordinates.
FVector2 raster2WorldSpace( const FVector2& w_coords, const ImagePlane& imgPlane ) {
    return { imgPlane.resolution.x / 2 + abs( w_coords.x ),
             imgPlane.resolution.y / 2 + abs( w_coords.y ) };
}

// Convert raster coodrinates to NDC (NSC - Normalized Screen Coordinates) [0.0, 1.0]
FVector2 raster2NDC( const FVector2& coords, const ImagePlane& imgPlane ) {
    return { coords.x / imgPlane.resolution.x, coords.y / imgPlane.resolution.y };
}

// Convert NDC to Screen Space (NSC to NDC - Normalized Device Coordinates) [-1.0, 1.0]
FVector2 NDC2ScreenSpace( const FVector2& coords ) {
    // Inverts Y
    return { (2.0 * coords.x) - 1.0, 1.0 - (2.0 * coords.y) };
    // No-invert version
     //return { (2.0 * coords.x) - 1.0, (2.0 * coords.y) -1.0 };
}

// Convert Screem Space to NDC (NDC to NSC) [0.0, 1.0]
FVector2 ScreenSpace2NDC( const FVector2& coords ) {
    // Inverts Y
    return { (coords.x + 1.0) / 2, (1.0 - coords.y) / 2 };
    // No-invert version
    //return { (coords.x + 1.0) / 2, (coords.y + 1.0) / 2 };
}

// Convert Screem Space to NDC (NDC to NSC) [0.0, 1.0] for a single value
double ScreenSpace2NDC( double val ) {
    return (val + 1.0) / 2;
}

// Convert Screem Space to NDC (NDC to NSC) [0.0, 1.0] for a single value, inverting it
double ScreenSpace2NDCInvert( double val ) {
    return (1.0 - val) / 2;
}

// Apply aspect ratio correction to an FVector2 (pixel).
FVector2 getFixedAspectRatio( const FVector2& vec, const ImagePlane& imgPlane ) {
    return { vec.x * (imgPlane.resolution.x / imgPlane.resolution.y), vec.y };
}

// Normalize a Camera Ray Vector.
FVector3 normalizeVector( const FVector3& vec ) {
    double vecLen{ sqrt( vec.x * vec.x + vec.y * vec.y + vec.z * vec.z ) };
    return { vec.x / vecLen, vec.y / vecLen, vec.z / vecLen };
}

/* A common function transforming a pixel from world space
directly to a Normalized Camera Ray Vector. */
FVector3 worldSpace2NormalizedVector( const FVector2& vec, const ImagePlane& imgPlane ) {
    FVector3 normRayDir{
        normalizeVector( {
            getFixedAspectRatio(
                NDC2ScreenSpace(
                    raster2NDC(
                        { vec.x, vec.y },
                        imgPlane
                    )
                ), imgPlane
            ), imgPlane.m_position.z
        } )
    };
    return { ScreenSpace2NDC( { normRayDir.x, normRayDir.y } ), ScreenSpace2NDC( normRayDir.z ) };
}

// Easy printing of a Vector2
std::ostream& operator<<( std::ostream& os, const Vector2& vec ) {
    os << "(" << std::setw( 4 ) << vec.x << ", " << std::setw( 4 ) << vec.y << ")";
    return os;
}

// Easy printing of a Color
std::ostream& operator<<( std::ostream& os, const Color& color ) {
    os << std::setw( 3 ) << color.r << " " << std::setw( 3 ) << color.g << " " << std::setw( 3 ) << color.b;
    return os;
}

int main() {
    const int width{ 1920 };
    const int height{ 1080 };
    const int maxColor_8bit{ 255 };
    FVector2 ndcCoords{};
    FVector2 screenCoords{};
    FVector2 fixedAspectCoords{};
    FVector3 normRayDir{};
    FVector3 ndcColor{};
    Color color{};
    Camera camera{ ImagePlane{width, height}, {0, 0, -1} };

    std::ofstream ppmFileStream( "CameraRays.ppm", std::ios::out | std::ios::binary );
    ppmFileStream << "P3\n";
    ppmFileStream << width << " " << height << "\n";
    ppmFileStream << maxColor_8bit << "\n";

    // Y = row, X = col
    for ( int y{}; y < height; ++y ) {
        for ( int x{}; x < width; ++x ) {
            ndcCoords = raster2NDC( { x + 0.5, y + 0.5 }, camera.m_imgPlane );
            screenCoords = NDC2ScreenSpace( ndcCoords );
            fixedAspectCoords = getFixedAspectRatio( screenCoords, camera.m_imgPlane );
            normRayDir = normalizeVector( FVector3{ fixedAspectCoords, camera.m_imgPlane.m_position.z } );
            ndcColor = { ScreenSpace2NDC( { normRayDir.x, normRayDir.y} ), ScreenSpace2NDC( normRayDir.z ) };
            // ndcColor = worldSpace2NormalizedVector( { x + 0.5, y + 0.5 }, camera.m_imgPlane );
            ppmFileStream << Color{ ndcColor.x, ndcColor.y, ndcColor.z } << "  ";
        }
        ppmFileStream << "\n";
        std::cout << "\rLine: " << y + 1 << " / " << height << std::flush;
    }

    ppmFileStream.close();

    return 0;
}
