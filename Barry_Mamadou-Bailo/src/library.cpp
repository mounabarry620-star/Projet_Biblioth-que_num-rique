/**
 * @file library.cpp
 * @brief Gestion des données de la bibliothèque (Livres, Import/Export).
 * @author Barry Mamadou Bailo
 * @version 1.0
 *
 * Contient les fonctions de manipulation des livres : ajout, suppression,
 * vérification d'ISBN, importation CSV et l'exportation du catalogue en HTML.
 * Gère également la persistance des données via library.db.
 */

#include <iostream>
#include <fstream>   // Pour lire/écrire dans les fichiers
#include <vector>
#include <string>
#include <algorithm> // Pour std::sort (tri des livres) et std::replace
#include <sstream>   // Pour std::istringstream (découpage des chaînes)
#include "library.hpp"
#include "utils.hpp" 

// Fonction utilitaire interne pour découper une ligne CSV.
// Elle sépare la chaîne 's' à chaque fois qu'elle rencontre le 'delimiter' (ici ';').
std::vector<std::string> splitLigne(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    
    // On lit flux par flux jusqu'au délimiteur
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    // Cas particulier : si la ligne finit par un point-virgule, on ajoute une chaîne vide
    // pour garder le bon nombre de colonnes.
    if (!s.empty() && s.back() == delimiter) {
        tokens.push_back("");
    }
    return tokens;
}

// Fonction pour nettoyer le texte avant sauvegarde.
// C'est crucial pour ne pas casser le format CSV : si un titre contient un ';',
// cela créerait une colonne fantôme. On le remplace par une virgule.
std::string nettoyerTexte(std::string s) {
    std::replace(s.begin(), s.end(), ';', ',');  // Sécurité CSV
    std::replace(s.begin(), s.end(), '\n', ' '); // On met tout sur une ligne
    // On enlève aussi les retours chariot Windows (\r) si présents
    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
    return s;
}

bool chargerBibliotheque(Library& lib, const std::string& filename) {
    std::ifstream fichier(filename);
    if (!fichier.is_open()) return false; // Le fichier n'existe pas encore

    // 1. Lecture de l'en-tête (Nom et Description sur les 2 premières lignes)
    if (!std::getline(fichier, lib.name)) lib.name = "Ma Bibliothèque";
    if (!std::getline(fichier, lib.description)) lib.description = "Description par défaut";

    // 2. Lecture des livres
    lib.books.clear(); // On vide la liste avant de charger pour éviter les doublons
    std::string line;
    
    while (std::getline(fichier, line)) {
        if (line.empty()) continue; // On ignore les lignes vides

        // On découpe la ligne en morceaux grâce aux points-virgules
        std::vector<std::string> data = splitLigne(line, ';');
        
        // On vérifie qu'on a au moins 6 colonnes pour créer un livre valide
        if (data.size() >= 6) {
            Book b;
            b.isbn = data[0];
            b.title = data[1];
            b.language = data[2];
            b.authors = data[3];
            b.date = data[4];
            b.genre = data[5];
            
            // La description est optionnelle, mais si elle est là, on la prend
            if (data.size() > 6) b.description = data[6];
            else b.description = "";

            lib.books.push_back(b);
        }
    }

    return true;
}

void sauvegarderBibliotheque(const Library& lib, const std::string& filename) {
    std::ofstream fichier(filename);
    if (fichier) {
        // En-tête : Nom et Description sur 2 lignes distinctes
        fichier << nettoyerTexte(lib.name) << std::endl;
        fichier << nettoyerTexte(lib.description) << std::endl;

        // Livres : Chaque livre est écrit sur UNE SEULE ligne (format CSV).
        // Les champs sont séparés par des points-virgules ';'.
        for (const auto& livre : lib.books) {
            fichier << nettoyerTexte(livre.isbn) << ";"
                    << nettoyerTexte(livre.title) << ";"
                    << nettoyerTexte(livre.language) << ";"
                    << nettoyerTexte(livre.authors) << ";"
                    << nettoyerTexte(livre.date) << ";"
                    << nettoyerTexte(livre.genre) << ";"
                    << nettoyerTexte(livre.description) << std::endl;
        }

    } else {
        std::cerr << "Erreur : Impossible d'écrire dans le fichier " << filename << std::endl;
    }
}

void initialiserBibliotheque(Library& lib) {
    // Valeurs par défaut pour une nouvelle installation
    lib.name = "Ma Bibliothèque";
    lib.description = "Gestionnaire de livres personnel";
    lib.books.clear();


}

bool isbnExiste(const Library& lib, const std::string& isbn) {
    // On parcourt tous les livres un par un
    for (const auto& livre : lib.books) {
        // Si on trouve l'ISBN, on renvoie vrai tout de suite
        if (livre.isbn == isbn) return true;
    }
    return false; // Si on a fini la boucle sans trouver
}

void ajouterLivre(Library& lib, const Book& nouveauLivre) {
    // Ajoute le livre à la fin du vecteur dynamique
    lib.books.push_back(nouveauLivre);
    // On pourrait trier ici, mais on le fera plus tard si besoin
}

void supprimerToutesReferences(Library& lib) {
    lib.books.clear(); // Vide le vecteur en mémoire
    // On sauvegarde tout de suite pour acter le changement dans le fichier
    sauvegarderBibliotheque(lib, "library.db");
}

int importerReferences(Library& lib, const std::string& filename) {
    std::ifstream fichier(filename);
    if (!fichier) return -1; // Erreur d'ouverture

    // --- DETECTION DU FORMAT ---
    // On regarde la première ligne pour deviner si c'est un CSV (avec ;) ou l'ancien format vertical
    std::string premiereLigne;
    std::getline(fichier, premiereLigne);
    
    // On remet le curseur de lecture au début du fichier
    fichier.clear();
    fichier.seekg(0);

    // Si on ne trouve pas de ';', on suppose que c'est un format vertical (une info par ligne)
    bool estFormatVertical = (premiereLigne.find(';') == std::string::npos);

    int compteur = 0;
    
    // CAS 1 : C'est un CSV standard (avec point-virgule)
    if (!estFormatVertical) {
        std::string ligne; 
        std::getline(fichier, ligne); // On ignore la ligne d'en-tête (Titres des colonnes)

        while (std::getline(fichier, ligne)) {
            if (ligne.empty()) continue;
            std::vector<std::string> data = splitLigne(ligne, ';');
            
            if (data.size() >= 6) {
                Book b;
                b.isbn = data[0];
                b.title = data[1];
                b.language = data[2];
                b.authors = data[3];
                b.date = data[4];
                b.genre = data[5];
                if (data.size() > 6) b.description = data[6];
                
                // Petit nettoyage : si la description est entourée de guillemets "", on les enlève
                if (b.description.size() >= 2 && b.description.front() == '"') {
                    b.description = b.description.substr(1, b.description.size()-2);
                }

                // On n'ajoute le livre que s'il n'existe pas déjà (pas de doublons)
                if (!isbnExiste(lib, b.isbn)) {
                    lib.books.push_back(b);
                    compteur++;
                }
            }
        }
    } 
    // CAS 2 : Lecture verticale (pour compatibilité avec d'anciens fichiers)
    else {
        Book b;
        // On lit champ par champ, ligne par ligne
        while (std::getline(fichier, b.isbn)) { 
            if (b.isbn.empty()) continue;
            std::getline(fichier, b.title);
            std::getline(fichier, b.language);
            std::getline(fichier, b.authors);
            std::getline(fichier, b.date);
            std::getline(fichier, b.description);
            std::getline(fichier, b.genre);       

            if (!isbnExiste(lib, b.isbn)) {
                lib.books.push_back(b);
                compteur++;
            }
        }
    }

    // NOTE IMPORTANTE : On ne sauvegarde PAS automatiquement ici.
    // L'utilisateur doit choisir de sauvegarder en quittant le menu.
    // Cela permet d'annuler l'importation si on s'est trompé.
    
    return compteur;
}

// === EXPORT HTML ===

// Nettoie le titre pour le tri alphabétique (enlève "Le", "La", met en majuscule...)
std::string nettoyerTitrePourTri(std::string titre) {
    // 1. Mettre en majuscules
    std::string s = titre;
    for (auto & c: s) c = toupper(c);

    // 2. Liste des préfixes à ignorer pour le tri
    const std::vector<std::string> prefixes = {
        "LE ", "LA ", "L'", "LES ", "UN ", "UNE ", "DES ", 
        "D'", "J'", "QU'", "S'"
    };

    // 3. Si le titre commence par un préfixe, on l'ignore
    for (const auto& p : prefixes) {
        if (s.substr(0, p.size()) == p) {
            return s.substr(p.size()); // On retourne la suite du titre
        }
    }
    return s;
}

// Fonction de comparaison utilisée par std::sort
bool comparerLivres(const Book& a, const Book& b) {
    return nettoyerTitrePourTri(a.title) < nettoyerTitrePourTri(b.title);
}

// === FONCTION PRINCIPALE D'EXPORT ===

void exporterHTML(const Library& lib, const std::string& filename) {
    // 1. On crée une copie locale de la liste pour la trier
    // (on ne veut pas changer l'ordre dans l'application)
    std::vector<Book> livresTries = lib.books;
    
    // 2. On trie cette liste avec notre règle spéciale
    std::sort(livresTries.begin(), livresTries.end(), comparerLivres);

    std::ofstream fichier(filename);
    if (!fichier) {
        std::cerr << "Erreur lors de la création du fichier HTML" << std::endl;
        return;
    }

    // 2. Écriture de l'en-tête HTML standard
    fichier << "<!DOCTYPE html>" << std::endl;
    fichier << "<html lang='fr'>" << std::endl;
    fichier << "<head>" << std::endl;
    fichier << "<meta charset='UTF-8'>" << std::endl;
    fichier << "<meta name='viewport' content='width=device-width, initial-scale=1.0'>" << std::endl;
    fichier << "<title>" << lib.name << " - Catalogue</title>" << std::endl;
    
    // CSS Intégré (Pour que le fichier HTML soit autonome et joli)
    fichier << "<style>" << std::endl;
    fichier << "body { font-family: sans-serif; background-color: #f4f4f9; color: #333; margin: 20px; }" << std::endl;
    fichier << ".container { max-width: 900px; margin: 0 auto; background: white; padding: 20px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }" << std::endl;
    fichier << "h1 { text-align: center; color: #2c3e50; }" << std::endl;
    fichier << ".subtitle { text-align: center; color: #7f8c8d; font-style: italic; margin-bottom: 30px; }" << std::endl;
    
    // Style de la barre d'index (A B C D...)
    fichier << ".index-bar { text-align: center; margin-bottom: 20px; }" << std::endl;
    fichier << ".index-bar a { display: inline-block; padding: 5px 10px; margin: 2px; text-decoration: none; color: white; background-color: #3498db; border-radius: 4px; }" << std::endl;
    
    // Style des cartes de livres
    fichier << "h2 { border-bottom: 2px solid #3498db; color: #3498db; margin-top: 30px; }" << std::endl;
    fichier << ".livre-card { border-left: 5px solid #3498db; padding: 10px 15px; margin-bottom: 15px; background: #f9f9f9; }" << std::endl;
    fichier << ".livre-titre { font-weight: bold; font-size: 1.1em; }" << std::endl;
    fichier << ".livre-infos { font-size: 0.9em; color: #555; }" << std::endl;
    fichier << "</style>" << std::endl;
    fichier << "</head>" << std::endl;
    
    fichier << "<body>" << std::endl;
    fichier << "<div class='container'>" << std::endl;
    fichier << "<h1>" << lib.name << "</h1>" << std::endl;
    fichier << "<p class='subtitle'>" << lib.description << "</p>" << std::endl;

    // 3. Génération de l'index Alphabétique
    fichier << "<div class='index-bar'>" << std::endl;
    std::string lettresPresentes = "";
    // On repère quelles lettres sont utilisées
    for (const auto& livre : livresTries) {
        std::string cle = nettoyerTitrePourTri(livre.title);
        char premiereLettre = cle[0];
        if (!isalpha(premiereLettre)) premiereLettre = '#';
        if (lettresPresentes.find(premiereLettre) == std::string::npos) {
            lettresPresentes += premiereLettre;
        }
    }
    // On affiche les liens
    std::string alphabet = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (char c : alphabet) {
        if (lettresPresentes.find(c) != std::string::npos) {
            fichier << "<a href='#section-" << c << "'>" << c << "</a>";
        } else {
            fichier << "<span>" << c << "</span>";
        }
    }
    fichier << "</div>" << std::endl;

    // 4. Génération du contenu (Livres)
    char sectionActuelle = 0;
    for (const auto& livre : livresTries) {
        std::string cle = nettoyerTitrePourTri(livre.title);
        char lettre = cle[0];
        if (!isalpha(lettre)) lettre = '#';

        // Si on change de lettre, on crée une nouvelle section
        if (lettre != sectionActuelle) {
            sectionActuelle = lettre;
            fichier << "<h2 id='section-" << sectionActuelle << "'>" << sectionActuelle << "</h2>" << std::endl;
        }

        // Affichage du livre
        fichier << "<div class='livre-card'>" << std::endl;
        fichier << "<div class='livre-titre'>" << livre.title << "</div>" << std::endl;
        fichier << "<div class='livre-infos'>" << std::endl;
        fichier << "Par <strong>" << livre.authors << "</strong> &bull; ";
        fichier << "ISBN: " << livre.isbn << " &bull; ";
        fichier << livre.date << std::endl;
        fichier << "</div>" << std::endl;
        fichier << "</div>" << std::endl;
    }

    fichier << "</div>" << std::endl; // Fin container

    fichier << "</body></html>" << std::endl;
}