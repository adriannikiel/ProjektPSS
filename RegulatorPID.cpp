/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel  *
 ***************************************************************************/

#include "RegulatorPID.h"
#include "wyjatki.h"
#include <sstream>
#include <fstream>
#include <vector>

RegulatorPID::RegulatorPID(void): IRegulator()
{
}

RegulatorPID::RegulatorPID(string sciezka, string obiekt) throw(OpenException, ReadDataException): IRegulator(obiekt)
{
	string doWczytania[] = {"k=", "Td=", "Ti=", "Tp=", "WARUNEK_POCZATKOWY=", "N=", "b=", "SKOK=", "AMPLITUDA_PROSTOKATA=",
		"OKRES_PROSTOKATA=", "WYPELNIENIE_PROSTOKATA=", "AMPLITUDA_SINUSA=", "OKRES_SINUSA=", "AMPLITUDA_TROJKATA=",
		"OKRES_TROJKATA=", "WARIANCJA_SZUMU=", "DLUGOSC_ZAKLOCENIA=", "SYGNAL=", "WYJSCIE_MIN=", "WYJSCIE_MAX="};
	const int DO_WCZYTANIA = 20; 
	
	ifstream plik; 
	plik.open(sciezka.c_str(), ios::in);

	// gdy próba otwarcia pliku nie powiod³a siê.
	if(!plik)
	{
		plik.clear(plik.rdstate() & ~ios::failbit);
		OpenException openEx("B³¹d otwarcia pliku konfiguracyjnego !!!");
		throw openEx;
	}
	
	char linia[100];		// linia wczytana z pliku konfiguracyjnego
	
	// zmienna wskazuj¹ca na poprawne wczytanie danych gdy jest równa 20
	int wczytano = 0;

	int test = 0;
			
	while(plik.getline(linia, 100))
	{
		string slinia(linia);
		
		string::size_type pozycja1 = slinia.find("MODEL=");
		string::size_type pozycja2 = slinia.find(obiekt);
			
		// gdy znaleziono model o podanej nazwie
		if(pozycja1 != string::npos && pozycja2 != string::npos)
		{
			bool koniec = false;
			
			do
			{
				string::size_type pozycja = slinia.find("#");
				
				// gdy koniec opisu modelu
				if(pozycja != string::npos)
					koniec = true;
					
				// gdy nie koniec opisu modelu wczytujemy dane
				else
				{
					for(int i=0; i<DO_WCZYTANIA; i++)
					{
						if( wczytaj(doWczytania[i],slinia) )
						{test = test|(1<<i);
							wczytano++;
							plik.getline(linia, 100);
							slinia = linia;
							continue;
						}
					}
					
					plik.getline(linia, 100);
					slinia = linia;
				}
			}while(!koniec);
		}
	}
	
	plik.close();
		
	// sprawdzenie czy wszystkie dane zosta³y poprawnie wczytane
	if(wczytano != DO_WCZYTANIA)
	{
		ReadDataException readEx("B³¹d wcztywania danych z pliku konfiguracyjnego !!!");
		throw readEx;
	}

	// generacja zak³ócenia
	zakl = Zaklocenie(dlugoscZaklucenia, wariancja);

	maxIter = okresP * okresS * okresT;

	// ustawienie pocz¹tkowych wartoœci wyjœæ porzednich
	y1 = 0;
	y2 = 0;
	r1 = 0;
	poprzWyj = warunekPocz;
}
RegulatorPID::RegulatorPID(double k, double stRoz, double stCal, double stTp,
						   double warPocz, double wspN, double stB): IRegulator()
{
	wzmocnienie = k;
	Td = stRoz;
	Ti = stCal;
	Tp = stTp;
	warunekPocz = warPocz;
	poprzWyj = warPocz;
	N = wspN;
	b = stB;

	y1 = 0;
	y2 = 0;
	r1 = 0;

	maxIter = okresP * okresS * okresT;
}

RegulatorPID::~RegulatorPID(void)
{
}

void RegulatorPID::setNastawyRegulatora(const vector<double> &nastawy) throw(ReadDataException)
{
	if(nastawy.size() == 7)
	{
		wzmocnienie = nastawy[0];
		Td = nastawy[1];
		Ti = nastawy[2];
		Tp = nastawy[3];
		warunekPocz = nastawy[4];
		N = nastawy[5];
		b = nastawy[6];
	}
	else
	{
		ReadDataException readEx("Podano nieprawid³owy wektor danych !!!");
		throw readEx;
	}
}

void RegulatorPID::setNastawyRegulatora(int parametr, double nastawa)
{
	if(parametr == K)
		wzmocnienie = nastawa;
	else if(parametr == TD)
		Td = nastawa;
	else if(parametr == TI)
		Ti = nastawa;
	else if(parametr == TP)
		Tp = nastawa;
	else if(parametr == WARUNEK_POCZATKOWY)
		warunekPocz = nastawa;
	else if(parametr == N_PARAM)
		N = nastawa;
	else if(parametr == B)
		b = nastawa;
	else
	{
		ReadDataException readEx("Wybrano nieprawid³owy parametr !!!");
		throw readEx;
	}
}

string RegulatorPID::getNastawy()
{
	stringstream ss;
	string str;
	string konf;

	konf.append("k= ");
	ss << wzmocnienie;
	ss >> str;
	konf.append(str);

	konf.append("\nTd= ");
	ss << Td;
	ss >> str;
	konf.append(str);

	konf.append("\nTi= ");
	ss << Ti;
	ss >> str;
	konf.append(str);

	konf.append("\nTp= ");
	ss << Tp;
	ss >> str;
	konf.append(str);

	konf.append("\nWARUNEK_POCZATKOWY= ");
	ss << warunekPocz;
	ss >> str;
	konf.append(str);

	konf.append("\nN= ");
	ss << N;
	ss >> str;
	konf.append(str);

	konf.append("\nb= ");
	ss << b;
	ss >> str;
	konf.append(str);

	return konf;
}
double RegulatorPID::liczWyjscie(double r, double y)
{
	double czescP=0, czescI=0, czescD=0, wyj=0;

	// obliczanie czêœci proporcjonalnej regulatora
	czescP = wzmocnienie*(b*r-y - b*r1+y1);

	// obliczanie czêœci ca³kuj¹cej regulatora
	czescI = Tp*wzmocnienie*(r-y)/Ti;

	// obliczanie czêœci ró¿niczkuj¹cej regulatora
	double a = Td/(Td+Tp*N);
	double bb = - wzmocnienie*N*Td/(Td+Tp*N);
	czescD = bb*(y-2*y1+y2)/(1-a);

	wyj = poprzWyj + (czescP + czescI + czescD);

	poprzWyj = wyj;
	y2 = y1;
	y1 = y;
	r1 = r;

	return wyj;
}

bool RegulatorPID::wczytaj(string dana, string slinia) throw(ReadDataException)
{
	string::size_type pozycja = slinia.find(dana);
	
	// gdy w lini znajduj¹ siê szukane dane
	if(pozycja == 0)
	{
		string sliczba;
		
		// pêtla wczytuj¹ca dane
		do
		{
			pozycja = slinia.find(" ",pozycja+1);
			sliczba = slinia.substr(pozycja+1, slinia.find(" ",pozycja+1) - pozycja-1);
			if(sliczba != "" && sliczba != dana)
			{
				if(dana == "k=")
					wzmocnienie = strtod (sliczba.c_str(),NULL);
				else if(dana == "Td=")
					Td = strtod (sliczba.c_str(),NULL);
				else if(dana == "Ti=")
					Ti = strtod (sliczba.c_str(),NULL);
				else if(dana == "Tp=")
					Tp = strtod (sliczba.c_str(),NULL);
				else if(dana == "WARUNEK_POCZATKOWY=")
					warunekPocz = strtod (sliczba.c_str(),NULL);
				else if(dana == "N=")
					N = strtod (sliczba.c_str(),NULL);
				else if(dana == "b=")
					b = strtod (sliczba.c_str(),NULL);
				else if(dana == "SKOK=")
					skok = strtod (sliczba.c_str(),NULL);
				else if(dana == "AMPLITUDA_PROSTOKATA=")
					amplitudaP = strtod (sliczba.c_str(),NULL);
				else if(dana == "OKRES_PROSTOKATA=")
					okresP = atoi (sliczba.c_str());
				else if(dana == "WYPELNIENIE_PROSTOKATA=")
					wypelnienieP = strtod (sliczba.c_str(),NULL);
				else if(dana == "AMPLITUDA_SINUSA=")
					amplitudaS = strtod (sliczba.c_str(),NULL);
				else if(dana == "OKRES_SINUSA=")
					okresS = atoi (sliczba.c_str());
				else if(dana == "AMPLITUDA_TROJKATA=")
					amplitudaT = atoi (sliczba.c_str());
				else if(dana == "OKRES_TROJKATA=")
					okresT = atoi (sliczba.c_str());
				else if(dana == "WARIANCJA_SZUMU=")
				{
					if(strtod (sliczba.c_str(),NULL) >= 0)
						wariancja = strtod (sliczba.c_str(),NULL);
					else
					{
						ReadDataException readEx("Wariancja powinna byæ wiêksza lub równa 0.");
						throw readEx;
						return false; 
					}
				}
				else if(dana == "DLUGOSC_ZAKLOCENIA=")
				{
					if(atoi (sliczba.c_str()) > 0)
						dlugoscZaklucenia = atoi (sliczba.c_str());
					else
					{
						ReadDataException readEx("D³ugoœæ zak³ócenia powinna byæ wiêksza od 0.");
						throw readEx;
						return false; 
					}
				}
				else if(dana == "SYGNAL=")
					sygnalZadany = atoi (sliczba.c_str());
				else if(dana == "WYJSCIE_MIN=")
					wyMin = strtod (sliczba.c_str(),NULL);
				else if(dana == "WYJSCIE_MAX=")
					wyMax = strtod (sliczba.c_str(),NULL);
			}
		}while(pozycja != string::npos);
		
		return true;
	}
	
	// gdy w lini nie ma szukanych danych
	return false; 
}

void RegulatorPID::resetRegulatora()
{
	r1 = 0;
	y1 = 0;
	y2 = 0;
	poprzWyj = warunekPocz;
}