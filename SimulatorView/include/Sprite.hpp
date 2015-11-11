/**
Copyright (C)

creation date : 08/11/2015
file : sprite.hpp
author : Julien Schoumacher

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**/


#ifndef SPRITE_HPP
#define SPRITE_HPP


#include <SFML/Graphics.hpp>
#include <boost/regex.hpp>


class Sprite
{
    public:
        Sprite();

        bool update(const std::string& parameter, const std::string& value);
        bool createSprite();

        const sf::Sprite& getSprite() const;

        static const Sprite& emptySprite();

    private:
        std::string imagePath;
        int alpha;
        sf::Color colorToAdd;
        sf::Color transparencyColor;

        sf::Image img;
        sf::Texture text;
        sf::Sprite sprite;

        static Sprite nullSprite;
};


#endif
