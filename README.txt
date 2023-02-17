312CD, Draganoiu Andreea

TEMA 2 - Ierarhie

Cerinta0 - Am afisat in preordine angajatii din ierarhie.

Cerinta1 - hire-> Apelez o functie (ReturnPerson) care imi da nodul
corespunzator managerului cautat. Realoc vectorul de angajati ai managerului 
pentru a face loc angajatului dat ca parametru care trebuie repartizat. Apoi 
sortez vectorul de angajati ai managerului pentru a mentine ordinea alfabetica.
	 - fire-> Realoc vectorul cu angajatii managerului angajatului parametru 
pentru a face loc copiilor sai. Dupa ce adaug copiii angajatului concediat alaturi
de el in vector, voi cauta pozitia acestui angajat pentru a-l elimina.
Sortez vectorul pentru ca adaug copiii la final iar apoi eliberez memoria
concediatului.
	 - promote-> Realoc memoria vectorului de angajati ai managerului 
angajatului  care trebuie promovat pentru a-i adauga subordonatii. Dupa ce adaug 
copiii angajatului promovat alaturi de el in vector, voi cauta pozitia acestui 
angajat pentru a-l elimina din acest vector. Angajatul va fi adaugat insa in echipa
managerului managerului si voi rupe vechile legaturi cu subordonatii sai. 
	 - move_employee-> Realoc vectorul de angajati ai managerului 
angajatului  care trebuie promovat pentru a-i adauga subordonatii. Dupa ce adaug 
copiii angajatului promovat alaturi de el in vector, voi cauta pozitia acestui 
angajat pentru a-l elimina din acest vector. Angajatul va fi adaugat insa in echipa
noul sau manager si voi rupe vechile legaturi cu subordonatii sai.
	 - move_team-> Caut pozitia in echipa managerului a angajatului care conduce 
echipa pentru a fi mutat impreuna cu echipa sa. Refac vectorul si fac loc in echipa
noul manager unde va fi mutat tot subarborele.
	 - fire_team-> Caut pozitia in echipa managerului a angajatului care conduce 
echipa pentru a fi concediat impreuna cu echipa sa. Eliberez apoi memoria echipei 
acestuia.

Cerinta2 - get_employees_by_manager-> Voi distribui intr-un vector tot subarborele de
anagajati impreuna cu acesta. Il sortez in ordine alfabetica.
	 - get_employees_by_level - Determin toti angajatii care au ordinul cerut si
ii pun intr-un vector pentru a-i putea afisa apoi in ordine alfabetica.
	 - get_best_manager-> Voi pune intr-un vector toti angajatii din arbore si voi
itera pentru a determina care este maximul de copii pe care poate un angajat sa ii aiba.
Parcurg din nou acest vector pentru a determina care sunt acei angajati care conduc
cei mai multi subordonati.

Cerinta3 - Voi parcurge linia de manageri pe care se afla angajatul in ordine inversa
pana la radacina. Parcurg cu 3 pointeri pentru nu a pierde legaturile, dar pentru ca
ultimul pointer(z) ajunge NULL inainte de a se efectua ultimul pas voi repeta procedura
doar pentru o legatura. Acest caz il folosesc si cand angajatul care trebuie mutat
are nivelul 1 in arbore.
