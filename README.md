# BinBigInt
Binary Big Integer object and it's arithmetic, bit-manipulating and comparison operations base function's and some other function's (power, factorial, positive product, string &lt;-> BinBigInt conversion) implementation library

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
* The sift left bishl() function possible to call with extend and truncate behavior, for these two version there are two explicit named wrapper functions too.
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
* That functions which provide BinBigInt result and binded for any operator hasn't got distinct wrapper function version which's returned function value is BinBigInt. (The operator function available for this purpose.)
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
* A balra tolás bishl() függvény vektorméret növeléssel és csonkolással is hívható, ezen két verzióra külön egyértelmű elnevezésű wrapper függvények is rendelkezésre állnak.
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
