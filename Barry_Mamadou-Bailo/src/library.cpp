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
#include <fstream>
#include <vector>
#include <string>
#include "library.hpp"
#include "utils.hpp" // Pour les couleurs et clearScreen
#include <algorithm> // Pour std::sort
#include <sstream>   // Pour std::istringstream

// Fonction interne pour lire un livre depuis le flux
bool lireLivre(std::ifstream& fichier, Book& livre) {
    std::string ligne;
    
    // On lit l'ISBN
    if (!std::getline(fichier, livre.isbn)) return false;
    
    // Si la ligne est vide (fin de fichier potentielle), on arrête
    if (livre.isbn.empty()) return false;

    std::getline(fichier, livre.title);
    std::getline(fichier, livre.language);
    std::getline(fichier, livre.authors);
    std::getline(fichier, livre.date);
    std::getline(fichier, livre.genre);
    
    // Pour la description, le sujet dit "plusieurs lignes". 
    // Pour simplifier le stockage texte simple : on va dire que la description tient sur une ligne dans la DB 
    // (ou on utilise un marqueur spécial, mais restons simple pour l'instant).
    std::getline(fichier, livre.description);
    
    return true;
}

// Fonction utilitaire pour découper une ligne CSV (par point-virgule)
    std::vector<std::string> splitLigne(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    if (!s.empty() && s.back() == delimiter) {
        tokens.push_back("");
    }
    return tokens;
}

// Fonction pour nettoyer le texte avant sauvegarde (Retire les ; et les \n)
std::string nettoyerTexte(std::string s) {
    std::replace(s.begin(), s.end(), ';', ','); // Remplace les ; par des ,
    std::replace(s.begin(), s.end(), '\n', ' '); // Remplace les sauts de ligne par des espaces
    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end()); // Retire les retours chariot
    return s;
}

bool chargerBibliotheque(Library& lib, const std::string& filename) {
    std::ifstream fichier(filename);
    if (!fichier.is_open()) {
        return false; // Le fichier n'existe pas
    }
       
    

    // 1. On lit explicitement la première ligne (Le Nom)
    if (!std::getline(fichier, lib.name)) lib.name = "Ma Bibliotheque";

    // 2. On lit explicitement la deuxième ligne (La Description)
    // C'est ça qui évitera que la description soit prise pour un livre !
    if (!std::getline(fichier, lib.description)) lib.description = "Description par défaut";

    // 3. Ensuite, on boucle pour les livres
    lib.books.clear();
    std::string line;
    while (std::getline(fichier, line)) {
        if (line.empty()) continue; // On ignore les lignes vides

        std::vector<std::string> data = splitLigne(line, ';');
        
        // On vérifie qu'on a bien assez de colonnes pour créer un livre
        if (data.size() >= 6) {
            Book b;
            b.isbn = data[0];
            b.title = data[1];
            b.language = data[2];
            b.authors = data[3];
            b.date = data[4];
            b.genre = data[5];
            // La description est optionnelle ou peut être la dernière colonne
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
        fichier <<nettoyerTexte(lib.name) << std::endl;
        fichier << nettoyerTexte(lib.description) << std::endl;

        // Boucle for-each (C++11) pour parcourir le vecteur
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
        std::cerr << "Erreur : Impossible d'ecrire dans le fichier " << filename << std::endl;
    }
}

void initialiserBibliotheque(Library& lib) {
    lib.name = "Ma Bibliotheque";
    lib.description = "Gestionnaire de livres personnel";
    lib.books.clear();


}

bool isbnExiste(const Library& lib, const std::string& isbn) {
    // On parcourt tous les livres pour voir si l'ISBN correspond
    for (const auto& livre : lib.books) {
        if (livre.isbn == isbn) {
            return true;
        }
    }
    return false;
}

void ajouterLivre(Library& lib, const Book& nouveauLivre) {
    lib.books.push_back(nouveauLivre);
    // On pourrait trier ici, mais on le fera plus tard si besoin
}

void supprimerToutesReferences(Library& lib) {
    lib.books.clear(); // Vide le vecteur
    // On sauvegarde tout de suite pour acter le changement dans le fichier
    sauvegarderBibliotheque(lib, "library.db");
}
// Importe les références depuis un CSV
int importerReferences(Library& lib, const std::string& filename) {
    std::ifstream fichier(filename);
    if (!fichier) return -1; // Erreur fichier

    // Détection rapide du format du fichier importé
    // On regarde la première ligne pour voir si c'est un CSV (avec ;) ou vertical
    std::string premiereLigne;
    std::getline(fichier, premiereLigne);
    
    // On remet le curseur au début
    fichier.clear();
    fichier.seekg(0);

    bool estFormatVertical = (premiereLigne.find(';') == std::string::npos);

    int compteur = 0;
    
    // Si c'est un CSV standard (avec point-virgule)
    if (!estFormatVertical) {
        std::string ligne;
        // On ignore la première ligne si c'est des titres de colonnes
        std::getline(fichier, ligne); 

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
                
                // Nettoyage spécifique pour import
                if (b.description.size() >= 2 && b.description.front() == '"') {
                    b.description = b.description.substr(1, b.description.size()-2);
                }

                if (!isbnExiste(lib, b.isbn)) {
                    lib.books.push_back(b);
                    compteur++;
                }
            }
        }
    } 
    // Sinon, on tente une lecture verticale (l'ancien format)
    else {
        Book b;
        while (std::getline(fichier, b.isbn)) { // Ligne 1 : ISBN
            if (b.isbn.empty()) continue;
            std::getline(fichier, b.title);
            std::getline(fichier, b.language);
            std::getline(fichier, b.authors);
            std::getline(fichier, b.date);
            std::getline(fichier, b.description); // Ligne 6 : Desc
            std::getline(fichier, b.genre);       // Ligne 7 : Genre (Attention l'ordre variait)

            // Petit fix si le genre a été lu dans la desc
            // (Adaptation simple pour récupérer un maximum de données)
            
            if (!isbnExiste(lib, b.isbn)) {
                lib.books.push_back(b);
                compteur++;
            }
        }
    }    
    return compteur;
}

// === FONCTIONS UTILITAIRES POUR L'EXPORT HTML ===

// Nettoie le titre pour le tri (enlève les articles, met en majuscule)
std::string nettoyerTitrePourTri(std::string titre) {
    // 1. Mettre en majuscules
    std::string s = titre;
    for (auto & c: s) c = toupper(c);

    // 2. Liste des préfixes à ignorer (selon le sujet)
    const std::vector<std::string> prefixes = {
        "LE ", "LA ", "L'", "LES ", "UN ", "UNE ", "DES ", 
        "D'", "J'", "QU'", "S'"
    };

    // 3. Si le titre commence par un préfixe, on l'enlève
    for (const auto& p : prefixes) {
        if (s.substr(0, p.size()) == p) {
            return s.substr(p.size()); // On retourne la suite du titre
        }
    }
    return s; // Sinon on retourne le titre tel quel
}

// Fonction de comparaison pour le tri
bool comparerLivres(const Book& a, const Book& b) {
    return nettoyerTitrePourTri(a.title) < nettoyerTitrePourTri(b.title);
}

// === FONCTION PRINCIPALE D'EXPORT ===

void exporterHTML(const Library& lib, const std::string& filename) {
    // 1. On crée une copie de la liste des livres pour pouvoir la trier
    // sans perturber l'ordre de la bibliothèque principale.
    std::vector<Book> livresTries = lib.books;
    
    // 2. On trie cette liste avec notre règle spéciale
    std::sort(livresTries.begin(), livresTries.end(), comparerLivres);

    std::ofstream fichier(filename);
    if (!fichier) {
        std::cerr << "Erreur creation HTML" << std::endl;
        return;
    }

    // 3. Écriture de l'en-tête HTML
    fichier << "<!DOCTYPE html>" << std::endl;
    fichier << "<html lang='fr'>" << std::endl;
    fichier << "<head>" << std::endl;
    fichier << "<meta charset='UTF-8'>" << std::endl;
    fichier << "<meta name='viewport' content='width=device-width, initial-scale=1.0'>" << std::endl;
    fichier << "<title>" << lib.name << " - Catalogue</title>" << std::endl;
    
    // CSS INTEGRE (Pas de fichier externe, conforme au sujet)
    fichier << "<style>" << std::endl;
    fichier << "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f4f4f9; color: #333; margin: 0; padding-bottom: 50px; }" << std::endl;
    fichier << ".container { max-width: 900px; margin: 0 auto; padding: 20px; }" << std::endl;
    
    // Header et Index
    fichier << "h1 { text-align: center; color: #2c3e50; margin-bottom: 10px; }" << std::endl;
    fichier << ".subtitle { text-align: center; color: #7f8c8d; margin-bottom: 30px; font-style: italic; }" << std::endl;
    
    fichier << ".index-bar { position: sticky; top: 0; background: white; padding: 15px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); text-align: center; z-index: 100; }" << std::endl;
    fichier << ".index-bar a { display: inline-block; padding: 5px 10px; margin: 2px; text-decoration: none; color: #fff; background-color: #3498db; border-radius: 4px; font-weight: bold; font-size: 14px; transition: background 0.3s; }" << std::endl;
    fichier << ".index-bar a:hover { background-color: #2980b9; }" << std::endl;
    fichier << ".index-bar span { display: inline-block; padding: 5px 10px; margin: 2px; color: #bdc3c7; font-size: 14px; }" << std::endl;

    // Sections et Livres
    fichier << "h2 { border-bottom: 2px solid #3498db; color: #3498db; padding-bottom: 5px; margin-top: 40px; }" << std::endl;
    
    fichier << ".livre-card { background: white; border-left: 5px solid #3498db; padding: 15px; margin-bottom: 15px; box-shadow: 0 2px 4px rgba(0,0,0,0.05); border-radius: 0 5px 5px 0; }" << std::endl;
    fichier << ".livre-titre { font-size: 1.2em; font-weight: bold; color: #2c3e50; }" << std::endl;
    fichier << ".livre-infos { color: #7f8c8d; font-size: 0.9em; margin-top: 5px; }" << std::endl;
    fichier << ".livre-isbn { font-family: monospace; background: #eee; padding: 2px 5px; border-radius: 3px; }" << std::endl;
    fichier << "</style>" << std::endl;
    fichier << "</head>" << std::endl;
    
    fichier << "<body>" << std::endl;

    // INDEX
    fichier << "<div class='index-bar'>" << std::endl;
    std::string lettresPresentes = "";
    for (const auto& livre : livresTries) {
        std::string cle = nettoyerTitrePourTri(livre.title);
        char premiereLettre = cle[0];
        if (!isalpha(premiereLettre)) premiereLettre = '#';
        if (lettresPresentes.find(premiereLettre) == std::string::npos) {
            lettresPresentes += premiereLettre;
        }
    }
    std::string alphabet = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (char c : alphabet) {
        if (lettresPresentes.find(c) != std::string::npos) {
            fichier << "<a href='#section-" << c << "'>" << c << "</a>";
        } else {
            fichier << "<span>" << c << "</span>";
        }
    }
    fichier << "</div>" << std::endl; // Fin index

    fichier << "<div class='container'>" << std::endl;
    fichier << "<h1>" << lib.name << "</h1>" << std::endl;
    fichier << "<p class='subtitle'>" << lib.description << "</p>" << std::endl;

    // CONTENU
    char sectionActuelle = 0;
    for (const auto& livre : livresTries) {
        std::string cle = nettoyerTitrePourTri(livre.title);
        char lettre = cle[0];
        if (!isalpha(lettre)) lettre = '#';

        if (lettre != sectionActuelle) {
            sectionActuelle = lettre;
            fichier << "<h2 id='section-" << sectionActuelle << "'>" << sectionActuelle << "</h2>" << std::endl;
        }

        // Carte de livre
        fichier << "<div class='livre-card'>" << std::endl;
        fichier << "<div class='livre-titre'>" << livre.title << "</div>" << std::endl;
        fichier << "<div class='livre-infos'>" << std::endl;
        fichier << "Par <strong>" << livre.authors << "</strong> &bull; ";
        fichier << "ISBN: <span class='livre-isbn'>" << livre.isbn << "</span> &bull; ";
        fichier << livre.date << std::endl;
        fichier << "</div>" << std::endl;
        // On peut ajouter le résumé si tu veux, mais le sujet montrait une liste simple.
        // fichier << "<p style='font-size:0.9em; margin-top:10px;'>" << livre.description << "</p>" << std::endl;
        fichier << "</div>" << std::endl;
    }
    fichier << "</div>" << std::endl; // Fin container

    fichier << "</body></html>" << std::endl;
}