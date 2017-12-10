/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/

#ifndef PETLAREGULACJI_H
#define PETLAREGULACJI_H

#include "objekt.h"
#include "IRegulator.h"

class PetlaRegulacji
{
public:
	/**
		Konstruktor domniemany klasy PetlaRegulacji.
	*/
	PetlaRegulacji();

	/**
		Konstruktor klasy PetlaRegulacji.
		@param obDyskr obiekt dyskretny
		@param reg regulator
	*/
	PetlaRegulacji(Objekt *obDyskr, IRegulator *reg);

	/**
		Destruktor domniemany klasy PetlaRegulacji.
	*/
	~PetlaRegulacji();

	/**
		Funkcja przeprowadzaj¹ce jeden krok symulajci
		@return wartoœæ wyjœæia z obiektu
	*/
	double symuluj();

	/**
		Funkcja zwracaj¹ca aktualne wyjœcie z obiektu.
		@return aktualne wyjœcie z obiektu
	*/
	double getWyjscie()
	{
		return wyj;
	}

	/**
		Funkcja zwracaj¹ca aktualne sterowanie.
		@return aktualne sterowanie
	*/
	double getSterowanie()
	{
		return ster;
	}

	/**
		Funkcja zwracaj¹ca aktualn¹ wartoœæ zadan¹.
		@return aktualna wartoœæ zadana
	*/
	double getWartoscZadana()
	{
		return warZad;
	}

	/**
		Funkcaja resetuj¹ca ca³¹ symulacjê.
	*/
	void reset();

	/**
		Funkcja z³u¿y do zmiany regulatora w pêtli regulacji.
		@param reg regulator
	*/
	void zmienRegulator(IRegulator *reg)
	{
		regulator = reg;
	}

	/**
		Funkcja czysto wirtualna umo¿liwiaj¹ca zmianê parametrów regulatora.
		@param param nastawy regulatora
	*/
	void setNastawyRegulatora(vector<double> &param);

	/**
		Funkcja s³u¿y do zmiany prarametrów modelu armax.
		@param dana nazwa danej do zmiany
		@param wartosc string zawieraj¹cy liniê któr¹ nale¿y przeszukaæ w celu wczytania danych
		@return zwraca true gdy znaleziono szukane dane lub false gdy tych danych nie znaleziono
	*/
	void setParametrObiektu(string dana, string wartosc);

	/**
		Funkcja z³u¿y do zmiany obiektu w pêtli regulacji.
		@param ob obiekt
	*/
	void zmienObjekt(Objekt *ob)
	{
		obiekt = ob;
	}

private:
	Objekt *obiekt;			// obiekt regulacji
	IRegulator *regulator;	// regulator

	double wyj;				// aktualne wyjœcie z obiektu
	double ster;			// sterowanie
	double warZad;			// wattoœæ zadana
};
#endif