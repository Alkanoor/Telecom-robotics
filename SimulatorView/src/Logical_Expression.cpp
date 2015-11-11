#include "Logical_Expression.hpp"


bool Logical_Expression::isVerified(const std::shared_ptr<Element_Instanciation>& instance) const
{return false;}

Logical_And_Expression::Logical_And_Expression(const std::vector<std::shared_ptr<Logical_Expression> >& members)
{
    this->members = members;
}

bool Logical_And_Expression::isVerified(const std::shared_ptr<Element_Instanciation>& instance) const
{
    for(unsigned int i=0;i<members.size();i++)
        if(!members[i]->isVerified(instance))
            return false;
    return true;
}

Logical_Or_Expression::Logical_Or_Expression(const std::vector<std::shared_ptr<Logical_Expression> >& members)
{
    this->members = members;
}

bool Logical_Or_Expression::isVerified(const std::shared_ptr<Element_Instanciation>& instance) const
{
    for(unsigned int i=0;i<members.size();i++)
        if(members[i]->isVerified(instance))
            return true;
    return false;
}
