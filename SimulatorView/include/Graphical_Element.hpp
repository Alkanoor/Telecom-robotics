/**
Copyright (C)

creation date : 09/11/2015
file : graphical_element.hpp
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


#ifndef GRAPHICAL_ELEMENT_HPP
#define GRAPHICAL_ELEMENT_HPP


#include <sstream>

#include "Element_Instanciation.hpp"
#include "Logical_Expression.hpp"
#include "Element.hpp"
#include "Sprite.hpp"


class Graphical_Element
{
    public:
        Graphical_Element(const std::shared_ptr<Element>& model);

        const Sprite& getSpriteAtIndex(unsigned int index);
        const Sprite& getFirstSpriteCorrespondingTo(const std::shared_ptr<Element_Instanciation>& instance);

        char addPredicate(const std::string& pred);
        bool addPropertyToCurrentPredicate(const std::string& property, const std::string& value);

        static void constructViewFromFile(const std::string& path);
        static std::map<std::string,std::shared_ptr<Graphical_Element> > getViewModel();

    private:
        std::shared_ptr<Element> abstractElement;
        std::vector<Sprite> sprites;
        std::vector<std::shared_ptr<Logical_Expression> > predicates;

        static std::map<std::string,std::shared_ptr<Graphical_Element> > viewModel;
        static std::vector<std::string> splitOutOfParentheses(const std::string& str, const std::string& separator);

        std::shared_ptr<Logical_Expression> addPredicateRecursive(const std::string& pred, char& errCode);
};


#endif
