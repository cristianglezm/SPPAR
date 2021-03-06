#include <iostream>
#include <gtest/gtest.h>
#include "Quadtree/QuadtreeTest.hpp"
#include "Rect/RectTest.hpp"

int main(int argc, char** argv){
    std::cout << "Initializing Tests...." << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
