/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#include "AdaptPID.h"
#include "wyjatki.h"
#include <vector>

const double PI = 3.1415926535;

AdaptPID::AdaptPID(void)
{
}

AdaptPID::AdaptPID(double k, double Td, double Ti, double wartOn)
{   
	regulatorPID = new RegulatorPID(k, Td, Ti, 1, 0, 10, 1);
    przekaznik = new Przekaznik(wartOn);
    strojenie = false;
    
	this->wzmocnienie = k;	// wzmocnienie regulatora
	this->Td = Td;			// czas wyprzedzenia
	this->Ti = Ti;			// czas zdwojenia
    this->wartoscOn = wartOn;
	uchyb = 0;
	czas = 0;
	iloscPrzejscPrzezZero = 0;
	maxReg = 0;
    minReg = 0;

	// poczatkowe, domyslne parametry generatora wartosci zadanej
	skok = 1;
	amplitudaP = 1;
	okresP = 10;
	wypelnienieP = 0.5;
	amplitudaS = 1;
    okresS = 10;
	amplitudaT = 1;
	okresT = 10;
	wariancja = 0.5;
	dlugoscZaklucenia = 100;
	sygnalZadany = 4;
	wyMin = -500;
	wyMax = 500;
}

AdaptPID::~AdaptPID(void)
{
}

void AdaptPID::wlaczStrojenie() {
    strojenie = true;
}

void AdaptPID::setNastawyRegulatora(const vector<double> &nastawy) throw(ReadDataException)
{
	if(nastawy.size() == 4)
	{
		wzmocnienie = nastawy[0];
		Td = nastawy[1];
		Ti = nastawy[2];
		wartoscOn = nastawy[3];
	}
	else
	{
		ReadDataException readEx("Podano nieprawidłowy wektor danych !!!");
		throw readEx;
	}
	
	delete regulatorPID;
	delete przekaznik;

	regulatorPID = new RegulatorPID(wzmocnienie, Td, Ti, 1, 0, 10, 1);
    przekaznik = new Przekaznik(wartoscOn);
}

void AdaptPID::setNastawyRegulatora(int parametr, double nastawa)
{
	if(parametr == K)
		wzmocnienie = nastawa;
	else if(parametr == TD)
		Td = nastawa;
	else if(parametr == TI)
		Ti = nastawa;
	else if(parametr == ON)
		wartoscOn = nastawa;
	else
	{
		ReadDataException readEx("Wybrano nieprawidłowy parametr !!!");
		throw readEx;
	}
	delete regulatorPID;
	delete przekaznik;

	regulatorPID = new RegulatorPID(wzmocnienie, Td, Ti, 1, 0, 10, 1);
    przekaznik = new Przekaznik(wartoscOn);
}

string AdaptPID::getNastawy()
{
	string konf = "pusty";
	return konf;
}
double AdaptPID::liczWyjscie(double warZadana, double wyjZObiektu)
{    
    double sterowanie;
	double popUchyb = uchyb;
	
	if( !isStrojenie() )
	{                        
        sterowanie = regulatorPID->liczWy(warZadana,wyjZObiektu);       // gdy nie ma strojenia
        uchyb = warZadana - wyjZObiektu;                                // sterowanie pochodzi od regulatora PID
    }
	else
	{                          
       if(poczStrojenia){             
         
			trzymanaWartosc = warZadana;	// potrzymywanie wartosci skoku jako wejscie do przekaznika przez caly czas strojenia
			poczStrojenia = false;
		}
		sterowanie = przekaznik->liczWy(trzymanaWartosc, wyjZObiektu);	// gdy jest strojenia
																		// sterowanie pochodzi od przekaznika
		uchyb = trzymanaWartosc - wyjZObiektu;

		if(popUchyb*uchyb < 0 ){           // zmianie ulegl znak uchybu
            
            iloscPrzejscPrzezZero++;
			
		}

		if(iloscPrzejscPrzezZero > 3){           // wykres sie ustabilizuje na stale oscylacje
                                    
            if(wyjZObiektu > maxReg){    
                maxReg = wyjZObiektu;
            }
            if(wyjZObiektu < minReg){
                minReg = wyjZObiektu;
            }
			czas++; 
        }

		if(iloscPrzejscPrzezZero == 12)
		{    
                
			double tKryt = (czas-1) / 4.0;
                                
            double amplituda = (maxReg - minReg)/2.0;
                
            double kKryt = (4.0 * wartoscOn) / (PI*amplituda);
                
            double k = 0.6 * kKryt;
            double Td = 0.12 * tKryt;
            double Ti = 0.5 * tKryt;
            
			this->setNastawyRegulatora(0, k);
			this->setNastawyRegulatora(1, Td);
			this->setNastawyRegulatora(2, Ti);
            strojenie = false;          
			poczStrojenia = true;
			iloscPrzejscPrzezZero = 0;     
            czas = 0;
			maxReg = 0;
	        minReg = 0;
		}
  	
    }

    return sterowanie;
}

void AdaptPID::resetRegulatora()
{
	regulatorPID->reset();
	uchyb = 0;
}


