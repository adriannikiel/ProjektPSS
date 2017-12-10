/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/

#include "IRegulator.h"
#include "zaklocenie.h"
#include "wyjatki.h"
#include <math.h>
#include <time.h>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

IRegulator::IRegulator(void):PI(3.1415926535897932384626433832795)
{
	// inicjalizacja licznika iteracji
	iteracja = 0;

	// inicjalizacja sygna³ów wartoœci zadanej
	sygnalZadany = 0;

	skok = 1;

	amplitudaP = 1;
	wypelnienieP = 0.5;
	okresP = 10;

	amplitudaS = 1;
	okresS = 10;

	amplitudaT = 1;
	okresT = 10;

	wariancja = 0.5;
	dlugoscZaklucenia = 100;
	zakl = Zaklocenie(dlugoscZaklucenia, wariancja);

	wyMin = -100;
	wyMax = 100;

	maxIter = okresP * okresS * okresT;
}

IRegulator::~IRegulator(void)
{
}

double IRegulator::symuluj(double sygnal)
{
	double wymuszenie = 0;

	if(sygnalZadany & SKOK)
	{
		wymuszenie += skok;
	}

	if(sygnalZadany & KRONEKER)
	{
		if( iteracja == 0)
			wymuszenie += 1;
	}

	if(sygnalZadany & PROSTOKAT)
	{
		int faza = iteracja%okresP;

		if(faza <= wypelnienieP*okresP)
			wymuszenie += amplitudaP;
	}

	if(sygnalZadany & SINUS)
	{
		int faza = iteracja%okresS;
		
		wymuszenie += amplitudaS * sin(2*PI*faza/okresS);
	}

	if(sygnalZadany & TROJKAT)
	{
		double wartosc;
		int faza = iteracja%okresT;

		if(faza < 0.25*okresT)
			wartosc = (4.0*amplitudaT/okresT)*faza;
		else if (faza >= 0.25*okresT && faza < 0.75*okresT)
			wartosc = 2*amplitudaT - (4.0*amplitudaT/okresT)*faza;
		else if (faza >= 0.75*okresT)
			wartosc = -4*amplitudaT + (4.0*amplitudaT/okresT)*faza;

		wymuszenie += wartosc;
	}

	if(sygnalZadany & SZUM)
	{
		wymuszenie += zakl.getWartosc(iteracja%dlugoscZaklucenia);
	}

	iteracja++;
	if(iteracja >= maxIter)
		iteracja = 0;

	wartoscZadana = wymuszenie;
	uchyb = wymuszenie - sygnal;

	wartoscWyjscia = liczWyjscie(wymuszenie, sygnal);

	if(wartoscWyjscia > wyMax)
		wartoscWyjscia = wyMax;

	if(wartoscWyjscia < wyMin)
		wartoscWyjscia = wyMin;

	return wartoscWyjscia;
}

void IRegulator::symuluj(string we, string wy)
{
}

void IRegulator::setSygnal(int typSygnalu)
{
	iteracja = 0;
	sygnalZadany = typSygnalu;
}

int IRegulator::getSygnal()
{
	return sygnalZadany;
}

void IRegulator::setSkok(double wartoscSkoku)
{
	skok = wartoscSkoku;
}

void IRegulator::setProstokat(double amplituda, int okres, double wypelnienie)
{
	amplitudaP = amplituda;
	okresP = okres;
	wypelnienieP = wypelnienie;

	maxIter = okresP * okresS * okresT;
}

void IRegulator::setSinus(double amplituda, int okres)
{
	amplitudaS = amplituda;
	okresS = okres;

	maxIter = okresP * okresS * okresT;
}

void IRegulator::setTrojkat(double amplituda, int okres)
{
	amplitudaT = amplituda;
	okresT = okres;

	maxIter = okresP * okresS * okresT;
}

void IRegulator::setSzum(double war, int dlZakl)
{
	wariancja = war;
	dlugoscZaklucenia = dlZakl;
	zakl = Zaklocenie(dlugoscZaklucenia, wariancja);
}

void IRegulator::setWyMax(double maxWy)
{
	wyMax = maxWy;
}

void IRegulator::setWyMin(double minWy)
{
	wyMin = minWy;
}

string IRegulator::getKonfiguracja()
{
	stringstream ss;
	string str;
	string konf;

	konf.append("SKOK= ");
	ss << skok << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nAMPLITUDA_PROSTOKATA= ");
	ss << amplitudaP << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nOKRES_PROSTOKATA= ");
	ss << okresP << endl;
	ss >> str;
	konf.append(str);


	konf.append("\nWYPELNIENIE_PROSTOKATA= ");
	ss << wypelnienieP << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nAMPLITUDA_SINUSA= ");
	ss << amplitudaS << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nOKRES_SINUSA= ");
	ss << okresS << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nAMPLITUDA_TROJKATA= ");
	ss << amplitudaT << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nOKRES_TROJKATA= ");
	ss << okresT << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nWARIANCJA_SZUMU= ");
	ss << wariancja << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nDLUGOSC_ZAKLOCENIA= ");
	ss << dlugoscZaklucenia << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nSYGNAL= ");
	ss << sygnalZadany << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nWYJSCIE_MIN= ");
	ss << wyMin << endl;
	ss >> str;
	konf.append(str);

	konf.append("\nWYJSCIE_MAX= ");
	ss << wyMax << endl;
	ss >> str;
	konf.append(str);

	return konf;
}

void IRegulator::zapisNastawRegulatora(string plik, string nazwa) throw(OpenException)
{
	ofstream strum;
	strum.open(plik.c_str(), ios::out|ios::app);

	if(!strum)
	{
		strum.clear(strum.rdstate() & ~ios::failbit);
		OpenException openEx("B³¹d otwarcia pliku konfiguracyjnego: regulatoryP.conf");
		throw openEx;
	}
	
	strum << endl << endl;
	strum << "MODEL= " << nazwa.c_str() << endl;
	strum << getNastawy() << endl;
	strum << getKonfiguracja().c_str() << endl;
	strum << endl;
	strum << "#####################################"
		  << "####################################";

	strum.close();
}