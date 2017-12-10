/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/

#ifndef REGULATORPID_H
#define REGULATORPID_H

#include "IRegulator.h"
#include "wyjatki.h"
#include <vector>

class RegulatorPID: public IRegulator
{
public:
	/**
		Konstruktor domniemany klasy RegulatorPID.
	*/
	RegulatorPID(void);

	/**
		Konstruktor klasy RegulatorPID.
		@param sciezka �cie�ka do pliku konfiguracyjnego
		@param obiekt nazwa obiektu
		@throws OpenException
		@throws ReadDataException
	*/
	RegulatorPID(string sciezka, string obiekt);

	/**
		Konstruktor klasy RegulatorPID.
		@param k wzmocnienie regulatora PID
		@param stRoz czas wyprzedzenia
		@param stCal czas zdwojenia
		@param stTp czas r�nicy progresywnej
		@param warPocz warunek pocz�tkowy elementu ca�kuj�cego
		@param wspN wsp�czynnik N filtru
		@param stB stala b
	*/
	RegulatorPID(double k, double stRoz, double stCal, double stTp,
						   double warPocz, double wspN, double stB);

	/**
		Destruktor klasy RegulatorPID.
	*/
	virtual ~RegulatorPID(void);

	/**
		Funkcja umo�liwiaj�ca zmian� parametr�w regulatora.
		Elementy wektora: {wzmocnienie,Td,Ti,Tp,warunekPocz�tkowy,N,b}
		@param nastawy nastawy regulatora
		@throws ReadDataException
	*/
	void setNastawyRegulatora(const vector<double> &nastawy);

	/**
		Funkcja czysto wirtualna umo�liwiaj�ca zmian� jednego parametru regulatora.
		@param parametr parametr regulatora
		@param nastawa nastawa regulatora
	*/
	void setNastawyRegulatora(int parametr, double nastawa);

	/**
		Funkcja zwracaj�ca warto�� wzmocnienia regulatora PID.
		@return warto�� wzmocnienia regulatora PID
	*/
	double getK()
	{
		return wzmocnienie;
	}

	/**
		Funkcja zwracaj�ca warto�� czasu wyprzedzenia regulatora PID.
		@return warto�� czasu wyprzedzenia regulatora PID
	*/
	double getTd()
	{
		return Td;
	}

	/**
		Funkcja zwracaj�ca warto�� czasu zdwojenia regulatora PID.
		@return warto�� czasu zdwojenia regulatora PID
	*/
	double getTi()
	{
		return Ti;
	}

	/**
		Funkcja zwracaj�ca warto�� czasu r�nicy progresywnej regulatora PID.
		@return warto�� czasu r�nicy progresywnej regulatora PID
	*/
	double getTp()
	{
		return Tp;
	}

	/**
		Funkcja zwracaj�ca warto�� warunku pocz�tkowego regulatora PID.
		@return warto�� warunku pocz�tkowego regulatora PID
	*/
	double getWarPocz()
	{
		return warunekPocz;
	}
	/**
		Funkcja zwracaj�ca warto�� parametru N regulatora PID.
		@return warto�� parametru N regulatora PID
	*/
	double getN()
	{
		return N;
	}
	/**
		Funkcja zwracaj�ca warto�� parametru b regulatora PID.
		@return warto�� wzmocnienia regulatora PID
	*/
	double getB()
	{
		return b;
	}

	/**
		Funkcja zwracaj�ca nastawy konkretnego regulatora.
		@return nastawy regulatora
	*/
	string getNastawy();

	static const int K = 0;
	static const int TD = 1;
	static const int TI = 2;
	static const int TP = 3;
	static const int WARUNEK_POCZATKOWY = 4;
	static const int N_PARAM = 5;
	static const int B = 6;

	double liczWy(double warZadana, double wyjZObiektu) { 
		return liczWyjscie(warZadana, wyjZObiektu);
	}
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
	void resetRegulatora();

	double wzmocnienie;	// wzmocnienie regulatora
	double Td;			// czas wyprzedzenia
	double Ti;			// czas zdwojenia
	double Tp;			// czas r�nicy progresywnej
	double warunekPocz; // warunek pocz�tkowy elementu ca�kuj�cego
	double N;			// wsp�czynnnik filtru
	double b;			// sta�a

	double poprzWyj;	// poprzednie wyj�cie z regulatora
	double y1, y2;	// warto��i ostatnich wyj�� z obiektu
	double r1;		// warto�ci ostatnich warto�ci zadanch
};
#endif