#include <iostream>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <boost/regex.hpp>

#include "log/Info_Warning_Error_Logger.hpp"
#include "log/Stdout_Handler.hpp"
#include "Graphical_Element.hpp"
#include "log/Log_Init.hpp"
#include "IO_With_Log.hpp"
#include "Element.hpp"

#include "Logical_Basic_Expression.hpp"


int main()
{
    //sf::Window window(sf::VideoMode(800, 600), "My window");
    //1: create the view
    //2: initialize elements => from network, from default, from config ...
    //3: hook events
    //4: update the view => synchroneously, asynchroneously

    Info_Warning_Error_Logger::static_log->addHandler(std::shared_ptr<Handler>(new File_Handler("logs/IO/io_log_date_"+Logger::date()+".log")));
    IO_With_Log io("127.0.0.1","5401",Info_Warning_Error_Logger::static_log,false);

    Element::constructModelFromFile("robots.model");
    Graphical_Element::constructViewFromFile("robots.view");

    Logical_Basic_Expression<int>* a = new Logical_Basic_Expression<int>("ok",INFERIOR,456);
    delete a;

    return 0;
}
