#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "mapping.h"
#include "fonctions.h"

extern  unsigned char vitesse_max;
extern  unsigned char vitesse_max_g;
extern  unsigned char vitesse_max_d;
extern volatile unsigned char vitmotG,vitmotD;
unsigned int temps;


 /*--------------------------------------------------------------------------------------------------*/
void config(void)
  {
  /*----------------- Configuration pleine et totale du robot pour le démarrage ---------*/
  SOPT1 = SOPT1&0b00111111; // desable le watchdog
    
/*---------------------------Réglages de l'horloge interne TPM1-------------------------------------------------------*/



/* les deux lignes de sens sont config en sortie */

  PTADD = 0b00001100;
  
  
/* la ligne du ballon config en sortie */
  PTBDD = 0b00000001;

/* set des canaux 0 (CH0) et 1 (CH1) en mode edge aligned PWM, High true pulses  */

ref_pwm = 250; //set de l'horloge du pwm sur 1ms

TPM1C0SC = 0b00101000; // ATT
TPM1C0V = 10;
TPM1C1SC = 0b00101000; //ATT

TPM1C1V = 10;
 /*set comme 0 (All channels operate as input capture, output compare, or edge-aligned PWM mode as selected by the
MSnB:MSnA control bits in each channel’s status and control register)00(Bus rate clock)110(Clock divided by 126)*/ 

TPM1SC  = 0b00001110;

/*set des pins a lire (télé) pour qu'ils soient en lecture analogique (1)*/ 
APCTL1 = 0b10000000;
APCTL2 = 0b00000011;

/*ADCCFG is used to select the mode of operation, clock source, clock divide, and configure for low power
or long sample time*/
ADCCFG = 0b01000000;      /*x10xxxxx pour diviser la fréquence du bus clock par 4 et avoir une fréquence ~1MHz*/


/*configuration de la résistance de pullup pour l'entrée Jack (PTB1)*/

PTBPE = 0b00100000;

//config d'un timer de secondes
RTCMOD = 100;
temps = 0;
  }
  
 /*--------------------------------------------------------------------------------------------------*/
unsigned char lecture_telemetreG(void)
    {
      /*--------- Aquisition des données télémétrique 1 ----------*/
      volatile unsigned char res1;
      ADCSC1 = 0b00100111;
      tempo2us(12);
      res1 = ADCRL;
      return res1;
    }
    
 /*--------------------------------------------------------------------------------------------------*/
unsigned char lecture_telemetreM(void)
    {
      /*--------- Aquisition des données télémétrique 2 ----------*/ //Gauche
      volatile unsigned char res2;
      ADCSC1 = 0b00101000;
      tempo2us(12); 
      res2  = ADCRL;
      return res2;
    }
    
 /*--------------------------------------------------------------------------------------------------*/
unsigned char lecture_telemetreD(void)
    {
      /*--------- Aquisition des données télémétrique 3 ----------*/
      volatile unsigned char res3;
      ADCSC1 = 0b00101001;
      tempo2us(12);
      res3  = ADCRL;
      return res3;
    }
    
 /*--------------------------------------------------------------------------------------------------*/
char concatenation_retour_teles(unsigned char teleG, unsigned char teleD, unsigned char teleM)
  {
      /*-------- Concaténation des résultats des télémètres pour le switch -------*/
   volatile char concat;

      concat = 0;
              if (teleG>27) //remplacer 0 par une valeur suffisante !!!!!
                concat = concat | 0x04;
              if (teleM>27) //remplacer 0 par une valeur suffisante !!!!!
                concat = concat | 0x02;
              if (teleD>25) //remplacer 0 par une valeur suffisante !!!!!
                concat = concat | 0x01;
      return concat;    
  }
  
  char alarmes_concat(unsigned char teleG, unsigned char teleD, unsigned char teleM)
  {
      /*-------- Concaténation des résultats des télémètres pour le switch -------*/
   volatile char concat2;

      concat2 = 0;
              if (teleG>115) //remplacer 0 par une valeur suffisante !!!!!
                concat2 = concat2 | 0x04;
              if (teleM>115) //remplacer 0 par une valeur suffisante !!!!!
                concat2 = concat2 | 0x02;
              if (teleD>115) //remplacer 0 par une valeur suffisante !!!!!
                concat2 = concat2 | 0x01;
      return concat2;    
  }
 /*--------------------------------------------------------------------------------------------------*/  
unsigned char lecture_balise(void)
  {
      /*-------- Recherche de la position de la balise si il n'y a aucun obstacle ---------*/
      char concate = 0;
      unsigned char baliseG, baliseM, baliseD, baliseA;

      baliseG = captbaliseg;
      baliseM = captbalisem;
      baliseD = captbalised;
      baliseA = captbalisea; 
      
              if (baliseA>0)
                concate = concate | 0x08;
              if (baliseG>0) 
                concate = concate | 0x04;
              if (baliseM>0) 
                concate = concate | 0x02;
              if (baliseD>0) 
                concate = concate | 0x01;
              
      return concate;
  }
  
 /*--------------------------------------------------------------------------------------------------*/  
  void phase_depart(void)
    {
      /*-------- Départ du robot -----------*/
     
     do if(jack==0) /*retrait du jack*/ {
        
        
     }
     while(couleur ==1); //sortie de la zone de départ 
    }

 /*--------------------------------------------------------------------------------------------------*/    
  void lecture_couleur(void)
    {
      /*---------- Acquisition de la présence ou non de la surface blanche ----------*/
      if(couleur==1)
        vitmotG = 0;
        vitmotD = 0;
        ballon = 1;
        for(;;);
    }

 /*--------------------------------------------------------------------------------------------------*/
 unsigned char vitesse(unsigned char rt)
    {
      /*---------- Calcul de la vitesse suivant la position de l'obstacle détecté ---------*/
        char vit1;
        vit1 = vitesse_max - (rt/4);
        return vit1;
    }

 /*--------------------------------------------------------------------------------------------------*/
  unsigned char vitesse2(unsigned char rt)
    {
      /*---------- Calcul de la vitesse suivant la position de l'obstacle détecté ---------*/
        char vit2;
        vit2 = vitesse_max - (rt/4);
        return vit2;
    } 
    
    void tempo2us(unsigned char i)  //durée = 2µs*i
      {
      do
        {
        i--;
        }
        while (i>0);
      }
    void sensmotsavant(void)
      {
        sensmotG = 0;
        sensmotD = 0;
      }
      
    void sensmotsarriere(void)
      {
        sensmotG = 1;
        sensmotD = 1;
      }
      
    void interrupt 25 Overflow_RTC(void)
      {
        temps++;
        RTCSC_RTIF=1;//acquittement interruption
      }