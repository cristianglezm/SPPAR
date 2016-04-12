////////////////////////////////////////////////////////////
// Copyright 2014-2016 Cristian Glez <Cristian.glez.m@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
////////////////////////////////////////////////////////////

#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#ifdef RENDER_QTREE
    #include <SFML/Graphics/RectangleShape.hpp>
    #include <SFML/Graphics/RenderWindow.hpp>
#endif

#include <vector>
#include <array>
#include <algorithm>
#include <memory>

#include "Rect.hpp"

namespace SPPAR{
    /**
     * @class Quadtree
     * @brief Divides the space.
     * @tparam T class that will be used
     * @tparam BinaryPredicate Function that should return a
     * int between -1 and 3 which they correspond:
     * -# -1 The T stays at the parent node.
     * -# 0 The T goes into child node 0
     * -# 1 The T goes into child node 1
     * -# 2 The T goes into child node 2
     * -# 3 The T goes into child node 3
     * @code
     * [](const T& e, Rectf bounds) -> int{
     *      // check e position and decide which node it will go into.
     *      return -1; // stays in parent node.
     * }
     * @endcode
     * @author Cristian Glez <Cristian.glez.m@gmail.com>
     * @version 0.2
     */
    template<typename T,typename BinaryPredicate = int (*)(const T&, Rectf)>
    class Quadtree{
        public:
            /**
             * @brief Container that uses the quadtree.
             */
            using container = typename std::vector<T*>;
            /**
             * @brief Constructor
             * @param bounds Rect bounds of the quadtree.
             * @param gIndex function to insert entities to its appropiate node.
             */
            Quadtree(Rectf bounds, BinaryPredicate gIndex);
            /**
             * @brief adds a pointer from the entity and adds it to its appropiate node, 
             * if it cannot fit within a node, it will be inserted at the parent.
             * @Tparam e T
             */
            void insert(T* e) noexcept;
            /**
             * @brief getter for entities
             * @return container&
             */
            container& getEntities() noexcept;
            /**
             * @brief getter for entities of the node
             * @return container&
             */
            container& getEntities(std::size_t index) noexcept;
            /**
             * @brief Clears the quadtree and its nodes.
             */
            void clear() noexcept;
            /**
             * @brief Adds the entities(T) from the same space of the specified entity to the container(the specified entity is also added)
             *
             * @param eList Quadtree::container& to add the entities from the same space of the one provided.
             * @param e T* Entity to check.
             * @return Quadtree::container container of entities
             */
            void retrieve(T* e, container& eList) noexcept;
            /**
             * @brief Overload for retrieve without the container.
             * @param e
             * @return container
             */
            container retrieve(T* e) noexcept;
        #ifdef RENDER_QTREE
            /**
             * @brief Lets you see the borders of the quadtree
             * @param win sf::RenderWindow& Window to draw
             */
            void render(sf::RenderWindow& win);
        #endif
            /**
             * @brief sets the new bounds and updates the nodes if it has been split.
             * @param bounds
             * @return Quadtree<T,BinaryPredicate>&
             */
            Quadtree<T,BinaryPredicate>& setBounds(Rectf bounds) noexcept;
            /**
             * @brief getter for bounds
             * @return Rectf
             */
            Rectf getBounds() const noexcept;
            /**
             * @brief getter for bounds for node
             * @return Rectf
             */
            Rectf getBounds(int index) const noexcept;
            /**
             * @brief sets the new max capacity.
             * @param maxCap
             * @return Quadtree<T,BinaryPredicate>&
             */
            Quadtree<T,BinaryPredicate>& setMaxCapacity(std::size_t maxCap) noexcept;
            /**
             * @brief getter for maxCapacity
             * @return std::size_t
             */
            const std::size_t& getMaxCapacity() const noexcept;
            /**
             * @brief sets the new max Level
             * @param maxLvl
             * @return Quadtree<T,BinaryPredicate>&
             */
            Quadtree<T,BinaryPredicate>& setMaxLevel(std::size_t maxLvl) noexcept;
            /**
             * @brief getter for maxLevel
             * @return size_t
             */
            const std::size_t& getMaxLevel() const noexcept;
            /**
             * @brief getter for the index where T is in.
             * @param e
             * @return int
             */
            int getPosition(const T& e) const noexcept;
            /**
             * @brief setter for the index lambda
             * @param gIndex
             * @return Quadtree
             */
            Quadtree<T,BinaryPredicate>& setGetIndex(BinaryPredicate gIndex) noexcept;
            /**
             * @brief getter for node
             * @return Quadtree
             */
            Quadtree<T,BinaryPredicate>& getNode(std::size_t index) noexcept;
            /**
             * @brief checks if it has been splited
             * @return bool
             */
            bool isSplit() const noexcept;
            ~Quadtree() = default;
        private:
            /**
             * @brief Private constructor for nodes inside the quadtree.
             * @param level std::size_t the level it will be.
             * @param bounds Rect bounds of the node.
             * @tparam gIndex BinaryPredicate is a function that returns the
             * index to where the T is in -1 root number between 0 and 3 for nodes.
             */
            Quadtree(std::size_t level,Rectf bounds, BinaryPredicate gIndex);
            /**
             * @brief private method to subdivide the quadtree.
             */
            void split();
        private:
            std::size_t maxCapacity;
            std::size_t maxLevel;
            std::size_t level;
            BinaryPredicate getIndex;
            std::array<std::unique_ptr<Quadtree>,4> nodes;
            Rectf bounds;
            Quadtree::container entities;
    };
//////////////////////////////////////////////////
//////// Quadtree Impl
//////////////////////////////////////////////////
    template<typename T,class BinaryPredicate>
    Quadtree<T,BinaryPredicate>::Quadtree(std::size_t level, Rectf bounds, BinaryPredicate gIndex)
    : maxCapacity(15)
    , maxLevel(100)
    , level(level)
    , getIndex(gIndex)
    , nodes()
    , bounds(bounds)
    , entities(){
        for(auto i=0u;i<nodes.size();++i){
            nodes[i] = nullptr;
        }
    }
    template<typename T,class BinaryPredicate>
    void Quadtree<T,BinaryPredicate>::split(){
        int subWidth = (int)(bounds.width / 2);
        int subHeight = (int)(bounds.height / 2);
        int x = (int)bounds.left;
        int y = (int)bounds.top;
        nodes[0].reset(new Quadtree((level+1), Rectf(x + subWidth, y, subWidth, subHeight),getIndex));
        nodes[1].reset(new Quadtree((level+1), Rectf(x, y, subWidth, subHeight),getIndex));
        nodes[2].reset(new Quadtree((level+1), Rectf(x, y + subHeight, subWidth, subHeight),getIndex));
        nodes[3].reset(new Quadtree((level+1), Rectf(x + subWidth, y + subHeight, subWidth, subHeight),getIndex));
        for(auto i=0u;i<nodes.size();++i){
            nodes[i]->setMaxCapacity(maxCapacity);
            nodes[i]->setMaxLevel(maxLevel);
        }
    }
    template<typename T,class BinaryPredicate>
    Quadtree<T,BinaryPredicate>::Quadtree(Rectf bounds, BinaryPredicate gIndex)
    : maxCapacity(15)
    , maxLevel(100)
    , level(0)
    , getIndex(gIndex)
    , nodes()
    , bounds(bounds)
    , entities(){
        for(auto i=0u;i<nodes.size();++i){
            nodes[i] = nullptr;
        }
    }
    template<typename T,class BinaryPredicate>
    void Quadtree<T,BinaryPredicate>::insert(T* e) noexcept{
        if (isSplit()) {
            int index = getIndex(*e,bounds);
            if (index != -1) {
                nodes[index]->insert(e);
                return;
            }
        }
        entities.emplace_back(e);
        if(entities.size() > maxCapacity && level < maxLevel){
            if(!isSplit()) {
                split();
            }
            entities.erase(std::remove_if(std::begin(entities),std::end(entities),
                [&](typename container::value_type i){
                    int index = getIndex(*i,bounds);
                    if(index != -1){
                        nodes[index]->insert(i);
                        return true;
                    }
                    return false;
                }),std::end(entities));
        }
    }
    template<typename T, class BinaryPredicate>
    typename Quadtree<T,BinaryPredicate>::container& Quadtree<T,BinaryPredicate>::getEntities() noexcept{
        return entities;
    }
    template<typename T, class BinaryPredicate>
    typename Quadtree<T,BinaryPredicate>::container& Quadtree<T,BinaryPredicate>::getEntities(std::size_t index) noexcept{
        if(index == -1){
            return entities;
        }else if(isSplit()){
            return nodes[index]->getEntities();
        }
        return entities;
    }
    template<typename T,class BinaryPredicate>
    void Quadtree<T,BinaryPredicate>::clear() noexcept{
        entities.clear();
        if(isSplit()){
            for(auto i=0u;i<nodes.size();++i) {
                nodes[i]->clear();
                nodes[i] = nullptr;
            }
        }
    }
    template<typename T,class BinaryPredicate>
    typename Quadtree<T,BinaryPredicate>::container Quadtree<T,BinaryPredicate>::retrieve(T* e) noexcept{
        typename Quadtree<T,BinaryPredicate>::container entitiesList;
        retrieve(e, entitiesList);
        return entitiesList;
    }
    template<typename T,class BinaryPredicate>
    void Quadtree<T,BinaryPredicate>::retrieve(T* e, container& eList) noexcept{
        typename Quadtree<T,BinaryPredicate>::container internDst;
        int index = getIndex(*e,bounds);
        if(index != -1 && nodes[0] != nullptr){
            nodes[index]->retrieve(e,internDst);
        }
        std::sort(std::begin(entities),std::end(entities));
        std::sort(std::begin(internDst),std::end(internDst));
        std::merge(std::begin(entities),std::end(entities), 
                    std::begin(internDst),std::end(internDst),
                    std::back_inserter(eList));
    }
#ifdef RENDER_QTREE
    template<typename T,class BinaryPredicate>
    void Quadtree<T,BinaryPredicate>::render(sf::RenderWindow& win){
        sf::RectangleShape boundsShape(sf::Vector2f(bounds.width,bounds.height));
        boundsShape.setPosition(bounds.left,bounds.top);
        int b,g,r;
        b = level > 255 ? 255: level+bounds.left;
        g = level > 255 ? 255: level+bounds.top;
        r = level > 255 ? 255: level+(bounds.top+bounds.height);
        if(b> 255 || b <0){b=255;}
        if(g> 255 || g <0){g=255;}
        if(r> 255 || r <0){r=255;}
        sf::Color color(r,b,g,255);
        boundsShape.setOutlineThickness(0.5);
        boundsShape.setFillColor(sf::Color::Transparent);
        boundsShape.setOutlineColor(color);
        win.draw(boundsShape);
        if(isSplit()){
            for(auto i=0u;i<nodes.size();++i){
                nodes[i]->render(win);
            }
        }
    }
#endif
    template<typename T, class BinaryPredicate>
    Quadtree<T,BinaryPredicate>& Quadtree<T,BinaryPredicate>::setBounds(Rectf bounds) noexcept{
        this->bounds = bounds;
        if(isSplit()){
            int subWidth = static_cast<int>(bounds.width / 2);
            int subHeight = static_cast<int>(bounds.height / 2);
            int x = static_cast<int>(bounds.left);
            int y = static_cast<int>(bounds.top);
            nodes[0]->setBounds(Rectf(x + subWidth, y, subWidth, subHeight));
            nodes[1]->setBounds(Rectf(x, y, subWidth, subHeight));
            nodes[2]->setBounds(Rectf(x, y + subHeight, subWidth, subHeight));
            nodes[3]->setBounds(Rectf(x + subWidth, y + subHeight, subWidth, subHeight));
        }
        return *this;
    }
    template<typename T, class BinaryPredicate>
    Rectf Quadtree<T,BinaryPredicate>::getBounds() const noexcept{
        return bounds;
    }
    template<typename T, class BinaryPredicate>
    Rectf Quadtree<T,BinaryPredicate>::getBounds(int index) const noexcept{
        if(index == -1){
            return bounds;
        }else if(isSplit()){
            return nodes[index]->getBounds();
        }
        return bounds;
    }
    template<typename T, class BinaryPredicate>
    Quadtree<T,BinaryPredicate>& Quadtree<T,BinaryPredicate>::setMaxCapacity(std::size_t maxCap) noexcept{
        maxCapacity = maxCap;
        if(isSplit()){
            for(auto i=0u;i<nodes.size();++i){
                nodes[i]->setMaxCapacity(maxCap);
            }
        }
        return *this;
    }
    template<typename T, class BinaryPredicate>
    const std::size_t& Quadtree<T,BinaryPredicate>::getMaxCapacity() const noexcept{
        return maxCapacity;
    }
    template<typename T, class BinaryPredicate>
    Quadtree<T,BinaryPredicate>& Quadtree<T,BinaryPredicate>::setMaxLevel(std::size_t maxLvl) noexcept{
        maxLevel = maxLvl;
        if(isSplit()){
            for(auto i=0u;i<nodes.size();++i){
                nodes[i]->setMaxLevel(maxLvl);
            }
        }
        return *this;
    }
    template<typename T, class BinaryPredicate>
    const std::size_t& Quadtree<T,BinaryPredicate>::getMaxLevel() const noexcept{
        return maxLevel;
    }
    template<typename T, class BinaryPredicate>
    int Quadtree<T,BinaryPredicate>::getPosition(const T& e) const noexcept{
        return getIndex(e,bounds);
    }
    template<typename T, class BinaryPredicate>
    Quadtree<T,BinaryPredicate>& Quadtree<T,BinaryPredicate>::setGetIndex(BinaryPredicate gIndex) noexcept{
        getIndex = gIndex;
        return *this;
    }
    template<typename T, class BinaryPredicate>
    Quadtree<T,BinaryPredicate>& Quadtree<T,BinaryPredicate>::getNode(std::size_t index) noexcept{
        if(index == -1){
            return *this;
        }else if(isSplit()){
            return nodes[index].get();
        }
        return *this;
    }
    template<typename T, class BinaryPredicate>
    bool Quadtree<T,BinaryPredicate>::isSplit() const noexcept{
        return (nodes[0] != nullptr);
    }
}
#endif // QUADTREE_HPP
