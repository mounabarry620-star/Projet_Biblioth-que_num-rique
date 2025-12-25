/**
 * @file library.hpp
 * @brief Définition de la structure Library et des fonctions de gestion.
 * @author Barry Mamadou Bailo
 * @version 1.0
 *
 * Ce fichier contient la définition de la bibliothèque (qui contient une liste de livres)
 * et les prototypes des fonctions pour manipuler ces données (ajout, import, export...).
 */

#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <string>
#include <vector>
#include "book.hpp" // Nécessaire car la structure Library utilise la structure Book

// Structure principale représentant la bibliothèque
struct Library {
    std::string name;               // Le nom de la bibliothèque (ex: "Ma Biblio Perso")
    std::string description;        // Une description affichée dans le menu
    
    // J'utilise std::vector car c'est un tableau dynamique qui gère la mémoire automatiquement.
    // Cela permet d'ajouter autant de livres que l'on veut sans connaître la taille à l'avance.
    std::vector<Book> books;        
};

// --- FONCTIONS DE GESTION DES FICHIERS ---

// Charge les données depuis le fichier DB au démarrage.
// Retourne 'true' si le fichier a été trouvé et chargé, 'false' sinon.
bool chargerBibliotheque(Library& lib, const std::string& filename);

// Sauvegarde les données actuelles dans le fichier DB.
// Le paramètre 'lib' est 'const' pour garantir qu'on ne modifie pas les données pendant la sauvegarde.
void sauvegarderBibliotheque(const Library& lib, const std::string& filename);

// Initialise une nouvelle bibliothèque avec des valeurs par défaut si aucun fichier n'existe.
void initialiserBibliotheque(Library& lib);

// --- FONCTIONS DE MANIPULATION DES LIVRES ---

// Vérifie si un ISBN existe déjà dans la liste pour éviter les doublons.
// Retourne true si trouvé.
bool isbnExiste(const Library& lib, const std::string& isbn);

// Ajoute un livre à la fin du vecteur 'books'.
void ajouterLivre(Library& lib, const Book& nouveauLivre);

// Vide le vecteur de livres (suppression totale).
void supprimerToutesReferences(Library& lib);

// Importe des livres depuis un fichier CSV externe.
// Retourne le nombre entier de livres ajoutés avec succès.
int importerReferences(Library& lib, const std::string& filename);

// Génère une page Web (HTML) listant tous les livres, triés par titre.
void exporterHTML(const Library& lib, const std::string& filename);

#endif // LIBRARY_HPP