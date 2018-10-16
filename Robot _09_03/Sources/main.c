#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "fonctions.h"
#include "mapping.h" /*mapping de notre µc SE8 */

unsigned char vitesse_max_g=+90;
unsigned char vitesse_max_d=+90;
/*sens des moteurs : 1 marche avant, 0 marche arrière*/
volatile unsigned char vitmotG,vitmotD;
extern unsigned int temps;
unsigned int temps_avant;

void main(void) {
 volatile unsigned char retour_telemetreG, retour_telemetreD, retour_telemetreM;
 volatile char retour_telemetres;
 volatile unsigned char retour_balise, alarme;
 char tempo1s = 2, tempo05s = 1;
 char test=0;

 
  config();
  
  RTCSC = 0b00011011; //  Utilisation d'une horloge 1kHz pour interrompre toutes les 100*10ms (RTC=100)
  EnableInterrupts;   
                 
 
  phase_depart();
   vitmotG = vitesse_max_g;
   vitmotD = vitesse_max_d;
   
    
  for(;;) {                                             

    retour_telemetreG = lecture_telemetreG();                                                               
    retour_telemetreM = lecture_telemetreM();
    retour_telemetreD = lecture_telemetreD();
    
    /*-------- Concaténation des résultats des télémètres pour le switch --------*/
    retour_telemetres = concatenation_retour_teles(retour_telemetreG, retour_telemetreD, retour_telemetreM);  
    alarme = alarmes_concat(retour_telemetreG, retour_telemetreD, retour_telemetreM);
   
    retour_balise = lecture_balise();

  
   switch (retour_telemetres) 
  { /*retour_telemetre --> XXXXXGMD*/
    //rien sur les télémètres
    case 0 :
             
             sensmotsavant();
             vitmotG = vitesse_max_g;
             vitmotD = vitesse_max_d;

            break;
    //capteur gauche qui réagit
    case 1 : 
             temps_avant = temps;
             if (alarme==4 )     // si l'alarme se déclenche, on fait marche arriere
             {
             while (temps-temps_avant <tempo05s) 
             {
               sensmotsarriere  ();
               vitmotG = vitesse_max_g-60;
               vitmotD = vitesse_max_d-40;   
             } 
             }
             sensmotsavant();
             vitmotG = vitesse_max_g+20;
             vitmotD = vitesse_max_d-40;  
            break;
            
    //capteur avant qui réagit
    case 2 : temps_avant = temps;
             if (alarme==2 )     // si l'alarme se déclenche, on fait marche arriere
             { 
             while (temps-temps_avant <tempo05s) 
             {
               sensmotsarriere  ();
               vitmotG = vitesse_max_g-60;
               vitmotD = vitesse_max_d-60; 
             }
             }
             sensmotsavant();
             vitmotG = vitesse_max_g-50;
             vitmotD = vitesse_max_d-40;
            break;
    
    //capteurs gauche et avant qui réagissent
    case 3 : temps_avant = temps;
             if (alarme==4 | alarme == 2 | alarme ==6)     // si l'alarme se déclenche, on fait marche arriere
             { 
               while (temps-temps_avant <tempo05s) 
             {
               sensmotsarriere  ();
               vitmotG = vitesse_max_g-60;
               vitmotD = vitesse_max_d-30;
             }
             }
             sensmotsavant();
             vitmotG = vitesse_max_g+50;
             vitmotD = vitesse_max_d-30;  
            break;
    
    //capteur droite qui réagit
    case 4 : temps_avant = temps;
             if (alarme==1 )     // si l'alarme se déclenche, on fait marche arriere
             { 
               while (temps-temps_avant <tempo05s) 
             {
               sensmotsarriere  ();
               vitmotG = vitesse_max_g-30;
               vitmotD = vitesse_max_d-60;
             }
             }
             sensmotsavant();
             vitmotG = vitesse_max_g-40;
             vitmotD = vitesse_max_d+20;  
            break;
                                    
    //capteurs droit et gauche qui réagissent
    case 5 : temps_avant = temps;
             if (alarme==4 | alarme==1 | alarme ==5)     // si l'alarme se déclenche, on fait marche arriere
             { 
               while (temps-temps_avant <tempo05s) 
             {
               sensmotsarriere  ();
               vitmotG = vitesse_max_g-60;
               vitmotD = vitesse_max_d-60;   
             }
             }
             sensmotsavant();
             vitmotG = vitesse_max_g;
             vitmotD = vitesse_max_d;  
            break;
    
    //capteurs droite et avant qui réagissent
    case 6 : temps_avant = temps;
             if (alarme==1 | alarme==2 |alarme == 3)     // si l'alarme se déclenche, on fait marche arriere
             {  
             while (temps-temps_avant <tempo05s) 
             {
               sensmotsarriere  ();
               vitmotG = vitesse_max_g-30;
               vitmotD = vitesse_max_d-60; 
             }
             }
             sensmotsavant();
             vitmotG = vitesse_max_g-30;
             vitmotD = vitesse_max_d+50;  
            break;
    
    //tout les capteurs réagissent
    case 7 : temps_avant = temps;
             if (alarme==1 | alarme==2 | alarme ==4 |alarme ==7 )     // si l'alarme se déclenche, on fait marche arriere
             { 
               while (temps-temps_avant <tempo05s) 
             {
               sensmotsarriere  ();
               vitmotG = vitesse_max_g-50;
               vitmotD = vitesse_max_d-60;   
             }  
             }
             while (temps-temps_avant <tempo05s) 
             {
                vitmotG = vitesse_max_g-50;
                sensmotG = 1;              // 1 = marche arriere
                vitmotD = vitesse_max_d+60; 
                sensmotD =1;
             }
            break;
                                                                                 
  }
  TPM1C0V = vitmotD;
  TPM1C1V = vitmotG;
  /* ----------- selon (variable) :   (télémètres)
  
    cas 0 (rien sur les télémètres)
      --> pleine puissance monsieur soulou ! direction balise détectée
      
    cas 1 (capteurs droit et gauche qui réagissent)
      --> tout droit selon distance pour la vitesse
        
    cas 2 (capteurs droite et avant qui réagissent)
      --> moteur droit à pleine puissance, le gauche ralentit
      
    cas 3 (capteurs gauche et avant qui réagissent)
      --> moteur gauche à pleine puissance, le droit ralentit
      
    cas 4 (capteur droit qui réagit)
      --> moteur gauche à pleine puissance, le droit ralentit
      
    cas 5 (capteur gauche qui réagit)
      --> moteur droit à pleine puissance, le gauche ralentit
      
    cas 6 (capteur avant qui réagit)
      --> pleine puissance monsieur soulou !
      
    cas 7 (tout les capteurs réagissent)
      --> marche arrière + décalage sur un coté (évitement) --------- */   
  
  switch (retour_balise)
    { //Balise à droite --> Virage à droite
      case 1 : vitmotG = vitesse_max_g;
               vitmotD = vitesse_max_d-60;
      //Balise à milieu --> Pleine puissance M. Zoulou !
      case 2 : vitmotG = vitesse_max_g;
               vitmotD = vitesse_max_d;
      //Balise à gauche --> Virage à gauche
      case 4 : vitmotG = vitesse_max_g-60;
               vitmotD = vitesse_max_d;
      //Balise à l'arrière --> Et on fait tourner les robots
      case 8 :
               while (temps-temps_avant <tempo05s) 
             {
              vitmotG = vitesse_max_g;
               sensmotG = 1;
               vitmotD = vitesse_max_d-70;
             }
    }
    
    //Sommes-nous arrivés ?
//    lecture_couleur();  
    
  } /* loop forever */
  /* please make sure that you never leave main */
}
