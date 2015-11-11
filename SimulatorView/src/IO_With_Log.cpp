#include "IO_With_Log.hpp"


IO_With_Log::IO_With_Log(const std::string& host_name, const std::string& port, bool exitWhenConnexionFailed)
{
    this->exitWhenConnexionFailed = exitWhenConnexionFailed;
    init(host_name, port);
}

IO_With_Log::IO_With_Log(const std::string& host_name, const std::string& port, std::shared_ptr<Info_Warning_Error_Logger> errors, bool exitWhenConnexionFailed) :
    error_log(errors)
{
    this->exitWhenConnexionFailed = exitWhenConnexionFailed;
    init(host_name, port);
}

IO_With_Log::IO_With_Log(const std::string& host_name, const std::string& port, std::shared_ptr<Info_Warning_Error_Logger> infos, std::shared_ptr<Info_Warning_Error_Logger> errors, bool exitWhenConnexionFailed) :
    info_log(infos),
    error_log(errors)
{
    this->exitWhenConnexionFailed = exitWhenConnexionFailed;
    init(host_name, port);
}

const boost::array<char, 128>& IO_With_Log::read(size_t& bytes_read)
{
    try
    {
        bytes_read = socket->read_some(boost::asio::buffer(buf), error);
        if(error == boost::asio::error::eof)
        {
            if(error_log)
                error_log->warning("Connexion closed by the host");
            return buf;
        }
        else if(error)
            throw boost::system::system_error(error);
    }
    catch(std::exception& e)
    {
        if(error_log)
            error_log->error(e.what());
        isConnexionGood = false;
        if(socket)
            socket->close();
        if(exitWhenConnexionFailed)
            exit(-1);
    }
    return buf;
}

bool IO_With_Log::resetConnexion()
{
    init(host, port);
    return isConnexionGood;
}

void IO_With_Log::init(const std::string& host_name, const std::string& port)
{
    this->host = host_name;
    this->port = port;
    this->isConnexionGood = true;
    try
    {
        boost::asio::io_service io_service;

        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(host, port);
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        socket = std::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(io_service));
        boost::asio::connect(*socket, endpoint_iterator);
    }
    catch(std::exception& e)
    {
        if(error_log)
            error_log->error(e.what());
        isConnexionGood = false;
        if(socket)
            socket->close();
        if(exitWhenConnexionFailed)
            exit(-1);
    }
}
