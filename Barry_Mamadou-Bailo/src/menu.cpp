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

#include <iostream>
#include <limits>
#include <string> // Important pour std::to_string
#include "menu.hpp"
#include "utils.hpp"
#include <iomanip> // Pour std::setw
#include <cstdlib> // Pour std::exit()
#include "config.hpp"

// ============================================================
// FONCTIONS UTILITAIRES D'AFFICHAGE
// ============================================================

// Prépare une cellule de texte d'une largeur fixe
std::string formaterCellule(std::string texte, int largeur) {
    std::string resultat;
    if ((int)texte.length() > largeur) {
        resultat = texte.substr(0, largeur - 3) + "...";
    } else {
        resultat = texte;
        int espaces = largeur - texte.length();
        for (int i = 0; i < espaces; i++) resultat += " ";
    }
    return resultat;
}

void afficherDetailsLivre(const Book& livre, const AppConfig& config) {
    afficherHeader("DETAILS DU LIVRE", config);

    // 1. Titre du livre avec icône et soulignement
    std::cout << "\n      " << "📘 " << CYAN << BOLD << livre.title << RESET << "\n";
    std::cout << "      " << repeat("-", livre.title.length() + 3) << "\n\n";

    // 2. Informations techniques (Alignement propre)
    // On définit une largeur de 12 pour les libellés
    
    // Auteurs
    std::cout << "      " << ITALIC << std::setw(12) << std::left << "Auteur(s)" << RESET 
              << ": " << WHITE << livre.authors << RESET << "\n";

    // Genre (Magenta)
    std::cout << "      " << ITALIC << std::setw(12) << std::left << "Genre" << RESET 
              << ": " << MAGENTA << livre.genre << RESET << "\n";

    // Parution (Bleu)
    std::cout << "      " << ITALIC << std::setw(12) << std::left << "Parution" << RESET 
              << ": " << BLUE << livre.date << RESET << "\n";

    // Langue (Blanc)
    std::cout << "      " << ITALIC << std::setw(12) << std::left << "Langue" << RESET 
              << ": " << livre.language << "\n";

    // ISBN (Jaune)
    std::cout << "      " << ITALIC << std::setw(12) << std::left << "ISBN" << RESET 
              << ": " << YELLOW << livre.isbn << RESET << "\n\n";

    // 3. Bloc Description
    std::cout << "      " << ITALIC << BOLD << "Description :" << RESET << "\n";
    // On ajoute un petit décalage pour le texte de description aussi
    std::cout << "      " << livre.description << "\n";

    // 4. Pied de page
    std::cout << "\n      " << repeat("-", 100) << "\n";
    std::cout << "      Appuyez 2 fois sur Entrée pour revenir...";
    
    // Pause
    std::cin.ignore(); 
    std::cin.get();
}

// CETTE FONCTION EST LE COEUR DE L'AFFICHAGE (Réutilisée pour Consulter et Chercher)
void afficherListePaginee(const Library& lib, const std::vector<Book>& livresAAfficher, std::string titreMenu, const AppConfig& config) {
   
    int livresParPage = config.livresParPage;

    int page = 0;
    int totalLivres = livresAAfficher.size();
    bool continuer = true;

    while (continuer) {
      
        // ICI : On appelle le header AVEC la config (donc le logo s'affiche)
        afficherHeader(titreMenu, config);

        // Infos Bibliothèque (Maison + Titre)
        if (titreMenu == "CONSULTER LES REFERENCES") {
            std::cout << "  🏠 " << YELLOW << BOLD << lib.name << RESET << std::endl;
            std::cout << "      " << WHITE << lib.description << RESET << std::endl;
        }
        
        // Compteur (affichera 0 si vide, ce qui est correct)
        std::cout << "\n  Nombre de livres : " << BOLD << totalLivres << RESET << std::endl;
        std::cout << "  " << repeat("-", 50) << std::endl;
        std::cout << "  " << GREEN << ITALIC << "References" << RESET << std::endl;
        std::cout << "  " << repeat("-", 50) << std::endl;

        // 2. CAS PARTICULIER : SI VIDE
        if (livresAAfficher.empty()) {
            std::cout << "\n    (o_o)  Aucun livre dans cette liste pour l'instant.\n" << std::endl;
            // On force page à 0 et on empêche d'afficher la boucle for
            continuer = false; // On sortira après le wait
            std::cout << "  " << repeat("-", 50) << std::endl;
            std::cout << "  Appuyez sur Entrée pour revenir...";
            std::cin.get(); 
            return; // On quitte proprement
        }

        // 3. AFFICHAGE DE LA LISTE (S'il y a des livres)
        int debut = page * livresParPage;
        int fin = debut + livresParPage;
        if (fin > totalLivres) fin = totalLivres;

        /// 4. BOUCLE D'AFFICHAGE (Style Liste)
        for (int i = debut; i < fin; ++i) {
            const Book& b = livresAAfficher[i];
            
            // LIGNE 1 : Numéro - Icône - Titre - Auteur
            // Ex: 1. 📖 Titre par Auteur
            std::cout << "  " << std::setw(2) << (i + 1) << ". " 
                      << "📘 " << CYAN << BOLD << b.title << RESET 
                      << " par " << WHITE << b.authors << RESET << std::endl;

            // LIGNE 2 : Infos techniques avec le connecteur L
            // Ex:    └── ISBN: ... | Genre: ... | Parution: ...
            std::cout << "      " << "└── " 
                      << "ISBN: " << YELLOW << b.isbn << RESET << " | "
                      << "Genre: " << MAGENTA << b.genre << RESET << " | "
                      << "Parution: " << BLUE << b.date << RESET << std::endl;
            std::cout << std::endl; 
        }

        // 4. PIED DE PAGE
        std::cout << "  " << repeat("-", 50) << std::endl;
        int nbPages = (totalLivres + livresParPage - 1) / livresParPage;

        std::cout << "  Page " << (page + 1) << " / " << nbPages << std::endl;
        if (page > 0)
            std::cout << "  " << (debut) << ". Page precedente [P]" << std::endl;
        if (fin < totalLivres)
        std::cout << "\n  " << (fin + 1) << ". Page suivante [S]" << std::endl;
        std::cout << "  " << (fin + 2) << ". Retour [Q]" << std::endl;
        std::cout << "\n " << GREEN << "> Votre choix : " << RESET;


        std::string choix;
        std::cin >> choix;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Vider le buffer
        // Gestion de la navigation
        if (choix == "s" || choix == "S") { 
            if (fin < totalLivres) page++; 
            else printColor("Vous êtes à la dernière page.", RED);
        }
        else if (choix == "p" || choix == "P") { 
            if (page > 0) page--; 
            else printColor("Vous êtes à la première page.", RED);
        }
        else if (choix == "q" || choix == "Q") { 
            continuer = false; 
        }
        else {
            // Tentative de conversion en numéro
            try {
                int index = std::stoi(choix);
                index--; // On passe de 1..N à 0..N-1
                if (index >= 0 && index < totalLivres) {
                    afficherDetailsLivre(livresAAfficher[index], config);
                }
            } catch (...) {}
        }
    }
}

// ============================================================
// FONCTIONS PRINCIPALES DU MENU
// ============================================================


void afficherMenuPrincipal(const AppConfig& config) {
    // Le logo sera affiché par le header
    afficherHeader("MENU PRINCIPAL", config);
    
    // On utilise des couleurs différentes pour les numéros
    std::cout << "      " << CYAN << "[1]" << RESET << " 📚 Consulter les references" << std::endl;
    std::cout << "      " << CYAN << "[2]" << RESET << " ✨ Gerer les references (Ajout/Import/Suppr)" << std::endl;
    std::cout << "      " << CYAN << "[3]" << RESET << " 🔍 Chercher une reference" << std::endl;
    std::cout << "      " << CYAN << "[4]" << RESET << " 🌐 Exporter en HTML" << std::endl;
    std::cout << "      " << CYAN << "[5]" << RESET << " ⚙️  Parametres" << std::endl;
    std::cout << "      " << RED  << "[6]" << RESET << " 🚪 Quitter" << std::endl;
    
    std::cout << "\n " << GREEN << "> Votre choix : " << RESET;
}

// Fonction simplifiée grâce à notre refactorisation !
void consulterReferences(const Library& lib, const AppConfig& config) {

    // On passe simplement tous les livres à la fonction d'affichage
    afficherListePaginee(lib, lib.books, "CONSULTER LES REFERENCES", config);
}

void chercherReferences(const Library& lib, const AppConfig& config) {
    clearScreen();
    // Affiche le logo + titre
    afficherHeader("RECHERCHE", config);
    std::cout << "      " << CYAN << "[1]" << RESET << " Par ISBN" << std::endl;
    std::cout << "      " << CYAN << "[2]" << RESET << " Par Titre" << std::endl;
    std::cout << "      " << CYAN << "[3]" << RESET << " Par Code Editeur" << std::endl;
    std::cout << "      " << CYAN << "[4]" << RESET << " ↩️  Retour au menu principal" << std::endl;
    std::cout << "      " << RED  << "[5]" << RESET << " 🚪 Quitter l'application" << std::endl;
    std::cout << "\n " << GREEN << "> Choix : " << RESET;

    int choix;
    std::cin >> choix;
    std::cin.ignore(); // Vider le buffer

    if (choix == 4) return; // Retour au menu principal
    if (choix == 5) {       // Quitter l'application directement
        printColor("\n  Au revoir ! A bientot.", GREEN);
        std::exit(0);
    }

    std::cout << "Entrez votre recherche : ";
    std::string recherche;
    std::getline(std::cin, recherche);

    // Vecteur qui contiendra les résultats
    std::vector<Book> resultats;
    std::string rechercheLower = toLower(recherche);

    for (const auto& livre : lib.books) {
        bool correspond = false;

        if (choix == 1) { 
            // Recherche Exacte ISBN
            if (livre.isbn == recherche) correspond = true;
        }
        else if (choix == 2) { 
            // Recherche Titre (contient le texte, insensible casse)
            // On cherche si le titre en minuscule contient la recherche en minuscule
            if (toLower(livre.title).find(rechercheLower) != std::string::npos) {
                correspond = true;
            }
        }
        else if (choix == 3) {
            // Recherche Editeur (partie de l'ISBN)
            // L'éditeur est la 3ème partie : 978-2-XXX-...
            // On simplifie : on regarde si l'ISBN contient "-CODE-"
            if (livre.isbn.find("-" + recherche + "-") != std::string::npos) {
                correspond = true;
            }
        }

        if (correspond) {
            resultats.push_back(livre);
        }
    }

    if (!resultats.empty()) {
        afficherListePaginee(lib, resultats, "RESULTATS DE RECHERCHE", config);
     } else {
        printColor("\n  Aucun résultat trouvé.", RED);
        std::cout << "  Appuyez sur Entrée..."; std::cin.get();
    }
}

// --- À placer avant gererReferences dans src/menu.cpp ---

void menuAjouterLivre(Library& lib, const AppConfig& config, bool& aDesModifs) {
    clearScreen();
    // Utilise le header avec le logo !
    afficherHeader("AJOUT D'UN LIVRE", config);

    Book b;
    std::cout << "  " << BOLD << "Saisie des informations :" << RESET << "\n\n";

    
    // 1. ISBN (Vérification unique)
    std::cout << "ISBN-13 (ex: 978-2-...) : ";
    std::cin >> b.isbn;
    std::cin.ignore(); // Pour vider le \n restant

    if (isbnExiste(lib, b.isbn)) {
        printColor("Erreur : Cet ISBN existe déjà !", 31); // Rouge
        std::cout << "Appuyez sur Entrée...";
        std::cin.get();
        return;
    }

    // 2. Titre
    std::cout << "Titre : ";
    std::getline(std::cin, b.title);

    // 3. Langue
    std::cout << "Langue : ";
    std::getline(std::cin, b.language);

    // 4. Auteurs (Saisie multiligne)
    std::cout << "Auteurs (un par ligne, Entrée vide pour finir) :" << std::endl;
    std::string auteurTemp;
    b.authors = "";
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, auteurTemp);
        if (auteurTemp.empty()) break; // Arrêt si vide
        
        if (!b.authors.empty()) b.authors += ", ";
        b.authors += auteurTemp;
    }
    // 5. Date avec validation stricte
    while (true) {
        std::cout << "Date de parution (JJ/MM/AAAA) : ";
        std::getline(std::cin, b.date);

        if (estDateValide(b.date)) {
            break; // Sort de la boucle si c'est bon
        } else {
            printColor("Erreur : Format invalide ou date incoherente (ex: 30/02). Réessayer.", 31);
        }
    }

    // 6. Genre
    std::cout << "Genre litteraire : ";
    std::getline(std::cin, b.genre);

    // 7. Description (Saisie multiligne)
    std::cout << "Description (plusieurs lignes, Entrée vide pour finir) :" << std::endl;
    std::string descTemp;
    b.description = "";
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, descTemp);
        if (descTemp.empty()) break;
        
        if (!b.description.empty()) b.description += "\n";
        b.description += descTemp;
    }

    // Validation
    ajouterLivre(lib, b);
    
    // Sauvegarde immédiate pour ne pas perdre les données
    aDesModifs = true;

    printColor("\nLivre ajouté (Pensez a sauvegarder en quittant) !", 32);
    std::cout << "Appuyez sur Entrée...";
    std::cin.get();
}

void gererReferences(Library& lib,const AppConfig& config, bool& aDesModifs) {
    int choix = 0;
    do {
        clearScreen();
        // Ajoute config ici aussi
        afficherHeader("GESTION DES REFERENCES", config);
        std::cout << "  " << CYAN << "[1]" << RESET << " ➕ Ajouter un livre manuellement" << std::endl;
        std::cout << "  " << CYAN << "[2]" << RESET << " 📂 Importer depuis un fichier CSV" << std::endl;
        std::cout << "  " << CYAN << "[3]" << RESET << " 🗑️  Supprimer toutes les references" << std::endl;
        std::cout << "  " << CYAN << "[4]" << RESET << " ↩️  Retour au menu principal" << std::endl;
        std::cout << "\n " << GREEN << "> Votre choix : " << RESET;

        if (!(std::cin >> choix)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choix = 0;
        }
        std::cin.ignore(); // Important pour le getline suivant

        switch (choix) {
            case 1:
                menuAjouterLivre(lib, config, aDesModifs);
                break;
            case 2:
                
                {
                    printColor("=== IMPORTER UN FICHIER CSV ===", 33);
                    std::cout << "Nom du fichier CSV ou chemin (ex: livres_test.csv ou /chemin/vers/fichier.csv) : ";
                    std::string nomFichier;
                    std::getline(std::cin, nomFichier);

                    std::cout << "Importation en cours..." << std::endl;
                    
                    int resultat = importerReferences(lib, nomFichier);
                    
                    if (resultat == -1) {
                        printColor("Erreur : Impossible d'ouvrir le fichier !", 31);
                    } else {
                        // On utilise std::to_string pour concaténer le nombre avec le texte
                        printColor("Succès ! " + std::to_string(resultat) + " livres importés.", 32);
                        aDesModifs = true; // Signale la modification
                    }
                    std::cout << "Appuyez sur Entrée...";
                    std::cin.get();
                }
                break;
            case 3:
                { // Accolades nécessaires car on déclare des variables dans un case
                    printColor("ATTENTION : Vous allez supprimer TOUS les livres !", 31); // Rouge
                    std::cout << "Etes-vous sur de vouloir continuer ? (O/N) : ";
                    char confirm;
                    std::cin >> confirm;
                    
                    if (confirm == 'O' || confirm == 'o') {
                        supprimerToutesReferences(lib);
                        printColor("La bibliotheque a ete videe.", 32); // Vert
                    } else {
                        std::cout << "Operation annulee." << std::endl;
                    }
                    // Petite pause
                    std::cin.ignore(); std::cin.get(); 
                }
                break;
            case 4:
                break; // On quitte la boucle
            case 5: { // sauvegarde automatique avant de quitter
               if (aDesModifs) {
                    printColor("Sauvegarde automatique des modifications...", YELLOW);
                    sauvegarderBibliotheque(lib, "library.db");
                    printColor("(v) Sauvegarde réussie.", GREEN);
                }
                printColor("Au revoir !", GREEN); 
                std::exit(0);
            }
        }
    } while (choix != 4);
}


void gererParametres(Library& lib, AppConfig& config, bool& aDesModifs) {
    int choix = 0;
    do {
        clearScreen();
        // Affiche le logo + titre
        afficherHeader("PARAMETRES", config);
        std::cout << "      " << CYAN << "[1]" << RESET << " 📝 Modifier titre/desc" << std::endl;
        std::cout << "      " << CYAN << "[2]" << RESET << " 📄 Livres par page (" << config.livresParPage << ")" << std::endl;
        std::cout << "      " << CYAN << "[3]" << RESET << " 🎨 Modifier le logo" << std::endl;
        std::cout << "      " << CYAN << "[4]" << RESET << " ↩️  Retour au menu principal" << std::endl;
        std::cout << "\n " << GREEN << "> Votre choix : " << RESET;

        if (!(std::cin >> choix)) {
            std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choix = 0;
        }
        std::cin.ignore(); 

        switch (choix) {
            case 1: { // Modifier Titre/Desc (Données Bibliothèque)
                printColor("--- Modifier Informations ---", 34);
                std::cout << "Laissez vide pour conserver la valeur actuelle.\n";
                
                std::cout << "Titre actuel : " << lib.name << std::endl;
                std::cout << "Nouveau titre : ";
                std::string temp;
                std::getline(std::cin, temp);
                
                // Si la saisie n'est pas vide, on met à jour ET on signale la modif
                if (!temp.empty()) {
                    lib.name = temp;
                    aDesModifs = true; // <--- On signale la modification
                }

                std::cout << "Description actuelle : " << lib.description << std::endl;
                std::cout << "Nouvelle description : ";
                std::getline(std::cin, temp);
                
                if (!temp.empty()) {
                    lib.description = temp;
                    aDesModifs = true; // <--- On signale la modification
                }

                // PLUS DE SAUVEGARDE AUTOMATIQUE ICI
                if (aDesModifs) {
                    printColor("Modifications prises en compte (Pensez a sauvegarder en quittant) !", 32);
                } else {
                    std::cout << "Aucun changement effectue." << std::endl;
                }
                std::cin.get();
                break;
            }
            case 2: { // Modifier Pagination (Préférences App)
                printColor("--- Parametrer Affichage ---", 34);
                std::cout << "Nombre de livres par page : ";
                int n;
                if (std::cin >> n && n > 0) {
                    config.livresParPage = n;
                    sauvegarderConfig(config, "app.conf"); // On sauvegarde la config directe
                    printColor("Parametre sauvegarde !", 32);
                } else {
                    printColor("Erreur : Nombre invalide.", 31);
                }
                std::cin.ignore(); std::cin.get();
                break;
            }
            case 3: { // Modifier Logo (Préférences App)
                printColor("--- Modifier Logo ---", 34);
                std::cout << "1. Saisir un nouveau logo manuellement (Ligne vide pour terminer)" << std::endl;
                std::cout << "2. Restaurer le logo par defaut" << std::endl;
                std::cout << "3. Annuler" << std::endl;
                std::cout << "> Choix : ";
                
                int subChoix;
                std::cin >> subChoix;
                std::cin.ignore(); // Vider le buffer

                if (subChoix == 1) {
                    // --- SAISIE MANUELLE ---
                    std::cout << "Entrez votre nouveau logo ligne par ligne." << std::endl;
                    std::cout << "Terminez par une ligne vide pour valider." << std::endl;
                    
                    std::string nouveauLogo = "";
                    std::string ligne;
                    while (true) {
                        std::getline(std::cin, ligne);
                        if (ligne.empty()) break;
                        nouveauLogo += ligne + "\n";
                    }

                    if (!nouveauLogo.empty()) {
                        config.logo = nouveauLogo;
                        sauvegarderConfig(config, "app.conf");
                        printColor("Nouveau logo enregistre !", 32);
                    }
                } 
                else if (subChoix == 2) {
                    // --- RESET PAR DEFAUT ---
                    creerConfigDefaut(config); // On recrée la config par défaut
                    sauvegarderConfig(config, "app.conf");
                    printColor("Logo par defaut restaure !", 32);
                }
                
                std::cin.get(); // Pause
                break;
            }
            case 4: break;
            case 5: {
               // Quitter l'application directement
               if (aDesModifs) {
                    printColor("Sauvegarde automatique des modifications...", YELLOW);
                    sauvegarderBibliotheque(lib, "library.db");
                    printColor("(v) Sauvegarde réussie.", GREEN);
                }
                printColor("Au revoir !", GREEN); 
                std::exit(0);
            }
        }
    } while (choix != 4);
}













