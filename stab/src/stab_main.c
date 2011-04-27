/*************************************************************************
 * @file stab_main.c 
 * @brief This is the main file that contains the init and run 
 *              functiion for the other groups to call (scheduler)
 *              
 * @author Adam Debbiche 	       
 * @date 05/04/2011
 * @history 
 *    05/04/2011: Initial version //Adam
 *    06/04/2011: Added description and comments //Adam
 *    18/04/2011: Updated the ifdef to reflect new scheduler code //Adam
 *                Updated code to reflect coding standards
 **************************************************************************/
#ifdef PC 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stab_interface.h"
#include "stab.h"
#elif defined ARDUINO 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stab_interface.h"
#include "stab.h"
#include <Wire.h>
#include <WProgram.h>
#endif 
/************************************************************
 * Global variables used to save input/output of the algorithm:
 *    acc_vector: Holds the values of the accelerometer output (x, y, z)
 *    gyro_vector: Holds the values of the gyroscope output (x, y, z)
 *    filter_est: Output of the algorithm with (estimated vector) 
 ************************************************************/
struct vector
{
  float x;
  float y;
  float z;
};

struct vector gyro_vect;
struct vector accel_vect;
struct vector magn_vect;
struct vector filter_vect;
float heading; // heading from magnetometer

/*
 * Inits the main hardware components of the shield
 * A LOT OF CHANGES EXPECTED 
 */
int16_t stab_init(void)
{
  // code to init the hardware goes in here as well
#if defined PC 
  init_filter();
#elif defined ARDUINO
  init();
  Serial.begin(9600);
  Wire.begin();
  init_gyro_hardware();
  init_magn_hardware();
#endif
  return 0; 

}

/*
 *  Runs the code when the scheduler calls it
 */
int16_t stab_run(void)
{
#ifdef PC
  gyro_vect = init_sim();
  accel_vect = init_sim();
  
  filter_vect.x = comp_filter(accel_vect.x, gyro_vect.y, filter_vect.x); // filtered pitch angle
  filter_vect.y = comp_filter(accel_vect.y, gyro_vect.x, filter_vect.y); // filtered roll angle
  //filter_est[2] = comp_filter(acc_vector[2], gyro_vect[2], filter_est[2]);
  filter_vect.z = gyro_vect.z;
  
  printf("ESTIMATED X %f\n", filter_vect.x);
  printf("ESTMIATED Y %f\n", filter_vect.y);
  printf("ESTMIATED Z %f\n", filter_vect.z);
  
#elif defined ARDUINO
  while(1)
    {
      gyro_vect =  read_gyro_data();
      accel_vect = readAccel();
      magn_vect = read_magn_data();
      heading = (atan2(magn_vect.y , magn_vect.x)+M_PI)*180/M_PI;

      Serial.println("Gyro data now:");
      Serial.println(gyro_vect.x);
      Serial.println(gyro_vect.y);
      Serial.println(gyro_vect.z);
      Serial.println();
      
      Serial.println("Accel data now:");
      Serial.println(accel_vect.x);
      Serial.println(accel_vect.y);
      Serial.println(accel_vect.z);
      Serial.println();
      
      Serial.println("Magnetometer data now:");
      Serial.println(magn_vect.x);
      Serial.println(magn_vect.y);
      Serial.println(magn_vect.z);
      Serial.println(heading);
      Serial.println();

      delay(1000);
    }
#endif
  
  return 0;
}
