/**
Copyright (C)

creation date : 05/11/2015
file : io_with_log.hpp
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


#ifndef IO_WITH_LOG_HPP
#define IO_WITH_LOG_HPP


#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "log/Info_Warning_Error_Logger.hpp"
#include "log/Logger.hpp"


class IO_With_Log
{
    public:
        IO_With_Log(const std::string& host_name, const std::string& port = "5401", bool exitWhenConnexionFailed = true);
        IO_With_Log(const std::string& host_name, const std::string& port, std::shared_ptr<Info_Warning_Error_Logger> errors, bool exitWhenConnexionFailed = true);
        IO_With_Log(const std::string& host_name, const std::string& port, std::shared_ptr<Info_Warning_Error_Logger> infos, std::shared_ptr<Info_Warning_Error_Logger> errors, bool exitWhenConnexionFailed = true);

        const boost::array<char, 128>& read(size_t& bytes_read);

        bool resetConnexion();

    private:
        std::string host;
        std::string port;
        bool isConnexionGood;
        bool exitWhenConnexionFailed;

        std::shared_ptr<boost::asio::ip::tcp::socket> socket;
        boost::array<char, 128> buf;
        boost::system::error_code error;

        std::shared_ptr<Info_Warning_Error_Logger> info_log;
        std::shared_ptr<Info_Warning_Error_Logger> error_log;

        void init(const std::string& host_name, const std::string& port);
};


#endif
