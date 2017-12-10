/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/
#ifndef SISO_H
#define SISO_H

#include <string>

using namespace std;

class SISO
{
public:
	/**
		Konstruktor domniemany klasy abstrakcyjnej SISO.
	*/
	SISO();

	/**
		Destruktor wirtualny domniemany klasy abstrakcyjnej objekt.
	*/
	virtual ~SISO(){}
	
	/**
		Funkcja wirtualna. S³u¿y do symulacji obiektu na podstawie 
		pojedyñczej próbki.
		@param wejscie próbka podawana na wejœcie obiektu
		@return wartoœæ wyjœcia z obiektu na podan¹ wartoœæ wejœciow¹
	*/
	virtual double symuluj(double wejscie) = 0;
	
	/**
		Funkcja wirtualna. S³u¿y do symulacji obiektu na podstawie 
		zestawu próbek wczytanego z pliku o podanej œcie¿ce.
		@param we œcie¿ka do pliku z zestawem próbek do symulacji
		@param wy œcie¿ka do pliku z gdzie zapisane bêd¹ wyniki symulacji
	*/
	virtual void symuluj(string we, string wy) = 0;
};

#endif