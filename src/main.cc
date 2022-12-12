#include <iostream>
#include <CLI/CLI.hpp>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

int main(int argc, char** argv)
{
	std::cout << "Hello World" << "\n";

	CLI::App app{"Dies ist ein Programm was uns den Umgang mit Lagerhaltungsdaten zeigen soll","Lagerhaltungsprogramm"};
    int option_int{0};
    app.add_option("-i",option_int,"int");

    bool flag_bool{false};
    app.add_flag("-b", flag_bool, "bool");



std::string filepath;
    app.add_option("-r,--read", filepath,"Path to config file")
        ->required()
        ->check(CLI::ExistingFile);


    try{                                //wird immer ausgeführt
        app.parse(argc, argv);
    } catch(const CLI::ParseError& e){  //wird nur im Fehler bzw. ausnahme Fall ausgeführt
        return app.exit(e);
    }

    if(option_int >0)
        std::cout <<"Option Int : "<< option_int <<std::endl;

    if(flag_bool)
        std::cout <<"Bool erkannt : "<<flag_bool <<std::endl;

    
std::ifstream file{filepath};
    if(!file.is_open()){
        std::cout << "Error opening file!\n";
        exit(0);
    }

    nlohmann::json FileToJson;
    file >> FileToJson;

    std::cout << FileToJson.dump() << "\n";

    return 0;
}

