// matchbox_advanced.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace

#define Car_Healthy 0
#define Car_Dead 1

#define Bullet_Flying 0
#define Bullet_NotFlying 1

using namespace tle;

//Function Declarations
bool collisionDetection(IModel* Model1, IModel* Model2, int& modelState);

void carMovementFrontwards(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear,
	float SteerAngle, float Speed);

void carMovementBackwards(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear,
	float SteerAngle, float Speed);

void carSteeringRight(I3DEngine* myEngine, IModel* MatchFront,
	float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease);

void carSteeringLeft(I3DEngine* myEngine, IModel* MatchFront,
	float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease);

void updateCar(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear,
	float Speed, float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease,
	IModel* Bullet, int& carState);

void carDynamics(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear,
	float Speed, float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease);

void carFSM(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear, IModel* Bullet, int& carState,
	float Speed, float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease);

void gameControls(I3DEngine* myEngine);

void updateBullet(I3DEngine* myEngine, IModel* Bullet, IModel* Matchbox, IModel* Turret, int& bulletState,
	IMesh* BulletMesh);


//Function Definitions
bool collisionDetection(IModel* Model1, IModel* Model2, int& modelState)
{
	bool collision = false;
	//location of model1
	float p1x = Model1->GetX();
	float p1y = Model1->GetY();
	float p1z = Model1->GetZ();

	//location of model2
	float p2x = Model2->GetX();
	float p2y = Model2->GetY();
	float p2z = Model2->GetZ();

	//distance model1 and model2
	float dx = p1x - p2x;
	float dy = p1y - p2y;
	float dz = p1z - p2z;
	float collisionDist = sqrt(dx*dx + dy * dy + dz * dz);

	if (collisionDist < 10) {
		collision = true;
	}
	return collision;
}

void carMovementFrontwards(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear,
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
	float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease) {
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

void updateCar(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear,
	float Speed, float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease,
	IModel* Bullet, int& carState)
{
	carDynamics(myEngine, Matchbox, MatchFront, MatchRear, Speed, MatchFrontRot, RotationLimit, RotationSpeed, SteerAngle, SteerAngleIncrease);

	carFSM(myEngine, Matchbox, MatchFront, MatchRear, Bullet, carState, Speed, MatchFrontRot, RotationLimit, RotationSpeed, SteerAngle, SteerAngleIncrease);
}


void carDynamics(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear,
	float Speed, float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease)
{
	carMovementFrontwards(myEngine, Matchbox, MatchFront, MatchRear, SteerAngle, Speed);

	carMovementBackwards(myEngine, Matchbox, MatchFront, MatchRear, SteerAngle, Speed);

	carSteeringRight(myEngine, MatchFront, MatchFrontRot, RotationLimit, RotationSpeed, SteerAngle, SteerAngleIncrease);

	carSteeringLeft(myEngine, MatchFront, MatchFrontRot, RotationLimit, RotationSpeed, SteerAngle, SteerAngleIncrease);
}

void carFSM(I3DEngine* myEngine, IModel* Matchbox, IModel* MatchFront, IModel* MatchRear, IModel* Bullet, int& carState,
	float Speed, float& MatchFrontRot, float RotationLimit, float RotationSpeed, float& SteerAngle, float SteerAngleIncrease)
{
	switch (carState)
	{
	case 0:
	{
		// Car is healthy. Control it with buttons.

		// (a) Update:
		carDynamics(myEngine, Matchbox, MatchFront, MatchRear, Speed, MatchFrontRot, RotationLimit, RotationSpeed, SteerAngle, SteerAngleIncrease);

		// (b) Transitions:
		if (collisionDetection(Matchbox, Bullet, carState))
			carState = 1;

	}
	break;
	case 1:
		// Car is broken. Do nothing.
	
		// (a) Update 

		// (b)
		break;
	}
}

void updateBullet(I3DEngine* myEngine, IModel* Bullet, IModel* Matchbox, IModel* Turret, int& bulletState,
	IMesh* BulletMesh) {

	switch (bulletState)
	{
	case Bullet_Flying: {
		Bullet->MoveLocalZ(0.6);
		}
		if (collisionDetection(Matchbox, Bullet, bulletState)) {
			bulletState = Bullet_NotFlying;
			//Need to include an if statement for the collision error
			//BulletMesh->RemoveModel(Bullet);
		}
		break;

	case Bullet_NotFlying: {
		Bullet->DetachFromParent();
		Bullet->SetLocalPosition(0, 0, 0);
		Bullet->AttachToParent(Matchbox);
		


		if (myEngine->KeyHit(Key_R)) {
			Bullet->SetLocalPosition(0, 0, 0);
			Bullet->AttachToParent(Turret);
			bulletState = Bullet_Flying;
		}
	}
		break;
	}
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
	const int knBullets = 10;
	IModel* aBullets[knBullets];

	// Create array of bullet models:
	for (int i = 0; i < knBullets; i++) {
		aBullets[i] = BulletMesh->CreateModel(0, 0, 0);
	}

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

	// Attach bullet to cannon
	Bullet->AttachToParent(Turret);

	camera->AttachToParent(Matchbox);

	// MatchBox car variables
	float Speed = 0.6f;
	float RotationSpeed = 0.2f;
	float RotationLimit = 25.0f;
	float MatchFrontRot = 0.0f;
	float SteerAngle = 0.0f;
	float SteerAngleIncrease = 0.008f;
	int carState = 0;
	int bulletState = 0;
	int aBulletState[knBullets];
	for (int i = 0; i < knBullets; i++) {
		aBulletState[i] = Bullet_NotFlying;
	}

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		//Update Turret
		Turret->LookAt(Matchbox);

		//Update Bomb
		updateBullet(myEngine,Bullet,Matchbox,Turret,bulletState,BulletMesh);

		for(int i=0; i<knBullets; i++)
			updateBullet(myEngine,aBullets[i],Matchbox,Turret,aBulletState[i],BulletMesh);

		updateCar(myEngine, Matchbox, MatchFront, MatchRear, Speed, MatchFrontRot, RotationLimit, RotationSpeed, SteerAngle, SteerAngleIncrease, Bullet, carState);
	
		gameControls(myEngine);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
