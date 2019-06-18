#include <string>
#include <random>
#include <vector>
#include <iostream>
#include "../include/SPPAR/Quadtree.hpp"

struct Entity{
    Entity(SPPAR::Rectf b)
    : bounds(b){}
    const SPPAR::Rectf getPosition() const{
        return bounds;
    }
    SPPAR::Rectf bounds;
};
int main(){
    static std::mt19937 engine(std::random_device{}());
    std::uniform_real_distribution<> diceForX(0,50);
    std::uniform_real_distribution<> diceForY(0,50);
    SPPAR::Rectf screenBounds(0,0,50,50);
    SPPAR::Quadtree<Entity> qtree(screenBounds);
    qtree.setMaxLevel(5);
    qtree.setMaxCapacity(1);
    std::vector<Entity> entities;
    entities.reserve(10); //
    std::cout << "Adding entities..." << std::endl;
    for(auto i=0u;i<10;++i){
        entities.emplace_back(Entity(SPPAR::Rectf(diceForX(engine),diceForY(engine),2,2)));
        auto e = &entities.back();
        std::cout << "[" << e->bounds.left << ", " << e->bounds.top << "]" << std::endl;
        qtree.insert(e);
    }
    std::cout << "Checking for collisions..." << std::endl;
    for(auto& e1:entities){
        auto entts = qtree.retrieve(&e1);
        std::cout << "e1[" << e1.bounds.left << ", " << e1.bounds.top << "]" << std::endl;
        for(auto& e2:entts){
            if((&e1) == e2){
                continue;
            }
            std::cout << "e2[" << e2->bounds.left << ", " << e2->bounds.top << "]" << std::endl;
            // check for collisions of e1 with e2.
            std::cout << "e1 collides with e2: " << (e1.bounds.intersects(e2->bounds) ? "True":"False") << std::endl;
        }
    }
    return 0;
}