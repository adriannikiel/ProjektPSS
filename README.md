# ProjektPSS

Projekt na zaliczenie przedmiotu Programowanie Systemów Sterowania napisany w języku C++ na platformie .NET.

## Wymagania projektowe

1. Program do symulacji obiektu dyskretnego w pętli otwartej:
	- dający możliwość symulacji obiektu dyskretnego dowolnego rzędu (program może mieć ograniczenie typu #define lub const),
	- postać symulowanego obiektu to ARMAX, czyli obiekt posiada wielomian C oraz generator losowy, z przeliczaniem wariancji generatora na wariancję „wyjścia” członu C/A,
	- dający możliwość pracy krokowej: w reakcji na jedną próbkę wejścia obiekt zwraca jedną próbkę wyjścia, przy czym obiekt przechowuje swój stan pomiędzy dwoma wywołaniami,
	- dający możliwość symulacji obiektu niestacjonarnego, z drugim zestawem parametrów, i określonymi dwoma chwilami przełączania, pomiędzy którymi następuje płynne (liniowe) przejście pomiędzy 		parametrami; w szczególnym przypadku, gdy obydwie te chwile są sobie równe, przełączenie jest skokowe; w przypadku, gdy chwile przełączenia są <=0, obiekt jest stacjonarny; niestacjonarność nie musi obejmować przypadku zmiany rozmiarów wielomianów,
	- niestacjonarność jest właściwością obiektu, czyli powinna być zapisaną w konfiguracji obiektu,
	- posiadający nieliniowości w postaci nasycenia na wejściu – w przypadku, gdy wartość wejściowa jest na moduł większa od strefy nasycenia, do symulacji używana jest wartość strefy nasycenia (i taka jest zapamiętywana),
	- dający możliwość przechowywania konfiguracji obiektu w pliku, którego minimalna postać to linia komentarza i linia parametrów; lepszą postacią jest plik wykorzystujący podział na sekcje odpowiadające poszczególnym obiektom, i zawierające wpisy typu klucz=wartość; w każdym z rozwiązań należy przewidzieć możliwość przechowywania konfiguracji kilku obiektów,		
	- dający możliwość wyprowadzenia danych z symulacji obiektu do pliku (przeładowanie operatora? obiekt typu strumień?),
	- korzystający – w sensowny sposób – z mechanizmu dziedziczenia i/lub zawierania klas. Minimalnym wymaganiem jest wydzielenie abstrakcyjnej klasy dyskretnego obiektu SISO, z co najmniej jedną funkcją jednoargumentową, służącą do wykonania jednego kroku symulacji (inne funkcje według potrzeby). Można także wydzielić klasę „Wielomian”, bo obiekt dyskretny ma trzy, tak samo działające, wielomiany; funkcjonalność takiej klasy nie może jednak polegać wyłącznie na przechowywaniu parametrów wielomianu! (taką właściwość ma „vector”).

2. Prosty, graficzny interfejs użytkownika, o następujących właściwościach:
	- posiadający dwa wykresy: wyjście i wartość zadana na jednym, sterowanie na drugim, umieszczonym poniżej pierwszego,
	- pozwalający na zmianę parametrów symulacji, co najmniej przez uruchomienie procedury ponownie odczytującej parametry z plików i aplikującej te parametry do obiektu bez restartowania symulacji (lepsze rozwiązanie to zakładki zawierające odpowiednie elementy dialogowe pozwalające na zmianę parametrów),
	- umożliwiający symulację krokową i zmianę szybkości tej symulacji. 


