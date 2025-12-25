/**
 * @file book.hpp
 * @brief Définition de la structure représentant un Livre.
 */

#ifndef BOOK_HPP
#define BOOK_HPP

#include <string> // Nécessaire pour utiliser std::string
#include <vector> // Inclus pour une évolution future (ex: liste de mots-clés)

// Structure représentant un livre.
// J'utilise une 'struct' plutôt qu'une 'class' car c'est un simple conteneur de données
// publiques, sans méthodes complexes associées directement.
struct Book {
    std::string isbn;           // Identifiant unique (ISBN-13). Ex: 978-2-...
    std::string title;          // Titre du livre
    std::string language;       // Langue du livre (ex: Français, Anglais)
    
    // Pour les auteurs, j'ai choisi une simple chaîne de caractères (std::string)
    // plutôt qu'un vecteur pour simplifier la saisie et l'affichage dans la console.
    std::string authors;        
    
    std::string date;           // Date de parution (Format texte pour simplifier)
    std::string genre;          // Genre littéraire (Roman, SF, etc.)
    std::string description;    // Résumé du livre (peut contenir des sauts de ligne)
};

#endif 