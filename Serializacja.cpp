#include "Serializacja.h"
#include <sstream>
#include "wyjatki.h"

Serializacja::Serializacja(void)
{
	rozkaz = start;
	dane = 0;
	iloscDanych = sizeof(dane);
	LRC = 0;
}

Serializacja::~Serializacja(void)
{
}

char* Serializacja::Koduj(enum TYP roz, float dana)
{
	rozkaz = roz;
	dane = dana;
	iloscDanych = sizeof(dane);

	string pomoc;
	ostringstream ss;

	ss << START;

	if(rozkaz<100)
		ss << ' ';
	if(rozkaz<10)
		ss << ' '; 
	ss << rozkaz;

	ss << iloscDanych;
	
	ostringstream doDanych;
	doDanych.width(20);
	doDanych << dec << left << dane;

	ss << doDanych.str();
	ss << " ";
	pomoc = ss.str();

	char *ramkaLRC;
	ramkaLRC = new char[ROZMIAR+1];
	
	strcpy(ramkaLRC,pomoc.c_str());
	
	LRC = policzLRC(ramkaLRC,ROZMIAR);
	
	ramkaLRC[25]=LRC;
	
	return ramkaLRC;
}

char* Serializacja::Koduj(enum TYP roz, string dana)
{
	rozkaz = roz;
	nazwa = dana;
	iloscDanych = sizeof(dane);

	string pomoc;
	ostringstream ss;

	ss << START;

	if(rozkaz<100)
		ss << ' ';
	if(rozkaz<10)
		ss << ' '; 
	ss << rozkaz;

	ss << iloscDanych;
	
	ostringstream doDanych;
	doDanych.width(20);
	doDanych << dec << left << nazwa;

	ss << doDanych.str();
	ss << " ";
	pomoc = ss.str();

	char *ramkaLRC;
	ramkaLRC = new char[ROZMIAR+1];
	
	strcpy(ramkaLRC,pomoc.c_str());
	
	LRC = policzLRC(ramkaLRC,ROZMIAR);
	
	ramkaLRC[25]=LRC;
	
	return ramkaLRC;
}

void Serializacja::Dekoduj(char* ramka)
{
	if( (*ramka) !=  this->START)
		throw SocketException("Blad bajtu startu\n");
	
	string doRozkazu;
	doRozkazu += *(ramka+1);
	doRozkazu += *(ramka+2);
	doRozkazu += *(ramka+3); 

	rozkaz = static_cast<TYP> (atoi(doRozkazu.c_str()));

	iloscDanych = *(ramka+4);

	ostringstream probny;
	//probny.width(20);

	for(int i=5;i<25;i++)
		probny << *(ramka+i);
	doRozkazu.clear();
	doRozkazu = probny.str();

	dane = atof(doRozkazu.c_str());
	nazwa = doRozkazu;

	string pomoc;
	ostringstream ss;

	ss << START;

	if(rozkaz<100)
		ss << ' ';
	if(rozkaz<10)
		ss << ' '; 
	ss << rozkaz;
	
	ss << iloscDanych;
	
	ostringstream doDanych;
	doDanych.width(20);

	doDanych << left << doRozkazu;
	
	ss << doDanych.str();
	ss << " ";
	pomoc = ss.str();

	char *ramkaLRC;
	ramkaLRC = new char[ROZMIAR+1];
	
	strcpy(ramkaLRC,pomoc.c_str());
	
	if( policzLRC(ramkaLRC,ROZMIAR) != *(ramka+25))
		throw SocketException("Blad sumy kontolnej\n");
	
	LRC = *(ramka+25);
}


char Serializacja::policzLRC(char *ramkaLRC,int nb)
{
	unsigned char sum = 0;
	
	while(nb--)
	{
		sum += *ramkaLRC++;
	}
	
	return ~sum + 1;
}

float Serializacja::getDane()
{
	return dane;
}

string Serializacja::getNazwa()
{
	return nazwa;
}

TYP Serializacja::getRozkaz()
{
	return rozkaz;
}

int Serializacja::getIloscDanych()
{
	return iloscDanych;
}

char Serializacja::START = '=';

