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
		Funkcja wirtualna. S�u�y do symulacji obiektu na podstawie 
		pojedy�czej pr�bki.
		@param wejscie pr�bka podawana na wej�cie obiektu
		@return warto�� wyj�cia z obiektu na podan� warto�� wej�ciow�
	*/
	virtual double symuluj(double wejscie) = 0;
	
	/**
		Funkcja wirtualna. S�u�y do symulacji obiektu na podstawie 
		zestawu pr�bek wczytanego z pliku o podanej �cie�ce.
		@param we �cie�ka do pliku z zestawem pr�bek do symulacji
		@param wy �cie�ka do pliku z gdzie zapisane b�d� wyniki symulacji
	*/
	virtual void symuluj(string we, string wy) = 0;
};

#endif