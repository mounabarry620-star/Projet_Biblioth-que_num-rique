/**
 * @file config.hpp
 * @brief Gestion de la configuration persistante de l'application.
 * @author Barry Mamadou Bailo
 * @version 1.0
 *
 * Gère le chargement et la sauvegarde du fichier 'app.conf', incluant
 * le logo ASCII personnalisé et les préférences d'affichage (pagination).
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>

// Structure pour regrouper tous les paramètres de l'application.
// Cela permet de passer une seule variable 'config' aux fonctions au lieu de plusieurs.
struct AppConfig {
    int livresParPage = 5;       // Valeur par défaut si aucun fichier de config n'est trouvé
    std::string logo;            // Le dessin (logo ASCII) affiché en haut du menu
};

// Charge la config depuis le fichier texte.
// Retourne 'false' si le fichier n'existe pas (ce qui permet de savoir s'il faut créer celui par défaut).
// Le paramètre 'config' est passé par référence (&) pour être modifié directement par la fonction.
bool chargerConfig(AppConfig& config, const std::string& filename);

// Remplit la structure 'config' avec les valeurs d'origine (le logo du sujet).
void creerConfigDefaut(AppConfig& config);

// Sauvegarde la configuration actuelle dans un fichier.
// Le paramètre est 'const' car la sauvegarde ne doit pas modifier les données, juste les lire.
void sauvegarderConfig(const AppConfig& config, const std::string& filename);

// Fonction utilitaire pour remettre uniquement le logo à zéro sans toucher au reste (ex: pagination).
void reinitialiserLogo(AppConfig& config);

#endif