/**
Copyright (C)

creation date : 08/11/2015
file : element_instanciation.hpp
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


#ifndef ELEMENTINSTANCIATION_H
#define ELEMENTINSTANCIATION_H


#include <tuple>

#include "Element.hpp"


class Element_Instanciation
{
    public:
        Element_Instanciation();

        void update(const std::string& attributeName, const std::string& newVal);
        bool hasChanged(const std::string& attributeName);

        template <typename T>
        T getValueOfAttributeOrStatic(const std::string& attributeName) const {return T(0);}

    private:
        Element& model;
        std::map<std::string,std::string> values;
        std::map<std::string,std::string> staticValues;
};


#endif
