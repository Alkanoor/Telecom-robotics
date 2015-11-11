#include "Sprite.hpp"


Sprite Sprite::nullSprite;

Sprite::Sprite():
    colorToAdd(0,0,0,0)
{}

const Sprite& Sprite::emptySprite()
{
    return nullSprite;
}

#include <iostream>
bool Sprite::update(const std::string& parameter, const std::string& value)
{
    boost::smatch match;

    if(parameter=="im")
    {
        std::cout<<"Image ! !"<<std::endl;
        imagePath = value;
    }
    else if(parameter=="transparencyMask")
    {
        boost::regex reg("\\s*\\(\\s*([[:digit:]]+)\\s*,\\s*([[:digit:]]+)\\s*,\\s*([[:digit:]]+)\\s*\\)\\s*,\\s*([[:digit:]]+)\\s*");
        if(boost::regex_search(value, match, reg))
        {
            std::cout<<"TRANSPARENCY !!!! !"<<std::endl;
            std::cout<<match[0]<<" "<<match[1]<<" "<<match[2]<<" "<<match[3]<<" "<<match[4]<<std::endl;
        }
        else
            return false;
    }
    else if(parameter=="color")
    {
        boost::regex reg("\\s*\\(\\s*([[:digit:]]+)\\s*,\\s*([[:digit:]]+)\\s*,\\s*([[:digit:]]+)\\s*(,\\s*([[:digit:]]+)\\s*)?\\)\\s*");
        if(boost::regex_search(value, match, reg))
        {
            std::cout<<"Color !"<<std::endl;
            std::cout<<match[0]<<" "<<match[1]<<" "<<match[2]<<" "<<match[3]<<" "<<match[4]<<std::endl;
        }
        else
            return false;
    }
    else if(parameter=="width")
    {
        boost::regex reg("\\s*([[:digit:]]+)\\s*");
        if(boost::regex_search(value, match, reg))
        {
            std::cout<<"Width !"<<std::endl;
            std::cout<<match[0]<<" "<<match[1]<<std::endl;
        }
        else
            return false;
    }
    else if(parameter=="height")
    {
        boost::regex reg("\\s*([[:digit:]]+)\\s*");
        if(boost::regex_search(value, match, reg))
        {
            std::cout<<"Height !"<<std::endl;
            std::cout<<match[0]<<" "<<match[1]<<std::endl;
        }
        else
            return false;
    }
    else
        return false;
}

bool Sprite::createSprite()
{
    if(!img.loadFromFile(imagePath)&&(width<=0||height<=0))
        return false;

    if(img)
        img.createMaskFromColor(transparencyColor,alpha);
    else
        img.create(width,height,colorToAdd);

    text.update(img);
    sprite.setTexture(text);
    sprite.setColor(colorToAdd);

    return true;
}

const sf::Sprite& Sprite::getSprite() const
{return sprite;}
