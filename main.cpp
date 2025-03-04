/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/


// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// ForbackMotors        motor_group   2, 3            
// TurnMotor            motor         1               
// GrabberMotor         motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----


#include "vex.h"


using namespace vex;


// A global instance of competition
competition Competition;


// define your global instances of motors and other devices here


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/


void pre_auton(void) {
 // Initializing Robot Configuration. DO NOT REMOVE!
 vexcodeInit();


 // All activities that occur before the competition starts
 // Example: clearing encoders, setting servo positions, ...
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


void autonomous(void) {
 // ..........................................................................
 // Insert autonomous user code here.
 // ..........................................................................
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


void usercontrol(void) {
 // User control code here, inside the loop
 while (1) {
    //Drive Train//
 double hundredspeedvolt = 0.12;
 double turnval = Controller1.Axis4.position(percent);
 double ForbackVal = Controller1.Axis3.position(percent);
 double turnVolt = turnval * hundredspeedvolt;
 double ForbackVolt = ForbackVal * hundredspeedvolt;
 ForbackMotors.spin(forward, ForbackVolt, voltageUnits::volt);
 TurnMotor.spin(forward, turnVolt, voltageUnits::volt);




 //GRABBING//
 bool GrabberIni = Controller1.ButtonR1.pressing();
 bool GrabberRe = Controller1.ButtonL1.pressing();
 if (GrabberIni){
   GrabberMotor.spinToPosition(360, degrees);
 }
 else if (GrabberRe){
   GrabberMotor.spinToPosition(-360, degrees);
 }
 else {
 }
 //Breaking//
 if (Controller1.ButtonB.pressing()){
   TurnMotor.stop(brake);
   ForbackMotors.stop(brake);
 }
//inverse kinematics//
 double n = 0;
 double nv = 0;
 double hy;
 float length = 16.5;
 double voltagemulti = length/12; //use same length for both parts of arm
 double ElbowAngler;
 double ElbowAngle;
 double pi = 3.1415926535897932384626433;
 double BaseArmAngler;
 double BaseArmAngle;
 double  BaseArmMs;
 double ElbowMs;




 if (Controller1.ButtonUp.pressing()){
     n=n+0.01;
         wait(10, msec);
}
 if  (Controller1.ButtonDown.pressing()){
   n=n-0.01;
       wait(10, msec);
}
 if (Controller1.ButtonLeft.pressing()){
   nv=nv+0.01;
   wait(10, msec);
}   
 if (Controller1.ButtonRight.pressing()){
   nv=nv-0.01;
       wait(10, msec);
}   
hy = sqrt((pow(voltagemulti*n, 2)+pow(voltagemulti*nv,2)));




 ElbowAngler= (2*pow(length,2)-pow(hy,2))/(2*pow(length,2));  //radians
 if (ElbowAngler > 1.0){
   ElbowAngler = 1.0;
 }  // Ensure valid input for acos()
 if (ElbowAngler < -1.0){
 ElbowAngler = -1.0;
 }
 ElbowAngle = acos(ElbowAngler) *(180/pi);  //degrees




 BaseArmAngler = pow(hy,2)/(length*2*hy); //radians
   if (BaseArmAngler > 1.0){
   BaseArmAngler = 1.0;
 }  // Ensure valid input for acos()
 if (BaseArmAngler < -1.0){
 BaseArmAngler = -1.0;
 }
 BaseArmAngle = acos(BaseArmAngler)*(180/pi);  //degrees




 BaseArmMs = BaseArmAngle * 11000;
 ElbowMs = ElbowAngle * 11000;




//Inverse Kinematics End//




//Run Inverse Kinematics on Motor//
 BaseArm.spinFor(forward, BaseArmAngle, rotationUnits::deg, false);
 ElbowMotor.spinFor(forward,ElbowAngle, rotationUnits::deg, false); // probably change because not perfectly similar to arduino//
while (BaseArm.isSpinning()||ElbowMotor.isSpinning()){
     wait(10, msec);
}
   wait(20, msec); // Sleep the task for a short amount of time to
                   // prevent wasted resources.
 }
}


//
// Main will set up the competition functions and callbacks.
//
int main() {
 // Set up callbacks for autonomous and driver control periods.
 Competition.autonomous(autonomous);
 Competition.drivercontrol(usercontrol);


 // Run the pre-autonomous function.
 pre_auton();


 // Prevent main from exiting with an infinite loop.
 while (true) {
   wait(100, msec);
 }
}



