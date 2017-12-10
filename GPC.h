/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#ifndef GPC_H
#define GPC_H

#include "IRegulator.h"
#include "wyjatki.h"
#include "Identyfikacja.h"
#include <vector>

/**
	Klasa regulatora predykcyjnego GPC.
	@author Adrian Nikiel
*/
class GPC: public IRegulator
{
public:
	/**
		Konstruktor domniemany klasy GPC.
	*/
	GPC(void);

	/**
		Konstruktor klasy GPC.
		@param sciezka œcie¿ka do pliku konfiguracyjnego
		@param obiekt nazwa obiektu
		@param lambda parametr lambda identyfikacji
		@throws OpenException
		@throws ReadDataException
	*/
	GPC(string sciezka, string obiekt, double lambda);

	/**
		Konstruktor klasy GPC.
		@param H horyzont predykcji
		@param L horyzont sterowania
		@param ro waga dla sterowania
		@param alfa parametr trajektorii odniesienia
		@param dA stopien identyfikowanego wielomianu A
		@param dB stopien identyfikowanego wielomianu B
	*/
	GPC(int H, int L, double ro, double alfa, int dA, int dB);

	/**
		Destruktor klasy GPC.
	*/
	virtual ~GPC(void);

	/**
		Funkcja umo¿liwiaj¹ca zmianê parametrów regulatora.
		Elementy wektora: {H,L,ro,alfa,dA,dB}
		@param nastawy nastawy regulatora
		@throws ReadDataException
	*/
	void setNastawyRegulatora(const vector<double> &nastawy);

	/**
		Funkcja umo¿liwiaj¹ca zmianê jednego parametru regulatora.
		@param parametr parametr regulatora
		@param nastawa nastawa regulatora
	*/
	void setNastawyRegulatora(int parametr, double nastawa);

/**
		Funkcja umo¿liwiaj¹ca zmianê parametrów dA, dB i alfa identyfikacji
		@param dA stopien wielomianu A modelu
		@param dB stopien wielomianu B modelu
		@param aalfaa parametr alfa identyfikacji
	*/
	void setParamABK(int dA, int dB, double aalfaa);


	/**
		Funkcja zwracaj¹ca wartoœæ horyzontu predykcji.
		@return wartoœæ horyzontu predykcji H
	*/
	double getH()
	{
		return H;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ horyzontu sterowania.
		@return wartoœæ horyzontu sterowania L
	*/
	double getL()
	{
		return L;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ wagi dla sterownia
		@return wartoœæ wagi dla sterownia ro
	*/
	double getRo()
	{
		return ro;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ parametru trajektorii odniesienia
		@return wartoœæ parametru trajektorii odniesienia alfa
	*/
	double getAlfa()
	{
		return alfa;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ stopnia wielomianu A identyfiacji
		@return wartoœæ stopnia wielomianu A identyfikacji
	*/
	double getDA()
	{
		return dA;
	}

	/**
		Funkcja zwracaj¹ca wartoœæ stopnia wielomianu B identyfiacji
		@return wartoœæ stopnia wielomianu B identyfikacji
	*/
	double getDB()
	{
		return dB;
	}

	/**
		Funkcja zwracaj¹ca nastawy konkretnego regulatora.
		@return nastawy regulatora
	*/
	string getNastawy();

	static const int pH = 0;
	static const int pL = 1;
	static const int pro = 2;
	static const int palfa = 3;
	static const int pdA = 4;
	static const int pdB = 5;

	Identyfikacja *identyf;   //obiekt klasy Identyfikacja
	
private:
	/**
		Funkcja wyznaczaj¹ca wartoœæ wyjœcia na podstawie wymuszenia
		i sygna³u z obiektu.
		@param warZadana wartoœæ zadana
		@param wyjZObiektu wartoœæ wyjœæia z obiektu regulowanego
	*/
	double liczWyjscie(double warZadana, double wyjZObiektu);

	
	/**
		Funkcja wczytuje dane o podanej nazwie z danej lini.
		Funkcja zwraca odpowiednio true lub false gdzy dane zostan¹ wczytane lub nie.
		Na przyk³ad:  wczytaj("H=","H= 3").
		
		@param dana nazwa danej do wczytania
		@param slinia string zawieraj¹cy liniê któr¹ nale¿y przeszukaæ w celu wczytania danych
		@return zwraca true gdy znaleziono szukane dane lub false gdy tych danych nie znaleziono 
		@throws ReadDataException
	*/
	bool wczytaj(string dana, string slinia);

	/**
		Funkcaja resetuj¹ca stan konkretnego regulatora.
	*/
	void resetRegulatora();

	int H;				// horyzont predykcji
	int L;				// horyzont sterowania
	double ro;			// waga dla sterowania
	double alfa;		// parametr trajektorii odniesienia
	int dA,dB;			// stopnie wielomianow A i B
	
	double wartoscRegulowana;
    double wartoscZadana;
	
   	vector<double> wekQ;	//wektor q
    vector<double> w0;		//wektor trajektorii odniesienia w0
    vector<double> odpSwob;	//wektor odpowiedzi swobodnej y0
    
	vector<double> A;		//wektor wspolczynnikow obiektu A
    vector<double> B;	//wektor wspolczynnikow obiektu B
    	 
    ///Funkcja obliczajaca wektor q
    void obliczQ();
    
    ///Funkcja wyznaczajaca trajektorie odniesienia
    void obliczW0();

    ///Funkcja wyznaczajaca odpowiedz swobodna
    void obliczOdpSwob();

	/**
		Funkcja wyznaczaj¹ca wektor X
		@param ro waga dla sterowania
		@param QQ wynik mnozenia macierzy Q'Q
		@return wektora X
	*/
	vector<double> obliczX(double ro,vector<vector<double> >  QQ);

	vector<double> uuPoprzednie;  // wektor poprzednich sterowan
	vector<double> yyPoprzednie;  // wektor poprzednich wyjsc
	double popSterowanie;		  // poprzednia wartosc sterowania	
								   
};

#endif