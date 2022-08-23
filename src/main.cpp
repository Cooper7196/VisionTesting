#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button()
{
	static bool pressed = false;
	pressed = !pressed;
	if (pressed)
	{
		pros::lcd::set_text(2, "I was pressed!");
	}
	else
	{
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {}

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
void opcontrol()
{
	// pros::Controller master(pros::E_CONTROLLER_MASTER);

	pros::Motor front_right_mtr(2);
	pros::Motor front_left_mtr(5);
	pros::Motor back_right_mtr(20);
	pros::Motor back_left_mtr(10);

	pros::Vision vision_sensor(16, pros::E_VISION_ZERO_CENTER);

	pros::vision_signature_s_t DISK_SIG = pros::Vision::signature_from_utility(1, 1761, 2259, 2010, -5943, -5567, -5755, 4.700, 0);
	vision_sensor.set_signature(1, &DISK_SIG);
	while (true)
	{
		int n = vision_sensor.get_object_count();
		if (n > 0)
		{
			pros::vision_object_s_t rtn = vision_sensor.get_by_sig(0, 1);
			printf("Sig: %d X: %d, Y: %d\n", rtn.signature, rtn.x_middle_coord, rtn.y_middle_coord);

			PID turnPID = PID(1, 0.0, 0.1);

			double turn = turnPID.calculate(rtn.x_middle_coord);
			front_right_mtr.move(turn);
			front_left_mtr.move(turn);
			back_right_mtr.move(turn);
			back_left_mtr.move(turn);
		}
		pros::delay(20);
	}
}
