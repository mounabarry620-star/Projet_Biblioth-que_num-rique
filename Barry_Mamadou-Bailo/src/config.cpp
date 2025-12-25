/**
 * @file config.cpp
 * @brief Gestion de la configuration persistante de l'application.
 * @author Barry Mamadou Bailo
 * @version 1.0
 *
 * Gère le chargement et la sauvegarde du fichier 'app.conf', incluant
 * le logo ASCII personnalisé et les préférences d'affichage (pagination).
 */

#include <fstream> // Pour ifstream (lecture) et ofstream (écriture)
#include <iostream>
#include "config.hpp"
#include "utils.hpp" // Pour récupérer les constantes de couleurs (RED, GREEN...)

// Fonction pour redessiner le logo par défaut (hardcodé dans le programme)
void reinitialiserLogo(AppConfig& config) {
    // On vide la chaîne avant de la reconstruire
    config.logo = "";

    // Construction du logo ASCII Art ligne par ligne.
    // L'utilisation de += permet d'assembler le logo morceau par morceau.
    
    // --- HAUT (ROUGE) --- 
    // Note : On utilise les couleurs définies dans utils.hpp pour le style.
    // Le \\ sert à afficher un seul antislash (car \ est un caractère d'échappement en C++).
    config.logo += RED    + "       __...--~~~~~-._   _.-~~~~~--...__" + RESET + "\n";
    config.logo += RED    + "     //               `V'               \\\\" + RESET + "\n";

    // --- MILIEU (JAUNE) ---
    // On décale progressivement vers la gauche pour donner l'effet de perspective du livre.
    config.logo += YELLOW + "    //                 |                 \\\\" + RESET + "\n";
    config.logo += YELLOW + "   //__      BIBLIOTHEQUE NUMERIQUE    __ \\\\" + RESET + "\n";

    // --- BAS (VERT) ---
    // Attention aux échappements : \\ affiche \ à l'écran
    config.logo += GREEN  + "  //___\\____________/ \\/ \\____________/___ \\\\" + RESET + "\n";
    
    // Base large (41 underscores) pour fermer le livre
    config.logo += GREEN  + " //_________________________________________\\\\" + RESET + "\n";
}

// Initialise la configuration avec des valeurs de base si aucun fichier n'existe
void creerConfigDefaut(AppConfig& config) {
    config.livresParPage = 10; // Par défaut, on affiche 10 livres par page
    reinitialiserLogo(config); // On charge le dessin du logo
}

bool chargerConfig(AppConfig& config, const std::string& filename) {
    // ifstream = Input File Stream (Flux de fichier en Entrée/Lecture)
    std::ifstream fichier(filename);
    
    // Si le fichier n'existe pas ou ne peut pas être ouvert, on retourne false
    if (!fichier) return false; 

    // 1. Lire le nombre de livres par page (c'est le premier chiffre du fichier)
    if (!(fichier >> config.livresParPage)) return false;
    
    // IMPORTANT : Après avoir lu un entier avec >>, le caractère de saut de ligne (\n)
    // reste bloqué dans le buffer. Si on ne l'enlève pas, le prochain getline lira une ligne vide.
    std::string dummy;
    std::getline(fichier, dummy); // On "consomme" le reste de la ligne inutile

    // 2. Lire le logo (tout le reste du fichier ligne par ligne)
    config.logo = "";
    std::string ligne;
    // La boucle continue tant qu'il y a des lignes dans le fichier
    while (std::getline(fichier, ligne)) {
        config.logo += ligne + "\n"; // On rajoute le saut de ligne car getline l'enlève
    }

    return true; // Chargement réussi
}

void sauvegarderConfig(const AppConfig& config, const std::string& filename) {
    // ofstream = Output File Stream (Flux de fichier en Sortie/Écriture)
    std::ofstream fichier(filename);
    
    if (fichier) {
        // On écrit d'abord les paramètres simples
        fichier << config.livresParPage << std::endl;
        // Puis on écrit le gros bloc de texte du logo
        fichier << config.logo; 
    }
}