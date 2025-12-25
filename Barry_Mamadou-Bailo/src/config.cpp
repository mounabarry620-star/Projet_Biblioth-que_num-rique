/**
 * @file config.cpp
 * @brief Gestion de la configuration persistante de l'application.
 * @author Barry Mamadou Bailo
 * @version 1.0
 *
 * Gère le chargement et la sauvegarde du fichier 'app.conf', incluant
 * le logo ASCII personnalisé et les préférences d'affichage (pagination).
 */

#include <fstream>
#include <iostream>
#include "config.hpp"
#include "utils.hpp" // Pour les couleurs et clearScreen
    
void reinitialiserLogo(AppConfig& config) {
    // Utilisation d'une "Raw String" R"(...)" pour ne pas s'embêter avec les \ et "
   config.logo = "";

    // --- HAUT (ROUGE) --- 
    // Alignement précis du haut
    config.logo += RED    + "       __...--~~~~~-._   _.-~~~~~--...__" + RESET + "\n";
    config.logo += RED    + "     //               `V'               \\\\" + RESET + "\n";

    // --- MILIEU (JAUNE) ---
    // On décale progressivement vers la gauche pour élargir le livre
    config.logo += YELLOW + "    //                 |                 \\\\" + RESET + "\n";
    config.logo += YELLOW + "   //__      BIBLIOTHEQUE NUMERIQUE    __ \\\\" + RESET + "\n";

    // --- BAS (VERT) ---
    // Attention aux échappements : \\ affiche \ à l'écran
    config.logo += GREEN  + "  //___\\____________/ \\/ \\____________/___ \\\\" + RESET + "\n";
    
    // Base large (41 underscores)
    config.logo += GREEN  + " //_________________________________________\\\\" + RESET + "\n";
}

void creerConfigDefaut(AppConfig& config) {
    config.livresParPage = 10; // Valeur par défaut
    reinitialiserLogo(config); // Utilise la fonction pour le logo par défaut
}

bool chargerConfig(AppConfig& config, const std::string& filename) {
    std::ifstream fichier(filename);
    if (!fichier) return false; // Le fichier n'existe pas

    // 1. Lire le nombre de livres par page
    if (!(fichier >> config.livresParPage)) return false;
    
    // On ignore le reste de la ligne (le \n après le nombre)
    std::string dummy;
    std::getline(fichier, dummy);

    // 2. Lire le logo (tout le reste du fichier)
    config.logo = "";
    std::string ligne;
    while (std::getline(fichier, ligne)) {
        config.logo += ligne + "\n";
    }

    return true;
}

void sauvegarderConfig(const AppConfig& config, const std::string& filename) {
    std::ofstream fichier(filename);
    if (fichier) {
        fichier << config.livresParPage << std::endl;
        fichier << config.logo; // Le logo contient déjà les \n
    }
}