#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>
#include <vector>

// Structure représentant un livre
struct Book {
    std::string isbn;           // ISBN-13 (ex: 978-2-...)
    std::string title;          // Titre du livre
    std::string language;       // Langue (ex: Français)
    std::string authors;        // On stockera les auteurs dans une seule chaîne pour faire simple au début, ou un vecteur si tu préfères gérer auteur par auteur.
    std::string date;           // Date de parution (AAAA-MM-JJ pour le tri facile)
    std::string genre;          // Genre littéraire
    std::string description;    // Résumé (peut être long)
};

#endif