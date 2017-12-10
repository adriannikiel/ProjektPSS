/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#include "Przekaznik.h"
#include "wyjatki.h"
#include <sstream>
#include <vector>


Przekaznik::Przekaznik(void): IRegulator()
{
}

Przekaznik::Przekaznik(double ON, double hist): IRegulator()
{
	wartON = ON;
    wartOFF = -ON;
    histereza = hist;
	uchyb = 0;
}

Przekaznik::Przekaznik(double ON, double OFF, double hist) {
   wartON = ON;
   wartOFF = OFF;
   histereza = hist;
   uchyb = 0;
}

Przekaznik::~Przekaznik(void)
{
}

void Przekaznik::setNastawyRegulatora(const vector<double> &nastawy) throw(ReadDataException)
{
	if(nastawy.size() == 3)
	{
		wartON = nastawy[0];		
		wartOFF = nastawy[1];		
		histereza = nastawy[2];
	}
	else
	{
		ReadDataException readEx("Podano nieprawid³owy wektor danych !!!");
		throw readEx;
	}
}

void Przekaznik::setNastawyRegulatora(int parametr, double nastawa)
{
	if(parametr == pwartON)
		wartON = nastawa;
	else if(parametr == pwartOFF)
		wartOFF = nastawa;
	else if(parametr == phistereza)
		histereza = nastawa;
	else
	{
		ReadDataException readEx("Wybrano nieprawid³owy parametr !!!");
		throw readEx;
	}
}

string Przekaznik::getNastawy()
{
	stringstream ss;
	string str;
	string konf;

	konf.append("ON= ");
	ss << wartON;
	ss >> str;
	konf.append(str);

	konf.append("\nOFF= ");
	ss << wartOFF;
	ss >> str;
	konf.append(str);

	konf.append("\nhistereza= ");
	ss << histereza;
	ss >> str;
	konf.append(str);

	return konf;
}

double Przekaznik::liczWy(double warZadana, double wyjZObiektu)
{
	return liczWyjscie(warZadana, wyjZObiektu);
}


double Przekaznik::liczWyjscie(double warZadana, double wyjZObiektu)
{    
    double sterowanie;
	double popUchyb = uchyb;
    double uchyb = warZadana - wyjZObiektu;
    
    if( (uchyb > histereza) || ( ( (uchyb > -histereza) && (uchyb-popUchyb < 0) ) ) )
	{
        sterowanie = wartON;
    }
	else if( (uchyb < -histereza) || ( (uchyb < histereza) && (uchyb-popUchyb > 0) ) )
	{
        sterowanie = wartOFF;
    }
	else
	{
        sterowanie = 0.0;
    }
    return sterowanie;
}

void Przekaznik::resetRegulatora()
{
	uchyb = 0;
}

