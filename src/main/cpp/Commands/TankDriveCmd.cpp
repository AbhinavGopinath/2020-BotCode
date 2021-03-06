// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "Commands/TankDriveCmd.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

TankDriveCmd::TankDriveCmd(): frc::Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(Robot::chassis.get());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::driveSubSystem.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void TankDriveCmd::Initialize()
{
    // initialize our local accessors
    driveSubSystem = Robot::driveSubSystem.get();
    controllerState = Robot::controllerState1.get();
	SetSpeedScale(1.0);	//TODO program in control system!!!
    printf("Tank drive command initialized\n");
}

// Called repeatedly when this Command is scheduled to run
void TankDriveCmd::Execute()
{
	// get raw joystick values and +- sign
	//this pulls the joystick values into the program.
	//(gets the state) gets what you want to do
	float leftX = -this->controllerState->GetLeftY();
	float rightX = -this->controllerState->GetRightY();

	// compute the adjusted motor values based n joystick / motor curve
	//this takes the value and matches it with the closest value on our parabola
	//so that the driving is smoother
	float leftY = SignOf(leftX) * SmoothDriveCurve(fabsf(leftX));
	float rightY = SignOf(rightX) * SmoothDriveCurve(fabsf(rightX));
    
	if (this->controllerState->GetButtonA()) // Invert drive code
	{
		float temp = leftY;
		leftY = rightY;
		rightY = temp;
	}

	if (this->controllerState->GetButtonB()) 
	{
		this->SetSpeedScale(.25); // 'slow' mode for motors
	}
	else
	{
		this->SetSpeedScale(1.0);
	}

    this->driveSubSystem->SetLeftSpeed((double)leftY);
    this->driveSubSystem->SetRightSpeed((double)rightY);
    this->driveSubSystem->Periodic();
}

// Make this return true when this Command no longer needs to run execute()
bool TankDriveCmd::IsFinished()
{
    return false;
}

// Called once after isFinished returns true
void TankDriveCmd::End()
{

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TankDriveCmd::Interrupted()
{

}

double TankDriveCmd::SmoothDriveCurve(double joystickYPosition) const
{
	if (joystickYPosition < .05)
	{
		return 0.0;		// allow for joystick calibration error
	}

	// data points on drive curve parabola
	//double x2 = .62, y2 = .36, y3 = 1.0;
	double x2 = .5, y2 = .5, y3 = 1.0;

	// variable a=a in equation b=b in equation
	//ax^2+bx+c the Equation for a parabola
	double b = ((y3 * x2 * x2) - y2) / ((x2 * x2) - x2);
	double a = y3 - b;

	// Returns the corresponding motor speed
	double motorSpeed = (a * joystickYPosition * joystickYPosition) + (b * joystickYPosition);
	return fabs(motorSpeed * this->motorSpeedScale);
}
