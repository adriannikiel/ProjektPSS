/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/

#include "zaklocenie.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

using namespace std;

Zaklocenie::Zaklocenie(int dlugosc, double wariancja)
{ 
	const int zakres = 200;
	srand( time(NULL) );

	for(int i=0; i<dlugosc; i++)
		zakl.push_back(rand()%zakres-100);

	// przeskalowanie wartości zakłóceń aby uzyskać wymaganą wariancję
	double skala = sqrt(wariancja/liczWariancje());

	for(int i=0; i<dlugosc; i++)
		zakl[i] *= skala;
}

Zaklocenie::Zaklocenie(int dlugosc, double wariancja, const vector<double> &C, const vector<double> &A)
{
  srand ( time(NULL) );
  double wartosc;
  
  for(int i=0; i<dlugosc; i++)
  {
      wartosc=(static_cast<double>( rand() ) / (static_cast<double> (RAND_MAX) + 1.0)-0.5)*2*sqrt(3.0);
      zakl.push_back(wartosc);
  }
	
  vector<double> wartoscToru;
    
  for(int i=0; i<dlugosc; i++)
  {
  	wartosc = 0;

    for(int j=1; j<A.size(); j++)             
    {
      if(i-j>=0)
      wartosc -= A[j]*zakl[i-j];
    }    
                
    for(int j=0; j<C.size(); j++) 
    {    
      if(i-j>=0)    
      wartosc += C[j]*zakl[i-j];  
    }     
    
    wartoscToru.push_back(wartosc);
  }
 	
 	double suma = 0;
	for(int i=0; i<wartoscToru.size(); i++)
		suma += wartoscToru[i];

	// liczenie średniej
	double srednia = suma/wartoscToru.size();

	// liczenie wariancji toru zakłócenia
	double wariancjaToru = 0;
	
	for(int i=0; i<wartoscToru.size(); i++)
		wariancjaToru += (wartoscToru[i]-srednia) * (wartoscToru[i]-srednia);

	wariancjaToru = wariancjaToru/wartoscToru.size();

	double skala = sqrt(wariancja/wariancjaToru);

	//przeskalowanie wartości zakłócenia
  for(int i=0; i<dlugosc; i++)
  	zakl[i] *= skala;
}

double Zaklocenie::liczWariancje()
{
	// liczenie sumy wszystkich wartości zakłócenia
	double suma = 0;
	for(int i=0; i<zakl.size(); i++)
		suma += zakl[i];

	// liczenie średniej
	double srednia = static_cast<double> (suma)/zakl.size();

	// liczenie wariancji zakłócenia
	double war = 0;
	for(int i=0; i<zakl.size(); i++)
		war += (zakl[i]-srednia) * (zakl[i]-srednia);

	war = war/zakl.size();

	return war;
}