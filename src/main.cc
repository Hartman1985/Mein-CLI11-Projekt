#include <iostream>
#include <CLI/CLI.hpp>

int main(int argc, char** argv)
{
	std::cout << "Hello World" << "\n";

	CLI::App app{"Dies ist ein Programm was uns den Umgang mit Lagerhaltungsdaten zeigen soll","Lagerhaltungsprogramm"};
    int flag_int{0};
    app.add_option("-i",flag_int,"int");

    bool flag_bool{false};
    app.add_flag("-b", flag_bool, "bool");

    try{
        app.parse(argc, argv);
    } catch(const CLI::ParseError& e){
        return app.exit(e);
    }

    if(flag_int >0)
        std::cout <<"Flag : "<< flag_int <<std::endl;

    if(flag_bool)
        std::cout <<"Bool erkannt : "<<flag_bool <<std::endl;
}
