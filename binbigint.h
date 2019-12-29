#ifndef BINBIGINT_H_INCLUDED
#define BINBIGINT_H_INCLUDED

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

typedef unsigned long long int ulli;

class BinBigInt
{

    public:

    //Constant definitions
    //Konstans definíciók
        enum{BIN=2,OCT=8,DEC=10,HEX=16};

        static const ulli ulliByteSize = sizeof(ulli);
        static const std::vector<ulli> vUlliByteSize;
        static const unsigned char ulliByteSizeSn = 0;
        static const ulli ulliBitSize = ulliByteSize*8;
        static const std::vector<ulli> vUlliBitSize;
        static const unsigned char ulliBitSizeSn = 0;
        static const ulli ulliLimit;
        static const std::vector<ulli> vUlliLimit;
        static const unsigned char ulliLimitSn = 0;

    //Member variables
    //Tagváltozók
        std::vector<ulli> v {0};
        unsigned char sigNeg = 0;

    //CONSTRUCTORS
    //KONSTRUKTOROK

        //Default instantiation and instantiation with Unsigned Long Long Int parameter (if "-" number given with (ulli) typecast then the two's complements
        //absolute value will be the number's value so e.g. the "BinBigInt biCd((ulli)-1);" instantiation's result is a BinBigInt object which contain
        //the + 18 446 744 073 709 551 615 value.
        //Default és Unsigned Long Long Int paraméterrel történő példányosítás (ha "-" negatív számot adunk meg (ulli) typecasttal akkor a kettes komplemens
        //abszolút értéke lesz a szám értéke tehát pl. a "BinBigInt biCd((ulli)-1);" példánysoítás eredménye egy a + 18 446 744 073 709 551 615 értéket tartalmazó
        //Bigint objektum
        BinBigInt
        (
            ulli a0 = 0
        );

        //Instantiation with Singed Long Long Int parameter
        //Singed Long Long Int paraméterrel történő példányosítás
        BinBigInt
        (
            signed long long int a0
        );

        //Instantiation with  vector and signum parameter (in the case of negative number should give the absolute/"+" value of the required
        //number in the vector parameter which will have been converted to two's complement and stored in that form by the constructor!)
        //Vektor paraméterrel és előjel értékkel történő példányosítás  (negatív szám esetén is a vektorban a szám aboszolút/"+" értékét
        //kell megadni amit a konstruktor kettes komplemenssé alakít és úgy tárolja!)
        BinBigInt
        (
            const std::vector<ulli> v, unsigned char sigNeg
        );

        //Srting based instantiation = The instantiation with the paramters of strToBi() function --> call strToBi()
        //String alapú példányosítás = A strToBi() függvénynek megfelelő paraméterekkel történő példányosítás --> strToBi() meghívása
        BinBigInt
        (
            const std::string & as,      //A konvertálandó string referenciája
            const unsigned char nfb,     //A bemeneti sztring számformátum alapja [2-35] (pl. bin = 2 vagy BIN, oct = 8 vagy OCT, dec = 10 vagy DEC, hex = 16 vagy HEX) [Min. 2, Max. 35 lehet]
            const std::string sepStr,    //A tagoló sztring
            const ulli sepw              //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
        );

        //Copy constructor
        //Másolókonstruktor (Erre valójában nincs szükség mert jó az alapértelmezett is...)
        BinBigInt
        (
            const BinBigInt & a //A lemásolandó BinBigInt objektum referenciája
        );

    //OPERATOS
    //OPERÁTOROK

        // = operator
        // = operátor (Erre valójában nincs szükség mert jó az alapértelmezett is...)
        BinBigInt& operator=(const BinBigInt & a);

        // += operator
        // += operátor
        BinBigInt& operator+=(const BinBigInt & a);

        // -= operator
        // -= operátor
        BinBigInt& operator-=(const BinBigInt & a);

        // ++ postfix operator
        // ++ postfix operátor (Az int nc paraméter csak a fordító számára történő jelzés, hogy ez egy post fix operátor)
        BinBigInt& operator++(int nc);

        // -- postfix operator
        // -- postfix operátor (Az int nc paraméter csak a fordító számára történő jelzés, hogy ez egy post fix operátor)
        BinBigInt& operator--(int nc);

        // ++ prefix operator
        // ++ prefix operátor
        BinBigInt& operator++();

        // -- prefix operator
        // -- prefix operátor
        BinBigInt& operator--();

/* //Amíg a tagok public elérésűek addig a freind függvények deklarálására nincs szükség...

        // + operátor
        friend BinBigInt operator+(const BinBigInt & a, const BinBigInt & b);

        // - operátor
        friend BinBigInt operator-(const BinBigInt & a, const BinBigInt & b);

        // * operátor
        friend BinBigInt operator*(const BinBigInt & a, const BinBigInt & b);

        // / operátor
        friend BinBigInt operator/(const BinBigInt & a, const BinBigInt & b);

        // % operátor (maradék)
        friend BinBigInt operator%(const BinBigInt & a, const BinBigInt & b);


        // - operátor (negatív előjel)
        friend BinBigInt operator-(const BinBigInt & a);


        // == operátor
        friend bool operator==(const BinBigInt & a, const BinBigInt & b);

        // != operátor
        friend bool operator!=(const BinBigInt & a, const BinBigInt & b);

        // <= operátor
        friend bool operator<=(const BinBigInt & a, const BinBigInt & b);

        // >= operátor
        friend bool operator>=(const BinBigInt & a, const BinBigInt & b);

        // < operátor
        friend bool operator<(const BinBigInt & a, const BinBigInt & b);

        // > operátor
        friend bool operator>(const BinBigInt & a, const BinBigInt & b);

        //SHL operátor (eltolás mértéke ulli)
        friend BinBigInt operator<<(const BinBigInt & a, const ulli n);

        //SHL operátor OVERIDE (eltolás mértéke BinBigInt)
        friend BinBigInt operator<<(const BinBigInt & a, const BinBigInt & n);

        //SHR operátor (eltolás mértéke ulli)
        friend BinBigInt operator>>(const BinBigInt & a, const ulli n);

        //SHR operátor OVERIDE (eltolás mértéke BinBigInt)
        friend BinBigInt operator>>(const BinBigInt & a, const BinBigInt & n);

        // & operátor
        friend BinBigInt operator&(const BinBigInt & a, const BinBigInt & b);

        // ^ operátor (XOR)
        friend BinBigInt operator^(const BinBigInt & a, const BinBigInt & b);

        // | operátor
        friend BinBigInt operator|(const BinBigInt & a, const BinBigInt & b);

        // ~ operátor
        friend BinBigInt operator~(const BinBigInt & a);
/*
        // operátor
        friend BinBigInt operator(const BinBigInt & a, const BinBigInt & b);
//*/

    //MEMBER FUNCTIONS
    //TAG FÜGGVÉNYEK

        //The bigint signum resulting function
        //A bigint előjelét visszaadó függvény (mivel/amíg a sigNeg publikus lényegében felesleges)
        unsigned char getSigNeg();

        //The function resulting the data pointer of vector which the base of the bigint
        //A bigint alapjául szolgáló vektor adat pointerét visszaadó függvény (mivel/amíg a "v" publikus valójában nem lenne szükséges de a ".v" leírása a kódban megspóróolható vele)
        ulli * data();

        //The function resulting the size of the vector which the base of the bigint
        //A bigint alapjául szolgáló vektor méretét visszaadó függvény (mivel/amíg a "v" publikus valójában nem lenne szükséges de a ".v" leírása a kódban megspóróolható vele)
        ulli size();

        //Function to check if bigint is zero
        //Nulla érték ellenőrző függvény
        static bool biIsZero
        (
            const std::vector<ulli> & a,  //The number to check container dynamic vector's reference //Az ellenőrizendő számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn       //The signum of number to check //Az ellenőrizendő szám előjele
        );

        //Nulla érték ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static bool biIsZero
        (
            const BinBigInt & a  //The number to check container BinBigInt's reference //Az ellenőrizendő számot tartalmazó BinBigInt referenciája
        );

        //Nulla érték ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        bool biIsZero();

        //Function to set bigint to zero (not modify the size)
        //Nullázó függvény (méretet nem módosít)
        static void biSetToZero
        (
            std::vector<ulli> & r, //A nullázandó dinamikus vektor referenciája
            unsigned char & rSn    //A nullázandó szám előjelének referenciája
        );

        //Nullázó függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biSetToZero
        (
            BinBigInt & r //A nullázandó BinBigInt referenciája
        );

        //Nullázó függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biSetToZero();

        //Function to copy bigint by value (the size of the bigint/vector set to the same as size of "a" parameter)
        //Másoló függvény (a méretet az "a" paraméter méretére állítja)
        static void biCopy
        (
            const std::vector<ulli> & a,  //A másolandó számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,      //A másolandó szám előjele
            std::vector<ulli> & r,        //A másolat vektor referenciája
            unsigned char & rSn           //A másolat szám előjelének referenciája
        );

        //Másoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biCopy
        (
            const BinBigInt & a,  //A másolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r  //A másolat vektor referenciája
        );

        //Másoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biCopy
        (
            BinBigInt & r  //A másolat vektor referenciája
        );

        //Funtion to resize the bigint
        //Átméretező függény
        static void biResize
        (
            const std::vector<ulli> & a, //Az igazítandó/átméretezendő számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,     //Az igazítandó/átméretezendő szám előjele
            const ulli newSize,          //Az új méret
            std::vector<ulli> & r,       //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn          //The signum of the result's reference //Az eredmény szám előjelének referenciája
        );

        //Átméretező függény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biResize
        (
            const BinBigInt & a, //Az igazítandó/átméretezendő számot tartalmazó BinBigInt referenciája
            const ulli newSize,  //Az új méret
            BinBigInt & r        //The result vector's reference //Az erdemény vektor referenciája
        );

        //Átméretező függény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biResize
        (
            const ulli newSize, //Az új méret
            BinBigInt & r       //The result vector's reference //Az erdemény vektor referenciája
        );

        //Átméretező függény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biResize
        (
            const BinBigInt & a,    //Az igazítandó/átméretezendő számot tartalmazó BinBigInt referenciája
            const ulli newSize      //Az új méret
        );

        //Átméretező függény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biResize
        (
            const ulli newSize  //Az új méret
        );

        //Function to size extending (signum correctly extend the size of the vector if the given size is bigger than the actual size)
        //(This suitable for safe (surely truncate avoiding) aligning two vector together without pre-comparison.)
        //Méret kiterjesztő/növelő függvény (előjel helyesen megnöveli a vektor méretét ha a megadott méret nagyobb mint az aktuális méret)
        //(Két vektor előellenőrzés nélküli biztonságos (biztosan csoknolás mentes) egymáshoz igazítására használható)
        static void biExtendIfShorter
        (
            const std::vector<ulli> & a, //Az igazítandó/átméretezendő számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,     //Az igazítandó/átméretezendő szám előjele
            const ulli sizeToExt,        //A méret amire meg kell növelni / ki kell terjeszteni
            std::vector<ulli> & r,       //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn          //The signum of the result's reference //Az eredmény szám előjelének referenciája
        );

        //Méret kiterjesztő/növelő függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biExtendIfShorter
        (
            const BinBigInt & a,    //Az igazítandó/átméretezendő számot tartalmazó BinBigInt referenciája
            const ulli sizeToExt,   //A méret amire meg kell növelni / ki kell terjeszteni
            BinBigInt & r           //The result vector's reference //Az erdemény vektor referenciája
        );

        //Méret kiterjesztő/növelő függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biExtendIfShorter
        (
            const ulli sizeToExt,   //A méret amire meg kell növelni / ki kell terjeszteni
            BinBigInt & r           //The result vector's reference //Az erdemény vektor referenciája
        );

        //Méret kiterjesztő/növelő függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biExtendIfShorter
        (
            const BinBigInt & a,    //Az igazítandó/átméretezendő számot tartalmazó BinBigInt referenciája
            const ulli sizeToExt    //A méret amire meg kell növelni / ki kell terjeszteni
        );

        //Méret kiterjesztő/növelő függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biExtendIfShorter
        (
            const ulli sizeToExt   //A méret amire meg kell növelni / ki kell terjeszteni
        );

        //Bitwise and function (operation extended to the singum too)
        //Bitenkénti ÉS (AND) függvény (előjelre is kiterjdeően)
        static void biBAnd
        (
            std::vector<ulli> a,    //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor
            unsigned char aSn,      //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> b,    //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor
            unsigned char bSn,      //The signum of other of the numbers //A másik szám előjele
            std::vector<ulli> & r,  //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn     //The signum of the result's reference //Az eredmény szám előjelének referenciája (A két bemeneti szám előjelén is elvégzésre kerül a művelet)
        );

        //Bitenkénti ÉS (AND) függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biBAnd
        (
            const BinBigInt & a,    //Az egyik számot tartalmazó BinBigInt referenciája
            const BinBigInt & b,    //A másik számot tartalmazó BinBigInt referenciája
            BinBigInt & r           //The result vector's reference //Az erdemény vektor referenciája
        );

        //Bitenkénti ÉS (AND) függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biBAnd
        (
            const BinBigInt & b,    //A másik számot tartalmazó BinBigInt referenciája
            BinBigInt & r           //The result vector's reference //Az erdemény vektor referenciája
        );

        //Bitwise inclusive or (OR) function (operation extended to the singum too)
        //Bitenkénti VAGY (OR) függvény (előjelre is kiterjdeően)
        static void biBOr
        (
            std::vector<ulli> a,    //The one of the numbers container dynamic vector //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor
            unsigned char aSn,      //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> b,    //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor
            unsigned char bSn,      //The signum of other of the numbers //A másik szám előjele
            std::vector<ulli> & r,  //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn     //The signum of the result's reference //Az eredmény szám előjelének referenciája (A két bemeneti szám előjelén is elvégzésre kerül a művelet)
        );

        //Bitenkénti VAGY (OR) függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biBOr
        (
            const BinBigInt & a, //Az egyik számot tartalmazó BinBigInt referenciája
            const BinBigInt & b,  //A másik számot tartalmazó BinBigInt referenciája
            BinBigInt & r        //The result vector's reference //Az erdemény vektor referenciája
        );

        //Bitenkénti VAGY (OR) függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biBOr
        (
            const BinBigInt & b,  //A másik számot tartalmazó BinBigInt referenciája
            BinBigInt & r        //The result vector's reference //Az erdemény vektor referenciája
        );

        //Bitwise exclusive or (XOR) function (operation extended to the singum too)
        //Bitenkénti KIZÁRÓ VAGY (XOR) függvény (előjelre is kiterjdeően)
        static void biBXor
        (
            std::vector<ulli> a,    //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor
            unsigned char aSn,      //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> b,    //The other of the numbers container dynamic vector //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor
            unsigned char bSn,      //The signum of other of the  numbers //A másik szám előjele
            std::vector<ulli> & r,  //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn     //The signum of the result's reference //Az eredmény szám előjelének referenciája (A két bemeneti szám előjelén is elvégzésre kerül a művelet)
        );

        //Bitenkénti KIZÁRÓ VAGY (XOR) függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biBXor
        (
            const BinBigInt & a,    //Az egyik számot tartalmazó BinBigInt referenciája
            const BinBigInt & b,    //A másik számot tartalmazó BinBigInt referenciája
            BinBigInt & r           //The result vector's reference //Az erdemény vektor referenciája
        );

        //Bitenkénti KIZÁRÓ VAGY (XOR) függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biBXor
        (
            const BinBigInt & b,    //A másik számot tartalmazó BinBigInt referenciája
            BinBigInt & r           //The result vector's reference //Az erdemény vektor referenciája
        );

        //Bitwise negate (NOT, NEG) function
        //Bitenkénti NEGÁLÓ (NOT, NEG) függvény
        static void biBNot
        (
            const std::vector<ulli> a,  //A negálandó számot tartalmazó dinamikus vektor
            const unsigned char aSn,    //A negálandó szám előjele
            std::vector<ulli> & r,      //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn         //The signum of the result's reference //Az eredmény szám előjelének referenciája
        );

        //Bitenkénti NEGÁLÓ (NOT) függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biBNot
        (
            const BinBigInt & a, //A negálandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r        //The result vector's reference //Az erdemény vektor referenciája
        );

        //Bitenkénti NEGÁLÓ (NOT) függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biBNot
        (
            BinBigInt & r   //The result vector's reference //Az erdemény vektor referenciája
        );

        //Function to remove the leading all zero or all one ulli elements (this modify the +0 valued vector to +0 valued, 1 element sized vector, and the -1 valued vector to -1 valued, 1 element sized vector)
        //Bevezető pozitív csupa nulla vagy negatív csupaegy ulli elemeket eltávolító függvény (a +0 értékű vektort +0 értékű 1 elemű vektorra, a -1 értékű vektort -1 értékű 1 elemű vektorra módosítja)
        static void biLeaderFillerCut //LeaderFillerCUT
        (
            const std::vector<ulli> & a, //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,     //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,       //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn          //The signum of the result's reference //The signum of the result's reference //Az eredmény szám előjelének referenciája
        );

        //Bevezető nulla elemeket (ulli-kat) eltávolító függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biLeaderFillerCut
        (
            const BinBigInt & a, //Az erdeti vektor referenciája
            BinBigInt & r        //The result BinBigInt's reference //Az eredmény BinBigInt referenciája
        );


        //Bevezető nulla byteokat eltávolító függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biLeaderFillerCut
        (
            BinBigInt & r  //The result BinBigInt's reference //Az eredmény BinBigInt referenciája
        );

        //Bevezető nulla elemeket (ulli-kat) eltávolító függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biLeaderFillerCut
        (
            const BinBigInt & a  //Az erdeti vektor referenciája
        );


        //Bevezető nulla byteokat eltávolító függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biLeaderFillerCut();

        //The operative addition subfunction (It makes the addition without the touch and regardless the value of singum and with the required carry transmission.)
        //(The function's vector parameters must be the same sized!!! [This guaranteed by the biAdd()])
        //A tényleges összeadást végző alfüggvény (Az előjelek érintése NÉLKÜL és azokra való tekintet NÉLKÜL az aktuális carry szükségszerű átvitelével végzi az összeadást.)
        //(A paraméterül kapott vektoroknak azonos méretűeknek kell lenniük!!! [Ezt garantálja a biAdd()])
        static unsigned char biAddSF0
        (
            const std::vector<ulli> a,  //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor
            const std::vector<ulli> b,  //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor
            std::vector<ulli> & r       //The result vector's reference //Az erdemény vektor referenciája
        );

        //Bigint addition function
        //Bigint összeadás
        static unsigned char biAdd
        (
            std::vector<ulli> a,    //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor
            unsigned char aSn,      //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> b,    //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor
            unsigned char bSn,      //The signum of other of the numbers //A másik szám előjele
            std::vector<ulli> & r,  //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn     //The signum of the result's reference //Az eredmény szám előjelének referenciája
        );

        //Bigint összeadás OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biAdd
        (
            const BinBigInt & a,  //Az egyik számot tartalmazó BinBigInt referenciája
            const BinBigInt & b,  //A másik számot tartalmazó BinBigInt referenciája
            BinBigInt & r         //The result BinBigInt's reference //Az eredmény BinBigInt referenciája
        );

        //Bigint összeadás OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biAdd
        (
            const BinBigInt & b,  //A másik számot tartalmazó BinBigInt referenciája
            BinBigInt & r         //The result BinBigInt's reference //Az eredmény BinBigInt referenciája
        );

        //Increment function
        //Inkrementáló függvény
        static void biInc
        (
            std::vector<ulli> & r,  //A növelendő számot tartalmazó dinamikus vektor referenciája
            unsigned char & rSn     //A növelendő szám előjele
        );

        //Inkrementáló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biInc
        (
            BinBigInt & r   //A növelendő számot tartalmazó BinBigInt referenciája
        );

        //Inkrementáló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biInc();

        //The operative subtraction subfunction (It makes the subtraction without the touch and regardless the value of singum and with the required carry transmission.)
        //(The function's vector parameters must be the same sized!!! [This guaranteed by the biAdd()])
        //A tényleges kivonást végző sub függvény (Az előjelek érintése NÉLKÜL és azokra való tekintet NÉLKÜL az aktuális carry szükségszerű átvitelével végzi a kivonást.)
        //(A paraméterül kapott vektoroknak azonos méretűeknek kell lenniük!!! Ezt garantálja a biSub())
        static unsigned char biSubSF0
        (
            const std::vector<ulli> a,  //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor
            const std::vector<ulli> b,  //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor
            std::vector<ulli> & r       //The result vector's reference //Az erdemény vektor referenciája
        );

        //Bigint subtraction function
        //Bigint kivonás
        static unsigned char biSub
        (
            std::vector<ulli> a,    //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor
            unsigned char aSn,      //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> b,    //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor
            unsigned char bSn,      //The signum of other of the numbers //A másik szám előjele
            std::vector<ulli> & r,  //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn     //The signum of the result's reference //Az eredmény szám előjelének referenciája
        );

        //Bigint kivonás OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biSub
        (
            const BinBigInt & a,    //Az alap számot tartalmazó BinBigInt referenciája
            const BinBigInt & b,    //A kivonandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r           //The result BinBigInt's reference //The result BinBigInt's reference //Az eredmény BinBigInt referenciája
        );

        //Bigint kivonás OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biSub
        (
            const BinBigInt & b,    //A kivonandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r           //The result BinBigInt's reference //Az eredmény BinBigInt referenciája
        );

        //Decrement function
        //Dekrementáló függvény
        static void biDec
        (
            std::vector<ulli> & r,  //A csökkentendő számot tartalmazó dinamikus vektor referenciája
            unsigned char & rSn     //A csökkentendő szám előjele
        );

        //Dekrementáló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biDec
        (
            BinBigInt & r   //A csökkentendő számot tartalmazó BinBigInt referenciája
        );

        //Dekrementáló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biDec();

        //Multiply fucntion
        //Szorzó függvény
        static void biMul
        (
            std::vector<ulli> a,    //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor
            unsigned char aSn,      //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> b,    //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor
            unsigned char bSn,      //The signum of other of the numbers //A másik szám előjele
            std::vector<ulli> & r,  //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn     //The signum of the result's reference //Az eredmény szám előjelének referenciája
        );

        //Szorzó függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biMul
        (
            const BinBigInt & a,    //Az egyik összeszorzandó számot tartalmazó dinamikus vektor referencia
            const BinBigInt & b,    //A másik összeszorzandó számot tartalmazó dinamikus vektor referencia
            BinBigInt & r           //Az eredményt tartalmazni hivatott dinamikus vektor referencia
        );

        //Szorzó függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biMul
        (
            const BinBigInt & b,    //A másik összeszorzandó számot tartalmazó dinamikus vektor referencia
            BinBigInt & r           //Az eredményt tartalmazni hivatott dinamikus vektor referencia
        );

        //Comparison function (It operates like the ISA CMP instructions: Subtract b from a and return the carry flag's value.)
        //(If a>=b --> 0 FALSE, if a<b --> 1 TRUE)
        //Biginteket összehasonlító függvény (Az ISA féle CMP utasításhoz hasonlóan mûködik: Az a-ból kivonja a b-t és a carryflag-et adja vissza.)
        //(Ha a>=b --> 0 FALSE, ha a<b --> 1 TRUE)
        static bool biCmp
        (
            std::vector<ulli> a,    //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor
            unsigned char aSn,      //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> b,    //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor
            unsigned char bSn       //The signum of other of the numbers //A másik szám előjele
        );

        //Biginteket összehasonlító függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static bool biCmp
        (
            const BinBigInt & a,    //Az egyik összehasonlítandó számot tartalmazó dinamikus vektor referencia
            const BinBigInt & b     //A másik összehasonlítandó számot tartalmazó dinamikus vektor referencia
        );

        //Biginteket összehasonlító függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        bool biCmp
        (
            const BinBigInt & b //A másik összehasonlítandó számot tartalmazó dinamikus vektor referencia
        );

        //Check if equal function
        //Egyenlőséget ellenőrző függvény
        static bool biIsEqual
        (
            const std::vector<ulli> & a,    //Az egyik öszehasonlítandó számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //Az egyik öszehasonlítandó szám előjele
            const std::vector<ulli> & b,    //A másik öszehasonlítandó számot tartalmazó dinamikus vektor referenciája
            const unsigned char bSn         //A másik öszehasonlítandó szám előjele
        );

        //Egyenlőséget ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static bool biIsEqual
        (
            const BinBigInt & a,    //Az egyik öszehasonlítandó számot tartalmazó BinBigInt referenciája
            const BinBigInt & b     //A másik öszehasonlítandó számot tartalmazó BinBigInt referenciája
        );

        //Egyenlőséget ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        bool biIsEqual
        (
            const BinBigInt & b //A másik öszehasonlítandó számot tartalmazó BinBigInt referenciája
        );

        //Check if not equal function
        //Nem egyenlőséget ellenőrző függvény
        static bool biIsNotEqual
        (
            const std::vector<ulli> & a, //Az egyik elenőrizendő számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,     //Az egyik elenőrizendő szám előjele
            const std::vector<ulli> & b, //A másik elenőrizendő számot tartalmazó dinamikus vektor referenciája
            const unsigned char bSn      //A másik elenőrizendő szám előjele
        );

        //Egyenlőséget ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static bool biIsNotEqual
        (
            const BinBigInt & a,    //Az egyik elenőrizendő számot tartalmazó BinBigInt referenciája
            const BinBigInt & b     //A másik elenőrizendő számot tartalmazó BinBigInt referenciája
        );

        //Egyenlőséget ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        bool biIsNotEqual
        (
            const BinBigInt & b //A másik elenőrizendő számot tartalmazó BinBigInt referenciája
        );

        //Check if bigger or equal function
        //Nagyobb egyenlő mint ellenőrző függvény
        static bool biIsBiggerOrEqual
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            const std::vector<ulli> & b,    //The other of the numbers container vector's reference //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor referenciája
            const unsigned char bSn         //The signum of other of the numbers //A másik szám előjele
        );

        //Nagyobb egyenlő mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static bool biIsBiggerOrEqual
        (
            const BinBigInt & a,    //Az összehasonlítandó számot tartalmazó BinBigInt referenciája
            const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
        );

        //Nagyobb egyenlő mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        bool biIsBiggerOrEqual
        (
            const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
        );

        //Check if smaller or equal function
        //Kisebb egyenlő mint ellenőrző függvény
        static bool biIsSmallerOrEqual
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            const std::vector<ulli> & b,    //The other of the numbers container vector's reference //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor referenciája
            const unsigned char bSn         //The signum of other of the numbers //A másik szám előjele
        );

        //Kisebb egyenlő mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static bool biIsSmallerOrEqual
        (
            const BinBigInt & a,    //Az összehasonlítandó számot tartalmazó BinBigInt referenciája
            const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
        );

        //Kisebb egyenlő mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        bool biIsSmallerOrEqual
        (
            const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
        );

        //Check if smaller function
        //Kisebb mint ellenőrző függvény
        static bool biIsSmaller
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            const std::vector<ulli> & b,    //The other of the numbers container vector's reference //The other of the numbers container vector's reference //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor referenciája
            const unsigned char bSn         //The signum of other of the numbers //A másik szám előjele
        );

        //Kisebb mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static bool biIsSmaller
        (
            const BinBigInt & a,    //Az összehasonlítandó számot tartalmazó BinBigInt referenciája
            const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
        );

        //Kisebb mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        bool biIsSmaller
        (
            const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
        );

        //Check if bigger function
        //Nagyobb mint ellenőrző függvény
        static bool biIsBigger
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            const std::vector<ulli> & b,    //The other of the numbers container vector's reference //The other of the numbers container dynamic vector //A másik számot tartalmazó dinamikus vektor referenciája
            const unsigned char bSn         //The signum of other of the numbers //A másik szám előjele
        );

        //Nagyobb mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static bool biIsBigger
        (
            const BinBigInt & a,    //Az összehasonlítandó számot tartalmazó BinBigInt referenciája
            const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
        );

        //Nagyobb mint ellenőrző függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        bool biIsBigger
        (
            const BinBigInt & b     //Az összehasonlítás alapját képező számot tartalmazó BinBigInt referenciája
        );

        //Power function
        //Hatványozó függvény
        static void biPow
        (
            const std::vector<ulli> a,  //A hatványozandó számot (hatvány alapot) tartalmazó dinamikus vektor
            const unsigned char aSn,    //A hatványozandó szám előjele
            const std::vector<ulli> n,  //A hatvány kitevő számot tartalmazó dinamikus vektor
            const unsigned char nSn,    //A hatvány kitevő szám előjele
            std::vector<ulli> & r,      //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn         //Az eredény szám előjel referenciája
        );
        //Hatványozó függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biPow
        (
            const BinBigInt & a,    //A hatvány alapját tartalmazó dinamikus vektor referencia
            const BinBigInt & n,    //A hatvány kitevőt tartalmazó dinamikus vektor referencia
            BinBigInt & r           //Az eredményt tartalmazni hivatott BinBigInt referenciája
        );

        //Hatványozó függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biPow
        (
            const BinBigInt & n,    //A hatvány kitevőt tartalmazó dinamikus vektor referencia
            BinBigInt & r           //Az eredményt tartalmazni hivatott BinBigInt referenciája
        );

        //Hatványozó függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt BinBigInt::biPow
        (
            const BinBigInt & a,    //A hatvány alapját tartalmazó dinamikus vektor referencia
            const BinBigInt & n     //A hatvány kitevőt tartalmazó dinamikus vektor referencia
        );

        //Hatványozó függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt BinBigInt::biPow
        (
            const BinBigInt & n     //A hatvány kitevőt tartalmazó dinamikus vektor referencia
        );


        //Shift left function (If overflow happen then depending on the extend parameter it truncate the content or extend the size of the vector but if extend the vector it gives back the overflow 1 value too)
        //In the case of the count of shift is bigger than 1 it gives back the last steps carry value interpreted on the original vector size in the cl! (Carry Last)
        //Because n is unsigned type if negative value given the shift operates of the number's two's complements times
        //e.g. if it called with ((ulli) -1) value then it's raw vector value is 0xffffffffffffffff (= ulli MAX) so in this case it shifting 18,446,744,073,709,551,615 bits left.
        //Biginteket balra eltoló függvény (Ha felülcsordulás történik akkor az extend paramétertõl függõen csonkolja a tartalmat vagy megnöveli a vektor méretet de a megnövelés esetén is ad vissza overflow 1 értéket)
        //1-nél nagyobb számú eltolás esetén az eredetei vektorméreten értelmezve visszaadja az utolsó lépés carry értékét is a cl-ben! (Carry Last)
        //mivel n unsigned típus negatív szám esetén a szám kettes komplemensének megfelelő számban hajtódik végre
        //pl. ha egy ((ulli) -1) értékkel hívjuk meg annak nyers vektor adatértéke 0xffffffffffffffff (= ulli MAX) szóval ez esetben 18,446,744,073,709,551,615 bitnyi eltolást végez
        static unsigned char biShl
        (
            const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //A szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of the result's reference //Az eredmény szám előjelének referenciája
            const ulli n,                   //The count of the shifting //Az eltolás mértéke
            unsigned char & cl,             //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték (CarryLast)
            const unsigned char extend      //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShl
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n,               //The count of the shifting //Az eltolás mértéke
            unsigned char & cl,         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShl
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n,               //The count of the shifting //Az eltolás mértéke
            unsigned char & cl,         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        static unsigned char biShlNoCl
        (
            const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //A szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of the result's reference //Az eredmény szám előjelének referenciája
            const ulli n,                   //The count of the shifting //Az eltolás mértéke
            const unsigned char extend      //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShlNoCl
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n,               //The count of the shifting //Az eltolás mértéke
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShlNoCl
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n,               //The count of the shifting //Az eltolás mértéke
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biShlNoClNoOf
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            const ulli n,               //The count of the shifting //Az eltolás mértéke
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biShlNoClNoOf
        (
            const ulli n,               //The count of the shifting //Az eltolás mértéke
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        static unsigned char biShlExtend
        (
            const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //A szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of the result's reference //Az eredmény szám előjelének referenciája
            const ulli n,                   //The count of the shifting //Az eltolás mértéke
            unsigned char & cl              //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték (CarryLast)
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShlExtend
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n,               //The count of the shifting //Az eltolás mértéke
            unsigned char & cl          //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShlExtend
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n,               //The count of the shifting //Az eltolás mértéke
            unsigned char & cl          //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        static unsigned char biShlExtendNoCl
        (
            const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //A szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of the result's reference //Az eredmény szám előjelének referenciája
            const ulli n                    //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShlExtendNoCl
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n                //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShlExtendNoCl
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n                //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biShlExtendNoClNoOf
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            const ulli n                //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biShlExtendNoClNoOf
        (
            const ulli n                //The count of the shifting //Az eltolás mértéke
        );

        static unsigned char biShlTruncate
        (
            const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //A szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of the result's reference //Az eredmény szám előjelének referenciája
            const ulli n,                   //The count of the shifting //Az eltolás mértéke
            unsigned char & cl             //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték (CarryLast)
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShlTruncate
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n,               //The count of the shifting //Az eltolás mértéke
            unsigned char & cl         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShlTruncate
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n,               //The count of the shifting //Az eltolás mértéke
            unsigned char & cl         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        static unsigned char biShlTruncateNoCl
        (
            const std::vector<ulli> & a,    //A számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //A szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of the result's reference //Az eredmény szám előjelének referenciája
            const ulli n                    //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShlTruncateNoCl
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n                //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShlTruncateNoCl
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n                //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biShlTruncateNoClNoUf
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            const ulli n                //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biShlTruncateNoClNoUf
        (
            const ulli n                //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény - OVERRIDE: Az eltolás lépészáma is vektor
        //(Ha felülcsordulás történik akkor a megadott paramétertõl függõen csonkol vagy megnöveli a vektor méretet de a megnövelés esetén során is ad vissza overflow értéket)
        //1-nél nagyobb számú eltolás esetén az eredetei vektorméreten értelmezve visszaadja az utolsó lépés carry értékét is a cl-ben! (Carry Last)
        //n vektor szándékosan csak előjel nélkül adható meg, ennek megfelelően a vektor nyers adatai szerinti eltolást végez a függvény, tehát ha negatív számhoz tartozó nyers vektort adunk meg akkor a kettes komplemens szerint léptet
        //pl. ha egy -1 értére definiált BinBigInt vektor tagjával hívjuk meg annak nyers vektor adatértéke 0xffffffffffffffff (= ulli MAX) szóval ez esetben 18,446,744,073,709,551,615 bitnyi eltolást végez
        //A BinBigInt overrideok negatív számmal való meghívás esetén kivétlet dobnak
        static unsigned char biShl
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,          //The result vector's reference //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of result's reference //Az erdmény előjel referenciája
            const std::vector<ulli> n,      //The count of the shifting //Az eltolás mértéke
            unsigned char & cl,             //The reference of the carry which happening at the last step of the shifting interpreted on the original siz of the vector //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
            const unsigned char extend      //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShl
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n,        //Az eltolás mértékének referenciája
            unsigned char & cl,         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShl
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n,        //Az eltolás mértékének referenciája
            unsigned char & cl,         //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        static unsigned char biShlNoCl
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of result's reference //The signum of result's reference //Az erdmény előjel referenciája
            const std::vector<ulli> n,      //The count of the shifting //Az eltolás mértéke
            const unsigned char extend      //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShlNoCl
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n,        //Az eltolás mértékének referenciája
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShlNoCl
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n,        //Az eltolás mértékének referenciája
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biShlNoClNoOf
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            const BinBigInt & n,        //Az eltolás mértékének referenciája
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biShlNoClNoOf
        (
            const BinBigInt & n,        //Az eltolás mértékének referenciája
            const unsigned char extend  //1 (vagy nagyobb) Megnöveli a vektort / 0 a vektorméret marad és csonkolja az adatot HA túlcsordulás történik.
        );

        static unsigned char biShlExtend
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of result's reference //Az erdmény előjel referenciája
            const std::vector<ulli> n,      //The count of the shifting //Az eltolás mértéke
            unsigned char & cl              //The reference of the carry which happening at the last step of the shifting interpreted on the original siz of the vector //The reference of the carry which happening at the last step of the shifting interpreted on the original siz of the vector //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShlExtend
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n,        //Az eltolás mértékének referenciája
            unsigned char & cl          //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShlExtend
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n,        //Az eltolás mértékének referenciája
            unsigned char & cl          //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        static unsigned char biShlExtendNoCl
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of result's reference //Az erdmény előjel referenciája
            const std::vector<ulli> n       //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShlExtendNoCl
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n         //Az eltolás mértékének referenciája
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShlExtendNoCl
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n         //Az eltolás mértékének referenciája
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biShlExtendNoClNoOf
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            const BinBigInt & n         //Az eltolás mértékének referenciája
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biShlExtendNoClNoOf
        (
            const BinBigInt & n        //Az eltolás mértékének referenciája
        );

        static unsigned char biShlTruncate
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of result's reference //Az erdmény előjel referenciája
            const std::vector<ulli> n,      //The count of the shifting //Az eltolás mértéke
            unsigned char & cl              //The reference of the carry which happening at the last step of the shifting interpreted on the original siz of the vector //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShlTruncate
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n,        //Az eltolás mértékének referenciája
            unsigned char & cl          //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShlTruncate
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n,        //Az eltolás mértékének referenciája
            unsigned char & cl          //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        static unsigned char biShlTruncateNoCl
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of result's reference //Az erdmény előjel referenciája
            const std::vector<ulli> n       //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShlTruncateNoCl
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n         //Az eltolás mértékének referenciája
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShlTruncateNoCl
        (
            BinBigInt & r,              //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n         //Az eltolás mértékének referenciája
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biShlTruncateNoClNoUf
        (
            const BinBigInt & a,        //Az eltolandó számot tartalmazó BinBigInt referenciája
            const BinBigInt & n         //Az eltolás mértékének referenciája
        );

        //Biginteket balra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biShlTruncateNoClNoUf
        (
            const BinBigInt & n        //Az eltolás mértékének referenciája
        );

        //Shift right function (If underflow happen then truncate the content. It returns the carry value which happen in the last step at the cl.)
        //Biginteket jobbra eltoló függvény (Ha alulcsordulás történik akkor a kilépõ biteket csonkolja. Az utolsó lépésben bekövetkezett carry értéket a cl-ben adja vissza.)
        static unsigned char biShr
        (
            const std::vector<ulli> & a, //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,     //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,       //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,         //The signum of the result's reference //Az eredmény szám előjelének referenciája
            const ulli n,                //The count of the shifting //Az eltolás mértéke
            unsigned char & cl           //The reference of the carry which happening at the last step of the shifting interpreted on the original siz of the vector //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShr
        (
            const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n,           //The count of the shifting //Az eltolás mértéke
            unsigned char & cl      //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShr
        (
            BinBigInt & r,      //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n,       //The count of the shifting //Az eltolás mértéke
            unsigned char & cl  //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        //Biginteket jobbra eltoló függvény az utolsó lépésben történt eltolás underflow érétkét adja vissza (Ha alulcsordulás történik akkor a kilépõ biteket csonkolja)
        static unsigned char biShrNoCl
        (
            const std::vector<ulli> & a, //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,     //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,       //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,         //The signum of the result's reference //Az eredmény szám előjelének referenciája
            const ulli n                 //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShrNoCl
        (
            const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n            //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShrNoCl
        (
            BinBigInt & r,      //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const ulli n        //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biShrNoCl
        (
            const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
            const ulli n            //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biShrNoCl
        (
            const ulli n       //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biShrNoClNoUf
        (
            const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
            const ulli n     //Az eltolás mértékének referenciája
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biShrNoClNoUf
        (
            const ulli n     //Az eltolás mértékének referenciája
        );

        //Biginteket jobbra eltoló függvény - OVERRIDE: Az eltolás lépészáma is vektor
        //az utolsó lépésben történt eltolás underflow érétkét adja vissza (Ha alulcsordulás történik akkor a kilépõ biteket csonkolja)
        static unsigned char biShr
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of result's reference //Az erdmény előjel referenciája
            const std::vector<ulli> n,      //The count of the shifting //Az eltolás mértéke
            unsigned char & cl              //The reference of the carry which happening at the last step of the shifting interpreted on the original siz of the vector //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték referenciája
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShr
        (
            const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n,    //Az eltolás mértékének referenciája
            unsigned char & cl      //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShr
        (
            BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n,    //Az eltolás mértékének referenciája
            unsigned char & cl      //Az utolsó eltolási lépésben az eredeti vektor méreten értelmezett carry érték
        );

        //Biginteket jobbra eltoló függvény - OVERRIDE: Az eltolás lépészáma is vektor
        //az utolsó lépésben történt eltolás underflow érétkét adja vissza (Ha alulcsordulás történik akkor a kilépõ biteket csonkolja)
        static unsigned char biShrNoCl
        (
            const std::vector<ulli> & a,    //The one of the numbers container vector's reference //The one of the numbers container dynamic vector //Az egyik számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & r,          //The result vector's reference //Az erdemény vektor referenciája
            unsigned char & rSn,            //The signum of result's reference //Az erdmény előjel referenciája
            const std::vector<ulli> n       //The count of the shifting //Az eltolás mértéke
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static unsigned char biShrNoCl
        (
            const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
            BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n     //Az eltolás mértékének referenciája
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        unsigned char biShrNoCl
        (
            BinBigInt & r,          //Az eredményt tartalmazni hivatott BinBigInt referenciája
            const BinBigInt & n     //Az eltolás mértékének referenciája
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biShrNoClNoUf
        (
            const BinBigInt & a,    //Az eltolandó számot tartalmazó BinBigInt referenciája
            const BinBigInt & n     //Az eltolás mértékének referenciája
        );

        //Biginteket jobbra eltoló függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biShrNoClNoUf
        (
            const BinBigInt & n     //Az eltolás mértékének referenciája
        );

        //Negative signum function = two's complement function
        //Bigint negatív előjel függvény = kettes komplemens függvény
        static void biNegative
        (
            const std::vector<ulli> & a,    //Az eredeti számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //Az a előjele
            std::vector<ulli> & r,          //Az eredményt tartalmazni hivatott dinamikus vektor referenciája
            unsigned char & rSn             //Az r előjele
        );

        //Bigint negatív előjel függvény = kettes komplemens függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biNegative
        (
            const BinBigInt & a,    //Az eredeti számot tartalmazó BinBigInt referenciája
            BinBigInt & r           //Az eredményt tartalmazni hivatott BinBigInt referenciája
        );

        //Bigint negatív előjel függvény = kettes komplemens  függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biNegative
        (
            BinBigInt & r   //Az eredményt tartalmazni hivatott BinBigInt referenciája
        );

        //Bigint negatív előjel függvény = kettes komplemens függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        BinBigInt BinBigInt::biNegative
        (
            const BinBigInt & a    //Az eredeti számot tartalmazó BinBigInt referenciája
        );

        //Bigint negatív előjel függvény = kettes komplemens  függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt BinBigInt::biNegative();

        //Absolute value function
        //Bigint abszolútérték függvény
        static void biAbs
        (
            const std::vector<ulli> & a,    //Az eredeti számot tartalmazó dinamikus vektor referenciája
            const unsigned char aSn,        //Az a előjele
            std::vector<ulli> & r,          //Az eredményt tartalmazni hivatott dinamikus vektor referenciája
            unsigned char & rSn             //Az r előjele
        );

        //Bigint abszolútérték függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biAbs
        (
            const BinBigInt & a,    //Az eredeti számot tartalmazó BinBigInt referenciája
            BinBigInt & r           //Az eredményt tartalmazni hivatott BinBigInt referenciája
        );

        //Bigint abszolútérték függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biAbs
        (
            BinBigInt & r   //Az eredményt tartalmazni hivatott BinBigInt referenciája
        );

        //Bigint abszolútérték függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biAbs
        (
            const BinBigInt & a    //Az eredeti számot tartalmazó BinBigInt referenciája
        );

        //Bigint abszolútérték függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biAbs();

        //Divide function (with remind)
        //Biginteger maradékos osztás
        static void biDiv
        (
            std::vector<ulli> a,        //The number to divide container vector //Az osztandó számot tartalmazó dinamikus vektor
            unsigned char aSn,          //The signum of the number to divide //Az osztandó szám előjele
            std::vector<ulli> b,        //The divider container vector //Az osztó számot tartalmazó dinamikus vektor
            unsigned char bSn,          //The signum of the divider //Az osztó  szám előjele
            std::vector<ulli> & quo,    //The quotient as result vector's reference //(quotient) Az eredmény hányadost tartalmazni hivatott dinamikus vektor referencia
            unsigned char & quoSn,      //The signum of one of the numers //Az egyik szám előjele
            std::vector<ulli> & rem,    //The reminder as result vector's reference //(reminder) A maradékot tartalmazni hivatott dinamikus vektor referencia
            unsigned char & remSn       //The signum of one of the numers //Az egyik szám előjele
        );

        //Biginteger maradékos osztás OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biDiv
        (
            const BinBigInt & a,    //Az osztandó számot tartalmazó BinBigInt referenciája
            const BinBigInt & b,    //Az osztó számot tartalmazó BinBigInt referenciája
            BinBigInt & quo,        //(quotient) Az eredmény hányadost tartalmazni hivatott BinBigInt referenciája
            BinBigInt & rem         //(reminder) A maradékot tartalmazni hivatott BinBigInt referenciája
        );

        //Biginteger maradékos osztás OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biDiv
        (
            const BinBigInt & b,    //Az osztó számot tartalmazó dinamikus vektor referencia
            BinBigInt & quo,        //(quotient) Az eredmény hányadost tartalmazni hivatott BinBigInt referenciája
            BinBigInt & rem         //(reminder) A maradékot tartalmazni hivatott BinBigInt referenciája
        );

        //Factorial function
        //Bigint faktoriális függvény (a megvalósítás akár meg hívhatná a biPosProductOfSequence() pozitív produktum függvényt is 1-es i paraméterrel mert lényegében azzal egyenértékű... Ez kicsit gyorsabb mert két feltétel vizsgálattal kevesebb van benne)
        static void biFactorial
        (
            const std::vector<ulli>  f,     //A faktoriális száma
            const unsigned char fSn,        //A faktoriális előjele (Csak pozitív lehet, csak ellenőrzés miatt szüskéges)
            std::vector<ulli> & r,          //Az eredményt tartalmazni hivatott vektor referenciája
            unsigned char & rSn             //Az eredmény előjele (Csak pozitív lehet)
        );

        //Bigint faktoriális függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biFactorial
        (
            const BinBigInt & f,    //A faktoriális száma
            BinBigInt & r           //Az eredményt tartalmazni hivatott vektor referenciája
        );

        //Bigint faktoriális függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biFactorial
        (
            BinBigInt & r   //Az eredményt tartalmazni hivatott vektor referenciája
        );

        //Bigint faktoriális függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biFactorial
        (
            const BinBigInt & f    //A faktoriális száma
        );

        //Bigint faktoriális függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biFactorial();

        //Product function (!!This only interpreted to positive integers!!!)
        //Bigint produktum függvény (!!!Csak pozitív egészekre értelmezett!!!)
        static void biPosProductOfSequence
        (
            std::vector<ulli>  i,     //A produktum kezdő száma
            unsigned char iSn,        //A produktum kezdő száma előjele (Csak pozitív lehet, csak ellenőrzés miatt szüskéges)
            std::vector<ulli>  n,     //A produktum befejező száma
            unsigned char nSn,        //A produktum befejező számának előjele (Csak pozitív lehet, csak ellenőrzés miatt szüskéges)
            std::vector<ulli> & r,    //Az eredményt tartalmazni hivatott vektor referenciája
            unsigned char & rSn       //Az eredmény előjele (Csak pozitív lehet)
        );

        //Bigint produktum függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biPosProductOfSequence
        (
            const BinBigInt  & i,     //A produktum kezdő száma
            const BinBigInt & n,    //A produktum befejező száma
            BinBigInt & r           //Az eredményt tartalmazni hivatott vektor referenciája
        );

        //Bigint produktum függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        void biPosProductOfSequence
        (
            const BinBigInt & n,    //A produktum kezdő száma
            BinBigInt & r   //Az eredményt tartalmazni hivatott vektor referenciája
        );

        //Bigint produktum függvény OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt biPosProductOfSequence
        (
            const BinBigInt & i,     //A produktum kezdő száma
            const BinBigInt & n     //A produktum befejező száma
        );

        //Bigint produktum függvény OVERRIDE: példányosított objektum tagfüggvényeként meghívva --> Az "a" bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott) az egyéb vektor referenciák helyett BinBigInt referenciák
        BinBigInt biPosProductOfSequence
        (
            const BinBigInt & n    //A produktum befejező száma
        );

        //Write Bigint to string function
        //Bigint sztringbe kiírása
        static void biToStr
        (
            std::vector<ulli> a,        //A konvertálandó bigint vektor referencia
            unsigned char aSn,          //A bemeneti bigint előjel
            const unsigned char nfb,    //(NumberFormatBase) A kimeneti számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
            std::string &rs,            //Eredmény sztring
            const std::string sepStr,   //A tagoló sztring
            const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
        );

        //Bigint sztringbe kiírása OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void biToStr
        (
            const BinBigInt & a,          //A konvertálandó bigint vektor referencia
            const unsigned char nfb,      //A kimeneti számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
            std::string &rs,              //Eredmény sztring
            const std::string sepStr,     //A tagoló sztring
            const ulli sepw               //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
        );
        //Bigint sztringbe kiírása OVERRIDE: példányosított objektum tagfüggvényekét meghívva --> A bemeneti érték maga az objektum vektora (bementi vektor referencia paraméter nem megadott)
        void biToStr
        (
            const unsigned char nfb,    //A kimeneti számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
            std::string &rs,            //Eredmény sztring
            const std::string sepStr,   //A tagoló sztring
            const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
        );

        //Write numeric value given in string format into BigInt
        //Sztring formátumbanmegadott szám Bigintbe írása
        static void strToBi
        (
            const std::string & as,     //A konvertálandó string referenciája
            const unsigned char nfb,    //A bemeneti sztring számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
            std::vector<ulli> &r,       //Az eredmény bigint vektor
            unsigned char &rSn,         //Az eredmény bigint előjel
            const std::string sepStr,   //A tagoló sztring
            const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
        );

        //Sztring formátumbanmegadott szám Bigintbe írása OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static void strToBi
        (
            const std::string & as,     //A konvertálandó string referenciája
            const unsigned char nfb,    //A bemeneti sztring számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
            BinBigInt &r,               //Az eredmény bigint vektor
            const std::string sepStr,   //A tagoló sztring
            const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
        );

        //Sztring formátumbanmegadott szám Bigintbe írása OVERRIDE: példányosított objektum tagfüggvényekkel meghívva --> A vektor referenciák helyett BinBigInt referenciák
        static BinBigInt BinBigInt::strToBi
        (
            const std::string & as,     //A konvertálandó string referenciája
            const unsigned char nfb,    //A bemeneti sztring számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
            const std::string sepStr,   //A tagoló sztring
            const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
        );

        //Sztring formátumbanmegadott szám Bigintbe írása OVERRIDE: példányosított objektum tagfüggvényekét meghívva --> A visszatérési érték maga az objektum (visszatérési referencia paraméter nem megadott)
        void strToThisBi
        (
            const std::string & as,     //A konvertálandó string referenciája
            const unsigned char nfb,    //A bemeneti sztring számformátum alapja [2-35] (pl. bin = 2, oct = 8, dec = 10, hex = 16) [Min. 2, Max. 35 lehet]
            const std::string sepStr,   //A tagoló sztring
            const ulli sepw             //A tagolsági szélesség ( = a tagoló karakterek között lévő számjegyek darabszáma)
        );

};

//OPERATOR FUNCTIONS
//OPERÁTOR FÜGGVÉNYEK

//Arithmetic operators
//Aritmetikai operátorok

// + operator
// + operátor
BinBigInt operator+(const BinBigInt & a, const BinBigInt & b);

// - operator (subtraction)
// - operátor (kivonás)
BinBigInt operator-(const BinBigInt & a, const BinBigInt & b);


// * operator
// * operátor
BinBigInt operator*(const BinBigInt & a, const BinBigInt & b);

// / operator
// / operátor
BinBigInt operator/(const BinBigInt & a, const BinBigInt & b);

// % operator (reminder)
// % operátor (maradék)
BinBigInt operator%(const BinBigInt & a, const BinBigInt & b);

// - operator (negative sign)
// - operátor (negatív előjel)
BinBigInt operator-(const BinBigInt & a);


//Bitwise operators
//Bit műveleti operátorok

// << operator (Shift left BinBigInt with extend it equivalent to multiply with 2^n)
// << operátor (BinBigInt kiterjesztéssel történő balra eltolása 2^n-el történő szorzással ekvivalens)
BinBigInt operator<<(const BinBigInt & a, const ulli n);

// << operator (Shift left BinBigInt with extend it equivalent to multiply with 2^n)
// << operátor (BinBigInt kiterjesztéssel történő balra eltolása 2^n-el történő szorzással ekvivalens)
BinBigInt operator<<(const BinBigInt & a, const BinBigInt & n);

// >> operator (Shift right BinBigInt with extend)
// >> operátor (BinBigInt kiterjesztéssel történő jobbra eltolása)
BinBigInt operator>>(const BinBigInt & a, const ulli n);

// >> operator (Shift right BinBigInt with extend)
// >> operátor (BinBigInt kiterjesztéssel történő jobbra eltolása)
BinBigInt operator>>(const BinBigInt & a, const BinBigInt & n);

// & operator
// & operátor
BinBigInt operator&(const BinBigInt & a, const BinBigInt & b);

// ^ operator (XOR)
// ^ operátor (XOR)
BinBigInt operator^(const BinBigInt & a, const BinBigInt & b);

// | operátor (OR)
// | operátor (OR)
BinBigInt operator|(const BinBigInt & a, const BinBigInt & b);

// ~ operátor (NEGATE)
// ~ operátor (NEGATE)
BinBigInt operator~(const BinBigInt & a);


//Comparison operators
//Összehasonlító operátorok

// == operator
// == operátor
bool operator==(const BinBigInt & a, const BinBigInt & b);

// != operator
// != operátor
bool operator!=(const BinBigInt & a, const BinBigInt & b);

// <= operator
// <= operátor
bool operator<=(const BinBigInt & a, const BinBigInt & b);

// >= operator
// >= operátor
bool operator>=(const BinBigInt & a, const BinBigInt & b);

// < operator
// < operátor
bool operator<(const BinBigInt & a, const BinBigInt & b);

// > operator
// > operátor
bool operator>(const BinBigInt & a, const BinBigInt & b);

#endif // BINBIGINT_H_INCLUDED
