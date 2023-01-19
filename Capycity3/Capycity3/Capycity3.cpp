#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

//-------------Materialien------------------

class Material {
public:
	Material(string name, double price) : name_(name), price_(price) {}

	string getName() const { return name_; }
	double getPrice() const { return price_; }

	//operator überladen, um map verwenden zu können(Map sortiert automatisch)
	bool operator<(const Material& other) const {
		return name_ < other.name_;
	}

private:
	string name_;
	double price_;
};
//Materialentypen als eigene Klassen, die sich von Material ableiten -> hoffe das war so gewollt
class Wood : public Material {
public:
	Wood() : Material("Holz", 10.0) {}
};

class Metal : public Material {
public:
	Metal() : Material("Metall", 20.0) {}
};

class Plastic : public Material {
public:
	Plastic() : Material("Plastik", 5.0) {}
};

std::ostream& operator<<(std::ostream& os, const Material& material) {
	os << material.getName() << " kostet " << material.getPrice() << " pro Stueck.";
	return os;
}

//-------------Gebäude------------------

class Building {
public:
	Building(string name, char label)
		: name_(name), label_(label), price_(0) {}

	string getName() const { return name_; }
	double getPrice() const { return price_; }
	char getLabel() const { return label_; }
	map<Material,int> getMats() const { return mats_; }
	void printMats() {
		for(const auto& m : mats_) {
			cout << " " << m.first.getName() << " " << m.second << "x" << endl;

		}
	}
	//Berechnet aktuellen Preis
	void calcPrice()
	{
		price_ = 0;
		for (const auto& m : mats_) {
			price_ += m.first.getPrice() * m.second;
		}
	}
	

private:
	string name_;
	double price_;
	char label_;

//*Protected, um es in den Unterklassen verwenden zu können
protected:
	/*
	Ich habe mich für eine Map anstatt einer Klasse entschieden, um die Übersichtlichkeit des Programms zu bewahren.
	Im Grunde halte ich eine extra Klasse für die Verwaltung sinnvoller, wenn es sich um ein größeres Projekte handelt,
	da es in größeren Programmen eine klarere Struktur schafft. In diesem "kleineren" Projekt jedoch, reicht eine Map
	vollkommen aus, um das gefragte Ziel zu erhalten, ohne viel Code "unnötig" hinzuzufügen.
	*/
	map<Material, int> mats_;
	Wood wood;
	Plastic plastic;
	Metal metal;
};

class House : public Building {
public:
	House() : Building("Haus ", 'h') {
		mats_[wood] = 3;//*Siehe hier
		mats_[plastic] = 2;
		mats_[metal] = 5;
	}
};

class Shop : public Building {
public:
	Shop() : Building("Laden", 's') {
		mats_[wood] = 4;
		mats_[plastic] = 5;
		mats_[metal] = 1;
	}
};

std::ostream& operator<<(std::ostream& os, const Building& building) {
	os << "Das Gebaeude " << building.getName() << " kostet " << building.getPrice()
		<< " und benutzt das Label " << building.getLabel() << "."; //benötigte Materialien
	return os;
}

//-------------VERWALTUNG------------------
class CapycitySim
{
public:
	//Konstruktor
	CapycitySim(int rows, int cols)
	{
		rows_ = rows;
		cols_ = cols;
		//dynamisch 2D Array erstellen
		buildingarea = new char* [rows_];
		for (int i = 0; i < rows_; i++) {
			buildingarea[i] = new char[cols_];
		}

		for (int x = 0; x < rows_; x++)
		{
			for (int y = 0; y < cols_; y++)
			{
				buildingarea[x][y] = 'o';//o ist leeres Feld
			}
		}
	}
	//Ausgabe der Baufläche
	void printArea()
	{
		int fullPrice = 0;
		cout << "Ausgabe der Bauchflaeche: " << endl << endl;
		//Ausgabe
		for (int x = 0; x < rows_; x++)
		{
			for (int y = 0; y < cols_; y++)
			{
				cout << char(buildingarea[x][y]) << " ";
			}
			cout << " |" << x << endl;
		}
		for (int i = 0; i < cols_; i++)
		{
			cout << "__";
		}
		cout << endl;
		for (int i = 0; i < cols_; i++)
		{
			cout << i << " ";
		}
		//Materialen etc. werden erst ausgegeben, wenn es Gebäude gibt
		if (!buildings.empty())
		{
			printf("\n\nGebaeude  |  Label  |  Preis  \n");
			printf("----------------------------");
			for (Building b : buildings)
			{
				cout << endl << b.getName() << "     |   " << b.getLabel() << "     |    "
					<< b.getPrice();
			}
			printf("\n----------------------------\n");
			printf("Benoetigte Materialien:\n");
			for (Building b : buildings)
			{
				b.printMats();
				fullPrice += b.getPrice();
			}
			printf("Gesamtpreis aller Gebaeude:\n %d", fullPrice);
		}
	}

	void addBuilding(Building b, int x, int y, int width, int height)
	{
		bool check = true;
		int overlapCounter = 0;
		//Prueft ob zu bauendes Gebäude im Baubereich ist
		if (!(x + width <= cols_ && y + height <= rows_)) {
			check = false;
			cout << "Gebaeude geht außerhalb des Baubereichs\n";
		}

		if (check)//Wenn oben schon fehlgeschlagen ist muss hier nicht noch einmal geprüft werden
		{
			//Prueft ob zu bauendes Gebäude mit anderem Gebäude überlappt
			for (int posy = y; posy < y + height; posy++)
			{
				for (int posx = x; posx < x + width; posx++)
				{
					if (buildingarea[posy][posx] != 'o')//o ist leeres Feld
					{
						check = false;
						overlapCounter++;
					}
				}
			}
			if (!check)
			{
				printf("Gebaeude ueberlappt sich an %d Stellen mit anderen Gebaeuden\n", overlapCounter);
			}
		}

		if (check)//Prüft ob eines der oberen Überprüfungen fehlschlagen
		{
			buildings.push_back(b);
			//Baubereich mit neuem Gebäude beschreiben
			for (int posy = y; posy < y + height; posy++)
			{
				for (int posx = x; posx < x + width; posx++)
				{
					buildingarea[posy][posx] = b.getLabel();
				}
			}
		}
	}

	void removeArea(int x, int y, int width, int height)
	{
		bool check = true;
		//Prueft ob zu bauendes Objekt im Baubereich ist
		if (!(x + width <= cols_ && y + height <= rows_)) {
			check = false;
			cout << "Gebaeude geht außerhalb des Baubereichs\n";
		}
		//Wenn ja, Bereich löschen(verkleinern)
		if (check)
		{
			for (int posy = y; posy < y + height; posy++)
			{
				for (int posx = x; posx < x + width; posx++)
				{
					buildingarea[posy][posx] = 'o';
				}
			}
		}
	}

private:
	//buildingarea für Ausgabe, buildings für eigentlich gespeicherte Gebäude
	char** buildingarea;
	vector<Building> buildings;
	int rows_;
	int cols_;
};

int main()
{
	int rows, cols;
	//Eingabe Länge, Breite des Baubereichs
	cout << "Geben Sie die Hoehe des Baubereichs an: " << endl;
	cin >> rows;
	cout << "Geben Sie die Breite des Baubereichs an: " << endl;
	cin >> cols;

	//Variablen
	char label;
	int posx;
	int posy;
	int width;
	int height;
	int input = 0;

	//Mats
	Wood wood;
	Metal metal;
	Plastic plastic;

	//Gebäude
	House house;
	house.calcPrice();
	Shop shop;
	shop.calcPrice();


	//Stadt
	CapycitySim city(rows, cols);

	while (input != 4)
	{
		//Fängt Fehler bei Eingabe auf
		if (cin.fail()) {
			system("CLS");//Funktioniert nur bei Windows, ist auch nur für meine Übersichtlichkeit(gibt bei anderen OS keine Fehler aus)
			cout << "Falsche Eingabe.";
			cin.clear();
			cin.ignore();
		}
		//Variablen zurücksetzen
		label = ' ';
		posx = 0;
		posy = 0;
		width = 0;
		height = 0;
		input = ' ';

		//Menue
		printf("\nSie befinden sich im Baumenue:\n\n");
		printf("1. Gebaeude setzen\n2. Bereich loeschen\n3. Bauplan ausgeben\n4. Programm beenden\n\n");
		cin >> input;
		cout << endl;

		switch (input)
		{
		case 1:
			cout << "Gebaeude setzen: " << endl;

			//Eingabe
			cout << "Bitte geben Sie den Gebaeudetyp ein.\n Moegliche Gebaeudetypen sind: HOUSE(h), SHOP(s): ";
			cin >> label;
			cout << "Position x: ";
			cin >> posx;
			cout << "Position y: ";
			cin >> posy;
			cout << "Breite: ";
			cin >> width;
			cout << "Hoehe: ";
			cin >> height;

			//Prüft welches Gebäude bebaut werden soll
			if (label == 'h')
			{
				city.addBuilding(house, posx, posy, width, height);
			}
			else if (label == 's')
			{
				city.addBuilding(shop, posx, posy, width, height);
			}
			else
			{
				cout << "Falsche Eingabe.";
			}
			break;
		case 2:
			cout << "Bereich verkleinern: " << endl;

			//Eingabe
			cout << "Position x: ";
			cin >> posx;
			cout << "Position y: ";
			cin >> posy;
			cout << "Breite: ";
			cin >> width;
			cout << "Hoehe: ";
			cin >> height;

			city.removeArea(posx, posy, width, height);
			break;
		case 3:
			system("CLS");
			city.printArea();
			cout << endl;
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

