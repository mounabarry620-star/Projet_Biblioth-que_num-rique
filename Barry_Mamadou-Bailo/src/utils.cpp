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
#include <ctime>    // Pour les fonctions de temps (si besoin)
#include <string>
#include "utils.hpp"
#include <cctype>   // Pour std::tolower et isdigit

// Fonction pour nettoyer l'écran.
// J'utilise des directives de préprocesseur (#ifdef) pour que le code
// fonctionne aussi bien sur Windows que sur Linux/Mac.
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // Commande Windows
    #else
        // Sur Linux/Mac, on utilise les codes ANSI :
        // \033[2J : Efface tout l'écran
        // \033[1;1H : Replace le curseur en haut à gauche
        std::cout << "\033[2J\033[1;1H";
    #endif
}

// Affiche du texte en couleur en utilisant un code numérique (ex: 31 pour rouge)
void printColor(const std::string& text, int colorCode) {
    // Construction du code ANSI : \033[ + code + m
    std::cout << "\033[" << colorCode << "m" << text << "\033[0m" << std::endl;
}

// Convertit une chaîne en minuscules pour faciliter les comparaisons
// (ex: "Paris" devient "paris", utile pour la recherche insensible à la casse)
std::string toLower(const std::string& str) {
    std::string result = str;
    // On parcourt chaque caractère par référence (&) pour le modifier directement
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

// Fonction robuste de validation de date (Format JJ/MM/AAAA)
bool estDateValide(const std::string& date) {
    // 1. Vérification de la longueur stricte (10 caractères)
    if (date.length() != 10) return false;

    // 2. Vérification que les séparateurs '/' sont au bon endroit
    if (date[2] != '/' || date[5] != '/') return false;

    // 3. Extraction et conversion des nombres
    // J'utilise un bloc try-catch car std::stoi peut planter si ce n'est pas un nombre
    try {
        std::string jourStr = date.substr(0, 2);
        std::string moisStr = date.substr(3, 2);
        std::string anneeStr = date.substr(6, 4);

        // Vérifie caractère par caractère que ce sont bien des chiffres
        for (char c : jourStr) if (!isdigit(c)) return false;
        for (char c : moisStr) if (!isdigit(c)) return false;
        for (char c : anneeStr) if (!isdigit(c)) return false;

        // Conversion en entiers pour les tests logiques
        int jour = std::stoi(jourStr);
        int mois = std::stoi(moisStr);
        int annee = std::stoi(anneeStr);

        // 4. Vérifications logiques
        if (annee < 1000 || annee > 3000) return false; // On limite aux années réalistes
        if (mois < 1 || mois > 12) return false;

        // Tableau du nombre de jours par mois (index 0 ignoré pour correspondre aux mois 1-12)
        int joursDansMois[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        // Gestion des années bissextiles pour Février :
        // Une année est bissextile si divisible par 4 ET (pas par 100 OU par 400)
        if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0)) {
            joursDansMois[2] = 29;
        }

        // Vérification finale du jour par rapport au mois
        if (jour < 1 || jour > joursDansMois[mois]) return false;

        return true;

    } catch (...) {
        return false; // En cas d'erreur de conversion imprévue
    }
}

// Affiche l'en-tête commun à tous les menus
void afficherHeader(const std::string& titre, const AppConfig& config) {
    clearScreen(); // On commence toujours par nettoyer l'écran
    
    // 1. On affiche le Logo personnalisé (si présent)
    if (!config.logo.empty()) {
        std::cout  << config.logo << RESET << std::endl;
    }

    // 2. On affiche le Titre de la section dans un bandeau Bleu
    // Les espaces servent à centrer/aérer le texte
    std::cout << "  " << BG_BLUE << WHITE << BOLD << "      " << titre << "      " << RESET << "\n" << std::endl;
}

// Surcharge de la fonction printColor pour utiliser directement nos constantes string (ex: RED)
void printColor(const std::string& text, const std::string& colorCode) {
    std::cout << colorCode << text << RESET << std::endl;
}

// Fonction utilitaire pour générer une ligne de séparation (ex: "------")
std::string repeat(std::string c, int n) {
    std::string res = "";
    for (int i = 0; i < n; i++) res += c;
    return res;
}
