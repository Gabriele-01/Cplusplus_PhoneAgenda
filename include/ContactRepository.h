// Interfaccia astratta per permettere l'utilizzo di diversi backend di storage: SQLite, JSON e testuale
class ContactRepository {
public:
    virtual ~ContactRepository() = default;
    virtual int addContact(const Contact& c) = 0;
    virtual std::vector<Contact> getAllContacts() const = 0;
    virtual std::vector<Contact> searchContacts(const std::string& term) const = 0;
    virtual bool updateContact(const Contact& c) = 0;
    virtual bool deleteContact(int ID) = 0;
    virtual bool setFavorite(int ID, bool isFavorite) = 0;
};
