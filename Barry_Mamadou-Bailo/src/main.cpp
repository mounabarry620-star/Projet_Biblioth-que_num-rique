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
#include <limits> // Pour nettoyer cin en cas d'erreur (std::numeric_limits)
#include "utils.hpp"
#include "menu.hpp"
#include "library.hpp"
#include "config.hpp"


// Fonction pour configurer la bibliothèque si library.db n'existe pas encore
// C'est le "Wizard" de première installation.
void initialiserNouvelleBibliotheque(Library& lib, const AppConfig& config) {
    afficherHeader("BIENVENUE - CONFIGURATION", config);
    
    printColor("\n  (i) Aucun fichier de données trouvé.", YELLOW);
    std::cout << "      Commençons par configurer votre espace.\n\n";

    // 1. Nom de la bibliothèque
    std::cout << "  " << BOLD << "Nom de votre bibliothèque : " << RESET;
    // Si l'utilisateur tape juste Entrée, on met un nom par défaut
    if (std::getline(std::cin, lib.name) && lib.name.empty()) {
        lib.name = "Ma Bibliothèque"; 
    }

    // 2. Description Multi-lignes
    std::cout << "  " << BOLD << "Description (Laissez une ligne vide pour finir) :" << RESET << "\n";
    std::string ligne, descComplete;
    
    // Boucle pour permettre à l'utilisateur d'écrire plusieurs paragraphes
    while (true) {
        std::cout << "    > ";
        std::getline(std::cin, ligne);
        if (ligne.empty()) break; // Une ligne vide signifie la fin de la saisie
        
        if (!descComplete.empty()) descComplete += "\n";
        descComplete += ligne;
    }
    // Valeur par défaut si l'utilisateur n'a rien écrit
    lib.description = descComplete.empty() ? "Gestionnaire de livres personnel" : descComplete;

    // 3. Confirmation et Pause
    printColor("\n  (v) Configuration enregistrée avec succès !", GREEN);
    std::cout << "  Appuyez 2 fois sur Entrée pour accéder au menu principal...";
    
    // On nettoie le buffer et on attend une touche
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::cin.get(); 
}

int main() {

    // Nom du fichier de la base de données (persistance)
    const std::string dbFile = "library.db";
    
    // 1. Chargement de la configuration (logo, préférences d'affichage)
    AppConfig config;
    if (!chargerConfig(config, "app.conf")) {
        // Si pas de config, on en crée une par défaut et on la sauvegarde
        creerConfigDefaut(config);
        sauvegarderConfig(config, "app.conf");
    }

    // 2. Chargement de la bibliothèque (les livres)
    Library maBiblio;
    
    // On essaye de charger le fichier. La fonction renvoie false s'il n'existe pas.
    if (!chargerBibliotheque(maBiblio, "library.db")) {
        // Premier lancement : on lance l'initialisation guidée
        initialiserNouvelleBibliotheque(maBiblio, config);
        
        // On sauvegarde immédiatement pour créer le fichier physique library.db
        sauvegarderBibliotheque(maBiblio, "library.db");
    }

    // Variable d'état pour suivre si des modifications ont eu lieu (pour la sauvegarde en quittant)
    bool aDesModifs = false;

    // 3. Boucle principale du menu
    int choix = 0;

    // La boucle do-while assure que le menu s'affiche au moins une fois
    do {
        // On nettoie l'écran à chaque tour pour une interface propre
        clearScreen();

        // On affiche le logo et les options disponibles
        afficherMenuPrincipal(config);

        // Récupération sécurisée du choix utilisateur
        if (!(std::cin >> choix)) {
            // Si l'utilisateur tape des lettres au lieu d'un chiffre, on répare le flux cin
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // On vide le buffer
            choix = 0; // On force un choix invalide pour recommencer la boucle
        }
        std::cin.ignore(); // On consomme le retour à la ligne restant

        // 4. Traitement du choix via un switch
        switch (choix) {
            case 1:
                // Affichage de la liste des livres
                consulterReferences(maBiblio, config); 
                break;
            case 2:
                // Menu de gestion (Ajout, Import, Suppression)
                // On passe 'aDesModifs' pour savoir si on doit proposer de sauvegarder plus tard
                gererReferences(maBiblio, config, aDesModifs); 
                break;
            case 3:
                // Recherche filtrée
                chercherReferences(maBiblio, config);
                break;
            case 4:
                // Exportation vers une page Web
                afficherHeader("EXPORT HTML", config); 
                exporterHTML(maBiblio, "catalogue.html");
                std::cout << ">> Export terminé ! Ouvrez 'catalogue.html' dans votre navigateur." << std::endl;
                std::cout << "Appuyez sur Entrée...";
                std::cin.ignore(); std::cin.get();
                break;
            case 5:
                // Configuration (Logo, Titre...)
                gererParametres(maBiblio, config, aDesModifs); 
                break;
            case 6:
                // Quitter l'application
                // Vérification de sécurité : y a-t-il des données non sauvegardées ?
                if (aDesModifs) {
                    clearScreen();
                    printColor("=== MODIFICATIONS NON ENREGISTRÉES ===", 31); // Rouge
                    std::cout << "Vous avez des modifications en attente." << std::endl;
                    std::cout << "1. Sauvegarder et quitter" << std::endl;
                    std::cout << "2. Quitter sans sauvegarder" << std::endl;
                    std::cout << "3. Annuler (Ne pas quitter)" << std::endl;
                    std::cout << "> Choix : ";
                    
                    int sousChoix;
                    std::cin >> sousChoix;
                    
                    if (sousChoix == 1) {
                        sauvegarderBibliotheque(maBiblio, dbFile);
                        printColor("Sauvegarde effectuée. Au revoir !", 32);
                        // 'choix' reste à 6, donc on sortira de la boucle
                    }
                    else if (sousChoix == 2) {
                        printColor("Modifications perdues. Au revoir !", 33);
                        // 'choix' reste à 6, on sort
                    }
                    else {
                        // Annulation : on remet 'choix' à 0 pour rester dans la boucle principale
                        choix = 0; 
                    }
                } else {
                    // Pas de modifications en attente, on quitte directement
                    printColor("Au revoir !", 34);
                }
                break;
            default:
                // Gestion des entrées incorrectes (ex: 7, 8...)
                printColor("Choix invalide. Veuillez réessayer.", 31);
                std::cin.ignore(); std::cin.get(); // Pause pour lire l'erreur
                break;
        }

    } while (choix != 6); // La boucle continue tant qu'on ne choisit pas de quitter

    return 0;
}