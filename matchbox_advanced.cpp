// matchbox_advanced.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace

using namespace tle;

void carMovementFrontwards(I3DEngine* myEngine,IModel* Matchbox,IModel* MatchFront, IModel* MatchRear,
float SteerAngle, float Speed)
{
	//Makes the car move frontwards and also rolls the wheels.

	if (myEngine->KeyHeld(Key_Up))
	{
		Matchbox->RotateLocalY(SteerAngle);

		Matchbox->MoveLocalZ(Speed);
		MatchFront->RotateLocalX(Speed * 3);
		MatchRear->RotateLocalX(Speed * 3);
	}
}


void carMovementBackwards(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear,
	float SteerAngle, float Speed)
{
	//Makes the car move backwards and also rolls the wheels.

	if (myEngine->KeyHeld(Key_Down))
	{
		Matchbox->RotateLocalY(-SteerAngle);

		Matchbox->MoveLocalZ(-Speed);
		MatchFront->RotateLocalX(-Speed * 3);
		MatchRear->RotateLocalX(-Speed * 3);
	}
}


void carSteeringRight(I3DEngine* myEngine, IModel* MatchFront,
	float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease){
	if (myEngine->KeyHeld(Key_Right))
	{
		if (MatchFrontRot <= RotationLimit)
		{
			MatchFrontRot = MatchFrontRot + RotationSpeed;
			MatchFront->RotateY(RotationSpeed);

			SteerAngle = SteerAngle + SteerAngleIncrease;
		}
	}
}


void carSteeringLeft(I3DEngine* myEngine, IModel* MatchFront,
	float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease) {
	if (myEngine->KeyHeld(Key_Left))
	{
		if (MatchFrontRot >= -RotationLimit)
		{
			MatchFrontRot = MatchFrontRot - RotationSpeed;
			MatchFront->RotateY(-RotationSpeed);

			SteerAngle = SteerAngle - SteerAngleIncrease;
		}
	}
}


void carControls(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear,
	float Speed, float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease)
{
	carMovementFrontwards(myEngine, Matchbox, MatchFront, MatchRear, SteerAngle, Speed);

	carMovementBackwards(myEngine, Matchbox, MatchFront, MatchRear, SteerAngle, Speed);

	carSteeringRight(myEngine, MatchFront, MatchFrontRot, RotationLimit, RotationSpeed, SteerAngle, SteerAngleIncrease);

	carSteeringLeft(myEngine, MatchFront, MatchFrontRot, RotationLimit, RotationSpeed, SteerAngle, SteerAngleIncrease);
}


void gameControls(I3DEngine* myEngine) {
	if (myEngine->KeyHit(Key_Escape))
	{
		myEngine->Stop();
	}
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");

	/**** Set up your scene here ****/
	// camera 
	ICamera* camera = myEngine->CreateCamera(kManual, 0, 80, -160);
	camera->RotateX(25);

	// meshs
	IMesh* MatchboxMesh = myEngine->LoadMesh("Matchbox.x");
	IMesh* FloorMesh = myEngine->LoadMesh("Floor.x");
	IMesh* MatchMesh = myEngine->LoadMesh("Match.x");
	IMesh* TwoPenceMesh = myEngine->LoadMesh("TwoPence.x");
	IMesh* GridMesh = myEngine->LoadMesh("Grid.x");
	IMesh* TurretMesh = myEngine->LoadMesh("Turret.x");
	IMesh* BulletMesh = myEngine->LoadMesh("Bullet.x");

	// enviroment models
	IModel* Floor = FloorMesh->CreateModel(0, 0, 0);
	IModel* Grid = GridMesh->CreateModel(0, 0.1, 0);

	//Canon models
	IModel* Turret = TurretMesh->CreateModel(150, 25, 150);
	IModel* Bullet = BulletMesh->CreateModel(0, 0, 0);

	// matchbox car models
	IModel* Matchbox = MatchboxMesh->CreateModel(0, 13, 0);
	IModel* MatchFront = MatchMesh->CreateModel();
	IModel* MatchRear = MatchMesh->CreateModel();
	IModel* TwoPenceFrontR = TwoPenceMesh->CreateModel();
	IModel* TwoPenceRearR = TwoPenceMesh->CreateModel();
	IModel* TwoPenceFrontL = TwoPenceMesh->CreateModel();
	IModel* TwoPenceRearL = TwoPenceMesh->CreateModel();

	// matchbox car dimentions
	const float MatchboxWidth = 18.0f;
	const float MatchboxLenght = 25.0f;
	const float AxlesOffset = 2.0f;
	const float TyresOffset = 4.0f;

	// matchbox car parts attach
	MatchFront->AttachToParent(Matchbox);
	MatchRear->AttachToParent(Matchbox);
	TwoPenceFrontR->AttachToParent(MatchFront);
	TwoPenceFrontL->AttachToParent(MatchFront);
	TwoPenceRearR->AttachToParent(MatchRear);
	TwoPenceRearL->AttachToParent(MatchRear);

	// Positioning Matchbox car parts
	MatchFront->MoveZ(MatchboxLenght - AxlesOffset);
	MatchRear->MoveZ(-MatchboxLenght + AxlesOffset);
	TwoPenceFrontR->MoveX(MatchboxWidth + TyresOffset);
	TwoPenceFrontL->MoveX(-MatchboxWidth - TyresOffset);
	TwoPenceRearR->MoveX(-MatchboxWidth - TyresOffset);
	TwoPenceRearL->MoveX(MatchboxWidth + TyresOffset);

	// Attach bullet to canon
	Bullet->AttachToParent(Turret);

	camera->AttachToParent(Matchbox);

	// MatchBox car variables
	float Speed = 0.6f;
	float RotationSpeed = 0.2f;
	float RotationLimit = 25.0f;
	float MatchFrontRot = 0.0f;
	float SteerAngle = 0.0f;
	float SteerAngleIncrease = 0.008f;
	int carState = 1;

	float collisionDist = 0;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

	//	carControls(myEngine, Matchbox, MatchFront, MatchRear, Speed, MatchFrontRot, RotationLimit, RotationSpeed, SteerAngle, SteerAngleIncrease);
		
		Turret->LookAt(Matchbox);
		Bullet->MoveLocalZ(0.3);

		// Collision Detection
		//location of car
		float p1x = Matchbox->GetX();
		float p1y = Matchbox->GetY();
		float p1z = Matchbox->GetZ();

		//location of bullet
		float p2x = Bullet->GetX();
		float p2y = Bullet->GetY();
		float p2z = Bullet->GetZ();

		float dx = p1x - p2x;
		float dy = p1y - p2y;
		float dz = p1z - p2z;
		collisionDist = sqrt(dx*dx + dy*dy + dz*dz);
		
		
		
		switch (carState)
		{
		case 1:
		{
			// Car is healthy. Control it with buttons.

			// (a) Update:
			carControls(myEngine, Matchbox, MatchFront, MatchRear, Speed, MatchFrontRot, RotationLimit, RotationSpeed, SteerAngle, SteerAngleIncrease);

			// (b) Transitions:
		}
			break;
		case 0:
			// Car is broken. Do nothing.

			// (a) Update 

			// (b)
			   break;
		}

		if (collisionDist < 10) {
			carState = 0;
		}

		gameControls(myEngine);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}