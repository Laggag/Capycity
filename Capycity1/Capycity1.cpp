#include <iostream>
#include <string>
using namespace std;

//Funktionen anlegen
void printArea();
void addBuilding();
void checkArea();
void deleteBuilding();

//Ich habe einfach irgendwelche Gebäude genommen, da ich mir nicht sicher war,
//ob Wasserkraftwerk etc. als Gebäude genommen werden sollten (daraus könnte man ja sonst keine Stadt bauen)
enum BuildingType {
    NONE = '0',
    HOUSE = 'h',//h = Buchstabe, welcher Gebäudetyp kennzeichnet bei Ausgabe
    SHOP = 's',
    OFFICE = 'o'
};
//Ich verwende ein Struct damit ich nicht immer alles den Funktionen übergeben muss
struct BuildingArea {
    BuildingType** buildingarea;
    int rows;
    int cols;
};

void printArea(BuildingArea& area)
{
    //Ausgabe
    for (unsigned int i = 0; i < area.rows; i++) 
    {
        for (unsigned int j = 0; j < area.cols; j++) 
        {
            std::cout << char(area.buildingarea[i][j]) << " ";
        }
        cout << " |" << i << endl;
    }
    for (int i = 0; i < area.cols; i++)
    {
        cout << "__";
    }
    cout << endl;
    for (int i = 0; i < area.cols; i++)
    {
        cout << i << " ";
    }
    cout << endl;
}

bool checkArea(BuildingArea& area, int posx, int posy, int height, int width, BuildingType btype)
{
    bool check = true;
    //Prueft ob zu bauendes Objekt im Baubereich ist
    if (!(posx + width <= area.cols && posy + height <= area.rows))
    {
        check = false;
        cout << "Gebaeude geht außerhalb des Baubereichs\n";
        return check;
    }

    //Prueft ob zu bauendes Objekt mit anderem Objekt ueberlappt
    for (int y = posy; y < posy + height; y++)
    {
        for (int x = posx; x < posx + width; x++)
        {
            if (area.buildingarea[y][x] != NONE)
            {
                check = false;
                cout << "Gebaeude ueberlappt sich mit einem anderen Gebaeude\n";
                return check;
            }
        }
    }

    return check;
}

void addBuilding(BuildingArea& area, int posx, int posy, int width, int height, string type)
{
    BuildingType btype;
    //Umwandeln von String in Enum
    if (type == "h" || type == "H")
    {
        btype = HOUSE;
    }
    else if (type == "s" || type == "S")
    {
        btype = SHOP;
    }
    else if (type == "o" || type == "O")
    {
        btype = OFFICE;
    }
    else
    {
        btype = NONE;
        cout << "Sie haben eine Falsche eingabe getaetigt.\n";
    }

    if (checkArea(area, posx, posy, height, width, btype))
    {
        //Baubereich beschreiben
        for (int y = posy; y < posy + height; y++)
        {
            for (int x = posx; x < posx + width; x++)
            {
                area.buildingarea[y][x] = btype;
            }
        }
    }
}

void deleteBuilding(BuildingArea& area, int posx, int posy, int width, int height)
{
    bool check = true;
    //Prueft ob zu bauendes Objekt im Baubereich ist
    if (!(posx + width <= area.cols && posy + height <= area.rows))
    {
        check = false;
        cout << "Gebaeude geht außerhalb des Baubereichs\n";
    }
    //Wenn ja, Bereich löschen(verkleinern)
    if (check)
    {
        for (int y = posy; y < posy + height; y++)
        {
            for (int x = posx; x < posx + width; x++)
            {
                area.buildingarea[y][x] = NONE;
            }
        }
    }
}


int main(int argc, char* argv[])
{
    //Überprüft, ob Konsoleneingabe genug argumente hat
    if (argc != 3) {
        std::cout << "Zu wenige Argumente(hoehe breite)" << std::endl;
        return 1;
    }

    //Variablen
    int posx, posy, width, height, input = 0;
    string type;
    BuildingType btype;
    int rows = atoi(argv[1]);//Umwandeln von char -> int
    int cols = atoi(argv[2]);

    //Struct
    BuildingArea area;
    area.rows = rows;
    area.cols = cols;
    area.buildingarea = new BuildingType * [rows];
    
    //Aus 1-D Array 2-D Array machen (dynamisch für Eingabe)
    for (unsigned int i = 0; i < rows; i++) 
    {
        area.buildingarea[i] = new BuildingType[cols];
    }
    //Initialisierung
    for (unsigned int i = 0; i < rows; i++)
    {
        for (unsigned int j = 0; j < cols; j++)
        {
            area.buildingarea[i][j] = NONE;
        }
    }

    while (input != 4)
    {
        //Fängt falsche Eingaben ab
        if (cin.fail()) {
            system("CLS");//Funktioniert nur bei Windows, ist auch nur für meine Übersichtlichkeit(gibt bei anderen OS keine Fehler aus)
            cout << "Falsche Eingabe." << endl;
            cin.clear();
            cin.ignore();
        }
        //Menue
        printf("Sie befinden sich im Baumenue:\n\n");
        printf("1. Gebaeude setzen\n2. Bereich loeschen\n3. Bauplan ausgeben\n4. Programm beenden\n");
        cin >> input;
        cout << endl;

        switch(input)
        {
            case 1:
                cout << "Gebaeude setzen: " << endl;
                //
                //Eingabe
                cout << "Bitte geben Sie den Gebaeudetyp ein.\n Moegliche Gebaeudetypen sind: HOUSE(h), SHOP(s), OFFICE(o): ";
                cin >> type;
                cout << "Position x: ";
                cin >> posx;
                cout << "Position y: ";
                cin >> posy;
                cout << "Breite: ";
                cin >> width;
                cout << "Tiefe: ";
                cin >> height;

                addBuilding(area, posx,posy,width,height,type);//(BuildingArea& area, int posx, int posy, int width, int height, string type)
                break;
            case 2:
                cout << "Bereich loeschen: " << endl;
                //
                //Eingabe
                cout << "Position x: ";
                cin >> posx;
                cout << "Position y: ";
                cin >> posy;
                cout << "Breite: ";
                cin >> width;
                cout << "Tiefe: ";
                cin >> height;

                deleteBuilding(area,posx,posy,width,height);//(BuildingArea& area, int posx, int posy, int width, int height)
                break;
            case 3:
                system("CLS");
                cout << "Ausgabe der Bauchflaeche: " << endl;
                printArea(area);
                break;
            case 4:
                cout << "Programm wird beendet." << endl;
                break;
            default:
                system("CLS");
                cout << "Falsche eingabe." << endl;
                break;
        }
    }

}
