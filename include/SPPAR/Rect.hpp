////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef RECT_HPP
#define RECT_HPP

namespace SPPAR{
    /**
     * @class Rect
     * @author Cristian Glez <cristian.glez.m@gmail.com>
     * @brief modified SFML Rect
     */
    template<typename T>
    class Rect{
        public:
            Rect();
            Rect(T left, T top, T width, T height);
            bool contains(T x,T y) const noexcept;
            bool contains(const Rect<T>& rectangle) const noexcept;
            bool intersects(const Rect<T>& rectangle) const noexcept;
            bool intersects(const Rect<T>& rectangle, Rect<T>& intersection) const noexcept;
            ~Rect() = default;
        public:
            T top;
            T left;
            T width;
            T height;
    };
    using Rectf = Rect<float>;
    using Recti = Rect<int>;
    
/////////////////////////////////////
////// Rect Impl
/////////////////////////////////////
    template<typename T>
    Rect<T>::Rect()
    : top(0)
    , left(0)
    , width(0)
    , height(0){}
    template<typename T>
    Rect<T>::Rect(T left, T top, T width, T height)
    : top(top)
    , left(left)
    , width(width)
    , height(height){}
    template<typename T>
    bool Rect<T>::contains(T x, T y) const noexcept{
        T minLeft = std::min(left, static_cast<T>(left + width));
        T maxLeft = std::max(left, static_cast<T>(left + width));
        T minRight = std::min(top, static_cast<T>(top + height));
        T maxRight = std::max(top, static_cast<T>(top + height));
        return (x >= minLeft) && (x < maxLeft) && (y >= minRight) && (y < maxRight);
    }
    template<typename T>
    bool Rect<T>::contains(const Rect<T>& rectangle) const noexcept{
        return contains(rectangle.left, rectangle.top) &&
                contains(rectangle.left + rectangle.width, rectangle.top) &&
                contains(rectangle.left, rectangle.top + rectangle.height) &&
                contains(rectangle.left + rectangle.width, rectangle.top + rectangle.height);
    }
    template <typename T>
    bool Rect<T>::intersects(const Rect<T>& rectangle) const noexcept{
        Rect<T> intersection;
        return intersects(rectangle, intersection);
    }
    template <typename T>
    bool Rect<T>::intersects(const Rect<T>& rectangle, Rect<T>& intersection) const noexcept{
        T r1MinX = std::min(left, static_cast<T>(left + width));
        T r1MaxX = std::max(left, static_cast<T>(left + width));
        T r1MinY = std::min(top, static_cast<T>(top + height));
        T r1MaxY = std::max(top, static_cast<T>(top + height));
        
        T r2MinX = std::min(rectangle.left, static_cast<T>(rectangle.left + rectangle.width));
        T r2MaxX = std::max(rectangle.left, static_cast<T>(rectangle.left + rectangle.width));
        T r2MinY = std::min(rectangle.top, static_cast<T>(rectangle.top + rectangle.height));
        T r2MaxY = std::max(rectangle.top, static_cast<T>(rectangle.top + rectangle.height));
        
        T interLeft   = std::max(r1MinX, r2MinX);
        T interTop    = std::max(r1MinY, r2MinY);
        T interRight  = std::min(r1MaxX, r2MaxX);
        T interBottom = std::min(r1MaxY, r2MaxY);
        
        if ((interLeft < interRight) && (interTop < interBottom)){
            intersection = Rect<T>(interLeft, interTop, interRight - interLeft, interBottom - interTop);
            return true;
        }else{
            intersection = Rect<T>(0, 0, 0, 0);
            return false;
        }
    }
    template <typename T>
    inline bool operator==(const Rect<T>& left, const Rect<T>& right){
        return (left.left == right.left) && (left.width == right.width) &&
               (left.top == right.top) && (left.height == right.height);
    }
    template <typename T>
    inline bool operator!=(const Rect<T>& left, const Rect<T>& right){
        return !(left == right);
    }
}
#endif // RECT_HPP