// Interfaccia per le funzioni per gestire il database

#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include <string>
#include "Contact.h"

class SqliteRepository : public ContactRepository {
public:
    explicit SqliteRepository(const std::string& dbPath);

    int addContact(const Contact& contact) override;
    std::vector<Contact> getAllContacts() const override;
    std::vector<Contact> searchContacts(const std::string& query) const override;
    bool updateContact(const Contact& contact) override;
    bool deleteContact(int ID) override;
    bool setFavorite(int ID, bool isFavorite) override;

private:
    SQLite::Database db_;
    void createTable();
};

#endif
