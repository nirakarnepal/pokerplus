#include "mainwin.h"
#include "chat_client.h"
#include "dealer.h"
#include <X11/Xlib.h>

Mainwin *win = nullptr;

int main(int argc, char* argv[]){
    XInitThreads();
    auto app = Gtk::Application::create(argc,argv,"Nirakar.test");
    Gtk::Main kit(argc, argv);
    if (argc != 3)
    {
        std::cerr << "Usage: pokerplus <host> <port>\n";
        return 1;
    }
    
    asio::io_context io_context;
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });
    win = new Mainwin(&c);
    win->show();
    kit.run();
    app->run(*win);

    // you get here after the window is killed
    c.close();
    t.join();
}

