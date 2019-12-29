/** BinBigInt (2019.12.26)
*
* binbigint.cpp
* Copyright (C) 2019 - Filip Roland - git@filiproland.hu
*
* BinBigInt is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, version 3 of the License.
*
* BinBigInt is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
* EN
* This is a binary big integer object and it's arithmetic, bit-manipulating and comparison operations base function's
* and some other function's (power, factorial, positive product, string <-> BinBigInt conversion) implementation library.
*
* The binary big integer object implementation based on an unsigned long long int ("ulli") typed vector (std::vector<ulli>) and an unsigned char typed signum member variable.
*
* The binary big integer object can be defined by / their constructors can called with parameters mentioned below:
*
*   unsigned long long int value (This is the default constructor too so if you instantiates without parameter this constructor called)
*   signed long long int value
*   std::vector<ulli> vector and a char signum values
*   BinBigInt value
*   std::string value, unsigned char base of number format , std::string separate string value and ulli separate width
*
*       With the use of string based instantiation possible to choose the base of number format 2-35, you can use the common numeric literal formats and
*       any separate string an width, the use of the common numeric format prefixes is accepted and the common base of numeric formats (radixs) predefined as enum{BIN=2,OCT=8,DEC=10,HEX=16}
*
* For the representation the result there is a conversion function from BinBigInt to string which variable like the string based instantiates function.
*
* The negative numbers should set by negative signum and their absolute value in every case of instantiation. (So neither of constructors and number formats require the
* two's complement of the number) The constructor provide the two's complement of the negative number and store the two's complement of the negative number into the vector.
* (In the case if you manipulate the raw data of the vector of course you should take count with the two's complement of the negative number)
* The negative zero not represented. The instantiation with negative zero is interpreted and stored as + 0 by the constructors.
* Any set or result of the all bit zero raw vector value with a negative signum (except the instantiation with any constructor) resulting the smallest possible number which can represented on the
* actual vector size (=-(2^n+1) where n is the counts of the whole vector's data bits.)
*
* The object's both member variables and all of the functions are public and each base version of the functions directly manipulate the member variables so beside the BinBigInt parameterized wrapper
* functions anytime is possible to handle the datas directly as vectors and the signum can access directly too, so essentially all of the functions are callable with vector values and unsigned char signum parameters too.
*
* The sift left biShl() function possible to call with extend and truncate behavior, for these two version there are two explicit named wrapper functions too.
*
* The "over-/underflow" and "carry" implementation differs in several aspects from the ISA kind of flag implementation!!!
* The "over-/underflow" in the case of the bit shift shifter functions interpreted for the whole range of the steps, the OF for the most significant position overflow and UF at right shift for the least significant position underflow.
* TH OF/UT then become to 1 if the operation or in the chase of shifting until the whole range of the steps cause to step out an opposite value bit as the signum's value interpreted to the original vector size, so at case of + number
* step out 1 and at case of - number step out 0.
* (e.g. at left shift called with extend behavior the function result overflow to 1 if the effective size of the vector is grow because it mean the original vector size were "overflowed".)
* The cl (CarryLast) in all case set to the same as the last stepped out bit, independently from the signum's value.
*
* To avoiding unnecessary memory allocation at default case the functions provide the result through references which they get as parameters and allways if it possible they get the other parameters trough (constant) references.
* (This cause that the most of the base function returns void "function values" except where they provide e.g. bool, carry or overflow (or other not vector oe not BinBigInt) value returned)
*
* That functions which provide BinBigInt result and not binded for any operator and that have meaning has got wrapper function version which's returned function value is BinBigInt.
* That functions which provide BinBigInt result and binded for any operator haSn't got distinct wrapper function version which's returned function value is BinBigInt. (The operator function available for this purpose.)
*/

/**
* HUN
* Ez egy bináris big integer objektum és annak aritmetikai, bit maipulációs és összehasonlító műveleti alap függvényeinek
* és néhány további függvény (hatvány, faktoriális, pozitív produktum, string <-> BinBigInt konverzió) megvalósítását tartalmazó könyvtár
*
* A bináris big integer objektum megvalósításának alapját egy unsigned long long int ("ulli") típusú vektor (std::vector<ulli>) és egy unsigned char típusú előjel tagváltozó képezi
*
* A bigint objektum az alábbiak szerint definiálható / a konstruktorai az alábbi típusú paraméterekkel hívhatók meg:
*
*   unsigned long long int értékkel (Ez az alapértelmezett konstruktor is egyben így a paraméter nélkli példányosítás ezt hívja meg mely esetben 0-ra inicializál)
*   signed long long int értékkel
*   std::vector<ulli> vektorral és char előjel értékkel
*   BinBigInt értékkel
*   std::string értékkel, unsigned char számformátum alappal, std::string tagoló sztring értékel és ulli tagolsági szélességgel
*
*       A string alapú példányosítás során 2-35-ig tetszőleges alapú számformátum használható, a hagyományos numerikus literáloknak megfelelő formátumok és tetszőleges tagoló karakterek illetve
*       tagolási széleség is használható, a szokásos prefixek használhatók, a hagyományos számrendszer alapok enum{BIN=2,OCT=8,DEC=10,HEX=16}-ként predefiniáltak.
*
* Az eredmény kiíráshoz a létherhozásoz hasonlóan variálható formátumú sztrigbe konvertáló függvény rendelkezésre áll
*
* A negatív számokat minden esetben negatív előjellel, pozitív számformátum szerinti értékkel (abszolút értékkel) kell megadni. (Tehát egyik konstruktor és számformátum esetén sem kell kettes
* komplemenst képezni) A kettes komplemenst a konstruktor előállítja és azt tárolja el a vektor tagban. (Ha esetleg közvetlenül a vektor tagon keresztül akarjuk manipulálni a nyers adatot az
* esetben a kettes komplemensre értelemszerűen tekintettel kell lenni a negatív számok esetén.)
* A negatív nulla nem értelmezett. A negatív nullával történő létrehozást (példányosítást) a konstruktorok +0-ként értelmezik és tárolják.
* A negatív előjelű csupa nulla nyers érték megadása (a konstruktorokkal történő létrehozás esetét leszámítva) az adott vektor méreten értelmezett legkisebb negatív számot eredményezi. (=-(2^n+1)
* ahol n a teljes vektor adatbitjeinek száma)
*
* Az objektum mindkét tagváltozója és összes függvénye publikus és valamennyi függvény alap változata közvetlenül a tagváltozókat manipulálja így a definiált BinBigInt paraméterezésű wrapper függvények
* mellett bármikor akár közvetlenül vektorként is kezelhetők az adatok és az előjel is közvetlenül módosítható és lényegében az összes alap függvény hívható vektor érték és unsigned char előjel paraméterekkel is.
*
* A balra tolás biShl() függvény vektorméret növeléssel és csonkolással is hívható, ezen két verzióra külön egyértelmű elnevezésű wrapper függvények is rendelkezésre állnak.
*
* Az "over-/underflow" és "carry" megvalósítás több szempontból is eltér az ISA féle flag megvalósítástól!!!:
* Az "over-/underflow" a léptető függvények esetében a teljes lépésszámra vonatkozóan értelmezett, az OF a magas helyi értéken történő kilépésre az UF jobbra tolás estén az alacsony helyiértéken történő kilépésre értelmezett
* Az OF/UF akkor lesz 1 ha művelet vagy léptetés esetén a teljes léptetés során bármikor az előjelbittel ellentétes érték lép ki az eredeti vektorméreten értelmezetten
* tehát + szám esetén 1-es bit lép ki ill - szám esetén 0 bit lép ki.
* (pl. balra tolás esetén ha kiterjesztéssel hívjuk a függvényt akkor is overflow = 1 értéket advissza ha az eltolás során megnő a hasznos vektor méret mert ekkor az eredeti vektor méreten felül "kilép")
* A cl (CarryLast) pedig előjeltől függetlenül az utolsó lépésben kilépő bit értékét veszi fel.
*
* A felesleges ideiglenes memória foglalások elkerülése érdekében a függvények alapértelmezésben a paraméterül kapott referenciákon keresztül adják vissza az eredményt és ahol csak lehet
* (konstans) referenciákon keresztül fogadják az egyéb paramétereiket is. (A legtöbb alap függvény ezért void visszatérési értékű kivéve ahol pl. bool, carry vagy overflow (vagy egyéb nem vektor vagy
* nem BinBigInt) eredmény kerül visszaadásra)
*
* Azon függvények melyek eredménye BinBigInt és operátorhoz nem kötöttek ha az értelmes rendelkeznek BinBigInt visszaértési értékű wrapper függénnyel is. (override vagy kiterjesztett név alatt)
* Azon függvények amelyek eredménye BinBigInt és operátorhoz kötöttek nem rendelkeznek külön BinBigInt visszaértési értékű wrapper függénnyel. (Erre a célra az operátor függvény áll rendelkezésre)
*/


#define _UNICODE
#define UNICODE

//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>
#include <binbigint.h>

//typedef unsigned long long int ulli;

//STATIKUS ADATTAGOK INICIALIZÁLÁSA
const std::vector<ulli> BinBigInt::vUlliByteSize {BinBigInt::vUlliByteSize};
const std::vector<ulli> BinBigInt::vUlliBitSize {BinBigInt::ulliBitSize};
const ulli BinBigInt::ulliLimit = (ulli)std::numeric_limits<ulli>::max;
const std::vector<ulli> BinBigInt::vUlliLimit {BinBigInt::ulliLimit};

//Konstans definíciók --> Headerben

//Tagváltozók --> Headerben

//CONSTRUCTORS
//KONSTRUKTOROK

//Default instantiation and instantiation with Unsigned Long Long Int parameter (if "-" number given with (ulli) typecast then the two's complements
//absolute value will be the number's value so e.g. the "BinBigInt biCd((ulli)-1);" instantiation's result is a BinBigInt object which contain
//the + 18 446 744 073 709 551 615 value.
//Default és Unsigned Long Long Int paraméterrel történő példányosítás (ha "-" negatív számot adunk meg (ulli) typecasttal akkor a kettes komplemens
//abszolút értéke lesz a szám értéke tehát pl. a "BinBigInt biCd((ulli)-1);" példánysoítás eredménye egy a + 18 446 744 073 709 551 615 értéket tartalmazó
//Bigint objektum
BinBigInt::BinBigInt
(
    ulli a0
)
{
    *(v.data()) = a0;
    sigNeg = 0;
}

//Instantiation with Singed Long Long Int parameter
//Singed Long Long Int paraméterrel történő példányosítás
BinBigInt::BinBigInt
(
    signed long long int a0
)
{
    *(v.data()) = (ulli)a0;
    a0 < 0 ? sigNeg = 1 : sigNeg = 0;
}

//Instantiation with  vector and signum parameter (in the case of negative number should give the absolute/"+" value of the required
//number in the vector parameter which will have been converted to two's complement and stored in that form by the constructor!)
//Vektor paraméterrel és előjel értékkel történő példányosítás  (negatív szám esetén is a vektorban a szám aboszolút/"+" értékét
//kell megadni amit a konstruktor kettes komplemenssé alakít és úgy tárolja!)
BinBigInt::BinBigInt
(
    const std::vector<ulli> v, unsigned char sigNeg
)
{
    //+/-0 eset Ha a megadott szám pozitív előjellel nulla (lenne) (az esetleges negatív nulla bevitel kiszűrése miatt van külön esetként kezelve)
    if (biIsZero(v, 0))
    {
        biCopy(v, 0, this->v, this->sigNeg); //Pozitív nullára inicializálás
    }
    else
    {
        biCopy(v, sigNeg, this->v, this->sigNeg); //A megadott előjeles értékre inicializálás
        if (sigNeg == 1) //Ha a szám előjele negatív
        {
            //kettes komplemens képzés (a függvény ELŐJELET IS VÁLT!!! --> ami itt valójában szükségtelen --> Az előjelet ezért vissza kell állítani)
            biNegative(this->v, this->sigNeg, this->v, this->sigNeg);
            this->sigNeg = 1;   //Az eredeti negatív előjel visszaállítása
        }
    }
}

//Srting based instantiation = The instantiation with the paramters of strToBi() function --> call strToBi()
//String alapú példányosítás = A strToBi() függvénynek megfelelő paraméterekkel történő példányosítás --> strToBi() meghívása
BinBigInt::BinBigInt
(
    const std::string & as,      //A konvertálandó string referenciája
    const unsigned char nfb,     //A bemeneti sztring számformátum alapja [2-35] (pl. bin = 2 vagy BIN, oct = 8 vagy OCT, dec = 10 vagy DEC, hex = 16 vagy HEX) [Min. 2, Max. 35 lehet]
    const std::string sepStr,    //A tagoló sztring
    const ulli sepw              //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
)
{
    strToBi(as, nfb, v, sigNeg, sepStr, sepw);
}

//Copy constructor
//Másolókonstruktor (Erre valójában nincs szükség mert jó az alapértelmezett is...)
BinBigInt::BinBigInt
(
    const BinBigInt & a //A lemásolandó BinBigInt objektum referenciája
)
{
    biCopy(a.v, a.sigNeg, this->v, this->sigNeg);
}

//OPERATOS
//OPERÁTOROK

// = operator
// = operátor (Erre valójában nincs szükség mert jó az alapértelmezett is...)
BinBigInt& BinBigInt::operator=(const BinBigInt & a)
{
    biCopy(a, *this);
    return *this;
}

// += operator
// += operátor
BinBigInt& BinBigInt::operator+=(const BinBigInt & a)
{
    biAdd(a, *this, *this);
    return *this;
}

// -= operator
// -= operátor
BinBigInt& BinBigInt::operator-=(const BinBigInt & a)
{
    biSub(*this, a, *this);
    return *this;
}

// ++ postfix operator
// ++ postfix operátor (Az int nc paraméter csak a fordító számára történő jelzés, hogy ez egy post fix operátor)
BinBigInt& BinBigInt::operator++(int nc)
{
    biInc(*this);
    return *this;
}

// -- postfix operator
// -- postfix operátor (Az int nc paraméter csak a fordító számára történő jelzés, hogy ez egy post fix operátor)
BinBigInt& BinBigInt::operator--(int nc)
{
    biDec(*this);
    return *this;
}

// ++ prefix operator
// ++ prefix operátor
BinBigInt& BinBigInt::operator++()
{
    biInc(*this);
    return *this;
}

// -- prefix operator
// -- prefix operátor
BinBigInt& BinBigInt::operator--()
{
    biDec(*this);
    return *this;
}

//MEMBER FUNCTIONS
//TAG FÜGGVÉNYEK

//The bigint signum resulting function
//A bigint előjelét visszaadó függvény (mivel/amíg a sigNeg publikus lényegében felesleges)
unsigned char BinBigInt::getSigNeg()
{
    return sigNeg;
}

//The function resulting the data pointer of vector which the base of the bigint
//A bigint alapjául szolgáló vektor adat pointerét visszaadó függvény (mivel/amíg a "v" publikus valójában nem lenne szükséges de a ".v" leírása a kódban megspóróolható vele)
ulli * BinBigInt::data()
{
    return v.data();
}

//The function resulting the size of the vector which the base of the bigint
//A bigint alapjául szolgáló vektor méretét visszaadó függvény (mivel/amíg a "v" publikus valójában nem lenne szükséges de a ".v" leírása a kódban megspóróolható vele)
ulli BinBigInt::size()
{
    return v.size();
}

//Function to check if bigint is zero
//Nulla érték ellenőrző függvény
bool BinBigInt::biIsZero
(
    const std::vector<ulli> & a,  //Az ellenőrizendő számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn       //Az ellenőrizendő szám előjele
)
{
    if (aSn){return false;}     //Ha az előjel negatív akkor nem nulla, false-al kilép
    for (ulli i = 0; i < a.size(); i++)
    {
        if (*(a.data()+i) != 0) //Ha bármelyik elem nem 0
        {
            return false; //akkor false-al kilép
        }
    }
    return true;
}

//Nulla érték ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsZero
(
    const BinBigInt & a  //Az ellenőrizendő számot tartalmazó BinBigInt referenciája
)
{
    return biIsZero(a.v, a.sigNeg);
}

//Nulla érték ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsZero()
{
    return biIsZero(v, sigNeg);
}

//Function to set bigint to zero (not modify the size)
//Nullázó függvény (méretet nem módosít)
void BinBigInt::biSetToZero
(
    std::vector<ulli> & r, //A nullázandó dinamikus vektor referenciája
    unsigned char & rSn    //A nullázandó szám előjelének referenciája
)
{
    //Az "r" vektor nullázása
    for (ulli i = 0; i<r.size(); i++)
    {
        *(r.data()+i) = 0; //Az "r" vektor aktuális elemének nullázása
    }
    rSn = 0; //Az előjel "+"-ra állítása
}

//Nullázó függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biSetToZero
(
    BinBigInt & r //A nullázandó BinBigInt referenciája
)
{
    biSetToZero(r.v, r.sigNeg);
}

//Nullázó függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biSetToZero()
{
    biSetToZero(v, sigNeg);
}

//Function to copy bigint by value (the size of the bigint/vector set to the same as size of "a" parameter)
//Másoló függvény (a méretet az "a" paraméter méretére állítja)
void BinBigInt::biCopy
(
    const std::vector<ulli> & a,  //A másolandó számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,      //A másolandó szám előjele
    std::vector<ulli> & r,        //A másolat vektor referenciája
    unsigned char & rSn           //A másolat szám előjelének referenciája
)
{
    //Az "a" vektor értékének lemásolása érték szerint ar r vektorbe
    if(&a != &r) //Ha a és r paraméterben átvett vektorok nem azonos entitások (mert lehet a bemeneti vektor az eredmény is egyben)
    {
        if(a.size() != r.size())
        {
            //r vektor átméretezése a vektor méretûre
            r.resize(a.size(), 0); //*r átméretezés és ha növelésre kerül az új elemek 0-ra inicializálása
        }

        //Az "a" vektor értékének lemásolása érték szerint az r vektorba
        for (ulli i = 0; i<a.size(); i++)
        {
            *(r.data()+i) = *(a.data()+i);
        }
        //Az előjel másolása
        rSn = aSn;
    }
}

//Másoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biCopy
(
    const BinBigInt & a,  //A másolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r  //A másolat vektor referenciája
)
{
   biCopy(a.v, a.sigNeg, r.v, r.sigNeg);
}

//Másoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biCopy
(
    BinBigInt & r  //A másolat vektor referenciája
)
{
   biCopy(v, sigNeg, r.v, r.sigNeg);
}

//Funtion to resize the bigint
//Átméretező függény
void BinBigInt::biResize
(
    const std::vector<ulli> & a, //Az igazítandó/átméretezendő számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,     //Az igazítandó/átméretezendő szám előjele
    const ulli newSize,          //Az új méret
    std::vector<ulli> & r,       //Az erdemény vektor referenciája
    unsigned char & rSn          //Az eredény szám előjele
)
{
    if(&a != &r) //Ha a nem azonos r-el
    {
        //a átmásolása r-be
        biCopy(a, aSn, r, rSn);
    }

    if(aSn) //Ha a előjele negatív
    {
        r.resize(newSize, -1); //r méretének növelése, az új elemek -1-re (0xffffffffffffffff) inicializálása
    }
    else //Ha a előjele pozitív
    {
        r.resize(newSize, 0); //r méretének növelése, az új elemek 0-ra inicializálása
    }
}

//Átméretező függény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biResize
(
    const BinBigInt & a, //Az igazítandó/átméretezendő számot tartalmazó BinBigInt referenciája
    const ulli newSize,        //A méret amire ki
    BinBigInt & r       //Az erdemény vektor referenciája
)
{
    biResize(a.v, a.sigNeg, newSize, r.v, r.sigNeg);
}

//Átméretező függény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biResize
(
    const ulli newSize, //A méret amire ki
    BinBigInt & r       //Az erdemény vektor referenciája
)
{
    biResize(v, sigNeg, newSize, r.v, r.sigNeg);
}

//Átméretező függény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biResize
(
    const BinBigInt & a,    //Az igazítandó/átméretezendő számot tartalmazó BinBigInt referenciája
    const ulli newSize      //A méret amire ki
)
{
    BinBigInt r;       //Az erdemény BinBigInt
    biResize(a.v, a.sigNeg, newSize, r.v, r.sigNeg);
    return r;
}

//Átméretező függény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biResize
(
    const ulli newSize  //A méret amire ki
)
{
    BinBigInt r;       //Az erdemény BinBigInt
    biResize(v, sigNeg, newSize, r.v, r.sigNeg);
    return r;
}

//Function to size extending (signum correctly extend the size of the vector if the given size is bigger than the actual size)
//(This suitable for safe (surely truncate avoiding) aligning two vector together without pre-comparison.)
//Méret kiterjesztő/növelő függvény (előjel helyesen megnöveli a vektor méretét ha a megadott méret nagyobb mint az aktuális méret)
//(Két vektor előellenőrzés nélküli biztonságos (biztosan csoknolás mentes) egymáshoz igazítására használható)
void BinBigInt::biExtendIfShorter
(
    const std::vector<ulli> & a, //Az igazítandó/átméretezendő számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,     //Az igazítandó/átméretezendő szám előjele
    const ulli sizeToExt,        //A méret amire meg kell növelni / ki kell terjeszteni
    std::vector<ulli> & r,       //Az erdemény vektor referenciája
    unsigned char & rSn          //Az eredény szám előjele
)
{
    if(a.size()<sizeToExt)//Ha az a vektor mérete kisebb mint amekkorára meg kell növelni
    {
        biCopy(a, aSn, r, rSn); //a átmásolása r-be
        biResize(r, rSn, sizeToExt, r, rSn); //r megnövelése
    }
}

//Méret kiterjesztő/növelő függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biExtendIfShorter
(
    const BinBigInt & a,    //Az igazítandó/átméretezendő számot tartalmazó BinBigInt referenciája
    const ulli sizeToExt,   //A méret amire meg kell növelni / ki kell terjeszteni
    BinBigInt & r           //Az erdemény vektor referenciája
)
{
    biExtendIfShorter(a.v, a.sigNeg, sizeToExt, r.v, r.sigNeg);
}

//Méret kiterjesztő/növelő függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biExtendIfShorter
(
    const ulli sizeToExt,   //A méret amire meg kell növelni / ki kell terjeszteni
    BinBigInt & r           //Az erdemény vektor referenciája
)
{
    biExtendIfShorter(v, sigNeg, sizeToExt, r.v, r.sigNeg);
}

//Méret kiterjesztő/növelő függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biExtendIfShorter
(
    const BinBigInt & a,    //Az igazítandó/átméretezendő számot tartalmazó BinBigInt referenciája
    const ulli sizeToExt    //A méret amire meg kell növelni / ki kell terjeszteni
)
{
    BinBigInt r;       //Az erdemény BinBigInt
    biExtendIfShorter(a.v, a.sigNeg, sizeToExt, r.v, r.sigNeg);
    return r;
}

//Méret kiterjesztő/növelő függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biExtendIfShorter
(
    const ulli sizeToExt   //A méret amire meg kell növelni / ki kell terjeszteni
)
{
    BinBigInt r;       //Az erdemény BinBigInt
    biExtendIfShorter(v, sigNeg, sizeToExt, r.v, r.sigNeg);
    return r;
}

//Bitwise and function (operation extended to the singum too)
//Bitenkénti ÉS (AND) függvény (előjelre is kiterjdeően)
void BinBigInt::biBAnd
(
    std::vector<ulli> a,    //Az egyik számot tartalmazó dinamikus vektor
    unsigned char aSn,      //Az egyik szám előjele
    std::vector<ulli> b,    //A másik számot tartalmazó dinamikus vektor
    unsigned char bSn,      //A másik szám előjele
    std::vector<ulli> & r,  //Az erdemény vektor referenciája
    unsigned char & rSn     //Az eredény szám előjele (A két bemeneti szám előjelén is elvégzésre kerül a művelet)
)
{
    //A vektorok méretazonossági kritériumának teljesítése
    biExtendIfShorter(b, bSn, a.size(), b, bSn); //b méretének a-éhoz igazítása ha kisebb mint a mérete
    biExtendIfShorter(a, aSn, b.size(), a, aSn); //a méretének b-éhoz igazítása ha kisebb mint b mérete

    //biResize(r, rSn, a.size(), r, rSn); //r méretének a-éhoz igazítása
    r.resize(a.size(), 0); //r átméretezése, az új elemek 0-ra inicializálása

    //Az "r" vektor nullázása
    biSetToZero(r, rSn);

    //A AND művelet elévgzése minden elemen és r-be írása
    for (ulli i = 0; i < a.size(); i++)
    {
        *(r.data()+i) = *(a.data()+i) & *(b.data()+i);
    }
    rSn = aSn & bSn;
}

//Bitenkénti ÉS (AND) függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biBAnd
(
    const BinBigInt & a,    //Az egyik számot tartalmazó BinBigInt referenciája
    const BinBigInt & b,    //A másik számot tartalmazó BinBigInt referenciája
    BinBigInt & r           //Az erdemény vektor referenciája
)
{
    biBAnd(a.v, a.sigNeg, b.v, b.sigNeg, r.v, r.sigNeg);
}

//Bitenkénti ÉS (AND) függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biBAnd
(
    const BinBigInt & b,    //A másik számot tartalmazó BinBigInt referenciája
    BinBigInt & r           //Az erdemény vektor referenciája
)
{
    biBAnd(v, sigNeg, b.v, b.sigNeg, r.v, r.sigNeg);
}

//Bitwise inclusive or (OR) function (operation extended to the singum too)
//Bitenkénti VAGY (OR) függvény (előjelre is kiterjdeően)
void BinBigInt::biBOr
(
    std::vector<ulli> a,    //Az egyik számot tartalmazó dinamikus vektor
    unsigned char aSn,      //Az egyik szám előjele
    std::vector<ulli> b,    //A másik számot tartalmazó dinamikus vektor
    unsigned char bSn,      //A másik szám előjele
    std::vector<ulli> & r,  //Az erdemény vektor referenciája
    unsigned char & rSn     //Az eredmény szám előjele (A két bemeneti szám előjelén is elvégzésre kerül a művelet)
)
{
    //A vektorok méretazonossági kritériumának teljesítése
    biExtendIfShorter(b, bSn, a.size(), b, bSn); //b méretének a-éhoz igazítása ha kisebb mint a mérete
    biExtendIfShorter(a, aSn, b.size(), a, aSn); //a méretének b-éhoz igazítása ha kisebb mint b mérete

    r.resize(a.size(), 0); //r átméretezése, az új elemk 0-ra inicializálása

    //Az "r" vektor nullázása
    biSetToZero(r, rSn);

    //A OR művelet elévgzése minden elemen és r-be írása
    for (ulli i = 0; i < a.size(); i++)
    {
        *(r.data()+i) = *(a.data()+i) | *(b.data()+i);
    }
    rSn = aSn | bSn;
}

//Bitenkénti VAGY (OR) függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biBOr
(
    const BinBigInt & a, //Az egyik számot tartalmazó BinBigInt referenciája
    const BinBigInt & b,  //A másik számot tartalmazó BinBigInt referenciája
    BinBigInt & r        //Az erdemény vektor referenciája
)
{
    biBOr(a.v, a.sigNeg, b.v, b.sigNeg, r.v, r.sigNeg);
}

//Bitenkénti VAGY (OR) függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biBOr
(
    const BinBigInt & b,  //A másik számot tartalmazó BinBigInt referenciája
    BinBigInt & r        //Az erdemény vektor referenciája
)
{
    biBOr(v, sigNeg, b.v, b.sigNeg, r.v, r.sigNeg);
}

//Bitwise exclusive or (XOR) function (operation extended to the singum too)
//Bitenkénti KIZÁRÓ VAGY (XOR) függvény (előjelre is kiterjdeően)
void BinBigInt::biBXor
(
    std::vector<ulli> a,    //Az egyik számot tartalmazó dinamikus vektor
    unsigned char aSn,      //Az egyik szám előjele
    std::vector<ulli> b,    //A másik számot tartalmazó dinamikus vektor
    unsigned char bSn,      //A másikszám előjele
    std::vector<ulli> & r,  //Az erdemény vektor referenciája
    unsigned char & rSn     //Az eredmény szám előjele (A két bemeneti szám előjelén is elvégzésre kerül a művelet)
)
{
    //A vektorok méretazonossági kritériumának teljesítése
    biExtendIfShorter(b, bSn, a.size(), b, bSn); //b méretének a-éhoz igazítása ha kisebb mint a mérete
    biExtendIfShorter(a, aSn, b.size(), a, aSn); //a méretének b-éhoz igazítása ha kisebb mint b mérete

    r.resize(a.size(), 0); //r átméretezése, az új elemk 0-ra inicializálása

    //Az "r" vektor nullázása
    biSetToZero(r, rSn);

    //A XOR művelet elévgzése minden elemen és r-be írása
    for (ulli i = 0; i < a.size(); i++)
    {
        *(r.data()+i) = *(a.data()+i) ^ *(b.data()+i);
    }
    rSn = aSn ^ bSn;
}

//Bitenkénti KIZÁRÓ VAGY (XOR) függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biBXor
(
    const BinBigInt & a,    //Az egyik számot tartalmazó BinBigInt referenciája
    const BinBigInt & b,    //A másik számot tartalmazó BinBigInt referenciája
    BinBigInt & r           //Az erdemény vektor referenciája
)
{
    biBXor(a.v, a.sigNeg, b.v, b.sigNeg, r.v, r.sigNeg);
}

//Bitenkénti KIZÁRÓ VAGY (XOR) függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biBXor
(
    const BinBigInt & b,    //A másik számot tartalmazó BinBigInt referenciája
    BinBigInt & r           //Az erdemény vektor referenciája
)
{
    biBXor(v, sigNeg, b.v, b.sigNeg, r.v, r.sigNeg);
}

//Bitwise negate (NOT, NEG) function
//Bitenkénti NEGÁLÓ (NOT, NEG) függvény
void BinBigInt::biBNot
(
    const std::vector<ulli> a,  //A negálandó számot tartalmazó dinamikus vektor
    const unsigned char aSn,    //A negálandó szám előjele
    std::vector<ulli> & r,      //Az erdemény vektor referenciája
    unsigned char & rSn         //Az erdmény előjele
)
{
    r.resize(a.size(), 0); //r átméretezése, az új elemk 0-ra inicializálása

    //Az "r" vektor nullázása
    biSetToZero(r, rSn);

    for (ulli i = 0; i < a.size(); i++)
    {
        *(r.data()+i) = ~ *(a.data()+i);
    }
    rSn = (aSn & 1) ^ 1; //az előjel váltása (XOR-olás 1-el, az & 1 csak az esetlegesen 0-tól vagy 1-től eltérő értékekből eredeő hibák kiszűrése miatt van)
}

//Bitenkénti NEGÁLÓ (NOT) függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biBNot
(
    const BinBigInt & a, //A negálandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r        //Az erdemény vektor referenciája
)
{
    biBNot(a.v, a.sigNeg, r.v, r.sigNeg);
}

//Bitenkénti NEGÁLÓ (NOT) függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biBNot
(
    BinBigInt & r   //Az erdemény vektor referenciája
)
{
    biBNot(v, sigNeg, r.v, r.sigNeg);
}

//Function to remove the leading all zero or all one ulli elements (this modify the +0 valued vector to +0 valued, 1 element sized vector, and the -1 valued vector to -1 valued, 1 element sized vector)
//Bevezető pozitív csupa nulla vagy negatív csupaegy ulli elemeket eltávolító függvény (a +0 értékű vektort +0 értékű 1 elemű vektorra, a -1 értékű vektort -1 értékű 1 elemű vektorra módosítja)
void BinBigInt::biLeaderFillerCut //LeaderFillerCut
(
    const std::vector<ulli> & a, //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,     //Az egyik szám előjele
    std::vector<ulli> & r,       //Az erdemény vektor referenciája
    unsigned char & rSn          //Az erdmény előjele
)
{
    //Az a másolása r-be (=r inicializálása)
    biCopy(a, aSn, r, rSn);

    //-1 értékű vektor
    std::vector<ulli> negOne {0xffffffffffffffff};
    unsigned char negOneSn = 1;

    //Ha a kapott vektor értéke +0 vagy -1 (!!! Ez nem csak a gyorsítás miatt fontos, hanem ezen értékű vektorok megsemmisítésének elkerülése miatt is szüskéges (Ha nem vizsgált az eset akkor 0 hosszúságú vektort eredményezne a másik ág ezen esetekben))
    if(biIsZero(r, rSn) || biIsEqual(r, rSn, negOne, negOneSn))
    {
        r.resize(1);
    }
    else //Ha vizsgálni kell
    {
        //Az esetleges csak bevezetõ nullákat tartalmazó ulli elemek elhagyásával egy lehetõ legkisebb elemszámú
        //de minden értékes bitet tartalmazó átmeneti vektor létrehozása az osztóból (sebességnövelés miatt)
        ulli sl = a.size(); //A nulla értékû leader ulli elemek nélküli vektor méret számlálója
                    //Ha pozitív és csupa nulla     Ha negatív és csupa egy  7766554433221100
        while(((*(a.data()+(sl-1)) == 0 && !aSn) || (*(a.data()+(sl-1)) == 0xffffffffffffffff && aSn)) && sl > 0)
        {
            sl--; //A pozitív csupa nulla vagy negatív csupaegy értékû leader ulli elemek nélküli vektor méret számlálója (az eredeti teljes mérettõl visszafele számlál)
        }
        r.resize(sl, 0); //Az r méretének csökkentése az értékes adattal bíró méretre (Mivel a méret csökken jó a sima vektor resize)
    }
}

//Bevezető nulla elemeket (ulli-kat) eltávolító függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biLeaderFillerCut
(
    const BinBigInt & a, //Az erdeti vektor referenciája
    BinBigInt & r        //Az eredmény vektor referenciája
)
{
    biLeaderFillerCut(a.v, a.sigNeg, r.v, r.sigNeg);
}


//Bevezető nulla byteokat eltávolító függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biLeaderFillerCut
(
    BinBigInt & r  //Az eredmény vektor referenciája
)
{
    biLeaderFillerCut(v, sigNeg, r.v, r.sigNeg);
}

//Bevezető nulla elemeket (ulli-kat) eltávolító függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biLeaderFillerCut
(
    const BinBigInt & a  //Az erdeti vektor referenciája
)
{
    BinBigInt r;  //Az eredmény BinBigInt
    biLeaderFillerCut(a.v, a.sigNeg, r.v, r.sigNeg);
    return r;
}


//Bevezető nulla byteokat eltávolító függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biLeaderFillerCut()
{
    BinBigInt r;  //Az eredmény BinBigInt
    biLeaderFillerCut(v, sigNeg, r.v, r.sigNeg);
    return r;
}

//The operative addition subfunction (It makes the addition without the touch and regardless the value of singum and with the required carry transmission.)
//(The function's vector parameters must be the same sized!!! [This guaranteed by the biAdd()])
//A tényleges összeadást végző alfüggvény (Az előjelek érintése NÉLKÜL és azokra való tekintet NÉLKÜL az aktuális carry szükségszerű átvitelével végzi az összeadást.)
//(A paraméterül kapott vektoroknak azonos méretűeknek kell lenniük!!! [Ezt garantálja a biAdd()])
unsigned char BinBigInt::biAddSF0
(
    const std::vector<ulli> a,  //Az egyik számot tartalmazó dinamikus vektor
    const std::vector<ulli> b,  //A másik számot tartalmazó dinamikus vektor
    std::vector<ulli> & r       //Az eredmény vektor referenciája
)
{
    if (!((a.size() == b.size()) && b.size() == r.size())) {throw std::invalid_argument("biAddSF0: all of the parameters need have to be equivalent sized");} //Ha a paraméterek mérete nem azonos az hiba
    unsigned char carry = 0;
    for (ulli i=0; i<a.size(); i++)
    {
        *(r.data()+i) = carry + *(a.data()+i) + *(b.data()+i);
        if
        (
            *(a.data()+i) > ((std::numeric_limits<ulli>::max)() - *(b.data()+i)) ||
            carry > ((std::numeric_limits<ulli>::max)() - *(b.data()+i)) ||
            *(a.data()+i) > ((std::numeric_limits<ulli>::max)() - *(b.data()+i) - carry)
        )
        {
            carry = 1; //Ha az összeadás túlcsordulást okozott a carry = 1 (A feltétel vizsgálat azért ilyen bonyolult, mert ha b.data()[i] = az ULLI MAX értékével akkor az 1 értékű carry hozzáadása már átfordítja az unsigned változót 0-ra...)
        }
        else
        {
            carry = 0;
        }
    }
    return carry;
}

//Bigint addition function
//Bigint összeadás
unsigned char BinBigInt::biAdd
(
    std::vector<ulli> a,    //Az egyik számot tartalmazó dinamikus vektor
    unsigned char aSn,      //Az egyik szám előjele
    std::vector<ulli> b,    //A másik számot tartalmazó dinamikus vektor
    unsigned char bSn,      //A másik szám előjele
    std::vector<ulli> & r,  //Az erdemény vektor referenciája
    unsigned char & rSn     //Az eredény szám előjele
)
{
    //A vektorok méretazonossági kritériumának teljesítése
    biExtendIfShorter(b, bSn, a.size(), b, bSn); //b méretének a-éhoz igazítása ha kisebb mint a mérete
    biExtendIfShorter(a, aSn, b.size(), a, aSn); //a méretének b-éhoz igazítása ha kisebb mint b mérete

    //Az r vektor megfelelő méretűre állítása és nullázása
    //r vektor átméretezése a vektor méretûre
    r.resize(a.size(), 0); //r átméretezés és ha növelésre kerül az új elemk 0-ra inicializálása

    //Az "r" vektor nullázása
    biSetToZero(r, rSn);

    unsigned char carry = 0;
    carry = biAddSF0(a, b, r); //A tényleges összeadást végzõ alfüggvény hívása

    if ((aSn+bSn)==0 && carry) //Ha mindkét szám pozitív és a carry 1
    {   //Ez esetben az eredeti vektor nem volt képes tárolni az eredményt azaz vektor szinten is túlcsordulás történt
        //mely esetben meg kell növelni 1-el az eredmény vektor méretét és 1-re kell állítani a legfelsõ elem értékét

        r.resize(r.size()+1, 1);   //A tömb méretének növelése egy ulli elemmel és az új elem 1-re inicializálása
    }
    else if((aSn+bSn)==2) //Ha mindkét szám negatív
    {
        if(!carry) //és a carry 0
        {
            //Ez esetben az eredeti vektor nem volt képes tárolni az eredményt azaz vektor szinten is túlcsordulás történt
            //mely esetben meg kell növelni 1-el az eredmény vektor méretét és fffffffffffffff0-ra = (-2)-re kell állítani a legfelsõ elem értékét

            r.resize(r.size()+1, -2);   //A tömb méretének növelése egy ulli elemmel és az új elem -2-re inicializálása
        }

        rSn = 1; //Eredmény előjel negatív (Megjegyzés: az rSn alapértelmezetten 0 ("+") mivel az r korábban nullára lett inicializálva)
    }
    else if((aSn+bSn)==1) //Ha az egyik szám pozitív a másik negatív
    {   //Ez esetben az eredmény abszolútértéke bizonyosan kisebb mint az összeadandó számok abszolút értéke tehát nem lehet szükséges a tömb
        //méretének növelése ugyanakkor az előjel beállítása szükséges

        if(!carry) //és a carry 0
        {
            rSn = 1; //Eredmény előjel negatív (Megjegyzés: az rSn alapértelmezetten 0 ("+") mivel az r korábban nullára lett inicializálva)
        }
    }

    return carry;
}

//Bigint összeadás OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biAdd
(
    const BinBigInt & a,  //Az alap számot tartalmazó BinBigInt referenciája
    const BinBigInt & b,  //A kivonandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r  //Az eredmény vektor referenciája
)
{
    return biAdd(a.v, a.sigNeg, b.v, b.sigNeg, r.v, r.sigNeg); //Carry visszaadása
}

//Bigint összeadás OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biAdd
(
    const BinBigInt & b,  //A kivonandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r  //Az eredmény vektor referenciája
)
{
    return biAdd(v, sigNeg, b.v, b.sigNeg, r.v, r.sigNeg); //Carry visszaadása
}

//Increment function
//Inkrementáló függvény
void BinBigInt::biInc
(
    std::vector<ulli> & r,  //A növelendő számot tartalmazó dinamikus vektor referenciája
    unsigned char & rSn     //A növelendő szám előjele
)
{
     const std::vector<ulli> one {1};
     unsigned char oneSn = 0;
     biAdd(r, rSn, one, oneSn, r, rSn);
}

//Inkrementáló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biInc
(
    BinBigInt & r   //A növelendő számot tartalmazó BinBigInt referenciája
)
{
     biInc(r.v, r.sigNeg);
}

//Inkrementáló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biInc()
{
     biInc(v, sigNeg);
}

//The operative subtraction subfunction (It makes the subtraction without the touch and regardless the value of singum and with the required carry transmission.)
//(The function's vector parameters must be the same sized!!! [This guaranteed by the biAdd()])
//A tényleges kivonást végző sub függvény (Az előjelek érintése NÉLKÜL és azokra való tekintet NÉLKÜL az aktuális carry szükségszerű átvitelével végzi a kivonást.)
//(A paraméterül kapott vektoroknak azonos méretűeknek kell lenniük!!! Ezt garantálja a biSub())
unsigned char BinBigInt::biSubSF0
(
    const std::vector<ulli> a,  //Az egyik számot tartalmazó dinamikus vektor
    const std::vector<ulli> b,  //A másik számot tartalmazó dinamikus vektor
    std::vector<ulli> & r       //Az eredmény vektor referenciája
)
{
    if (!((a.size() == b.size()) && b.size() == r.size())) throw std::invalid_argument("biSubSF0: all of the parameters need have to be equivalent sized"); //Ha a paraméterek mérete nem azonos az hiba
    unsigned char carry = 0;
    for (ulli i=0; i<a.size(); i++)
    {
        *(r.data()+i) = *(a.data()+i) - *(b.data()+i) - carry;
        if
        (
            *(a.data()+i) < *(b.data()+i) ||
            carry > ((std::numeric_limits<ulli>::max)() - *(b.data()+i)) ||
            *(a.data()+i)< (*(b.data()+i) + carry)
        )
        {
            carry = 1; //Ha a kivonás túlcsordulást okozott (az eredmény negatív) a carry = 1 (A feltétel vizsgálat azért ilyen bonyolult, mert ha b.data()[i] = az ULLI MAX értékével akkor az 1 értékű carry kivonása már átfordítja az unsigned változót ff..ff-re...)
        }
        else
        {
            carry = 0;
        }
    }
    return carry;
}

//Bigint subtraction function
//Bigint kivonás
unsigned char BinBigInt::biSub
(
    std::vector<ulli> a,    //Az egyik számot tartalmazó dinamikus vektor
    unsigned char aSn,      //Az egyik szám előjele
    std::vector<ulli> b,    //A másik számot tartalmazó dinamikus vektor
    unsigned char bSn,      //A másik szám előjele
    std::vector<ulli> & r,  //Az erdemény vektor referenciája
    unsigned char & rSn     //Az eredény szám előjele
)
{
    //A vektorok méretazonossági kritériumának teljesítése
    biExtendIfShorter(b, bSn, a.size(), b, bSn); //b méretének a-éhoz igazítása ha kisebb mint a mérete
    biExtendIfShorter(a, aSn, b.size(), a, aSn); //a méretének b-éhoz igazítása ha kisebb mint b mérete

    //Az r vektor megfelelő méretűre állítása és nullázása
    //r vektor átméretezése a vektor méretûre
    r.resize(a.size(), 0); //*r átméretezés és ha növelésre kerül az új elemk 0-ra inicializálása

    //Az "r" vektor nullázása
    biSetToZero(r, rSn);

    unsigned char carry = 0;
    carry = biSubSF0(a, b, r); //A tényleges kivonást végzõ alfüggvény hívása (azonos méretű vektorokat kell neki átadni!!!)

   if ((aSn+bSn)==0 || (aSn+bSn)==2) //Ha mindkét szám pozitív vagy mindkét szám negatív
    {   //Ez esetben az eredmény abszolútértéke bizonyosan kisebb mint a kivonandó számok abszolút értéke tehát nem lehet szükséges a tömb
        //méretének növelése ugyanakkor az előjel beállítása szükséges
        if(carry) //és a carry 1
        {
            rSn = 1; //Eredmény előjel negatív
        }
    }
     else if((aSn+bSn)==1) //Ha az egyik szám pozitív a másik negatív
    {
        if(aSn) //Ha a kivonás alapja a negatív és a kivonandó a pozitív --> Eredmény negatív
        {
            if(carry) //és a carry 1
            {
                //Ez esetben az eredeti vektor nem volt képes tárolni az eredményt azaz vektor szinten is túlcsordulás történt
                //mely esetben meg kell növelni 1-el az eredmény vektor méretét és fffffffffffffff0-ra = (-2)-re kell állítani a legfelsõ elem értékét

                r.resize(r.size()+1, -2);   //A tömb méretének növelése egy ulli elemmel és az új elem -2-re inicializálása
            }

            rSn = 1; //Eredmény előjel negatív
        }
        else //Ha a kivonás alapja a pozitív és a kivonandó a negatív --> Eredmény pozitív
        {
            if(!carry) //és a carry 0 (Az általam választott számábrázolás miatt (az előjel nem a vektor adatain hanem külön char változóban értelmezett)
                                     //egyel megnövelt vektorméretet kell elképzelni ahol a tényleges legfelsőből a képzelt legfelsőbe történő carryt kell vizsgálni
                                     //Ha ez a carry 1 lenne akkor az a megnövelt vektoron értelmezendő negatív szám leading 1-es bitjeit teljesen kinullázná, ...
            {                        //ha viszont a carry 0 akkor a megnövelt vektor legfelső ulli elemén a leading egyesek leading 0-k ból kivonásával 000..001 = 1 érték adódik...
                                     //amit figyelmbe is kell venni különben téves lesz az eredmény!) -->
                //Ez esetben az eredeti vektor nem volt képes tárolni az eredményt azaz vektor szinten is túlcsordulás történt
                //mely esetben meg kell növelni 1-el az eredmény vektor méretét és 1-re kell állítani a legfelsõ elem értékét

                r.resize(r.size()+1, 1);   //A tömb méretének növelése egy ulli elemmel és az új elem -2-re inicializálása
            }
            rSn = 0; //Eredmény előjel poztív
        }
    }

    return carry;
}

//Bigint kivonás OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biSub
(
    const BinBigInt & a,    //Az alap számot tartalmazó BinBigInt referenciája
    const BinBigInt & b,    //A kivonandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r           //Az eredmény vektor referenciája
)
{
    return biSub(a.v, a.sigNeg, b.v, b.sigNeg, r.v, r.sigNeg); //Carry visszaadása
}

//Bigint kivonás OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biSub
(
    const BinBigInt & b,    //A kivonandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r           //Az eredmény vektor referenciája
)
{
    return biSub(v, sigNeg, b.v, b.sigNeg, r.v, r.sigNeg); //Carry visszaadása
}

//Decrement function
//Dekrementáló függvény
void BinBigInt::biDec
(
    std::vector<ulli> & r,  //A csökkentendő számot tartalmazó dinamikus vektor referenciája
    unsigned char & rSn     //A csökkentendő szám előjele
)
{
     const std::vector<ulli> one {1};
     unsigned char oneSn = 0;
     biSub(r, rSn, one, oneSn, r, rSn);
}

//Dekrementáló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biDec
(
    BinBigInt & r   //A csökkentendő számot tartalmazó BinBigInt referenciája
)
{
     biDec(r.v, r.sigNeg);
}

//Dekrementáló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biDec()
{
     biDec(v, sigNeg);
}

//Multiply fucntion
//Szorzó függvény
void BinBigInt::biMul
(
    std::vector<ulli> a,    //Az egyik számot tartalmazó dinamikus vektor
    unsigned char aSn,      //Az egyik szám előjele
    std::vector<ulli> b,    //A másik számot tartalmazó dinamikus vektor
    unsigned char bSn,      //A másik szám előjele
    std::vector<ulli> & r,  //Az erdemény vektor referenciája
    unsigned char & rSn     //Az eredény szám előjele
)
{
    //Az előjel előállítása
    unsigned char tmprSn = aSn ^ bSn;

    //Abszolút értékekre váltás
    biAbs(a, aSn, a, aSn);
    biAbs(b, bSn, b, bSn);

    //r inicializálása
    r.resize(1, 0); //r 1 eleműre állítása
    *(r.data()) = 0; //Az "r" vektor nullázása

    unsigned char carry = 0; //A carry

    std::vector<ulli>  bShNr {0}; ////A "b" vektor két 1-es bit közötti eltolás lépéseinek számlálója (Mivel a vektor bithossza elméletileg lényegesen nagyobb lehet(ne) mint a legnagyobb alaptípus mérete ezt az értéket is vektorban szükséges tárolni)
    unsigned char bShNrSn = 0; //A "vbl" vektor előjele

    unsigned char actLowBit = 0; //A "b" vektor aktuálisan legalsó bitjét tartalmazó érték

    ulli bl = b.size(); //A b vektor for ciklusaihoz az eredeti vektor elemszám átmásolása (ha a többi vektor mérete nõ ennek akkor sem kell az csak feleslegesn növelné a futásidõt)

    //Mivel a vektor bithossza elméletileg lényegesen nagyobb lehet mint a legnagyobb alaptípus mérete ezt az értéket is vektorban szüskgées tárolni

    std::vector<ulli> vbBitLen {bl}; //Egy a "b" vektor bitekben értelmezett méretének megfelelő számot tartalmazó vektor. (A a "b" vektor byteban (és NEM bitben) értelmezett méretére inicializálva. A ténylegesen tárolandó bitméret később kerül előállításra)
    unsigned char vbBitLenSn = 0; //A "vbBitLen" vektor előjele

    //Mivel a vektor bitméretének előállításához a vektor vektorban tárolt méretét össze kell szorozni a vektor alap elemének bit számával (ami jelenleg amúgy 64) de közben ez itt maga a vektor összeszorzó függvényem a szorzást
    //eltolással oldom meg. A megfelelő eltolás mértéke a szorzó 2-es alapú logaritmusa (64 esetén ez 6) de a jövőre is gondolva (lehet template-síteni fogom az objektumot és/vagy lesznek 64 bitesnél nagyobb architektúrák)
    //az alap elem byte méretét (ami az ulli esetén 8) nem hardcoded formában adom meg hanem jobbra tolással előállítom a kettes alapú logaritmusát. mivel a byte méret nem fog változini hardcoded formában adom meg a 8
    //kettes alapú logaritmusát azaz a 3-at)

    //Az alap elem (ulli) byte méretének kettes alapú logaritmusának kiszámítása
    ulli us = ulliByteSize; //Az ulli típus méretének tárolására szolságló változó (valójában az ulli mérete 8 szóval ez nem lenne szüskéges de ha esetleg template-sítem akkor jól jön majd ha nem hardcoded az érték)
    ulli baseTypLogTwo = 0; //Az ulli típus méretének kettes alapú logaritmusának tárolására szolságló változó (nyilván a későbbiekben is minden alaptípus mérete kettőnek valamilyen hatványa lesz)
    while(us > 1)
    {
        us = us >> 1; //léptetés jobbra = osztás kettővel
        ++baseTypLogTwo;
    }

    //A vektor bitekben értelmezett méretének megfelelő szám előállítása
    biShlNoCl(vbBitLen, vbBitLenSn, vbBitLen, vbBitLenSn,  (baseTypLogTwo * 3), 1); //A kifejezés lényegében egyenértékű a vbBitLen = vbl*(ulliBitSize) = vbl*64 kifejezésel (A balra létetés itt lényegében a szorzás helyett van. Mivel ez a kód maga a BinBigInt szorzás függvény így itt értelemszerűen nem hívhatja önmagát)

    unsigned char iSn = 0;

    for (std::vector<ulli> i {0}; biIsSmaller(i, iSn, vbBitLen, vbBitLenSn); biInc(i, iSn))  //A "b" vektor minden bitjének egyesével történõ megállapítása shr-el a legkisebb helyiértékû ulli-vel kezdve (carry vizsgálatával) (külső ciklus a vektor bitszáma szerinti)
    {
        //Az "b" a vektor minden ulli elemének szükséges mértékû eltolása jobbra e-el (shr-el) a legkisebb helyiértékû ulli-vel kezdve az aktuális soron következő bit megállapításához (belsõ ciklus a vektorelemek száma szerinti9
        for (ulli j = 0; j<bl; j++)
        {
            //r aktuális ulli elemének 1 bittel balra történõ eltolása
            actLowBit = *(b.data()+j) & 1; //Az aktuális jobb szélső bit értékének megállapítása (ha 1 akkor az aktuális jobbratolás "carryt" eredményez)
            *(b.data()+j) = *(b.data()+j) >> 1;  //r aktuális ulli elemének 1 bittel jobbra történõ eltolása

            if(actLowBit) //Ha az aktuális "b" vektorelemben van alulcsordulás azt a magasabb helyiértékrõl egyrészt át kell vinni alacsonyabb helyiértékre, ha pedig a legalsó helyi értéken akkor az "a" vektor eltolását és összeadását el kell végezni
            {
                if(j == 0) //Ha a b vektorban a legalacsonyabb helyiértékû ulli elem esetén történt az alul csordulás
                {
                    if(!biIsZero(i, iSn)) //Ha a "b" alulcsordulás nem az eredeti 0. bitpozícióban történt (Ha a nulladikban történt az "a" vektort nem kell eltolni úgy kell hozzáadni az r vektorhoz)
                    //if(i != 0) //Ha a "b" alulcsordulás nem az eredeti 0. bitpozícióban történt (Ha a nulladikban történt az "a" vektort nem kell eltolni úgy kell hozzáadni az r vektorhoz)
                    {   //Ez esetben el kell tolni az "a" vektort az aktuális b vektor bit helyiértéknek megfelelõen és összeadni az eltolt értéket az r vektorel
                        //A "a" vektor eltolása a b eltolás számlálójának megfelelõen (Az egyes lépésenkénti túlcsordulások kezelése miatt kell for ciklusban egyesével)
                        unsigned char kSn = 0;

                        for (std::vector<ulli> k {0}; biIsSmaller(k, kSn, bShNr, bShNrSn); biInc(k, kSn))
                        {
                            //A "shhta" vektor szükséges mértékû eltolása balra shl-el a legmagasabb helyiértékû ulli-vel kezdve
                            for (ulli m = a.size(); m>0; m--) //Azért nem a.size()-1 -> >= 0-ig mert ha unsigned a változó akkor átfordul és végtelen ciklusba esik! Így viszont minden indexbe (m-1)-kell
                            {
                                actLowBit = (*(a.data()+(m-1)) & 0x8000000000000000) >> 63; //Az aktuális bal szélső bit értékének megállapítása (ha 1 akkor a következő sorban szereplő aktuális balratolás carryt fog eredményezni)
                                *(a.data()+(m-1)) = *(a.data()+(m-1)) << 1;  //r aktuális ulli elemének 1 bittel balra történõ eltolása

                                if(actLowBit >= 1) //Ha van felülcsordulás azt az alacsonyabb helyi értékrõl egyrészt át kell vinni magasabb helyiértékre, ha pedig a legfelsõ helyi értéken akkor a a vektor egnövelését és átvitelét is el kell végezni
                                {
                                    if((m-1) == (a.size()-1)) //Ha a legmagasabb helyiértékû ulli elem esetén történt a felülcsordulás
                                    {
                                        //Az összes eredeti és a a vektor méretének megnövelése
                                        a.resize(a.size()+1, 1); //a átméretezése és az új elem 1-re inicializálása (Carry)
                                    }
                                    else //Ha a a vektor egyik alacsonyabb helyiértékû ulli elemében történt túlcsordulás
                                    {
                                        *(a.data()+(m-1)+1) = (*(a.data()+(m-1)+1))+1; //Az aktuálisnál egyel nagyobb helyiérték megnövelése egyel (ha nem a legnagyobb helyiértéken volt a felülcsordulás)
                                    }
                                }
                            }
                        }
                    }
                    //Az "r" és "a" összeadása minden ciklusban
                    carry = biAdd(r, rSn, a, aSn, r, rSn); //Az "r" vektorhoz az "a" vektor hozzáadása. Az eredmény az "r" vektorba kerül. A biAdd() a két vektort big_int-ként összeadó függvény

                    biSetToZero(bShNr, bShNrSn); //A "b" vektor alulcsordulása esetén nullázni kell a b eltolás számlálóját
                }
                else //Ha nem a legalacsonyabb helyiértékû ulli elem esetén történt az alul csordulás akkor az alulcsordulást át kell vinni az alacsonyabb helyiértékre
                {
                    *(b.data()+j-1) = *(b.data()+j-1)+0x8000000000000000;
                }
            }
        }
        biInc(bShNr, bShNrSn); //A "b" vektorban a bit jobbra eltolások vektorra értelmezett számát számláló változójának növelése. (tehát az, hogy a vektor összes (*l) elemét eltolja egyel az = 1) Értéke egyenlő az utolsó 1-es bit elérése óta megtett lépések számával
    }
    //Ha negatív akkor az előjelnek megfelelő szám és előjel előállítása
    if (tmprSn) biNegative(r, rSn, r, rSn);
}

//Szorzó függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biMul
(
    const BinBigInt & a,    //Az egyik összeszorzandó számot tartalmazó dinamikus vektor referencia
    const BinBigInt & b,    //A másik összeszorzandó számot tartalmazó dinamikus vektor referencia
    BinBigInt & r           //Az eredményt tartalmazni hivatott dinamikus vektor referencia
)
{
    return biMul(a.v, a.sigNeg, b.v, b.sigNeg, r.v, r.sigNeg); //Carry visszaadása
}

//Szorzó függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biMul
(
    const BinBigInt & b,    //A másik összeszorzandó számot tartalmazó dinamikus vektor referencia
    BinBigInt & r           //Az eredményt tartalmazni hivatott dinamikus vektor referencia
)
{
    return biMul(v, sigNeg, b.v, b.sigNeg, r.v, r.sigNeg); //Carry visszaadása
}

//Comparison function (It operates like the ISA CMP instructions: Subtract b from a and return the carry flag's value.)
//(If a>=b --> 0 FALSE, if a<b --> 1 TRUE)
//Biginteket összehasonlító függvény (Az ISA féle CMP utasításhoz hasonlóan mûködik: Az a-ból kivonja a b-t és a carryflag-et adja vissza.)
//(Ha a>=b --> 0 FALSE, ha a<b --> 1 TRUE)
bool BinBigInt::biCmp
(
    std::vector<ulli> a,    //Az egyik számot tartalmazó dinamikus vektor
    unsigned char aSn,      //Az egyik szám előjele
    std::vector<ulli> b,    //A másik számot tartalmazó dinamikus vektor
    unsigned char bSn       //A másik szám előjele
)
{
    //Ha nem azonos a két szám előjele
    if (aSn != bSn)
    {
        return aSn; //Az "a" előjel értékével mint carryvel kilép (Ha a "-" és b "+" akkor a < b --> 1, ha a "+" és b "-" akkor a > b --> 0)
    }

    //A vektorok méretazonossági kritériumának teljesítése
    biExtendIfShorter(b, bSn, a.size(), b, bSn); //b méretének a-éhoz igazítása ha kisebb mint a mérete
    biExtendIfShorter(a, aSn, b.size(), a, aSn); //a méretének b-éhoz igazítása ha kisebb mint b mérete

    unsigned char carry = 0;
    carry = biSubSF0(a, b, a);

    return carry;
}

//Biginteket összehasonlító függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biCmp
(
    const BinBigInt & a,    //Az egyik összehasonlítandó számot tartalmazó dinamikus vektor referencia
    const BinBigInt & b     //A másik összehasonlítandó számot tartalmazó dinamikus vektor referencia
)
{
    return biCmp(a.v, a.sigNeg, b.v, b.sigNeg); //Carry visszaadása
}

//Biginteket összehasonlító függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biCmp
(
    const BinBigInt & b //A másik összehasonlítandó számot tartalmazó dinamikus vektor referencia
)
{
    return biCmp(v, sigNeg, b.v, b.sigNeg); //Carry visszaadása
}

//Check if equal function
//Egyenlőséget ellenőrző függvény
bool BinBigInt::biIsEqual
(
    const std::vector<ulli> & a,    //Az egyik öszehasonlítandó számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik öszehasonlítandó szám előjele
    const std::vector<ulli> & b,    //A másik öszehasonlítandó számot tartalmazó dinamikus vektor referenciája
    const unsigned char bSn         //A másik öszehasonlítandó szám előjele
)
{
    std::vector<ulli> c {0};  //Az ideiglenes eredmény vektor
    unsigned char csn = 0;

    biSub(a, aSn, b, bSn, c, csn);   //A két paraméter kivonása egymásból
    return biIsZero(c, csn);         //Annak ellenőrzése, hogy az eredmény nulla-e és a logikai eredmény visszaadása
}

//Egyenlőséget ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsEqual
(
    const BinBigInt & a,    //Az egyik öszehasonlítandó számot tartalmazó BinBigInt referenciája
    const BinBigInt & b     //A másik öszehasonlítandó számot tartalmazó BinBigInt referenciája
)
{
    return biIsEqual(a.v, a.sigNeg, b.v, b.sigNeg);
}

//Egyenlőséget ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsEqual
(
    const BinBigInt & b //A másik öszehasonlítandó számot tartalmazó BinBigInt referenciája
)
{
    return biIsEqual(v, sigNeg, b.v, b.sigNeg);
}

//Check if not equal function
//Nem egyenlőséget ellenőrző függvény
bool BinBigInt::biIsNotEqual
(
    const std::vector<ulli> & a, //Az egyik elenőrizendő számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,     //Az egyik elenőrizendő szám előjele
    const std::vector<ulli> & b, //A másik elenőrizendő számot tartalmazó dinamikus vektor referenciája
    const unsigned char bSn      //A másik elenőrizendő szám előjele
)
{
    return !biIsEqual(a, aSn, b, bSn);  //Annak ellenőrzése és negálása, hogy az eredmény egyenlő-e
}

//Egyenlőséget ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsNotEqual
(
    const BinBigInt & a,    //Az egyik elenőrizendő számot tartalmazó BinBigInt referenciája
    const BinBigInt & b     //A másik elenőrizendő számot tartalmazó BinBigInt referenciája
)
{
    return biIsNotEqual(a.v, a.sigNeg, b.v, b.sigNeg);
}

//Egyenlőséget ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsNotEqual
(
    const BinBigInt & b //A másik elenőrizendő számot tartalmazó BinBigInt referenciája
)
{
    return biIsNotEqual(v, sigNeg, b.v, b.sigNeg);
}

//Check if bigger or equal function
//Nagyobb egyenlő mint ellenőrző függvény
bool BinBigInt::biIsBiggerOrEqual
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    const std::vector<ulli> & b,    //A másik számot tartalmazó dinamikus vektor referenciája
    const unsigned char bSn         //A másik szám előjele
)
{
    return !biCmp(a, aSn, b, bSn);  //Annak ellenőrzése, hogy a >= -e mint b
}

//Nagyobb egyenlő mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsBiggerOrEqual
(
    const BinBigInt & a,    //Az összehasonlítandó számot tartalmazó BinBigInt referenciája
    const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
)
{
    return biIsBiggerOrEqual(a.v, a.sigNeg, b.v, b.sigNeg);       //Annak ellenőrzése, hogy a >= -e mint b
}

//Nagyobb egyenlő mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsBiggerOrEqual
(
    const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
)
{
    return biIsBiggerOrEqual(v, sigNeg, b.v, b.sigNeg);       //Annak ellenőrzése, hogy a >= -e mint b
}

//Check if smaller or equal function
//Kisebb egyenlő mint ellenőrző függvény
bool BinBigInt::biIsSmallerOrEqual
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    const std::vector<ulli> & b,    //A másik számot tartalmazó dinamikus vektor referenciája
    const unsigned char bSn         //A másik szám előjele
)
{
    return !biCmp(b, bSn, a, aSn);  //Annak ellenőrzése, hogy a <= -e mint b
}

//Kisebb egyenlő mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsSmallerOrEqual
(
    const BinBigInt & a,    //Az összehasonlítandó számot tartalmazó BinBigInt referenciája
    const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
)
{
    return biIsSmallerOrEqual(a.v, a.sigNeg, b.v, b.sigNeg);       //Annak ellenőrzése, hogy a <= -e mint b
}

//Kisebb egyenlő mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsSmallerOrEqual
(
    const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
)
{
    return biIsSmallerOrEqual(v, sigNeg, b.v, b.sigNeg);       //Annak ellenőrzése, hogy a <= -e mint b
}

//Check if smaller function
//Kisebb mint ellenőrző függvény
bool BinBigInt::biIsSmaller
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    const std::vector<ulli> & b,    //A másik számot tartalmazó dinamikus vektor referenciája
    const unsigned char bSn         //A másik szám előjele
)
{
    return biCmp(a, aSn, b, bSn);   //Annak ellenőrzése, hogy a < -e mint b
}

//Kisebb mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsSmaller
(
    const BinBigInt & a,    //Az összehasonlítandó számot tartalmazó BinBigInt referenciája
    const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
)
{
    return biIsSmaller(a.v, a.sigNeg, b.v, b.sigNeg);   //Annak ellenőrzése, hogy a < -e mint b
}

//Kisebb mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsSmaller
(
    const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
)
{
    return biIsSmaller(v, sigNeg, b.v, b.sigNeg);   //Annak ellenőrzése, hogy a < -e mint b
}

//Check if bigger function
//Nagyobb mint ellenőrző függvény
bool BinBigInt::biIsBigger
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    const std::vector<ulli> & b,    //A másik számot tartalmazó dinamikus vektor referenciája
    const unsigned char bSn         //A másik szám előjele
)
{
    return biCmp(b, bSn, a, aSn);   //Annak ellenőrzése, hogy a > -e mint b
}

//Nagyobb mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsBigger
(
    const BinBigInt & a,    //Az összehasonlítandó számot tartalmazó BinBigInt referenciája
    const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
)
{
    return biCmp(b.v, b.sigNeg, a.v, a.sigNeg); //Annak ellenőrzése, hogy a > -e mint b
}

//Nagyobb mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
bool BinBigInt::biIsBigger
(
    const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
)
{
    return biCmp(b.v, b.sigNeg, v, sigNeg); //Annak ellenőrzése, hogy a > -e mint b
}

//Power function
//Hatványozó függvény
void BinBigInt::biPow
(
    const std::vector<ulli> a,  //A hatványozandó számot (hatvány alapot) tartalmazó dinamikus vektor
    const unsigned char aSn,    //A hatványozandó szám előjele
    const std::vector<ulli> n,  //A hatvány kitevő számot tartalmazó dinamikus vektor
    const unsigned char nSn,    //A hatvány kitevő szám előjele
    std::vector<ulli> & r,      //Az erdemény vektor referenciája
    unsigned char & rSn         //Az eredény szám előjel referenciája
)
{
    const std::vector<ulli> one {1};
    const unsigned char oneSn = 0;

    //r 1-re inicializálása
    biCopy(one, oneSn, r, rSn);

    //A ciklus végrehajtása nélkül eldönthető esetek (Futás gyorsítás miatt)

    const std::vector<ulli> negOne {0xffffffffffffffff};
    const unsigned char negOneSn = 1;

// NOTE (FR#1#): LEBEGŐPONTOSSÁ ALAKÍTÁS ESETÉN TÖRLENDŐ
    //A negatív kitevők kezelése (ez esetben a kitevő ciklus megspórólható [Ami csak integerek esetén igaz!!!])
    //Egész szám és negatív egész kitevő esetén az eredmény értéke csak 0 (ha az alap nagyob mint 1 --> az 1/n < 1 --> 0-ra kerekítődik), +/-1 (ha az alap +/-1) és nem értelmezett (ha az alap 0 --> 0/0) lehet
    if (nSn) //Ha a hatványkitevő előjele "-"
    {
        if (biIsZero(a, aSn)) //Ha az alap 0
        {
            throw std::invalid_argument("biPow(): 0^\"-\" --> 1/0 undetermined"); //Ha az alap nulla a pozitív hatvány is nulla és annak reciproka 1/0 ami nem értelmezett
        }
        else if (biIsEqual(a, aSn, one, aSn)) //Ha az alap +1
        {
            return; //Kilép (r = 1 értékadás korábban megtörtént)
        }
        else if (biIsEqual(a, aSn, negOne, negOneSn)) //Ha az alap -1 (ez esetben a hatványkitevő páros vagy páratlan voltától függ, hogy az eredmény +1 vagy -1)
        {
            const std::vector<ulli> two {2};
            const unsigned char twoSn = 0;

            std::vector<ulli> quo {2};
            unsigned char quoSn = 0;

            std::vector<ulli> rem {2};
            unsigned char remSn = 0;

            biDiv(n, nSn, two, twoSn, quo, quoSn, rem, remSn); //A hatványkitevő osztása 2-vel

            if (biIsZero(rem, remSn)) //Ha a maradék nulla --> a hatvány kitevő páros
            {
                return; //Kilép (r = 1 értékadás korábban megtörtént)
            }
            else //Ha a maradék nem nulla (1) --> a hatvány kitevő páratlan
            {
                biCopy(negOne, negOneSn, r, rSn); //(r = -1)
                return;  //Kilép
            }
        }
        else //Ha az alap nagyobb egyenlő 2 (Az Integer műveletek kerekítés nélkül csonkolnak így már az 1/2 is = 0 ...)
        {
            biSetToZero(r, rSn); //Az eredmény 0
            return; //Kilép
        }

    }

    //Ha az alap 0 vagy +/-1 (ez esetben a kitevő ciklus megspórólható)
    //Ha az alap 0
    if(biIsZero(a, aSn))
    {
        if(biIsZero(n, nSn)) //Ha a kitevő 0 az eredmény nem értelmezett és kivételt dob
        {
            throw std::invalid_argument("biPow(): 0^0 undetermined");
        }
// NOTE (FR#1#): ITT JELENLEG MAGÁNAK A POZITÍV KITEVŐ ESETNEK VIZSGÁLATA (MIVEL JELENLEG CSAK AZ LEHET) ÉS A KÖVETKEZŐ ELSE ÁG KOMPLETTEN (AMI A NEGATÍV ESET) SZÜKSÉGTELEN
//MIVEL A NEGATÍV ESETRE KORÁBBAN VAN KILÉPÉST EREDMÉNYEZŐ VIZSGÁLAT DE HA LEBEGŐPONTOSSÁ ALAKÍTOM AZ TÖRLÉSRE KERÜL ÍGY EZEK SZÜSKÉGESEK LESZNEK ITT...
        else if (nSn == 0)//Ha a kitevő (nem 0 és) pozitív az eredmény 0 és kiléphet
        {
            biSetToZero(r, rSn); //eredmény 0
            return; //Kilép
        }
        else //Ha a kitevő negatív
        {
            throw std::invalid_argument("biPow(): 0^\"-\" --> 1/0 undetermined"); //Ha az alap nulla a hatvány is nulla és annak reciproka 1/0 ami nem értelmezett
        }
    }
    else if (biIsEqual(a, aSn, one, aSn)) //Ha az alap +1
    {
        return; //Kilép (r = 1 értékadás korábban megtörtént)
    }
    else if (biIsEqual(a, aSn, negOne, negOneSn))//Ha az alap -1 (ez esetben a hatványkitevő páros vagy páratlan voltától függ, hogy az eredmény +1 vagy -1)
    {
        const std::vector<ulli> two {2};
        const unsigned char twoSn = 0;

        std::vector<ulli> quo {2};
        unsigned char quoSn = 0;

        std::vector<ulli> rem {2};
        unsigned char remSn = 0;

        biDiv(n, nSn, two, twoSn, quo, quoSn, rem, remSn); //A hatványkitevő osztása 2-vel

        if (biIsZero(rem, remSn)) //Ha a maradék nulla --> a hatvány kitevő páros
        {
            return; //Kilép (r = 1 értékadás korábban megtörtént)
        }
        else //Ha a maradék nem nulla (1) --> a hatvány kitevő páratlan
            biCopy(negOne, negOneSn, r, rSn); //(r = -1)
        {
            return;  //Kilép
        }
    }

    //Ha a hatványkitevő 0 (és az alap nem -> ez korábban vizsgált mely esetben ide nem jut el) az eredmény 1 tehát kiléphet (r = 1 értékadás korábban megtörtént)
    if(biIsZero(n, nSn))
    {
        return; //Kilép (r = 1 értékadás korábban megtörtént)
    }

// NOTE (FR#1#): LEBEGŐPONTOSSÁ ALAKÍTÁS ESETÉN SZÜKSÉGES
/* Ez egész számok esetén szükségtelen mert negatív egész kitevő esetén az eredmény értéke csak 0 (ha az alap nagyob mint 1 --> az 1/n < 1 --> 0-ra kerekítődik), +/-1 (ha az alap +/-1) és nem értelmezett (ha az alap 0 --> 0/0) lehet, így érdemes ehelyett korábban vizsgálni és előállítani (Mivel lehet majd átalakítom lebegőpontos szám alapúvá itthagyom az abs() kódot)
    //Az n eredeti előjelének (nSn) mentése
    ulli tmptnSn = nSn;

    //n abszolút értéke
    biAbs(n, nSn, n, nSn);
//*/
    std::vector<ulli> c {0};
    unsigned char csn = 0;

    biCopy(n, nSn, c, csn);      //a hatványkitevőnek megfelelő lépés számláló

    while(!biCmp(c, csn, one, oneSn)) //Amíg c nagyobb egyenlő mint egy (ez itt pont jó is de amúgy a nullára vizsgálat a cmp()-el nem lehetséges mert a c átfordul)
    {
        biMul(r, rSn, a, aSn, r, rSn);    //r-t megszorozzuk a-val eredmény r-be
        biDec(c, csn); //c csökkentése egyel
    }

// NOTE (FR#1#): LEBEGŐPONTOSSÁ ALAKÍTÁS ESETÉN SZÜKSÉGES
/* Ez egész számok esetén szükségtelen mert negatív egész kitevő esetén az eredmény értéke csak 0 (ha az alap nagyob mint 1 --> az 1/n < 1 --> 0-ra kerekítődik), +/-1 (ha az alap +/-1) és nem értelmezett (ha az alap 0 --> 0/0) lehet, így érdemes ehelyett korábban vizsgálni és előállítani (Mivel lehet majd átalakítom lebegőpontos szám alapúvá itthagyom az abs() kódot)
    if(tmptnSn != 0) //Ha "-" a hatványkitevő
    {
        std::vector<ulli> q {0};
        unsigned char qsn = 0;
        biDiv(one, oneSn, a, aSn, r, rSn, q, qsn); //A reciprok előállítása
    }
//*/
}

//Hatványozó függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biPow
(
    const BinBigInt & a,    //A hatvány alapját tartalmazó dinamikus vektor referencia
    const BinBigInt & n,    //A hatvány kitevőt tartalmazó dinamikus vektor referencia
    BinBigInt & r           //Az eredményt tartalmazni hivatott BinBigInt referenciája
)
{
    biPow(a.v, a.sigNeg, n.v, n.sigNeg, r.v, r.sigNeg);
}

//Hatványozó függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biPow
(
    const BinBigInt & n,    //A hatvány kitevőt tartalmazó dinamikus vektor referencia
    BinBigInt & r           //Az eredményt tartalmazni hivatott BinBigInt referenciája
)
{
    biPow(v, sigNeg, n.v, n.sigNeg, r.v, r.sigNeg);
}

//Hatványozó függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biPow
(
    const BinBigInt & a,    //A hatvány alapját tartalmazó dinamikus vektor referencia
    const BinBigInt & n     //A hatvány kitevőt tartalmazó dinamikus vektor referencia
)
{
    BinBigInt r;            //Az eredményt tartalmazni hivatott BinBigInt
    biPow(a.v, a.sigNeg, n.v, n.sigNeg, r.v, r.sigNeg);
    return r;
}

//Hatványozó függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biPow
(
    const BinBigInt & n     //A hatvány kitevőt tartalmazó dinamikus vektor referencia
)
{
    BinBigInt r;            //Az eredményt tartalmazni hivatott BinBigInt
    biPow(v, sigNeg, n.v, n.sigNeg, r.v, r.sigNeg);
    return r;
}

//Shift left function (If overflow happen then depending on the extend parameter it truncate the content or extend the size of the vector but if extend the vector it gives back the overflow 1 value too)
//In the case of the count of shift is bigger than 1 it gives back the last steps carry value interpreted on the original vector size in the cl! (Carry Last)
//Because n is unsigned type if negative value given the shift operates of the number's two's complements times
//e.g. if it called with ((ulli) -1) value then it's raw vector value is 0xffffffffffffffff (= ulli MAX) so in this case it shifting 18,446,744,073,709,551,615 bits left.
//Biginteket balra eltoló függvény (Ha felülcsordulás történik akkor az extend paramétertõl függõen csonkolja a tartalmat vagy megnöveli a vektor méretet de a megnövelés esetén is ad vissza overflow 1 értéket)
//1-nél nagyobb számú eltolás esetén az eredetei vektorméreten értelmezve visszaadja az utolsó lépés carry értékét is a cl-ben! (Carry Last)
//mivel n unsigned típus negatív szám esetén a szám kettes komplemensének megfelelő számban hajtódik végre
//pl. ha egy ((ulli) -1) értékkel hívjuk meg annak nyers vektor adatértéke 0xffffffffffffffff (= ulli MAX) szóval ez esetben 18,446,744,073,709,551,615 bitnyi eltolást végez
unsigned char BinBigInt::biShl
(
    const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //A szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjele
    const ulli n,                   //Az eltolás mértéke
    unsigned char & cl,             //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték (CarryLast)
    const unsigned char extend      //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    unsigned char overflow = 0;
    ulli actCarry = 0;
    cl = 0;

    //Az r vektor megfelelő méretűre állítása
    if(&a != &r) //Ha a és r paraméterben átvett vektorok nem azonos entitások (mert lehet a bemeneti vektor az eredmény is egyben)
    {
        //Az a lemásolása r-be érték szerint
        biCopy(a, aSn, r, rSn);
    }

    //Ha az eltolás mértéke 0 kilép
    if (n == 0) return overflow;

    //Ha a kapott vektor értéke +0 az eredmény önmaga, az overflow és a carry marad 0 (a -1 esete balra tolásnál nem egyszerűsíthető így mert nullák lépnek be az egyesek helyére)
    if(biIsZero(r, rSn))
    {
        r.resize(1); //1 elemű vektort adok vissza mivel a 0 az érték szempontjából indifferens a vektor méret
        return overflow;
    }

    ulli os = r.size(); //Az eredeti vektor méret

    ulli ns = os; //Az új vektor méret a régi vektorméretre inicializálva

    ulli un = n/(ulliBitSize); //n/64; //Az eltolás lépésszámának osztása 64-el --> a teljes ullielem méretű eltolások száma
    ulli um = n%(ulliBitSize); //n%64; //A 64-el történő osztás maradéka --> a teljes eltolásokat követően szüskéges bit szintű eltolás száma

    if (!extend && un >= os) //Ha csonkolással lett hívva az shl() és több vagy ugyan annyi elemnyit kell eltolni mint amekkora a vektor mérete (ez esetben lényegében nullázódik a vektor)
    {
        if (rSn) //Ha a szám negatív
        {
            //-1 értékű vektor
            std::vector<ulli> negOne {0xffffffffffffffff};
            unsigned char negOneSn = 1;

            if(!biIsEqual(r, rSn, negOne, negOneSn)) overflow = 1;//Ha a kapott vektor értéke nem -1 akkor van túlcsordulás
        }
        else //Ha a szám pozitív
        {
            overflow = 1; //Mivel ha ide jut a kód a vektor biztos nem nulla (a 0 eset korai returnnal kilép) ezért ha a szám pozitív biztos van túlcsordulás
        }

        if (os == un && um == 0) //Ha pont annyit kell eltolni mint amekkora az eredeti vektor akkor a cl-t be kell állítani (ha többet tolnánk akkor maradna 0)
        {                     //7766554433221100
            cl = (*r.data() & 0x0000000000000001); //A cl egynelő a legalacsonyabb helyiértékű ulli legalacsonyabb helyiértékű bitje
        }
        else //Ha többet kell eltolni mint az eredeti vektor
        {
            cl = 0;
        }
        biSetToZero(r, rSn); //Az eredmény egy 0 értékű vektor
    }
    else if (un > 0) //Ha nem csonkolással lett hívva vagy ha ugyan csonkolással lett hívva de kisebb az eltolás mint a vektor mérete tehát valóban szükséges az eltolás megvalósítása
    {
        ns = os + un; //Az új vektorméret egyenlő az eredeti méret + un
        r.resize(ns, 0); //A vektor átméretezése, az új elemek 0-val feltöltése

        //Az egész elemek átmásolása, új nulla elemek beállítása
        for (ulli usi = ns; usi>0; usi--) //cél index léptetése
        {
            //overflow beállítása                                                      //7766554433221100
            if (((!rSn && *(r.data()+(usi-1)-un)) || (rSn && *(r.data()+(usi-1)-un) != 0xffffffffffffffff)) && (usi > os)) overflow = 1;  //Ha a szám ((pozitív ÉS a forrás érték nem 0) VAGY (negatív ÉS a forrás érték nem ff.ff) ÉS a célindex nagyobb mint az eredeti vektorméret) akkor a overflow 1 különben marad 0
            //A cl carry előállítása
            if (!um && usi == (os+1)) cl = (*(r.data()+(usi-1)-un) & 0x0000000000000001); //Ha nincs maradék akkor amikor a célindex az eredeti vektorméretnél egyel nagyobb pozíciót eléri a cl = az aktuális forrás ulli legalsó bitje

            //Az adatok új pozícióba átmozgatása
            if(usi-1 >= un && (extend || (!extend && usi <= os))) //Ha a léptetés során a forrásoldali index (=(usi-1)-un) még nem érte el a vektor legalsó eredeti 0. elemét és vagy kiterjesztéssel lett hívva vagy nem de a cél index már az eredeti vektorra mutat az adatok régi pozícióból új pozícióba másolása
            {
                *(r.data()+(usi-1)) = *(r.data()+(usi-1)-un); //Az új felülről n. elem legyen egyenlő az eredeti n. elemmel
            }
            else //Ha a léptetés során a forrásoldali index már kisebb lenne mint 0 a céloldali elem 0-ra állítása
            {
                *(r.data()+(usi-1)) = 0;
            }
        }
    }

    //Ha van nem egész ulli méretű eltolás rész is,
    if (um)
    {
        ulli fl = 0; //új beveztő elem kitöltő érték inicializálás
                      //7766554433221100
        if (rSn) fl = 0xffffffffffffffff; //Ha a szám negatív a kitöltő érték ff..ff

        ns++;
        r.resize(ns, fl); //A vektor átméretezése egyel nagyobbra és az új elem előjel szerinti feltöltése

        //A maradék bitszámnak megfelelő eltolás elvégzése
        for (ulli umi = ns; umi>0; umi--) //Az új vektor minden elemén
        {
            ulli trm = 0; //Az átvivendő értékeket tartalmazó változó inicializálása

            if (umi > (ns - os)) //Ha az aktuális célpozíció (umi-1) nagyobb egyenlő mint az eredeti vektor végének új pozíciója (Az ezt követő elemek mind nullák)
            //Ez ekvivalens --> if(umi-1 >= un+1) //Ha a léptetés során a forrásoldali index (=(usi-1)-(un+1)) még nem érte el a vektor legalsó eredeti 0. elemét
            {
                trm = (*(r.data()+(umi-2))) >> (64 - um);  //Az aktuális pozíciónál egyel kisebb helyiértékű ulli elem aktuálsian átvivendő részének előállítása az adat jobbra léptetésével
                //overflow előállítása
                if (umi == (os+1) && ((!rSn && trm) || (rSn && trm != (0xffffffffffffffff >> (64 - um))))) overflow = 1; //Ha a célindex az eredeti vektorméretnél egyel nagyobb pozíciónál van és pozitív szám esetén az aktuális trm nem nulla vagy negatív szám esetén az átvitel méretén értelmezve nem ff..ff az overflow 1
                //A carry előállítása
                if (umi == (os+1)) cl = (trm & 0x0000000000000001); //Ha a célindex az eredeti vektorméretnél egyel nagyobb pozíciónál van a cl a trm legalacsonyabb helyiértékű bitje
                //A maradék eltolásból adódó eltolás megvalósítása
                if (extend || (!extend && umi <= os)) *(r.data()+(umi-1)) = (*(r.data()+(umi-1)) << um) + trm;  //Ha kiterjesztéssel lett hívva vagy nem dea cél index már az eredeti vektorra mutat az r aktuális ulli elemének um bittel balra történõ eltolása és az alacsonyabb helyiértékről történő átvitelből származó rész hozzáadása
            }
            else if (umi == (ns - os)) //Az utolsó még ténylegesen eltolandó ulli elem (ha ez a 0. elem akkor a trm-hez érvénytelen helyről venne adatot ezért külön kell kezelni)
            {
                *(r.data()+(umi-1)) = *(r.data()+(umi-1)) << um;
            }
        }
    }
    biLeaderFillerCut(r, rSn, r, rSn); //Esetleges felesleges leader elemek eltávolítása (ha voltak bevezető nullák illetve ha a maradék eltolás csak nullát visz át akkor lehetnek...)

    return overflow;
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShl
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n,               //Az eltolás mértéke
    unsigned char & cl,         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    return BinBigInt::biShl(a.v, a.sigNeg, r.v, r.sigNeg, n, cl, extend);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShl
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n,               //Az eltolás mértéke
    unsigned char & cl,         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    return BinBigInt::biShl(v, sigNeg, r.v, r.sigNeg, n, cl, extend);   //overflow visszaadása
}

unsigned char BinBigInt::biShlNoCl
(
    const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //A szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjele
    const ulli n,                   //Az eltolás mértéke
    const unsigned char extend      //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    unsigned char cl;               //Ebben a függvényverzióban nem használt de az alapfüggvényben használt technikai változó, hogy a paraméterlistában ne kelljen megadni
    return BinBigInt::biShl(a, aSn, r, rSn, n, cl, extend);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlNoCl
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n,               //Az eltolás mértéke
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    return BinBigInt::biShlNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n, extend);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlNoCl
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n,               //Az eltolás mértéke
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    return BinBigInt::biShlNoCl(v, sigNeg, r.v, r.sigNeg, n, extend);   //overflow visszaadása
}

//Biginteket balra eltoló függvény: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlNoClNoOf
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    const ulli n,               //Az eltolás mértéke
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    BinBigInt r;                //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n, extend);
    return r;
}

//Biginteket balra eltoló függvény: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlNoClNoOf
(
    const ulli n,               //Az eltolás mértéke
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    BinBigInt r;                //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlNoCl(v, sigNeg, r.v, r.sigNeg, n, extend);
    return r;
}

unsigned char BinBigInt::biShlExtend
(
    const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //A szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjele
    const ulli n,                   //Az eltolás mértéke
    unsigned char & cl              //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték (CarryLast)
)
{
    return BinBigInt::biShl(a, aSn, r, rSn, n, cl, 1);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlExtend
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n,               //Az eltolás mértéke
    unsigned char & cl          //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    return BinBigInt::biShlExtend(a.v, a.sigNeg, r.v, r.sigNeg, n, cl);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlExtend
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n,               //Az eltolás mértéke
    unsigned char & cl          //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    return BinBigInt::biShlExtend(v, sigNeg, r.v, r.sigNeg, n, cl);   //overflow visszaadása
}

unsigned char BinBigInt::biShlExtendNoCl
(
    const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //A szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjele
    const ulli n                    //Az eltolás mértéke
)
{
    unsigned char cl;               //Ebben a függvényverzióban nem használt de az alapfüggvényben használt technikai változó, hogy a paraméterlistában ne kelljen megadni
    return BinBigInt::biShl(a, aSn, r, rSn, n, cl, 1);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlExtendNoCl
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n                //Az eltolás mértéke
)
{
    return BinBigInt::biShlExtendNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlExtendNoCl
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n                //Az eltolás mértéke
)
{
    return BinBigInt::biShlExtendNoCl(v, sigNeg, r.v, r.sigNeg, n);   //overflow visszaadása
}

//Biginteket balra eltoló függvény: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlExtendNoClNoOf
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    const ulli n                //Az eltolás mértéke
)
{
    BinBigInt r;                //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlExtendNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n);
    return r;
}

//Biginteket balra eltoló függvény: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlExtendNoClNoOf
(
    const ulli n                //Az eltolás mértéke
)
{
    BinBigInt r;                //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlExtendNoCl(v, sigNeg, r.v, r.sigNeg, n);
    return r;
}

unsigned char BinBigInt::biShlTruncate
(
    const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //A szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjele
    const ulli n,                   //Az eltolás mértéke
    unsigned char & cl             //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték (CarryLast)
)
{
    return BinBigInt::biShl(a, aSn, r, rSn, n, cl, 0);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlTruncate
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n,               //Az eltolás mértéke
    unsigned char & cl         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    return BinBigInt::biShlTruncate(a.v, a.sigNeg, r.v, r.sigNeg, n, cl);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlTruncate
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n,               //Az eltolás mértéke
    unsigned char & cl         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    return BinBigInt::biShlTruncate(v, sigNeg, r.v, r.sigNeg, n, cl);   //overflow visszaadása
}

unsigned char BinBigInt::biShlTruncateNoCl
(
    const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //A szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjele
    const ulli n                    //Az eltolás mértéke
)
{
    unsigned char cl;               //Ebben a függvényverzióban nem használt de az alapfüggvényben használt technikai változó, hogy a paraméterlistában ne kelljen megadni
    return BinBigInt::biShl(a, aSn, r, rSn, n, cl, 0);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlTruncateNoCl
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n                //Az eltolás mértéke
)
{
    return BinBigInt::biShlTruncateNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlTruncateNoCl
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n                //Az eltolás mértéke
)
{
    return BinBigInt::biShlTruncateNoCl(v, sigNeg, r.v, r.sigNeg, n);   //overflow visszaadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlTruncateNoClNoUf
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    const ulli n                //Az eltolás mértéke
)
{
    BinBigInt r;                //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlTruncateNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n);
    return r;
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlTruncateNoClNoUf
(
    const ulli n                //Az eltolás mértéke
)
{
    BinBigInt r;                //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlTruncateNoCl(v, sigNeg, r.v, r.sigNeg, n);
    return r;
}

//Biginteket balra eltoló függvény - OVERRIDE: Az eltolás lépészáma is vektor
//(Ha felülcsordulás történik akkor a megadott paramétertõl függõen csonkol vagy megnöveli a vektor méretet de a megnövelés esetén során is ad vissza overflow értéket)
//1-nél nagyobb számú eltolás esetén az eredetei vektorméreten értelmezve visszaadja az utolsó lépés carry értékét is a cl-ben! (Carry Last)
//n vektor szándékosan csak előjel nélkül adható meg, ennek megfelelően a vektor nyers adatai szerinti eltolást végez a függvény, tehát ha negatív számhoz tartozó nyers vektort adunk meg akkor a kettes komplemens szerint léptet
//pl. ha egy -1 értére definiált BinBigInt vektor tagjával hívjuk meg annak nyers vektor adatértéke 0xffffffffffffffff (= ulli MAX) szóval ez esetben 18,446,744,073,709,551,615 bitnyi eltolást végez
//A BinBigInt overrideok negatív számmal való meghívás esetén kivétlet dobnak
unsigned char BinBigInt::biShl
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjel referenciája
    const std::vector<ulli> n,      //Az eltolás mértéke
    unsigned char & cl,             //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
    const unsigned char extend      //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    unsigned char overflow = 0;
    ulli actCarry = 0;
    cl = 0;

    unsigned char nSn = 0;

    //Az r vektor megfelelő méretűre állítása
    if(&a != &r) //Ha a és r paraméterben átvett vektorok nem azonos entitások (mert lehet a bemeneti vektor az eredmény is egyben)
    {
        //Az a lemásolása r-be érték szerint
        biCopy(a, aSn, r, rSn);
    }

    //Ha az eltolás mértéke 0 kilép
    if (biIsZero(n, nSn)) return 0;

    //Ha a kapott vektor értéke +0 az eredmény önmaga, az overflow és a carry marad 0 (a -1 esete balra tolásnál nem egyszerűsíthető így mert nullák lépnek be az egyesek helyére)
    if(biIsZero(r, rSn))
    {
        r.resize(1); //1 elemű vektort adok vissza mivel a 0 az érték szempontjából indifferens a vektor méret
        return overflow;
    }

    ulli os = r.size(); //Az "r" (="a") vektor eredeti byte mérete

    ulli ns = os; //Az új vektor méret a régi vektorméretre inicializálva

    std::vector<ulli> vun {0}; //A teljes ulli elem méretű eltolások számát tároló vektor = Az eltolás lépésszáma osztva vektor alaptípusának (ulli) bitméretével (=n/64)
    unsigned char vunSn = 0; //un előjele

    std::vector<ulli> vum {0}; //A teljes eltolásokat követően szüskéges (maradék) bit szintű eltolás számát tároló vektor (=n%64)
    unsigned char vumsn = 0; //um előjele

    biDiv(n, nSn, vUlliBitSize, ulliBitSizeSn, vun, vunSn, vum, vumsn); //n osztása a vektor alaptípusának (ulli) bitméretével ( vUlliBitSizeSn=64), a hányados visszaadása az vun-be, a maradék a vum-be

    std::vector<ulli> vos {os}; //Az "r" (="a") vektor eredeti byte méretét tároló vektor
    unsigned char vossn = 0; //vos előjele

    std::vector<ulli> vobs {r.size()*ulliBitSize}; //Az "r" (="a") vektor eredeti bit méretét tároló vektor (=r.size()*64)
    unsigned char vobssn = 0; //robs előjele

    //Mivel un nem lehet nagyobb mint az elméleti címméret (= ulli_MAX) és um nem lehet nagyobb mint az ulli alaptípus bitszáma (=64) ezért ezeket vissza konvertálom ulli-ra

    //Annak ellenőrzése, hogy a ulli elemek szerinti lépésszám nem nagyobb-e mint a rendszer címzés mérete (azaz n értéke nem volt-e nagyobb mint a max címméret*64)
    biLeaderFillerCut(vun, vunSn, vun, vunSn); //A leader nulla elemek eltávolítása

    //Ha a vun nyers mérete nagyobb mint 1 akkor az értéke nagyobb mint az std::numeric_limits<ulli>::max tehát elméletileg sem képes a rendszer megcímezni
    if (vun.size() > 1) throw std::invalid_argument("biShl(): n bigger than the theoretical maximum 'adressable bitcount' (=std::numeric_limits<ulli>::max*sizeof(ulli)*8) the n should not be bigger then the effective 'adressable bitcount'");

    biLeaderFillerCut(vum, vumsn, vum, vumsn); //A leader nulla elemek eltávolítása
    if (vum.size() > 1 || *vum.data() >= ulliBitSize) throw std::invalid_argument("biShl(): divide error. vum value should be less than :max*sizeof(ulli)*8");

    ulli un = *vun.data(); //A teljes ulli elem méretű eltolások száma
    ulli um = *vum.data(); //A teljes eltolásokat követően szüskéges (maradék) bit szintű eltolások száma

    //if (!extend && r.size() <= un) //Ha csonkolással lett hívva az shl() és több vagy ugyan annyi elemnyit kell eltolni mint amekkora a vektor mérete (ez esetben lényegében nullázzódik a vektor)
    if (!extend && un >= os) //Ha csonkolással lett hívva az shl() és több vagy ugyan annyi elemnyit kell eltolni mint amekkora a vektor mérete (ez esetben lényegében nullázzódik a vektor)
    {

        if (rSn) //Ha a szám negatív
        {
            //-1 értékű vektor
            std::vector<ulli> negOne {0xffffffffffffffff};
            unsigned char negOneSn = 1;

            if(!biIsEqual(r, rSn, negOne, negOneSn)) overflow = 1;//Ha a kapott vektor értéke nem -1 akkor van túlcsordulás
        }
        else //Ha a szám pozitív
        {
            overflow = 1; //Mivel ha ide jut a kód a vektor biztos nem nulla (a 0 eset korai returnnal kilép) ezért ha a szám pozitív biztos van túlcsordulás
        }

        if (os == un && um == 0) //Ha pont annyit kell eltolni mint amekkora az eredeti vektor akkor a cl-t be kell állítani (ha többet tolnánk akkor maradna 0)
        {                     //7766554433221100
            cl = (*r.data() & 0x0000000000000001); //A cl egynelő a legalacsonyabb helyiértékű ulli legalacsonyabb helyiértékű bitje
        }
        else //Ha többet kell eltolni mint az eredeti vektor
        {
            cl = 0;
        }
        biSetToZero(r, rSn); //Az eredmény egy 0 értékű vektor
    }
    else  if (un > 0) //Ha nem csonkolással lett hívva vagy ha ugyan csonkolással lett hívva de kisebb az eltolás mint a vektor mérete tehát valóban szükséges az eltolás megvalósítása
    {


        ns = os + un; //Az új vektorméret egyenlő az eredeti méret + un
        r.resize(ns, 0); //A vektor átméretezése, az új elemek 0-val feltöltése

        //Az egész elemek átmásolása, új nulla elemek beállítása
        for (ulli usi = ns; usi>0; usi--)
        {
            //overflow beállítása                                                      //7766554433221100
            if (((!rSn && *(r.data()+(usi-1)-un)) || (rSn && *(r.data()+(usi-1)-un) != 0xffffffffffffffff)) && (usi > os)) overflow = 1;  //Ha a szám ((pozitív ÉS a forrás érték nem 0) VAGY (negatív ÉS a forrás érték nem ff.ff) ÉS a célindex nagyobb mint az eredeti vektorméret) akkor a overflow 1 különben marad 0
            //A cl carry előállítása
            if (!um && usi == (os+1)) cl = (*(r.data()+(usi-1)-un) & 0x0000000000000001); //Ha nincs maradék akkor amikor a célindex az eredeti vektorméretnél egyel nagyobb pozíciót eléri a cl = az aktuális forrás ulli legalsó bitje

            //Az adatok új pozícióba átmozgatása
            if(usi-1 >= un && (extend || (!extend && usi <= os))) //Ha a léptetés során a forrásoldali index (=(usi-1)-un) még nem érte el a vektor legalsó eredeti 0. elemét és vagy kiterjesztéssel lett hívva vagy nem de a cél index már az eredeti vektorra mutat az adatok régi pozícióból új pozícióba másolása
            {
                *(r.data()+(usi-1)) = *(r.data()+(usi-1)-un); //Az új felülről n. elem legyen egyenlő az eredeti n. elemmel
            }
            else //Ha a léptetés során a forrásoldali index már kisebb lenne mint 0 a céloldali elem 0-ra állítása
            {
                *(r.data()+(usi-1)) = 0;
            }
        }
    }

    //Ha van nem egész ulli méretű eltolás rész is,
    if (um)
    {
        ulli fl = 0; //új beveztő elem kitöltő érték inicializálás
                      //7766554433221100
        if (rSn) fl = 0xffffffffffffffff; //Ha a szám negatív a kitöltő érték ff..ff

        ns++;
        r.resize(ns, fl); //A vektor átméretezése egyel nagyobbra és az új elem előjel szerinti feltöltése

        //A maradék bitszámnak megfelelő eltolás elvégzése
        for (ulli umi = ns; umi>0; umi--) //Az új vektor minden elemén
        {
            ulli trm = 0; //Az átvivendő értékeket tartalmazó változó inicializálása

            if (umi > (ns - os)) //Ha az aktuális célpozíció (umi-1) nagyobb egyenlő mint az eredeti vektor végének új pozíciója (Az ezt követő elemek mind nullák)
            //Ez ekvivalens --> if(umi-1 >= un+1) //Ha a léptetés során a forrásoldali index (=(usi-1)-(un+1)) még nem érte el a vektor legalsó eredeti 0. elemét
            {
                trm = (*(r.data()+(umi-2))) >> (64 - um);  //Az aktuális pozíciónál egyel kisebb helyiértékű ulli elem aktuálsian átvivendő részének előállítása az adat jobbra léptetésével
                //overflow előállítása
                if (umi == (os+1) && ((!rSn && trm) || (rSn && trm != (0xffffffffffffffff >> (64 - um))))) overflow = 1; //Ha a célindex az eredeti vektorméretnél egyel nagyobb pozíciónál van és pozitív szám esetén az aktuális trm nem nulla vagy negatív szám esetén az átvitel méretén értelmezve nem ff..ff az overflow 1
                //A carry előállítása
                if (umi == (os+1)) cl = (trm & 0x0000000000000001); //Ha a célindex az eredeti vektorméretnél egyel nagyobb pozíciónál van a cl a trm legalacsonyabb helyiértékű bitje
                //A maradék eltolásból adódó eltolás megvalósítása
                if (extend || (!extend && umi <= os)) *(r.data()+(umi-1)) = (*(r.data()+(umi-1)) << um) + trm;  //Ha kiterjesztéssel lett hívva vagy nem dea cél index már az eredeti vektorra mutat az r aktuális ulli elemének um bittel balra történõ eltolása és az alacsonyabb helyiértékről történő átvitelből származó rész hozzáadása
            }
            else if (umi == (ns - os)) //Az utolsó még ténylegesen eltolandó ulli elem (ha ez a 0. elem akkor a trm-hez érvénytelen helyről venne adatot ezért külön kell kezelni)
            {
                *(r.data()+(umi-1)) = *(r.data()+(umi-1)) << um;
            }
        }
    }
    biLeaderFillerCut(r, rSn, r, rSn); //Esetleges felesleges leader elemek eltávolítása (ha voltak bevezető nullák illetve ha a maradék eltolás csak nullát visz át akkor lehetnek...)

    return overflow;
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShl
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n,        //Az eltolás mértékének referenciája
    unsigned char & cl,         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShl(a.v, a.sigNeg, r.v, r.sigNeg, n.v, cl, extend); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShl
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n,        //Az eltolás mértékének referenciája
    unsigned char & cl,         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShl(v, sigNeg, r.v, r.sigNeg, n.v, cl, extend); //overflow visszadása
}

unsigned char BinBigInt::biShlNoCl
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjel referenciája
    const std::vector<ulli> n,      //Az eltolás mértéke
    const unsigned char extend      //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    unsigned char cl;               //Ebben a függvényverzióban nem használt de az alapfüggvényben használt technikai változó, hogy a paraméterlistában ne kelljen megadni
    return BinBigInt::biShl(a, aSn, r, rSn, n, cl, extend); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlNoCl
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n,        //Az eltolás mértékének referenciája
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShlNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n.v, extend); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlNoCl
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n,        //Az eltolás mértékének referenciája
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShlNoCl(v, sigNeg, r.v, r.sigNeg, n.v, extend); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlNoClNoOf
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    const BinBigInt & n,        //Az eltolás mértékének referenciája
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    BinBigInt r;    //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n.v, extend);
    return r;
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlNoClNoOf
(
    const BinBigInt & n,        //Az eltolás mértékének referenciája
    const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    BinBigInt r;    //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlNoCl(v, sigNeg, r.v, r.sigNeg, n.v, extend);
    return r;
}

unsigned char BinBigInt::biShlExtend
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjel referenciája
    const std::vector<ulli> n,      //Az eltolás mértéke
    unsigned char & cl             //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
)
{
    return BinBigInt::biShl(a, aSn, r, rSn, n, cl, 1); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlExtend
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n,        //Az eltolás mértékének referenciája
    unsigned char & cl         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShlExtend(a.v, a.sigNeg, r.v, r.sigNeg, n.v, cl); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlExtend
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n,        //Az eltolás mértékének referenciája
    unsigned char & cl         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShlExtend(v, sigNeg, r.v, r.sigNeg, n.v, cl); //overflow visszadása
}

unsigned char BinBigInt::biShlExtendNoCl
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjel referenciája
    const std::vector<ulli> n       //Az eltolás mértéke
)
{
    unsigned char cl;               //Ebben a függvényverzióban nem használt de az alapfüggvényben használt technikai változó, hogy a paraméterlistában ne kelljen megadni
    return BinBigInt::biShl(a, aSn, r, rSn, n, cl, 1); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlExtendNoCl
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n        //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShlExtendNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n.v); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlExtendNoCl
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n        //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShlExtendNoCl(v, sigNeg, r.v, r.sigNeg, n.v); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlExtendNoClNoOf
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    const BinBigInt & n        //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    BinBigInt r;    //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlExtendNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n.v);
    return r;
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlExtendNoClNoOf
(
    const BinBigInt & n        //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    BinBigInt r;    //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlExtendNoCl(v, sigNeg, r.v, r.sigNeg, n.v);
    return r;
}

unsigned char BinBigInt::biShlTruncate
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjel referenciája
    const std::vector<ulli> n,      //Az eltolás mértéke
    unsigned char & cl             //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
)
{
    return BinBigInt::biShl(a, aSn, r, rSn, n, cl, 0); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlTruncate
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n,        //Az eltolás mértékének referenciája
    unsigned char & cl         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShlTruncate(a.v, a.sigNeg, r.v, r.sigNeg, n.v, cl); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlTruncate
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n,        //Az eltolás mértékének referenciája
    unsigned char & cl         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShlTruncate(v, sigNeg, r.v, r.sigNeg, n.v, cl); //overflow visszadása
}

unsigned char BinBigInt::biShlTruncateNoCl
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjel referenciája
    const std::vector<ulli> n       //Az eltolás mértéke
)
{
    unsigned char cl;               //Ebben a függvényverzióban nem használt de az alapfüggvényben használt technikai változó, hogy a paraméterlistában ne kelljen megadni
    return BinBigInt::biShl(a, aSn, r, rSn, n, cl, 0); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlTruncateNoCl
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n        //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShlTruncateNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n.v); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShlTruncateNoCl
(
    BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n        //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    return BinBigInt::biShlTruncateNoCl(v, sigNeg, r.v, r.sigNeg, n.v); //overflow visszadása
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlTruncateNoClNoUf
(
    const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
    const BinBigInt & n        //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    BinBigInt r;    //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlTruncateNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n.v);
    return r;
}

//Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShlTruncateNoClNoUf
(
    const BinBigInt & n        //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShl(): the n shouldn't be negative");
    BinBigInt r;    //Az eredményt tartalmazni hivatott BinBigInt
    BinBigInt::biShlTruncateNoCl(v, sigNeg, r.v, r.sigNeg, n.v);
    return r;
}

//Shift right function (If underflow happen then truncate the content. It returns the carry value which happen in the last step at the cl.)
//Biginteket jobbra eltoló függvény (Ha alulcsordulás történik akkor a kilépõ biteket csonkolja. Az utolsó lépésben bekövetkezett carry értéket a cl-ben adja vissza.)
unsigned char BinBigInt::biShr
(
    const std::vector<ulli> & a, //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,     //Az egyik szám előjele
    std::vector<ulli> & r,       //Az erdemény vektor referenciája
    unsigned char & rSn,         //Az erdmény előjele
    const ulli n,                //Az eltolás mértéke
    unsigned char & cl           //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
)
{
    unsigned char underflow = 0;
    ulli actCarry = 0;
    cl = 0;

    //Az r vektor megfelelő méretűre állítása
    if(&a != &r) //Ha a és r paraméterben átvett vektorok nem azonos entitások (mert lehet a bemeneti vektor az eredmény is egyben)
    {
        //Az a lemásolása r-be érték szerint
        biCopy(a, aSn, r, rSn);
    }

    //Ha az eltolás mértéke 0 kilép,
    if (n == 0) return underflow;

    //-1 értékű vektor
    std::vector<ulli> negOne {0xffffffffffffffff};
    unsigned char negOneSn = 1;

    //Ha a kapott vektor értéke +0 vagy -1 az eredmény önmaga, underflow marad 0, cl carry egyenlő az előjellel
    if(biIsZero(r, rSn) || biIsEqual(r, rSn, negOne, negOneSn))
    {
        cl = rSn;
        r.resize(1); //1 elemű vektort adok vissza mivel a 0 és a -1 az érték szempontjából indifferens a vektor méret
        return underflow;
    }

    ulli os = r.size(); //Az eredeti vektor méret

    ulli un = n/(ulliBitSize); //n/64; //Az eltolás lépésszámának osztása 64-el --> a teljes ullielem méretű eltolások száma
    ulli um = n%(ulliBitSize); //n%64; //A 64-el történő osztás maradéka --> a teljes eltolásokat követően szüskéges bit szintű eltolás száma

    if (un >= os) //Ha több vagy ugyan annyi elemnyit kell eltolni mint amekkora a vektor mérete (ez esetben lényegében nullázzódik vagy -1-eződik a vektor)
    {
        underflow = 1; //Az underflow 1 (Mivel a 0 és -1 eset korábban vizsgált és korai kilépéssel végződik így ha ide ér biztos lesz előjellel ellentétes kilépő bit a teljes vektorban valahol)
        if (os == un && um == 0) //Ha pont annyit kell eltolni mint amekkora az eredeti vektor akkor a cl-t be kell állítani (ha többet tolnánk akkor maradna 0)
        {                     //7766554433221100
            cl = (*(r.data()+os-1) >> 63); //A cl egynelő a legmagasabb helyiértékű ulli legmagasabb helyiértékű bitje
        }
        else //Ha többet kell eltolni mint az eredeti vektor
        {
            cl = rSn;
        }

        //Az eredmény vektor előállítása
        if (rSn) //Ha negatív a szám
        {
            biCopy(negOne, negOneSn, r, rSn); //A -1 vektor másolása, mivel a negOne 1 elemű az eredmény is 1 elemű ami jó mert a 0 és a -1 az érték szempontjából indifferens a vektor méret
        }
        else
        {
            r.resize(1); //1 elemű vektort adok vissza mivel a 0 és a -1 az érték szempontjából indifferens a vektor méret
            biSetToZero(r, rSn); //Az eredmény egy 0 értékű vektor
        }
    }
    else if (un > 0) //Ha valóban el kell végezni az eltolást
    {
        //Az egész elemek átmásolása, új nulla elemek beállítása
        for (ulli usi = 0; usi < os; usi++)
        {
            //underflow beállítása                                                      //7766554433221100
            if (((!rSn && *(r.data()+usi)) || (rSn && *(r.data()+usi) != 0xffffffffffffffff)) && (usi < un)) underflow = 1;  //Ha a szám ((pozitív ÉS az eredeti érték nem 0) VAGY (negatív ÉS az eredeti érték nem ff.ff) ÉS az index kisebb mint az eltolás akkor a underflow 1 különben marad 0

            //A cl carry előállítása
            if (!um && usi == un-1) cl = (*(r.data()+usi) >> 63); //Ha nincs maradék akkor amikor az index az eltolás mértékénél egyel kisebb pozíciót eléri a cl = az aktuális erdeti ulli legfelső bitje

            //Az adatok új pozícióba átmozgatása
            if (usi < os-un) //Amíg az index a felülírandó tartományban van
            {
                *(r.data()+usi) = *(r.data()+usi+un); //Az alulról n. elem legyen egyenlő az eredeti n+un. elemmel
            }
            else //Az utolsó átmozgatás után a még fennmaradó elmeket 0-val vagy ff.ff-el kell feltölteni
            {
                if(rSn) //Ha a szám negatív
                {
                    *(r.data()+usi) = 0xffffffffffffffff;
                }
                else //Ha a szám pozitív
                {
                    *(r.data()+usi) = 0;
                }
            }
        }
    }

    //Ha van nem egész ulli méretű eltolás rész is,
    if (um)
    {
        //A maradék bitszámnak megfelelő eltolás elvégzése
        for (ulli umi = 0; umi < os; umi++) //A vektor minden elemén
        {
            ulli trm = 0; //Az átvivendő értékeket tartalmazó változó inicializálása

            if (umi < os-un) //Ha az aktuális pozíció még az eltolt eredeti értékeket tartalmazó szakaszra mutat
            {
                trm = (*(r.data()+(umi+1))) << (64-um);  //Az aktuális pozíciónál egyel nagyobb helyiértékű ulli elem aktuálsian átvivendő részének előállítása az adat balra léptetésével
                //overflow előállítása
                if (!umi && ((!rSn && (*r.data() << (64-um))) || (rSn && (*r.data() << (64-um)) != (0xffffffffffffffff << (64-um))))) underflow = 1; //Ha a 0. pozíciónál van és pozitív szám esetén az aktuális adatból kiléptetendő érték nem nulla vagy negatív szám esetén az átvitel méretén értelmezve nem ff..ff az underflow 1
                //A carry előállítása
                if (!umi) cl = (*r.data() >> (um-1) & 0x0000000000000001); //Ha a 0. pozíciónál van a cl az aktuális adat szükséges eltolásnál egy bittel kisebb eltolásának legalacsonyabb helyiértékű bitje
                //A maradék eltolásból adódó eltolás megvalósítása
                if (umi < os-1) //Ha az aktuális pozíció még a legnagyobb helyiérték alatt van
                {
                    *(r.data()+umi) = (*(r.data()+umi) >> um) + trm;  //Az r aktuális ulli elemének um bittel jobbra történõ eltolása és a magasabb helyiértékről történő átvitelből származó rész hozzáadása
                }
                else //Ha az aktuális pozíció a legnagyobb helyiértéken van (ez csak akkor lehet ha nem volt egész eltolás tehát ha un=0)
                {
                    if (rSn) //Ha a szám negatív
                    {
                        *(r.data()+umi) = (*(r.data()+umi) >> um) + (0xffffffffffffffff << (64-um));  //Az r aktuális ulli elemének um bittel jobbra történõ eltolása és a szükséges fff-ek hozzáadása
                    }
                    else //Ha a szám pozitív
                    {
                        *(r.data()+umi) = (*(r.data()+umi) >> um);  //Az r aktuális ulli elemének um bittel jobbra történõ eltolása
                    }
                }
            }
        }
    }
    biLeaderFillerCut(r, rSn, r, rSn); //Esetleges felesleges leader elemek eltávolítása (ha voltak bevezető nullák illetve ha a maradék eltolás csak nullát visz át akkor lehetnek...)

    return underflow;
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShr
(
    const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n,           //Az eltolás mértéke
    unsigned char & cl      //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    if (n < 0) throw std::invalid_argument("biShr(): the n shouldn't be negative");
    return biShr(a.v, a.sigNeg, r.v, r.sigNeg, n, cl);  //underflow visszaadása
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShr
(
    BinBigInt & r,      //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n,       //Az eltolás mértéke
    unsigned char & cl  //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    if (n < 0) throw std::invalid_argument("biShr(): the n shouldn't be negative");
    return biShr(v, sigNeg, r.v, r.sigNeg, n, cl);  //underflow visszaadása
}

//Biginteket jobbra eltoló függvény az utolsó lépésben történt eltolás underflow érétkét adja vissza (Ha alulcsordulás történik akkor a kilépõ biteket csonkolja)
unsigned char BinBigInt::biShrNoCl
(
    const std::vector<ulli> & a, //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,     //Az egyik szám előjele
    std::vector<ulli> & r,       //Az erdemény vektor referenciája
    unsigned char & rSn,         //Az erdmény előjele
    const ulli n                 //Az eltolás mértéke
)
{
    unsigned char cl;           //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
    return biShr(a, aSn, r, rSn, n, cl);  //underflow visszaadása
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShrNoCl
(
    const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n            //Az eltolás mértéke
)
{
    if (n < 0) throw std::invalid_argument("biShr(): the n shouldn't be negative");
    return biShrNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n);  //underflow visszaadása
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShrNoCl
(
    BinBigInt & r,      //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const ulli n        //Az eltolás mértéke
)
{
    if (n < 0) throw std::invalid_argument("biShr(): the n shouldn't be negative");
    return biShrNoCl(v, sigNeg, r.v, r.sigNeg, n);  //underflow visszaadása
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShrNoClNoUf
(
    const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
    const ulli n            //Az eltolás mértéke
)
{
    if (n < 0) throw std::invalid_argument("biShr(): the n shouldn't be negative");
    BinBigInt r;          //Az eredményt tartalmazni hivatott BinBigInt
    biShrNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n);
    return r;
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShrNoClNoUf
(
    const ulli n       //Az eltolás mértéke
)
{
    if (n < 0) throw std::invalid_argument("biShr(): the n shouldn't be negative");
    BinBigInt r;          //Az eredményt tartalmazni hivatott BinBigInt
    biShrNoCl(v, sigNeg, r.v, r.sigNeg, n);
    return r;
}

//Biginteket jobbra eltoló függvény - OVERRIDE: Az eltolás lépészáma is vektor
//az utolsó lépésben történt eltolás underflow érétkét adja vissza (Ha alulcsordulás történik akkor a kilépõ biteket csonkolja)
unsigned char BinBigInt::biShr
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjel referenciája
    const std::vector<ulli> n,      //Az eltolás mértéke
    unsigned char & cl              //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
)
{
    unsigned char underflow = 0;
    ulli actCarry = 0;
    cl = 0;
    unsigned char nSn = 0;

    //Az r vektor megfelelő méretűre állítása
    if(&a != &r) //Ha a és r paraméterben átvett vektorok nem azonos entitások (mert lehet a bemeneti vektor az eredmény is egyben)
    {
        //Az a lemásolása r-be érték szerint
        biCopy(a, aSn, r, rSn);
    }

    //Ha az eltolás mértéke 0 kilép,
    if (biIsZero(r, rSn)) return underflow;

    //-1 értékű vektor
    std::vector<ulli> negOne {0xffffffffffffffff};
    unsigned char negOneSn = 1;

    //Ha a kapott vektor értéke +0 vagy -1 az eredmény önmaga, underflow marad 0, cl carry egyenlő az előjellel
    if(biIsZero(r, rSn) || biIsEqual(r, rSn, negOne, negOneSn))
    {
        cl = rSn;
        r.resize(1); //1 elemű vektort adok vissza mivel a 0 és a -1 az érték szempontjából indifferens a vektor méret
        return underflow;
    }

    ulli os = r.size(); //Az eredeti vektor méret

    std::vector<ulli> vun {0}; //A teljes ulli elem méretű eltolások számát tároló vektor = Az eltolás lépésszáma osztva vektor alaptípusának (ulli) bitméretével (=n/64)
    unsigned char vunSn = 0; //un előjele

    std::vector<ulli> vum {0}; //A teljes eltolásokat követően szüskéges (maradék) bit szintű eltolás számát tároló vektor (=n%64)
    unsigned char vumsn = 0; //um előjele

    biDiv(n, nSn, vUlliBitSize, ulliBitSizeSn, vun, vunSn, vum, vumsn); //n osztása a vektor alaptípusának (ulli) bitméretével ( vUlliBitSizeSn=64), a hányados visszaadása az vun-be, a maradék a vum-be

    std::vector<ulli> vos {os}; //Az "r" (="a") vektor eredeti byte méretét tároló vektor
    unsigned char vossn = 0; //vos előjele

    std::vector<ulli> vobs {r.size()*ulliBitSize}; //Az "r" (="a") vektor eredeti bit méretét tároló vektor (=r.size()*64)
    unsigned char vobssn = 0; //robs előjele

    //Mivel un nem lehet nagyobb mint az elméleti címméret (= ulli_MAX) és um nem lehet nagyobb mint az ulli alaptípus bitszáma (=64) ezért ezeket vissza konvertálom ulli-ra

    //Annak ellenőrzése, hogy a ulli elemek szerinti lépésszám nem nagyobb-e mint a rendszer címzés mérete (azaz n értéke nem volt-e nagyobb mint a max címméret*64)
    biLeaderFillerCut(vun, vunSn, vun, vunSn); //A leader nulla elemek eltávolítása

    //Ha a vun nyers mérete nagyobb mint 1 akkor az értéke nagyobb mint az std::numeric_limits<ulli>::max tehát elméletileg sem képes a rendszer megcímezni
    if (vun.size() > 1) throw std::invalid_argument("biShl(): n bigger than the theoretical maximum 'adressable bitcount' (=std::numeric_limits<ulli>::max*sizeof(ulli)*8) the n should not be bigger then the effective 'adressable bitcount'");

    biLeaderFillerCut(vum, vumsn, vum, vumsn); //A leader nulla elemek eltávolítása
    if (vum.size() > 1 || *vum.data() >= ulliBitSize) throw std::invalid_argument("biShl(): divide error. vum value should be less than :max*sizeof(ulli)*8");

    ulli un = *vun.data(); //A teljes ulli elem méretű eltolások száma
    ulli um = *vum.data(); //A teljes eltolásokat követően szüskéges (maradék) bit szintű eltolások száma

    if (un >= os) //Ha több vagy ugyan annyi elemnyit kell eltolni mint amekkora a vektor mérete (ez esetben lényegében nullázzódik vagy -1-eződik a vektor)
    {
        underflow = 1; //Az underflow 1 (Mivel a 0 és -1 eset korábban vizsgált és korai kilépéssel végződik így ha ide ér biztos lesz előjellel ellentétes kilépő bit a teljes vektorban valahol)
        if (os == un && um == 0) //Ha pont annyit kell eltolni mint amekkora az eredeti vektor akkor a cl-t be kell állítani (ha többet tolnánk akkor maradna 0)
        {                     //7766554433221100
            cl = (*(r.data()+os-1) >> 63); //A cl egynelő a legmagasabb helyiértékű ulli legmagasabb helyiértékű bitje
        }
        else //Ha többet kell eltolni mint az eredeti vektor
        {
            cl = rSn;
        }

        //Az eredmény vektor előállítása
        if (rSn) //Ha negatív a szám
        {
            biCopy(negOne, negOneSn, r, rSn); //A -1 vektor másolása, mivel a negOne 1 elemű az eredmény is 1 elemű ami jó mert a 0 és a -1 az érték szempontjából indifferens a vektor méret
        }
        else
        {
            r.resize(1); //1 elemű vektort adok vissza mivel a 0 és a -1 az érték szempontjából indifferens a vektor méret
            biSetToZero(r, rSn); //Az eredmény egy 0 értékű vektor
        }
    }
    else if (un > 0) //Ha valóban el kell végezni az eltolást
    {
        //Az egész elemek átmásolása, új nulla elemek beállítása
        for (ulli usi = 0; usi < os; usi++)
        {
            //underflow beállítása                                                      //7766554433221100
            if (((!rSn && *(r.data()+usi)) || (rSn && *(r.data()+usi) != 0xffffffffffffffff)) && (usi < un)) underflow = 1;  //Ha a szám ((pozitív ÉS az eredeti érték nem 0) VAGY (negatív ÉS az eredeti érték nem ff.ff) ÉS az index kisebb mint az eltolás akkor a underflow 1 különben marad 0

            //A cl carry előállítása
            if (!um && usi == un-1) cl = (*(r.data()+usi) >> 63); //Ha nincs maradék akkor amikor az index az eltolás mértékénél egyel kisebb pozíciót eléri a cl = az aktuális erdeti ulli legfelső bitje

            //Az adatok új pozícióba átmozgatása
            if (usi < os-un) //Amíg az index a felülírandó tartományban van
            {
                *(r.data()+usi) = *(r.data()+usi+un); //Az alulról n. elem legyen egyenlő az eredeti n+un. elemmel
            }
            else //Az utolsó átmozgatás után a még fennmaradó elmeket 0-val vagy ff.ff-el kell feltölteni
            {
                if(rSn) //Ha a szám negatív
                {
                    *(r.data()+usi) = 0xffffffffffffffff;
                }
                else //Ha a szám pozitív
                {
                    *(r.data()+usi) = 0;
                }
            }
        }
    }

    //Ha van nem egész ulli méretű eltolás rész is,
    if (um)
    {
        //A maradék bitszámnak megfelelő eltolás elvégzése
        for (ulli umi = 0; umi < os; umi++) //A vektor minden elemén
        {
            ulli trm = 0; //Az átvivendő értékeket tartalmazó változó inicializálása

            if (umi < os-un) //Ha az aktuális pozíció még az eltolt eredeti értékeket tartalmazó szakaszra mutat
            {
                trm = (*(r.data()+(umi+1))) << (64-um);  //Az aktuális pozíciónál egyel nagyobb helyiértékű ulli elem aktuálsian átvivendő részének előállítása az adat balra léptetésével
                //overflow előállítása
                if (!umi && ((!rSn && (*r.data() << (64-um))) || (rSn && (*r.data() << (64-um)) != (0xffffffffffffffff << (64-um))))) underflow = 1; //Ha a 0. pozíciónál van és pozitív szám esetén az aktuális adatból kiléptetendő érték nem nulla vagy negatív szám esetén az átvitel méretén értelmezve nem ff..ff az underflow 1
                //A carry előállítása
                if (!umi) cl = (*r.data() >> (um-1) & 0x0000000000000001); //Ha a 0. pozíciónál van a cl az aktuális adat szükséges eltolásnál egy bittel kisebb eltolásának legalacsonyabb helyiértékű bitje
                //A maradék eltolásból adódó eltolás megvalósítása
                if (umi < os-1) //Ha az aktuális pozíció még a legnagyobb helyiérték alatt van
                {
                    *(r.data()+umi) = (*(r.data()+umi) >> um) + trm;  //Az r aktuális ulli elemének um bittel jobbra történõ eltolása és a magasabb helyiértékről történő átvitelből származó rész hozzáadása
                }
                else //Ha az aktuális pozíció a legnagyobb helyiértéken van (ez csak akkor lehet ha nem volt egész eltolás tehát ha un=0)
                {
                    if (rSn) //Ha a szám negatív
                    {
                        *(r.data()+umi) = (*(r.data()+umi) >> um) + (0xffffffffffffffff << (64-um));  //Az r aktuális ulli elemének um bittel jobbra történõ eltolása és a szükséges fff-ek hozzáadása
                    }
                    else //Ha a szám pozitív
                    {
                        *(r.data()+umi) = (*(r.data()+umi) >> um);  //Az r aktuális ulli elemének um bittel jobbra történõ eltolása
                    }
                }
            }
        }
    }
    biLeaderFillerCut(r, rSn, r, rSn); //Esetleges felesleges leader elemek eltávolítása (ha voltak bevezető nullák illetve ha a maradék eltolás csak nullát visz át akkor lehetnek...)

    return underflow;
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShr
(
    const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n,    //Az eltolás mértékének referenciája
    unsigned char & cl      //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    if (n.sigNeg) throw std::invalid_argument("biShr(): the n should not be negatíve");
    return biShr(a.v, a.sigNeg, r.v, r.sigNeg, n.v, cl); //underflow visszaadása
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShr
(
    BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n,    //Az eltolás mértékének referenciája
    unsigned char & cl      //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
)
{
    if (n.sigNeg) throw std::invalid_argument("biShr(): the n should not be negatíve");
    return biShr(v, sigNeg, r.v, r.sigNeg, n.v, cl); //underflow visszaadása
}

//Biginteket jobbra eltoló függvény - OVERRIDE: Az eltolás lépészáma is vektor
//az utolsó lépésben történt eltolás underflow érétkét adja vissza (Ha alulcsordulás történik akkor a kilépõ biteket csonkolja)
unsigned char BinBigInt::biShrNoCl
(
    const std::vector<ulli> & a,    //Az egyik számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az egyik szám előjele
    std::vector<ulli> & r,          //Az erdemény vektor referenciája
    unsigned char & rSn,            //Az erdmény előjel referenciája
    const std::vector<ulli> n       //Az eltolás mértéke
)
{
    unsigned char cl;               //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
    return biShr(a, aSn, r, rSn, n, cl); //underflow visszaadása
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShrNoCl
(
    const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
    BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n     //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShr(): the n should not be negatíve");
    return biShrNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n.v); //underflow visszaadása
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
unsigned char BinBigInt::biShrNoCl
(
    BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
    const BinBigInt & n     //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShr(): the n should not be negatíve");
    return biShrNoCl(v, sigNeg, r.v, r.sigNeg, n.v); //underflow visszaadása
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShrNoClNoUf
(
    const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
    const BinBigInt & n     //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShr(): the n should not be negatíve");
    BinBigInt r;          //Az eredményt tartalmazni hivatott BinBigInt
    biShrNoCl(a.v, a.sigNeg, r.v, r.sigNeg, n.v);
    return r;
}

//Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biShrNoClNoUf
(
    const BinBigInt & n     //Az eltolás mértékének referenciája
)
{
    if (n.sigNeg) throw std::invalid_argument("biShr(): the n should not be negatíve");
    BinBigInt r;          //Az eredményt tartalmazni hivatott BinBigInt
    biShrNoCl(v, sigNeg, r.v, r.sigNeg, n.v);
    return r;
}

//Negative signum function = two's complement function
//Bigint negatív előjel függvény = kettes komplemens függvény
void BinBigInt::biNegative
(
    const std::vector<ulli> & a,    //Az eredeti számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az a előjele
    std::vector<ulli> & r,          //Az eredményt tartalmazni hivatott dinamikus vektor referenciája
    unsigned char & rSn             //Az r előjele
)
{
    if (&a != &r)
    {
        //a átmásolása r-be
        biCopy(a, aSn, r, rSn);
    }

    //kettes komplemens képzés
    biBNot(r, rSn, r, rSn); //Negálás vektor + előjelen értelmezett --> Előjelet is vált!!!
    std::vector<ulli> one {1};
    unsigned char oneSn = 0;
    biAdd(r, rSn, one, oneSn, r, rSn); //+1 hozzáadás
}

//Bigint negatív előjel függvény = kettes komplemens függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biNegative
(
    const BinBigInt & a,    //Az eredeti számot tartalmazó BinBigInt referenciája
    BinBigInt & r           //Az eredményt tartalmazni hivatott BinBigInt referenciája
)
{
    biNegative(a.v, a.sigNeg, r.v, r.sigNeg);
}

//Bigint negatív előjel függvény = kettes komplemens  függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biNegative
(
    BinBigInt & r   //Az eredményt tartalmazni hivatott BinBigInt referenciája
)
{
    biNegative(v, sigNeg, r.v, r.sigNeg);
}

//Bigint negatív előjel függvény = kettes komplemens függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biNegative
(
    const BinBigInt & a    //Az eredeti számot tartalmazó BinBigInt referenciája
)
{
    BinBigInt r;          //Az eredményt tartalmazni hivatott BinBigInt
    biNegative(a.v, a.sigNeg, r.v, r.sigNeg);
    return r;
}

//Bigint negatív előjel függvény = kettes komplemens  függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biNegative()
{
    BinBigInt r;          //Az eredményt tartalmazni hivatott BinBigInt
    biNegative(v, sigNeg, r.v, r.sigNeg);
    return r;
}

//Absolute value function
//Bigint abszolútérték függvény
void BinBigInt::biAbs
(
    const std::vector<ulli> & a,    //Az eredeti számot tartalmazó dinamikus vektor referenciája
    const unsigned char aSn,        //Az a előjele
    std::vector<ulli> & r,          //Az eredményt tartalmazni hivatott dinamikus vektor referenciája
    unsigned char & rSn             //Az r előjele
)
{
    if (&a != &r)
    {
        biCopy(a, aSn, r, rSn); //A a másolása r-be
    }

    if (aSn == 1) //Ha negatív
    {
        //Negatív negatív --> pozitív
        biNegative(r, rSn, r, rSn);
    }
}

//Bigint abszolútérték függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biAbs
(
    const BinBigInt & a,    //Az eredeti számot tartalmazó BinBigInt referenciája
    BinBigInt & r           //Az eredményt tartalmazni hivatott BinBigInt referenciája
)
{
    biAbs(a.v, a.sigNeg, r.v, r.sigNeg);
}

//Bigint abszolútérték függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biAbs
(
    BinBigInt & r   //Az eredményt tartalmazni hivatott BinBigInt referenciája
)
{
    biAbs(v, sigNeg, r.v, r.sigNeg);
}

//Bigint abszolútérték függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biAbs
(
    const BinBigInt & a    //Az eredeti számot tartalmazó BinBigInt referenciája
)
{
    BinBigInt r;           //Az eredményt tartalmazni hivatott BinBigInt
    biAbs(a.v, a.sigNeg, r.v, r.sigNeg);
    return r;
}

//Bigint abszolútérték függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biAbs()
{
    BinBigInt r;           //Az eredményt tartalmazni hivatott BinBigInt
    biAbs(v, sigNeg, r.v, r.sigNeg);
    return r;
}

//Divide function (with remind)
//Biginteger maradékos osztás
void BinBigInt::biDiv
(
    std::vector<ulli> a,        //Az osztandó számot tartalmazó dinamikus vektor
    unsigned char aSn,          //Az osztandó szám előjele
    std::vector<ulli> b,        //Az osztó számot tartalmazó dinamikus vektor
    unsigned char bSn,          //Az osztó  szám előjele
    std::vector<ulli> & quo,    //(quotient) Az eredmény hányadost tartalmazni hivatott dinamikus vektor referencia
    unsigned char & quoSn,      //Az egyik szám előjele
    std::vector<ulli> & rem,    //(reminder) A maradékot tartalmazni hivatott dinamikus vektor referencia
    unsigned char & remSn       //Az egyik szám előjele
)
{
    //Az előjel előállítása
    unsigned char tmprSn = aSn ^ bSn;

    //Abszolút értékekre váltás
    biAbs(a, aSn, a, aSn);
    biAbs(b, bSn, b, bSn);

    //quo és rem inicializálása
    quo.resize(1, 0); //quo 1 eleműre állítása
    *(quo.data()) = 0; //Az "quo" vektor nullázása
    rem.resize(1, 0); //rem 1 eleműre állítása
    *(rem.data()) = 0; //Az "rem" vektor nullázása

    //Annak vizsgálata, hogy az osztó (b) nem nulla-e
    if(biIsZero(b, bSn))
    {
        throw std::invalid_argument("biDiv(): n/0 undetermined"); //Ha nullával osztunk hibát dob
    }

    //Annak vizsgálata, hogy el lehet-e egyáltalán osztani a-t b-vel, magyarul a nagyobb-e mint b (ha nem kilép)
    if(biCmp(a, aSn, b, bSn)) //Ha az a kisebb mint b -> 1 = TRUE
    {
        biCopy(a, aSn, rem, remSn); //a átmásolása rembe
        if (tmprSn) biNegative(rem, remSn, rem, remSn);
        return;
    }

    //Ha el lehet osztani:

    //A bevezetõ komplett nulla értékû ulli vektorelemek eltávolítása a munk változókról

    //Az esetleges csak bevezetõ nullákat tartalmazó ulli elemek elhagyásával egy lehetõ legkisebb elemszámú
    //de minden értékes bitet tartalmazó átmeneti vektor létrehozása az osztóból (sebességnövelés miatt)
    std::vector<ulli> sb {0};
    unsigned char sbSn = 0;
    biCopy(b, bSn, sb, sbSn); //A b vektor sb-be msolása
    biLeaderFillerCut(sb, sbSn, sb, sbSn); //Az sb vektor csak nullákat tartalmazó bevezető elemeinek levágása és az eredmény szintén sb-en történő visszaadása
    ulli sbl = sb.size();

    //Az esetleges csak bevezetõ nullákat tartalmazó ulli elemek elhagyásával egy lehetõ legkisebb elemszámú
    //de minden értékes bitet tartalmazó átmeneti vektor létrehozása az osztandóból (sebességnövelés miatt)
    std::vector<ulli> sta {0};
    unsigned char staSn = 0;
    biCopy(a, aSn, sta, staSn); //A a vektor sta-ba msolása
    biLeaderFillerCut(sta, staSn, sta, staSn); //Az sta vektor csak nullákat tartalmazó bevezető elemeinek levágása és az eredmény szintén sta-ban történő visszaadása
    ulli stal = sta.size();

    //A bevezetõ nulla bitek számának megálapítása az összerendezhetõséghez

    //A sb vektor legmagasabb helyiértékû 1-es bitje elõtti leader nullák számának megállapítása
    //Mindig ennyi nullával kell majd igazítani az a vektor aktuálisan vizsgált értékeit is...
    ulli actCarry = 0;
    ulli sblzc = 0; //sb Leader Zeros Count számláló (mivel korábban a vektor a leehtő legkisebb méretűre lett konvertálva sblzc <= 63 ! )
    ulli sbh = *(sb.data()+(sbl-1)); //Az eltolás elvégzésére szolgáló változó
    while (!(actCarry >= 1)) //Amíg nem történik túlcsordulás (carry)
    {
        actCarry = (sbh & 0x8000000000000000) >> 63; //Az aktuális bal szélső bit értékének megállapítása (ha 1 akkor a következő sorben szereplő aktuális balratolás carryt fog eredményezni)
        sbh = sbh << 1; //sb vektor legmagasabb helyiértékû ulli elemének 1 bittel jobbra történõ eltolása
        sblzc++; //A leader nullák és a legmagasabb helyiértékû 1-es bit darabszáma
    }
    sblzc--; //Mivel a flags akkor vált ha már túlcsordult ezért 1-et le kell vonni, hogy megkapjuk a bevezetõ nullák számát (Leader Zeros Count)

    ulli sbBil = ulliBitSize*sbl-sblzc; //sb értékes bithosszának kiszámítása

    //A a vektor legmagasabb helyiértékû 1-es bitje elõtti leader nullák számának megállapítása
    //Mindig ennyi nullával kell majd igazítani az a vektor aktuálisan vizsgált értékeit is...
    actCarry = 0;
    ulli stalzc = 0; //sta Leader Zeros Count számláló  (mivel korábban a vektor a leehtő legkisebb méretűre lett konvertálva sblzc <= 63 ! )
    ulli stah = *(a.data()+stal-1);  //Az eltolás elvégzésére szolgáló változó
    while (!(actCarry >= 1)) //Amíg nem történik túlcsordulás (carry)
    {
        actCarry = (stah & 0x8000000000000000) >> 63; //Az aktuális bal szélső bit értékének megállapítása (ha 1 akkor a következő sorban szereplő aktuális balratolás carryt fog eredményezni)
        stah = stah << 1; //sta vektor legmagasabb helyiértékû ulli elemének 1 bittel jobbra történõ eltolása
        stalzc++; //A leader nullák és a legmagasabb helyiértékû 1-es bit darabszáma
    }
    stalzc--; //Mivel a flags akkor vált ha már túlcsordult ezért 1-et le kell vonni, hogy megkapjuk a bevezetõ nullák számát (Leader Zeros Count)

    ulli staBil = ulliBitSize*stal-stalzc; //sta értékes bithosszának kiszámítása

    //A két szám vektor leader nulláinak összeegyeztetéséhez szükséges eltolás elvégzése
    //Ha egyenlõek nem kell csinálni semmit
    //Mivel az a vektor lehet nagyobb elemszámú mint a b vektor lehetséges, hogy kevesebb bevezetõ nullája van így ezt az esetet is vizsgálni kell
    if(stalzc > sblzc) //Ha a-nak van több bevezetõ nullája (a teljes sta vektor léptetése balra [átvitelekkel], egyszerûbb eset)
    {
        biShlNoCl(sta, staSn, sta, staSn, (stalzc-sblzc), 1);
    }
    else if(sblzc > stalzc)  //Ha b-nek van több bevezetõ nullája az sta-hoz adok egy leading nulla ulli-t majd az egész sta-t balra tolom a szükséges mértékben)
    {
        stal++;
        sta.resize(stal, 0); //A sta megnövelése egyel és az új ulli elem nullára inicializálása
        biShlNoCl(sta, staSn, sta, staSn, (stalzc+64-sblzc), 1);
    }

    ulli dStpc = staBil - sbBil; //A tényleges osztás elvégzése során szükséges bitléptetések száma (DivideSTePCount)

    //sbl növelése (csak azért mert lehet sa-t majd egy bittel jobbra kell tolni és, hogy ebbõl ne legyen gond (Ezzel elvileg a teljes folyamat során kizárt a túlcsordulás!)
    sbl++;
    sb.resize(sbl, 0); //Az sb megnövelése egyel és az új ulli elem nullára inicializálása

    //Az osztandóból az osztó hasznos méretének megfelelő rész képezése ()
    ulli sal = sbl; //sal hossz változó sb hosszával egyenlő

    //Egy sb méretű sa vektor létrehozása (összehasonlításhoz lesz használva)
    std::vector<ulli> sa {0};
    unsigned char saSn = 0;
    sa.resize(sal, 0);
    //A sta vektor legnagyobb helyiértékû sal-1 darabszámú bytejainak lemásolása érték szerint az sa vektorba (összehasonlításhoz)
    for (ulli i = 0; i<sal-1; i++)
    {
        *(sa.data()+i) = *(sta.data()+stal-1-(sal-1)+1+i);
    }
    *(sa.data()+sal-1) = 0;    //Az sa legfelsõ helyiértékû bytejának nullázása

    //A sta vektor csonkolása úgy, hogy csak az sa alatti bytjai maradjanak meg
    stal = stal-(sal-1); //A sta vektor új hossza = eredeti sta méret - az sa vektor mérete a hozzáfûzött csak beveztõ nullákat tartalmazó ulli elem nélkül (-1)
    sta.resize(stal, 0);

    ulli rstpc = 0; //A sta tényleges osztás során történõ eltolásának számlálója

    while(rstpc <= dStpc)   //Amíg a tényleges lépésszám eléri a szükséges lépések számát
    {
        rstpc++; //A sta tényleges osztás során történõ eltolásának számlálójának növelése egyel

        biShlNoCl(quo, quoSn, quo, quoSn, 1, 1);   //Az eredmény vektor quo eltolása 1-el balra (Mivel a hányados egész számok esetén szüskégszerûen kisebb vagy egyenlõ mint az osztandó és azonos a vektor méret túlcsordulás nem lehetséges)

        //sa és sb összehasonlítása
        if(biCmp(sa, saSn, sb, sbSn))    //Ha az sa kisebb mint sb -> 1 = TRUE tehát ha nem osztható el.
        {//Ha sa nem osztható el sb-vel -> eredmény bit 0 (Nem kell beállítani mert quo nullára inicializált)

            //sa léptetése a következõ lépéshez
            unsigned char ofsta = biShlNoCl(sta, staSn, sta, staSn, 1, 0);  //sta eltolása balra 1-el CSONKOLÁSSAL. A ofsta az eltolás során kilépõ bitnek megfeleõ érték amit majd hozzá kelladni a szitnén eltolt sa-hoz
            biShlNoCl(sa, saSn, sa, saSn, 1, 1);   //Az sa vektor eltolása 1-el balra (A plusz bevezetõ 0-kat tartalmazó ulli hozzáfûzése miatt túlcsordulás nem lehetséges)
            *(sa.data()) = *(sa.data()) + ofsta; //A ofsta hozzáadása sa-hoz (A ofsta a sta eltolása során kilépett bitnek megfeleõ 0 vagy 1 érték)
        }
        else
        {//Ha sa elosztható sb-vel -> erdemény bit 1
            *(quo.data()) = *(quo.data()) + 1;  //Az eredmény quo legalsó bitjének 1-re állítása (a quo nullára inicializált)

            //Az sa-ból sb kivonása és az eredmény sa-ban történõ visszaadása
            biSub(sa, saSn, sb, sbSn, sa, saSn);  //A két vektort big_int-ként kivonó függvény (Intel Intrinsics _subborrow_u64())
            //sa léptetése a következõ lépéshez
            unsigned char ofsta = biShlNoCl(sta, staSn, sta, staSn, 1, 0);  //sta eltolása balra 1-el CSONKOLÁSSAL. A ofsta az eltolás során kilépõ bitnek megfeleõ érték amit majd hozzá kell adni a szitnén eltolt sa-hoz
            biShlNoCl(sa, saSn, sa, saSn, 1, 1);   //Az sa vektor eltolása 1-el balra (A plusz bevezetõ 0-kat tartalmazó ulli hozzáfûzése miatt túlcsordulás nem lehetséges)
            *(sa.data()) = *(sa.data()) + ofsta; //A ofsta hozzáadása sa-hoz (A ofsta a sta eltolása során kilépett bitnek megfeleõ 0 vagy 1 érték)
        }
    }

    //A maradék:
    biShrNoCl(sa, saSn, sa, saSn, 1);   //Az sa vektor eltolása 1-el jobbra (Az osztás során történt legutolsó, ekkor már indokolatlan eltolás visszaállítása)

    //A maradék ellenõrzése
    if(biCmp(sb, sbSn, sa, saSn)) throw std::invalid_argument("biDiv(): the reminder bigger than the divider"); //Ha a maradék nagyobb mint az osztó hibát dob
    //Az sa vektor átmásolása a rem vektorba (visszadáshoz)
    biCopy(sa, saSn, rem, remSn);

    //Az előjelnek megfelelő szám és előjel előállítása
    if (tmprSn) biNegative(quo, quoSn, quo, quoSn);
    if (tmprSn) biNegative(rem, remSn, rem, remSn);
}

//Biginteger maradékos osztás OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biDiv
(
    const BinBigInt & a,    //Az osztandó számot tartalmazó BinBigInt referenciája
    const BinBigInt & b,    //Az osztó számot tartalmazó BinBigInt referenciája
    BinBigInt & quo,        //(quotient) Az eredmény hányadost tartalmazni hivatott BinBigInt referenciája
    BinBigInt & rem         //(reminder) A maradékot tartalmazni hivatott BinBigInt referenciája
)
{

    return biDiv(a.v, a.sigNeg, b.v, b.sigNeg, quo.v, quo.sigNeg, rem.v, rem.sigNeg); //Carry visszadása
}

//Biginteger maradékos osztás OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biDiv
(
    const BinBigInt & b,    //Az osztó számot tartalmazó dinamikus vektor referencia
    BinBigInt & quo,        //(quotient) Az eredmény hányadost tartalmazni hivatott BinBigInt referenciája
    BinBigInt & rem         //(reminder) A maradékot tartalmazni hivatott BinBigInt referenciája
)
{
    return biDiv(v, sigNeg, b.v, b.sigNeg, quo.v, quo.sigNeg, rem.v, rem.sigNeg);
}

//Factorial function
//Bigint faktoriális függvény (a megvalósítás akár meg hívhatná a biPosProductOfSequence() pozitív produktum függvényt is 1-es i paraméterrel mert lényegében azzal egyenértékű... Ez kicsit gyorsabb mert két feltétel vizsgálattal kevesebb van benne)
void BinBigInt::biFactorial
(
    const std::vector<ulli>  f,     //A faktoriális száma
    const unsigned char fSn,        //A faktoriális előjele (Csak pozitív lehet, csak ellenőrzés miatt szüskéges)
    std::vector<ulli> & r,          //Az eredményt tartalmazni hivatott vektor referenciája
    unsigned char & rSn             //Az eredmény előjele (Csak pozitív lehet)
)
{
    if (fSn != 0) throw std::invalid_argument("biFactorial(): factorial argument can't be negative");
    std::vector<ulli> fakt {0};
    unsigned char faktSn = 0;
    std::vector<ulli> one {1};
    unsigned char oneSn = 0;

    if(biIsSmallerOrEqual(f, fSn, one, oneSn)) //Ha f <= 1
    {
        biCopy(one, oneSn, r, rSn); //Eredmény = 1
    }
    else
    {
        biSub(f, fSn, one, oneSn, fakt, faktSn); //fakt = f-1
        biFactorial(fakt, faktSn, r, rSn); //rekurzív függvény hvás
        biMul(f, fSn, r, rSn, r, rSn);  //Eredmény = f*r
    }
}

//Bigint faktoriális függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biFactorial
(
    const BinBigInt & f,    //A faktoriális száma
    BinBigInt & r           //Az eredményt tartalmazni hivatott vektor referenciája
)
{
    biFactorial(f.v, f.sigNeg, r.v, r.sigNeg);
}

//Bigint faktoriális függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biFactorial
(
    BinBigInt & r   //Az eredményt tartalmazni hivatott vektor referenciája
)
{
    biFactorial(v, sigNeg, r.v, r.sigNeg);
}

//Bigint faktoriális függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biFactorial
(
    const BinBigInt & f    //A faktoriális száma
)
{
    BinBigInt r;   //Az eredményt tartalmazni hivatott BinBigInt
    biFactorial(f.v, f.sigNeg, r.v, r.sigNeg);
    return r;
}

//Bigint faktoriális függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biFactorial()
{
    BinBigInt r;   //Az eredményt tartalmazni hivatott BinBigInt
    biFactorial(v, sigNeg, r.v, r.sigNeg);
    return r;
}

//Product of sequences function (!!This only interpreted to positive integers!!!)
//Bigint szorzássorozat produktum függvény (!!!Csak pozitív egészekre értelmezett!!!)
void BinBigInt::biPosProductOfSequence
(
    std::vector<ulli>  i,     //A produktum kezdő száma
    unsigned char iSn,        //A produktum kezdő száma előjele (Csak pozitív lehet, csak ellenőrzés miatt szüskéges)
    std::vector<ulli>  n,     //A produktum befejező száma
    unsigned char nSn,        //A produktum befejező számának előjele (Csak pozitív lehet, csak ellenőrzés miatt szüskéges)
    std::vector<ulli> & r,    //Az eredményt tartalmazni hivatott vektor referenciája
    unsigned char & rSn       //Az eredmény előjele (Csak pozitív lehet)
)
{
    if (iSn != 0 || nSn != 0) throw std::invalid_argument("biPosProductOfSequence(): product argument can't be negative");
    std::vector<ulli> prod {0};
    unsigned char prodSn = 0;
    std::vector<ulli> one {1};
    unsigned char oneSn = 0;

    if (biIsBigger(i, iSn, n, nSn)) //Ha i > n felcseréli a két vektort
    {
        biCopy(i, iSn, prod, prodSn);
        biCopy(n, nSn, i, iSn);
        biCopy(prod, prodSn, n, nSn);
    }

    if(biIsSmallerOrEqual(n, nSn, one, oneSn)) //Ha n <= 1
    {
        biCopy(one, oneSn, r, rSn); //Eredmény = 1
    }
    else if(biIsEqual(n, nSn, i, iSn)) //Ha n = i
    {
        biCopy(i, iSn, r, rSn); //Eredmény = i
    }
    else
    {
        biSub(n, nSn, one, oneSn, prod, prodSn); //prod = n-1
        biPosProductOfSequence(i, iSn, prod, prodSn, r, rSn); //rekurzív függvény hívás
        biMul(n, nSn, r, rSn, r, rSn);  //Eredmény = n*r
    }
}

//Bigint produktum függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biPosProductOfSequence
(
    const BinBigInt & i,     //A produktum kezdő száma
    const BinBigInt & n,    //A produktum befejező száma
    BinBigInt & r           //Az eredményt tartalmazni hivatott vektor referenciája
)
{
    biPosProductOfSequence(i.v, i.sigNeg, n.v, n.sigNeg, r.v, r.sigNeg);
}

//Bigint produktum függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biPosProductOfSequence
(
    const BinBigInt & n,    //A produktum befejező száma
    BinBigInt & r           //Az eredményt tartalmazni hivatott vektor referenciája
)
{
    biPosProductOfSequence(v, sigNeg, n.v, n.sigNeg, r.v, r.sigNeg);
}

//Bigint produktum függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biPosProductOfSequence
(
    const BinBigInt & i,     //A produktum kezdő száma
    const BinBigInt & n     //A produktum befejező száma
)
{
    BinBigInt r;   //Az eredményt tartalmazni hivatott BinBigInt
    biPosProductOfSequence(i.v, i.sigNeg, n.v, n.sigNeg, r.v, r.sigNeg);
    return r;
}

//Bigint produktum függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::biPosProductOfSequence
(
    const BinBigInt & n    //A produktum befejező száma
)
{
    BinBigInt r;   //Az eredményt tartalmazni hivatott BinBigInt
    biPosProductOfSequence(v, sigNeg, n.v, n.sigNeg, r.v, r.sigNeg);
    return r;
}

//Write Bigint to string function
//Bigint sztringbe kiírása
void BinBigInt::biToStr
(
    std::vector<ulli> a,        //A konvertálandó bigint vektor referencia
    unsigned char aSn,          //A bemeneti bigint előjel
    const unsigned char nfb,    //(NumberFormatBase) A kimeneti számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
    std::string &rs,            //Eredmény sztring
    const std::string sepStr,   //A tagoló sztring
    const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
)
{
    rs = ""; //Az eredmény sztring inicializálása

    if(nfb < 2 || nfb > 35)  //Ha a paraméterként kapott kimeneti számformátum alapja kisebb mint 2 vagy nagyobb mint 35 kilép (A 35 az ASCII számok és kisbetűk darabszámából jön...)
    {
        return;
    }
    //Negatív előjel jelenlétének vizsgálata
    if(aSn == 1) //Ha negatív
    {
        rs = "-"; //A "-" előjel beállítása
        biAbs(a, aSn, a, aSn); //a abszolútértékének képzése
    }

    std::vector<ulli> nfbv {(ulli)nfb};  //A számrendszer alapja mint osztó
    unsigned char nfbvsn = 0;


    //Kimeneti eredmény sztring számformátum prefix beállítása
    switch (nfb)
    {
        case 2:
        {
            rs.append("0b");
        }
        break;
        case 8:
        {
            rs.append("0");
        }
        break;
        case 10:
        {
            ; //Ez csak azért kell, hogy ne a default "0?" előtagot kapja
        }
        break;
        case 16:
        {
            rs.append("0x");
        }
        break;
        default:
        {
            rs.append("0?");
        }
        break;
    }

    std::vector<ulli> rem {0};  //A maradék
    unsigned char remSn = 0;

    std::string revstr = ""; //A számjegyeket fordítva tartalmazó sztring

    ulli c = 1; //A lépés számláló (az osztó sztringek elhelyezéséhez)

    //A konverzió
    while(!biCmp(a, aSn, nfbv, nfbvsn)) //Ha a számformátum alapja kissebb vagy egyenlő mint a konvertálandó szám
    {
        biDiv(a, aSn, nfbv, nfbvsn, a, aSn, rem, remSn); //A konvertálandó szám osztása majd az eredmény eltárolása a bementi vektorba a következő lépéshez
        if(*rem.data() <= 9) //Ha a számjegy kisebb egyenlő 9
        {
            //revstr.push_back((char)(*rem.data()+0x30)); //A maradék hozzáfűzéde a sztring végéhez. A +0x30 az ascii "0" karakter kódja
            std::string s = std::to_string(*rem.data());
            revstr.append(s); //A maradék hozzáfűzéde a sztring végéhez. A +30 az ansi "0" karakter kódja
            if(c == sepw)
            {
                revstr.append(sepStr); //Az elválasztó karakter hozzáfűzése a sztring végéhez.
                c = 0;
            }
        }
        else //Ha a számjegy nagyobb egyenlő 10
        {
            revstr.push_back((char)(*rem.data()+87)); //A maradék hozzáfűzéde a sztring végéhez. A +87 az ascii "a" karakter decimális kódja - 10
            if(c == sepw)
            {
                revstr.append(sepStr); //Az elválasztó karakter hozzáfűzése a sztring végéhez.
                c = 0;
            }
        }
        c++;
    }
    //Az utolsó lépésben az eredmény változóban maradt számjegy hozzáadása (elvileg nem lehet nagyobb mint a számrendszer alapja)
    if(*a.data() <= 9) //Ha a számjegy kisebb egyenlő 9
    {
        //revstr.push_back((char)(*rem.data()+0x30)); //A maradék hozzáfűzéde a sztring végéhez. A +0x30 az ascii "0" karakter kódja
        std::string s = std::to_string(*a.data());
        revstr.append(s); //A maradék hozzáfűzéde a sztring végéhez. A +30 az ansi "0" karakter kódja
/*
        if(c == sepw)
        {
            revstr.append(sepStr); //Az elválasztó karakter hozzáfűzése a sztring végéhez.
            c = 0;
        }
*/
    }
    else //Ha a számjegy nagyobb egyenlő 10
    {
        revstr.push_back((char)(*a.data()+87)); //A maradék hozzáfűzéde a sztring végéhez. A +87 az ascii "a" karakter decimális kódja - 10
/*
        if(c == sepw)
        {
            revstr.append(sepStr); //Az elválasztó karakter hozzáfűzése a sztring végéhez.
            c = 0;
        }
*/
    }

    //A fordított sztring visszafordítása
    for(ulli i = revstr.size(); i > 0; i--)
    {
        rs.push_back(revstr[i-1]);
    }
}

//Bigint sztringbe kiírása OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::biToStr
(
    const BinBigInt & a,          //A konvertálandó bigint vektor referencia
    const unsigned char nfb,      //A kimeneti számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
    std::string &rs,              //Eredmény sztring
    const std::string sepStr,     //A tagoló sztring
    const ulli sepw               //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
)
{
    biToStr(a.v, a.sigNeg, nfb, rs, sepStr, sepw);
    //BinBigInt::biToStr(this.v, nfb, rs, sepStr, sepw);
}
//Bigint sztringbe kiírása OVERRIDE: példányosított objektum tagfüggvényekét meghívva --> A bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott)
void BinBigInt::biToStr
(
    const unsigned char nfb,    //A kimeneti számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
    std::string &rs,            //Eredmény sztring
    const std::string sepStr,   //A tagoló sztring
    const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
)
{
    biToStr(v, sigNeg, nfb, rs, sepStr, sepw);
}

//Write numeric value given in string format into BigInt
//Sztring formátumbanmegadott szám Bigintbe írása
void BinBigInt::strToBi
(
    const std::string & as,     //A konvertálandó string referenciája
    const unsigned char nfb,    //A bemeneti sztring számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
    std::vector<ulli> &r,       //Az eredmény bigint vektor
    unsigned char &rSn,         //Az eredmény bigint előjel
    const std::string sepStr,   //A tagoló sztring
    const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
)
{
    if(nfb < 2 || nfb > 35)  //Ha a paraméterként kapott kimeneti számformátum alapja kisebb mint 2 vagy nagyobb mint 35 kilép (A 35 az ASCII számok és kisbetűk darabszámából jön...)
    {
        r.resize(0); //r érvénytelenné tétele
        return;
    }

    //Eredeti string másolása
    std::string tas = as;

    ulli tass = tas.size();

    rSn = 0; //Az eredmény előjelének inicializálása
    unsigned char tmprSn = 0; //Az eredmény átmeneti előjelének inicializálása
    //Előjel jelenlétének vizsgálata
    if(tas.substr(0,1) == "+")
    {
        tas.erase(0,1);
    }
    else if(tas.substr(0,1) == "-")
    {
        tmprSn = 1; //Az átmeneti előjel 1-re állítása
        tas.erase(0,1);
    }

    //A numerikus sztring literál prefixek jelenlétének vizsgálata és eltávolítása
    //Kimeneti eredmény sztring inicializálása
    switch (nfb)
    {
        case 2:
        {
            //if(tas.starts_with("0b")) //Ez csak C++20-tól létezik vsz vagy az a baja, hogy az MSVC még nem ismeri vagy meg kellene adni a vonatkozó compiler kapcsolót... Próbálkoztam. Nem mentt. Hagytam...
            //https://devblogs.microsoft.com/cppblog/c20-concepts-are-here-in-visual-studio-2019-version-16-3/
            //https://isocpp.org/blog/2016/02/a-bit-of-background-for-concepts-and-cpp17-bjarne-stroustrup
            if(tas.substr(0,2) == "0b")
            {
                tas.erase(0,2);
            }
        }
        break;
        case 8:
        {
            if(tas.substr(0,2) == "0")
            {
                tas.erase(0,2);
            }
        }
        break;
        case 10:
        {
            if(tas.substr(0,2) == "0n")
            {
                tas.erase(0,2);
            }
        }
        break;
        case 16:
        {
            if(tas.substr(0,2) == "0x")
            {
                tas.erase(0,2);
            }
        }
        break;
        default:
        {
            if(tas.substr(0,2) == "0?")
            {
                tas.erase(0,2);
            }
        }
        break;
    }

    //Az eredmény bigint nullázása
    r.resize(1);
    *(r.data()) = 0; //Az "r" vektor nullázása

    std::vector<ulli> nfbv {(ulli)nfb}; //A számrendszer alapját mint szorzót tároló bigint vektor
    unsigned char nfbvsn = 0;
    std::vector<ulli> alvv {1};         //Az aktuális helyiérték (ActualLocalValueVector)
    unsigned char alvvsn = 0;

    ulli rsepw = 0;  //A tagoló karakter távolság számláló
    unsigned char d; //A sztring aktuális számjegyét számként tartalmazó változó

    //A sztring fordított irányú kiolvasása
    for(ulli i = tas.size(); i > 0; i--)
    {
        char s = (char)tas[i-1];         //Az aktuális számjegy ASCII kódjának kiolvasása
        //Az aktuális "számjegy" (0-9, a-z) számmá alakítása
        if(s >= 0x30 && s <= 0x39) //Ha a char érték nagyobb egyenlő mint a "0" és kisebb egyenlő mint a "9" számjegy ASCII kódja (0-9-ig futó ág)
        {
            d = s-0x30; //Karakter konvertálássa számmá
        }
        else if(s >= 0x61&& s <= 0x61+nfb-10-1) //Ha a char érték nagyobb mint az "a" és kisebb egyenlő mint a számrendszer alapja által megengdett legagyobb karakter (max. "z") ASCII kódja (a-z-ig futó ág de max a számrendszer szerint)
        {
            d = s-87; //87 = 97 - 10 = 0x61 - 0xa
        }
        else if (s = (char)sepStr[0]) //Ha tagoló karakter
        {
            if(rsepw == sepw) //Ha talál tagoló karaktert és helyes a távolság (hátulról [= kis helyiértéktől] jön előre tehát az első találat is mindig pontosan itt kell legyen!
            {
                rsepw = 0;
                continue; //ugrás a következő cikluslépésre
            }
            else //Amikor később talál tagoló karaktert de rossz a távolság
            {
                throw std::invalid_argument("strToBi(): Divider width fault or inconsistent parameter");
                //continue;
            }
        }
        else  //valamilyen illegális karakter
        {

            throw std::invalid_argument("strToBi(): Illegal character in numeric literal string or inconsistent parameter");
            //continue;
        }

        std::vector<ulli> tr {d}; //Az aktuális helyiértékhez tartozó számjegy bigint vektorrá alakítása
        unsigned char trSn = 0;
        biMul(tr, trSn, alvv, alvvsn, tr, trSn);       //Az aktuáis helyi érték és számjegy szerinti szám (= a számjegy és a helyiérték szorzata) előállíítás tr-be

        biAdd(r, rSn, tr, trSn, r, rSn);          //A tr hozzáadás az r vektorhoz, eredmény r vektorba

        biMul(alvv, alvvsn, nfbv, nfbvsn, alvv, alvvsn); //A számrendszer következő lépésben aktuális pozíciójához tartozó helyiértékének előállítása alvv bigint vektorba

        rsepw++; //A tagoló karakter távolság számláló növelése
    }

    if (tmprSn == 1 && !biIsZero(r, rSn)) //Ha a tmprSn átmeneti előjel 1 (Tehát ha a sztring "-" negatív előjelet/számot tartalmazott) és az értéke nem nulla. (A negatív nulla elkerülése miatt)
    {
        //kettes komplemens képzés
        biNegative(r, rSn, r, rSn);

        rSn = 1;
    }
}

//Sztring Bigintbe írása OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
void BinBigInt::strToBi
(
    const std::string & as,     //A konvertálandó string referenciája
    const unsigned char nfb,    //A bemeneti sztring számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
    BinBigInt &r,               //Az eredmény BinBigInt
    const std::string sepStr,   //A tagoló sztring
    const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
)
{
    strToBi(as, nfb, r.v, r.sigNeg, sepStr, sepw);
}

//Sztring Bigintbe írása OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
BinBigInt BinBigInt::strToBi
(
    const std::string & as,     //A konvertálandó string referenciája
    const unsigned char nfb,    //A bemeneti sztring számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
    const std::string sepStr,   //A tagoló sztring
    const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
)
{
    BinBigInt r;                //Az eredmény BinBigInt
    strToBi(as, nfb, r.v, r.sigNeg, sepStr, sepw);
    return r;
}

//Sztring Bigintbe írása OVERRIDE: példányosított objektum tagfüggvényekét meghívva --> A visszatérési érték maga az objektum (visszatérési referencia paraméter nem megadott)
void BinBigInt::strToThisBi
(
    const std::string & as,     //A konvertálandó string referenciája
    const unsigned char nfb,    //A bemeneti sztring számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
    const std::string sepStr,   //A tagoló sztring
    const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
)
{
    strToBi(as, nfb, v, sigNeg, sepStr, sepw);
}

//OPERATOR FUNCTIONS
//OPERÁTOR FÜGGVÉNYEK

//Arithmetic operators
//Aritmetikai operátorok

// + operator
// + operátor
BinBigInt operator+(const BinBigInt & a, const BinBigInt & b)
{
    BinBigInt r;
    BinBigInt::biAdd(a, b, r);
    return r;
}

// - operator (subtraction)
// - operátor (kivonás)
BinBigInt operator-(const BinBigInt & a, const BinBigInt & b)
{
    BinBigInt r;
    BinBigInt::biSub(a, b, r);
    return r;
}

// * operator
// * operátor
BinBigInt operator*(const BinBigInt & a, const BinBigInt & b)
{
    BinBigInt r;
    BinBigInt::biMul(a, b, r);
    return r;
}

// / operator
// / operátor
BinBigInt operator/(const BinBigInt & a, const BinBigInt & b)
{
    BinBigInt q, r;
    BinBigInt::biDiv(a, b, q, r);
    return q;
}


// % operator (reminder)
// % operátor (maradék)
BinBigInt operator%(const BinBigInt & a, const BinBigInt & b)
{
    BinBigInt q, r;
    BinBigInt::biDiv(a, b, q, r);
    return r;
}



// - operator (negative sign)
// - operátor (negatív előjel)
BinBigInt operator-(const BinBigInt & a)
{
    BinBigInt r;
    BinBigInt::biNegative(a, r);
    return r;
}

//Bitwise operators
//Bit műveleti operátorok

// << operator (Shift left BinBigInt with extend it equivalent to multiply with 2^n)
// << operátor (BinBigInt kiterjesztéssel történő balra eltolása 2^n-el történő szorzással ekvivalens)
BinBigInt operator<<(const BinBigInt & a, const ulli n)
{
    BinBigInt r;
    unsigned char cl;
    BinBigInt::biShl(a, r, n, cl, 1);
    return r;
}

// << operator (Shift left BinBigInt with extend it equivalent to multiply with 2^n)
// << operátor (BinBigInt kiterjesztéssel történő balra eltolása 2^n-el történő szorzással ekvivalens)
BinBigInt operator<<(const BinBigInt & a, const BinBigInt & n)
{
    BinBigInt r;
    unsigned char cl;
    BinBigInt::biShl(a, r, n, cl, 1);
    return r;
}

// >> operator (Shift right BinBigInt with extend)
// >> operátor (BinBigInt kiterjesztéssel történő jobbra eltolása)
BinBigInt operator>>(const BinBigInt & a, const ulli n)
{
    BinBigInt r;
    unsigned char cl;
    BinBigInt::biShr(a, r, n, cl);
    return r;
}

// >> operator (Shift right BinBigInt with extend)
// >> operátor (BinBigInt kiterjesztéssel történő jobbra eltolása)
BinBigInt operator>>(const BinBigInt & a, const BinBigInt & n)
{
    BinBigInt r;
    unsigned char cl;
    BinBigInt::biShr(a, r, n, cl);
    return r;
}

// & operator
// & operátor
BinBigInt operator&(const BinBigInt & a, const BinBigInt & b)
{
    BinBigInt r;
    BinBigInt::biBAnd(a, b, r);
    return r;
}

// ^ operator (XOR)
// ^ operátor (XOR)
BinBigInt operator^(const BinBigInt & a, const BinBigInt & b)
{
    BinBigInt r;
    BinBigInt::biBXor(a, b, r);
    return r;
}

// | operátor (OR)
// | operátor (OR)
BinBigInt operator|(const BinBigInt & a, const BinBigInt & b)
{
    BinBigInt r;
    BinBigInt::biBOr(a, b, r);
    return r;
}

// ~ operátor (NEGATE)
// ~ operátor (NEGATE)
BinBigInt operator~(const BinBigInt & a)
        {
    BinBigInt r;
    BinBigInt::biBNot(a, r);
    return r;
}

//Comparison operators
//Összehasonlító operátorok

// == operator
// == operátor
bool operator==(const BinBigInt & a, const BinBigInt & b)
{
    return BinBigInt::biIsEqual(a, b);
}

// != operator
// != operátor
bool operator!=(const BinBigInt & a, const BinBigInt & b)
{
    return BinBigInt::biIsNotEqual(a, b);
}

// <= operator
// <= operátor
bool operator<=(const BinBigInt & a, const BinBigInt & b)
{
    return BinBigInt::biIsSmallerOrEqual(a, b);
}

// >= operator
// >= operátor
bool operator>=(const BinBigInt & a, const BinBigInt & b)
{
    return BinBigInt::biIsBiggerOrEqual(a, b);
}

// < operator
// < operátor
bool operator<(const BinBigInt & a, const BinBigInt & b)
{
    return BinBigInt::biIsSmaller(a, b);
}

// > operator
// > operátor
bool operator>(const BinBigInt & a, const BinBigInt & b)
{
    return BinBigInt::biIsBigger(a, b);
}
