#include "log/File_Handler.hpp"


File_Handler::File_Handler(const std::string& path) :
    file(path.c_str(),std::ios::out|std::ios::trunc|std::ios::binary)
{}

File_Handler::~File_Handler()
{}

void File_Handler::write(const std::string& toWrite)
{
    if(file)
        file<<toWrite;
}

void File_Handler::writeEndline(const std::string& toWrite)
{
    if(file)
        file<<toWrite;
    if(file)
        file<<std::endl;
}

Handler& File_Handler::operator << (const std::string& toWrite)
{
    if(file)
        file<<toWrite;
    return *this;
}
