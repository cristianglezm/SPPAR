#ifndef SPPAR_RECT_TEST_HPP
#define SPPAR_RECT_TEST_HPP

#include "../include/SPPAR/Rect.hpp"

namespace SPPAR{
    namespace test{
        TEST(RectTest,constructors){
		Rectf r1(0,0,100,100);
		EXPECT_EQ(0,r1.left);
		EXPECT_EQ(0,r1.top);
		EXPECT_EQ(100,r1.width);
		EXPECT_EQ(100,r1.height);
		Rectf r2;
		EXPECT_EQ(0,r2.left);
		EXPECT_EQ(0,r2.top);
		EXPECT_EQ(0,r2.width);
		EXPECT_EQ(0,r2.height);
	}
	TEST(RectTest, contains){
		Rectf r1(0,0,100,100);
		EXPECT_TRUE(r1.contains(0,2));
		EXPECT_TRUE(r1.contains(0,99));
		EXPECT_FALSE(r1.contains(-1,-55));
		EXPECT_FALSE(r1.contains(0,150));
		Rectf biggerThanR1(-50,-50,150,150);
		Rectf smallerThanR1(0,0,50,50);
		EXPECT_FALSE(r1.contains(biggerThanR1));
		EXPECT_TRUE(r1.contains(smallerThanR1));		
	}
	TEST(RectTest, intersects){
		Rectf r1(0,0,100,100);
		Rectf inter(99,99,50,50);
		Rectf noInter(101,101,50,50);
		EXPECT_TRUE(r1.intersects(inter));
		Rectf result1;
		EXPECT_TRUE(r1.intersects(inter,result1));
		EXPECT_EQ(Rectf(99,99,1,1),result1);
		EXPECT_FALSE(r1.intersects(noInter));
		Rectf result2;
		EXPECT_FALSE(r1.intersects(noInter,result2));
		EXPECT_EQ(result2,Rectf());
	}
    }
}

#endif // SPPAR_RECT_TEST_HPP