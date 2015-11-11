/**
Copyright (C)

creation date : 08/11/2015
file : element.hpp
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


#ifndef ELEMENT_HPP
#define ELEMENT_HPP


#include <map>

#include "log/Log_Init.hpp"


class Element
{
    public:
        Element(const std::string& name, const std::shared_ptr<Element>& father = std::shared_ptr<Element>());

        void addAttribute(const std::string& name);
        void addStaticVariable(const std::string& name);

        const std::string& getName() const;

        template <typename T>
        void print(T& logger);

        static bool varExists(const std::string& nameVar);
        static void constructModelFromFile(const std::string& file);
        static const std::map<std::string,std::shared_ptr<Element> >& getModel();

    private:
        std::string name;
        std::shared_ptr<Element> father;
        std::vector<std::string> attributes;
        std::vector<std::string> staticVariables;

        static std::map<std::string,bool> allVariables;
        static std::map<std::string,std::shared_ptr<Element> > model;
};


template <typename T>
void Element::print(T& logger)
{
    if(father)
        logger.write("Has father ? ",true);
    else
        logger.write("Has father ? ",false);

    if(attributes.size())
        logger.write("Attributes : ");
    for(unsigned int i=0;i<attributes.size();i++)
        logger.write(attributes[i]);

    if(staticVariables.size())
        logger.write("Static vars : ");
    for(unsigned int i=0;i<staticVariables.size();i++)
        logger.write(staticVariables[i]);
}


#endif
