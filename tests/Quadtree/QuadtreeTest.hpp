#ifndef SPPAR_QUADTREE_TEST_HPP
#define SPPAR_QUADTREE_TEST_HPP

#include "../include/SPPAR/Quadtree.hpp"
#include <memory>
#include <utility>
#include <vector>
#include <chrono>
#include <ctime>

namespace SPPAR{
    namespace test{
        std::unique_ptr<Quadtree<int>> createQtree(){
            return std::make_unique<Quadtree<int>>(Rectf(0,0,100,100),
                [](const int& a,Rectf bounds){
                    int index = -1;
                    if(a > 10000){
                        index = 0;
                    }else if(a > 1000){
                        index = 1;
                    }else if(a > 100){
                        index = 2;
                    }else if(a > 10){
                        index = 3;
                    }
                    return index;
            });
        }
        TEST(QuadtreeTest,DefaultConstructor){
            auto qtree = createQtree();
            EXPECT_EQ(Rectf(0,0,100,100),qtree->getBounds());
            EXPECT_EQ(15u,qtree->getMaxCapacity());
            EXPECT_EQ(100u,qtree->getMaxLevel());
        }
        TEST(QuadtreeTest,Insert){
            auto qtree = createQtree();
            std::vector<int> numbers(50);
            for(auto i=0u;i<50u;++i){
                numbers.emplace_back(i);
                qtree->insert(&numbers.back());
            }
            for(auto e:qtree->getEntities()){
                auto index = qtree->getPosition(*e);
                if(*e > 10){
                    EXPECT_EQ(3,index);
                }else if(*e < 10){
                    EXPECT_EQ(-1,index);
                }
            }
            EXPECT_TRUE(qtree->isSplit());
        }
        TEST(QuadtreeTest,Retrieve){
            auto qtree = createQtree();
            std::vector<int> numbers(50);
            for(auto i=0u;i<50u;++i){
                numbers.emplace_back(i);
                qtree->insert(&numbers.back());
            }
            auto under10 = qtree->retrieve(&numbers[0]);
            for(auto e:under10){
                EXPECT_LT(*e,11);
            }
            EXPECT_TRUE(qtree->isSplit());
        }
        TEST(QuadtreeTest,setBounds){
            auto qtree = createQtree();
            Rectf r(0,0,1000,1000);
            qtree->setBounds(r);
            EXPECT_EQ(r,qtree->getBounds());
            std::vector<int> numbers(50);
            for(auto i=0u;i<50u;++i){
                numbers.emplace_back(i);
                qtree->insert(&numbers.back());
            }
            EXPECT_TRUE(qtree->isSplit());
            auto node1Bounds = Rectf(500,500,500,500);
            auto currentNode1Bounds = qtree->getBounds(3);
            EXPECT_EQ(node1Bounds,currentNode1Bounds);
        }
        TEST(QuadtreeTest,setMaxCapacity){
            auto qtree = createQtree();
            qtree->setMaxCapacity(150);
            EXPECT_EQ(150u,qtree->getMaxCapacity());
        }
        TEST(QuadtreeTest,setMaxLevel){
            auto qtree = createQtree();
            qtree->setMaxLevel(1000);
            EXPECT_EQ(1000u,qtree->getMaxLevel());
        }
    }
}

#endif // SPPAR_QUADTREE_TEST_HPP
