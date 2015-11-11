#include "Graphical_Element.hpp"


std::map<std::string,std::shared_ptr<Graphical_Element> > Graphical_Element::viewModel;

Graphical_Element::Graphical_Element(const std::shared_ptr<Element>& model) :
    abstractElement(model)
{}

const Sprite& Graphical_Element::getSpriteAtIndex(unsigned int index)
{
    if(index<sprites.size())
        return sprites[index];

    Log_Init::getErrorLog()->warning("No sprite at index ",index,", empty sprite returned");
    return Sprite::emptySprite();
}

const Sprite& Graphical_Element::getFirstSpriteCorrespondingTo(const std::shared_ptr<Element_Instanciation>& instance)
{
    for(unsigned int i=0;i<predicates.size();i++)
        if(predicates[i]->isVerified(instance))
            return sprites[i];

    Log_Init::getErrorLog()->warning("No sprite matching conditions for graphical element \""+abstractElement->getName()+"\", empty sprite returned");
    return Sprite::emptySprite();
}

std::shared_ptr<Logical_Expression> Graphical_Element::addPredicateRecursive(const std::string& pred, char& errCode)
{
    boost::regex reg("\\s*\\((.*)\\)\\s*");
    boost::smatch match;

    std::string toWorkOn = pred;
    if(boost::regex_match(pred, match, reg))
        toWorkOn = match[1];

    std::vector<std::string> splitAnd = splitOutOfParentheses(toWorkOn,"&&");
    if(!splitAnd.size())
    {
        errCode = 1;
        return std::shared_ptr<Logical_Expression>();
    }
    for(unsigned int i=0;i<splitAnd.size();i++)
        std::cout<<splitAnd[i]<<std::endl;

    std::vector<std::string> splitOr = splitOutOfParentheses(toWorkOn,"||");
    if(!splitOr.size())
    {
        errCode = 1;
        return std::shared_ptr<Logical_Expression>();
    }
    for(unsigned int i=0;i<splitOr.size();i++)
        std::cout<<splitOr[i]<<std::endl;

    if(splitAnd.size()>1&&splitOr.size()>1)
    {
        errCode = 2;
        return std::shared_ptr<Logical_Expression>();
    }

    if(splitAnd.size()>1)
    {
        std::vector<std::shared_ptr<Logical_Expression> > combination(splitAnd.size());
        for(unsigned int i=0;i<splitAnd.size();i++)
        {
            combination[i] = addPredicateRecursive(splitAnd[i],errCode);
            if(errCode)
                return std::shared_ptr<Logical_Expression>();
        }
        return std::shared_ptr<Logical_Expression>(new Logical_And_Expression(combination));
    }
    else if(splitOr.size()>1)
    {
        std::vector<std::shared_ptr<Logical_Expression> > combination(splitOr.size());
        for(unsigned int i=0;i<splitOr.size();i++)
        {
            combination[i] = addPredicateRecursive(splitOr[i],errCode);
            if(errCode)
                return std::shared_ptr<Logical_Expression>();
        }
        return std::shared_ptr<Logical_Expression>(new Logical_Or_Expression(combination));
    }
    else
    {
        boost::regex regPredicate("\\s*([a-zA-Z0-9_]*)\\s*(!=|==|<=|>=|<|>)\\s*(.*)");
        if(boost::regex_search(toWorkOn, match, regPredicate))
        {
            std::string nameVar(match[1]);
            if(!nameVar.size())
            {
                errCode = 4;
                return std::shared_ptr<Logical_Expression>();
            }
            if(!Element::varExists(nameVar))
            {
                errCode = 5;
                return std::shared_ptr<Logical_Expression>();
            }

            std::string operation(match[2]);
            std::string value(match[3]);
            std::cout<<"Name var : "<<nameVar<<" operation : "<<operation<<"  Value : "<<value<<std::endl;
            if(operation=="==")
                return std::shared_ptr<Logical_Expression>(new Logical_Basic_Expression(nameVar,EQUAL,value));
            else if(operation=="!=")
                return std::shared_ptr<Logical_Expression>(new Logical_Basic_Expression(nameVar,NOT_EQUAL,value));
            else if(operation==">=")
                return std::shared_ptr<Logical_Expression>(new Logical_Basic_Expression(nameVar,SUPERIOR,value));
            else if(operation==">")
                return std::shared_ptr<Logical_Expression>(new Logical_Basic_Expression(nameVar,STRICT_SUPERIOR,value));
            else if(operation=="<=")
                return std::shared_ptr<Logical_Expression>(new Logical_Basic_Expression(nameVar,INFERIOR,value));
            else if(operation=="<")
                return std::shared_ptr<Logical_Expression>(new Logical_Basic_Expression(nameVar,STRICT_INFERIOR,value));
            else
            {
                errCode = 3;
                return std::shared_ptr<Logical_Expression>();
            }
        }
        else
        {
            errCode = 6;
            return std::shared_ptr<Logical_Expression>();
        }
    }
}

char Graphical_Element::addPredicate(const std::string& pred)
{
    char ret = 0;
    std::shared_ptr<Logical_Expression> expr = addPredicateRecursive(pred,ret);

    if(!ret)
        predicate.push_back(expr);

    return ret;
}

bool Graphical_Element::addPropertyToCurrentPredicate(const std::string& property, const std::string& value)
{
    return sprites[sprites.size()-1].update(property, value);
}

bool Graphical_Element::updateSprites()
{
    for_each(sprites.begin(),sprites.end(),[](Sprite& s){s.createSprite();});
}

#include <iostream>
void Graphical_Element::constructViewFromFile(const std::string& path)
{
    std::ifstream ifs(path.c_str(),std::ios::in);
    if(!ifs)
    {
        Log_Init::getErrorLog()->error("Unable to open ",path);
        return;
    }

    boost::regex predicateRegex("(\\s*)\\|\\s*(when|When)\\s+(.*)");
    boost::regex attributeRegex("(\\s*)([^:]*):(.*)");
    boost::regex spacesRegex("(\\s*)(\\S*)(\\s*)(\\S*)");
    boost::smatch match;

    std::map<std::string,std::shared_ptr<Element> > model = Element::getModel();

    char temp[500];
    std::string tempStr;
    int counter = 1;
    bool ok = false;
    unsigned int numberSpacesBeforeForWhen = 0;
    unsigned int numberSpacesBeforeForProperty = 0;
    std::shared_ptr<Graphical_Element> current;
    while(ifs.getline(temp,500))
    {
        tempStr = std::string(temp);

        if(boost::regex_search(tempStr, match, predicateRegex))
        {
            if(!ok)
                Log_Init::getErrorLog()->warning("Predicate and following graphical properties not added : no viable parent, line ", counter);
            else
            {
                std::string spaces(match[1]);
                if(!numberSpacesBeforeForWhen&&spaces.size())
                    numberSpacesBeforeForWhen = spaces.size();
                else if(spaces.size()!=numberSpacesBeforeForWhen)
                {
                    Log_Init::getErrorLog()->warning("Predicate and following graphical properties not added, please check indentation of predicate, line ", counter);
                    ok = false;
                }

                if(ok)
                {
                    char err = current->addPredicate(match[3]);
                    if(err)
                        ok = false;
                    if(err==1)
                        Log_Init::getErrorLog()->warning("Predicate and following graphical properties not added, please check parentheses in predicate, line ", counter);
                    else if(err==2)
                        Log_Init::getErrorLog()->warning("Predicate and following graphical properties not added, please check mix of && and || in predicate, line ", counter);
                    else if(err==3)
                        Log_Init::getErrorLog()->warning("Predicate and following graphical properties not added, please check operators used in predicate, line ", counter);
                    else if(err==4)
                        Log_Init::getErrorLog()->warning("Predicate and following graphical properties not added, please check forbidden characters used in predicate, line ", counter);
                    else if(err==5)
                        Log_Init::getErrorLog()->warning("Predicate and following graphical properties not added, please check variable names used in predicate, line ", counter);
                    else if(err!=0)
                        Log_Init::getErrorLog()->warning("Predicate and following graphical properties not added, please check syntax of predicate, line ", counter);
                }
            }
        }
        else if(boost::regex_search(tempStr, match, attributeRegex))
        {
            if(!ok)
                Log_Init::getErrorLog()->warning("Graphical property not added : no viable parent, line ", counter);
            else
            {
                std::string spaces(match[1]);
                bool ok2 = true;
                if(!numberSpacesBeforeForProperty&&spaces.size()&&spaces.size()>numberSpacesBeforeForWhen)
                    numberSpacesBeforeForProperty = spaces.size();
                else if(spaces.size()!=numberSpacesBeforeForProperty)
                {
                    Log_Init::getErrorLog()->warning("Graphical property not added, please check indentation of property, line ", counter);
                    ok2 = false;
                }

                if(ok2)
                {
                    std::cout<<"Add property on current Element and current Predicate: "<<match[2]<<" => "<<match[3]<<std::endl;
                    if(!current->addPropertyToCurrentPredicate(match[2],match[3]))
                        Log_Init::getErrorLog()->warning("Graphical property not added, please check variable name or value, line ", counter);
                }
            }
        }
        else
        {
            boost::regex_search(tempStr, match, spacesRegex);
            std::string hasSpaceAtBeginning(match[1]);
            if(hasSpaceAtBeginning.size())
            {
                Log_Init::getErrorLog()->error("Unable to parse viewModel, please check indentation of element, line ", counter);
                return;
            }
            std::string isSeparated(match[4]);
            if(isSeparated.size())
            {
                Log_Init::getErrorLog()->error("Unable to parse viewModel, please check number of words on line ", counter);
                return;
            }
            std::string elementName = std::string(match[2]);
            ok = false;
            if(!model.count(elementName))
                Log_Init::getErrorLog()->warning("Graphical element not created : name doesn't match, line ", counter);
            else
            {
                if(current)
                    if(!current->updateSprites())
                        Log_Init::getErrorLog()->warning("Graphical element badly created : please check image path or dimensions until line ", counter);

                viewModel[elementName] = std::shared_ptr<Graphical_Element>(new Graphical_Element(model[elementName]));
                current = viewModel[elementName];
                std::cout<<elementName<<std::endl;
                numberSpacesBeforeForWhen = 0;
                numberSpacesBeforeForProperty = 0;
                ok = true;
            }
        }

        counter++;
    }
}

std::map<std::string,std::shared_ptr<Graphical_Element> > Graphical_Element::getViewModel()
{
    return viewModel;
}

std::vector<std::string> Graphical_Element::splitOutOfParentheses(const std::string& str, const std::string& separator)
{
    std::vector<std::string> ret;
    std::string temp = "";
    int nParentheses = 0;

    for(unsigned int i=0;i<str.size();i++)
        if(str[i]=='(')
        {
            temp.push_back(str[i]);
            nParentheses++;
        }
        else if(str[i]==')')
            if(nParentheses<=0)
                return std::vector<std::string>();
            else
            {
                temp.push_back(str[i]);
                nParentheses--;
            }
        else
            if(!nParentheses)
            {
                bool matchSeparator = true;
                for(unsigned int j=0;j<separator.size()&&matchSeparator;j++)
                    if(i+j>=str.size()||separator[j]!=str[i+j])
                        matchSeparator = false;

                if(matchSeparator)
                {
                    ret.push_back(temp);
                    temp = "";
                    i = i+separator.size()-1;
                }
                else
                    temp.push_back(str[i]);
            }
            else
                temp.push_back(str[i]);

    if(nParentheses)
        return std::vector<std::string>();

    if(temp.size())
        ret.push_back(temp);

    return ret;
}
