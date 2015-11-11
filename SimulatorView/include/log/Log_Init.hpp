/**
Copyright (C)

creation date : 08/11/2015
file : log_init.hpp
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


#ifndef LOG_INIT_HPP
#define LOG_INIT_HPP


#include <fstream>
#include <cstdio>

#include "Info_Warning_Error_Logger.hpp"
#include "File_Handler.hpp"


class Log_Init
{
    public:
        static Log_Init& getInstance();

        static std::shared_ptr<Info_Warning_Error_Logger> getInfoLog();
        static std::shared_ptr<Info_Warning_Error_Logger> getWarningLog();
        static std::shared_ptr<Info_Warning_Error_Logger> getErrorLog();

    private:
        Log_Init(const std::string& folderPath);
        Log_Init(const Log_Init& cpy);
        ~Log_Init();
        Log_Init& operator = (const Log_Init& noCpy);

        static Log_Init instance;

        std::shared_ptr<Info_Warning_Error_Logger> infoLog;
        std::shared_ptr<Info_Warning_Error_Logger> warningLog;
        std::shared_ptr<Info_Warning_Error_Logger> errorLog;

        std::string infoPath;
        std::string warningPath;
        std::string errorPath;

        void removeIfEmpty(const std::string& path);
};


#endif
