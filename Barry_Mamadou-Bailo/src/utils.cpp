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

#include <iostream>
#include <ctime>     // pour date
#include <string>
#include "utils.hpp"
#include <cctype> // pour tolower



// Fonction pour nettoyer l'écran (Windows ou Linux/Mac)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        std::cout << "\033[2J\033[1;1H";
    #endif
}

void printColor(const std::string& text, int colorCode) {
    // \033[ est le début du code, suivi du numéro de couleur, puis m
    // \033[0m permet de remettre la couleur par défaut à la fin
    std::cout << "\033[" << colorCode << "m" << text << "\033[0m" << std::endl;
}

// Convertit une chaîne en minuscules
std::string toLower(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}


bool estDateValide(const std::string& date) {
    // 1. Vérification de la longueur (10 caractères : JJ/MM/AAAA)
    if (date.length() != 10) return false;

    // 2. Vérification des séparateurs '/'
    if (date[2] != '/' || date[5] != '/') return false;

    // 3. Extraction des nombres
    try {
        std::string jourStr = date.substr(0, 2);
        std::string moisStr = date.substr(3, 2);
        std::string anneeStr = date.substr(6, 4);

        // Vérifie que ce sont bien des chiffres
        for (char c : jourStr) if (!isdigit(c)) return false;
        for (char c : moisStr) if (!isdigit(c)) return false;
        for (char c : anneeStr) if (!isdigit(c)) return false;

        int jour = std::stoi(jourStr);
        int mois = std::stoi(moisStr);
        int annee = std::stoi(anneeStr);

        // 4. Vérifications logiques
        if (annee < 1000 || annee > 3000) return false; // Année raisonnable
        if (mois < 1 || mois > 12) return false;

        // Nombre de jours par mois
        int joursDansMois[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        // Gestion année bissextile pour Février (optionnel mais propre)
        if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0)) {
            joursDansMois[2] = 29;
        }

        if (jour < 1 || jour > joursDansMois[mois]) return false;

        return true;

    } catch (...) {
        return false; // Erreur de conversion
    }
}

void afficherHeader(const std::string& titre, const AppConfig& config) {
    clearScreen();
    
    // 1. On affiche le Logo (en CYAN pour le style)
    if (!config.logo.empty()) {
        std::cout  << config.logo << RESET << std::endl;
    }

    // 2. On affiche le Titre de la section (Bandeau Bleu)
    std::cout << "  " << BG_BLUE << WHITE << BOLD << "      " << titre << "      " << RESET << "\n" << std::endl;
}


// Nouvelle (avec nos constantes string, ex: RED)
void printColor(const std::string& text, const std::string& colorCode) {
    std::cout << colorCode << text << RESET << std::endl;
}

std::string repeat(std::string c, int n) {
    std::string res = "";
    for (int i = 0; i < n; i++) res += c;
    return res;
}
