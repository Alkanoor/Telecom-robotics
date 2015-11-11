#include "Element.hpp"


std::map<std::string,std::shared_ptr<Element> > Element::model;

Element::Element(const std::string& name, const std::shared_ptr<Element>& father)
{
    this->name = name;
    this->father = father;
}

void Element::addAttribute(const std::string& name)
{
    attributes.push_back(name);
    allVariables[name] = true;
}

void Element::addStaticVariable(const std::string& name)
{
    staticVariables.push_back(name);
    allVariables[name] = true;
}

const std::string& Element::getName() const
{return name;}

bool Element::varExists(const std::string& nameVar)
{return allVariables.count(nameVar)>0;}

void Element::constructModelFromFile(const std::string& file)
{
    std::ifstream ifs(file.c_str(),std::ios::in);
    if(!ifs)
    {
        Log_Init::getErrorLog()->error("Unable to open ", file);
        return;
    }

    char temp[500];
    std::string tempStr;
    int counter = 1;
    bool rootBefore = true;
    std::vector<unsigned int> spacing;
    std::shared_ptr<Element> curElement;
    std::vector<std::shared_ptr<Element> > fathers;
    while(ifs.getline(temp,500))
    {
        unsigned int numberSpaces = 0;
        unsigned int numberTabs = 0;
        unsigned int i=0;
        tempStr = std::string(temp);
        for(;i<tempStr.size()&&(tempStr[i]==' '||tempStr[i]=='\t');i++)
        {
            if(tempStr[i]==' ')
                numberSpaces++;
            else if(tempStr[i]=='\t')
                numberTabs++;
        }

        std::string newName;
        if(i<tempStr.size())
            if((tempStr[i]>='a'&&tempStr[i]<='z')||(tempStr[i]>='A'&&tempStr[i]<='Z')||(tempStr[i]>='0'&&tempStr[i]<='9')||tempStr[i]=='_'||tempStr[i]=='+'||tempStr[i]=='/')
                newName.push_back(tempStr[i]);
        for(unsigned int k=i+1;k<tempStr.size();k++)
            if((tempStr[k]>='a'&&tempStr[k]<='z')||(tempStr[k]>='A'&&tempStr[k]<='Z')||(tempStr[k]>='0'&&tempStr[k]<='9')||tempStr[k]=='_')
                newName.push_back(tempStr[k]);

        if(newName.size())
        {
            if(newName[0]=='+')
            {
                if(!spacing.size())
                {
                    Log_Init::getErrorLog()->error("Unable to parse model, attribute without parent, line ", counter);
                    return;
                }
                if((rootBefore&&numberSpaces+numberTabs>spacing[spacing.size()-1])||(!rootBefore&&numberSpaces+numberTabs==spacing[spacing.size()-1]))
                {
                    if(rootBefore&&numberSpaces+numberTabs>spacing[spacing.size()-1])
                        spacing.push_back(numberSpaces+numberTabs);
                    curElement->addAttribute(newName.substr(1,newName.size()-1));
                    rootBefore = false;
                }
                else
                {
                    Log_Init::getErrorLog()->error("Unable to parse model, please check indentation of attribute, line ", counter);
                    return;
                }
            }
            else if(temp[i]=='/')
            {
                if(!spacing.size())
                {
                    Log_Init::getErrorLog()->error("Unable to parse model, static variable without parent, line ", counter);
                    return;
                }
                if((rootBefore&&numberSpaces+numberTabs>spacing[spacing.size()-1])||(!rootBefore&&numberSpaces+numberTabs==spacing[spacing.size()-1]))
                {
                    if(rootBefore&&numberSpaces+numberTabs>spacing[spacing.size()-1])
                        spacing.push_back(numberSpaces+numberTabs);
                    curElement->addStaticVariable(newName.substr(1,newName.size()-1));
                    rootBefore = false;
                }
                else
                {
                    Log_Init::getErrorLog()->error("Unable to parse model, please check indentation of static variable, line ", counter);
                    return;
                }
            }
            else
            {
                if(!spacing.size())
                {
                    model[newName] = std::shared_ptr<Element>(new Element(newName));
                    spacing.push_back(numberSpaces+numberTabs);
                }
                else if(numberSpaces+numberTabs==spacing[spacing.size()-1])
                {
                    if(rootBefore&&fathers.size()>1)
                        model[newName] = std::shared_ptr<Element>(new Element(newName,fathers[fathers.size()-2]));
                    else if(rootBefore)
                        model[newName] = std::shared_ptr<Element>(new Element(newName));
                    else
                        model[newName] = std::shared_ptr<Element>(new Element(newName,fathers[fathers.size()-1]));
                }
                else if(numberSpaces+numberTabs<spacing[spacing.size()-1])
                {
                    int found = -1;

                    for(unsigned int j=spacing.size();j>0;j--)
                        if(numberSpaces+numberTabs==spacing[j-1])
                            found = j-1;

                    if(found>0)
                        model[newName] = std::shared_ptr<Element>(new Element(newName,fathers[found-1]));
                    else if(found==0)
                        model[newName] = std::shared_ptr<Element>(new Element(newName));
                    else
                    {
                        Log_Init::getErrorLog()->error("Unable to parse model, please check indentation of element, line ", counter);
                        return;
                    }

                    spacing.resize(found+1);
                    fathers.resize(found+1);
                }
                else
                {
                    model[newName] = std::shared_ptr<Element>(new Element(newName,curElement));
                    spacing.push_back(numberSpaces+numberTabs);
                }
                fathers.push_back(model[newName]);
                curElement = model[newName];
                rootBefore = true;
            }
        }
        counter++;
    }

    /*for(std::map<std::string,std::shared_ptr<Element> >::iterator it = model.begin();it!=model.end();it++)
    {
        Log_Init::getInfoLog()->write(it->first," :");
        it->second->print(*Log_Init::getInfoLog());
        Log_Init::getInfoLog()->endLine();
    }*/
}

const std::map<std::string,std::shared_ptr<Element> >& Element::getModel()
{return model;}
