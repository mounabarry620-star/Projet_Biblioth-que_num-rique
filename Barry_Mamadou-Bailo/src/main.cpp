/**
 * @file main.cpp
 * @brief Point d'entrée de l'application de Gestion de Bibliothèque.
 * @author Barry Mamadou Bailo
 * @date Janvier 2026
 * @version 1.0
 *
 * Ce fichier contient la fonction main(), l'initialisation de l'application
 * (chargement de la base de données et de la configuration) et la boucle
 * principale du programme.
 */

#include <iostream>
#include <limits> // Pour nettoyer cin en cas d'erreur
#include "utils.hpp"
#include "menu.hpp"
#include "library.hpp"
#include "config.hpp"


// Fonction pour configurer la bibliothèque si library.db n'existe pas
void initialiserNouvelleBibliotheque(Library& lib, const AppConfig& config) {
    afficherHeader("BIENVENUE - CONFIGURATION", config);
    
    printColor("\n  (i) Aucun fichier de donnees trouve.", YELLOW);
    std::cout << "      Commencons par configurer votre espace.\n\n";

    // 1. Nom de la bibliothèque
    std::cout << "  " << BOLD << "Nom de votre bibliotheque : " << RESET;
    if (std::getline(std::cin, lib.name) && lib.name.empty()) {
        lib.name = "Ma Bibliotheque"; 
    }

    // 2. Description Multi-lignes (Correction demandée)
    std::cout << "  " << BOLD << "Description (Laissez une ligne vide pour finir) :" << RESET << "\n";
    std::string ligne, descComplete;
    while (true) {
        std::cout << "    > ";
        std::getline(std::cin, ligne);
        if (ligne.empty()) break; // Ligne vide = fin
        if (!descComplete.empty()) descComplete += "\n";
        descComplete += ligne;
    }
    // Valeur par défaut si vide
    lib.description = descComplete.empty() ? "Gestionnaire de livres personnel" : descComplete;

    // 3. Confirmation et Pause (Correction demandée)
    printColor("\n  (v) Configuration enregistree avec succes !", GREEN);
    std::cout << "  Appuyez 2 fois sur Entrée pour acceder au menu principal...";
    // On attend explicitement l'entrée
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Sécurité
    std::cin.get(); 
}

int main() {

    // Nom du fichier de la base de données
    const std::string dbFile = "library.db";
    
    // 1. Chargement de la configuration (logo, etc.)
    AppConfig config;
    if (!chargerConfig(config, "app.conf")) {
        creerConfigDefaut(config);
        sauvegarderConfig(config, "app.conf");
    }

    // 2. Chargement de la bibliothèque
    Library maBiblio;
    // On suppose que chargerBibliotheque renvoie false si le fichier n'existe pas
    if (!chargerBibliotheque(maBiblio, "library.db")) {
        // C'est ici que la magie opère pour le premier lancement !
        initialiserNouvelleBibliotheque(maBiblio, config);
        
        // On sauvegarde immédiatement pour créer le fichier library.db
        sauvegarderBibliotheque(maBiblio, "library.db");
    }

    // Variable d'état : Est-ce qu'on a des modifications non sauvegardées ?
    bool aDesModifs = false;

    // 2. Boucle principale du menu
    int choix = 0;

    do {
        // 1. On nettoie l'écran pour que ce soit propre
        clearScreen();

        // 2. On affiche le logo et le menu principal
        afficherMenuPrincipal(config);

        // 3. On récupère le choix de l'utilisateur
        if (!(std::cin >> choix)) {
            // Si l'utilisateur tape des lettres au lieu d'un chiffre
            std::cin.clear(); // On répare cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // On vide le buffer
            choix = 0; // On force un choix invalide pour recommencer
        }
        std::cin.ignore(); // On vide le buffer pour les getline suivants

        // 4. Traitement du choix (switch case - CM4)
        switch (choix) {
            case 1:
               // Appel de la nouvelle fonction
                consulterReferences(maBiblio, config); 
                break;
            case 2:
                // Connexion faite ici
                gererReferences(maBiblio, config, aDesModifs); 
                break;
            case 3:
                chercherReferences(maBiblio, config);
                break;
            case 4:
              // Export
                afficherHeader("EXPORT HTML", config); // Juste pour le style pendant l'export
                exporterHTML(maBiblio, "catalogue.html");
                std::cout << ">> Export termine ! Ouvrez 'catalogue.html' dans votre navigateur." << std::endl;
                // Pause
                std::cout << "Appuyez sur Entree...";
                std::cin.ignore(); std::cin.get();
                break;
            case 5:
                gererParametres(maBiblio, config, aDesModifs); // <--- Connexion du menu
                break;
            case 6:
           if (aDesModifs) {
                    clearScreen();
                    printColor("=== MODIFICATIONS NON ENREGISTREES ===", 31); // Rouge
                    std::cout << "Vous avez des modifications en attente." << std::endl;
                    std::cout << "1. Sauvegarder et quitter" << std::endl;
                    std::cout << "2. Quitter sans sauvegarder" << std::endl;
                    std::cout << "3. Annuler (Ne pas quitter)" << std::endl; // Option confort
                    std::cout << "> Choix : ";
                    
                    int sousChoix;
                    std::cin >> sousChoix;
                    
                    if (sousChoix == 1) {
                        sauvegarderBibliotheque(maBiblio, dbFile);
                        printColor("Sauvegarde effectuee. Au revoir !", 32);
                        // On laisse choix à 6 pour sortir de la boucle do..while
                    }
                    else if (sousChoix == 2) {
                        printColor("Modifications perdues. Au revoir !", 33);
                        // On laisse choix à 6 pour sortir
                    }
                    else {
                        // On annule la sortie
                        choix = 0; // On change 'choix' pour ne pas sortir du while(choix != 6)
                    }
                } else {
                    // Pas de modifs, on quitte direct
                    printColor("Au revoir !", 34);
                }
                break;
            default:
                printColor("Choix invalide. Veuillez reessayer.", 31);
                std::cin.ignore(); std::cin.get(); // Pause
                break;
        }

    } while (choix != 6); // Tant que ce n'est pas "Quitter"

    return 0;
}