#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

bool wczytanie_nazwy(string nazwa, fstream& plik, int numer)		//wczytuje nazwe pliku tak dlugo, az zostanie podana prawidlowa
{
	cout << "\nProsze podac nazwe " << numer << " pliku\n";
	cin >> nazwa;

	plik.open(nazwa.c_str());

	if (!plik)
	{
		cerr << "\nNie udalo sie otworzyc pliku " << numer << endl << "sprobuj jeszcze raz, pamietaj o koncowce *.txt\n\n";
		return false;
	}
	return true;
}

int ilosc_znakow(fstream& plik)		//liczy ile znakow ma plik
{
	char znak;
	int ileznakow = 0;

	plik.clear();
	plik.seekg(0, ios::beg);

	while (plik.get(znak))
		ileznakow++;

	return ileznakow;
}

void usuniecie_bialych(fstream& plik, fstream& plik_poprawiony)		//usuwa znaki biale i interpunkcyjne, zamienia duze litery na male
{
	char znak;

	plik.clear();
	plik.seekg(0, ios::beg);

	while (plik.get(znak))
	{
		if ((int(znak) > 96 and int(znak) < 123) or (int(znak) > 47 and int(znak) < 58))
			plik_poprawiony << znak;
		else if (int(znak) > 64 and int(znak) < 91)
		{
			znak = int(znak) + 32;
			plik_poprawiony << znak;
		}

	}
}

void dopiszzera(fstream& plik1, fstream& plik2)		//do krotszego z plikow dopisuje zera
{
	int roznica = ilosc_znakow(plik1) - ilosc_znakow(plik2);

	plik1.clear();
	plik1.seekg(0, ios::beg);
	plik2.clear();
	plik2.seekg(0, ios::beg);

	if (roznica > 0)
	{
		plik2.clear();
		plik2.seekg(0, ios::end);
		for (int i = 0; i < roznica; i++)
		{
			plik2 << "0";
		}
	}

	else if (roznica < 0)
	{
		plik1.clear();
		plik1.seekg(0, ios::end);
		for (int i = 0; i > roznica; i--)
		{
			plik1 << "0";
		}
	}
}

void czyszczenie_temp()			//czyszczenie plikow tymczasowych ze zedytowanym funkcja usuniecie_bialych tekstem
{
	ofstream plik1pop("pierwszypoprawiony.txt");
	ofstream plik2pop("drugipoprawiony.txt");

	plik1pop.close();
	plik2pop.close();
}

int przygotowanie()		//funkcja, ktora przygotowuje pliki do wyliczenia z nich wspolczynnika
{
	int numer = 1;
	string nazwa_pliku1, nazwa_pliku2;
	fstream plik1, plik2;

	bool poprawne_wczytanie = false;
	while (poprawne_wczytanie != true)
		poprawne_wczytanie = wczytanie_nazwy(nazwa_pliku1, plik1, numer);

	numer++;
	poprawne_wczytanie = false;
	while (poprawne_wczytanie != true)
		poprawne_wczytanie = wczytanie_nazwy(nazwa_pliku2, plik2, numer);

	czyszczenie_temp();

	fstream plik1pop("pierwszypoprawiony.txt");
	fstream plik2pop("drugipoprawiony.txt");

	usuniecie_bialych(plik1, plik1pop);
	usuniecie_bialych(plik2, plik2pop);
	dopiszzera(plik1pop, plik2pop);

	plik1.close();
	plik2.close();
	plik1pop.close();
	plik2pop.close();

	return 0;
}

double srednia_pliku(fstream& plik)		//funkcja liczaca srednia liczbowa wszystkich znakow z pliku
{
	int suma = 0;
	int n = ilosc_znakow(plik);
	char znak;
	double wynik;

	plik.clear();
	plik.seekg(0, ios::beg);

	while (plik.get(znak))
		suma = suma + int(znak);

	wynik = suma / n;

	return wynik;
}

double obliczanie_rxy()		//funkcja obliczajaca wspolczynnik rxy
{
	fstream plik1fin("pierwszypoprawiony.txt");
	fstream plik2fin("drugipoprawiony.txt");

	char znak1, znak2;
	double rxy = 0;
	double sredniax = srednia_pliku(plik1fin);
	double sredniay = srednia_pliku(plik2fin);
	int n = ilosc_znakow(plik1fin);
	double sumaroznicx = 0, sumaroznicy = 0, gora = 0;


	plik1fin.clear();
	plik1fin.seekg(0, ios::beg);
	plik2fin.clear();
	plik2fin.seekg(0, ios::beg);

	for (int i = 0; i < n; i++)
	{
		plik1fin.get(znak1);
		plik2fin.get(znak2);

		sumaroznicx += (znak1 - sredniax) * (znak1 - sredniax);
		sumaroznicy += (znak2 - sredniay) * (znak2 - sredniay);
		gora += (znak1 - sredniax) * (znak2 - sredniay);
	}

	rxy = gora / (sqrt(sumaroznicx) * sqrt(sumaroznicy));

	plik1fin.close();
	plik2fin.close();

	return rxy;
}

int main()
{
	przygotowanie();
	double wspolczynnik = obliczanie_rxy();

	if (wspolczynnik > 0.5)
		cout << "\n\nTeksty sa podobne (plagiat). Wspolczynnik wynosi " << wspolczynnik;
	else
		cout << "\n\nTeksty nie sa podobne. Wspolczynnik wynosi " << wspolczynnik;
}
