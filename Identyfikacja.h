/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/
#ifndef IDENTYFIKACJA_H
#define IDENTYFIKACJA_H

#include <vector>

using namespace std;

/**
	Klasa do identyfikacji obiektów armax.
	@author Adrian Nikiel
*/
class Identyfikacja
{
public:
	/**
		Konstruktor domniemany klasy Identyfikacja.
	*/
	Identyfikacja(void);

	/**
		Konstruktor klasy Identyfikacja.
		@param dAA stopien wielomianu A
		@param dBB stopien wielomianu B
		@param dCC stopien wielomianu C
		@param opoz czas opoznienia
		@param wspZapom wspolczynnik zapominania
		@param lambda warunek poczatkowy dla macierzy P
	*/
	Identyfikacja(int dAA, int dBB, int dCC, int opoz, double wspZapom,double lambda);

	/**
		Funkcja s³u¿y do liczenia parametrów identyfikacji modelu armax.
		@param wejscie aktualna wartosc wejsciowa do obiektu
		@param wyjscie aktualna wartosc wyjsciowa z obiektu
		@return zwraca estymowany wektor parametrów teta
	*/
	void liczParam(double wejscie, double wyjscie);
	
	/**
		Funkcja s³u¿y do zwracania wektora parametrów identyfikacji modelu armax.
		@return zwraca estymowany wektor parametrów teta
	*/
	vector<double> getParam();
	
	/**
		Funkcja zwracaj¹ca stopien wielomianu A.
		@return zwraca stopien wielomianu A - dA
	*/
	int getDa()
	{
		return dA;
	}

	/**
		Funkcja zwracaj¹ca stopien wielomianu B.
		@return zwraca stopien wielomianu B - dB
	*/
	int getDb()
	{
		return dB;
	}

	/**
		Funkcja ustawiaj¹ca wartoœæ parametru alfa.
		@param nowaAlfa nowa wartoœæ parametru alfa
	*/
	void setAlfa(double nowaAlfa)
	{
		alfa = nowaAlfa;
	}

	/**
		Funkcja s³u¿y do resetowania ustawien obiektu typu Identyfikacja.
	*/
	void reset();

	/**
		Funkcja s³ó¿¹ca do zapisywania wyniu identyfikacji do pliku.
		@param plik œcie¿ka do pliku
	*/
	void zapisDoPliku(string plik);

	/**
		Destruktor domniemany klasy Identyfikacja.
	*/
	~Identyfikacja(){};

private:
	int dA,dB,dC,k;
	double alfa,lambda;
	vector<double> A, B, C, ePoprzednie,uPoprzednie,yPoprzednie;
	vector<double> teta;
	vector<vector<double> >  P;

};

#endif