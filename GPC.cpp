#include "GPC.h"
#include "wyjatki.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>

vector<double> metodaGaussa(int liczbaRownan, vector<vector<double> >  T);
using namespace std;

GPC::GPC(void): IRegulator()
{
}

GPC::GPC(string sciezka, string obiekt, double lambda) throw(OpenException, ReadDataException): IRegulator(obiekt)
{
	string doWczytania[] = {"H=", "L=", "ro=", "alfa=", "dA=", "dB=", "SKOK=", "AMPLITUDA_PROSTOKATA=", "OKRES_PROSTOKATA=",
		"WYPELNIENIE_PROSTOKATA=", "AMPLITUDA_SINUSA=", "OKRES_SINUSA=", "AMPLITUDA_TROJKATA=",
		"OKRES_TROJKATA=", "WARIANCJA_SZUMU=", "DLUGOSC_ZAKLOCENIA=", "SYGNAL=", "WYJSCIE_MIN=", "WYJSCIE_MAX="};
	const int DO_WCZYTANIA = 19; 
	
	ifstream plik; 
	plik.open(sciezka.c_str(), ios::in);

	// gdy próba otwarcia pliku nie powiod³a siê.
	if(!plik)
	{
		plik.clear(plik.rdstate() & ~ios::failbit);
		OpenException openEx("B³¹d otwarcia pliku konfiguracyjnego !!!");
		throw openEx;
	}
	
	char linia[100];		// linia wczytana z pliku konfiguracyjnego
	
	// zmienna wskazuj¹ca na poprawne wczytanie danych gdy jest równa 14
	int wczytano = 0;

	int test = 0;
			
	while(plik.getline(linia, 100))
	{
		string slinia(linia);
		
		string::size_type pozycja1 = slinia.find("MODEL=");
		string::size_type pozycja2 = slinia.find(obiekt);
			
		// gdy znaleziono model o podanej nazwie
		if(pozycja1 != string::npos && pozycja2 != string::npos)
		{
			bool koniec = false;
			
			do
			{
				string::size_type pozycja = slinia.find("#");
				
				// gdy koniec opisu modelu
				if(pozycja != string::npos)
					koniec = true;
					
				// gdy nie koniec opisu modelu wczytujemy dane
				else
				{
					for(int i=0; i<DO_WCZYTANIA; i++)
					{
						if( wczytaj(doWczytania[i],slinia) )
						{test = test|(1<<i);
							wczytano++;
							plik.getline(linia, 100);
							slinia = linia;
							continue;
						}
					}
					
					plik.getline(linia, 100);
					slinia = linia;
				}
			}while(!koniec);
		}
	}
	
	plik.close();
		
	// sprawdzenie czy wszystkie dane zosta³y poprawnie wczytane
	if(wczytano != DO_WCZYTANIA)
	{
		ReadDataException readEx("B³¹d wcztywania danych z pliku konfiguracyjnego !!!");
		throw readEx;
	}

	// generacja zak³ócenia
	zakl = Zaklocenie(dlugoscZaklucenia, wariancja);

	maxIter = okresP * okresS * okresT;
	
	// inicjalizacja identyfikacji
	
	identyf = new Identyfikacja(dA,dB,1,1,0.99,lambda);
	popSterowanie = 0.0001;
    wartoscRegulowana = 0.0;
    wartoscZadana = 0.0;
    
	// ustawienie pocz¹tkowych sterowañ na 0
	for(int i=0; i<dB+2; i++)   ////  i<B.size()+1
		uuPoprzednie.push_back(0);
		
	// ustawienie pocz¹tkowych wyjœæ na 0
	for(int i=0; i<dA+1; i++)    /////  i<A.size()
		yyPoprzednie.push_back(0);

}

GPC::GPC(int HH, int LL, double Ro, double Alfa, int ddA, int ddB): IRegulator()
{
	H = HH;
	L = LL;
	ro = Ro;
	alfa = Alfa;
	dA = ddA;
	dB = ddB;

	// inicjalizacja identyfikacji
	identyf = new Identyfikacja(dA,dB,1,1,0.99,1000);
	// ustawienie pocz¹tkowych sterowañ na 0
	for(int i=0; i<dB+2; i++)   ////  i<B.size()+1
		uuPoprzednie.push_back(0);
		
	// ustawienie pocz¹tkowych wyjœæ na 0
	for(int i=0; i<dA+1; i++)    /////  i<A.size()
    		yyPoprzednie.push_back(0);

	popSterowanie = 0.0001;
	wartoscRegulowana = 0.0;
    wartoscZadana = 0.0;
	
}

GPC::~GPC(void)
{
}

void GPC::setNastawyRegulatora(const vector<double> &nastawy) throw(ReadDataException)
{
	if(nastawy.size() == 6)
	{
		H = nastawy[0];
		L = nastawy[1];
		ro = nastawy[2];
		alfa = nastawy[3];
		dA = nastawy[4];
		dB = nastawy[5];
		
	}
	else
	{
		ReadDataException readEx("Podano nieprawid³owy wektor danych !!!");
		throw readEx;
	}
}

void GPC::setNastawyRegulatora(int parametr, double nastawa)
{
	if(parametr == pH)
		H = nastawa;
	else if(parametr == pL)
		L = nastawa;
	else if(parametr == pro)
		ro = nastawa;
	else if(parametr == palfa)
		alfa = nastawa;
	else if(parametr == pdA)
		dA = nastawa;
	else if(parametr == pdB)
		dB = nastawa;
	
	else
	{
		ReadDataException readEx("Wybrano nieprawid³owy parametr !!!");
		throw readEx;
	}
}

string GPC::getNastawy()
{
	stringstream ss;
	string str;
	string konf;

	konf.append("H= ");
	ss << H;
	ss >> str;
	konf.append(str);

	konf.append("\nL= ");
	ss << L;
	ss >> str;
	konf.append(str);

	konf.append("\nro= ");
	ss << ro;
	ss >> str;
	konf.append(str);

	konf.append("\nalfa= ");
	ss << alfa;
	ss >> str;
	konf.append(str);

	konf.append("\ndA= ");
	ss << dA;
	ss >> str;
	konf.append(str);

	konf.append("\ndB= ");
	ss << dB;
	ss >> str;
	konf.append(str);

	return konf;
}

void GPC::setParamABK(int ddA, int ddB, double aalfaa)
{
	dA = ddA;
	dB = ddB;
	delete identyf;

	identyf = new Identyfikacja(dA, dB, 1, 1, aalfaa, 1000);
	resetRegulatora();

}

double GPC::liczWyjscie(double warZadana, double y)
{	

	wartoscRegulowana = y;
    wartoscZadana = warZadana;

	// aktualizacja wektora poprzednich sterowañ
	for(int i=dB+1; i>0; i--)  //i<  uuPoprzednie.size()-1;
		uuPoprzednie[i] = uuPoprzednie[i-1];
	
	uuPoprzednie[0] = popSterowanie;

	// aktualizacja wektora poprzednich wyjœæ z obiektu
	for(int i=dA; i>0; i--)  // yyPoprzednie.size()-1
		yyPoprzednie[i] = yyPoprzednie[i-1];
	
	yyPoprzednie[0] = wartoscRegulowana;
   
    vector<double> noweTeta;
	    
	A.clear();
	B.clear();

	identyf->liczParam(popSterowanie, y);
	noweTeta = identyf->getParam();
				
	for(int i=0; i<=dB; i++)
		B.push_back(noweTeta[i]);
	
	A.push_back(1);
	for(int i=0; i<dA; i++)
		A.push_back(noweTeta[i+dB+1]);
	
    obliczQ();
	
	obliczW0();

	obliczOdpSwob();

	double deltaSterowanie = 0;   // przyrost nowego sterowania

	// wyznacznie mnozenie wektorów  q'*odpSwob
	for(int i=0;i<H;i++)
		deltaSterowanie -= wekQ[i]*odpSwob[i];
	
	 //wyznacznie mnozenie wektorów  q'*w0 i koncowe obliczenie przyrostu sterowania
	for(int i=0;i<H;i++)
		deltaSterowanie += wekQ[i]*w0[i];

	popSterowanie = popSterowanie + deltaSterowanie;
	return popSterowanie;   // zwraca nowe sterowanie

}


bool GPC::wczytaj(string dana, string slinia) throw(ReadDataException)
{
	string::size_type pozycja = slinia.find(dana);
	
	// gdy w linii znajduj¹ siê szukane dane
	if(pozycja == 0)
	{
		string sliczba;
		
		// pêtla wczytuj¹ca dane
		do
		{
			pozycja = slinia.find(" ",pozycja+1);
			sliczba = slinia.substr(pozycja+1, slinia.find(" ",pozycja+1) - pozycja-1);
			if(sliczba != "" && sliczba != dana)
			{
				if(dana == "H=")
					H = strtod (sliczba.c_str(),NULL);
				else if(dana == "L=")
					L = strtod (sliczba.c_str(),NULL);
				else if(dana == "ro=")
					ro = strtod (sliczba.c_str(),NULL);
				else if(dana == "alfa=")
					alfa = strtod (sliczba.c_str(),NULL);
				else if(dana == "dA=")
					dA = strtod (sliczba.c_str(),NULL);
				else if(dana == "dB=")
					dB = strtod (sliczba.c_str(),NULL);
				else if(dana == "SKOK=")
					skok = strtod (sliczba.c_str(),NULL);
				else if(dana == "AMPLITUDA_PROSTOKATA=")
					amplitudaP = strtod (sliczba.c_str(),NULL);
				else if(dana == "OKRES_PROSTOKATA=")
					okresP = atoi (sliczba.c_str());
				else if(dana == "WYPELNIENIE_PROSTOKATA=")
					wypelnienieP = strtod (sliczba.c_str(),NULL);
				else if(dana == "AMPLITUDA_SINUSA=")
					amplitudaS = strtod (sliczba.c_str(),NULL);
				else if(dana == "OKRES_SINUSA=")
					okresS = atoi (sliczba.c_str());
				else if(dana == "AMPLITUDA_TROJKATA=")
					amplitudaT = atoi (sliczba.c_str());
				else if(dana == "OKRES_TROJKATA=")
					okresT = atoi (sliczba.c_str());
				else if(dana == "WARIANCJA_SZUMU=")
				{
					if(strtod (sliczba.c_str(),NULL) >= 0)
						wariancja = strtod (sliczba.c_str(),NULL);
					else
					{
						ReadDataException readEx("Wariancja powinna byæ wiêksza lub równa 0.");
						throw readEx;
						return false; 
					}
				}
				else if(dana == "DLUGOSC_ZAKLOCENIA=")
				{
					if(atoi (sliczba.c_str()) > 0)
						dlugoscZaklucenia = atoi (sliczba.c_str());
					else
					{
						ReadDataException readEx("D³ugoœæ zak³ócenia powinna byæ wiêksza od 0.");
						throw readEx;
						return false; 
					}
				}
				else if(dana == "SYGNAL=")
					sygnalZadany = atoi (sliczba.c_str());
				else if(dana == "WYJSCIE_MIN=")
					wyMin = strtod (sliczba.c_str(),NULL);
				else if(dana == "WYJSCIE_MAX=")
					wyMax = strtod (sliczba.c_str(),NULL);
			}
		}while(pozycja != string::npos);
		
		return true;
	}
	
	// gdy w lini nie ma szukanych danych
	return false; 
}

void GPC::resetRegulatora()
{
	
	// ustawienie pocz¹tkowych sterowañ na 0
	uuPoprzednie.clear();
	for(int i=0; i<dB+2; i++)   ////  i<B.size()+1
		uuPoprzednie.push_back(0);
		
	// ustawienie pocz¹tkowych wyjœæ na 0
	yyPoprzednie.clear();
	for(int i=0; i<dA+1; i++)    /////  i<A.size()
		yyPoprzednie.push_back(0);

	//popSterowanie = 0.0001;
	identyf->reset();
	
    
	popSterowanie = 0.0001;
    wartoscRegulowana = 0.0;
    wartoscZadana = 0.0;

}

void GPC::obliczQ() 
{
	/////////////    Wyznaczenie odpowiedzi skokowej B/A - wektora [h[0],h[1],...,h[H-1]]'  /////////

	vector<double> h;
	double odpskoku;

	for(int i=0;i<H;i++)
	{
		odpskoku = 0;

		for(int j=0;j<B.size();j++)
		{
			if( i-j >= 0 )
				odpskoku += B[j];
		}

		for(int j=1;j<A.size();j++)
		{
			if( i-j >= 0 )
				odpskoku -= A[j]*h[i-j];
		}

		h.push_back(odpskoku);

	}


	/////////////   Wypelnienie macierzy Q   ///////////////////

	//		| h[0]		0		...		0		|
	//		| h[1]		h[0]	...		0		|
	//  Q = | ...		...		...		...		|
	//		| h[L-1]	h[L-2]	...		h[0]	|	
	//		| ...		...		...		...		|
	//		| h[H-1]	h[H-2]	...		h[H-L]	|

	vector<vector<double> >  Q;

	vector<double> q;
	q.clear();
	Q.clear();

	for(int i=0;i<L;i++)
		q.push_back(i);

	for(int i=0;i<H;i++)
		Q.push_back(q);
    
  	for (int i = 0; i < H; i++) 
	{
        for (int j = 0; j < L; j++)
		{
			Q[i][j]=0;

			int aa=i-j;
			if( aa >= 0)
				Q[i][j]=h[i-j];
		}
    }

	///////       Rozwiazywanie ukladu rownan             ///////////////
	///////		  [Q'Q + ro*I]x = _1   _1=[1 0 0 .. 0]'   //////////////	

	vector<vector<double> >  QQ;

	q.clear();
	QQ.clear();
	for(int i=0;i<L+1;i++)
		q.push_back(i);

	for(int i=0;i<L;i++)
		QQ.push_back(q);
	
	////   Mnozenie macierzy  Q'Q

	for (int i=0;i<L;i++)
        for (int j=0;j<L;j++) {
            QQ[i][j] = 0;
			for (int k=0;k<H;k++) 
			    QQ[i][j] += Q[k][i] * Q[k][j];
			
		}

	vector<double> wektorX;

	try{
		wektorX = obliczX(ro,QQ);
       		
    }catch(OpenException &e){
    
	wektorX = obliczX(0.5,QQ);
	       
    }

	q.clear();
	for(int i=0;i<H;i++)
		q.push_back(i);

	for(int i=0;i<H;i++) {

		q[i] = 0;
		for(int j=0;j<L;j++)
			q[i] += wektorX[j]*Q[i][j];
	}

	wekQ = q;
}


void GPC::obliczW0()
{
	// Wyznaczenie wektora trajektorii odniesienia w0 = [w0[i] w0[i+1],w0[i+2],...,w0[i+H] ]
	
	// zak³adamy, ¿e nie posiadamy znajomoœci zmian wartoœci zadanej 
		
	w0.clear();
	for(int i=0;i<H;i++)
		w0.push_back(0);

	for(int i=0;i<H;i++)
	{	
		if(i < 1)
			w0[i] = (1-alfa)*wartoscZadana + alfa*wartoscRegulowana;   // warunek poczatkowy dla trajektorii odniesienia
		else
			w0[i] = (1-alfa)*wartoscZadana + alfa*w0[i-1];
	}
}


void GPC::obliczOdpSwob()
{
	
	vector<double> ynowe;
	ynowe = yyPoprzednie;
	// liczenie przyrostów sterowania dla poprzednich chwil czasu
	vector<double> deltaU;
	double roznica;

	for(int i=0;i<B.size();i++)
	{
		roznica = uuPoprzednie[i] - uuPoprzednie[i+1]; 
		deltaU.push_back(roznica);
	}
	
	/////  Wyznaczanie wektora odpowiedzi swobodnej y0=[y0[i+1],y0[i+2],...,y0[i+H] ]
	
	//vector<double> odpSwob;   // wektor odpowiedzi swobodnej
	
	double y = 0;  //nowa wartosc wyjsciowa
	
	for(int i=0;i<B.size();i++)
		y += B[i]*deltaU[i];
    for(int i=0;i<A.size()-1;i++)
		y += (A[i]-A[i+1])*yyPoprzednie[i];
    y += A[A.size()-1]*yyPoprzednie[A.size()-1];

	// aktualizacja wektora poprzednich wyjœæ z obiektu
	for(int i=dA; i>0; i--)  // yyPoprzednie.size()-1
		ynowe[i] = ynowe[i-1];
	
	ynowe[0] = y;

	odpSwob.clear();
	for(int i=0;i<H;i++)
		odpSwob.push_back(i);

	for(int i=0;i<H;i++)
	{
		odpSwob[i]=0;
		
		/////   czêœæ  odpSwob[0]  = b[1]*deltaU[i-1]  + b[2]*deltaU[i-2]   + ... + b[dB]*deltaU[i-dB]
		/////          odpSwob[1]  = b[1]*deltaU[i]    + b[2]*deltaU[i-1]   + ... + b[dB]*deltaU[i-dB+1]
		/////          ...
		/////          odpSwob[H-1]= b[1]*deltaU[i+H]  + b[2]*deltaU[i+H-1] + ... + b[dB]*deltaU[i-dB+H-1]

		for(int j=0;j<B.size()-1;j++)
		{
			if( j>=i )     /////  pomijamy przyrosty deltaU dla chwil aktualnej i przyszlych, bo wtedy deltaU = 0
 				odpSwob[i] += B[j+1]*deltaU[j-i];
		}

		/////   czêœæ  odpSwob[0]  += (A[0]-A[1])*y[i]      + (A[1]-A[2])*y[i-1]   + ... + A[dA]*y[i-dA]
		/////          odpSwob[1]  += (A[0]-A[1])*y[i+1]    + (A[1]-A[2])*y[i]     + ... + A[dA]*y[i-dA+1]
		/////          ...
		/////          odpSwob[H-1]+= (A[0]-A[1])*y[i+H-1]  + (A[1]-A[2])*y[i+H-2] + ... + A[dA]*y[i-dA+H-1]


		for(int j=0;j<A.size()-1;j++)
		{
			if(j>=i)
				odpSwob[i] += (A[j]-A[j+1])*ynowe[j-i];
			else
				odpSwob[i] += (A[j]-A[j+1])*odpSwob[i-j-1];
		}
		
		if(i<A.size())
			odpSwob[i] += A[A.size()-1]*ynowe[A.size()-i-1];
		else
			odpSwob[i] += A[A.size()-1]*odpSwob[i-A.size()];

	}
	for(int i=0; i>dA; i++)  // yyPoprzednie.size()-1
		ynowe[i] = ynowe[i+1];

}

vector<double> GPC::obliczX(double ro, vector<vector<double> >  QQ)
{
	for(int i=0;i<L;i++)
		for(int j=0;j<L;j++)
			if(i==j)
				QQ[i][j] += ro;

	//// Dodanie wektora _1 do macierzy Q'Q + ro*I

	for(int i=0;i<L;i++)
		if( i==0 )
			QQ[i][L] = 1;
		else
			QQ[i][L] = 0;
	

	vector<double> wektorX;

	//////  Liczenie ukladu rownan [Q'Q + ro*I]x = _1 metoda Gaussa
	wektorX = metodaGaussa(L,QQ);

	return wektorX;
}


vector<double> metodaGaussa(int liczbaRownan, vector<vector<double> >  t)
{
	int n=liczbaRownan;
	int k,x,p,w;
	
    double r,l;
		
    k=0;

	vector<vector<double> > matrix;
    vector<vector<double> > new_matrix;
    vector<vector<double> > temp_matrix;
	vector<double> qqq;

	for(int i=0;i<n;i++)
		qqq.push_back(i);

	for(int i=0;i<n;i++)
		matrix.push_back(qqq);

	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			matrix[i][j]=t[i][j];
	
	new_matrix = matrix;
	temp_matrix = matrix;

	int SIZE = n;
	double wyznacznik;
    if (SIZE == 1){
        wyznacznik = t[0][0];
    }

    if (SIZE == 2) {
        wyznacznik = (t[0][0]* t[1][1]) - (t[0][1]*t[1][0]);
    } else {
        int pivot_row = 0;
        int pivot_col = 0;
        double pivot_point = matrix[pivot_row][pivot_col];

        bool last_pivot = false;
        bool first_pivot = true;
        while (!last_pivot) {
            for (int row = (pivot_row + 1); row < SIZE; ++row) {
                for (int col = 0; col < SIZE; ++col) {
                    //Check if last pivot.
                    if (pivot_point == 0) {
                        new_matrix[row][col] = 0;
                    } else {
                        if (first_pivot != false) {

                            new_matrix[row][col] = matrix[row][col] - matrix[row][pivot_col] * 
									matrix[pivot_row][col]/pivot_point;
                        } else {

                            new_matrix[row][col] = temp_matrix[row][col] - temp_matrix[row][pivot_col] *
                                    temp_matrix[pivot_row][col]/pivot_point;
                        }
                    }
                }
            }
            if (pivot_row == (SIZE - 2)) {
                last_pivot = true;
                break;
            }
            //Move the pivot up one
            pivot_row = pivot_row + 1;
            pivot_col = pivot_col + 1;
            pivot_point = new_matrix[pivot_row][pivot_col];
            first_pivot = false;
            temp_matrix = new_matrix;
        }

        //compute the determinant from the new matrix
        double determinant = 1;
        for (int r = 0; r < SIZE; ++r) {
            determinant = determinant * new_matrix[r][r];
        }

        wyznacznik = determinant;
    }
	
		
	if (abs(wyznacznik) <= 0.0001) {
        throw OpenException("Nie mozna odwrocic macierzy");
    }

	bool sameZera = true;

	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			if( t[i][j] != 0) 
			{
				sameZera = false;
				break;
			}

	vector<double> q;
	 for(int i=0;i<n;i++)
	  q.push_back(0);
	q.push_back(1);

	if(sameZera == true)
		return q;

	for(int i=0;i<n-1;i++) {

       x=i;
       do {
		 if(x==i)  r=t[k][k];
		 t[i][x]=t[i][x]/r;
		 x++;
       } while(x<=n);

       p=1;

       do {
			w=i;

			do {
			   if(w==i)  l=t[i+p][i];
			   t[i+p][w]=t[i+p][w]-(l*t[i][w]);
			   w++;
			}while(w<=n);

			p++;

	   } while( (i+p)<=(n-1) );

       k++;
   }

   int m;
   x=m=n-1;
   r=t[n-1][n-1];

   do {
    t[m][x]=t[m][x]/r;
    x++;
   } while(x<=n);

  int j=0;
  
 for(int i=n;i>=1;i--)
 {
	q[i-1]=t[i-1][n]*q[n];
	for(k=n-1;k>=n-j;k--)
		q[i-1]-= t[i-1][k]*q[k];
	j++;
    
 }

 return q;
}