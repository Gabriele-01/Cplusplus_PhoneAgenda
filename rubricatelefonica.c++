//Questo semplice programma implementa una rubrica telefonica

#include <unicode/unistr.h>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <fstream>
#include <array>
#include <vector>
#include <chrono> //Usato per la data di nascita
#include <format>
#include <regex>
#include <optional>
#include <expected>
#include <utility>
using namespace std;

// Definisco la struct di un contatto della rubrica
struct structDatiContatto {
    int ID;
    string nome;
    string cognome;
    string numero_tel;
    string email;
    string indirizzo;
    chrono::year_month_day data_nascita; // Nel file ad esempio 01/02/2000
    bool preferiti = false;
};

ostream& operator<<(ostream& os, const structDatiContatto& c) {
    return os << c.ID << '\n'
              << c.nome << '\n'
              << c.cognome << '\n'
              << c.numero_tel << '\n'
              << c.email << '\n'
              << c.indirizzo << '\n'
              << c.data_nascita << '\n'
              << c.preferiti << '\n';

}

void scriviFile (const vector<structDatiContatto>& c, const string& nomeFile, ios::openmode modalita) {
    ofstream file(nomeFile, modalita);
    if (!file) {
        cerr << "Errore nell'apertura del file" << endl;
        return;
    }
    for (const auto& contatto : c) {
        file << contatto;
    }
    file.close();
    return;
}

optional<chrono::year_month_day> checkDataNascita (const string& riga) {
    regex pattern(R"(\d{2}/\d{2}/\d{4})");
    if (!regex_match(riga, pattern)) {
        return nullopt;
    }
    try {
        chrono::year_month_day data_nascita {
            chrono::year{stoi(riga.substr(6, 4))},
            chrono::month{static_cast<unsigned>(stoul(riga.substr(3, 2)))},
            chrono::day{static_cast<unsigned>(stoul(riga.substr(0, 2)))}
        };
        if (data_nascita.ok()) {
            return data_nascita;
        } else {
            return nullopt;
        }
    }
    catch (...) {
        return nullopt;
    }
}

optional<chrono::year_month_day> checkDataNascitaFile(const string& riga) {
    try {
        if (riga.size() != 10 ||
            riga[4] != '-' ||
            riga[7] != '-') {
            return nullopt;
            }
            chrono::year_month_day data{
                chrono::year{stoi(riga.substr(0, 4))},
                chrono::month{static_cast<unsigned>(stoi(riga.substr(5, 2)))},
                chrono::day{static_cast<unsigned>(stoi(riga.substr(8, 2)))}
            };

        if (data.ok()) {
            return data;
        }
        return nullopt;
    }
    catch (...) {
        return nullopt;
    }
}

optional<string> checkTelefono(const string& riga, const vector<structDatiContatto>& vectorDatiContatto) {
    const regex pattern("^[+]{1}(?:[0-9\\-\\(\\)\\/"
    "\\.]\\s?){6,15}[0-9]{1}$");
    if (riga.empty()) {
        return nullopt;
    }
    if (regex_match(riga, pattern)) {
        auto it = find_if(
            vectorDatiContatto.begin(),
            vectorDatiContatto.end(),
            [&riga](const structDatiContatto& c) {
                return (c.numero_tel == riga);
            }
        );
        if (it == vectorDatiContatto.end()) {
            return riga;
        }
        else {
            return nullopt;
        }
    }
    else {
        return nullopt;
    }
}

optional<string> checkEmail (const string& riga) {
    regex pattern(R"(?:[a-z0-9!#$%&'*+\x2f=?^_`\x7b-\x7d~\x2d]+(?:\.[a-z0-9!#$%&'*+\x2f=?^_`\x7b-\x7d~\x2d]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9\x2d]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9\x2d]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9\x2d]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])");
    if (regex_match(riga, pattern)) {
        return riga;
    }
    else {
        return nullopt;
    }
}

int prossimoID(const vector<structDatiContatto>& c) {
    int max_ID = 0;
    for (int i = 0; i < size(c); i++) {
        if (c[i].ID >= max_ID) {
            max_ID = c[i].ID;
        }
    }
    max_ID++;
    return max_ID;

}

string inputNome() {
    string nome;
    while (empty(nome)) {
        getline(cin >> ws, nome);
    }
    return nome;
}
string inputCognome() {
    string cognome;
    while (empty(cognome)) {
        getline(cin >> ws, cognome);
    }
    return cognome;
}
string inputTelefono(const vector<structDatiContatto>& vectorDatiContatto) {
    string numero_tel;
    while (true) {
        getline (cin >> ws, numero_tel);
        auto numero = checkTelefono(numero_tel, vectorDatiContatto);
        if (numero) {
            break;
        }
    }
    return numero_tel;
}
string inputEmail() {
    string email;
    while (true) {
        getline(cin >> ws, email);
        auto email_checked = checkEmail(email);
        if (email_checked) {
            break;
        }
    }
    return email;
}
string inputIndirizzo() {
    string indirizzo;
    while (empty(indirizzo)) {
        getline(cin >> ws, indirizzo);
    }
    return indirizzo;
}
chrono::year_month_day inputDataNascita() {
    string riga;
    chrono::year_month_day data_nascita;
    while (true) {
        getline(cin >> ws, riga);
        auto data = checkDataNascita(riga);
        if (data) {
            data_nascita = data.value();
            break;
        }
    }
    return data_nascita;
}
bool inputPreferiti() {
    string riga;
    while (true) {
        getline(cin >> ws, riga);
        if (riga == "Si") {
            return true;
        }
        if (riga == "No") {
            return false;
        }
    }
    return false;
}

structDatiContatto InputContatto (structDatiContatto c, const vector<structDatiContatto>& vectorDatiContatto, const array<bool,8>& b) {
    // if (b[0]) {
    //     cin >> c.ID; // Va modificato solo in casi particolari
    // }
    if (b[1]) {
        cout <<"Nome: " << endl;
        c.nome = inputNome();
    }
    if (b[2]) {
        cout <<"Cognome: " << endl;
        c.cognome = inputCognome();
    }
    if (b[3]) {
        cout <<"Numero di telefono: " << endl;
        c.numero_tel = inputTelefono(vectorDatiContatto);
    }
    if (b[4]) {
        cout <<"Email: " << endl;
        c.email = inputEmail();
    }
    if (b[5]) {
        cout <<"Indirizzo: " << endl;
        c.indirizzo = inputIndirizzo();
    }
    if (b[6]) {
        cout <<"Data di nascita: " << endl;
        c.data_nascita = inputDataNascita();
    }
    if (b[7]) {
        cout <<"Preferito, Si o No " << endl;
        c.preferiti = inputPreferiti();
    }
    return c;
}

void stampaContatto (const structDatiContatto& c) {
    cout <<"1) Nome: " << c.nome << endl;
    cout <<"2) Cognome: " << c.cognome << endl;
    cout <<"3) Numero di telefono: " << c.numero_tel << endl;
    cout <<"4) Email: " << c.email << endl;
    cout <<"5) Indirizzo: " << c.indirizzo << endl;
    cout <<"6) Data di nascita: " << c.data_nascita << endl;
    cout <<"7) Preferito: " << c.preferiti << endl;
    cout << endl;
    return;
}

// Funzione per l'inserimento di un contatto
// In questo caso con semplice lettura da tastiera
void AggiungiContatto (const string& fileContatti, vector<structDatiContatto>& vectorDatiContatto) {
    structDatiContatto c{};
    cout <<"Inserisci un dato per volta e premi invio" << endl;
    array<bool, 8> flags = {false, true, true, true, true, true, true, true};
    c = InputContatto(c, vectorDatiContatto, flags);
    c.ID = prossimoID(vectorDatiContatto);
    vectorDatiContatto.push_back(c);
    vector<structDatiContatto> tempVector;
    tempVector.push_back(c);
    scriviFile (tempVector, fileContatti, ios::app);
    return;
}

void ModificaContatto(const int ID, vector<structDatiContatto>& vectorDatiContatto, const string& fileContatti) {
    structDatiContatto c;
    bool flag = false;
    int j = 0;
    for (int i = 0; i < vectorDatiContatto.size(); i++) {
        if (vectorDatiContatto[i].ID == ID) {
            flag = true;
            j = i;
            c = vectorDatiContatto[i];
            cout <<"Contatto: " << ID << endl;
            stampaContatto(c);
        }
    }
    if (!flag) {
        cout <<"ID Inesistente" << endl;
        return;
    }
    int campo_da_modificare = 0;
    array<bool, 8> flags = {false, false, false, false, false, false, false, false};
    while (campo_da_modificare <= 0 || campo_da_modificare > size(flags)-1) {
        cout <<"Scegli quale campo modificare " << endl;
        cin >> campo_da_modificare;
    }

    cout <<"Scrivi la modifica: " << endl;
    flags[campo_da_modificare] = true;
    c = InputContatto(c, vectorDatiContatto, flags);
    vectorDatiContatto[j] = c;
    scriviFile (vectorDatiContatto, fileContatti, ios::trunc);
    cout <<"Il contatto è ora: " << endl;
    stampaContatto(c);
    return;
}

void EliminaContatto(int ID, vector<structDatiContatto>& c, const string& fileContatti) {
    int i;
    for (i = 0; i < size(c); i++) {
        if (c[i].ID == ID) {
            c.erase(c.begin() + i);
            break;
        }
    }
    scriviFile (c, fileContatti, ios::trunc);
    return;
}

string caseFoldUTF8(const string& riga) {
    icu::UnicodeString unicode = icu::UnicodeString::fromUTF8(riga);
    unicode.foldCase();
    string result;
    unicode.toUTF8String(result);
    return result;
}

auto CercaContatto(const string& riga, vector<structDatiContatto>& c) {
    // auto contattiTrovati = c | views::filter(
    //     [&riga](const structDatiContatto& contatto) {
    //
    //         string data = format("{:%d/%m/%Y}", contatto.data_nascita);
    //
    //         return contatto.nome.find(riga)       != string::npos ||
    //                contatto.cognome.find(riga)    != string::npos ||
    //                contatto.indirizzo.find(riga)  != string::npos ||
    //                contatto.numero_tel.find(riga) != string::npos ||
    //                contatto.email.find(riga)      != string::npos ||
    //                data.find(riga)                != string::npos;
    //     }
    // );
    string foldedRiga = caseFoldUTF8(riga);
    auto contattiTrovati = c | views::filter(
        [&riga, foldedRiga](const structDatiContatto& contatto) {

            string data = format("{:%d/%m/%Y}", contatto.data_nascita);

            return caseFoldUTF8(contatto.nome).find(foldedRiga)      != string::npos ||
                   caseFoldUTF8(contatto.cognome).find(foldedRiga)   != string::npos ||
                   caseFoldUTF8(contatto.indirizzo).find(foldedRiga) != string::npos ||
                   contatto.numero_tel.find(riga)                    != string::npos ||
                   caseFoldUTF8(contatto.email).find(foldedRiga)     != string::npos ||
                   data.find(riga)                                   != string::npos;
        }
    );
    return contattiTrovati;
}

void RiordinaContatti (int ordinamento, vector<structDatiContatto>& c, const string& fileContatti) {
    switch (ordinamento) {
        case 1: {
            sort(c.begin(), c.end(), [](const structDatiContatto& a, const structDatiContatto& b) {
                return a.nome < b.nome;
            }
            );
            break;
        }
        case 2: {
            sort(c.begin(), c.end(), [](const structDatiContatto& a, const structDatiContatto& b) {
                return a.nome > b.nome;
            }
            );
            break;
        }
        case 3: {
            sort(c.begin(), c.end(), [](const structDatiContatto& a, const structDatiContatto& b) {
                return a.cognome < b.cognome;
            }
            );
            break;
        }
        case 4: {
            sort(c.begin(), c.end(), [](const structDatiContatto& a, const structDatiContatto& b) {
                return a.cognome > b.cognome;
            }
            );
            break;
        }
        case 5: {
            sort(c.begin(), c.end(), [](const structDatiContatto& a, const structDatiContatto& b) {
                return a.ID < b.ID;
            }
            );
            break;
        }
        case 6: {
            sort(c.begin(), c.end(), [](const structDatiContatto& a, const structDatiContatto& b) {
                return a.ID > b.ID;
            }
            );
            break;
        }

    }
    scriviFile (c, fileContatti, ios::trunc);
    return;
}

expected<
    pair<vector<structDatiContatto>, int>,
    string
    >
inizializzaRubrica (const string& file_contatti, const string& file_config) {
    ifstream fileContatti(file_contatti); // Apre il file
    if (!fileContatti) {
        ofstream nuovoFile(file_contatti); // Se il file non esiste, lo crea
        if (!nuovoFile) {
            cerr << "Errore nella creazione di " << file_contatti << endl;
            return unexpected("Errore nella lettura del file"); // Se anche dopo la creazione comunque ci sono problemi, esci ritornando errore
        }
        nuovoFile.close();
        fileContatti.open(file_contatti); //Adesso la variabile fileContatti è correttamente associata
    }
    vector <structDatiContatto> vectorDatiContatto;

    string riga;
    while (getline(fileContatti, riga)) {
        structDatiContatto c;
        c.ID = stoi(riga);
        getline(fileContatti, c.nome);
        getline(fileContatti, c.cognome);
        getline(fileContatti, c.numero_tel);
        getline(fileContatti, c.email);
        getline(fileContatti, c.indirizzo);
        getline(fileContatti, riga);
        auto data = checkDataNascitaFile(riga);
        if (data) {
            c.data_nascita = data.value();
        }
        getline(fileContatti, riga);
        if (riga == "0") {
            c.preferiti = false;
        }
        if (riga == "1") {
            c.preferiti = true;
        }
        vectorDatiContatto.push_back(c);
    }
    fileContatti.close(); // Chiudo il file

    int flag_riordino = 0;
    ifstream config(file_config); // Apre il file
    if (!config) {
        ofstream nuovoFile(file_config); // Se il file non esiste, lo crea
        if (!nuovoFile) {
            cerr << "Errore nella creazione di " << file_config << endl;
            return unexpected("Errore nella lettura del file"); // Se anche dopo la creazione comunque ci sono problemi, esci ritornando l'errore'
        }
        nuovoFile << 0;
        nuovoFile.close();
        config.open(file_config); //Adesso la variabile config è correttamente associata
    }
    getline(config, riga);
    flag_riordino = stoi(riga);
    config.close();

    return pair{vectorDatiContatto, flag_riordino};
}

int main() {
    auto risultato = inizializzaRubrica ("filecontatti.txt", "config.txt");
    if (!risultato) {
        cerr << "Errore: " << risultato.error() << endl;
        return 1;
    }
    auto [vectorDatiContatto, flag_riordino] = *risultato;

    while (true) {
        cout <<
        "Scrivi il numero e premi Invio" << endl <<
        "1) Aggiungi contatto" << endl <<
        "2) Modifica contatto" << endl <<
        "3) Elimina contatto"  << endl <<
        "4) Cerca contatto"  << endl <<
        "5) Mostra tutti i contatti"  << endl <<
        "6) Ordina contatti"  << endl <<
        "7) Preferiti" << endl <<
        "8) Carica rubrica"  << endl <<
        //"9) Salva rubrica"  << endl << // Inutile: ormai ogni rubrica salva sempre su file ogni modifica
        "0) Esci"  << endl;
        string riga;
        int scelta;
        regex pattern(R"(^[0-9]$)");
        getline(cin >> ws, riga);
        if (regex_match(riga, pattern)) {
            scelta = stoul(riga);
            riga = "";
        }
        else {
            continue;
        }
        switch(scelta) {
            case 1: {
                AggiungiContatto("filecontatti.txt", vectorDatiContatto);
                break;
            }
            case 2: {
                regex patternID(R"(^[1-9][0-9]*$)");
                unsigned long ID_to_change = 0;
                while (ID_to_change == 0) {
                    cout <<"Scrivi il numero dell'ID del contatto da modificare" << endl;
                    getline(cin >> ws, riga);
                    if (regex_match(riga, patternID)) {
                        ID_to_change = stoul(riga);
                        break;
                    }
                }
                ModificaContatto(ID_to_change, vectorDatiContatto, "filecontatti.txt");
                break;
            }
            case 3: {
                regex patternID(R"(^[1-9][0-9]*$)");
                unsigned long ID_to_remove = 0;
                while (ID_to_remove == 0) {
                    cout <<"Scrivi l'ID del contatto da eliminare" << endl;
                    getline(cin >> ws, riga);
                    if (regex_match(riga, patternID)) {
                        ID_to_remove = stoul(riga);
                        break;
                    }
                }
                EliminaContatto(ID_to_remove, vectorDatiContatto, "filecontatti.txt");
                break;
            }
            case 4: {
                cout <<"Inserisci il termine di ricerca " << endl;
                getline(cin >> ws, riga);
                auto c = CercaContatto(riga, vectorDatiContatto);
                cout<<"I contatti trovati sono: " << endl;
                for (const auto& contatto : c) {
                    stampaContatto(contatto);
                }
                break;
            }
            case 5: {
                for (int i = 0; i < size(vectorDatiContatto); i++) {
                    stampaContatto (vectorDatiContatto[i]);
                }
                break;
            }
            case 6: {
                unsigned long a = 0;
                while (true) {
                    cout <<"Scegli come riordinare la rubrica" << endl
                    << "1) Nome A-Z" << endl
                    << "2) Nome Z-A" << endl
                    << "3) Cognome A-Z" << endl
                    << "4) Cognome Z-A" << endl
                    << "5) ID crescente" << endl
                    << "6) ID decrescente" << endl;
                    getline(cin >> ws, riga);
                    regex pattern(R"(^\d+$)");
                    if (regex_match(riga, pattern)) {
                        a = stoul(riga);
                        if (a >0 && a < 7) {
                           break;
                        }
                    }
                }
                     RiordinaContatti(a, vectorDatiContatto, "filecontatti.txt");
                break;
            }
            case 7: {
                bool flag = true;
                cout << "I preferiti sono: " << endl;
                for (const auto& contatto : vectorDatiContatto) {
                    if (contatto.preferiti) {
                        flag = false;
                        stampaContatto(contatto);
                    }
                }
                if (flag) {
                    cout << "Nessun contatto preferito " << endl;
                }
                break;
            }

            case 8: {
                cout << "Scrivi il nome del file contenente i contatti della rubrica";
                getline(cin >> ws, riga);
                auto risultato = inizializzaRubrica (riga, "config.txt");
                if (!risultato) {
                    cerr << "Errore: " << risultato.error() << endl;
                    return 1;
                }
                else {
                    vectorDatiContatto = move(risultato->first);
                    flag_riordino = risultato->second;

                }

                break;
            }
            // case 9:
            //     break;
            case 0:
                return 0;
        }
    }

    return 0;
}
