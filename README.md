Ce dépôt est organisé de manière à séparer la documentation de conception du code source technique. Pour naviguer efficacement, le projet utilise deux branches distinctes.

## Organisation des branches

- **Branche main**: Centralise la documentation, les rapports de projet et les études d'impact.
- **Branche partie-logicielle**: Contient exclusivement le code source et les fichiers de configuration.

## Contenu de la branche Main

- **DOCS**: Ressources techniques et rapports des membres du groupe par séance.
- **ecoconception**: Analyse du cycle de vie et choix techniques pour minimiser l'empreinte énergétique.
- **Hardware-pcb-eagle**: Schémas électroniques et fichiers PCB réalisés sous Eagle.
- **ProjetS7.pdf**: Présentation S8.
- **RAPPORT.pdf**: Rapport de synthèse présentant les objectifs et résultats du semestre 7 et 8.

## Architecture logicielle

Le code est segmenté par modules fonctionnels(exemple) :
- **Watering_System.ino**: Point d'entrée principal du programme.
- **SensorManagment (.cpp/.h)**: Logique de traitement des capteurs.
- **VanneCase (.h)**: Gestion des états de l'électrovanne.
- **ModuleHorloge (.cpp/.h)**: Gestion du temps réel (RTC).
