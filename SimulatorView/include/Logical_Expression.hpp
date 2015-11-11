/**
Copyright (C)

creation date : 10/11/2015
file : logical_expression.hpp
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


#ifndef LOGICAL_EXPRESSION_HPP
#define LOGICAL_EXPRESSION_HPP


#include <memory>
#include <vector>

#include "Element_Instanciation.hpp"


class Logical_Expression
{
    public:
        Logical_Expression() {}
        virtual ~Logical_Expression() {}

        virtual bool isVerified(const std::shared_ptr<Element_Instanciation>& instance) const = 0;
};


class Logical_And_Expression : public Logical_Expression
{
    public:
        Logical_And_Expression(const std::vector<std::shared_ptr<Logical_Expression> >& members);

        bool isVerified(const std::shared_ptr<Element_Instanciation>& instance) const;

    private:
        std::vector<std::shared_ptr<Logical_Expression> > members;
};

class Logical_Or_Expression : public Logical_Expression
{
    public:
        Logical_Or_Expression(const std::vector<std::shared_ptr<Logical_Expression> >& members);

        bool isVerified(const std::shared_ptr<Element_Instanciation>& instance) const;

    private:
        std::vector<std::shared_ptr<Logical_Expression> > members;
};


#endif
