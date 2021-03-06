#include<iostream>
#include<string>
#include<vector>
#include <exception>
#include <regex>
#include <mutex>

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

using namespace std;

char * AlocirajNizKaraktera(const char * tekst) {
	if (tekst == nullptr)
		return nullptr;

	int len = strlen(tekst) + 1;
	char * temp = new char[len];
	strcpy_s(temp, len, tekst);
	return temp;
}

enum Oblast { SoftverskiInzinjering, KomunikacijskiSistemi, SigurnostInformacijskihSistema, InteligentniSistemi };
const char* oblast_txt[] = { "Softverski inzinjering", "Komunikacijski sistemi", "Sigurnost informacijskih sistema", "Inteligentni sistemi" };

template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutno;
public:
	Kolekcija()
	{
		_trenutno = 0;
	}

	void AddElement(const T1& elem1, const T2& elem2)
	{
		if (_trenutno == max)
			throw exception("Dostigli ste maksimalan broj elemenata u kolekciji!");

		for (int i = 0; i < _trenutno; i++)
		{
			if (_elementi1[i] == elem1 || _elementi2[i] == elem2)
				throw exception("Nije moguce dodati duple elemente u kolekciju!");
		}

		_elementi1[_trenutno] = elem1;
		_elementi2[_trenutno++] = elem2;

	}

	T1 * getElementi1Pok() { return _elementi1; }
	T2 * getElementi2Pok() { return _elementi2; }
	T1 & getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2 & getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }

	friend ostream & operator<<(ostream & COUT, Kolekcija<T1, T2, max> & obj)
	{
		for (size_t i = 0; i < obj._trenutno; i++)
		{
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		}
		return COUT;
	}
};

//Deklaracija klase Nastavnik omogucava njeno koristenje u klasi ZavrsniRad, a definicija je data naknadno
class Nastavnik;

class ZavrsniRad
{
	char* _nazivTeme;
	Oblast* _oblastTeme;
	string _datumPrijave; //Format: dd.MM.gggg
	//U vector pohraniti samo adresu objekta tipa Nastavnik, dakle bez alokacije nove memorije
	vector<Nastavnik*> _komisija;
	string _datumOdbrane; //Format: dd.MM.gggg (najranije 2 mjeseca od datuma prijave)
	int _ocjena;
public:
	ZavrsniRad() :_nazivTeme(nullptr), _oblastTeme(nullptr), _datumPrijave("NEMA VRIJEDNOST"), _datumOdbrane("NEMA VRIJEDNOST"), _ocjena(5)
	{ }

	ZavrsniRad(string nazivTeme, Oblast oblastTeme, string datumPrijave) : _datumPrijave(datumPrijave), _oblastTeme(new Oblast(oblastTeme))
	{
		_nazivTeme = new char[nazivTeme.size() + 1];
		strcpy_s(_nazivTeme, nazivTeme.size() + 1, nazivTeme.c_str());
	}

	~ZavrsniRad()
	{
		delete[] _nazivTeme; _nazivTeme = nullptr;
		delete _oblastTeme; _oblastTeme = nullptr;
	}
	bool SetOcjenu(int ocjena) {
		if (_datumOdbrane.length() > 1 && _komisija.size() > 0) {
			_ocjena = ocjena;
			return true;
		}
		return false;
	}
	void ZakaziOdbranuRada(string datum, vector<Nastavnik*> komisija) {
		_datumOdbrane = datum;
		_komisija = komisija;
	}
	char* GetNazivTeme()const { return _nazivTeme; }
	Oblast GetOblastTeme()const { return *_oblastTeme; }
	string GetDatumOdbrane() { return _datumOdbrane; }
	string GetDatumPrijave() { return _datumPrijave; }
	int GetOcjena() { return _ocjena; }
	vector<Nastavnik*>& GetKomisija() { return _komisija; };
	bool operator==(const ZavrsniRad& drugi) {
		return strcmp(_nazivTeme, drugi._nazivTeme) == 0 && *_oblastTeme == *drugi._oblastTeme && _datumPrijave == drugi._datumPrijave &&
			_datumOdbrane == drugi._datumOdbrane;
	}
	ZavrsniRad& operator=(const ZavrsniRad& drugi) {
		if (this != &drugi) {
			delete[] _nazivTeme;
			delete _oblastTeme;

			_nazivTeme = AlocirajNizKaraktera(drugi._nazivTeme);
			_oblastTeme = new Oblast(*drugi._oblastTeme);
			_datumPrijave = drugi._datumPrijave;
			_komisija = drugi._komisija;
			_datumOdbrane = drugi._datumOdbrane;
			_ocjena = drugi._ocjena;
		}
		return *this;
	}
	friend ostream& operator<<(ostream& COUT, ZavrsniRad& r)
	{
		COUT << "Tema rada: " << r._nazivTeme << endl;
		COUT << "Oblast teme: " << oblast_txt[*r._oblastTeme] << endl;
		COUT << "Datum prijave rada: " << r._datumPrijave << endl;
		//Podatke o nastavnicima nije moguce ispisati ovdje jer klasa jos nije definisana
		return COUT;
	}
};

int GetDan(string datum) {
	char temp[5];
	temp[0] = datum[0];
	temp[1] = datum[1];
	return atoi(temp);
}
int GetMjesec(string datum) {
	char temp[5];
	temp[0] = datum[3];
	temp[1] = datum[4];
	return atoi(temp);
}
int GetGodina(string datum) {
	char temp[5];
	temp[0] = datum[6];
	temp[1] = datum[7];
	temp[2] = datum[8];
	temp[3] = datum[9];
	return atoi(temp);
}
bool ManjiDatum(string datum1, string datum2) {
	int d1 = GetDan(datum1), d2 = GetDan(datum2);
	int m1 = GetMjesec(datum1), m2 = GetMjesec(datum2);
	int g1 = GetGodina(datum1), g2 = GetGodina(datum2);

	if (g1 < g2) return true;
	if (g1 == g2 && m1 < m2) return true;
	if (g1 == g2 && m1 == m2 && d1 < d2) return true;
	return false;
}
class Nastavnik
{
	string _imePrezime;
	Oblast _oblastIzboraUZvanje;
	//Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog nastavnika
	Kolekcija<string, ZavrsniRad, 10> _teme;

public:
	Nastavnik(string imePrezime, Oblast oblastIzboraUZvanje) : _imePrezime(imePrezime), _oblastIzboraUZvanje(oblastIzboraUZvanje)
	{}
	void DodajZavrsniRad(string indeks, string naslov, Oblast oblast, string datumPrijave) {
		if (_teme.getTrenutno() >= 10) return;

		for (int i = 0; i < _teme.getTrenutno(); i++) {
			if (_teme.getElement2(i).GetNazivTeme() == naslov)
				return;
		}

		if (_oblastIzboraUZvanje != oblast)
			throw exception("Nastavnik ne posjeduje potreban izbor u zvanje!");

		_teme.AddElement(indeks, ZavrsniRad(naslov, oblast, datumPrijave));
	}
	void ZakaziOdbranuRada(string indeks, string datum, vector<Nastavnik*> komisija) {
		int foundIndeks = -1;
		for (int i = 0; i < _teme.getTrenutno(); i++) {
			if (_teme.getElement1(i) == indeks)
				foundIndeks = i;
		}
		if (foundIndeks == -1)
			throw exception("Student nije pronadjen!");

		int brOblasti = 0;
		for (int i = 0; i < komisija.size(); i++) {
			if (komisija[i]->GetOblast() == _teme.getElement2(foundIndeks).GetOblastTeme())
				brOblasti++;
		}
		if (brOblasti < 2)
			throw exception("Barem 2 clana komisije trebaju imati potreban izbor u zvanje!");

		if (ManjiDatum(datum, _teme.getElement2(foundIndeks).GetDatumPrijave()))
			throw exception("Ne moze datum odbrane biti manji od datuma prijave!");

		_teme.getElement2(foundIndeks).ZakaziOdbranuRada(datum, komisija);
	}
	string GetImePrezime() { return _imePrezime; }
	Oblast GetOblast() { return _oblastIzboraUZvanje; }
	Kolekcija<string, ZavrsniRad, 10>& GetTeme() { return _teme; };
	bool operator()(string indeks, int ocjena) {
		for (int i = 0; i < _teme.getTrenutno(); i++) {
			if (_teme.getElement1(i) == indeks) {
				return _teme.getElement2(i).SetOcjenu(ocjena);
			}
		}
		return false;
	}
	friend ostream& operator<<(ostream& COUT, Nastavnik& obj);
	float GetProsjek() {
		float ukupnoOcjena = 0;
		int brOcjena = 0;
		float prosjek = 0;
		for (int i = 0; i < _teme.getTrenutno(); i++) {
			ukupnoOcjena += _teme.getElement2(i).GetOcjena();
			brOcjena++;
		}
		prosjek = ukupnoOcjena / brOcjena;
		return prosjek;
	}
};
ostream& operator<<(ostream& COUT, Nastavnik& obj) {
	COUT << endl << endl << "Ime i prezime nastavnika: " << obj._imePrezime << endl;
	COUT << "Oblast: " << oblast_txt[obj._oblastIzboraUZvanje] << endl;
	COUT << ":: MENTORSTVA ::" << endl;
	
	Kolekcija<string, ZavrsniRad, 10>& teme = obj._teme;
	for (int i = 0; i < teme.getTrenutno(); i++) {
		COUT << endl << "Indeks studenta: " << teme.getElement1(i) << endl;
		COUT << teme.getElement2(i) << endl;
		COUT << ": KOMISIJA : " << endl;
		for (int j = 0; j < teme.getElement2(i).GetKomisija().size(); j++) {
			COUT << teme.getElement2(i).GetKomisija()[j]->_imePrezime << endl;
		}
	}
	return COUT;
}

mutex cuvarResursa;
void SendEmail(string TO, string tekst) {
	cuvarResursa.lock();
	cout << endl << "TO: " << TO << endl;
	cout << "Postovani, \n" << tekst << endl;
	cuvarResursa.unlock();
}
string PronadjiNajStudenta(Nastavnik* nastavnici[], const int max) {
	string imePrezimeNastavnika;
	int indeksNastavnika = -1;
	float maxProsjekNastavnik = INT_MIN;
	for (int i = 0; i < max; i++) {
		float prosjek = nastavnici[i]->GetProsjek();
		if (prosjek > maxProsjekNastavnik) {
			imePrezimeNastavnika = nastavnici[i]->GetImePrezime();
			maxProsjekNastavnik = prosjek;
			indeksNastavnika = i;
		}
	}
	
	if (indeksNastavnika == -1)
		return "NIJE PRONADJEN";

	string brIndeksa;
	float maxProsjekUcenik = INT_MIN;
	for (int i = 0; i < nastavnici[indeksNastavnika]->GetTeme().getTrenutno(); i++) {
		int ocjena = nastavnici[indeksNastavnika]->GetTeme().getElement2(i).GetOcjena();
		string indeks = nastavnici[indeksNastavnika]->GetTeme().getElement1(i);
		if (ocjena > maxProsjekUcenik) {
			thread nit1(SendEmail, indeks + "@edu.fit.ba", "Odbranili ste rad ocjenom vecom od prosjecne!");
			nit1.join();
			brIndeksa = indeks;
			maxProsjekUcenik = ocjena;
		}
		else {
			thread nit1(SendEmail, indeks + "@edu.fit.ba", "Odbranili ste rad ocjenom manjom od prosjecne!");
			nit1.join();
		}
	}

	return brIndeksa;

	return brIndeksa;
}
void main()
{
	const int max = 4;
	Nastavnik* nastavnici[max];

	nastavnici[0] = new Nastavnik("Denis Music", SoftverskiInzinjering);
	nastavnici[1] = new Nastavnik("Zanin Vejzovic", KomunikacijskiSistemi);
	nastavnici[2] = new Nastavnik("Jasmin Azemovic", SigurnostInformacijskihSistema);
	nastavnici[3] = new Nastavnik("Emina Junuz", SoftverskiInzinjering);

	try
	{
		/*Funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. Sprijeciti dodavanje zavrsnih 
		radova sa istom temom vise puta. Nastavnik moze imati (mentorisati) samo radove iz oblasti za koju posjeduje izbor u zvanje.
		U slucaju da se nastavniku pokusa dodati rad koji nije iz njegove oblasti funkcija treba da baci izuzetak sa odgovarajucom porukom */

		//indeks, naslov, oblast, datum prijave
		nastavnici[0]->DodajZavrsniRad("IB130011", "Multimedijalni informacijski sistem za visoko-obrazovnu ustanovu", SoftverskiInzinjering, "01.04.2017");
		nastavnici[0]->DodajZavrsniRad("IB120051", "Sistem za podršku rada kablovskog operatera", SoftverskiInzinjering, "03.03.2017");

		nastavnici[1]->DodajZavrsniRad("IB140102", "Praktična analiza sigurnosti bežičnih računarskih mreža", KomunikacijskiSistemi, "22.08.2017");

		nastavnici[2]->DodajZavrsniRad("IB140002", "Primjena teorije informacija u procesu generisanja kriptografskih ključeva", SigurnostInformacijskihSistema, "10.09.2017");

		vector<Nastavnik*> komisija;//formira se komisija
		komisija.push_back(nastavnici[0]);
		komisija.push_back(nastavnici[2]);
		komisija.push_back(nastavnici[3]);

		/*Funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva 
		definisanje datuma odbrane i liste clanova komisije pred kojima ce student braniti zavrsni rad. Odbrana rada se moze zakazati samo 
		studentu koji je rad prethodno prijavio. Komisiju trebaju ciniti najmanje 2 nastavnika koji imaju izbor u zvanje u oblasti kojoj 
		pripada tema rada. Datum odbrane ne smije biti manji od datuma prijave. U slucaju da bilo koji od navedenih uslova nije ispunjen 
		funkcija treba da baci izuzetak*/
		nastavnici[0]->ZakaziOdbranuRada("IB130011", "25.09.2017", komisija);
		//nastavnici[0]->ZakaziOdbranuRada("IB130111", "25.09.2017", komisija);//student sa brojem indeksa IB130111 jos uvijek nije prijavio rad

		/*Studentu sa brojem indeksa IB130011 dodjeljuje ocjenu 8 na zavrsnom radu. Uslov za dodjelu ocjene je da student posjeduje definisan
		datum odbrane i listu clanova komisije. U zavisnosti od uspjesnosti izvrsenja, funkcija vraca true ili false*/

		if ((*nastavnici[0])("IB130011", 8))
			cout << "Uspjesno ste ocijenili zavrsni rad!" << endl;

		/*Ispisuje sve podatke o nastavniku i njegovim mentorstvima. Za clanove komisije je dovoljno ispisati samo ime i prezime.*/
		cout << *nastavnici[0] << endl;

		/*Funkcija PronadjiNajStudenta ima zadatak da pronadje broj indeksa studenta koji je na zavrsnom radu ostvario najvecu ocjenu kod 
		nastavnika koji posjeduje najvecu prosjecnu ocjenu na zavrsnim radovima. Ukoliko se kod nastavnika sa najvecom prosjecnom ocjenom 
		pojavi vise studenata sa istom ocjenom, onda funkcija vraca broj indeksa prvog pronadjenog studenta. Svim studentima koji su odbranili 
		rad kod nastavnika sa najvecom prosjecno ocjenom, u zasebnom thread.u, poslati email poruku (mail adresa: brojIndeksa@edu.fit.ba) sa 
		sadrzajem da su svoj zavrsni rad uspjesno odbranili sa vecom ili manjom ocjenom od prosjecne. Ukoliko niti jedan od nastavnika ne 
		posjeduje evidentirano mentorstvo na zavrsnom radu, funkcija vraca tekst: NIJE PRONADJEN*/

		cout << "Najsupjesniji student: " << PronadjiNajStudenta(nastavnici, max) << endl;

		//Baca izuzetak zbog neadekvatnog izbora u zvanje, odnosno oblasti
		nastavnici[2]->DodajZavrsniRad("IB150008", "Razvoj sistema autentifikacije na osnovu biometrije glasa", InteligentniSistemi, "15.05.2017");
	}
	catch (exception& ex)
	{
		cout << "GRESKA -> " << ex.what() << endl;
	}

	for (int i = 0; i < max; i++)
	{
		delete nastavnici[i];
		nastavnici[i] = nullptr;
	}

	system("PAUSE");
}