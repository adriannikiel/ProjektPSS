/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/
#ifndef ZAKLOCENIE_H
#define ZAKLOCENIE_H

#include <vector>

using namespace std;

/**
	Klasa generująca zakłócenie o podanej długości i wariancji.
	@author Adrian Nikiel
*/
class Zaklocenie{

public:
	/**
		Konstruktor domniemany klasy Zaklocenie.
	*/
	Zaklocenie(){};
	
	/**
		Konstruktor klasy Zaklocenie. Wariancja liczona jest dla samego szumu.
		@param dlugosc dlugość ciągu zakłócenia 
		@param wariancja wariancja wygenerowanego zakłócenia
	*/
	Zaklocenie(int dlugosc, double wariancja);
	
	/**
		Konstruktor klasy Zaklocenie. Wariancja liczona jest dla toru zakłócenia.
		@param dlugosc dlugość ciągu zakłócenia 
		@param wariancja wariancja wygenerowanego zakłócenia
		@param C wektor C modelu ARMAX.
		@param A wektor A modelu ARMAX.
	*/
	Zaklocenie(int dlugosc, double wariancja, const vector<double> &C, const vector<double> &A);
	
	/**
		Destruktor domniemany klasy Zaklocenie.
	*/
	~Zaklocenie(){};

	/**
		Funkcja licząca wariancję wygenerowanego zakłócenia.
		@return wariancja zakłócenia
	*/
	double liczWariancje();
	
	/**
		Funkcja zwraca wartość dla zakłócenia o podanym indeksie.
		@param pozycja indeks próbki zakłócenia
		@return wartość dla zakłócenia o podanym indeksie
	*/
	double getWartosc(int pozycja) { return zakl[pozycja]; }

protected:
	vector<double> zakl; 
};

#endif