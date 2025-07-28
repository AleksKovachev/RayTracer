#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cstring>
#include "sstream"
#include <cmath>
#include <cstdint>
#include <stack>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <random>
#include <limits>

#include "Bases.h"
#include "Camera.h"
#include "Shapes.h"
#include "SpaceConversions.h"
#include "Vectors.h"


int main() {
    FVector3 vec1{ 3.5, 0 , 0 };
    FVector3 vec2{ 1.75, 3.5, 0 };
    std::cout << vec1 << " X " << vec2 << " = " << vec1 * vec2 << std::endl;

    FVector3 vec3{ 3, -3, 1 };
    FVector3 vec4{ 4, 9, 3 };
    std::cout << vec3 << " X " << vec4 << " = " << vec3 * vec4 << std::endl;

    std::cout << "Area of Parallelogram from " << vec3 << " and " << vec4 <<
        " is: " << (vec3 * vec4).getLength() << std::endl;
    std::cout << "Area of Triangle from " << vec3 << " and " << vec4 <<
        " is: " << (vec3 * vec4).getLength() / 2 << std::endl;

    FVector3 vec5{ -12, 12, -4 };
    std::cout << "Area of Parallelogram from " << vec3 << " and " << vec5 <<
        " is: " << (vec3 * vec5).getLength() << std::endl;

    Triangle tri1{ { -1.75, -1.75, -3 }, { 1.75, -1.75, -3 }, { 0, 1.75, -3 } };
    std::cout << "Triangle 1 normal: " << tri1.getNormalVector() << std::endl;
    std::cout << "Triangle 1 area: " << tri1.getArea() << std::endl;

    Triangle tri2{ { 0, 0, -1 }, { 1, 0, 1 }, { -1, 0, 1 } };
    std::cout << "Triangle 2 normal: " << tri2.getNormalVector() << std::endl;
    std::cout << "Triangle 2 area: " << tri2.getArea() << std::endl;

    Triangle tri3{ { 0.56, 1.11, 1.23 }, { 0.44, -2.368, -0.54 }, { -1.56, 0.15, -1.92 } };
    std::cout << "Triangle 3 normal: " << tri3.getNormalVector() << std::endl;
    std::cout << "Triangle 3 area: " << tri3.getArea() << std::endl;

    return 0;
}
