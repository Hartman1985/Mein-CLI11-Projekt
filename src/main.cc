#include <CLI/CLI.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

using json = nlohmann::json;
 
struct statemachine {
    json& mein_json;

    statemachine(json& database_object)
        :mein_json{database_object} {
        std::cout << "Konstruktor called with Initilizerlist!\n";
    }

    void aendern() 
    {
        //bevor Daten geändert werden sollen die Inhalte Ausgegeben werden
        ausgeben();

        int regalnummer; //Integer für die Options
        std::string neuerinhalt,alterinhalt,wahl;

        std::cout << "Welches Regal soll geändert werden?" << "\n";
        std::cin>> regalnummer;

        for (auto& change : mein_json["Regale"])
        {
            if(change ["Regal"] == regalnummer)
            {  
                std::cout << change["Inhalt"] << "\n"; 
                std::cout << "Welcher Inhalt soll geändert werden?" << "\n"; 
                std::cin>> alterinhalt;
                for (auto& aktuellinhalt : change["Inhalt"] )
                {
                    if (aktuellinhalt==alterinhalt)
                    {   
                        std::cout << aktuellinhalt << "\n";         
                        std::cout << "Neuer Wert" << "\n"; 
                        std::cin >> neuerinhalt;
                        aktuellinhalt=neuerinhalt; // Wenn direkt in Variable geschriebenwird, dann gibt es einen fehler beim parsen
                        //Hier erfolgt ein Early-Return wenn die Arbeit erfolgreich abgeschlossen wurde
                        return;
                    }
                }

                //Wird nur Ausgeführt, wenn der zu verändernde Inhalt nicht gefunden wird
                std::cout <<"Leider wurde der Inhalt " << alterinhalt <<" nicht gefunden!!! Soll er hinzugefügt werden?? [J/N]" << "\n";
                std::cin >> wahl;

                if (wahl == "J")
                {
                    change ["Inhalt"].push_back(alterinhalt);
                    break;
                }
                else
                {
                    std::cout << "Dann eben nicht!" << "\n";
                    break;
                }
            }
        }
    }

    void ausgeben() 
    {
        for (auto& element : mein_json["Regale"])
        {
            int belegte_plaetze, anzahl_plaetze, leere_plaetze;

            //Hier wird durch die Funktion ".size" der Inhalt gezählt und dieser als Integer gesetzt
            belegte_plaetze = element["Inhalt"].size();

            //Hier wird "Anzahl Lagerplätze" aufgerufen und auch als Integer gesetzt
            anzahl_plaetze = element["Anzahl Lagerplätze"];

            //Hier wird er Aktuelle Inhalt, der "Anzahl Lagerplätze" abgezogen. Die zuvor deklarierten Integer werden verrechnet
            leere_plaetze = anzahl_plaetze - belegte_plaetze;

            //Hier wird der Inhalt ausgegeben -Aufgabe 1
            std::cout << "Das Regal " << element["Regal"] << " beinhaltet folgende dinge: " << element["Inhalt"] << std::endl;

            //Hier werden die restlichen Lagerplätze ausgegebnen -Aufgabe 2
            std::cout << "Es sind " << leere_plaetze << " von " << anzahl_plaetze << " Plätze frei\n" << std::endl;
        }
    }

    void speichern() 
    {
        std::string speicherpfad{};
        std::cout << "Name der Datei: ";
        std::cin >> speicherpfad;

        //Optional. Gibt nur im Terminal den Speicherpfad aus, damit es "schön" aussieht!
        std::cout << "\nSpeicherpfad: /CLI11/" << speicherpfad << std::endl;    
        //Dieser muss je nach dem geändert werden, sollte euer Ordner "wo anders liegen"
        //bzw. sollte die Datei "anders heißen"
        //Beispiel: "\nSpeicherpfad: Server/maxmustermann/Projekte/Lagersysteme/" usw.
        std::ofstream save_as{speicherpfad};

        save_as << mein_json.dump(4);
        //Warum auch immer wird die Reihenfolge bei neuem Speichern nicht eingehalten
        //Gerne Lösungen dazu Teilen!
        save_as.close();
    }
};

int main(int argc, char** argv) 
{
    json database_object;
    std::string filepath;
    
    CLI::App app{"Schreibe: -r src/lagerhalterung.json \n"};
    app.add_option("-r,--read", filepath, "Path to config file")
    ->required()
    ->check(CLI::ExistingFile);
    
    std::cout << "Moin! Hier ist ein Lagerhalterungssystem" << std::endl;

    //try & catch Funktion, sollte ein Parse Error auftreten, so wird die "app" geschlossen
    try
    {
        app.parse(argc, argv);
    }
    catch(const CLI::ParseError &e)
    {
        return app.exit(e);
    }

    std::ifstream file{filepath};
    if(!file.is_open())
    {
        std::cout << "Error opening file!\n" << std::endl;
        exit(0);
    }

    //try & catch Funktion, sollte die file nicht dem "database_object" übergeben werden, so schreibe den Fehler und exit mit Fehler
    try
    {
        database_object = nlohmann::json::parse(file);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
        std::exit(EXIT_FAILURE);
    }

    statemachine lagerhaltung_statemachine{database_object};

    while(true)
    {
        int aktueller_state = 0;
        while((aktueller_state < 1) || (aktueller_state > 4 ))
        {
            std::cout <<    "Du kannst wählen zwischen: \n"
                      <<    "(1) für Inhalt Ändern \n"
                      <<    "(2) für Inhalt Ausgeben \n"
                      <<    "(3) für Inhalt Speichern \n" 
                      <<    "(4) Ende des Programms" << std::endl;
            std::cin >> aktueller_state;

            if((aktueller_state < 1) || (aktueller_state > 4 ))
            {
                std::cout << "Keine gültige Auswahl!" << std::endl;
            }
        }

        if(aktueller_state == 1)
        {
            lagerhaltung_statemachine.aendern();
        }
        else if(aktueller_state == 2)
        {
            lagerhaltung_statemachine.ausgeben();
        }
        else if(aktueller_state == 3)
        {
            lagerhaltung_statemachine.speichern();
        }
        else if(aktueller_state == 4)
        {
            break;
        }
    }
}