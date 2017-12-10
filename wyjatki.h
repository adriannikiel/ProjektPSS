/***************************************************************************
 *   Copyright (C) 2008 by Adrian Nikiel   *
 ***************************************************************************/

#ifndef WYJATKI_H
#define WYJATKI_H

/**
	Klasa wyjątku. Błąd przy otwieraniu pliku konfiguracyjnego.
	@author Adrian Nikiel
*/
class OpenException: public exception
{
	public:
	/**
		Konstruktor klasy OpenException
		@param str komunikat błędu
	*/
	OpenException(string str)
	{
		error = str;
	}

	/**
		Funkcja zwraca informację o przyczynie błędu.
		@return Informacja o przczynie błędu.
	*/
	virtual const char* what() const throw()
	{
		return error.c_str();
	}

private:
	string error;
};

/**
	Klasa wyjątku. Błąd przy odczycie danych z pliku konfiguracyjnego.
	@author Adrian Nikiel
*/
class ReadDataException: public exception
{
public:
	/**
		Konstruktor klasy ReadDataException
		@param str komunikat błędu
	*/
	ReadDataException(string str)
	{
		error = str;
	}

	/**
		Funkcja zwraca informację o przyczynie błędu.
		@return Informacja o przczynie błędu.
	*/
	virtual const char* what() const throw()
	{
		return error.c_str();
	}

private:
	string error;
};

/**
	Klasa wyjątku. Błąd przy Socketach.
	@author Adrian Nikiel
*/
class SocketException: public exception
{
	public:
	/**
		Konstruktor klasy SocketException
		@param str komunikat błędu
	*/
	SocketException(string str)
	{
		error = str;
	}

	/**
		Funkcja zwraca informację o przyczynie błędu.
		@return Informacja o przyczynie błędu.
	*/
	virtual const char* what() const throw()
	{
		return error.c_str();
	}

private:
	string error;
};

typedef void (*WSKF)();

/**
	Klasa służąca do podmiany funkcji obsługującej niespodziewane wyjątki.
*/
class Instalator
{
	WSKF poprzednia;

public:
	/**
		Konstruktor klasy Instalator który podmnienia funkcję obsługującą niespodziewane wyjątki.
		@param nowa wskaźnik to nowej funkcji obsługującej niespodziewane wyjątki.
	*/
	Instalator(WSKF nowa)
	{
		poprzednia = set_unexpected(nowa);
	}

	/**
		Destruktor klasy Instalator, który przywraca poprzednią funkcję
		do obsługi niespodziewanych wyjątków.
	*/
	~Instalator()
	{
		set_unexpected(poprzednia);
	}
};
#endif