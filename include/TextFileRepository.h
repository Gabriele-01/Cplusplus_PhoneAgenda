// Interfaccia per le funzioni per gestire il salvataggio su file testuale

#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <vector>
#include <string>
#include "Contact.h"

class TextFileRepository : public ContactRepository {
public:
    explicit TextFileRepository(const std::string& filePath);

    int addContact(const Contact& contact) override;
    std::vector<Contact> getAllContacts() const override;
    std::vector<Contact> searchContacts(const std::string& query) const override;
    bool updateContact(const Contact& contact) override;
    bool deleteContact(int ID) override;
    bool setFavorite(int ID, bool isFavorite) override;

private:
    std::string filePath_;
    std::vector<Contact> contacts_;   // Copia in memoria
    void load();
    void save() const;
};

#endif
