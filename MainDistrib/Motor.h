#ifndef MOTOR
#define MOTOR


// Nombre de micro-pas pour 1 tour complet
/*  
  200 = pas de microstep
  400 = microstep 1/2
  800 = microstep 1/4
  1600 = microstep 1/8
  3200 = microstep 1/16e
*/
const int microsteps_per_revolution = 3200;

//Prototypes
void motor_init();
void motor_start(float nb_tour, int vitesse);
void motor_inv();
void motor_start_async(float nb_tours, int vitesse);


#endif //ends #ifndef MOTOR