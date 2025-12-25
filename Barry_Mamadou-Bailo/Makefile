# ==============================================================================
# FICHIER : Makefile
# PROJET  : Gestion de Bibliothèque Numérique
# AUTEUR  : Barry Mamadou Bailo
# DATE    : Janvier 2026
# ==============================================================================

# --- 1. CONFIGURATION DU PROJET ---

# Nom de l'exécutable final
TARGET = app

# Compilateur et options
# -Wall -Wextra : Active tous les avertissements (bonnes pratiques)
# -std=c++17    : Utilise le standard C++17 moderne
# -Iinclude     : Indique au compilateur où trouver les fichiers .hpp
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

# Structure des dossiers
SRCDIR = src
OBJDIR = build
TESTDIR = tests

# --- 2. DÉTECTION AUTOMATIQUE DES FICHIERS ---

# Liste tous les fichiers .cpp présents dans le dossier src/
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# Déduit la liste des fichiers objets .o correspondants dans le dossier build/
# (Transforme src/xxx.cpp en build/xxx.o)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# --- 3. RÈGLES DE COMPILATION ---

# Règle par défaut (lancée par la commande "make")
all: $(TARGET)

# Édition de liens (Linking) : Crée l'exécutable à partir des objets
# $@ représente la cible (TARGET) et $^ représente les dépendances (OBJECTS)
$(TARGET): $(OBJECTS)
	@echo "🔨 Création de l'exécutable $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✅ Compilation terminée avec succès ! Lancez avec : ./$(TARGET)"

# Compilation séparée : Crée un fichier .o pour chaque fichier .cpp
# $< représente le fichier source (.cpp)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@# Crée le dossier build s'il n'existe pas
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- 4. TESTS ET NETTOYAGE ---

# Règle pour lancer les tests automatisés (nécessite l'exécutable)
test: $(TARGET)
	@echo "🧪 Lancement du scénario de test automatique..."
	@# Rend le script exécutable (au cas où)
	@chmod +x $(TESTDIR)/test.exp
	@# Exécute le script Expect
	@$(TESTDIR)/test.exp

# Nettoyage complet pour rendre un dossier propre (Consigne du prof)
# Supprime l'exécutable, les objets de compilation, et les fichiers générés par l'app
clean:
	@echo "🧹 Nettoyage complet des fichiers..."
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
	rm -f library.db
	rm -f app.conf
	rm -f catalogue.html
	rm -f $(TESTDIR)/livres-lot-2.csv
	@echo "✨ Dossier propre (Prêt pour l'archivage)."

# .PHONY indique que ces règles ne correspondent pas à des fichiers réels
.PHONY: all clean test