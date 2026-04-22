



--------    Description du projet : Station d'Arrosage automatique ------

Ce projet consiste en la conception matérielle et logicielle d'une station d'arrosage automatisée fonctionnant sur batterie. 
Le système repose sur un microcontrôleur ESP32 (ESP-WROOM-32). Il pilote jusqu'à 4 électrovannes de manière séquentielle et asynchrone pour optimiser la pression hydraulique. 
Le système est doté d'une "intelligence environnementale" : il ajuste les durées d'arrosage en fonction de la température et de l'humidité (capteur DHT22) et annule les cycles 
en cas de précipitations (pluviomètre géré par le coprocesseur ULP en mode Deep Sleep).
L'interface homme-machine est assurée localement par un écran TFT et 4 boutons poussoirs tactiles.


---------   La définition de l'Unité Fonctionnelle (UF) ------------

Quel est l'impact environnemental d'un système d'arrosage automatisé permettant d'irriger une surface de 500 m² sur une durée de 3 ans ?

