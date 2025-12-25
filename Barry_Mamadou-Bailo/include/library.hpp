/**
 * @file library.cpp
 * @brief Gestion des données de la bibliothèque (Livres, Import/Export).
 * @author Barry Mamadou Bailo
 * @version 1.0
 *
 * Contient les fonctions de manipulation des livres : ajout, suppression,
 * vérification d'ISBN, importation CSV et l'exportation du catalogue en HTML.
 * Gère également la persistance des données via library.db.
 */

#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <string>
#include <vector>
#include "book.hpp" // On a besoin de connaître la structure Book

#include <string>
#include <vector>
#include "book.hpp" // On a besoin de connaître la structure Book

struct Library {
    std::string name;           // Le nom de la bibliothèque
    std::string description;    // La description de la bibliothèque
    std::vector<Book> books;    // La liste dynamique de tous les livres (CM5)
};

// Fonctions de gestion de la bibliothèque
// Charge les données depuis le fichier DB
bool chargerBibliotheque(Library& lib, const std::string& filename);

// Sauvegarde les données dans le fichier DB
void sauvegarderBibliotheque(const Library& lib, const std::string& filename);

// Initialise une nouvelle bibliothèque (demande à l'utilisateur)
void initialiserBibliotheque(Library& lib);

// Vérifie si un ISBN existe déjà dans la bibliothèque
bool isbnExiste(const Library& lib, const std::string& isbn);

// Ajoute un livre à la bibliothèque
void ajouterLivre(Library& lib, const Book& nouveauLivre);

// Supprime tous les livres de la bibliothèque
void supprimerToutesReferences(Library& lib);

// Importe des livres depuis un fichier CSV
// Retourne le nombre de livres importés
int importerReferences(Library& lib, const std::string& filename);

// Exporte la bibliothèque dans un fichier HTML avec tri et index
void exporterHTML(const Library& lib, const std::string& filename);

#endif