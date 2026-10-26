// Questo file implementa le funzioni specificate nel file include/SqliteRepository.h

#include "SqliteRepository.h"
#include <iostream>
#include <format>
#include <chrono>
#include <stdexcept>

using namespace std;
/* Le prime 3 funzioni sono funzioni di supporto:
    dateToString()
    stringToDate()
    rowToContact()
*/
// Funzione di conversione da data in formato chrono a stringa
static string dateToString(const chrono::year_month_day& d) {
    return format("{:%Y-%m-%d}", d);
}
// Funzione di conversione da data in formato stringa a formato chrono
static optional<chrono::year_month_day> stringToDate(const string& s) {
    if (s.size() !=10 || s[4] != '-' || s[7] != '-') return nullopt;
    try {
        chrono::year_month_day d {
            chrono::year{stoi(s.substr(0, 4))},
            chrono::month{static_cast <unsigned>(stoi(s.substr(5, 2)))},
            chrono::day{static_cast<unsigned>(stoi(s.substr(8, 2)))}
        };
        if (d.ok()) return d; // Se è tutto ok, ritorna d
    } catch (...) {}
    return nullopt; // Altrimenti ritorna nullopt
}
// Funzione per convertire l'output di una query al db in un oggetto Contact
static Contact rowToContact(SQLite::Statement& query) {
    Contact c;
    c.ID            = query.getColumn(0).getInt();
    c.firstName     = query.getColumn(1).getString();
    c.lastName      = query.getColumn(2).getString();
    c.phoneNumber   = query.getColumn(3).getString();
    c.email         = query.getColumn(4).getString();
    c.address       = query.getColumn(5).getString();
    if (!query.getColumn(6).isNull())
        c.birthday = stringToDate(query.getColumn(6).getString());
    c.isFavorite    = query.getColumn(7).getInt() != 0;
    return c;
}

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : db_(dbPath, SQLite::OPEN_READWRITE | SQLite:: OPEN_CREATE) {
        createTable();
    }
/* Funzione per creare la table, necessaria quando si usa il programma la prima volta
   L'id del contatto viene generato automaticamente
   SQLite non supporta il formato DATE quindi birthday è TEXT in formato YYYY-MM-DD
*/
void DatabaseManager::createTable() {
    const string sql = R"(
        CREATE TABLE IF NOT EXISTS contacts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            firstName TEXT NOT NULL,
            lastName TEXT NOT NULL,
            phoneNumber TEXT NOT NULL UNIQUE,
            email TEXT UNIQUE,
            address TEXT,
            birthday TEXT,
            isFavorite INTEGER NOT NULL DEFAULT 0
        );
    )";
    db_.exec(sql);
}

// Funzione per aggiungere un contatto al database
// Prende in ingresso un elemendo della struct Contact
int DatabaseManager::addContact(const Contact& contact) {
    SQLite::Statement query(db_, "INSERT INTO contacts (firstName, lastName, phoneNumber, email, address, birthday, isFavorite) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.bind(1, contact.firstName);
    query.bind(2, contact.lastName);
    query.bind(3, contact.phoneNumber);
    if (contact.email.empty()) query.bind(4);
        else query.bind(4, contact.email);
    query.bind(5, contact.address);
    if (contact.birthday)query.bind(6, dateToString(*contact.birthday));
        else query.bind(6);
    query.bind(7, contact.isFavorite ? 1 : 0);

    query.exec();
    return static_cast<int>(db_.getLastInsertRowid()); // Ritorno l'ID che ho assegnato al nuovo contatto nel db
}

vector<Contact> DatabaseManager::getAllContacts() const {
    vector<Contact> contacts;
    SQLite::Statement query(db_, "SELECT id, firstName, lastName, phoneNumber, email, address, birthday, isFavorite FROM contacts ORDER BY lastName, firstName");

    while(query.executeStep()) {
        contacts.push_back(rowToContact(query));
    }
    return contacts;
}

vector<Contact> DatabaseManager::searchContacts(const string& searchTerm) const {
    vector<Contact> contacts;
    SQLite::Statement query(db_,
                            "SELECT id, firstName, lastName, phoneNumber, email, address, birthday, isFavorite FROM contacts "
                            "WHERE firstName LIKE ? OR lastName LIKE ? OR phoneNumber LIKE ? "
                            "ORDER BY lastName, firstName");
    string pattern = "%" + searchTerm + "%";
    query.bind(1, pattern);
    query.bind(2, pattern);
    query.bind(3, pattern);

    while (query.executeStep()) {
        contacts.push_back(rowToContact(query));
    }
    return contacts;
}

bool DatabaseManager::setFavorite(int ID, bool isFavorite) {
    SQLite::Statement query(db_, "UPDATE contacts SET isFavorite = ? WHERE id = ?");
    query.bind(1, isFavorite ? 1 : 0);
    query.bind(2, ID);
    return query.exec() > 0;
}

bool DatabaseManager::updateContact(const Contact& contact) {
    // Verifico che il contatto che si vuole aggiornare abbia un ID valido prima di fare l'UPDATE
    if (!contact.ID.has_value()) {
        throw invalid_argument("Impossibile aggiornare un contatto senza ID.");
    }
    SQLite::Statement query(db_,
                            "UPDATE contacts "
                            "SET firstName = ?, lastName = ?, phoneNumber = ?, email = ?, address = ?, birthday = ?, isFavorite = ? "
                            "WHERE id = ?");
    // Assegno i nuovi valori ai campi
    query.bind(1, contact.firstName);
    query.bind(2, contact.lastName);
    query.bind(3, contact.phoneNumber);
    if (contact.email.empty()) query.bind(4);
        else query.bind(4, contact.email);
    query.bind(5, contact.address);

    // Gestisco la data che è un valore opzionale
    if (contact.birthday)query.bind(6, dateToString(*contact.birthday));
        else query.bind(6);

    query.bind(7, contact.isFavorite);
    // Infine assegno l'ID al campo del WHERE
    query.bind(8, contact.ID.value());

    // query.exec() ritorna il numero di righe modificate
    return query.exec() > 0;
}

bool DatabaseManager::deleteContact(int ID) {
    SQLite::Statement query(db_, "DELETE FROM contacts WHERE id = ?");
    query.bind(1, ID);
    return query.exec() > 0;
}
