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
	Interfejs regulatora. Mo¿na go wyko¿ystaæ do implementacji dowolnego regulatora.
	Nale¿y zaimplementowaæ nastêpuj¹ce funkne: <br>
	void setNastawyRegulatora(vector<double> &nastawy) <br>
	void setNastawyRegulatora(string plik) <br>
	virtual double liczWyjscie(double wejscieReg) <br>
*/
class IRegulator: public SISO
{

public:
	/**
		Funkcja czysto wirtualna umo¿liwiaj¹ca zmianê parametrów regulatora.
		@param nastawy nastawy regulatora
	*/
	virtual void setNastawyRegulatora(const vector<double> &nastawy)=0;

	/**
		Funkcja czysto wirtualna umo¿liwiaj¹ca zmianê jednego parametru regulatora.
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
		Funkcja zwracaj¹ca nazwê regulatora.
		@return nazwa regulaotra
	*/
	string getNazwa()
	{
		return nazwa;
	}

	/**
		Funkcja wyznaczaj¹ca sterowanie na podstawie sygna³u z obiektu.
		@param sygnal sygna³ z obiektu
	*/
	double symuluj(double sygnal);

	/**
		Funkcja wyznaczaj¹ca sterowanie na podstawie sygna³u z obiektu.
		@param we plik z wejœciami do regulatora
		@param wy plik z wyjœciami z regulatora
	*/
	void symuluj(string we, string wy);

	/**
		Funkcja zwracaj¹ca wartoœæ uchybu regulacji.
		@return wartoœæ uchybu regulacji
	*/
	double getUchyb()
	{
		return uchyb;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ wyjœcia obiektu.
		@return wartoœæ wyjœcia obiektu
	*/
	double getWartoscWyjscia()
	{
		return wartoscWyjscia;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ zadan¹.
		@return wartoœæ zadana
	*/
	double getWartoscZadana()
	{
		return wartoscZadana;
	}

	/**
		Funkcja umo¿liwiaj¹ca zmianê sygna³u wartoœci zadanej.
		Mo¿liwe sygna³y do wykozystania: <br>
		 - skok jednostkowy (IRegulator::SKOK) <br>
		 - impuls Kronekera (IRegulator::KRONEKER) <br>
		 - fala prostok¹tna (IRegulator::PROSTOKAT) <br>
		 - sinus (IRegulator::SINUS) <br>
		 - fala trójk¹tna (IRegulator::TROJKAT) <br>
		 - szum (IRegulator::SZUM) <br>
		Mo¿liwe jest równie¿ ³¹czenie sygna³ów, np. IRegulator::SKOK | IRegulator::SINUS.
		@param typSygnalu sygna³ lub z³o¿enie sygna³ów wartoœci zadanej
	*/
	void setSygnal(int typSygnalu);

	/**
		Funkcja umo¿liwiaj¹ca odczyt sygna³u wartoœci zadanej.
		Mo¿liwe sygna³y: <br>
		 - skok jednostkowy (IRegulator::SKOK) <br>
		 - impuls Kronekera (IRegulator::KRONEKER) <br>
		 - fala prostok¹tna (IRegulator::PROSTOKAT) <br>
		 - sinus (IRegulator::SINUS) <br>
		 - fala trójk¹tna (IRegulator::TROJKAT) <br>
		 - szum (IRegulator::SZUM) <br>
		Mo¿liwe jest równie¿ ³¹czenie sygna³ów, np. IRegulator::SKOK | IRegulator::SINUS.
		@return sygna³ lub z³o¿enie sygna³ów wartoœci zadanej
	*/
	int getSygnal();

	// Mo¿liwe do wyboru typy sygna³ów
	static const int SKOK = 1;
	static const int KRONEKER = 1<<1;
	static const int PROSTOKAT = 1<<2;
	static const int SINUS = 1<<3;
	static const int TROJKAT = 1<<4;
	static const int SZUM = 1<<5;

	/**
		Funkcja umo¿liwiaj¹ca zmianê parametrow skoku jednostkowego.
		@param wartoscSkoku wartosc skoku
	*/
	void setSkok(double wartoscSkoku);

	/**
		Funkcja zwracaj¹ca wartoœæ skoku jednostkowego.
		@return wartoœæ skoku jednostkowego
	*/
	double getSkok()
	{
		return skok;
	}

	/**
		Funkcja umo¿liwiaj¹ca zmianê parametrow fali prostokatnej.
		@param amplituda amplituda fali prostokatnej
		@param okres okres fali prostokatnej
		@param wypelnienie wypelnienie fali prostokatnej
	*/
	void setProstokat(double amplituda, int okres, double wypelnienie);

	/**
		Funkcja zwracaj¹ca wartoœæ amplitudy fali prostok¹tnej.
		@return wartoœæ amplitudy fali prostok¹tnej
	*/
	double getProstokatAmplituda()
	{
		return amplitudaP;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ okresu fali prostok¹tnej.
		@return wartoœæ okresu fali prostok¹tnej
	*/
	int getProstokatOkres()
	{
		return okresP;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ wype³nienia fali prostok¹tnej.
		@return wartoœæ wype³nienia fali prostok¹tnej
	*/
	double getProstokatWypelnienie()
	{
		return wypelnienieP;
	}

	/**
		Funkcja umo¿liwiaj¹ca zmianê parametrow przebiegu sinusoidalnego.
		@param amplituda amplituda sinusoidy
		@param okres okres sinusoidy
	*/
	void setSinus(double amplituda, int okres);

	/**
		Funkcja zwracaj¹ca wartoœæ amplitudy przebiegu sinusoidalnego.
		@return wartoœæ amplitudy przebiegu sinusoidalnego
	*/
	double getSinusAmplituda()
	{
		return amplitudaS;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ okresu przebiegu sinusoidalnego.
		@return wartoœæ okresu przebiegu sinusoidalnego
	*/
	int getSinusOkres()
	{
		return okresS;
	}

	/**
		Funkcja umo¿liwiaj¹ca zmianê parametrow przebiegu trójk¹tnego.
		@param amplituda amplituda przebiegu trójk¹tnego
		@param okres okres przebiegu trójk¹tnego
	*/
	void setTrojkat(double amplituda, int okres);

	/**
		Funkcja zwracaj¹ca wartoœæ amplitudy przebiegu trójk¹tnego.
		@return wartoœæ amplitudy przebiegu trójk¹tnego
	*/
	double getTrojkatAmplituda()
	{
		return amplitudaT;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ okresu przebiegu trójk¹tnego.
		@return wartoœæ okresu przebiegu trójk¹tnego
	*/
	int getTrojkatOkres()
	{
		return okresT;
	}

	/**
		Funkcja umo¿liwiaj¹ca zmianê parametrow szumu.
		@param war wariancja szumu
		@param dlZakl iloœæ próbek szumu
	*/
	void setSzum(double war, int dlZakl);

	/**
		Funkcja zwracaj¹ca wartoœæ wariancji szumu.
		@return wartoœæ wariancji szumu
	*/
	double getSzumWariancja()
	{
		return wariancja;
	}

	/**
		Funkcja zwracaj¹ca iloœæ próbek szumu.
		@return iloœæ próbek szumu
	*/
	int getSzumDlZakl()
	{
		return dlugoscZaklucenia;
	}

	/**
		Funkcja umo¿liwiaj¹ca zmianê maksymalnej wartosci wyjœcia z regulatora.
		@param maxWy wariancja szumu
	*/
	void setWyMax(double maxWy);

	/**
		Funkcja zwracaj¹ca wartoœæ maksymaln¹ na wyjœciu regulatora.
		@return wartoœæ maksymalna na wyjœciu regulatora
	*/
	double getWyMax()
	{
		return wyMax;
	}

	/**
		Funkcja umo¿liwiaj¹ca zmianê minimalnej wartosci wyjœcia z regulatora.
		@param minWy wariancja szumu
	*/
	void setWyMin(double minWy);

	/**
		Funkcja zwracaj¹ca wartoœæ minimaln¹ na wyjœciu regulatora.
		@return wartoœæ minimalna na wyjœciu regulatora
	*/
	double getWyMin()
	{
		return wyMin;
	}

	/**
		Funkcja umo¿liwiaj¹ca zapis nastaw regulatora do pliku.
		@param plik œcie¿ka do pliku konfiguracyjnego
		@param nazwa nazwa regulatora
		@throws OpenException
	*/
	void zapisNastawRegulatora(string plik, string nazwa);

	/**
		Funkcaja resetuj¹ca stan regulatora.
	*/
	void reset()
	{
		iteracja = 0;
		resetRegulatora();
	}

protected:
	/**
		Funkcja czysto wirtualna wyznaczaj¹ca wartoœæ wyjœcia na podstawie wymuszenia
		i sygna³u z obiektu.
		@param warZadana wartoœæ zadana
		@param wyjZObiektu wartoœæ wyjœæia z obiektu regulowanego
	*/
	virtual double liczWyjscie(double warZadana, double wyjZObiektu)=0;

	/**
		Funkcja czysto wirtualna zwracaj¹ca nastawy konkretnego regulatora.
		@return nastawy regulatora
	*/
	virtual string getNastawy()=0;

	/**
		Funkcja zwracaj¹ca konfiguracjê zawart¹ w interfejsie IRegulator.
		@return konfiguracja regulatora
	*/
	string getKonfiguracja();
	
	/**
		Funkcaja virtualna resetuj¹ca stan konkretnego regulatora.
	*/
	virtual void resetRegulatora() = 0;

	string nazwa;

	double skok;					// parametry skoku jednostkowego
	double amplitudaP, wypelnienieP;// parametry fali prostok¹tnej
	int okresP;
	double amplitudaS;				// parametry sinusoidy
	int okresS;
	double amplitudaT;				// parametry trójk¹ta
	int okresT;
	double wariancja;				// parametry szumu
	int dlugoscZaklucenia;
	Zaklocenie zakl; 

	double wyMin, wyMax;			// zakres wyjœcia regulatora

	double wartoscZadana;			// wartoœæ zadana
	double uchyb;					// uchyb regulacji
	double wartoscWyjscia;			// wyjscie obiektu

	int sygnalZadany;				// typ zadanego sygna³u

	int iteracja;					// licznik iteracji
	int maxIter;

	const double PI;				// liczba PI
};

#endif