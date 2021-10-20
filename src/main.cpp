#include "main.h"
#include "pros/apix.h"
#include "autoSelect/selection.h"	
//Port Definitions

//Motors
#define FR_MOTOR_PORT 1
#define FL_MOTOR_PORT 2
#define BR_MOTOR_PORT 9
#define BL_MOTOR_PORT 10
#define ARM_MOTOR_PORT 3
#define CLAW_MOTOR_PORT 8

//Controller Inputs
#define TOP_RIGHT_SHOLDER DIGITAL_R1
#define BOTTOM_RIGHT_SHOLDER DIGITAL_R2
#define TOP_LEFT_SHOLDER DIGITAL_L1
#define BOTTOM_LEFT_SHOLDER DIGITAL_L2

//Sensors
#define VISION_SENSOR_PORT 20
#define DISTANCE_SENSOR_PORT 7
#define INERTIA_SENSOR_PORT 11
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	selector::init();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	//Pros Device Control Initialization
	pros::Motor fr_motor(FR_MOTOR_PORT);
	pros::Motor fl_motor(FL_MOTOR_PORT, true);
	pros::Motor rr_motor(BR_MOTOR_PORT);
	pros::Motor rl_motor(BL_MOTOR_PORT, true);
	pros::Motor arm_motor(ARM_MOTOR_PORT);
	pros::Motor claw_motor(CLAW_MOTOR_PORT);
	pros::Imu inertia_sensor(INERTIA_SENSOR_PORT);
	pros::Vision vision_sensor(VISION_SENSOR_PORT);
	pros::Distance distance_sensor(DISTANCE_SENSOR_PORT);

	if(selector::auton == 1){
		printf("Red Left");

		if(distance_sensor.get() > 5){
			fl_motor.move_velocity(10);
			fr_motor.move_velocity(10);
			rl_motor.move_velocity(10);
			rr_motor.move_velocity(10);
		}

		else{
			fl_motor.move_velocity(0);
			fr_motor.move_velocity(0);
			rl_motor.move_velocity(0);
			rr_motor.move_velocity(0);
		}

	}
	if(selector::auton == 2){
		printf("Red Right");
	}
	if(selector::auton == 3){
		printf("Red Disabled");
	}
	if(selector::auton == -1){
		printf("Blue Left");
	}
	if(selector::auton == -2){
		printf("Blue Right");
	}
	if(selector::auton == -3){
		printf("Blue Disabled");
	}
	if(selector::auton == 0){
		printf("Skills");
	}

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol() {
	//Pros Device Control Initialization
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor fr_motor(FR_MOTOR_PORT);
	pros::Motor fl_motor(FL_MOTOR_PORT, true);
	pros::Motor rr_motor(BR_MOTOR_PORT);
	pros::Motor rl_motor(BL_MOTOR_PORT, true);
	pros::Motor arm_motor(ARM_MOTOR_PORT);
	pros::Motor claw_motor(CLAW_MOTOR_PORT);
	pros::Imu inertia_sensor(INERTIA_SENSOR_PORT);
	pros::Vision vision_sensor(VISION_SENSOR_PORT);
	pros::Distance distance_sensor(DISTANCE_SENSOR_PORT);

			//Motor Break Modes
	arm_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	claw_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	
	while (true) {
		 	//Strafe Main Control
		int turn = master.get_analog(ANALOG_RIGHT_X);
		int power = master.get_analog(ANALOG_LEFT_Y);
		int strafe = master.get_analog(ANALOG_LEFT_X);
			//Maths
		int turnreversed = turn * -1;
		int straferevered = strafe * -1;
		int fl = power + turnreversed + straferevered;
		int rl = power + turnreversed - straferevered;
		int fr = power - turnreversed - straferevered;
		int rr = power - turnreversed + straferevered;

		fl_motor.move(fl);
		fr_motor.move(fr);
		rl_motor.move(rl);
		rr_motor.move(rr);

			//Arm Control
		if(master.get_digital(TOP_LEFT_SHOLDER)) {
			arm_motor.move_velocity(100);
		}

		else if(master.get_digital(BOTTOM_LEFT_SHOLDER)) {
			arm_motor.move_velocity(-100);
		}

		else {
			arm_motor.move_velocity(0);
		}
			//Claw Control
		if(master.get_digital(TOP_RIGHT_SHOLDER)) {
			claw_motor.move_velocity(100);
		}

		else if(master.get_digital(BOTTOM_RIGHT_SHOLDER)) {
			claw_motor.move_velocity(-100);
		}

		else {
			claw_motor.move_velocity(0);
		}
	}
}
