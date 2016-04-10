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
	float subWidth = bounds.width / 2;
	float subHeight = bounds.height / 2;
	float x = bounds.left;
	float y = bounds.top;
	std::cout << "Bounds: " << bounds.top << ", " << bounds.left << ", " 
	            << bounds.width << ", " << bounds.height << std::endl;
	SPPAR::Rectf rightTop(x + subWidth,y,subWidth,subHeight);
	SPPAR::Rectf leftTop(x,y,subWidth,subHeight);
	SPPAR::Rectf leftBottom(x,y+subHeight,subWidth,subHeight);
	SPPAR::Rectf rightBottom(x+subWidth,y+subHeight,subWidth,subHeight);

	if(rightTop.contains(e.bounds)){
		index = 0;
                std::cout << "Entity is inside " << "top right" << std::endl;
	}else if(leftTop.contains(e.bounds)){
                std::cout << "Entity is inside " << "top left" << std::endl;
		index = 1;
	}else if(leftBottom.contains(e.bounds)){
		index = 2;
                std::cout << "Entity is inside " << "down left" << std::endl;
	}else if(rightBottom.contains(e.bounds)){
		index = 3;
                std::cout << "Entity is inside " << "down right" << std::endl;
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
