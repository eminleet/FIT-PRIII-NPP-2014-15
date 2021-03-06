#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <exception>
#include <thread>
#include <chrono>
#include <mutex>
#include <cmath>

using namespace std;

const char *crt = "\n---------------------------------------\n";

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
	DatumVrijeme(const DatumVrijeme& drugo) {
		_dan = new int(*drugo._dan);
		_mjesec = new int(*drugo._mjesec);
		_godina = new int(*drugo._godina);
		_sati = new int(*drugo._sati);
		_minuti = new int(*drugo._minuti);
	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;

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
	bool operator==(const DatumVrijeme& drugo) {
		return *_dan == *drugo._dan && *_mjesec == *drugo._mjesec && *_godina == *drugo._godina && *_sati == *drugo._sati;
	}
	friend ostream& operator<< (ostream &COUT, DatumVrijeme &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
		return COUT;
	}
};

template<class T1, class T2 = int>
class FITKolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutno;
public:
	FITKolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	~FITKolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
	void Dodaj(T1& el1, T2& el2) {
		T1 * temp1 = new T1[_trenutno + 1];
		T2 * temp2 = new T2[_trenutno + 1];

		for (int i = 0; i < _trenutno; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno++] = el2;
	}
	void Remove(int lokacija) {
		T1 * temp1 = new T1[_trenutno - 1];
		T2 * temp2 = new T2[_trenutno - 1];

		unsigned int l = 0;
		for (int i = 0; i < _trenutno; i++) {
			if (i == lokacija) continue;
			temp1[l] = _elementi1[i];
			temp2[l++] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;	

		_trenutno--;
	}
	void DodajNaLokaciju(T1& el1, T2& el2, int indeks) {
		T1 * temp1 = new T1[_trenutno + 1];
		T2 * temp2 = new T2[_trenutno + 1];

		for (int i = 0; i < indeks; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}

		temp1[indeks] = el1;
		temp2[indeks] = el2;

		for (int i = indeks + 1; i < _trenutno + 1; i++) {
			temp1[i] = _elementi1[i - 1];
			temp2[i] = _elementi2[i - 1];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_trenutno++;
	}
	FITKolekcija<T1, T2>* GetJedinstveni() {
		FITKolekcija<T1, T2>* kolekcija = new FITKolekcija<T1, T2>;
		for (int i = 0; i < _trenutno; i++) {
			kolekcija->Dodaj(_elementi1[i], _elementi2[i]);
			if (_elementi1[i] == _elementi1[i + 1] && _elementi2[i] == _elementi2[i + 1])
				i++;
		}

		return kolekcija;
	}
	T1 * GetT1() { return _elementi1; }
	T2 * GetT2() { return _elementi2; }
	int GetTrenutno() { return _trenutno; }
	void Sortiraj() {
		int brProlaza = _trenutno - 1;
		bool promjena = true;
		while (promjena) {
			promjena = false;
			for (int i = 0; i < brProlaza; i++) {
				if (_elementi1[i] > _elementi1[i + 1]) {
					swap(_elementi1[i], _elementi1[i + 1]);
					swap(_elementi2[i], _elementi2[i + 1]);
					promjena = true;
				}
			}
			brProlaza--;
		}
	}
	FITKolekcija& operator=(const FITKolekcija& druga) {
		if (this != &druga) {
			delete[] _elementi1;
			delete[] _elementi2;

			_trenutno = druga._trenutno;
			_elementi1 = new T1[druga._trenutno];
			_elementi2 = new T2[druga._trenutno];

			for (int i = 0; i < druga._trenutno; i++) {
				_elementi1[i] = druga._elementi1[i];
				_elementi2[i] = druga._elementi2[i];
			}
		}
		return *this;
	}
	friend ostream& operator<< (ostream &COUT, FITKolekcija &obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
		return COUT;
	}
};

enum vrstaDokumenta { PDF, DOC, TXT, HTML };

class Dokument {
	vrstaDokumenta _vrsta;
	string _naziv;
	string _sadrzaj;
	int _brojStranica;
public:
	Dokument() { _brojStranica = 0; }
	Dokument(vrstaDokumenta vrsta, string naziv) {
		_naziv = naziv; _vrsta = vrsta; _brojStranica = 0;
	}
	void DodajSadrzaj(string noviSadrzaj) {
		float brStranica = noviSadrzaj.length() / 30.;
		_brojStranica += ceil(brStranica);
		_sadrzaj += noviSadrzaj;
	}
	string GetNaziv() { return _naziv; };
	string GetSadrzaj() { return _sadrzaj; };
	vrstaDokumenta GetVrsta() { return _vrsta; };
	void UvecajBrojStranica() { _brojStranica++; }
	int GetBrojStranica() { return _brojStranica; }
	friend ostream & operator<<(ostream&COUT, Dokument & obj) {
		COUT << obj._naziv << "." << obj._vrsta << " (" << obj._brojStranica << ")\n" << obj._sadrzaj << endl;
		return COUT;
	}
};
class Uredjaj {
protected:
	char * _proizvodjac;
	char * _model;
public:
	Uredjaj(const char * proizvodjac, const char * model) {
		int vel = strlen(proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, proizvodjac);
		vel = strlen(model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, model);
	}
	Uredjaj(const Uredjaj& drugi) {
		int vel = strlen(drugi._proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, drugi._proizvodjac);
		vel = strlen(drugi._model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, drugi._model);
	}
	~Uredjaj() {
		delete[] _proizvodjac; _proizvodjac = nullptr;
		delete[] _model; _model = nullptr;
	}
	friend ostream & operator<<(ostream&COUT, Uredjaj & obj) {
		COUT << obj._proizvodjac << "." << obj._model << endl;
		return COUT;
	}
};
mutex cuvarResursa;
class Printer : public Uredjaj {
	FITKolekcija<DatumVrijeme, Dokument> _printaniDokumenti;
	vector<string> _zabranjeneRijeci;
	void Printanje(string sadrzaj) {
		cuvarResursa.lock();
		for (int i = 0; i < sadrzaj.length(); i++) {
			if (i == 30) {
				this_thread::sleep_for(chrono::seconds(2));
			}
			cout << sadrzaj[i];
		}
		cout << endl;
		cuvarResursa.unlock();
	}
public:
	Printer(const char * proizvodjac, const char * model) : Uredjaj(proizvodjac, model) {

	}
	Printer(const Printer& drugi):Uredjaj(drugi) {
		_printaniDokumenti = drugi._printaniDokumenti;
		_zabranjeneRijeci = drugi._zabranjeneRijeci;
	}
	void DodajZabranjenuRijec(string rijec) {
		vector<string>::iterator it = _zabranjeneRijeci.begin();
		while (it != _zabranjeneRijeci.end()) {
			if (*it == rijec)
				return;
			it++;
		}

		_zabranjeneRijeci.push_back(rijec);
	}
	void Printaj(DatumVrijeme& datumVrijeme, Dokument& dokument) {
		_printaniDokumenti.Dodaj(datumVrijeme, dokument);

		string pravilo1 = "([a-zA-Z]{5,15})([.]{1})(pdf|PDF|doc|DOC|txt|TXT|html|HTML)";

		string pravilo2 = "";
		for (int i = 0; i < _zabranjeneRijeci.size() - 1; i++) {
			pravilo2 += _zabranjeneRijeci[i] + "|";
		}
		pravilo2 += _zabranjeneRijeci[_zabranjeneRijeci.size() - 1];

		if (!regex_match(dokument.GetNaziv(), regex(pravilo1)) || regex_search(dokument.GetSadrzaj(), regex(pravilo2))) {
			throw exception("Sadrzaj sadrzi zabranjenu rijec ili ne odgovara trazenom formatu!");
		}
		else {
			thread nit1(&Printer::Printanje, this, dokument.GetSadrzaj());
			nit1.join();
		}
	}
	int GetProsjecanBrojStranicaPoDatumu(DatumVrijeme& datumVrijeme) {
		int printanoStranicaNaDatum = 0;

		for (int i = 0; i < _printaniDokumenti.GetTrenutno(); i++) {
			if (datumVrijeme == _printaniDokumenti.GetT1()[i]) {
				printanoStranicaNaDatum += _printaniDokumenti.GetT2()[i].GetBrojStranica();
			}
		}

		return printanoStranicaNaDatum;
	}
	void UkloniDokumente() {
		string pravilo1 = "([a-zA-Z]{5,15})([.]{1})(pdf|PDF|doc|DOC|txt|TXT|html|HTML)";

		string pravilo2 = "";
		for (int i = 0; i < _zabranjeneRijeci.size() - 1; i++) {
			pravilo2 += _zabranjeneRijeci[i] + "|";
		}
		pravilo2 += _zabranjeneRijeci[_zabranjeneRijeci.size() - 1];

		for (int i = 0; i < _printaniDokumenti.GetTrenutno(); i++) {
			if (!regex_match(_printaniDokumenti.GetT2()[i].GetNaziv(), regex(pravilo1)) || 
				regex_search(_printaniDokumenti.GetT2()[i].GetSadrzaj(), regex(pravilo2))) {
				_printaniDokumenti.Remove(i);
			}
		}
	}
	string GetTopZabranjenuRijec() {
		int* nizRijeci = new int[_zabranjeneRijeci.size()];
		for (int i = 0; i < _zabranjeneRijeci.size(); i++) {
			nizRijeci[i] = 0;
		}

		string pravilo = "";
		for (int i = 0; i < _zabranjeneRijeci.size(); i++) {
			pravilo = _zabranjeneRijeci[i];
			for (int j = 0; j < _printaniDokumenti.GetTrenutno(); j++) {
				if (regex_search(_printaniDokumenti.GetT2()[j].GetSadrzaj(), regex(pravilo))) {
					nizRijeci[i]++;
				}
			}
		}

		int maxPuta = 0;
		string maxRijec;

		for (int i = 0; i < _zabranjeneRijeci.size(); i++) {
			if (nizRijeci[i] > maxPuta) {
				maxPuta = nizRijeci[i];
				maxRijec = _zabranjeneRijeci[i];
			}
		}

		return maxRijec;
	}
	FITKolekcija<DatumVrijeme, Dokument> & GetPrintaniDokumenti() { return _printaniDokumenti; };
	vector<string> & GetZabranjeneRijeci() { return _zabranjeneRijeci; };
	friend ostream& operator<<(ostream& COUT, Printer& printer);
};
ostream& operator<<(ostream& COUT, Printer& printer) {
	COUT << static_cast<Uredjaj&>(printer);
	COUT << ":: DOKUMENTI :: " << endl << printer._printaniDokumenti << endl;
	COUT << ":: ZABRANJENE RIJECI :: " << endl;
	for (int i = 0; i < printer._zabranjeneRijeci.size()-1; i++) {
		COUT << printer._zabranjeneRijeci[i] << " && ";
	}
	COUT << printer._zabranjeneRijeci[printer._zabranjeneRijeci.size() - 1];
	return COUT;
}

void main()
{
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/

#pragma region TestiranjeDatumVrijeme
	DatumVrijeme danas(1, 2, 2017, 10, 15);
	DatumVrijeme sutra(danas);
	DatumVrijeme prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region FITKolekcija
	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//SORTIRANJE ČLANOVA KOLEKCIJE SE VRŠI U RASTUĆEM REDOSLIJEDU NA OSNOVU VRIJEDNOSTI ELEMENTA TIPA T1
	brojevi.Sortiraj();
	cout << brojevi << crt;
	//BROJ 2 SE ODNOSI NA LOKACIJU/INDEKS UNUTAR LISTE NA KOJI JE POTREBNO DODATI NOVE ELEMENTE
	brojevi.DodajNaLokaciju(v109, v6, 2);
	cout << brojevi << crt;
	brojevi.Sortiraj();
	cout << brojevi << crt;
	/*METODA GetJedinstveni VRAĆA LISTU JEDINSTVENIH ELEMENATA TJ. ELEMENATA KOJI NE SADRŽE DUPLIKATE (POJAM DUPLIKAT SE ODNOSI NA ISTE
	VRIJEDNOSTI ELEMENATA T1 I T2). ELEMENTI KOJI SE DUPLIRAJU SE U OVOJ LISTI TREBAJU POJAVITI SAMO JEDNOM.*/
	FITKolekcija<int, int> * jedinstveni = brojevi.GetJedinstveni();
	cout << *jedinstveni << crt;
	*jedinstveni = brojevi;
	cout << *jedinstveni << crt;

	Dokument ispitPRIII(DOC, "ispitPRIII.doc");
	//BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREĐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU STRANICU MOŽE 
	//STATI 30 ZNAKOVA UKLJUČUJUĆI I RAZMAKE
	ispitPRIII.DodajSadrzaj("NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO RE");
	cout << "Broj stranica -> " << ispitPRIII.GetBrojStranica() << endl;
	Dokument ispitBaze(DOC, "ispitBaze.doc");
	ispitBaze.DodajSadrzaj("PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT");
	cout << ispitBaze << endl;//ISPISUJE SVE DOSTUPNE PODATKE O DOKUMENTU

	Printer hp3200("HP", "3200");
	//PRINTER NEĆE DOZVOLITI PRINTANJE DOKUMENATA U ČIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEČI
	hp3200.DodajZabranjenuRijec("RE");// :)
	hp3200.DodajZabranjenuRijec("RAT");
	hp3200.DodajZabranjenuRijec("UBITI");
	hp3200.DodajZabranjenuRijec("RE");// ONEMOGUĆITI PONAVLJANJE ZABRANJENIH RIJEČI, KORISTITI ITERATORE
	hp3200.DodajZabranjenuRijec("MRZITI");
	try
	{
		/*
		DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEĆA PRAVILA:
		1. NAZIV DOKUMENTA MORA BITI U SLJEDEĆEM FORMATU npr: ispitPRIII.doc
		NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE
		BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
		2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEČ
		UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA
		ODGOVARAJUĆOM PORUKOM, ALI ĆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti.
		UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI
		SADRŽAJ DOKUMENTA KOJI SE PRINTA

		*/
		hp3200.Printaj(danas, ispitPRIII);
		hp3200.Printaj(danas, ispitBaze);

		/*
		IMPLEMENTIRATI PRINTANJE U ZASEBNIM NITIMA UZ KORIŠTENJE sleep_for FUNKCIJE
		VODITI RAČUNA O DIJELJENJU ZAJEDNIČKIH RESURSA
		*/

	}
	catch (exception& err)
	{
		cout << err.what() << endl;
	}
	//KOD POREĐENJA DATUMA PRINTANJA NIJE POTREBNO POREDITI MINUTE, SAMO DATUM I SATE
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp3200.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	cout << "Najcesce koristena zabranjena rijec je -> " << hp3200.GetTopZabranjenuRijec() << crt;

	Printer hp4000(hp3200);
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp4000.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	cout << "Najcesce koristena zabranjena rijec je -> " << hp4000.GetTopZabranjenuRijec() << crt;
	hp4000.UkloniDokumente();//UKLANJA SVE DOKUMENTE KOJI NISU ZADOVOLJILI USLOVE ZA PRINTANJE
	cout << hp4000 << crt;

#pragma endregion
	system("PAUSE");
}