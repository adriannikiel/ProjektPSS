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
		Funkcja przeprowadzaj�ce jeden krok symulajci
		@return warto�� wyj��ia z obiektu
	*/
	double symuluj();

	/**
		Funkcja zwracaj�ca aktualne wyj�cie z obiektu.
		@return aktualne wyj�cie z obiektu
	*/
	double getWyjscie()
	{
		return wyj;
	}

	/**
		Funkcja zwracaj�ca aktualne sterowanie.
		@return aktualne sterowanie
	*/
	double getSterowanie()
	{
		return ster;
	}

	/**
		Funkcja zwracaj�ca aktualn� warto�� zadan�.
		@return aktualna warto�� zadana
	*/
	double getWartoscZadana()
	{
		return warZad;
	}

	/**
		Funkcaja resetuj�ca ca�� symulacj�.
	*/
	void reset();

	/**
		Funkcja z�u�y do zmiany regulatora w p�tli regulacji.
		@param reg regulator
	*/
	void zmienRegulator(IRegulator *reg)
	{
		regulator = reg;
	}

	/**
		Funkcja czysto wirtualna umo�liwiaj�ca zmian� parametr�w regulatora.
		@param param nastawy regulatora
	*/
	void setNastawyRegulatora(vector<double> &param);

	/**
		Funkcja s�u�y do zmiany prarametr�w modelu armax.
		@param dana nazwa danej do zmiany
		@param wartosc string zawieraj�cy lini� kt�r� nale�y przeszuka� w celu wczytania danych
		@return zwraca true gdy znaleziono szukane dane lub false gdy tych danych nie znaleziono
	*/
	void setParametrObiektu(string dana, string wartosc);

	/**
		Funkcja z�u�y do zmiany obiektu w p�tli regulacji.
		@param ob obiekt
	*/
	void zmienObjekt(Objekt *ob)
	{
		obiekt = ob;
	}

private:
	Objekt *obiekt;			// obiekt regulacji
	IRegulator *regulator;	// regulator

	double wyj;				// aktualne wyj�cie z obiektu
	double ster;			// sterowanie
	double warZad;			// watto�� zadana
};
#endif