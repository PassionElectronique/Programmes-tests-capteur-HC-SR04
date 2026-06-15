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
//  Nom Projet  :       Tuto HC-SR04 P (capteur à ultrasons)
//  Desc Projet :       Programme de test capteur HC-SR04P utilisant les lignes ECHO et TRIG,
//                      en utilisant la fonction arduino "pulseIn"
//
//  Fichier     :       prg1-HCSR04-P-TestEchoTrigAvecPulseIn.ino
//  Créé le     :       26.04.2026
//  Auteur      :       Jérôme TOMSKI
//  Site        :       https://passionelectronique.fr/
//  GitHub      :       https://github.com/PassionElectronique/Programmes-tests-capteur-HC-SR04
//  Licence     :       https://creativecommons.org/licenses/by-nc-nd/4.0/deed.fr (BY-NC-ND 4.0 CC)
//
// ===================================================================================================

// Inclusion des librairies nécessaires

// Constantes
#define brochePourLigneECHO  2   // La broche D2 du microcontrôleur sera reliée à la ligne ECHO du HC-SR04P
#define brochePourLigneTRIG  3   // La broche D3 du microcontrôleur sera reliée à la ligne TRIG du HC-SR04P


// ========================
// Initialisation programme
// ========================
void setup() {

  // Définition des entrées/sorties
  pinMode(brochePourLigneECHO, INPUT);   // Sortie ECHO du HC-SR04 --> vers entrée microcontrôleur (donc "input" côté µC)
  pinMode(brochePourLigneTRIG, OUTPUT);  // Entrée TRIG du HC-SR04 <-- reçoit sortie microcontrôleur (donc "output" côté µC)

  // Mise à l'état bas de la ligne TRIG, pour que le HC-SR04 soit au repos
  digitalWrite(brochePourLigneTRIG, LOW);

  // Démarrage du moniteur série
  Serial.begin(9600);
  Serial.println(F("======================================================="));
  Serial.println(F("Programme #1 : mesures de distances toutes les secondes"));
  Serial.println(F("               avec le capteur HC-SR04, en utilisant la"));
  Serial.println(F("               fonction arduino 'pulseIn'              "));
  Serial.println(F("======================================================="));
  Serial.println();

  // Petite pause de stabilisation avant de passer à la boucle loop (permet aussi d'ignorer certaines mesures "involontaires" du HC-SR04, au démarrage)
  delay(500);
  
}


// =================
// Boucle principale
// =================
void loop() {

  // Variables
  long duree_en_microsecondes;
  float distance_en_centimetres;

  // Lance une mesure ultrason, récupère la durée écoulée (entre émission ultrason et réception écho),
  // puis calcule la distance correspondante et affiche cette distance sur le moniteur série
  duree_en_microsecondes = effectueUneMesureSurLeHCSR04();
  distance_en_centimetres = convertitMicrosecondesEnCentimetres(duree_en_microsecondes);
  afficheDistanceEnCmSurMoniteurSerie(distance_en_centimetres);
  
  // Pause d'une seconde (1000 ms) entre chaque rebouclage (remarque : ne pas aller trop vite ici, en tout cas pas en dessous de 60ms, sinon on aurait possiblement des échos parasites)
  delay(1000);
  
}


// =======================================
// Fonction : effectueUneMesureSurLeHCSR04
// =======================================
long effectueUneMesureSurLeHCSR04(void) {

  // -------------------------------------------------------------
  // Procédure à suivre avec le HC-SR04, pour effectuer une mesure
  // -------------------------------------------------------------
  // - mettre la ligne TRIG à l'état bas pendant 2 µs (inutile si elle l'était déjà avant, mais c'est une "bonne pratique", recommandée par les fabricants)
  // - mettre la ligne TRIG à l'état haut pendant 10 µs (pas moins, même si on aurait pu mettre plus) ; nota : c'est ce "front montant" sur TRIG qui lance l'émission d'ultrasons sur le HC-SR04
  // - mettre la ligne TRIG à l'état bas, pour préparer la lecture suivante, plus tard
  // - mesure la durée d'impulsion sur la ligne ECHO, en microsecondes) ; nota : la ligne ECHO passe immédiatement à l'état HAUT "dès le 1er ultrason émis", et repassera à l'état BAS dès le 1er ultrason reçu

  // Sécurité avant démarrage
  digitalWrite(brochePourLigneTRIG, LOW);
  delayMicroseconds(2);

  // Impulsion de 10 µs à l'état haut sur TRIG, pour lancer une émission d'ultrason par le HC-SR4
  digitalWrite(brochePourLigneTRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(brochePourLigneTRIG, LOW);

  // Mesure de la durée d'impulsion à l'état HAUT sur ECHO, et retourne cette valeur (durée en microsecondes, pour rappel)
  // Remarque : j'ai rajouté ici un timeout de 30000 µs (30 ms), afin qu'on arrête la mesure de durée sur la ligne ECHO, si jamais cette ligne ne revenait pas à l'état bas (absence d'écho ou dysfonctionnement HC-SR04, donc)
  return pulseIn(brochePourLigneECHO, HIGH, 30000);
  
}


// ==============================================
// Fonction : convertitMicrosecondesEnCentimetres
// ==============================================
float convertitMicrosecondesEnCentimetres(long duree_en_microsecondes) {

  // -----------------------
  // Explications techniques
  // -----------------------
  // La vitesse du son est approximativement égale à 343 mètres par seconde (si on fait abstraction de l'incidence de la température et tout et tout !!!)
  //
  // Donc :
  //  - comme vitesse_son = 343 m/s
  //  - alors vitesse_son = 34300 cm/s
  //  - alors vitesse_son = 34.3 cm/ms
  //  - alors vitesse_son = 0.0343 cm/µs
  //
  // Toutes ces lignes sont donc équivalentes, mais avec la dernière, on peut calculer un nombre de centimètres (cm) à partir d'une durée en microsecondes (µs)

  return duree_en_microsecondes * 0.0343 / 2; // Nota : on divise par 2 car là on a mesuré un temps aller/retour au niveau du HC-SR04 ;
                                              //        donc si on veut une distance, il faut qu'on prenne que l'aller ou le retour seul, donc la moitié de l'aller retour, en somme !
}


// ==============================================
// Fonction : afficheDistanceEnCmSurMoniteurSerie
// ==============================================
void afficheDistanceEnCmSurMoniteurSerie(float distance_en_centimetres) {

  // Envoi des données sur le moniteur série
  Serial.print(F("Distance = "));
  Serial.print(distance_en_centimetres);
  Serial.println(F(" cm"));
  
}
