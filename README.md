# BE_POO_Food_Dispenser
Ce projet est un système embarqué de distribution automatique de nourriture capable de s'adapter à différents types d'animaux (Chat, Chien, Cheval, Baleine, Souris). Il utilise une architecture logicielle modulaire basée sur la POO pour gérer les spécificités de chaque espèce, les horaires de repas et l'interface utilisateur.

# Matériel Requis
Microcontrôleur : ESP32 (Dev Module).

Moteur : Moteur pas à pas (Nema 17) & Driver A4988.

Écran : Écran TFT circulaire GC9A01.

Capteur de distance : VL53L0X (I2C).

Contrôles : 4 Boutons tactiles (Module TTP224).

Alimentation : 12V (Moteur) converti en 5V (ESP32).

# Pinout
| Composant | Pin ESP32 | Fonction |
| --------- | --------- | -------- |
| Moteur (A4988) | GPIO 12 | DIR |
|                | GPIO 13 | STEP |
|                | GPIO 16 | ENABLE |
| Capteur (VL53L0X) |	GPIO 27 |	SDA |
|                   | GPIO 14 |	SCL |
|Boutons | GPIO 32 | Retour (Back) |
|        | GPIO 33 | Bas (Down) |
|        | GPIO 25 | Haut (Up) |
|        | GPIO 26 | Valider (Enter) |

# Installation et configuration
Ce projet a été développé avec l'Arduino IDE.


1) Assurez-vous d'avoir installé le gestionnaire de cartes pour ESP32 dans l'IDE Arduino.
2) Vous devez aussi installer les bibliothèques suivantes via le gestionnaire de bibliothèques :
   + TFT_eSPI par Bodme (Version 2.5.43) **Attention, il faut modifier le fichier `User_Setup.h` pour correspondre à notre écran**
   + VL53L0X par Pololu (Version 1.3.1)
3) Les bibliothèques suivantes sont natives à l'ESP32 et n'ont pas besoin d'installation :
    + WiFi
    + Preferences
    + Wire (I2C)
    + SPI
## Configuration WiFi
Avant de téléverser, ouvrez le fichier `Secret.h` et modifiez vos identifiants WiFi :
```
#define SECRET_SSID "VOTRE_SSID"
#define SECRET_PASSWORD "VOTRE_MOT_DE_PASSE"
```
## Téléversement
1) Ouvrez le fichier `MainDistrib.ino`.
2) Sélectionnez votre carte ESP32 et le port COM.
3) Compilez et téléversez.

# Utilisation
À l'allumage, le distributeur se connecte au WiFi pour récupérer l'heure, charge les données sauvegardées et affiche le tableau de bord.  
## Navigation
Bouton 1 (Retour) : Revenir en arrière / Annuler.  
Bouton 2 (Bas) : Descendre dans le menu / Curseur droite.  
Bouton 3 (Haut) : Monter dans le menu / Curseur gauche.  
Bouton 4 (Entrée) : Valider / Entrer dans un sous-menu.

## Menus
Horaires : Définir les heures de repas (Format HH:MM).  
Animal : Configurer le profil (Nom, Espèce, Poids, Taille, Comportement).  
Distrib Manuel : Forcer une distribution immédiate (sécurisée par le capteur de niveau).


# Auteurs
Léo Lefebvre & Estelle Coudon
