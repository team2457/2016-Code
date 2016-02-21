#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	//Speed controllers
	CANTalon* sc_left = new CANTalon(1);
	CANTalon* sc_right = new CANTalon(2);
	CANTalon* l_slave = new CANTalon(4);
	CANTalon* r_slave = new CANTalon(5);
	CANTalon* intake = new CANTalon(3);
	//Talon* l_shoot = new Talon(0);
	Victor* l_shoot = new Victor(0);
	Talon* r_shoot = new Talon(1);
	Talon* l_arm = new Talon(2);
	Talon* r_arm = new Talon(3);

	//Relays
	Relay *fleshlight = new Relay(1, Relay::Direction::kForwardOnly);

	//Joysticks
	Joystick* evan = new Joystick(0);
	Joystick* hunter = new Joystick(1);

	//Automatically Generated Stuff
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

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

		zeroAll();
	}

	void TeleopPeriodic()
	{
		//Drive
		if(SmartDashboard::GetBoolean("DB/Button 0", false))
		{
			sc_left -> Set(evan -> GetRawAxis(1));
			sc_right -> Set(evan -> GetRawAxis(5));
		}
		else
		{
			sc_left -> Set(-evan->GetRawAxis(1) + evan->GetRawAxis(5));
			sc_right -> Set(evan->GetRawAxis(1) + evan->GetRawAxis(5));
		}

		//Shooting Mechanism
		if(hunter -> GetRawButton(1)) //Pop out ball
		{
			intake -> Set(-hunter -> GetRawAxis(3));
		}
		else //Shoot
		{
			intake -> Set(.55*hunter -> GetRawAxis(3));
		}

		//Fleshlight
		fleshlight -> Set(evan->GetRawAxis(3)>0.05 ? Relay::Value::kForward : Relay::Value::kOff);;

		/*
		if(evan -> GetRawButton(6)) //Invert shooter
		{
			l_shoot -> Set(-evan -> GetRawAxis(3));
			r_shoot -> Set(evan -> GetRawAxis(3));
		}
		else //Do not invert shooter
		{
			l_shoot -> Set(evan -> GetRawAxis(3));
			r_shoot -> Set(-evan -> GetRawAxis(3));
		}
		 */

		//DPAD Stuff
		switch (evan -> GetPOV())
		{
		case (0):
			l_shoot -> Set(.25*evan -> GetRawAxis(3));
			r_shoot -> Set(.25*-evan -> GetRawAxis(3));
			break;
		case (90):
			l_shoot -> Set(.5*evan -> GetRawAxis(3));
			r_shoot -> Set(.5*-evan -> GetRawAxis(3));
			break;
		case (180):
			l_shoot -> Set(.75*evan -> GetRawAxis(3));
			r_shoot -> Set(.75*evan -> GetRawAxis(3));
			break;
		default:
			l_shoot -> Set(evan -> GetRawAxis(3));
			r_shoot -> Set(evan -> GetRawAxis(3));
		}

		//Arm
		l_arm -> Set(hunter-> GetRawAxis(5));
		r_arm -> Set(hunter-> GetRawAxis(5));

		//FORCEFEEDBACK
		evan -> SetRumble(Joystick::RumbleType::kLeftRumble, hunter -> GetRawButton(2));
		evan -> SetRumble(Joystick::RumbleType::kRightRumble, hunter -> GetRawButton(2));

		//Message
		SmartDashboard::PutString("DB/String 0", "Evan's a lil' bitch.");
		//std::cout << "Fuck you." << std::endl;

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
