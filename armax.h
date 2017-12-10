/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/
#ifndef ARMAX_H
#define ARMAX_H

#include "objekt.h"
#include <vector>

/**
	Klasa modelu ARMAX.
	@author Adrian Nikiel
*/
class armax : public Objekt
{

public:

	/**
		Konstruktor klasy armax.
		@param ob Nazwa obiektu (obiekt o podanej nazwie
							powinien znajdować się w pliku konfiguracyjnym.
		@throws OpenException
		@throws ReadDataException
	*/
	armax(string ob);

	/**
		Destruktor domniemany klasy armax.
	*/
	~armax(){};

	/**
		Funkcja służąca do symulacji obiektu na podstawie 
		pojedyńczej próbki.
		@param wejscie próbka podawana na wejście obiektu
		@return wartość wyjścia z obiektu na podaną wartość wejściową
	*/
	double symuluj(double wejscie);
	
	/**
		Funkcja służąca do symulacji obiektu na podstawie 
		zestawu próbek wczytanego z pliku o podanej ścieżce.
		@param we ścieżka do pliku z zestawem próbek do symulacji
		@param wy ścieżka do pliku z gdzie zapisane będą wyniki symulacji
	*/
	void symuluj(string we, string wy);

	/**
		Funkcja służy do zrestartowania symulacji krokowej.
	*/
	void reset();

	/**
		Funkcja służy do zmiany prarametrów modelu armax.
		@param dana nazwa danej do zmiany
		@param wartosc string zawierający linię którą należy przeszukać w celu wczytania danych
		@return zwraca true gdy znaleziono szukane dane lub false gdy tych danych nie znaleziono
	*/
	bool zmienParametr(string dana, string wartosc);
	
	/**
		Funkcja zwracająca wartośći współczynników wielomianu A1.
		@return wartośći współczynników wielomianu A1
	*/
	vector<double> getA1()
	{
		return A1;
	}
	
	/**
		Funkcja zwracająca wartośći współczynników wielomianu B1.
		@return wartośći współczynników wielomianu B1
	*/
	vector<double> getB1()
	{
		return B1;
	}
	
	/**
		Funkcja zwracająca wartośći współczynników wielomianu C1.
		@return wartośći współczynników wielomianu C1
	*/
	vector<double> getC1()
	{
		return C1;
	}
	
	/**
		Funkcja zwracająca wartośći współczynników wielomianu A2.
		@return wartośći współczynników wielomianu A2
	*/
	vector<double> getA2()
	{
		return A2;
	}
	
	/**
		Funkcja zwracająca wartośći współczynników wielomianu B2.
		@return wartośći współczynników wielomianu B2
	*/
	vector<double> getB2()
	{
		return B2;
	}
	
	/**
		Funkcja zwracająca wartośći współczynników wielomianu C2.
		@return wartośći współczynników wielomianu C2
	*/
	vector<double> getC2()
	{
		return C2;
	}
	
	/**
		Funkcja zwracająca wartośći opóźnienia d.
		@return wartośći opóźnienia d
	*/
	int getD()
	{
		return d;
	}
	
	/**
		Funkcja zwracająca wartośći kroku od którego rozpoczyna się niestacjonarność.
		@return wartośći od którego rozpoczyna się niestacjonarność
	*/
	int getI1()
	{
		return I1;
	}
	
	/**
		Funkcja zwracająca wartośći ktoku na którym kończy się niestacjonarność.
		@return wartośći ktoku na którym kończy się niestacjonarność
	*/
	int getI2()
	{
		return I2;
	}

	/**
		Funkcja zwracająca wartość maksymalną na wyjściu obiektu.
		@return wartość maksymalna na wyjściu obiektu
	*/
	double getWyMax()
	{
		return wyMax;
	}

	/**
		Funkcja zwracająca wartość maksymalną na wyjściu obiektu.
		@return wartość maksymalna na wyjściu obiektu
	*/
	double getWyMin()
	{
		return wyMin;
	}

	/**
		Funkcja zwracająca wartość wariancji zakłócenia.
		@return wartość wariancji zakłócenia
	*/
	double getWariancja()
	{
		return wariancja;
	}

	/**
		Funkcja zwracająca ilość próbek zakłócenia.
		@return ilość próbek zakłócenia
	*/
	int getDlZakl()
	{
		return dlZakl;
	}

	/**
		Funkcja zwracająca stopień wielomianu A.
		@return stopień wielomianu A
	*/
	int get_dA()
	{
		return A1.size()-1;
	}

	/**
		Funkcja zwracająca stopień wielomianu B.
		@return stopień wielomianu B
	*/
	int get_dB()
	{
		return B1.size()-1;
	}

	/**
		Funkcja zwracająca stopień wielomianu C.
		@return stopień wielomianu C
	*/
	int get_dC()
	{
		return C1.size()-1;
	}

private:
	
	/**
		Funkcja wylicza wartość wyjścia w danym kraku dla
		podanych parametrów modelu ARMAX.
		@param A wektor parametrów A modelu ARMAX
		@param B wektor parametrów B modelu ARMAX
		@param C wektor parametrów C modelu ARMAX
		@return wartość wyjścia dla jednego kroku
	*/
	double liczWyjscie(vector<double>& A, vector<double>& B, vector<double>& C);
	
	/**
		Funkcja wczytuje dane o podanej nazwie z danej lini. Funkcja zwraca odpowiednio true lub false gdzy dane zostaną wczytane lub nie.
		Na przykład:  wczytaj("A1=","A1= 1 2 3").
		
		@param dana nazwa danej do wczytania
		@param slinia string zawierający linię którą należy przeszukać w celu wczytania danych
		@return zwraca true gdy znaleziono szukane dane lub false gdy tych danych nie znaleziono 
	*/
	bool wczytaj(string dana, string slinia);
	
	vector<double> A1, B1, C1, A2, B2, C2, yPoprzednie, uPoprzednie;
	double wyMax, wyMin, wariancja;
	int d, I1, I2, dlZakl, indexZakl, licznikIteracji;
};

#endif
