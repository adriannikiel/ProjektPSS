/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/

#ifndef REGULATORP_H
#define REGULATORP_H

#include "IRegulator.h"
#include "wyjatki.h"
#include <vector>

class RegulatorP: public IRegulator
{
public:
	/**
		Konstruktor domniemany klasy RegulatorP.
	*/
	RegulatorP(void);

	/**
		Konstruktor klasy RegulatorP.
		@param sciezka �cie�ka do pliku konfiguracyjnego
		@param obiekt nazwa obiektu
		@throws OpenException
		@throws ReadDataException
	*/
	RegulatorP(string sciezka, string obiekt);

	/**
		Konstruktor klasy RegulatorP.
		@param k wzmocnienie regulatora P
	*/
	RegulatorP(double k);

	/**
		Destruktor klasy RegulatorP.
	*/
	virtual ~RegulatorP(void);

	/**
		Funkcja umo�liwiaj�ca zmian� parametr�w regulatora.
		Elementy wektora: {wzmocnienie}
		@param nastawy nastawy regulatora
		@throws ReadDataException
	*/
	void setNastawyRegulatora(const vector<double> &nastawy);

	/**
		Funkcja zwracaj�ca warto�� wzmocnienia regulatora P.
		@return warto�� wzmocnienia regulatora P
	*/
	double getK()
	{
		return wzmocnienie;
	}

	/**
		Funkcja czysto wirtualna umo�liwiaj�ca zmian� jednego parametru regulatora.
		@param parametr parametr regulatora
		@param nastawa nastawa regulatora
	*/
	void setNastawyRegulatora(int parametr, double nastawa);

	/**
		Funkcja zwracaj�ca nastawy konkretnego regulatora.
		@return nastawy regulatora
	*/
	string getNastawy();

	static const int K = 0;

private:
	/**
		Funkcja czysto wirtualna wyznaczaj�ca warto�� wyj�cia na podstawie wymuszenia
		i sygna�u z obiektu.
		@param warZadana warto�� zadana
		@param wyjZObiektu warto�� wyj��ia z obiektu regulowanego
	*/
	double liczWyjscie(double warZadana, double wyjZObiektu);

	/**
		Funkcja wczytuje dane o podanej nazwie z danej lini.
		Funkcja zwraca odpowiednio true lub false gdzy dane zostan� wczytane lub nie.
		Na przyk�ad:  wczytaj("SKOK=","SKOK= 1").
		
		@param dana nazwa danej do wczytania
		@param slinia string zawieraj�cy lini� kt�r� nale�y przeszuka� w celu wczytania danych
		@return zwraca true gdy znaleziono szukane dane lub false gdy tych danych nie znaleziono 
		@throws ReadDataException
	*/
	bool wczytaj(string dana, string slinia);

	/**
		Funkcaja virtualna resetuj�ca stan konkretnego regulatora.
	*/
	void resetRegulatora(){};

	double wzmocnienie;	// wzmocnienie regulatora
};
#endif