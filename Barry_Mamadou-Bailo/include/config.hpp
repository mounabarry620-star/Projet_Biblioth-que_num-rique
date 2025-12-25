/**
 * @file config.cpp
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

struct AppConfig {
    int livresParPage = 5;       // Valeur par défaut
    std::string logo;            // Le logo ASCII
};

// Charge la config depuis le fichier. Retourne false si le fichier n'existe pas.
bool chargerConfig(AppConfig& config, const std::string& filename);

// Crée une config par défaut (avec le logo du sujet)
void creerConfigDefaut(AppConfig& config);

// Sauvegarde la config dans le fichier
void sauvegarderConfig(const AppConfig& config, const std::string& filename);
// Réinitialise le logo ASCII aux valeurs par défaut
void reinitialiserLogo(AppConfig& config);

#endif