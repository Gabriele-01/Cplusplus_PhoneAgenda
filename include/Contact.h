#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <optional>
#include <chrono>

// Definisco la struct di un contatto della rubrica
struct Contact {
    std::optional<int> ID; // È opzionale nel caso ad esempio di nuovi contatti ancora senza ID
    std::string firstName;
    std::string lastName;
    std::string phoneNumber;
    std::string email;
    std::string address;
    std::optional<std::chrono::year_month_day> birthday; // Ricordarsi che qua si usa il formato chrono. È opzionale: non tutti i contatti hanno una data di nascita e nel db può essere NULL.
    bool isFavorite = false;
};

#endif
