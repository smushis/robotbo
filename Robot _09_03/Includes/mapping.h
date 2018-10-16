

/* -----------------------------------------Branchage des diff�rentes entr�es et sorties aux pattes du micro controleur--------------------------------------------------*/

#define telemetreg            PTBD_PTBD1    /*entr�e analogique*/
#define telemetrem            PTBD_PTBD2   /*entr�e analogique*/
#define telemetred            PTBD_PTBD3   /*entr�e analogique*/

#define captbaliseg           PTCD_PTCD3    /*entr�e num�rique*/
#define captbalisem           PTBD_PTBD4    /*entr�e num�rique*/
#define captbalised           PTBD_PTBD5    /*entr�e num�rique*/
#define captbalisea           PTCD_PTCD2    /*entr�e num�rique*/

#define sensmotG              PTAD_PTAD2    /*sortie num�rique*/
#define sensmotD              PTAD_PTAD3    /*sortie num�rique*/
#define couleur               PTAD_PTAD6    /*entr�e analogique*/
#define potentiometre         PTBD_PTBD0                                          

#define jack                  PTAD_PTAD7    /*entr�e num�rique*/
#define ballon                PTCD_PTCD0    /*sortie num�rique*/

//rapport cyclique des moteurs, en d�cimal (250 = 1 de rapport)
//#define vitmotG               TPM1C0VL  
//#define vitmotD               TPM1C1VL

#define ref_pwm               TPM1MOD  
