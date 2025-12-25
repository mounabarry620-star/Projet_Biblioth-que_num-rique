========================================================================
PROJET : GESTION DE BIBLIOTHÈQUE NUMÉRIQUE
Auteur : Barry Mamadou Bailo
Date   : Janvier 2026
========================================================================

DESCRIPTION
-----------
Cette application permet de gérer une bibliothèque de livres en C++.
Elle permet d'ajouter, consulter, importer (CSV), rechercher et exporter (HTML)
des références bibliographiques. Les données sont persistantes (fichier library.db).

CONTENU DE L'ARCHIVE
--------------------
- src/      : Les fichiers sources (.cpp)
- include/  : Les fichiers d'en-tête (.hpp)
- tests/    : Le script de test automatique et le scénario
- Makefile  : Le fichier de compilation automatisée
- README.txt: Ce fichier d'explications

PRÉREQUIS
---------
- Compilateur g++ (supportant C++17)
- Utilitaire 'make'
- Pour les tests automatiques : 'expect' (sudo apt install expect)

COMPILATION
-----------
Ouvrez un terminal dans le dossier du projet et lancez :
    make

Cela créera un exécutable nommé 'app'.

UTILISATION
-----------
Pour lancer l'application :
    ./app

Une fois dans l'application, utilisez les touches numériques pour naviguer
dans les menus.
Laissez-vous guider par les instructions à l'écran.

JEUX DE TESTS (AUTOMATISÉS)
---------------------------
J'ai conçu un script de test complet qui simule un utilisateur pour vérifier
toutes les fonctionnalités (Ajout, Import, Navigation, Sauvegarde, Erreurs).

Pour lancer la batterie de tests, exécutez simplement :
    make test

(Ou manuellement : ./tests/test.exp)

Le scénario détaillé des tests se trouve dans : tests/SCENARIO.txt

NETTOYAGE
---------
Pour supprimer les fichiers objets (.o) et l'exécutable :
    make clean