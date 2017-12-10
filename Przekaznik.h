/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#ifndef PRZEKAZNIK_H
#define PRZEKAZNIK_H

#include "IRegulator.h"
#include "wyjatki.h"
#include <vector>

/**
	Klasa przekaznika.
	@author Adrian Nikiel
*/
class Przekaznik: public IRegulator
{
public:
	/**
		Konstruktor domniemany klasy Przekaznik.
	*/
		Przekaznik(void);

	/**
		Konstruktor klasy Przekaznik.
		@param ON wartosc wlaczenia przekaznika
		@param OFF wartosc wylaczenia przekaznika
		@param histereza szerokosc strefy histerezy
	*/
		Przekaznik(double ON, double OFF, double histereza=0);

	/**
		Konstruktor klasy Przekaznik.
		@param ON wartosc wlaczenia przekaznika
		@param histereza szerokosc strefy histerezy
	*/
		Przekaznik(double ON, double histereza=0);

	/**
		Destruktor klasy Przekaznik.
	*/
	virtual ~Przekaznik(void);

	/**
		Funkcja umo¿liwiaj¹ca zmianê parametrów regulatora.
		Elementy wektora: {wartON, wartOFF, histereza }
		@param nastawy nastawy przekaznika
		@throws ReadDataException
	*/
	void setNastawyRegulatora(const vector<double> &nastawy);

	/**
		Funkcja czysto wirtualna umo¿liwiaj¹ca zmianê jednego parametru regulatora.
		@param parametr parametr regulatora
		@param nastawa nastawa regulatora
	*/
	void setNastawyRegulatora(int parametr, double nastawa);

	/**
		Funkcja zwracaj¹ca nastawy konkretnego regulatora.
		@return nastawy regulatora
	*/
	string getNastawy();

	static const int pwartON = 0;
	static const int pwartOFF = 1;
	static const int phistereza = 2;

	double liczWy(double warZadana, double wyjZObiektu);
private:
	/**
		Funkcja wyznaczaj¹ca wartoœæ wyjœcia na podstawie wymuszenia
		i sygna³u z obiektu.
		@param warZadana wartoœæ zadana
		@param wyjZObiektu wartoœæ wyjœæia z obiektu regulowanego
	*/
	double liczWyjscie(double warZadana, double wyjZObiektu);

	/**
		Funkcaja resetuj¹ca stan konkretnego regulatora.
	*/
	void resetRegulatora();

	double uchyb;

    double wartON;		//wartosc ON przekaznika
    double wartOFF;		//wartosc OFF przekaznika
    double histereza;	//szerokosc strefy histerezy
	
};

#endif