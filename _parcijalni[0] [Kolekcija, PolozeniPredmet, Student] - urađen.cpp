#include <iostream>
using namespace std;


/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
6. ZA POTREBE TESTIRANJA, UNUTAR MAIN FUNKCIJE MOZETE DODAVATI NOVE TESTNE PODATKE
****************************************************************************/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char * crt = "\n----------------------------------------------------\n";
class Datum {
	int * _dan, *_mjesec, *_godina;
public:
	Datum(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	Datum(const Datum& drugi) {
		_dan = new int(*drugi._dan);
		_mjesec = new int(*drugi._mjesec);
		_godina = new int(*drugi._godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum& operator=(const Datum& drugi) {
		if (this != &drugi) {
			*_dan = *drugi._dan;
			*_mjesec = *drugi._mjesec;
			*_godina = *drugi._godina;
		}
		return *this;
	}
	friend ostream & operator<<(ostream & COUT, Datum obj) {
		COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina;
		return COUT;
	}
};

template<class T, int velicina>
class FITArray {
	T _niz[velicina];
	int _trenutno;
	bool _dozvoliDupliciranje;
public:
	//OSTALE FUNKCIJE CLANICE DEFINISATI VAN TEMPLATE KLASE
	FITArray(bool dozvoliDupliciranje = true) { _dozvoliDupliciranje = dozvoliDupliciranje; _trenutno = 0; }
	int GetTrenutno() { return _trenutno; }
	T * GetNiz() { return _niz; }
	bool operator+=(T el);
	void operator-=(T el);
	friend ostream& operator<< <>(ostream& COUT, const FITArray<T, velicina>& obj);
	FITArray<T, velicina> operator()(int OD, int DO);
};
template<class T, int velicina>
bool FITArray<T, velicina>::operator+=(T el) {
	if (_trenutno >= velicina) return false;

	if (!_dozvoliDupliciranje) {
		for (int i = 0; i < _trenutno; i++)
			if (_niz[i] == el)
				return false;
	}

	_niz[_trenutno++] = el;
	return true;
}

template<class T, int velicina>
void FITArray<T, velicina>::operator-=(T el) {
	for (int i = 0; i < _trenutno; i++) {
		if(_niz[i] == el){
			for (int j = i; j < _trenutno - 1; j++) {
				_niz[j] = _niz[j + 1];
			}
			_trenutno--;
			return;
		}
	}
}

template<class T, int velicina>
FITArray<T, velicina> FITArray<T, velicina>::operator()(int OD, int DO) {
	if (DO > _trenutno)
		DO = _trenutno;

	FITArray<T, velicina> povratna;
	for (int i = OD; i < DO; i++) {
		povratna += _niz[i];
	}
	return povratna;
}

template<class T, int velicina>
ostream& operator<< <>(ostream& COUT, const FITArray<T, velicina>& obj) {
	for (int i = 0; i < obj._trenutno; i++) {
		COUT << obj._niz[i] << endl;
	}
	COUT << endl;
	return COUT;
}

class PolozeniPredmet {
	char * _naziv;
	int _ocjena;
	Datum _datumPolaganja;
public:
	PolozeniPredmet():_datumPolaganja(1,1,2000) {
		_naziv = new char('\0');
	}
	PolozeniPredmet(Datum& datum, const char * naziv, int ocjena): _datumPolaganja(datum) {
		int len = strlen(naziv) + 1;
		_naziv = new char[len];
		strcpy_s(_naziv, len, naziv);
		_ocjena = ocjena;
	}
	PolozeniPredmet(const PolozeniPredmet& drugi): _datumPolaganja(drugi._datumPolaganja) {
		int len = strlen(drugi._naziv) + 1;
		_naziv = new char[len];
		strcpy_s(_naziv, len, drugi._naziv);
		_ocjena = drugi._ocjena;
	}
	~PolozeniPredmet() { delete[] _naziv; _naziv = nullptr; }
	bool operator==(const PolozeniPredmet& drugi) {
		return strcmp(_naziv, drugi._naziv) == 0 && _ocjena == drugi._ocjena;
	}
	PolozeniPredmet& operator=(const PolozeniPredmet& drugi) {
		if (this != &drugi) {
			delete[] _naziv;
			int len = strlen(drugi._naziv) + 1;
			_naziv = new char[len];
			strcpy_s(_naziv, len, drugi._naziv);
			_ocjena = drugi._ocjena;
			_datumPolaganja = drugi._datumPolaganja;
		}
		return *this;
	}
	int GetOcjena() { return _ocjena; }
	friend ostream & operator<<(ostream & COUT, const PolozeniPredmet & obj) {
		COUT << obj._naziv << "(" << obj._ocjena << ")" << obj._datumPolaganja << endl;
		return COUT;
	}
};

class Student {
	static int _indeksCounter; //sluzi za odredjivanje narednog broja indeksa koji ce biti dodijeljen studentu, pocevsi od 160000
	const int _indeks;
	char * _imePrezime;
	FITArray<PolozeniPredmet, 40> _polozeniPredmeti;
public:
	Student(const char * imePrezime): _indeks(_indeksCounter) {
		int len = strlen(imePrezime) + 1;
		_imePrezime = new char[len];
		strcpy_s(_imePrezime, len, imePrezime);
	}
	~Student() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	void DodajPredmet(PolozeniPredmet& p) {
		_polozeniPredmeti += p;
	}
	float GetProsjek() {
		float ukupnoOcjena = 0;
		int brOcjena = 0;
		for (int i = 0; i < _polozeniPredmeti.GetTrenutno(); i++) {
			ukupnoOcjena += _polozeniPredmeti.GetNiz()[i].GetOcjena();
			brOcjena++;
		}
		return ukupnoOcjena / brOcjena;
	}
	Student& operator=(const Student& drugi) {
		if (this != &drugi) {
			delete[] _imePrezime;
			int len = strlen(drugi._imePrezime) + 1;
			_imePrezime = new char[len];
			strcpy_s(_imePrezime, len, drugi._imePrezime);
			_polozeniPredmeti = drugi._polozeniPredmeti;
		}
		return *this;
	}
	friend ostream & operator<<(ostream & COUT, Student & obj) {
		COUT << obj._indeks << " " << obj._imePrezime << endl << obj._polozeniPredmeti;
		return COUT;
	}
};
int Student::_indeksCounter = 160000;

void main() {
	const int max = 20;

	Datum jucer(20, 11, 2017), danas(21, 11, 2017), sutra(22, 11, 2017);
	cout << jucer << danas << sutra << endl;
	jucer = danas;
	cout << jucer << danas << sutra << endl;

	//parametar tipa bool odredjuje da li je u nizu dozvoljeno dupliciranje elemenata
	FITArray<int, max> nizIntegera(false);

	for (size_t i = 0; i < max - 1; i++)
		nizIntegera += i;//dodaje novog clana niza

	if (!(nizIntegera += 6))//pokusavamo dodati dupli clan niza
		cout << "Element nije dodan u niz" << endl;

	cout << crt << nizIntegera << crt; //ispisuje sve clanove niza

	nizIntegera -= 9; //uklanja clan niza sa vrijednoscu 9
	nizIntegera -= 17;

	FITArray<int, max> noviNizIntegera(nizIntegera);

	cout << crt << noviNizIntegera << crt;

	/*parametri odredjuju lokacije (indekse u nizu) elemenata OD - DO koje je potrebno vratiti. u slucaju da u nizu ne postoji trazeni broj 
	elemenata funkcija treba da vrati sve element od lokacije OD pa do posljednje dostupnog elementa */
	cout << "Clanovi niza od lokacije 2 do lokacije 5 su -> " << noviNizIntegera(2, 5) << crt;

	Student adel("Adel Handzic"), jasmin("Jasmin Azemovic");

	PolozeniPredmet prII(jucer, "PRII", 8), prIII(sutra, "PRIII", 7);

	adel.DodajPredmet(prII);
	adel.DodajPredmet(prIII);
	cout << adel << endl;
	//vraca prosjecnu ocjenu studenta
	cout << "Prosjecna ocjena -> " << adel.GetProsjek() << crt;

	jasmin = adel;
	cout << jasmin << endl;
	cout << "Prosjecna ocjena -> " << jasmin.GetProsjek() << crt;

	system("PAUSE");
}
