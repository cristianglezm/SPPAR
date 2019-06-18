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

#ifndef SPPAR_QUADTREE_HPP
#define SPPAR_QUADTREE_HPP

#ifdef RENDER_QTREE
    #include <SFML/Graphics/RectangleShape.hpp>
    #include <SFML/Graphics/RenderWindow.hpp>
#endif

#include <vector>
#include <array>
#include <algorithm>
#include <memory>
#include <type_traits>

#include "Rect.hpp"

namespace SPPAR{
    /**
     * @class Quadtree
     * @brief Divides the space into four rectangles.
     * 
     * @tparam T class that will be used, it will be saved as a pointer
     * @author Cristian Glez <Cristian.glez.m@gmail.com>
     * @version 0.3
     */
    template<class T>
    class Quadtree{
            static_assert(std::is_member_function_pointer<decltype(&T::getPosition)>::value, "T need to have function 'const Rectf& getPosition() const'");
        public:
            /**
             * @brief Container that uses the quadtree.
             * we use a std::vector<T*> @todo explain more
             */
            using Container = typename std::vector<T*>;
            /**
             * @brief Constructor
             * @param bounds Rect bounds of the quadtree.
             */
            Quadtree(const Rectf& bounds);
            /**
             * @brief adds a pointer from the entity and adds it to its appropriate node, 
             * if it cannot fit within a node, it will be inserted at the parent.
             * @Tparam e T*
             */
            void insert(T* e) noexcept;
            /**
             * @brief getter for entities
             * @return Container&
             */
            Container& getEntities() noexcept;
            /**
             * @brief getter for entities of the node
             * @return Container&
             */
            Container& getEntities(std::size_t index) noexcept;
            /**
             * @brief Clears the quadtree and its nodes.
             */
            void clear() noexcept;
            /**
             * @brief Adds the entities(T) from the same space of the specified entity to the Container(the specified entity is also added)
             *
             * @param eList Quadtree::Container& to add the entities from the same space of the one provided.
             * @param e T* Entity to check.
             * @return Quadtree::Container Container of entities
             */
            void retrieve(T* e, Container& eList) noexcept;
            /**
             * @brief Overload for retrieve without the Container.
             * @param e T*
             * @return Container
             */
            Container retrieve(T* e) noexcept;
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
             * @return Quadtree<T>&
             */
            Quadtree<T>& setBounds(const Rectf& bounds) noexcept;
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
            Quadtree<T>& setMaxCapacity(std::size_t maxCap) noexcept;
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
            Quadtree<T>& setMaxLevel(std::size_t maxLvl) noexcept;
            /**
             * @brief getter for maxLevel
             * @return size_t
             */
            const std::size_t& getMaxLevel() const noexcept;
            /**
             * @brief getter for node
             * @return Quadtree
             */
            Quadtree<T>& getNode(std::size_t index) noexcept;
            /**
             * @brief Direct Access for Quadtree nodes, it doesn't check bounds or if it is splited.
             * @return Quadtree &
             */
            Quadtree<T>& operator[](std::size_t index) noexcept;
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
             */
            Quadtree(std::size_t level,const Rectf& bounds);
            /**
             * @brief getter for the index where T is in.
             * @param e
             * @return int if return is -1 is in the parent node else in the given index.
             */
            int getIndex(const T& e) const noexcept;
            /**
             * @brief private method to subdivide the quadtree.
             */
            void split();
        private:
            std::size_t maxCapacity;
            std::size_t maxLevel;
            std::size_t level;
            std::array<std::unique_ptr<Quadtree>,4> nodes;
            Rectf bounds;
            Quadtree::Container entities;
    };
//////////////////////////////////////////////////
//////// Quadtree Impl
//////////////////////////////////////////////////
    template<class T>
    Quadtree<T>::Quadtree(std::size_t level, const Rectf& bounds)
    : maxCapacity(15)
    , maxLevel(100)
    , level(level)
    , nodes()
    , bounds(bounds)
    , entities(){
        for(auto i=0u;i<nodes.size();++i){
            nodes[i] = nullptr;
        }
    }
    template<class T>
    void Quadtree<T>::split(){
        int subWidth = static_cast<int>(bounds.width / 2);
        int subHeight = static_cast<int>(bounds.height / 2);
        int x = static_cast<int>(bounds.left);
        int y = static_cast<int>(bounds.top);
        nodes[0].reset(new Quadtree<T>((level+1), Rectf(x, y, subWidth, subHeight)));
        nodes[1].reset(new Quadtree<T>((level+1), Rectf(x + subWidth, y, subWidth, subHeight)));
        nodes[2].reset(new Quadtree<T>((level+1), Rectf(x, y + subHeight, subWidth, subHeight)));
        nodes[3].reset(new Quadtree<T>((level+1), Rectf(x + subWidth, y + subHeight, subWidth, subHeight)));
        for(auto i=0u;i<nodes.size();++i){
            nodes[i]->setMaxCapacity(maxCapacity);
            nodes[i]->setMaxLevel(maxLevel);
        }
    }
    template<class T>
    Quadtree<T>::Quadtree(const Rectf& bounds)
    : maxCapacity(15)
    , maxLevel(100)
    , level(0)
    , nodes()
    , bounds(bounds)
    , entities(){
        for(auto i=0u;i<nodes.size();++i){
            nodes[i] = nullptr;
        }
    }
    template<class T>
    void Quadtree<T>::insert(T* e) noexcept{
        if(isSplit()){
            int index = getIndex(*e);
            if(index != -1){
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
                [&](typename Container::value_type i){
                    int index = getIndex(*i);
                    if(index != -1){
                        nodes[index]->insert(i);
                        return true;
                    }
                    return false;
                }),std::end(entities));
        }
    }
    template<class T>
    typename Quadtree<T>::Container& Quadtree<T>::getEntities() noexcept{
        return entities;
    }
    template<class T>
    typename Quadtree<T>::Container& Quadtree<T>::getEntities(std::size_t index) noexcept{
        if(index == -1){
            return entities;
        }else if(isSplit()){
            return nodes[index]->getEntities();
        }
        return entities;
    }
    template<class T>
    void Quadtree<T>::clear() noexcept{
        entities.clear();
        if(isSplit()){
            for(auto i=0u;i<nodes.size();++i) {
                nodes[i]->clear();
                nodes[i] = nullptr;
            }
        }
    }
    template<class T>
    typename Quadtree<T>::Container Quadtree<T>::retrieve(T* e) noexcept{
        typename Quadtree<T>::Container entitiesList;
        retrieve(e, entitiesList);
        return entitiesList;
    }
    template<class T>
    void Quadtree<T>::retrieve(T* e, Container& eList) noexcept{
        typename Quadtree<T>::Container internDst;
        int index = getIndex(*e);
        if(index != -1 && isSplit()){
            nodes[index]->retrieve(e,internDst);
        }
        std::sort(std::begin(entities),std::end(entities));
        std::sort(std::begin(internDst),std::end(internDst));
        std::merge(std::begin(entities),std::end(entities), 
                    std::begin(internDst),std::end(internDst),
                    std::back_inserter(eList));
    }
#ifdef RENDER_QTREE
    template<class T>
    void Quadtree<T>::render(sf::RenderWindow& win){
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
    template<class T>
    Quadtree<T>& Quadtree<T>::setBounds(const Rectf& bounds) noexcept{
        this->bounds = bounds;
        if(isSplit()){
            int subWidth = static_cast<int>(bounds.width / 2);
            int subHeight = static_cast<int>(bounds.height / 2);
            int x = static_cast<int>(bounds.left);
            int y = static_cast<int>(bounds.top);
            nodes[0]->setBounds(Rectf(x, y, subWidth, subHeight));
            nodes[1]->setBounds(Rectf(x + subWidth, y, subWidth, subHeight));
            nodes[2]->setBounds(Rectf(x, y + subHeight, subWidth, subHeight));
            nodes[3]->setBounds(Rectf(x + subWidth, y + subHeight, subWidth, subHeight));
        }
        return *this;
    }
    template<class T>
    Rectf Quadtree<T>::getBounds() const noexcept{
        return bounds;
    }
    template<class T>
    Rectf Quadtree<T>::getBounds(int index) const noexcept{
        if(index == -1){
            return bounds;
        }else if(isSplit()){
            return nodes[index]->getBounds();
        }
        return bounds;
    }
    template<class T>
    Quadtree<T>& Quadtree<T>::setMaxCapacity(std::size_t maxCap) noexcept{
        maxCapacity = maxCap;
        if(isSplit()){
            for(auto i=0u;i<nodes.size();++i){
                nodes[i]->setMaxCapacity(maxCap);
            }
        }
        return *this;
    }
    template<class T>
    const std::size_t& Quadtree<T>::getMaxCapacity() const noexcept{
        return maxCapacity;
    }
    template<class T>
    Quadtree<T>& Quadtree<T>::setMaxLevel(std::size_t maxLvl) noexcept{
        maxLevel = maxLvl;
        if(isSplit()){
            for(auto i=0u;i<nodes.size();++i){
                nodes[i]->setMaxLevel(maxLevel);
            }
        }
        return *this;
    }
    template<class T>
    const std::size_t& Quadtree<T>::getMaxLevel() const noexcept{
        return maxLevel;
    }
    template<class T>
    int Quadtree<T>::getIndex(const T& e) const noexcept{
        int index = -1;
        if(!isSplit()){
            return index;
        }
        auto leftTop = nodes[0]->getBounds();
        auto rightTop = nodes[1]->getBounds();
        auto leftBottom = nodes[2]->getBounds();
        auto rightBottom = nodes[3]->getBounds();
        auto pos = e.getPosition();
        if(leftTop.contains(pos.left,pos.top)){
            index = 0;
        }else if(rightTop.contains(pos.left,pos.top)){
            index = 1;
        }else if(leftBottom.contains(pos.left,pos.top)){
            index = 2;
        }else if(rightBottom.contains(pos.left,pos.top)){
            index = 3;
        }
        return index;
    }
    template<class T>
    Quadtree<T>& Quadtree<T>::getNode(std::size_t index) noexcept{
        if(isSplit()){
            return *nodes[index].get();
        }
        return *this;
    }
    template<class T>
    Quadtree<T>& Quadtree<T>::operator[](std::size_t index) noexcept{
        return *nodes[index].get();
    }
    template<class T>
    bool Quadtree<T>::isSplit() const noexcept{
        return (nodes[0] != nullptr);
    }
}
#endif // SPPAR_QUADTREE_HPP