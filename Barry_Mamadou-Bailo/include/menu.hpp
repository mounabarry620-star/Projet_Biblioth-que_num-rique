/**
 * @file menu.hpp
 * @brief Prototypes des fonctions d'interface utilisateur (Console).
 * @author Barry Mamadou Bailo
 * @version 1.0
 *
 * Ce fichier déclare toutes les fonctions qui gèrent l'affichage à l'écran
 * et la navigation entre les différents menus (Principal, Gestion, Recherche...).
 */

#ifndef MENU_HPP
#define MENU_HPP

#include "library.hpp" // Nécessaire pour manipuler la structure Library et Book
#include "config.hpp"  // Nécessaire pour accéder aux paramètres (couleurs, pagination)

// Affiche le menu principal (logo + choix 1 à 6).
// On passe 'config' en référence constante (const &) pour éviter de copier la structure
// inutilement en mémoire, tout en garantissant qu'on ne la modifie pas ici.
void afficherMenuPrincipal(const AppConfig& config);

// Fonction générique pour afficher n'importe quelle liste de livres page par page.
// Elle est utilisée aussi bien pour "Consulter" (tous les livres) que pour "Rechercher" (résultats filtrés).
// - lib : sert à afficher le nom de la bibliothèque en haut.
// - livresAAfficher : le vecteur contenant les livres à montrer.
// - titreMenu : le titre à afficher en haut (ex: "RÉSULTATS DE RECHERCHE").
void afficherListePaginee(const Library& lib, const std::vector<Book>& livresAAfficher, std::string titreMenu, const AppConfig& config);

// Affiche la fiche détaillée d'un livre spécifique (toutes les infos).
void afficherDetailsLivre(const Book& livre, const AppConfig& config);

// --- FONCTIONS DE NAVIGATION (Sous-menus) ---

// Gère l'option 1 : Affiche tous les livres de la bibliothèque.
void consulterReferences(const Library& lib, const AppConfig& config);

// Gère l'option 3 : Menu de recherche (par titre, ISBN...).
void chercherReferences(const Library& lib, const AppConfig& config);

// Gère l'option 2 : Menu d'ajout, d'importation et de suppression.
// Le booléen 'aDesModifs' est passé par référence (&) modifiable.
// Cela permet au menu de signaler au main() si une modification a eu lieu (pour la sauvegarde).
void gererReferences(Library& lib, const AppConfig& config, bool& aDesModifs);

// Gère l'option 5 : Modification des paramètres (Titre, Logo, Pagination).
// Ici, 'config' n'est PAS const car on veut justement pouvoir le modifier.
void gererParametres(Library& lib, AppConfig& config, bool& aDesModifs);

#endif // MENU_HPP