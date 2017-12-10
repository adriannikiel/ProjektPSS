/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#ifndef ADAPTPID_H
#define ADAPTPID_H

#include "IRegulator.h"
#include "wyjatki.h"
#include "Przekaznik.h"
#include "RegulatorPID.h"
#include <vector>

/**
	Klasa adaptacyjnej wersji regulatora PID z eksperymentem przekaznikowym.
	@author Adrian Nikiel
*/
class AdaptPID: public IRegulator
{
public:
	/**
		Konstruktor domniemany klasy AdaptPID.
	*/
		AdaptPID(void);

	/**
		Konstruktor klasy AdaptPID.
		@param wzm wzmocnienie regulatora PID
		@param stRoz stala rozniczkowania regulatora PID
		@param stCal stala calkowania regulatora PID
		@param ON wartosc ON przekaznika
	*/
	AdaptPID(double wzm=1, double stRoz=0, double stCal=1000000, double ON=2.0);

	/** Funkcja wlaczajaca strojenie regulatora 
	*/
    void wlaczStrojenie();

	/** Funkcja sprawdzaj�ca czy regulator jest w stanie strojenia 
		@return strojenie - zmienna boolowska o wartosc true, jesli regulator stroi sie
	*/
	bool isStrojenie() { return strojenie;}

	/**
		Destruktor klasy AdaptPID.
	*/
	virtual ~AdaptPID(void);

	/**
		Funkcja umo�liwiaj�ca zmian� parametr�w regulatora.
		Elementy wektora: {wzmocnienie,Td,Ti,On}
		@param nastawy nastawy przekaznika
		@throws ReadDataException
	*/
	void setNastawyRegulatora(const vector<double> &nastawy);

	/**
		Funkcja umo�liwiaj�ca zmian� jednego parametru regulatora.
		@param parametr parametr regulatora
		@param nastawa nastawa regulatora
	*/
	void setNastawyRegulatora(int parametr, double nastawa);

	/**
		Funkcja zwracaj�ca nastawy konkretnego regulatora.
		@return nastawy regulatora
	*/
	string getNastawy();

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
		Funkcja zwracaj�ca warto�� ON przekaznika
		@return warto�� ON przekaznika
	*/
	double getOn()
	{
		return wartoscOn;
	}

	static const int K = 0;
	static const int TD = 1;
	static const int TI = 2;
	static const int ON = 3;

private:
	/**
		Funkcja wyznaczaj�ca warto�� wyj�cia na podstawie wymuszenia
		i sygna�u z obiektu.
		@param warZadana warto�� zadana
		@param wyjZObiektu warto�� wyj��ia z obiektu regulowanego
	*/
	double liczWyjscie(double warZadana, double wyjZObiektu);

	/**
		Funkcja resetuj�ca stan konkretnego regulatora.
	*/
	void resetRegulatora();
 	
    Przekaznik *przekaznik;		//przekaznik stosowany do doswiadczenia przekaznikowego
    RegulatorPID *regulatorPID;	//regulator PID, ktory bedzie adaptacyjnie strojony
    bool strojenie;				//flaga oznaczajace etap strojenia
    
	double uchyb; 
	
	double wzmocnienie;	// wzmocnienie regulatora
	double Td;			// czas wyprzedzenia
	double Ti;			// czas zdwojenia
	double wartoscOn;	//wartosc d przekaznika
	
	// zmienne pomocnicze przy wyliczaniu sterowania podczas strojenia
	int iloscPrzejscPrzezZero, czas;  
    double maxReg,minReg;
	double trzymanaWartosc;
	bool poczStrojenia;

};

#endif