/***************************************************************************
 *   Copyright (C) 2009 by Adrian Nikiel   *
 ***************************************************************************/

#include <string>

using namespace std;

/**
	Typ wyliczeniowy dla mozliwych danych wysylanych przez TCP/IP
*/
enum TYP{	 typRegulatora=1, start, stop, reset, czasKrokuTrackBar, czasKrokuNumericUpDown,
			 nazwaRegP, k, wyMaxP, wyMinP,																// regulator P
			 skokP, kronekerP, prostokatP, sinusP, trojkatP, szumP, 
			 nowySkokP, nowaAmplitudaPP, nowyOkresPP, noweWypelnieniePP, nowaAmplitudaSP, nowyOkresSP,  
			 nowaAmplitudaTP, nowyOkresTP, nowaWariancjaP, nowaLiczbaProbekP, 
			 nazwaRegPID, kPID, tiPID, tdPID, tpPID, wpPID, nPID, bPID, wyMaxPID, wyMinPID,				// regulator PID
			 skokPID, kronekerPID, prostokatPID, sinusPID, trojkatPID, szumPID,
			 nowySkokPID, nowaAmplitudaPPID, nowyOkresPPID, noweWypelnieniePPID, nowaAmplitudaSPID, nowyOkresSPID,
			 nowaAmplitudaTPID, nowyOkresTPID, nowaWariancjaPID, nowaLiczbaProbekPID,
			 nazwaRegGPC, HGPC, LGPC, roGPC, alfaGPC, dAGPC, dBGPC, wyMaxGPC, wyMinGPC,					// regulator GPC
			 skokGPC, kronekerGPC, prostokatGPC, sinusGPC, trojkatGPC, szumGPC, 
			 nowySkokGPC, nowaAmplitudaPGPC, nowyOkresPGPC, noweWypelnieniePGPC, nowaAmplitudaSGPC, nowyOkresSGPC,
			 nowaAmplitudaTGPC, nowyOkresTGPC, nowaWariancjaGPC, nowaLiczbaProbekGPC, nowaAlfaGPC,
			 ZmienNastawy, kAPID, tiAPID, tdAPID, onAPID, wyMaxAPID, wyMinAPID,							// regulator APID
			 skokAPID, kronekerAPID, prostokatAPID, sinusAPID, trojkatAPID, szumAPID, 
			 nowySkokAPID, nowaAmplitudaPAPID, nowyOkresPAPID, noweWypelnieniePAPID, nowaAmplitudaSAPID, nowyOkresSAPID,
			 nowaAmplitudaTAPID, nowyOkresTAPID, nowaWariancjaAPID, nowaLiczbaProbekAPID,
			 wartoscZadana, wyjscieObiektu, sterowanie,													// wykres
			 typDostepu, nazwaUzytkownika, hasloUzytkownika											
};

/**
	Klasa do serializacji danych w postaci ramki.
	@author Adrian Nikiel
*/
class Serializacja
{
public:
	/**
		Konstruktor domniemany klasy Serializacja.
	*/
	Serializacja(void);

	/**
		Destruktor klasy Serializacja.
	*/
	~Serializacja(void);

	/**
		Funkcja tworz�ca ramk� do wysy�ania danych.
		   Postac ramki:
		     START|ROZKAZ|ILOSC DANYCH|DANE|LRC
		
		   START			- 1 bajt
		   ROZKAZ			- 3 bajty
		   ILOSC DANYCH		- 1 bajt
		   DANE				- 20 bajt�w
		   LRC				- 1 bajt

		@param roz typ rozkazu
		@param dana dane w ramce w postaci liczby
		@return wskaznik do stworzonej ramki
	*/
	char* Koduj(enum TYP roz, float dana);

	/**
		Funkcja tworz�ca ramk� do wysy�ania danych.
		   Postac ramki:
		     START|ROZKAZ|ILOSC DANYCH|DANE|LRC
		
		   START			- 1 bajt
		   ROZKAZ			- 3 bajty
		   ILOSC DANYCH		- 1 bajt
		   DANE				- 20 bajt�w
		   LRC				- 1 bajt

		@param roz typ rozkazu
		@param dana dane w ramce w postaci stringu
		@return wskaznik do stworzonej ramki
	*/
	char* Koduj(enum TYP roz, string dana);

	/**
		Funkcja wydobywaj�ca dane z ramki.
		@param ramka otrzymana ramka
		@throws SocketException
	*/
	void Dekoduj(char* ramka);

	/**
		Funkcja licz�ca sum� kontroln� LRC.
		@param ramkaLRC ramka z kt�rej b�dzie liczona LRC
		@param nb liczba bajt�w 
		@return funkcja zwraca wyliczone LRC
	*/
	char policzLRC(char *ramkaLRC, int nb);

	/**
		Funkcja zwracaj�ca dane z ramki je�li jest liczb�.
		@return warto�� danych z ramki
	*/
	float getDane();

	/**
		Funkcja zwracaj�ca typ rozkazu z ramki je�li jest stringiem.
		@return warto�� danych z ramki
	*/
	string getNazwa();

	/**
		Funkcja zwracaj�ca typ rozkazu z ramki.
		@return typ rozkazu z ramki
	*/
	TYP getRozkaz();

	/**
		Funkcja zwracaj�ca ilo�� danych przesy�anych przez ramk�.
		@return liczba bajt�w danych
	*/
	int getIloscDanych();

	static const int ROZMIAR=26;
	
private:
	float dane;			// dane z ramki, jesli s� liczb�
	string nazwa;		// dane z ramki, je�li jest stringiem
	enum TYP rozkaz;	// typ rozkazu w ramce

	static char START;	// bajt startu
	char LRC;			// suma kontrolna ramki
	char iloscDanych;	// ilosc danych przesy�anych przez ramk�
	
};

