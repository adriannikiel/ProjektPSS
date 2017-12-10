#include "Identyfikacja.h"
#include "Wyjatki.h"

#include <iostream>
#include <fstream>
#include <numeric>
#include <iomanip>

using namespace std;

Identyfikacja::Identyfikacja(void)
{
}

Identyfikacja::Identyfikacja(int dAA, int dBB, int dCC, int opoz, double wspZapom,double lam)
{
	dA=dAA;  // stopien wielomianu A
	dB=dBB;  // stopien wielomianu B
	dC=dCC;  // stopien wielomianu C
	k=opoz;	 // opoznienie
	alfa=wspZapom;  // wspolczynnik zapominania
	lambda = lam;   // wartosci na przek¹tnych macierzy P 

	vector<double> q;
	for(int i=0;i<dA+dB+dC+1;i++)
		q.push_back(i);

	for(int i=0;i<dA+dB+dC+1;i++)
		P.push_back(q);
    
    for (vector< vector<int> >::size_type u = 0; u < P.size(); u++) 
	{
        for (vector<int>::size_type v = 0; v < P[u].size(); v++)
            if(u==v)
				P[u][v]=lambda;  // warunek poczatkowy dla macierzy P
			else
				P[u][v]=0;
    }

	// ustawienie pocz¹tkowych wektora A
	for(int i=0; i<dA; i++)
		A.push_back(0);
	 	
	// ustawienie pocz¹tkowych wektora B
	for(int i=0; i<=dB; i++)
		B.push_back(0);
	
	// ustawienie pocz¹tkowych wektora C
	for(int i=0;i<dC; i++)
		C.push_back(0);
	
	// ustawienie pocz¹tkowych wejœæ na 0
	for(int i=0; i<dB+k+1; i++)
		uPoprzednie.push_back(0);
		
	// ustawienie pocz¹tkowych wyjœæ na 0
	for(int i=0; i<dA+1; i++)
		yPoprzednie.push_back(0);

		// ustawienie pocz¹tkowych zaklocen na 0
	for(int i=0; i<dC+1; i++)
		ePoprzednie.push_back(0);

	// ustawienie pocz¹tkowe wektora teta
	for(int i=0; i<dB+dA+dC+1; i++)
		teta.push_back(0);
}

void Identyfikacja::liczParam(double wejscie, double wyjscie)
{
	// aktualizacja wektora poprzednich wejœæ
	for(int i=uPoprzednie.size()-1; i>0; i--)
		uPoprzednie[i] = uPoprzednie[i-1];
	
	uPoprzednie[0] = wejscie;

	// aktualizacja wektora poprzednich wyjœæ
	for(int i=yPoprzednie.size()-1; i>0; i--)
		yPoprzednie[i] = yPoprzednie[i-1];
	
	yPoprzednie[0] = wyjscie;

	double e = 0;
	
	e-= inner_product(B.begin(), B.end(), uPoprzednie.begin()+k, 0.0);
	e+= inner_product(A.begin(), A.end(), yPoprzednie.begin()+1, 0.0);
	e-= inner_product(C.begin(), C.end(), ePoprzednie.begin(), 0.0);

	e += yPoprzednie[0];
	
	// aktualizacja wektora poprzednich zaklocen
	for(int i=ePoprzednie.size()-1; i>0; i--)
		ePoprzednie[i] = ePoprzednie[i-1];
	
	ePoprzednie[0] = e; 
	
	vector<double> k1;
	double k2=0;
	vector<vector<double> > k3;
	vector<vector<double> > k4;
	
	// inicjalizacja zerami wektora k
	for(int i=0;i<dA+dB+dC+1;i++)
		k1.push_back(0);
	
	vector<double> q;
	for(int i=0;i<dA+dB+dC+1;i++)
		q.push_back(i);

	for(int i=0;i<dA+dB+dC+1;i++)
	{
		k3.push_back(q);
		k4.push_back(q);
	}
    // inicjalizacja zerami macierzy k3 i k4
    for (vector< vector<int> >::size_type u = 0; u < k3.size(); u++) 
	   for (vector<int>::size_type v = 0; v < k3[u].size(); v++)
	   {
		   k3[u][v]=0;
		   k4[u][v]=0;
	   }
	
	for(int i=0;i<dA+dB+dC+1;i++)
		for(int j=0;j<dA+dB+dC+1;j++)
		{
			if(j<dB+1)
				k1[i]+= P[i][j]*uPoprzednie[j+k];
			
			if(j>=dB+1 && j<dA+dB+1)
				k1[i]-= P[i][j]*yPoprzednie[j-dB];
			
			if(j>=dA+dB+1)
				k1[i]+= P[i][j]*ePoprzednie[j-dA-dB];
			
		}


	for(int j=0;j<dA+dB+dC+1;j++)
	{
		if(j<dB+1)
			k2+= k1[j]*uPoprzednie[j+k];
		if(j>=dB+1 && j<dA+dB+1)
			k2-= k1[j]*yPoprzednie[j-dB];
		if(j>=dA+dB+1)
			k2+= k1[j]*ePoprzednie[j-dA-dB];
	}
	
	k2+=alfa;

	for(int i=0;i<dA+dB+dC+1;i++)
		for(int j=0;j<dA+dB+dC+1;j++)
		{
			k3[i][j]=0;
			k4[i][j]=0;
		}

	for(int i=0;i<dA+dB+dC+1;i++)
		for(int j=0;j<dA+dB+dC+1;j++)
		{
			if(j<dB+1)
				k3[i][j]= k1[i]*uPoprzednie[j+k];
			if(j>=dB+1 && j<dA+dB+1)
				k3[i][j]= -k1[i]*yPoprzednie[j-dB];
			if(j>=dA+dB+1)
				k3[i][j]= k1[i]*ePoprzednie[j-dA-dB];
		}

	for (int i=0;i<dA+dB+dC+1;i++)
        for (int j=0;j<dA+dB+dC+1;j++) {
            k4[i][j] = 0;
			for (int k=0;k<dA+dB+dC+1;k++) 
			    k4[i][j] += k3[i][k] * P[k][j];
			
		}
	
	for (int i=0;i<dA+dB+dC+1;i++)
        for (int j=0;j<dA+dB+dC+1;j++)
			k4[i][j]=k4[i][j]/k2;

	for(int i=0;i<dA+dB+dC+1;i++)
		for(int j=0;j<dA+dB+dC+1;j++)
			P[i][j]=(P[i][j]-k4[i][j])/alfa;
		
	// zabezpieczenie przed wybuchem estymatora
	double maxi,mini,suma=0;
	mini=10;
	maxi=100000;
		
	for(int i=0;i<dA+dB+dC+1;i++)
		for(int j=0;j<dA+dB+dC+1;j++)
			suma+= P[i][j];
		

	if(suma <= mini)
		for(int i=0;i<dA+dB+dC+1;i++)
			for(int j=0;j<dA+dB+dC+1;j++)
				P[i][j]*=10;
			
	else if(suma > maxi)
		for(int i=0;i<dA+dB+dC+1;i++)
			for(int j=0;j<dA+dB+dC+1;j++)
				P[i][j]/=10;
	
	for(int j=0;j<dA+dB+dC+1;j++)
		k1[j]=k1[j]/k2;

	for(int i=0;i<dA+dB+dC+1;i++)
		teta[i]= teta[i] + k1[i]*ePoprzednie[0];

	
	for(int i=0; i<=dB; i++)
		B[i]=teta[i];
	
	for(int i=0; i<dA; i++)
		A[i]=teta[i+dB+1];
		
	for(int i=0;i<dC; i++)
		C[i]=teta[i+dA+dB+1];
}

vector<double> Identyfikacja::getParam() 
{
	return teta;
}

void Identyfikacja::reset()
{
	// zerowanie wektora teta
	for(int i=0;i<dA+dB+dC+1;i++)
		teta[i]=0;

	vector<double> q;
	for(int i=0;i<dA+dB+dC+1;i++)
		q.push_back(i);

	for(int i=0;i<dA+dB+dC+1;i++)
		P.push_back(q);
    
		// ustawienie pocz¹tkowych wektora A
	A.clear();
	for(int i=0; i<dA; i++)
		A.push_back(0);
	 	
	// ustawienie pocz¹tkowych wektora B
	B.clear();
	for(int i=0; i<=dB; i++)
		B.push_back(0);
	
	
	// ustawienie pocz¹tkowych wektora C
	C.clear();
	for(int i=0;i<dC; i++)
		C.push_back(0);
	

	// ustawienie pocz¹tkowych wejœæ na 0
	uPoprzednie.clear();
	for(int i=0; i<dB+k+1; i++)
		uPoprzednie.push_back(0);
		
	// ustawienie pocz¹tkowych wyjœæ na 0
	yPoprzednie.clear();
	for(int i=0; i<dA+1; i++)
		yPoprzednie.push_back(0);

	// resetowanie ustawien macierzy P
    for (vector< vector<int> >::size_type u = 0; u < P.size(); u++) 
	{
        for (vector<int>::size_type v = 0; v < P[u].size(); v++)
            if(u==v)
				P[u][v]=lambda;  // warunek poczatkowy dla macierzy P
			else
				P[u][v]=0;
    }
	
	// usuwanie zawartosci pliku z wektorami teta
	ofstream plikWy; 
	plikWy.open("tety.txt", ios::out);  // historia wspolczynników teta
	
	// gdy utworzenie pliku nie powiod³o siê.
	if(!plikWy)
	{
		OpenException openEx("B³¹d podczas tworzenia pliku z wektorami teta.");
		throw openEx;
		plikWy.clear(plikWy.rdstate() & ~ios::failbit);
		return;
	}
	plikWy.close();
}

void Identyfikacja::zapisDoPliku(string plik)
{
	ofstream plikWy; 
	plikWy.open(plik.c_str(), ios::out|ios::app);
	
	// gdy otworzenie pliku nie powiod³o siê.
	if(!plikWy)
	{
		OpenException openEx("B³¹d podczas otwiercia pliku z wektorami teta.");
		throw openEx;
		plikWy.clear(plikWy.rdstate() & ~ios::failbit);
		return;
	}
	
	for(int i=0;i<dA+dB+dC+1; i++)
		plikWy << setw(15) << teta[i]; 
	plikWy << endl;
	
	plikWy.close();
}
