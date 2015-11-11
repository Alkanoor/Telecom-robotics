#include "log/Info_Warning_Error_Logger.hpp"


std::shared_ptr<Info_Warning_Error_Logger> Info_Warning_Error_Logger::static_log = std::shared_ptr<Info_Warning_Error_Logger>(new Info_Warning_Error_Logger());

Info_Warning_Error_Logger::Info_Warning_Error_Logger(bool date) :
    endLineAuto(true),
    printDateAuto(date),
    currentType(0)
{}

void Info_Warning_Error_Logger::setType(const std::string& type)
{
    if(type=="info")
        currentType = 0;
    else if(type=="warning")
        currentType = 1;
    else if(type=="error")
        currentType = 2;
    else
        currentType = 3;
}

void Info_Warning_Error_Logger::printDate()
{
    for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
        (**it)<<"<"<<date()<<"> ";
}

void Info_Warning_Error_Logger::endLine()
{
    for(std::vector<std::shared_ptr<Handler> >::iterator it = handlers.begin(); it != handlers.end(); it++)
        (*it)->writeEndline("");
}
