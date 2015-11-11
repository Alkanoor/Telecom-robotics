#include "log/Log_Init.hpp"


Log_Init Log_Init::instance = Log_Init("logs/");

Log_Init& Log_Init::getInstance()
{return instance;}

std::shared_ptr<Info_Warning_Error_Logger> Log_Init::getInfoLog()
{return instance.infoLog;}

std::shared_ptr<Info_Warning_Error_Logger> Log_Init::getWarningLog()
{return instance.warningLog;}

std::shared_ptr<Info_Warning_Error_Logger> Log_Init::getErrorLog()
{return instance.errorLog;}

Log_Init::Log_Init(const std::string& folderPath) :
    infoLog(std::shared_ptr<Info_Warning_Error_Logger>(new Info_Warning_Error_Logger())),
    warningLog(std::shared_ptr<Info_Warning_Error_Logger>(new Info_Warning_Error_Logger())),
    errorLog(std::shared_ptr<Info_Warning_Error_Logger>(new Info_Warning_Error_Logger()))
{
    infoPath = folderPath+Logger::date()+".infoLog";
    warningPath = folderPath+Logger::date()+".warningLog";
    errorPath = folderPath+Logger::date()+".errorLog";
    infoLog->addHandler(std::shared_ptr<Handler>(new File_Handler(infoPath)));
    warningLog->addHandler(std::shared_ptr<Handler>(new File_Handler(warningPath)));
    errorLog->addHandler(std::shared_ptr<Handler>(new File_Handler(errorPath)));
}

Log_Init::Log_Init(const Log_Init& cpy)
{}

Log_Init::~Log_Init()
{
    removeIfEmpty(infoPath);
    removeIfEmpty(warningPath);
    removeIfEmpty(errorPath);
}

Log_Init& Log_Init::operator = (const Log_Init& noCpy)
{return instance;}

void Log_Init::removeIfEmpty(const std::string& path)
{
    std::ifstream ifs(path.c_str(), std::ios::in|std::ios::binary);
    auto cur = ifs.tellg();
    ifs.seekg(0, std::ios_base::end);
    if(ifs.tellg()==cur)
        std::remove(path.c_str());
}
