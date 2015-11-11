/**
Copyright (C)

creation date : 11/11/2015
file : graphical_element_instance.hpp
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


#ifndef GRAPHICAL_ELEMENT_INSTANCE_HPP
#define GRAPHICAL_ELEMENT_INSTANCE_HPP


#include "Graphical_Element.hpp"
#include "Sprite.hpp"


class Graphical_Element_Instance
{
    public:
        Graphical_Element_Instance();
        ~Graphical_Element_Instance();

    private:
        Graphical_Element model;
};


#endif
