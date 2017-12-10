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
		Funkcja tworz¹ca ramkê do wysy³ania danych.
		   Postac ramki:
		     START|ROZKAZ|ILOSC DANYCH|DANE|LRC
		
		   START			- 1 bajt
		   ROZKAZ			- 3 bajty
		   ILOSC DANYCH		- 1 bajt
		   DANE				- 20 bajtów
		   LRC				- 1 bajt

		@param roz typ rozkazu
		@param dana dane w ramce w postaci liczby
		@return wskaznik do stworzonej ramki
	*/
	char* Koduj(enum TYP roz, float dana);

	/**
		Funkcja tworz¹ca ramkê do wysy³ania danych.
		   Postac ramki:
		     START|ROZKAZ|ILOSC DANYCH|DANE|LRC
		
		   START			- 1 bajt
		   ROZKAZ			- 3 bajty
		   ILOSC DANYCH		- 1 bajt
		   DANE				- 20 bajtów
		   LRC				- 1 bajt

		@param roz typ rozkazu
		@param dana dane w ramce w postaci stringu
		@return wskaznik do stworzonej ramki
	*/
	char* Koduj(enum TYP roz, string dana);

	/**
		Funkcja wydobywaj¹ca dane z ramki.
		@param ramka otrzymana ramka
		@throws SocketException
	*/
	void Dekoduj(char* ramka);

	/**
		Funkcja licz¹ca sumê kontroln¹ LRC.
		@param ramkaLRC ramka z której bêdzie liczona LRC
		@param nb liczba bajtów 
		@return funkcja zwraca wyliczone LRC
	*/
	char policzLRC(char *ramkaLRC, int nb);

	/**
		Funkcja zwracaj¹ca dane z ramki jeœli jest liczb¹.
		@return wartoœæ danych z ramki
	*/
	float getDane();

	/**
		Funkcja zwracaj¹ca typ rozkazu z ramki jeœli jest stringiem.
		@return wartoœæ danych z ramki
	*/
	string getNazwa();

	/**
		Funkcja zwracaj¹ca typ rozkazu z ramki.
		@return typ rozkazu z ramki
	*/
	TYP getRozkaz();

	/**
		Funkcja zwracaj¹ca iloœæ danych przesy³anych przez ramkê.
		@return liczba bajtów danych
	*/
	int getIloscDanych();

	static const int ROZMIAR=26;
	
private:
	float dane;			// dane z ramki, jesli s¹ liczb¹
	string nazwa;		// dane z ramki, jeœli jest stringiem
	enum TYP rozkaz;	// typ rozkazu w ramce

	static char START;	// bajt startu
	char LRC;			// suma kontrolna ramki
	char iloscDanych;	// ilosc danych przesy³anych przez ramkê
	
};

