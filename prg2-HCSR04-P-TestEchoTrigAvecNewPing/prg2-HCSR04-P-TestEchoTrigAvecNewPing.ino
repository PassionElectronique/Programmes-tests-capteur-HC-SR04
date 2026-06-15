// ===================================================================================================
//   ______               _                  _///_ _           _                   _
//  /   _  \             (_)                |  ___| |         | |                 (_)
//  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
//  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
//  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
//  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
//                                                                                      | |
//                                                                                      \_|
// ===================================================================================================
//
//  Nom Projet  :       Tuto HC-SR04P (capteur à ultrasons)
//  Desc Projet :       Programme de test capteur HC-SR04-P utilisant les lignes ECHO et TRIG,
//                      en utilisant la librairie "newping"
//
//  Fichier     :       prg2-HCSR04-P-TestEchoTrigAvecNewPing.ino
//  Créé le     :       08.05.2026
//  Auteur      :       Jérôme TOMSKI
//  Site        :       https://passionelectronique.fr/
//  GitHub      :       https://github.com/PassionElectronique/Programmes-tests-capteur-HC-SR04
//  Licence     :       https://creativecommons.org/licenses/by-nc-nd/4.0/deed.fr (BY-NC-ND 4.0 CC)
//
// ===================================================================================================

// Inclusion des librairies nécessaires
#include <NewPing.h>    // À installer à partir du menu Outils > Gérer les bibliothèques (nom de la librairie = "NewPing" de Tim Eckel)

// Constantes
#define brochePourLigneECHO           2   // La broche D2 du microcontrôleur sera reliée à la ligne ECHO du HC-SR04 P
#define brochePourLigneTRIG           3   // La broche D3 du microcontrôleur sera reliée à la ligne TRIG du HC-SR04 P
#define distanceMaxiEnCentimetres   400   // Distance maximale qu'on "s'autorise" à mesurer, en centimètres

// Création d'un objet, arbitrairement nommé 'monSonar', pour contrôler notre capteur HC-SR04P
NewPing monSonar(brochePourLigneTRIG, brochePourLigneECHO, distanceMaxiEnCentimetres);


// ========================
// Initialisation programme
// ========================
void setup() {

  // Démarrage du moniteur série
  Serial.begin(9600);
  Serial.println(F("======================================================="));
  Serial.println(F("Programme #2 : mesures de distances toutes les secondes"));
  Serial.println(F("               avec le capteur HC-SR04, en utilisant la"));
  Serial.println(F("               librairie 'NewPing' de Tim Eckel        "));
  Serial.println(F("======================================================="));
  Serial.println();

  // Petite pause de stabilisation avant de passer à la boucle loop (permet aussi d'ignorer certaines mesures "involontaires" du HC-SR04, au démarrage)
  delay(500);
  
}


// =================
// Boucle principale
// =================
void loop() {

  // Lancement d'une mesure (ping) et récupération de la distance mesurée
  unsigned int distanceMesureeEnCentimetres = monSonar.ping_cm();
      // Nota : si la valeur retournée est égale à 0, c'est qu'elle se situe en dehors de la plage de distance définie

  // Affichage de cette valeur sur le port série
  if (distanceMesureeEnCentimetres == 0) {
    Serial.println("Hors de portée ou erreur");
  } else {
    Serial.print("Distance : ");
    Serial.print(distanceMesureeEnCentimetres);
    Serial.println(" cm");
  }
  
  // Pause d'une seconde (1000 ms) entre chaque rebouclage (pour que les valeurs mesurées ne défilent pas trop vite sur le moniteur série)
  delay(1000);
      // Remarque : vous pouvez évidemment baisser cette valeur de 1000ms, histoire d'aller plus vite ! Toutefois,
      // évitez de descendre en dessous de 50/60 ms, sans quoi vous allez vous retrouver avec des échos parasites !!
  
}
