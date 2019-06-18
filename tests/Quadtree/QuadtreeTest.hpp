#ifndef SPPAR_QUADTREE_TEST_HPP
#define SPPAR_QUADTREE_TEST_HPP

#include "../include/SPPAR/Quadtree.hpp"
#include <memory>
#include <utility>
#include <vector>
#include <chrono>
#include <random>
#include <ctime>

namespace SPPAR{
    namespace test{
        template<typename T,
             typename Distribution = std::uniform_int_distribution<T>,
             typename Engine = std::default_random_engine(std::random_device())>
        T random(T min,T max,Engine rEngine){
            Distribution dice(min,max);
            return dice(rEngine);
        }
        struct Entity{
            Entity(int id, float x, float y)
            : id(id) 
            , b(x,y,0,0){}
            int id;
            Rectf b;
            const Rectf& getPosition() const{ return b;}
        };
        std::unique_ptr<Quadtree<Entity>> createQtree(){
            return std::make_unique<Quadtree<Entity>>(Rectf(0,0,50,50));
        }
        TEST(QuadtreeTest,DefaultConstructor){
            auto qtree = createQtree();
            EXPECT_EQ(Rectf(0,0,50,50),qtree->getBounds());
            EXPECT_EQ(15u,qtree->getMaxCapacity());
            EXPECT_EQ(100u,qtree->getMaxLevel());
        }
        TEST(QuadtreeTest,Insert){
            auto qtree = createQtree();
            qtree->setMaxCapacity(1);
            qtree->setMaxLevel(2);
            std::vector<Entity> entities;
            entities.reserve(5);
            entities.emplace_back(0,12,12); // to node 0
            qtree->insert(&entities.back());
            entities.emplace_back(1,37,12); // to node 1
            qtree->insert(&entities.back());
            entities.emplace_back(2,12,37); // to node 2
            qtree->insert(&entities.back());
            entities.emplace_back(3,37,37); // to node 3
            qtree->insert(&entities.back());
            entities.emplace_back(4,37,37); // to root
            qtree->insert(&entities.back());
            for(auto e:qtree->getEntities(-1)){
                EXPECT_EQ(4,e->id);
            }
            for(auto e:qtree->getEntities(0)){
                EXPECT_EQ(0,e->id);
            }
            for(auto e:qtree->getEntities(1)){
                EXPECT_EQ(1,e->id);
            }
            for(auto e:qtree->getEntities(2)){
                EXPECT_EQ(2,e->id);
            }
            for(auto e:qtree->getEntities(3)){
                EXPECT_EQ(3,e->id);
            }
            EXPECT_TRUE(qtree->isSplit());
        }
        TEST(QuadtreeTest,Retrieve){
            auto qtree = createQtree();
            qtree->setMaxCapacity(2);
            qtree->setMaxLevel(2);
            std::random_device rd;
            std::mt19937 rEngine(rd());
            std::vector<Entity> entities;
            entities.reserve(100);
            for(auto i=0u;i<50u;++i){
                entities.emplace_back(0,random<int>(0,24,rEngine),0);
                qtree->insert(&entities.back());
            }
            for(auto i=0u;i<50u;++i){
                entities.emplace_back(1,random<int>(25,50,rEngine),0);
                qtree->insert(&entities.back());
            }
            auto insideRect0 = qtree->retrieve(&entities[0]);
            Rectf rect0(0,0,25,25);
            for(auto e:insideRect0){
                EXPECT_EQ(0,e->id);
                EXPECT_TRUE(rect0.contains(e->getPosition().left, e->getPosition().top));
            }
            auto insideRect1 = qtree->retrieve(&entities[51]);
            Rectf rect1(25,0,50,50);
            for(auto e:insideRect1){
                EXPECT_EQ(1,e->id);
                EXPECT_TRUE(rect1.contains(e->getPosition().left, e->getPosition().top));
            }
            EXPECT_TRUE(qtree->isSplit());
        }
        TEST(QuadtreeTest,setBounds){
            auto qtree = createQtree();
            Rectf r(0,0,1000,1000);
            qtree->setBounds(r);
            EXPECT_EQ(r,qtree->getBounds());
            std::vector<Entity> entities;
            for(auto i=0u;i<50u;++i){
                entities.emplace_back(i,i,0);
                qtree->insert(&entities.back());
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
        TEST(QuadtreeTest,Compiles){
            //Quadtree<int> qtree({0,0,1000,1000}); // invalid T so no compilation possible.
            Quadtree<Entity> qtree({0,0,1000,1000});
        }
    }
}

#endif // SPPAR_QUADTREE_TEST_HPP
