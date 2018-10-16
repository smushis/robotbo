

/* -----------------------------------------Branchage des différentes entrées et sorties aux pattes du micro controleur--------------------------------------------------*/

#define telemetreg            PTBD_PTBD1    /*entrée analogique*/
#define telemetrem            PTBD_PTBD2   /*entrée analogique*/
#define telemetred            PTBD_PTBD3   /*entrée analogique*/

#define captbaliseg           PTCD_PTCD3    /*entrée numérique*/
#define captbalisem           PTBD_PTBD4    /*entrée numérique*/
#define captbalised           PTBD_PTBD5    /*entrée numérique*/
#define captbalisea           PTCD_PTCD2    /*entrée numérique*/

#define sensmotG              PTAD_PTAD2    /*sortie numérique*/
#define sensmotD              PTAD_PTAD3    /*sortie numérique*/
#define couleur               PTAD_PTAD6    /*entrée analogique*/
#define potentiometre         PTBD_PTBD0                                          

#define jack                  PTAD_PTAD7    /*entrée numérique*/
#define ballon                PTCD_PTCD0    /*sortie numérique*/

//rapport cyclique des moteurs, en décimal (250 = 1 de rapport)
//#define vitmotG               TPM1C0VL  
//#define vitmotD               TPM1C1VL

#define ref_pwm               TPM1MOD  
