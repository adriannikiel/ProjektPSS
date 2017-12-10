/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/

#include "RegulatorP.h"
#include "wyjatki.h"
#include <sstream>
#include <fstream>
#include <vector>

RegulatorP::RegulatorP(void): IRegulator()
{
}

RegulatorP::RegulatorP(string sciezka, string obiekt) throw(OpenException, ReadDataException): IRegulator(obiekt)
{
	string doWczytania[] = {"k=", "SKOK=", "AMPLITUDA_PROSTOKATA=", "OKRES_PROSTOKATA=",
		"WYPELNIENIE_PROSTOKATA=", "AMPLITUDA_SINUSA=", "OKRES_SINUSA=", "AMPLITUDA_TROJKATA=",
		"OKRES_TROJKATA=", "WARIANCJA_SZUMU=", "DLUGOSC_ZAKLOCENIA=", "SYGNAL=", "WYJSCIE_MIN=", "WYJSCIE_MAX="};
	const int DO_WCZYTANIA = 14; 
	
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
	
	// zmienna wskazuj¹ca na poprawne wczytanie danych gdy jest równa 14
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
}
RegulatorP::RegulatorP(double k): IRegulator()
{
	wzmocnienie = k;
}

RegulatorP::~RegulatorP(void)
{
}

void RegulatorP::setNastawyRegulatora(const vector<double> &nastawy) throw(ReadDataException)
{
	if(nastawy.size() == 1)
		wzmocnienie = nastawy[0];
	else
	{
		ReadDataException readEx("Podano nieprawid³owy wektor danych !!!");
		throw readEx;
	}
}

void RegulatorP::setNastawyRegulatora(int parametr, double nastawa)
{
	if(parametr == K)
		wzmocnienie = nastawa;
	else
	{
		ReadDataException readEx("Wybrano nieprawid³owy parametr !!!");
		throw readEx;
	}
}

string RegulatorP::getNastawy()
{
	stringstream ss;
	string str;
	string konf;

	konf.append("k= ");
	ss << wzmocnienie;
	ss >> str;
	konf.append(str);

	return konf;
}
double RegulatorP::liczWyjscie(double warZadana, double wyjZObiektu)
{
	double wejscieReg = warZadana - wyjZObiektu;
	return wzmocnienie*wejscieReg;
}

bool RegulatorP::wczytaj(string dana, string slinia) throw(ReadDataException)
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