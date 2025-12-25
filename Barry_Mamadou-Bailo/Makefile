# Nom de l'exécutable
TARGET = app

# Compilateur et options
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

# Dossiers
SRCDIR = src
OBJDIR = build
TESTDIR = tests

# Liste automatique des fichiers sources .cpp (sauf ceux supprimés)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# Liste automatique des fichiers objets .o correspondants
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Règle par défaut (ce qui se lance quand on tape juste "make")
all: $(TARGET)

# Édition de liens (Création de l'exécutable)
$(TARGET): $(OBJECTS)
	@echo "====== Création de l'exécutable... ======"
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "====== Compilation terminée ! Lancez avec ./$(TARGET) ======"

# Compilation des fichiers objets (.cpp -> .o)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour lancer les tests
test: $(TARGET)
	@echo "====== Lancement du test automatique... ======"
	@# On rend le script exécutable
	@chmod +x $(TESTDIR)/test.exp
	@# On l'exécute
	@$(TESTDIR)/test.exp

# Nettoyage complet (Conforme aux exigences du prof)
clean:
	@echo "====== Nettoyage des fichiers...====="
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
	rm -f library.db
	rm -f app.conf
	rm -f catalogue.html
	rm -f $(TESTDIR)/livres-lot-2.csv
	@echo "====== Dossier propre. ======"

.PHONY: all clean test