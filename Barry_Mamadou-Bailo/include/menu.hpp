/**
 * @file menu.cpp
 * @brief Interface Utilisateur (Console) et navigation.
 * @author Barry Mamadou Bailo
 * @version 1.0
 *
 * Ce module gère toutes les interactions avec l'utilisateur :
 * - Affichage des menus et sous-menus.
 * - Saisie sécurisée des informations (formulaires).
 * - Affichage des listes paginées (consultation, recherche).
 * - Gestion des paramètres visuels.
 */

#ifndef MENU_HPP
#define MENU_HPP

#include "library.hpp" // On a besoin de connaître le type Library
#include "config.hpp"  // On a besoin de connaître le type AppConfig

// Affiche le menu principal
void afficherMenuPrincipal(const AppConfig& config);

// Affiche la liste paginée (Le cœur du design)
// On ajoute 'const Library& lib' pour afficher le titre/description en haut
void afficherListePaginee(const Library& lib, const std::vector<Book>& livresAAfficher, std::string titreMenu, const AppConfig& config);

// Affiche les détails d'un livre (Fiche)
void afficherDetailsLivre(const Book& livre, const AppConfig& config);

// Fonctions de menus
void consulterReferences(const Library& lib, const AppConfig& config);
void chercherReferences(const Library& lib, const AppConfig& config);
void gererReferences(Library& lib, const AppConfig& config, bool& aDesModifs);
void gererParametres(Library& lib, AppConfig& config, bool& aDesModifs);

#endif