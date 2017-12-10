/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#include "Autentykacja.h"
#include "wyjatki.h"
#include <sstream>
#include <fstream>
#include <vector>


Autentykacja::Autentykacja(void)
{
}

Autentykacja::Autentykacja(string sciezka)
{
	setNazwaPliku(sciezka);
}

Autentykacja::~Autentykacja(void)
{
}

DOSTEP Autentykacja::getPrawaDostepu(string nazwa, string haslo) throw(OpenException)
{
	string doWczytania[] = {"access="};
	const int DO_WCZYTANIA = 1; 
	
	ifstream plik; 
	plik.open(nazwaPliku.c_str(), ios::in);

	// gdy próba otwarcia pliku nie powiod³a siê.
	if(!plik)
	{
		plik.clear(plik.rdstate() & ~ios::failbit);
		OpenException openEx("B³¹d otwarcia pliku z prawami dostepu  !!!");
		throw openEx;
	}
	
	char linia[100];		// linia wczytana z pliku konfiguracyjnego
	
	// zmienna wskazuj¹ca na poprawne wczytanie danych gdy jest równa 3
	int wczytano = 0;

	int test = 0;
			
	while(plik.getline(linia, 100))
	{
		string slinia(linia);
		
		string::size_type pozycja1 = slinia.find("user=");
		string::size_type pozycja2 = slinia.find(nazwa);
		
		
		if(pozycja1 != string::npos && pozycja2 != string::npos)
		{
			plik.getline(linia, 100);

			slinia = linia;

			pozycja1 = slinia.find("pass=");
			pozycja2 = slinia.find(haslo);

			if(pozycja1 != string::npos && pozycja2 != string::npos)
			{
				bool koniec = false;
				
				do
				{
					string::size_type pozycja = slinia.find("#");
					
					// gdy koniec opisu
					if(pozycja != string::npos)
						koniec = true;
						
					// gdy nie koniec opisu wczytujemy dane
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
	
	}
	
	plik.close();
		
	// sprawdzenie czy istnieje uzytkownik o danej nazwie i hasle
	if(wczytano != DO_WCZYTANIA)
	{
		dostep = DOSTEP::Brak;
	}

	return dostep;

}

void Autentykacja::setNazwaPliku(string sciezka) throw(OpenException)
{
	ifstream plik; 
	plik.open(sciezka.c_str(), ios::in);

	// gdy próba otwarcia pliku nie powiod³a siê.
	if(!plik)
	{
		plik.clear(plik.rdstate() & ~ios::failbit);
		OpenException openEx("B³¹d otwarcia pliku z prawami dostepu !!!");
		throw openEx;
	}

	nazwaPliku = sciezka;
	plik.close();
}

bool Autentykacja::wczytaj(string dana, string slinia) 
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
				if(dana == "access=")
					dostep = static_cast<DOSTEP> (atoi (sliczba.c_str()));
				
			}
		}while(pozycja != string::npos);
		
		return true;
	}
	
	// gdy w lini nie ma szukanych danych
	return false; 
}
