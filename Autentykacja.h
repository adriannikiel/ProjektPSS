/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#ifndef AUTENTYKACJA_H
#define AUTENTYKACJA_H

#include <string>

using namespace std;

/**
	Typ wyliczeniowy ukazujacy prawa dostepu
*/
enum DOSTEP{ Czytanie=1, Pisanie, Brak };

/**
	Klasa do autentykacji uzytkownika.
	@author Adrian Nikiel
*/
class Autentykacja
{
public:
	/**
		Konstruktor domniemany klasy Autentykacja.
	*/
	Autentykacja(void);

	/**
		Konstruktor klasy Autentykacja.
		@param sciezka œcie¿ka do pliku z prawami dostepu
	*/
	Autentykacja(string sciezka);

	/**
		Destruktor klasy Autentykacja.
	*/
	~Autentykacja(void);

	/**
		Funkcja zwracaj¹ca prawa dostepu 
		@param nazwa nazwa uzytkownika
		@param haslo haslo uzytkownika
		@throws OpenException
		@return funkcja zwraca rodzaj dostepu
	*/
	DOSTEP getPrawaDostepu(string nazwa, string haslo);

	/**
		Funkcja ustawiajaca plik z prawami dostepu
		@param sciezka œcie¿ka do pliku z prawami dostepu
		@throws OpenException
	*/
	void setNazwaPliku(string sciezka);

private:
	
	/**
		Funkcja wczytuje dane o podanej nazwie z danej lini.
		Funkcja zwraca odpowiednio true lub false gdzy dane zostan¹ wczytane lub nie.
		Na przyk³ad:  wczytaj("SKOK=","SKOK= 1").
		
		@param dana nazwa danej do wczytania
		@param slinia string zawieraj¹cy liniê któr¹ nale¿y przeszukaæ w celu wczytania danych
		@return zwraca true gdy znaleziono szukane dane lub false gdy tych danych nie znaleziono 
	*/
	bool wczytaj(string dana, string slinia);

	string nazwaPliku;
	DOSTEP dostep;

};

#endif