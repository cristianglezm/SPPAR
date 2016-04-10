#include <string>
#include <random>
#include <vector>
#include <iostream>
#include "../include/SPPAR/Quadtree.hpp"

struct Entity{
    Entity(SPPAR::Rectf b)
    : bounds(b){}
    SPPAR::Rectf bounds;
};
int main(){
    static std::mt19937 engine(std::random_device{}());
    std::uniform_real_distribution<> diceForX(0,50);
    std::uniform_real_distribution<> diceForY(0,50);
    SPPAR::Rectf screenBounds(0,0,50,50);
    SPPAR::Quadtree<Entity> qtree(screenBounds,[](const Entity& e, SPPAR::Rectf bounds){
        int index = -1;
	std::cout << "Bounds: " << bounds.top << ", " << bounds.left << ", " 
	            << bounds.width << ", " << bounds.height << std::endl;
        double verticalMidpoint = bounds.left + (bounds.width / 2);
        double horizontalMidpoint = bounds.top + (bounds.height / 2);
        // Object can completely fit within the top quadrants
        SPPAR::Rectf eBounds = e.bounds;
        bool topQuadrant = (eBounds.top < horizontalMidpoint && eBounds.top + eBounds.height < horizontalMidpoint);
        // Object can completely fit within the bottom quadrants
        bool bottomQuadrant = (eBounds.top > horizontalMidpoint);
        // Object can completely fit within the left quadrants
        if (eBounds.left < verticalMidpoint && eBounds.left + eBounds.width < verticalMidpoint) {
            if(topQuadrant) {
                index = 1;
                std::cout << "Entity is inside " << "top right" << std::endl;
            }else if(bottomQuadrant) {
                index = 2;
                std::cout << "Entity is inside " << "down left" << std::endl;
            }
        }
        // Object can completely fit within the right quadrants
        else if (eBounds.left > verticalMidpoint) {
            if (topQuadrant) {
                index = 0;
                std::cout << "Entity is inside " << "top left" << std::endl;
            }else if(bottomQuadrant){
                index = 3;
                std::cout << "Entity is inside " << "down right" << std::endl;
            }
        }
	if(index == -1){
	    std::cout << "Entity is inside root" << std::endl;
	}
        return index;
    });
    qtree.setMaxLevel(5);
    qtree.setMaxCapacity(1);
    std::vector<Entity> entities;
    for(auto i=0u;i<10;++i){
        entities.emplace_back(Entity(SPPAR::Rectf(diceForX(engine),diceForY(engine),2,2)));
    }
    for(auto i=0u;i<entities.size();++i){
        qtree.insert(&entities[i]);
    }
    for(auto& e1:entities){
        auto entts = qtree.retrieve(&e1);
        for(auto& e2:entts){
            // check for collisions of e1 with e2.
        }
    }
    return 0;
}
