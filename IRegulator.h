/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/

#ifndef IREGULATOR_H
#define IREGULATOR_H

#include "zaklocenie.h"
#include "SISO.h"
#include <vector>
#include <string.h>

using namespace std;

/**
	Interfejs regulatora. Mo�na go wyko�ysta� do implementacji dowolnego regulatora.
	Nale�y zaimplementowa� nast�puj�ce funkne: <br>
	void setNastawyRegulatora(vector<double> &nastawy) <br>
	void setNastawyRegulatora(string plik) <br>
	virtual double liczWyjscie(double wejscieReg) <br>
*/
class IRegulator: public SISO
{

public:
	/**
		Funkcja czysto wirtualna umo�liwiaj�ca zmian� parametr�w regulatora.
		@param nastawy nastawy regulatora
	*/
	virtual void setNastawyRegulatora(const vector<double> &nastawy)=0;

	/**
		Funkcja czysto wirtualna umo�liwiaj�ca zmian� jednego parametru regulatora.
		@param parametr parametr regulatora
		@param nastawa nastawa regulatora
	*/
	virtual void setNastawyRegulatora(int parametr, double nastawa)=0;

	/**
		Konstruktor domniemany interfejsu IRegulator.
	*/
	IRegulator(void);

	/**
		Konstruktor interfejsu IRegulator.
		@param nazwaRegulatora nazwa regulatora
	*/
	IRegulator(string nazwaRegulatora):PI(3.1415926535897932384626433832795)
	{
		nazwa = nazwaRegulatora;

		// inicjalizacja licznika iteracji
		iteracja = 0;
	}

	/**
		Wirtualny destruktor interfejsu IRegulator.
	*/
	virtual ~IRegulator(void);

	/**
		Funkcja zwracaj�ca nazw� regulatora.
		@return nazwa regulaotra
	*/
	string getNazwa()
	{
		return nazwa;
	}

	/**
		Funkcja wyznaczaj�ca sterowanie na podstawie sygna�u z obiektu.
		@param sygnal sygna� z obiektu
	*/
	double symuluj(double sygnal);

	/**
		Funkcja wyznaczaj�ca sterowanie na podstawie sygna�u z obiektu.
		@param we plik z wej�ciami do regulatora
		@param wy plik z wyj�ciami z regulatora
	*/
	void symuluj(string we, string wy);

	/**
		Funkcja zwracaj�ca warto�� uchybu regulacji.
		@return warto�� uchybu regulacji
	*/
	double getUchyb()
	{
		return uchyb;
	}

	/**
		Funkcja zwracaj�ca warto�� wyj�cia obiektu.
		@return warto�� wyj�cia obiektu
	*/
	double getWartoscWyjscia()
	{
		return wartoscWyjscia;
	}

	/**
		Funkcja zwracaj�ca warto�� zadan�.
		@return warto�� zadana
	*/
	double getWartoscZadana()
	{
		return wartoscZadana;
	}

	/**
		Funkcja umo�liwiaj�ca zmian� sygna�u warto�ci zadanej.
		Mo�liwe sygna�y do wykozystania: <br>
		 - skok jednostkowy (IRegulator::SKOK) <br>
		 - impuls Kronekera (IRegulator::KRONEKER) <br>
		 - fala prostok�tna (IRegulator::PROSTOKAT) <br>
		 - sinus (IRegulator::SINUS) <br>
		 - fala tr�jk�tna (IRegulator::TROJKAT) <br>
		 - szum (IRegulator::SZUM) <br>
		Mo�liwe jest r�wnie� ��czenie sygna��w, np. IRegulator::SKOK | IRegulator::SINUS.
		@param typSygnalu sygna� lub z�o�enie sygna��w warto�ci zadanej
	*/
	void setSygnal(int typSygnalu);

	/**
		Funkcja umo�liwiaj�ca odczyt sygna�u warto�ci zadanej.
		Mo�liwe sygna�y: <br>
		 - skok jednostkowy (IRegulator::SKOK) <br>
		 - impuls Kronekera (IRegulator::KRONEKER) <br>
		 - fala prostok�tna (IRegulator::PROSTOKAT) <br>
		 - sinus (IRegulator::SINUS) <br>
		 - fala tr�jk�tna (IRegulator::TROJKAT) <br>
		 - szum (IRegulator::SZUM) <br>
		Mo�liwe jest r�wnie� ��czenie sygna��w, np. IRegulator::SKOK | IRegulator::SINUS.
		@return sygna� lub z�o�enie sygna��w warto�ci zadanej
	*/
	int getSygnal();

	// Mo�liwe do wyboru typy sygna��w
	static const int SKOK = 1;
	static const int KRONEKER = 1<<1;
	static const int PROSTOKAT = 1<<2;
	static const int SINUS = 1<<3;
	static const int TROJKAT = 1<<4;
	static const int SZUM = 1<<5;

	/**
		Funkcja umo�liwiaj�ca zmian� parametrow skoku jednostkowego.
		@param wartoscSkoku wartosc skoku
	*/
	void setSkok(double wartoscSkoku);

	/**
		Funkcja zwracaj�ca warto�� skoku jednostkowego.
		@return warto�� skoku jednostkowego
	*/
	double getSkok()
	{
		return skok;
	}

	/**
		Funkcja umo�liwiaj�ca zmian� parametrow fali prostokatnej.
		@param amplituda amplituda fali prostokatnej
		@param okres okres fali prostokatnej
		@param wypelnienie wypelnienie fali prostokatnej
	*/
	void setProstokat(double amplituda, int okres, double wypelnienie);

	/**
		Funkcja zwracaj�ca warto�� amplitudy fali prostok�tnej.
		@return warto�� amplitudy fali prostok�tnej
	*/
	double getProstokatAmplituda()
	{
		return amplitudaP;
	}

	/**
		Funkcja zwracaj�ca warto�� okresu fali prostok�tnej.
		@return warto�� okresu fali prostok�tnej
	*/
	int getProstokatOkres()
	{
		return okresP;
	}

	/**
		Funkcja zwracaj�ca warto�� wype�nienia fali prostok�tnej.
		@return warto�� wype�nienia fali prostok�tnej
	*/
	double getProstokatWypelnienie()
	{
		return wypelnienieP;
	}

	/**
		Funkcja umo�liwiaj�ca zmian� parametrow przebiegu sinusoidalnego.
		@param amplituda amplituda sinusoidy
		@param okres okres sinusoidy
	*/
	void setSinus(double amplituda, int okres);

	/**
		Funkcja zwracaj�ca warto�� amplitudy przebiegu sinusoidalnego.
		@return warto�� amplitudy przebiegu sinusoidalnego
	*/
	double getSinusAmplituda()
	{
		return amplitudaS;
	}

	/**
		Funkcja zwracaj�ca warto�� okresu przebiegu sinusoidalnego.
		@return warto�� okresu przebiegu sinusoidalnego
	*/
	int getSinusOkres()
	{
		return okresS;
	}

	/**
		Funkcja umo�liwiaj�ca zmian� parametrow przebiegu tr�jk�tnego.
		@param amplituda amplituda przebiegu tr�jk�tnego
		@param okres okres przebiegu tr�jk�tnego
	*/
	void setTrojkat(double amplituda, int okres);

	/**
		Funkcja zwracaj�ca warto�� amplitudy przebiegu tr�jk�tnego.
		@return warto�� amplitudy przebiegu tr�jk�tnego
	*/
	double getTrojkatAmplituda()
	{
		return amplitudaT;
	}

	/**
		Funkcja zwracaj�ca warto�� okresu przebiegu tr�jk�tnego.
		@return warto�� okresu przebiegu tr�jk�tnego
	*/
	int getTrojkatOkres()
	{
		return okresT;
	}

	/**
		Funkcja umo�liwiaj�ca zmian� parametrow szumu.
		@param war wariancja szumu
		@param dlZakl ilo�� pr�bek szumu
	*/
	void setSzum(double war, int dlZakl);

	/**
		Funkcja zwracaj�ca warto�� wariancji szumu.
		@return warto�� wariancji szumu
	*/
	double getSzumWariancja()
	{
		return wariancja;
	}

	/**
		Funkcja zwracaj�ca ilo�� pr�bek szumu.
		@return ilo�� pr�bek szumu
	*/
	int getSzumDlZakl()
	{
		return dlugoscZaklucenia;
	}

	/**
		Funkcja umo�liwiaj�ca zmian� maksymalnej wartosci wyj�cia z regulatora.
		@param maxWy wariancja szumu
	*/
	void setWyMax(double maxWy);

	/**
		Funkcja zwracaj�ca warto�� maksymaln� na wyj�ciu regulatora.
		@return warto�� maksymalna na wyj�ciu regulatora
	*/
	double getWyMax()
	{
		return wyMax;
	}

	/**
		Funkcja umo�liwiaj�ca zmian� minimalnej wartosci wyj�cia z regulatora.
		@param minWy wariancja szumu
	*/
	void setWyMin(double minWy);

	/**
		Funkcja zwracaj�ca warto�� minimaln� na wyj�ciu regulatora.
		@return warto�� minimalna na wyj�ciu regulatora
	*/
	double getWyMin()
	{
		return wyMin;
	}

	/**
		Funkcja umo�liwiaj�ca zapis nastaw regulatora do pliku.
		@param plik �cie�ka do pliku konfiguracyjnego
		@param nazwa nazwa regulatora
		@throws OpenException
	*/
	void zapisNastawRegulatora(string plik, string nazwa);

	/**
		Funkcaja resetuj�ca stan regulatora.
	*/
	void reset()
	{
		iteracja = 0;
		resetRegulatora();
	}

protected:
	/**
		Funkcja czysto wirtualna wyznaczaj�ca warto�� wyj�cia na podstawie wymuszenia
		i sygna�u z obiektu.
		@param warZadana warto�� zadana
		@param wyjZObiektu warto�� wyj��ia z obiektu regulowanego
	*/
	virtual double liczWyjscie(double warZadana, double wyjZObiektu)=0;

	/**
		Funkcja czysto wirtualna zwracaj�ca nastawy konkretnego regulatora.
		@return nastawy regulatora
	*/
	virtual string getNastawy()=0;

	/**
		Funkcja zwracaj�ca konfiguracj� zawart� w interfejsie IRegulator.
		@return konfiguracja regulatora
	*/
	string getKonfiguracja();
	
	/**
		Funkcaja virtualna resetuj�ca stan konkretnego regulatora.
	*/
	virtual void resetRegulatora() = 0;

	string nazwa;

	double skok;					// parametry skoku jednostkowego
	double amplitudaP, wypelnienieP;// parametry fali prostok�tnej
	int okresP;
	double amplitudaS;				// parametry sinusoidy
	int okresS;
	double amplitudaT;				// parametry tr�jk�ta
	int okresT;
	double wariancja;				// parametry szumu
	int dlugoscZaklucenia;
	Zaklocenie zakl; 

	double wyMin, wyMax;			// zakres wyj�cia regulatora

	double wartoscZadana;			// warto�� zadana
	double uchyb;					// uchyb regulacji
	double wartoscWyjscia;			// wyjscie obiektu

	int sygnalZadany;				// typ zadanego sygna�u

	int iteracja;					// licznik iteracji
	int maxIter;

	const double PI;				// liczba PI
};

#endif