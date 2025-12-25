/**
 * @file utils.cpp
 * @brief Fonctions utilitaires transversales.
 * @author Barry Mamadou Bailo
 * @version 1.0
 *
 * Bibliothèque de fonctions d'aide :
 * - Gestion des couleurs dans le terminal (ANSI).
 * - Nettoyage de l'écran (cross-platform).
 * - Validation de format (Dates).
 * - Manipulation de chaînes (minuscules, nettoyage).
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include "config.hpp"

// --- PALETTE DE COULEURS (ANSI) ---
const std::string RESET   = "\033[0m";
const std::string BOLD    = "\033[1m";
const std::string RED     = "\033[31m";
const std::string GREEN   = "\033[32m";
const std::string YELLOW  = "\033[33m";
const std::string BLUE    = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN    = "\033[36m";
const std::string WHITE   = "\033[37m";
const std::string ITALIC  = "\033[3m";

// Couleurs de fond (Background)
const std::string BG_BLUE = "\033[44m";

// Fonction pour nettoyer le terminal (comme "clear")
void clearScreen();

// Fonction pour afficher un texte en couleur avec un retour à la ligne
// codes couleurs basés sur ton fichier texte : 31=rouge, 32=vert, 34=bleu, 0=reset
void printColor(const std::string& text, int colorCode);
// Fonction pour afficher un texte en couleur avec un retour à la ligne
void printColor(const std::string& text, const std::string& colorCode);

// MODIFICATION : On ajoute 'config' pour pouvoir afficher le logo
void afficherHeader(const std::string& titre, const AppConfig& config);

// Convertit une chaîne en minuscules
std::string toLower(const std::string& str);

// Vérifie si une date est au format JJ/MM/AAAA et valide (ex: pas 30/02)
bool estDateValide(const std::string& date);

// Répète un caractère n fois (ex: repeat("-", 10) -> "----------")
std::string repeat(std::string c, int n);

#endif