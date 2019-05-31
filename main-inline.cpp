/**
    Cerinte comune tuturor temelor:
        1. tratarea exceptiilor
        2. utilizarea variabilelor și functiilor statice
        3. utilizarea conceptelor de Dynamic_Cast și Static_Cast
        4. utilizarea sabloanelor
        5. citirea informațiilor complete a n obiecte, memorarea și afisarea acestora

    Cerinte generale aplicate fiecarei teme din acest fisier:
        - să se identifice și să se implementeze ierarhia de clase
        - clasele sa conțină constructori, destructori, =, funcție prietena de citire a datelor
        - clasa de baza sa conțină funcție virtuala de afisare, rescrisa în clasele derivate
        - clasele derivate trebuie sa contina constructori parametrizati prin care sa se
                 evidentieze transmiterea parametrilor către constructorul din clasa de baza

    Tema 1. Firma X are un domeniu de business unde este necesar să se urmărească modul în
        care clientii plătesc (numerar, cec sau card de credit). Indiferent de modul de plata,
        firma X știe în ce data s-a efectuat plata și ce suma a fost primita. Dacă se plătește
        cu cardul, atunci se cunoaște și numărul cardului de credit. Pentru cash, nu e necesara
        identificarea clientului care a făcut plata.

    Cerinta suplimentara:
        - Sa se construiasca clasa template Gestiune care sa conțină numărul total de plati
        (incrementat automat la adaugarea unei noi chitante) și un vector de pointeri la obiecte
        de tip Plata, alocat dinamic. Sa se supraincarce operatorul += pentru inserarea unei
        plati în lista, indiferent de tipul acesteia
        - Sa se construiasca o specializare pentru tipul char* care sa stocheze numarul de clienti,
        impreuna cu numele acestora

    Plata: data, suma
      - Anonima = numerar: adresa
      - Client: nume, prenume
          - Cec: cnp
          - Card: id, cvv
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <typeinfo>

using namespace std;

struct Date
{
    int year , month, day; ///, hour 20, minute = 41, second = 47
    Date (int y = 2019, int m = 5, int d = 7): year (y), month (m), day (d) {test();}
    ~Date() {}
    Date & operator= (const Date & other) {year = other.year; month = other.month; day = other.day; return *this;}
    friend istream & operator>> (istream & in, Date & d);
    friend ostream & operator<< (ostream & out, const Date & d);
    void test();
};
void Date::test ()
{
    try {
        if (year <= 0 || month <= 0 || day <= 0) throw '1';
        if (month > 12) throw 1.0;
        if (month == 4 || month == 6 || month == 9 || month == 1) {if (day > 30) throw 1;}
        else if (month != 2) {if (day > 31) throw 1;}
            else if(day > 28 + (year%4 == 0 && year % 100 != 0)) throw 1;
    }
    catch (char c) {cout << "Negative values...\n";}
    catch (double d) {cout << "It's a new year...\n";}
    catch (int i) {cout << "It's a new month...\n";}
    catch (...) {cout << "Unknown Error\n";}
}
istream & operator>> (istream & in, Date & d) {in >> d.year >> d.month >> d.day; d.test(); return in;}
ostream & operator<< (ostream & out, const Date & d) {return (out << d.year << " " << d.month << " " << d.day);}

template <class TS, class TD> class Plata;
template <class TS, class TD> istream & operator>> (istream & in, Plata <TS, TD> & p) {return (in >> p.sum >> p.date);}
template <class TS, class TD> class Plata
{
    TS sum;
    TD date;
public:
    inline Plata (TS s, TD d) {sum = s; date = d;}
    virtual ~Plata () {}
    inline Plata <TS, TD> & operator= (const Plata <TS, TD> & other) {sum = other.sum; date = other.date; return *this;}
    friend istream & operator>> <TS, TD> (istream & in, Plata <TS, TD> & p);
    //virtual void print (ostream & out, const Plata <TS, TD> & p) = 0;
    TS getS () {return sum;}
    TD getD () {return date;}
};
template <> class Plata <int, char*>
{
    int sum;
    char* date;/// max: yyyy/mm/dd
public:
    inline Plata (int s, char* d) {sum = s; date = new char [17]; strcpy(date, "asedf");}
    ~Plata () {delete date;}
    inline Plata <int, char*> & operator= (const Plata <int, char*> & other) {sum = other.sum; strcpy(date,other.date); return *this;}
    friend istream & operator>> <int, char*> (istream & in, Plata <int, char*> & p);
    virtual void print (ostream & out, const Plata <int, char*> & p) = 0;
};

template <class TS, class TD> class Anonym;
template <class TS, class TD> istream & operator>> (istream & in, Anonym <TS, TD> & p) {return (in >> (static_cast <Plata <TS, TD> &> (p)) >> p.location.first >> p.location.second);}
template <class TS, class TD> class Anonym: public Plata <TS, TD>
{
    pair <int, int> location;
public:
    Anonym (TS s, TD d, pair <int, int> l = {0, 0}): Plata <TS, TD> (s, d) {location = l;}
    ~Anonym () {}
    Anonym <TS, TD> & operator= (const Anonym <TS, TD> & other) {static_cast <Plata <TS, TD> &> (*this) = other; location = other.location; return *this;}
    friend istream & operator>> <TS, TD> (istream & in, Anonym <TS, TD> & p);
    virtual void print (ostream & out, Plata <TS, TD> & p)
    {
        Anonym *a = dynamic_cast <Anonym*> (&p);
        out << a->getS() << "$ payed on " << a->getD() << " at location: " << a->location.first << " " << a->location.second << "\n";
    }
};

template <class TS, class TD> class ByPerson;
template <class TS, class TD> istream & operator>> (istream & in, ByPerson <TS, TD> & p) {return (in >> (static_cast <Plata <TS, TD> &> (p)) >> p.surname >> p.name);}
template <class TS, class TD> class ByPerson: public Plata <TS, TD>
{
    string name, surname;
public:
    ByPerson (TS s, TD d, string nam = "", string sur = ""): Plata <TS, TD> (s, d) {name = nam; surname = sur;}
    ~ByPerson () {}
    ByPerson <TS, TD> & operator= (const ByPerson <TS, TD> & other) {static_cast <Plata <TS, TD> &> (*this) = other; name = other.name; surname = other.surname; return *this;}
    friend istream & operator>> <TS, TD> (istream & in, ByPerson <TS, TD> & p);
    //virtual void print (ostream & out, const Plata <TS, TD> & p) = 0;
    string getName() {return name;}
    string getSurname() {return surname;}
};

template <class TS, class TD> class Cec;
template <class TS, class TD> istream & operator>> (istream & in, Cec <TS, TD> & p) {return (in >> (static_cast <ByPerson <TS, TD> &> (p)) >> p.cnp);}
template <class TS, class TD> class Cec: public ByPerson <TS, TD>
{
    string cnp;
public:
    Cec (TS s, TD d, string nam = "", string sur = "", string c = ""): ByPerson <TS, TD> (s, d, nam, sur) {cnp = c;}
    ~Cec () {}
    Cec <TS, TD> & operator= (const Cec <TS, TD> & other) {static_cast <ByPerson <TS, TD> &> (*this) = other; cnp = other.cnp; return *this;}
    friend istream & operator>> <TS, TD> (istream & in, Cec <TS, TD> & p);
    virtual void print (ostream & out, Plata <TS, TD> & p)
    {
        Cec *c = dynamic_cast <Cec*> (&p);
        out << c->getS() << "$ payed on " << c->getD() << " by: " << c->getSurname() << " " << c->getName() << " with CNP: " << c->cnp << "\n";
    }
};

template <class TS, class TD> class Card;
template <class TS, class TD> istream & operator>> (istream & in, Card <TS, TD> & p) {return (in >> (static_cast <ByPerson <TS, TD> &> (p)) >> p.id >> p.cvv);}
template <class TS, class TD> class Card: public ByPerson <TS, TD>
{
    string id;
    int cvv;
public:
    Card (TS s, TD d, string nam = "", string sur = "", string i = "", int c = 0): ByPerson <TS, TD> (s, d, nam, sur) {id = i; cvv = c;}
    ~Card () {}
    Card <TS, TD> & operator= (const Card <TS, TD> & other) {static_cast <ByPerson <TS, TD> &> (*this) = other; id = other.id; cvv = other.cvv; return *this;}
    friend istream & operator>> <TS, TD> (istream & in, Card <TS, TD> & p);
    virtual void print (ostream & out, Plata <TS, TD> & p)
    {
        Card *c = dynamic_cast <Card*> (&p);
        out << c->getS() << "$ payed on " << c->getD() << " by: " << c->getSurname() << " " << c->getName() << " with card ID: " << c->id << " and CVV: " << c->cvv << "\n";
    }
};

class Lista
{
    Plata <int, Date> * info;
    Lista *next;
    static int objCount;
public:
    static void add(int n = 1) {objCount += n;}
    static int get() {return objCount;}
    Lista (Plata <int, Date> * i = NULL, Lista *n = NULL): info(i), next(n) {add(1);}
    ~Lista () {add(-1); delete info; delete next;}
    Lista& operator+= (Plata <int, Date> *p);
    void af();
};
int Lista:: objCount = -1;
Lista& Lista:: operator+= (Plata <int, Date> *p)
{
    Lista *i = this;
    while (i->next != NULL) {i = i->next;}
    i->next = new Lista (p);
    return *this;
}
void Lista:: af()
{
    Lista* i = this->next;
    Date d;
    Anonym <int, Date> obop1 (0, d);
    Cec <int, Date> obop2 (0, d);
    Card <int, Date> obop3 (0, d);
    while (i != NULL) {
        if (typeid(*i->info) == typeid(obop1)) {obop1.print(cout, *i->info);}
        else if (typeid(*i->info) == typeid(obop2)) {obop2.print(cout, *i->info);}
            else if (typeid(*i->info) == typeid(obop3)) {obop3.print(cout, *i->info);}
                else {cout << "Unknown\n";}
        i = i->next;
    }
}

int main()
{
    ifstream in ("date.in");
    Date d;
    int n, i, option;
    Lista *l = new Lista; /// that's why I add 1 when I print the nr of objects at the end
    Anonym <int, Date> *obop1;
    Cec <int, Date> *obop2;
    Card <int, Date> *obop3;
    in >> n;

    cout << "\nnr of objects in list: " << Lista::get() << "\n";
    for (i = 0; i < n; ++i)
    {
        in >> option;
        switch (option)
        {
            case 1:
                obop1 = new Anonym <int, Date>(0, d);
                in >> (*obop1);
                (*l) += static_cast <Plata <int, Date> *>  (obop1);
                break;
            case 2:
                obop2 = new Cec <int, Date>(0, d);
                in >> (*obop2);
                (*l) += static_cast <Plata <int, Date> *>  (obop2);
                break;
            case 3:
                obop3 = new Card <int, Date>(0, d);
                in >> (*obop3);
                (*l) += static_cast <Plata <int, Date> *>  (obop3);
                break;
            default:
                cout << "Unkown type of payment";
                delete l;
                return 0;
        }
    }
    cout << "\nnr of objects in list: " << Lista::get() << "\n";
    l->af();
    delete l;
    cout << "\nnr of objects in list: " << Lista::get() + 1 << "\n";
    return 0;
}
