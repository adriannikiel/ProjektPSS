/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/
#ifndef OBJEKT_H
#define OBJEKT_H

#include "zaklocenie.h"
#include "SISO.h"
#include <string>

using namespace std;

/**
	Klasa abstrakcyjna. Stanowi interface dla modelu ARMAX.
	@author Adrian Nikiel
*/
class Objekt: public SISO {

public:

	/**
		Konstruktor domniemany klasy abstrakcyjnej objekt.
	*/
	Objekt(){}
	
	/**
		Konstruktor klasy abstrakcyjnej.
		@param ob nazwa obiektu.
	*/
	Objekt(string ob): nazwaObiektu(ob){}
	
	/**
		Destruktor wirtualny domniemany klasy abstrakcyjnej objekt.
	*/
	virtual ~Objekt(){}
	
	/**
		Funkcja wirtualna. Służy do symulacji obiektu na podstawie 
		pojedyńczej próbki.
		@param wejscie próbka podawana na wejście obiektu
		@return wartość wyjścia z obiektu na podaną wartość wejściową
	*/
	virtual double symuluj(double wejscie) = 0;
	
	/**
		Funkcja wirtualna. Służy do symulacji obiektu na podstawie 
		zestawu próbek wczytanego z pliku o podanej ścieżce.
		@param we ścieżka do pliku z zestawem próbek do symulacji
		@param wy ścieżka do pliku z gdzie zapisane będą wyniki symulacji
	*/
	virtual void symuluj(string we, string wy) = 0;

	/**
		Funkcja czysto wirtualna służy do zrestartowania symulacji krokowej.
	*/
	virtual void reset() = 0;

	/**
		Funkcja czysto wirtualna służąca do zmiany prarametrów obiektu.
		@param dana nazwa danej do zmiany
		@param wartosc string zawierający linię którą należy przeszukać w celu wczytania danych
		@return zwraca true gdy znaleziono szukane dane lub false gdy tych danych nie znaleziono
	*/
	virtual bool zmienParametr(string dana, string wartosc) = 0;

	/**
		Funkcja zwraca nazwę obiektu.
		@return string z nazwą obiektu
	*/
	string getNazwa() { return nazwaObiektu; };

protected:
	string nazwaObiektu;	
	Zaklocenie zakl;				
};

#endif
