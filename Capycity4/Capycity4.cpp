#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
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


//-------------Gebäude------------------

class Building {
public:
	Building(string name, char label, int MW)
		: name_(name), label_(label), MW_(MW), price_(0) {
	}

	string getName() const { return name_; }
	//Berechnet jetzt direkt Preis
	double getPrice() {
		price_ = 0;
		for (const auto& m : mats_) {
			price_ += m.first.getPrice() * m.second;
		}
		return price_; 
	}
	char getLabel() const { return label_; }
	int getMW() { return MW_; }
	map<Material, int> getMats() const { return mats_; }
	void printMats() {

		for (const auto& m : mats_) {
			cout << " " << m.first.getName() << " " << m.second << "x" << endl;
		}
	}


private:
	string name_;
	double price_;
	char label_;
	int MW_;
protected:
	map<Material, int> mats_;
	Wood wood;
	Plastic plastic;
	Metal metal;
};

class House : public Building {
public:
	House() : Building("Haus ", 'h', 3) {
		mats_[wood] = 3;
		mats_[plastic] = 2;
		mats_[metal] = 5;
	}
};

class Shop : public Building {
public:
	Shop() : Building("Laden", 's', 2) {
		mats_[wood] = 4;
		mats_[plastic] = 5;
		mats_[metal] = 1;
	}
};


//-------------VERWALTUNG------------------
//Blueprint ist jetzt die ursprüngliche Capycitysim Klasse nur mit ID(Kennzahl)
//ich hoffe das war so gewollt
class Blueprint
{
public:
	//Standardkonstruktor
	Blueprint() : ID_(0), rows_(0), cols_(0), buildingarea(nullptr) {}

	//Konstruktor
	Blueprint(int rows, int cols) : ID_(0)
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
		double fullPrice = 0;
		printf("Ausgabe der Bauchflaeche mit Kennzahl %.2f:\n\n", getID());
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
		if (!buildings_.empty())
		{
			printf("\n\nGebaeude  |  Label  |  Preis  \n");
			printf("----------------------------");
			for (Building b : buildings_)
			{
				cout << endl << b.getName() << "     |   " << b.getLabel() << "     |    "
					<< b.getPrice();
			}
			printf("\n----------------------------\n");
			printf("Benoetigte Materialien:\n");
			for (Building b : buildings_)
			{
				b.printMats();
				fullPrice += b.getPrice();
			}
			printf("Gesamtpreis aller Gebaeude:\n %f", fullPrice);
		}
		printf("\n----------------------------\n");
	}

	//void addMat(Building b) { buildings.push_back(b); }
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
			buildings_.push_back(b);
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

	double getID() { 
		ID_ = 0;
		for (Building b : buildings_)
		{
			//Ich berechne hier MW * 1000000, weil ich denke es soll am Ende in Watt berechnet sein(sieht zumindest besser aus)
			//-> Außerdem wird mit größerem Baubereich ansonsten die Zahl sehr klein
			ID_ += (b.getMW() * 1000000) / (b.getPrice() * rows_ * cols_);
		}
		return ID_; 
	}
	//Funktor
	bool operator()(Blueprint& other) {
		return getID() == other.getID();
	}

private:
	char** buildingarea;
	vector<Building> buildings_;
	double ID_;
	int rows_;
	int cols_;
};

class CapycitySim {
public:

	CapycitySim(int rows, int cols)
	{
		createPlan(rows, cols);
		//Clear da er sonst mit Standardkonstruktor schon einen leeren Plan beinhaltet
		existingPlans.clear();
	}
	//Erstellen eines Plans
	void createPlan(int rows, int cols) {
		Blueprint newPlan(rows,cols);
		//hier wollte ich mal iteratoren ausprobiern, damit hatte ich vorher noch schwierigkeiten
		for (auto it = existingPlans.begin(); it != existingPlans.end();)
		{
			//Nutzt Funktor, prüft ob ID eines Plans identisch zu aktuellem ist
			if (currentPlan(*it))
			{
				it = existingPlans.erase(it);
			}
			else
			{
				++it;
			}
		}
		existingPlans.push_back(currentPlan);
		currentPlan = newPlan;
	}
	//hab ich am ende nicht gebrauch
	/*bool isDuplicate(Blueprint& newPlan) {
		for (auto& plan : existingPlans) {
			if (newPlan(plan))
				return true;
		}
		return false;
	}*/

	//Druckt aktuellen Plan + alle die gespeichert sind
	void printExistingPlans() {
		printf("Der aktuelle Plan:\n");
		currentPlan.printArea();
		if (!existingPlans.empty())
		{
			printf("\nGespeicherte Plaene:");
			//Hier habe ich sort verwendet, ich hoffe das war so gemeint
			sort(existingPlans.begin(), existingPlans.end(), [](Blueprint& a, Blueprint& b) {
				return a.getID() < b.getID();
				});
			for (auto& plan : existingPlans) {
				plan.printArea();
			}
		}
	}
	Blueprint & getCurrentPlan() { return currentPlan; }


private:
	Blueprint currentPlan;
	/*
	Ich habe ursprünglich mit Set gearbeitet, da ich Set für eine sinnvollere
	Datenstruktur für diesen Code halte. Ein Set ist aber schon sortiert
	(da hatte ich meinen Lamda Ausdruck auch genutzt), aber ich habe später
	erst gelesen, dass man direkt vor der Ausgabe sortieren soll, deshalb 
	bin ich auch vector umgestiegen, da ich mir unsicher war
	*/
	vector<Blueprint> existingPlans;
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

	//Buildings
	House house;

	Shop shop;


	//City
	CapycitySim city(rows,cols);

	while (input != 5)
	{
		//Fängt Fehler bei Eingabe auf
		if (cin.fail()) {
			system("CLS");
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
		Blueprint& currentPlan = city.getCurrentPlan();
		//Menue
		printf("\nSie befinden sich im Baumenue:\n\n");
		printf("1. Neuen Plan erstellen\n2. Gebaeude setzen\n3. Bereich loeschen\n4. Bauplan ausgeben\n5. Programm beenden\n\n");
		cin >> input;
		cout << endl;

		switch (input)
		{
		case 1:
			system("CLS");
			//Eingabe Länge, Breite des Baubereichs
			cout << "Geben Sie die Hoehe des Baubereichs an: " << endl;
			cin >> rows;
			cout << "Geben Sie die Breite des Baubereichs an: " << endl;
			cin >> cols;

			city.createPlan(rows, cols);
			cout << "Ein Neuer Plan wurde erstellt." << endl;
			break;
		case 2:
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
				currentPlan.addBuilding(house, posx, posy, width, height);
			}
			else if (label == 's')
			{
				currentPlan.addBuilding(shop, posx, posy, width, height);
			}
			else
			{
				cout << "Falsche Eingabe.";
			}
			break;
		case 3:
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

			currentPlan.removeArea(posx, posy, width, height);
			break;
		case 4:
			system("CLS");
			city.printExistingPlans();
			cout << endl;
			break;
		case 5:
			cout << "Programm wird beendet." << endl;
			break;
		default:
			system("CLS");
			cout << "Falsche eingabe." << endl;
			break;
		}

	}

}

