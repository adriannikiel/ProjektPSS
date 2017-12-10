/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/
#ifndef IDENTYFIKACJA_H
#define IDENTYFIKACJA_H

#include <vector>

using namespace std;

/**
	Klasa do identyfikacji obiekt�w armax.
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
		Funkcja s�u�y do liczenia parametr�w identyfikacji modelu armax.
		@param wejscie aktualna wartosc wejsciowa do obiektu
		@param wyjscie aktualna wartosc wyjsciowa z obiektu
		@return zwraca estymowany wektor parametr�w teta
	*/
	void liczParam(double wejscie, double wyjscie);
	
	/**
		Funkcja s�u�y do zwracania wektora parametr�w identyfikacji modelu armax.
		@return zwraca estymowany wektor parametr�w teta
	*/
	vector<double> getParam();
	
	/**
		Funkcja zwracaj�ca stopien wielomianu A.
		@return zwraca stopien wielomianu A - dA
	*/
	int getDa()
	{
		return dA;
	}

	/**
		Funkcja zwracaj�ca stopien wielomianu B.
		@return zwraca stopien wielomianu B - dB
	*/
	int getDb()
	{
		return dB;
	}

	/**
		Funkcja ustawiaj�ca warto�� parametru alfa.
		@param nowaAlfa nowa warto�� parametru alfa
	*/
	void setAlfa(double nowaAlfa)
	{
		alfa = nowaAlfa;
	}

	/**
		Funkcja s�u�y do resetowania ustawien obiektu typu Identyfikacja.
	*/
	void reset();

	/**
		Funkcja s��ca do zapisywania wyniu identyfikacji do pliku.
		@param plik �cie�ka do pliku
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