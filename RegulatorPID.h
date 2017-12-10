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
		@param sciezka œcie¿ka do pliku konfiguracyjnego
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
		@param stTp czas ró¿nicy progresywnej
		@param warPocz warunek pocz¹tkowy elementu ca³kuj¹cego
		@param wspN wspó³czynnik N filtru
		@param stB stala b
	*/
	RegulatorPID(double k, double stRoz, double stCal, double stTp,
						   double warPocz, double wspN, double stB);

	/**
		Destruktor klasy RegulatorPID.
	*/
	virtual ~RegulatorPID(void);

	/**
		Funkcja umo¿liwiaj¹ca zmianê parametrów regulatora.
		Elementy wektora: {wzmocnienie,Td,Ti,Tp,warunekPocz¹tkowy,N,b}
		@param nastawy nastawy regulatora
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
		Funkcja zwracaj¹ca wartoœæ wzmocnienia regulatora PID.
		@return wartoœæ wzmocnienia regulatora PID
	*/
	double getK()
	{
		return wzmocnienie;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ czasu wyprzedzenia regulatora PID.
		@return wartoœæ czasu wyprzedzenia regulatora PID
	*/
	double getTd()
	{
		return Td;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ czasu zdwojenia regulatora PID.
		@return wartoœæ czasu zdwojenia regulatora PID
	*/
	double getTi()
	{
		return Ti;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ czasu ró¿nicy progresywnej regulatora PID.
		@return wartoœæ czasu ró¿nicy progresywnej regulatora PID
	*/
	double getTp()
	{
		return Tp;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ warunku pocz¹tkowego regulatora PID.
		@return wartoœæ warunku pocz¹tkowego regulatora PID
	*/
	double getWarPocz()
	{
		return warunekPocz;
	}
	/**
		Funkcja zwracaj¹ca wartoœæ parametru N regulatora PID.
		@return wartoœæ parametru N regulatora PID
	*/
	double getN()
	{
		return N;
	}
	/**
		Funkcja zwracaj¹ca wartoœæ parametru b regulatora PID.
		@return wartoœæ wzmocnienia regulatora PID
	*/
	double getB()
	{
		return b;
	}

	/**
		Funkcja zwracaj¹ca nastawy konkretnego regulatora.
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
		Funkcja czysto wirtualna wyznaczaj¹ca wartoœæ wyjœcia na podstawie wymuszenia
		i sygna³u z obiektu.
		@param warZadana wartoœæ zadana
		@param wyjZObiektu wartoœæ wyjœæia z obiektu regulowanego
	*/
	double liczWyjscie(double warZadana, double wyjZObiektu);

	/**
		Funkcja wczytuje dane o podanej nazwie z danej lini.
		Funkcja zwraca odpowiednio true lub false gdzy dane zostan¹ wczytane lub nie.
		Na przyk³ad:  wczytaj("SKOK=","SKOK= 1").
		
		@param dana nazwa danej do wczytania
		@param slinia string zawieraj¹cy liniê któr¹ nale¿y przeszukaæ w celu wczytania danych
		@return zwraca true gdy znaleziono szukane dane lub false gdy tych danych nie znaleziono 
		@throws ReadDataException
	*/
	bool wczytaj(string dana, string slinia);

	/**
		Funkcaja virtualna resetuj¹ca stan konkretnego regulatora.
	*/
	void resetRegulatora();

	double wzmocnienie;	// wzmocnienie regulatora
	double Td;			// czas wyprzedzenia
	double Ti;			// czas zdwojenia
	double Tp;			// czas ró¿nicy progresywnej
	double warunekPocz; // warunek pocz¹tkowy elementu ca³kuj¹cego
	double N;			// wspó³czynnnik filtru
	double b;			// sta³a

	double poprzWyj;	// poprzednie wyjœcie z regulatora
	double y1, y2;	// wartoœæi ostatnich wyjœæ z obiektu
	double r1;		// wartoœci ostatnich wartoœci zadanch
};
#endif