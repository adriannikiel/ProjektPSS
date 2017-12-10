/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#include "armax.h"
#include "wyjatki.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <numeric>

using namespace std;

armax::armax(string ob) throw(OpenException, ReadDataException) : Objekt(ob)
{
	string doWczytania[] = {"d=", "A1=", "B1=", "C1=", "A2=", "B2=", "C2=", "I1=", "I2=", "WYJŚCIE_MAX=", "WYJŚCIE_MIN=", "WARIANCJA=", "DŁUGOSC_ZAKŁÓCENIA="};
	const int DO_WCZYTANIA = 13; 
	
	ifstream plik; 
	plik.open("konfiguracja.conf", ios::in);

	// gdy próba otwarcia pliku nie powiodła się.
	if(!plik)
	{
		plik.clear(plik.rdstate() & ~ios::failbit);
		OpenException openEx("Błąd otwarcia pliku konfiguracyjnego: konfiguracja.conf");
		throw openEx;
	}
	
	char linia[100];		// linia wczytana z pliku konfiguracyjnego
	
	// zmienna wskazująca na poprawne wczytanie danych gdy jest równa 13
	int wczytano = 0;
			
	while(plik.getline(linia, 100))
	{
		string slinia(linia);
		
		string::size_type pozycja1 = slinia.find("MODEL=");
		string::size_type pozycja2 = slinia.find(ob);
			
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
						{
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
		
	// sprawdzenie czy wszystkie dane zostały poprawnie wczytane
	if(wczytano != DO_WCZYTANIA)
	{
		ReadDataException readEx("Błąd wcztywania danych z pliku konfiguracyjnego !!!");
		throw readEx;
	}
	
	// generacja zakłócenia
	//zakl = Zaklocenie(dlZakl, wariancja);
	zakl = Zaklocenie(dlZakl, wariancja,C1,A1);
	
	// ustawienie początkowych wyjść na 0
	for(int i=0; i<A1.size()-1; i++)
		yPoprzednie.push_back(0);
		
	// ustawienie początkowych wejść na 0
	for(int i=0; i<B1.size()+d; i++)
		uPoprzednie.push_back(0);
	
	// ustawienie indeksu zakłócenia na 0
	indexZakl = 0;
	
	licznikIteracji = 0;
}

double armax::symuluj(double wejscie)
{
	double wyjscie = 0;
	
	// aktualizacja wektora poprzednich wejść
	for(int i=uPoprzednie.size()-1; i>0; i--)
		uPoprzednie[i] = uPoprzednie[i-1];
	
	uPoprzednie[0] = wejscie;
	
	// Parametry 1 dla układu niestacjonarnego
	if(licznikIteracji <= I1)
		wyjscie += liczWyjscie(A1, B1, C1);
		
	// Parametry pośrednie dla układu niestacjonarnego	
	else if(licznikIteracji > I1 && licznikIteracji < I2)
	{
		vector<double> AT, BT, CT;
		
		// współczynnik kierunkowy prostej
		double k = 1.0/(I2-I1);
		
		for(int i=0; i<A1.size(); i++)
			AT.push_back( A1[i] + k*(licznikIteracji-I1)*(A2[i]-A1[i]) );
			
		for(int i=0; i<B1.size(); i++)
			BT.push_back( B1[i] + k*(licznikIteracji-I1)*(B2[i]-B1[i]) );
		
		for(int i=0; i<C1.size(); i++)
			CT.push_back( C1[i] + k*(licznikIteracji-I1)*(C2[i]-C1[i]) );

		wyjscie += liczWyjscie(AT, BT, CT);
	}
	
	// Parametry 2 dla układu niestacjonarnego
	else if(licznikIteracji >= I2)
		wyjscie += liczWyjscie(A2, B2, C2);
	
	// aktualizacja wektora poprzednich wyjść
	for(int i=yPoprzednie.size()-1; i>0; i--)
		yPoprzednie[i] = yPoprzednie[i-1];
	
	yPoprzednie[0] = wyjscie;
	
	// aktualizacja indexu zakłócenia
	indexZakl++;
	if(indexZakl>=dlZakl)
		indexZakl = 0;

	// inkrementacja licznika iteracji
	licznikIteracji++;
	
	return wyjscie;
}

void armax::symuluj(string we, string wy) throw(OpenException, ReadDataException)
{
	ifstream plikWe; 
	plikWe.open(we.c_str(), ios::in);
	
	// gdy próba otwarcia pliku nie powiodła się.
	if(!plikWe)
	{
		OpenException openEx("Błąd otwarcia pliku danych symulacji.");
		throw openEx;
		plikWe.clear(plikWe.rdstate() & ~ios::failbit);
		return;
	}
	
	ofstream plikWy; 
	plikWy.open(wy.c_str(), ios::out);
	
	// gdy utworzenia pliku nie powiodła się.
	if(!plikWy)
	{
		OpenException openEx("Błąd podczas tworzenia pliku wyników symulacji.");
		throw openEx;
		plikWy.clear(plikWy.rdstate() & ~ios::failbit);
		return;
	}
	
	reset();
	
	char linia[100];
	while(plikWe.getline(linia, 100))
		plikWy << symuluj( strtod(linia, NULL) ) << endl;
	
	plikWe.close();
	plikWy.close();
}

void armax::reset()
{
	// ustawienie licznika iteracji na 0
	licznikIteracji = 0;
	
	// ustawienie początkowych wyjść na 0
	for(int i=0; i<yPoprzednie.size(); i++)
		yPoprzednie[i] = 0;
		
	// ustawienie początkowych wejść na 0
	for(int i=0; i<uPoprzednie.size(); i++)
		uPoprzednie[i] = 0;
	
	// ustawienie indeksu zakłócenia na 0
	indexZakl = 0;
}

bool armax::zmienParametr(string dana, string wartosc)
{
	if(dana == "A1")
	{
		A1.clear();

		int stare = yPoprzednie.size();
		bool ret;

		ret = wczytaj(dana + "=", dana + "= " + wartosc);

		if(A1.size()-1 > stare)
			for(int i=stare; i<=A1.size()-1; i++)
				yPoprzednie.push_back(0);

		return ret;
	}

	else if(dana == "B1")
	{
		B1.clear();

		int stare = uPoprzednie.size();
		bool ret;

		ret = wczytaj(dana + "=", dana + "= " + wartosc);

		if(B1.size()+d > stare)
			for(int i=stare; i<=B1.size()+d; i++)
				yPoprzednie.push_back(0);

		return ret;
	}

	else if(dana == "C1")
		C1.clear();

	else if(dana == "A2")
		A2.clear();

	else if(dana == "B2")
		B2.clear();

	else if(dana == "C2")
		C2.clear();

	else if(dana == "d")
	{
		int stare = d;
		bool ret;

		ret = wczytaj(dana + "=", dana + "= " + wartosc);

		if(d > stare)
			for(int i=stare; i<=d; i++)
				uPoprzednie.push_back(0);

		return ret;
	}

	else if(dana == "wyMin")
		return wczytaj("WYJŚCIE_MIN=", "WYJŚCIE_MIN= " + wartosc);

	else if(dana == "wyMax")
		return wczytaj("WYJŚCIE_MAX=", "WYJŚCIE_MAX= " + wartosc);

	else if(dana == "WARIANCJA")
	{
		bool ret;

		indexZakl = 0;
		ret = wczytaj(dana + "=", dana + "= " + wartosc);
		zakl = Zaklocenie(dlZakl, wariancja,C1,A1);

		return ret;
	}

	else if(dana == "dlZakl")
	{
		bool ret;

		indexZakl = 0;
		ret = wczytaj("DŁUGOSC_ZAKŁÓCENIA=", "DŁUGOSC_ZAKŁÓCENIA= " + wartosc);
		zakl = Zaklocenie(dlZakl, wariancja,C1,A1);

		return ret;
	}

	return wczytaj(dana + "=", dana + "= " + wartosc);
}

double armax::liczWyjscie(vector<double>& A, vector<double>& B, vector<double>& C)
{
	double wyjscie = 0;

	// dodanie wpływu poprzednich wyjść
	wyjscie -= inner_product(A.begin()+1, A.end(), yPoprzednie.begin(), 0.0);
		
	// dodanie wpływu poprzednich wejść
	wyjscie += inner_product(B.begin(), B.end(), uPoprzednie.begin()+d, 0.0);
		
	// dodanie wpływu poprzednich zakłóceń
	for(int i=0; i<C.size(); i++)
	{
		// gdy poprzedni index jest ujemny przechodzimy na koniec uszeregowania zakłócenia
		int index = indexZakl-i;
		while(index<0)
			index += dlZakl-1;	
		wyjscie += C[i]*zakl.getWartosc(index);
	}
	
	if(wyjscie > wyMax)
		wyjscie = wyMax;
	else if(wyjscie < wyMin)
		wyjscie = wyMax;
		
	return wyjscie;
}

bool armax::wczytaj(string dana, string slinia) throw(OpenException, ReadDataException)
{
	string::size_type pozycja = slinia.find(dana);
	
	// gdy w lini znajdują się szukane dane
	if(pozycja == 0)
	{
		string sliczba;
		
		// pętla wczytująca dane
		do
		{
			pozycja = slinia.find(" ",pozycja+1);
			sliczba = slinia.substr(pozycja+1, slinia.find(" ",pozycja+1) - pozycja-1);
			if(sliczba != "" && sliczba != dana)
			{
				if(dana == "d=")
					d = atoi (sliczba.c_str());
				else if(dana == "A1=")
					A1.push_back( strtod (sliczba.c_str(),NULL) );
				else if(dana == "B1=")
					B1.push_back( strtod (sliczba.c_str(),NULL) );
				else if(dana == "C1=")
					C1.push_back( strtod (sliczba.c_str(),NULL) );
				else if(dana == "A2=")
					A2.push_back( strtod (sliczba.c_str(),NULL) );
				else if(dana == "B2=")
					B2.push_back( strtod (sliczba.c_str(),NULL) );
				else if(dana == "C2=")
					C2.push_back( strtod (sliczba.c_str(),NULL) );
				else if(dana == "I1=")
					I1 = atoi (sliczba.c_str());
				else if(dana == "I2=")
					I2 = atoi (sliczba.c_str());
				else if(dana == "WYJŚCIE_MAX=")
					wyMax = strtod (sliczba.c_str(),NULL);
				else if(dana == "WYJŚCIE_MIN=")
					wyMin = strtod (sliczba.c_str(),NULL);
				else if(dana == "WARIANCJA=")
				{
					if(strtod (sliczba.c_str(),NULL) >= 0)
						wariancja = strtod (sliczba.c_str(),NULL);
					else
					{
						ReadDataException readEx("Wariancja powinna być większa lub równa 0.");
						throw readEx;
						return false; 
					}
				}
				else if(dana == "DŁUGOSC_ZAKŁÓCENIA=")
				{
					if(atoi (sliczba.c_str()) > 0)
						dlZakl = atoi (sliczba.c_str());
					else
					{
						ReadDataException readEx("Długość zakłócenia powinna być większa od 0.");
						throw readEx;
						return false; 
					}
				}
			}
		}while(pozycja != string::npos);
		
		return true;
	}
	
	// gdy w lini nie ma szukanych danych
	return false; 
}

