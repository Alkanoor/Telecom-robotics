/**
Copyright (C)

creation date : 10/11/2015
file : logical_basic_expression.hpp
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


#ifndef LOGICAL_BASIC_EXPRESSION_HPP_INCLUDED
#define LOGICAL_BASIC_EXPRESSION_HPP_INCLUDED


#include <functional>
#include <string>
#include <map>

#include "Logical_Expression.hpp"


enum Operator{EQUAL,NOT_EQUAL,SUPERIOR,STRICT_SUPERIOR,INFERIOR,STRICT_INFERIOR};
template <typename T>
class Logical_Basic_Expression : public Logical_Expression
{
    public:
        Logical_Basic_Expression(const std::string& nameVar, Operator operation, T refVal);

        bool isVerified(const std::shared_ptr<Element_Instanciation>& instance) const;

        static std::map<Operator,std::function<bool(T,T)> > operationsTable;

    private:
        std::string nameVar;
        Operator operation;
        T refVal;
};

template <typename T>
std::map<Operator,std::function<bool(T,T)> > Logical_Basic_Expression<T>::operationsTable =
                                                    {{EQUAL,[](T a,T b){return a==b;}},
                                                    {NOT_EQUAL,[](T a,T b){return a!=b;}},
                                                    {SUPERIOR,[](T a,T b){return a>=b;}},
                                                    {STRICT_SUPERIOR,[](T a,T b){return a>b;}},
                                                    {INFERIOR,[](T a,T b){return a<=b;}},
                                                    {STRICT_INFERIOR,[](T a,T b){return a<b;}}};

template <typename T>
Logical_Basic_Expression<T>::Logical_Basic_Expression(const std::string& nameVar, Operator operation, T refVal)
{
    this->nameVar = nameVar;
    this->operation = operation;
    this->refVal = refVal;
}

template <typename T>
bool Logical_Basic_Expression<T>::isVerified(const std::shared_ptr<Element_Instanciation>& instance) const
{
    return operationsTable[operation](instance->getValueOfAttributeOrStatic<T>(nameVar),refVal);
}


#endif
