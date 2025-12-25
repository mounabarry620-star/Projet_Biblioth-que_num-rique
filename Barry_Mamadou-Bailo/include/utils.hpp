/**
 * @file utils.hpp
 * @brief Fonctions utilitaires transversales (Couleurs, Nettoyage, Validation).
 * @author Barry Mamadou Bailo
 * @version 1.0
 *
 * Bibliothèque de fonctions d'aide utilisées partout dans le projet :
 * - Gestion des couleurs dans le terminal (Codes ANSI).
 * - Nettoyage de l'écran.
 * - Validation de format (Dates).
 * - Manipulation de chaînes (minuscules pour la recherche).
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include "config.hpp" // Nécessaire pour afficher le logo stocké dans la config

// --- PALETTE DE COULEURS (ANSI Escape Codes) ---
// Ces codes permettent de changer la couleur du texte dans le terminal Linux.
const std::string RESET   = "\033[0m";  // Remet la couleur par défaut
const std::string BOLD    = "\033[1m";  // Texte en gras
const std::string RED     = "\033[31m"; // Rouge (Erreurs)
const std::string GREEN   = "\033[32m"; // Vert (Succès)
const std::string YELLOW  = "\033[33m"; // Jaune (Avertissements / Titres)
const std::string BLUE    = "\033[34m"; // Bleu (Information)
const std::string MAGENTA = "\033[35m"; // Magenta (Catégories)
const std::string CYAN    = "\033[36m"; // Cyan (Menus)
const std::string WHITE   = "\033[37m"; // Blanc
const std::string ITALIC  = "\033[3m";  // Italique

// Couleurs de fond (Background)
const std::string BG_BLUE = "\033[44m";

// --- FONCTIONS D'AFFICHAGE ---

// Fonction pour nettoyer le terminal (simule la commande "clear" ou "cls").
// Utile pour afficher des menus propres.
void clearScreen();

// Surcharge 1 : Affiche un texte en couleur avec un code entier (ex: 31 pour rouge).
void printColor(const std::string& text, int colorCode);

// Surcharge 2 : Affiche un texte en couleur avec nos constantes (ex: RED).
// Le C++ permet d'avoir deux fonctions avec le même nom si les paramètres changent.
void printColor(const std::string& text, const std::string& colorCode);

// Affiche l'en-tête standard en haut de chaque menu (Logo + Titre encadré).
// On passe 'config' pour récupérer le logo personnalisé.
void afficherHeader(const std::string& titre, const AppConfig& config);

// --- FONCTIONS DE TRAITEMENT DE TEXTE ---

// Convertit une chaîne en minuscules.
// Indispensable pour la recherche insensible à la casse (ex: "Paris" == "paris").
std::string toLower(const std::string& str);

// Vérifie si une date respecte strictement le format JJ/MM/AAAA.
// Vérifie aussi la cohérence (pas de 32 janvier ou 30 février).
bool estDateValide(const std::string& date);

// Fonction utilitaire pour répéter un caractère N fois.
// Utile pour créer des lignes de séparation (ex: "----------").
std::string repeat(std::string c, int n);

#endif // UTILS_HPP