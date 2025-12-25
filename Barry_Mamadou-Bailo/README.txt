========================================================================
PROJET  : GESTION DE BIBLIOTHÈQUE NUMÉRIQUE (C++)
AUTEUR  : Barry Mamadou Bailo
DATE    : Janvier 2026
VERSION : 1.0
========================================================================

1. PRÉSENTATION
---------------
Bienvenue dans mon application de gestion de bibliothèque. Ce projet a été 
conçu pour offrir une solution robuste et conviviale pour gérer une collection 
de livres via une interface en ligne de commande (CLI).

L'accent a été mis sur la persistance des données, la robustesse face aux 
erreurs de saisie et l'ergonomie (pagination, menus clairs, code couleurs).

2. FONCTIONNALITÉS CLÉS
-----------------------
- [x] Persistance des données : Sauvegarde automatique et manuelle (fichier library.db).
- [x] Importation CSV : Capacité de charger des données en masse avec validation.
- [x] Navigation avancée : Affichage paginé des livres (Page Suivante/Précédente).
- [x] Moteur de recherche : Filtrage par ISBN, Titre ou Code Éditeur.
- [x] Export Web : Génération d'un catalogue HTML complet avec index alphabétique et CSS intégré.
- [x] Robustesse : Validation stricte des dates (ex: gestion des années bissextiles) et des entrées.
- [x] Interface : Utilisation de codes ANSI pour une interface colorée et lisible.

3. CONTENU DE L'ARCHIVE
-----------------------
L'organisation du projet respecte les standards de séparation du code :

📂 Barry_Mamadou_Bailo/
├── 📄 Makefile          : Script de compilation automatisé
├── 📄 README.txt        : Ce fichier de documentation
├── 📂 src/              : Code source (.cpp) - Logique métier et implémentation
├── 📂 include/          : En-têtes (.hpp) - Déclarations et structures
└── 📂 tests/            : Environnement de test
    ├── 📜 test.exp      : Script d'automatisation (Tcl/Expect)
    └── 📄 SCENARIO.txt  : Description détaillée des cas de test

4. PRÉREQUIS TECHNIQUES
-----------------------
- Système : Linux / Unix (ou WSL sous Windows)
- Compilateur : g++ (compatible C++17 ou supérieur)
- Outil de build : make
- Pour les tests (optionnel) : expect (paquet 'expect')

5. COMPILATION ET NETTOYAGE
---------------------------
Le projet utilise un Makefile pour simplifier la gestion.

> Pour compiler le projet (génère l'exécutable 'app') :
    $ make

> Pour nettoyer les fichiers temporaires (.o) et l'exécutable :
    $ make clean

6. UTILISATION
--------------
Une fois compilé, lancez l'application :
    $ ./app

Laissez-vous guider par les menus interactifs.
Note : Si aucun fichier 'library.db' n'est trouvé, l'application lancera 
automatiquement un assistant de configuration initiale.

7. JEUX DE TESTS AUTOMATISÉS (Point Fort)
-----------------------------------------
Plutôt que de tester manuellement chaque fonctionnalité à chaque modification,
j'ai développé un script complet avec 'Expect'.

Ce script simule un utilisateur réel qui :
1. Initialise la bibliothèque.
2. Tente de faire planter l'appli (erreurs de date, quitter sans sauver).
3. Importe 15 livres automatiquement.
4. Teste la pagination et la recherche.
5. Vérifie que la sauvegarde fonctionne réellement.

> Pour lancer la démonstration automatique :
    $ make test

(Voir le fichier tests/SCENARIO.txt pour le détail étape par étape).

8. CHOIX D'IMPLÉMENTATION (Note de l'auteur)
--------------------------------------------
- Structures de données : Utilisation de `std::vector<Book>` pour la flexibilité.
- Flux de fichiers : Utilisation de `std::ifstream` et `std::ofstream` pour la gestion CSV/DB.
- Algo : Utilisation de `std::sort` avec un comparateur personnalisé pour l'export HTML.
- UX : J'ai choisi de ne pas utiliser de bibliothèques externes complexes (comme ncurses) 
  mais de gérer l'affichage proprement avec les codes d'échappement ANSI standards.

Merci de l'intérêt porté à ce projet !