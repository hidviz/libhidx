#include <libhidx/server/Server.hh>
#include <iostream>

#include "args.hh"

int main(int argc, char* argv[]){

    args::ArgumentParser parser{"Daemon"};
    args::Group modes{parser, "", args::Group::Validators::Xor};
    args::Group unixSocketGroup{modes, "", args::Group::Validators::All};
    args::Group unixSocketRequiredGroup{unixSocketGroup, "", args::Group::Validators::All};
    args::Flag unixSocket{unixSocketRequiredGroup, "u", "", {'u'}};
    args::Positional<std::string> socketDirectory{unixSocketRequiredGroup, "", ""};

    args::Group unixSocketOptionalGroup{unixSocketGroup, ""};
    args::Flag watchParent{unixSocketOptionalGroup, "p", "", {'p'}};


    try {
        parser.ParseCLI(argc, argv);
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (args::ValidationError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    libhidx::server::run(socketDirectory.Get(), watchParent);
    return 0;
}
