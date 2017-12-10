/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/

#include "PetlaRegulacji.h"

PetlaRegulacji::PetlaRegulacji()
{
	wyj = 0;
}

PetlaRegulacji::PetlaRegulacji(Objekt *obDyskr, IRegulator *reg)
{
	obiekt = obDyskr;
	regulator = reg;

	wyj = 0;
}

PetlaRegulacji::~PetlaRegulacji()
{
}

double PetlaRegulacji::symuluj()
{
	ster = regulator->symuluj(wyj);
	warZad = regulator->getWartoscZadana();
	wyj = obiekt->symuluj(ster);
	return wyj;
}

void PetlaRegulacji::reset()
{
	obiekt->reset();
	regulator->reset();

	wyj = 0;
}


void PetlaRegulacji::setNastawyRegulatora(vector<double> &param)
{
	regulator->setNastawyRegulatora(param);
}

void PetlaRegulacji::setParametrObiektu(string dana, string wartosc)
{
	obiekt->zmienParametr(dana, wartosc);
}