#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

#pragma warning(disable:4996)
const char *crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char * eRazredChar[] = {"", "PRVI", "DRUGI", "TRECI", "CETVRTI"};
const float minimalan_prosjek = 4.5;

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
public:
	Kolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}
	Kolekcija(const Kolekcija& druga) {
		_trenutno = new int(*druga._trenutno);
		_elementi1 = new T1[*_trenutno];
		_elementi2 = new T2[*_trenutno];

		for (int i = 0; i < *_trenutno; i++) {
			_elementi1[i] = druga._elementi1[i];
			_elementi2[i] = druga._elementi2[i];
		}
	}
	~Kolekcija() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}
	Kolekcija& operator=(const Kolekcija& druga) {
		if (this != &druga) {
			delete[] _elementi1;
			delete[] _elementi2;
			*_trenutno = *druga._trenutno;

			_elementi1 = new T1[*_trenutno];
			_elementi2 = new T2[*_trenutno];

			for (int i = 0; i < *_trenutno; i++) {
				_elementi1[i] = druga._elementi1[i];
				_elementi2[i] = druga._elementi2[i];
			}
		}
		return *this;
	}
	void SortOpadajuciByT2() {
		int brProlaza = *_trenutno - 1;
		bool promjena = true;
		while (promjena) {
			promjena = false;
			for (int i = 0; i < brProlaza; i++) {
				if (_elementi2[i] < _elementi2[i + 1]) {
					swap(_elementi1[i], _elementi1[i + 1]);
					swap(_elementi2[i], _elementi2[i + 1]);
					promjena = true;
				}
			}
			brProlaza--;
		}
	}
	void AddElement(const T1& el1, const T2& el2) {
		T1* temp1 = new T1[*_trenutno + 1];
		T2* temp2 = new T2[*_trenutno + 1];

		for (int i = 0; i < *_trenutno; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[*_trenutno] = el1;
		_elementi2[*_trenutno] = el2;
		(*_trenutno)++;
	}
	T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() { return *_trenutno; }
	friend ostream& operator<< (ostream &COUT, Kolekcija &obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};

class DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	DatumVrijeme(const DatumVrijeme& drugi) {
		_dan = new int(*drugi._dan);
		_mjesec = new int(*drugi._mjesec);
		_godina = new int(*drugi._godina);
		_sati = new int(*drugi._sati);
		_minuti = new int(*drugi._minuti);
	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	int GetMinute() { return *_minuti; }
	int GetSate() { return *_sati; }
	char* ToCharArray() {
		string povratni = to_string(*_dan) + "." + to_string(*_mjesec) + "." + to_string(*_godina) + " " + to_string(*_sati) + ":" + to_string(*_minuti);
		char * temp = new char[povratni.length() + 1];
		strcpy_s(temp, povratni.length() + 1, povratni.c_str());
		return temp;
	}
	DatumVrijeme& operator=(const DatumVrijeme& drugo) {
		if (this != &drugo) {
			*_dan = *drugo._dan;
			*_mjesec = *drugo._mjesec;
			*_godina = *drugo._godina;
			*_sati = *drugo._sati;
			*_minuti = *drugo._minuti;
		}
		return *this;
	}
	friend ostream& operator<< (ostream &COUT, DatumVrijeme &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}
};
int OduzmiMinuteISate(DatumVrijeme& d1, DatumVrijeme& d2) {
	if (d1.GetSate() != d2.GetSate())
		return 2;
	return d1.GetMinute() - d2.GetMinute();
}

class Predmet {
	char * _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const char * naziv, int ocjena, string napomena = "") :_ocjena(ocjena), _napomena(napomena) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, Predmet &obj) {
		COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
		return COUT;
	}
	Predmet& operator=(const Predmet& drugi) {
		if (this != &drugi) {
			delete[] _naziv;
			_naziv = AlocirajNizKaraktera(drugi._naziv);
			_ocjena = drugi._ocjena;
			_napomena = drugi._napomena;
		}
		return *this;
	}
	char * GetNaziv() { return _naziv; }
	string GetNapomena() { return _napomena; }
	int GetOcjena() { return _ocjena; }
	void DodajNapomenu(string napomena) {
		_napomena += " " + napomena;
	}
};

mutex cuvarResursa;
void SendMail(string tekst) {
	cuvarResursa.lock();
	cout << tekst << endl;
	cuvarResursa.unlock();
}
void SendSMS(string tekst) {
	cuvarResursa.lock();
	cout << tekst << endl;
	cuvarResursa.unlock();
}

struct Uspjeh {
	eRazred _razred;
	//datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Kolekcija<Predmet*, DatumVrijeme> * _predmeti;
public:
	Uspjeh(eRazred razred) :_razred(razred), _predmeti(nullptr) {
	}
	Uspjeh(const Uspjeh& drugi) {
		_razred = drugi._razred;
		_predmeti = new Kolekcija<Predmet*, DatumVrijeme>(*drugi._predmeti);
	}
	~Uspjeh() {
		delete _predmeti; _predmeti = nullptr;
	}

	Kolekcija<Predmet*, DatumVrijeme> * GetPredmeti() { return _predmeti; }
	bool AddPredmet(Predmet& predmet, DatumVrijeme& datum) {
		if (_predmeti == nullptr) {
			_predmeti = new Kolekcija<Predmet*, DatumVrijeme>;
			_predmeti->AddElement(&predmet, datum);
			return true;
		}

		if (_predmeti->getTrenutno() >= 6) return false;

		for (int i = 0; i < _predmeti->getTrenutno(); i++) {
			if (strcmp(_predmeti->getElement1(i)->GetNaziv(), predmet.GetNaziv()) == 0) {
				return false;
			}
		}

		int zadnjiPredmet = _predmeti->getTrenutno() - 1;
		if (OduzmiMinuteISate(datum, _predmeti->getElement2(zadnjiPredmet)) < 1)
			return false;

		_predmeti->AddElement(&predmet, datum);
		if (_predmeti->getTrenutno() == 6) {
			thread nit1(SendMail, "evidentirali ste uspjeh za " + string(eRazredChar[_razred]) + " razred");
			nit1.join();
			
			float ukupnoOcjene = 0;
			int brOcjena = 0;
			for (int i = 0; i < _predmeti->getTrenutno(); i++) {
				ukupnoOcjene += _predmeti->getElement1(i)->GetOcjena();
				brOcjena++;
			}
			float prosjek = ukupnoOcjene / brOcjena;
			if (prosjek > minimalan_prosjek) {
				thread nit2(SendSMS, "svaka cast za uspjeh " + to_string(prosjek));
				nit2.join();
			}
		}
		return true;
	}
	eRazred GetRazred() { return _razred; }
	friend ostream& operator<< (ostream &COUT, const Uspjeh &obj) {
		COUT << eRazredChar[obj._razred] << endl;
		for (int i = 0; i < obj._predmeti->getTrenutno(); i++) {
			COUT << *obj._predmeti->getElement1(i) << " " << obj._predmeti->getElement2(i) << endl;
		}
		return COUT;
	}
};

class Kandidat {
	char * _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh *> _uspjeh;
public:
	Kandidat(const char * imePrezime, string emailAdresa, string brojTelefona) :_emailAdresa(emailAdresa), _brojTelefona(brojTelefona) {
		_imePrezime = AlocirajNizKaraktera(imePrezime);
	}
	~Kandidat() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _uspjeh.size(); i++) {
			delete _uspjeh[i];
			_uspjeh[i] = nullptr;
		}
	}
	bool AddPredmet(eRazred razred, Predmet& predmet, DatumVrijeme& datum) {
		for (int i = 0; i < _uspjeh.size(); i++) {
			if (_uspjeh[i]->_razred == razred) {
				return _uspjeh[i]->AddPredmet(predmet, datum);
			}
		}
		_uspjeh.push_back(new Uspjeh(razred));
		return _uspjeh[_uspjeh.size() - 1]->AddPredmet(predmet, datum);
	}
	friend ostream& operator<< (ostream &COUT, Kandidat &obj) {
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
		for (size_t i = 0; i < obj._uspjeh.size(); i++)
			COUT << *obj._uspjeh[i] << endl;
		return COUT;
	}
	vector<Uspjeh *> GetUspjeh() { return _uspjeh; }
	Uspjeh* operator()(eRazred razred) {
		for (int i = 0; i < _uspjeh.size(); i++) {
			if (_uspjeh[i]->GetRazred() == razred)
				return _uspjeh[i];
		}
		return nullptr;
	}
};

void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/

	DatumVrijeme temp,
		datum19062018_1015(19, 6, 2018, 10, 15),
		datum20062018_1115(20, 6, 2018, 11, 15),
		datum30062018_1215(30, 6, 2018, 12, 15),
		datum05072018_1231(5, 7, 2018, 12, 31),
		datum20062018_1115_Copy(datum20062018_1115);
	//funkcija ToCharArray vraca datum i vrijeme kao char *
	cout << datum19062018_1015.ToCharArray() << endl;//treba ispisati: 19.6.2018 10:15
	temp = datum05072018_1231;
	cout << temp.ToCharArray() << endl;//treba ispisati: 5.7.2018 12:31
	cout << datum20062018_1115_Copy.ToCharArray() << endl;//treba ispisati: 20.6.2018 11:15

	const int kolekcijaTestSize = 10;
	Kolekcija<int, int> kolekcija1;
	for (size_t i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i + 1, i * i);
	cout << kolekcija1 << endl;

	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	Kolekcija<int, int> kolekcija3;
	kolekcija3 = kolekcija1;
	kolekcija3.SortOpadajuciByT2(); //od najvece prema najmanjoj vrijednosti
	cout << kolekcija3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet Matematika("Matematika", 5, "Ucesce na takmicenju"),
		Fizika("Fizika", 5),
		Hemija("Hemija", 4),
		Engleski("Engleski", 5),
		Geografija("Geografija", 4),
		Muzicko("Muzicko", 5);
	Fizika.DodajNapomenu("Pohvala za ostvareni uspjeh"); // dodaje novu napomenu zadrzavajuci ranije dodane
	cout << Matematika << endl;

	Kandidat jasmin("Jasmin Azemovic", "jasmin@gmail.com", "033 281 172");
	Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");

	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	tom prilikom onemoguciti:
	- dodavanje vise od 6 predmeta za jedan razred
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti
	najmanje minut).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II
	razred, pa onda za I razred i sl.). Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (jasmin.AddPredmet(DRUGI, Fizika, datum20062018_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Hemija, datum30062018_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Engleski, datum19062018_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Matematika, datum20062018_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Matematiku jer je vec dodana u prvom razredu
	if (jasmin.AddPredmet(PRVI, Matematika, datum05072018_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Fiziku jer nije prosao minut od dodavanja posljednjeg predmeta
	if (jasmin.AddPredmet(PRVI, Fizika, datum05072018_1231))
		cout << "Predmet uspjesno dodan!" << crt;

	if (jasmin.AddPredmet(PRVI, Hemija, datum19062018_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Geografija, datum20062018_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Muzicko, datum30062018_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	/*pretpostavka je da na nivou jednog razreda kandidati imaju 6 predmeta.
	upravo zbog toga, nakon evidentiranja 6 predmeta na nivou jednog razreda kandidatu se salje mail
	sa porukom: "evidentirali ste uspjeh za X razred". ukoliko je prosjek na nivou tog razreda veci od minimalan_prosjek
	kandidatu se salje SMS sa porukom: "svaka cast za uspjeh 4.X". slanje poruka i emailova implemenitrati koristeci
	zasebne thread-ove.
	*/

	Uspjeh * u = jasmin(PRVI);//vraca uspjeh kandidata ostvaren u prvom razredu
	if (u != nullptr)
		cout << *u << endl;

	system("pause>0");
}