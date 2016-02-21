#include "WPILib.h"
#include <iostream>

using namespace std;

class Robot: public IterativeRobot
{
private:
	//Speed controllers
	CANTalon* sc_left = new CANTalon(1);
	CANTalon* sc_right = new CANTalon(2);
	CANTalon* r_slave = new CANTalon(4);
	CANTalon* l_slave = new CANTalon(5);
	CANTalon* intake = new CANTalon(3);
	Talon* l_shoot = new Talon(9);
	Talon* r_shoot = new Talon(8);
	Talon* l_arm = new Talon(7);
	Talon* r_arm = new Talon(6);

	//Limit Switches
	DigitalInput* i_limit = new DigitalInput(9);
	DigitalInput* a_limit = new DigitalInput(6);

	//Relays
	Relay *fleshlight = new Relay(0, Relay::Direction::kForwardOnly);

	//Joysticks
	Joystick* evan = new Joystick(0);
	Joystick* hunter = new Joystick(1);

	//Automatically Generated Stuff
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	//Timers for elapsed time
	int count=0;
	Timer* timer1 = new Timer();
	Timer* timer2 = new Timer();

	//encoders
	Encoder *r_Encoder = new Encoder(0, 1, true, Encoder::EncodingType::k2X);

	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);

		zeroAll();
	}

	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void TeleopInit()
	{
		//Set slaves
		l_slave->SetControlMode(CANTalon::ControlMode::kFollower);
		r_slave->SetControlMode(CANTalon::ControlMode::kFollower);
		l_slave->Set(1);
		r_slave->Set(2);
		//l_slave->SetClosedLoopOutputDirection(true);
		//r_slave->SetClosedLoopOutputDirection(true);

		zeroAll();
	}

	void TeleopPeriodic()
	{
		//Drive
		if(SmartDashboard::GetBoolean("DB/Button 0", false))
		{
			sc_left -> Set(-evan -> GetRawAxis(1));
			sc_right -> Set(evan -> GetRawAxis(5));
		}
		else
		{
			sc_left -> Set(-evan->GetRawAxis(1) + .5*evan->GetRawAxis(0));
			sc_right -> Set(evan->GetRawAxis(1) + .5*evan->GetRawAxis(0));
		}

		//Intake
		if (i_limit->Get()) {
			if(hunter->GetRawButton(1))
			{
				l_shoot->Set(-1);
				r_shoot->Set(1);
				if(count == 0)
				{
					timer1->Start();
					count = 1;
				}
			}
			else if(evan->GetRawAxis(2) > 0.2)
			{
				intake->Set(-evan->GetRawAxis(2));
			}
			else{
				intake->Set(0);
			}
		}
		else
		{
			if(evan->GetRawAxis(3) > 0.2)
			{
				intake->Set(.40*evan->GetRawAxis(3));
			}
			else if(evan->GetRawAxis(2) > 0.2)
			{
				intake->Set(-evan->GetRawAxis(2));
			}
			else {
				intake->Set(0);
			}
		}

		if(timer1->Get()>2){
			intake->Set(1);
			timer2->Start();
			if(timer2->Get()>.5){
				l_shoot->Set(0);
				r_shoot->Set(0);
				intake->Set(0);
				timer1->Stop();
				timer1->Reset();
				timer2->Stop();
				timer2->Reset();
				count = 0;
			}
		}


		//Fleshlight
		fleshlight->Set(evan->GetRawButton(1) ? Relay::Value::kForward : Relay::Value::kOff);;

		/*
		//DPAD Stuff
		switch (evan -> GetPOV())
		{
		case (0):
			l_shoot -> Set(-.25*evan -> GetRawAxis(3));
			r_shoot -> Set(.25*evan -> GetRawAxis(3));
			break;
		case (90):
			l_shoot -> Set(-.5*evan -> GetRawAxis(3));
			r_shoot -> Set(.5*evan -> GetRawAxis(3));
			break;
		case (180):
			l_shoot -> Set(-.75*evan -> GetRawAxis(3));
			r_shoot -> Set(.75*evan -> GetRawAxis(3));
			break;
		case (270):
			l_shoot -> Set(evan-> GetRawAxis(3));
			r_shoot ->Set(-evan->GetRawAxis(3));
			break;
		default:
			l_shoot -> Set(-evan -> GetRawAxis(3));
			r_shoot -> Set(evan -> GetRawAxis(3));
			break;
		}*/

		//Hunter controls Arm
		if ((hunter->GetRawAxis(5) < 0.1 and hunter->GetRawAxis(5) > -0.1) or (a_limit->Get() and hunter->GetRawAxis(5) > 0))
		{
			l_arm->Set(0);
			r_arm->Set(0);
		}
		else
		{
			l_arm -> Set(-hunter-> GetRawAxis(5));
			r_arm -> Set(hunter-> GetRawAxis(5));
		}

		//let evan control arm
		/*if ((evan->GetRawAxis(5) < 0.1 and evan->GetRawAxis(5) > -0.1) or (a_limit->Get() and evan->GetRawAxis(5) > 0))
		{

			l_arm->Set(0);
			r_arm->Set(0);
		}
		else
		{
			l_arm -> Set(evan-> GetRawAxis(5));
			r_arm -> Set(-evan-> GetRawAxis(5));
		}*/


		//FORCEFEEDBACK
		evan -> SetRumble(Joystick::RumbleType::kLeftRumble, hunter -> GetRawButton(2));
		evan -> SetRumble(Joystick::RumbleType::kRightRumble, hunter -> GetRawButton(2));

		//Message
		SmartDashboard::PutNumber("DB/String 0", evan->GetPOV() );
		//std::cout << evan->GetPOV() << std::endl;

		//Wait
		Wait(0.001);
	}

	void DisabledInit() {
		zeroAll();
		std::cout << "Goodbye!" << std::endl;
	}

	void TestPeriodic()
	{
		lw->Run();
	}

	void zeroAll() {
		//Zero all speed controllers
		std::cout << "Stopping all speed controllers..." << std::endl;
		sc_left->StopMotor();
		sc_right->StopMotor();
		intake ->StopMotor();
		l_shoot ->StopMotor();
		r_shoot ->StopMotor();
		l_arm ->StopMotor();
		r_arm->StopMotor();

		//Turn off fleshlight
		std::cout << "Turning off fleshlight..." << std::endl;
		fleshlight -> Set(Relay::Value::kOff);
	}
};

START_ROBOT_CLASS(Robot)
