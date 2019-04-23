//Floris Alexandrou

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "CHover.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include <iostream>
#include <sstream>
using namespace tle;

#define Demo 0
#define Count_Down 1
#define Stage_0 2 
#define Stage_1 3
#define Stage_2 4
#define Stage_3 5
#define Stage_4 6
#define Complete 7
#define First_Person 1
#define Free_Moving 2
#define Third_Person 3
#define Surveillance 4
#define Going_Up 0
#define Going_Down 1
#define Waypoint_1 1
#define Waypoint_2 2
#define Waypoint_3 3
#define Waypoint_4 4
#define enemyTurn 6
#define vertical 0
#define horizontal 1

//Function Definitions
void restartGame(CPlayer& p1, CEnemy aEnemies[], IModel* aCheckpoints[], IModel* cross,
	int& currentCheckpoint, int& currentLap, int& stateGame, float& startTimer, float crossY) {
	const int kNumEnemies = 5;
	currentCheckpoint = 0;
	currentLap = 1;
	startTimer = 4;
	p1.SetHealth(100);
	p1.SetSpeed(0);
	p1.GetModel()->SetLocalPosition(0, 0, 30);
	p1.GetModel()->ResetOrientation();
	p1.GetModel()->SetSkin("sp02-01.jpg");
	aEnemies[0].GetModel()->SetPosition(-10, 0, 30);
	aEnemies[1].GetModel()->SetPosition(0, 0, 50);
	aEnemies[2].GetModel()->SetPosition(10, 0, 30);
	aEnemies[3].GetModel()->SetPosition(10, 0, 50);
	aEnemies[4].GetModel()->SetPosition(-10, 0, 50);
	aCheckpoints[0]->SetLocalPosition(0, 0, 100);
	aCheckpoints[1]->SetLocalPosition(85, 0, 183.5);
	aCheckpoints[2]->SetLocalPosition(170, 0, 100);
	aCheckpoints[3]->SetLocalPosition(85, 0, 13.5);
	cross->MoveLocalY(-crossY);
	for (int i = 0; i < kNumEnemies; i++) {
		aEnemies[i].GetModel()->ResetOrientation();
		aEnemies[i].SetWaypoint(Waypoint_1);
		aEnemies[i].SetSpeed(0);
		aEnemies[i].ResetTurningAngle();
	}
	stateGame = Demo;
}

void changeSkin(I3DEngine* myEngine, CPlayer p1, int& stateSkin) {
	switch (stateSkin)
	{
	case 0: {
		if (myEngine->KeyHit(Key_Q)) {
			stateSkin = 1;
			p1.GetModel()->SetSkin("Fire.JPG");
		}
	}
			break;
	case 1: {
		if (myEngine->KeyHit(Key_Q)) {
			stateSkin = 2;
			p1.GetModel()->SetSkin("top.jpg");
		}
	}
			break;
	case 2: {
		if (myEngine->KeyHit(Key_Q)) {
			stateSkin = 0;
			p1.GetModel()->SetSkin("sp02-01.jpg");
		}
	}
			break;
	}
}

void displayText(IFont* Font, int currentCheckpoint, int currentLap, CPlayer p1) {
	//Stringstrams
	stringstream sCheckpoint;
	stringstream sLap;
	stringstream sHoverSpeed;
	stringstream sBoostMeter;
	stringstream sHP;

	//Display text on screen
	sCheckpoint << "Checkpoint: " << currentCheckpoint << " /4";
	Font->Draw(sCheckpoint.str(), 230, 660);
	sLap << "Lap: " << currentLap << " /3";
	Font->Draw(sLap.str(), 50, 660);
	sHoverSpeed << "Speed: " << int(p1.GetSpeed()) << " KM/H";
	Font->Draw(sHoverSpeed.str(), 530, 660);
	sBoostMeter << "Boost: " << int(p1.GetBoostMeter());
	Font->Draw(sBoostMeter.str(), 830, 660);
	sHP << "HP: " << int(p1.GetHealth());
	Font->Draw(sHP.str(), 1050, 660);
}

void setupScene(IMesh* isleMesh, IMesh* wallMesh, IMesh* tankMesh, IMesh* checkpointMesh, IMesh* flareMesh,
	IModel* aIsles[], IModel* aWalls[], IModel* aTanks[], IModel* aCheckpoints[], IModel* aWaypoints[]) {
	//constants
	const int kNumIsles = 80;
	const int kNumWalls = 72;
	const int kNumTanks = 36;
	const int kNumCheckpoints = 4;
	const int kNumWaypoints = 10;

	//variables
	float isleX = 20;
	float isleZ = 40;
	float wallX = -20;
	float wallZ = 46;
	float tankX = 20;
	float tankZ = 165;

	//Checkpoints
	aCheckpoints[0] = checkpointMesh->CreateModel(0, 0, 100);
	aCheckpoints[1] = checkpointMesh->CreateModel(85, 0, 183.5);
	aCheckpoints[1]->RotateLocalY(90);
	aCheckpoints[2] = checkpointMesh->CreateModel(170, 0, 100);
	aCheckpoints[3] = checkpointMesh->CreateModel(85, 0, 13.5);
	aCheckpoints[3]->RotateLocalY(90);

	//Waypoints
	aWaypoints[0] = flareMesh->CreateModel(0, -2, 140);
	aWaypoints[1] = flareMesh->CreateModel(130, -2, 180);
	aWaypoints[2] = flareMesh->CreateModel(170, -2, 55);
	aWaypoints[3] = flareMesh->CreateModel(40, -2, 15);

	//ISLES
	//Left Vertical Rows
	for (int i = 0; i < kNumIsles / 4; i++) {
		if (i == kNumIsles / 8) {
			isleX = 1 * -isleX;
			isleZ = 40;
		}
		aIsles[i] = isleMesh->CreateModel(isleX, 0, isleZ);
		isleZ += 13;
	}

	isleZ -= 7;
	isleX += 45;
	float tempIsleX = isleX;

	//Front Horizontal Rows
	for (int i = kNumIsles / 4; i < kNumIsles / 2; i++) {
		if (i == kNumIsles / 4 + ((kNumIsles / 2) / 4)) {
			isleX = tempIsleX;
			isleZ += 40;
		}
		aIsles[i] = isleMesh->CreateModel(isleX, 0, isleZ);
		aIsles[i]->RotateLocalY(90);
		isleX += 13;
	}

	isleX -= 5;
	isleZ -= 45;
	float tempIsleZ = isleZ;

	//Right Vertical Rows
	for (int i = kNumIsles / 2; i < kNumIsles - (kNumIsles / 4); i++) {
		if (i == kNumIsles / 2 + ((kNumIsles / 2) / 4)) {
			isleX += 40;
			isleZ = tempIsleZ;
		}
		aIsles[i] = isleMesh->CreateModel(isleX, 0, isleZ);
		isleZ -= 13;
	}

	isleX -= 45;
	tempIsleX = isleX;
	isleZ += 5;
	//Back Horizontal Rows
	for (int i = kNumIsles - (kNumIsles / 4); i < kNumIsles; i++) {
		if (i == kNumIsles - ((kNumIsles / 2) / 4)) {
			isleX = tempIsleX;
			isleZ -= 40;
		}
		aIsles[i] = isleMesh->CreateModel(isleX, 0, isleZ);
		aIsles[i]->RotateLocalY(90);
		isleX -= 13;
	}

	//WALLS
	//Left Vertical Rows
	for (int i = 0; i < kNumWalls / 4; i++) {
		if (i == kNumWalls / 8) {
			wallX = 1 * -wallX;
			wallZ = 46;
		}
		aWalls[i] = wallMesh->CreateModel(wallX, 0, wallZ);
		wallZ += 13;
	}


	wallX += 13;
	float tempWallX = wallX;

	//Front Horizontal Rows
	for (int i = kNumWalls / 4; i < kNumWalls / 2; i++) {
		if (i == kNumWalls / 4 + ((kNumWalls / 2) / 4)) {
			wallZ += 40;
			wallX = tempWallX;
		}
		aWalls[i] = wallMesh->CreateModel(wallX, 0, wallZ);
		aWalls[i]->RotateLocalY(90);
		wallX += 13;
	}

	wallZ -= 53;
	float tempWallZ = wallZ;

	//Right Verical Rows
	for (int i = kNumWalls / 2; i < kNumWalls - (kNumWalls / 4); i++) {
		if (i == kNumWalls / 2 + ((kNumWalls / 2) / 4)) {
			wallX += 40;
			wallZ = tempWallZ;
		}
		aWalls[i] = wallMesh->CreateModel(wallX, 0, wallZ);
		wallZ -= 13;
	}

	wallX -= 53;
	tempWallX = wallX;

	//Back Horizontal Rows
	for (int i = kNumWalls - (kNumWalls / 4); i < kNumWalls; i++) {
		if (i == kNumWalls - ((kNumWalls / 2) / 4)) {
			wallZ -= 40;
			wallX = tempWallX;
		}
		aWalls[i] = wallMesh->CreateModel(wallX, 0, wallZ);
		aWalls[i]->RotateLocalY(90);
		wallX -= 13;
	}

	//TANKS
	for (int i = 0; i < kNumTanks; i++) {
		if (i < 9) {
			aTanks[i] = tankMesh->CreateModel(-tankX, 0, tankZ);
			tankZ += 10;
			if (i > 3) {
				tankX -= 10;
				tankZ -= 10;
			}
		}
		else if (i < 18) {
			if (i == 9) {
				tankX = 150;
				tankZ = 205;
			}
			aTanks[i] = tankMesh->CreateModel(tankX, 0, tankZ);
			tankX += 10;
			if (i > 12) {
				tankX -= 10;
				tankZ -= 10;
			}

		}
		else if (i < 27) {
			if (i == 18) {
				tankX = 190;
				tankZ = 35;
			}
			aTanks[i] = tankMesh->CreateModel(tankX, 0, tankZ);
			tankZ -= 10;
			if (i > 21) {
				tankX -= 10;
				tankZ += 10;
			}
		}
		else {
			if (i == 27) {
				tankX = 20;
				tankZ = -5;
			}
			aTanks[i] = tankMesh->CreateModel(tankX, 0, tankZ);
			tankX -= 10;
			if (i > 30) {
				tankX += 10;
				tankZ += 10;
			}
		}
	}
}

bool sphere2sphereCollisionDetection(IModel* Model1, IModel* Model2, float mRadius1, float mRadius2, float& collisionTimer)

{
	bool collision = false;
	float timeToCollide = 0.03;
	//location of model1
	float p1x = Model1->GetX();
	float p1y = Model1->GetY();
	float p1z = Model1->GetZ();

	//location of model2
	float p2x = Model2->GetX();
	float p2y = Model2->GetY();
	float p2z = Model2->GetZ();

	//distance between model1 and model2
	float dx = p1x - p2x;
	float dy = p1y - p2y;
	float dz = p1z - p2z;
	float collisionDist = sqrt(dx*dx + dy * dy + dz * dz);

	if (collisionDist < mRadius1 + mRadius2 && collisionTimer > timeToCollide) {
		collision = true;
		collisionTimer = 0;
	}
	return collision;
}

bool collisionDetectionCheckpointLegs(IModel* Model1, IModel* Model2, float mRadius1, float& collisionTimer, int alignment)

{
	bool collision = false;
	float timeToCollide = 0.03;
	//location of model1
	float p1x = Model1->GetX();
	float p1z = Model1->GetZ();
	float collisionDist;
	float collisionDist_2;

	if (alignment == horizontal) {
		//location of model2
		float p2x = Model2->GetX() - 8;
		float p2x_2 = Model2->GetX() + 8;
		float p2z = Model2->GetLocalZ();
		//distance between model1 and model2
		float dx = p1x - p2x;
		float dx_2 = p1x - p2x_2;
		float dz = p1z - p2z;
		collisionDist = sqrt(dx*dx + dz * dz);
		collisionDist_2 = sqrt(dx_2*dx_2 + dz * dz);
	}
	else if (alignment == vertical) {
		float p2x = Model2->GetX();
		float p2z = Model2->GetLocalZ() - 8;
		float p2z_2 = Model2->GetLocalZ() + 8;
		//distance between model1 and model2
		float dx = p1x - p2x;
		float dz = p1z - p2z;
		float dz_2 = p1z - p2z_2;
		collisionDist = sqrt(dx*dx + dz * dz);
		collisionDist_2 = sqrt(dx*dx + dz_2 * dz_2);
	}


	//Left leg
	if (collisionDist < mRadius1 -1 && collisionTimer > timeToCollide) {
		collision = true;
		collisionTimer = 0;
	}
	//Right leg
	else if (collisionDist_2 < mRadius1 - 1 && collisionTimer > timeToCollide) {
		collision = true;
		collisionTimer = 0;
	}
	return collision;
}

bool sphere2boxCollisionDetection(IModel* Sphere, IModel* Box, float sphereRadius, float& collisionTimer)
{
	bool collision = false;
	float timeToCollide = 0.03;
	float closeX = 0;
	//location of Sphere
	float p1x = Sphere->GetX();
	float p1y = Sphere->GetY();
	float p1z = Sphere->GetZ();

	//location of Box
	float p2x = Box->GetX();
	float p2y = Box->GetY();
	float p2z = Box->GetZ();

	//Code from lecture slides
	if (p1x < p2x + sphereRadius && p1x > p2x - sphereRadius &&
		p1y < p2y + sphereRadius && p1y > p2y - sphereRadius &&
		p1z < p2z + sphereRadius && p1z > p2z - sphereRadius)
	{	
		collision = true;
	}

	return collision;
}

void cameraFSM(I3DEngine* myEngine, ICamera* camera, IModel* hover, int& stateCamera) {

	//Transitions for all states
	if (myEngine->KeyHit(Key_1) && stateCamera != First_Person){
		camera->SetLocalPosition(0, 5, 3);
		camera->AttachToParent(hover);
		camera->ResetOrientation();
		camera->RotateLocalX(10);
		stateCamera = First_Person;
	}
	
	if (myEngine->KeyHit(Key_2) && stateCamera != Free_Moving) {
		camera->SetLocalPosition(0, 100, -150);
		camera->DetachFromParent();
		camera->ResetOrientation();
		camera->RotateLocalX(25);
		stateCamera = Free_Moving;
	}
		
	if (myEngine->KeyHit(Key_3) && stateCamera != Third_Person) {
		camera->SetLocalPosition(0, 30, -50);
		camera->AttachToParent(hover);	
		camera->ResetOrientation();
		camera->RotateLocalX(25);
		stateCamera = Third_Person;
	}
	if (myEngine->KeyHit(Key_4) && stateCamera != Surveillance) {
		camera->SetLocalPosition(-100, 150, -10);
		camera->DetachFromParent();
		stateCamera = Surveillance;
	}
	//The 4 camera states	
	switch (stateCamera)
	{
		
	case First_Person: {
		if (myEngine->KeyHit(Key_C)) {
			camera->SetLocalPosition(0, 5, 3);
			camera->ResetOrientation();
			camera->RotateLocalX(10);
		}
	}
					   break;
	case Free_Moving: {
		if (myEngine->KeyHit(Key_C)) {
			camera->SetLocalPosition(0, 100, -150);
			camera->ResetOrientation();
			camera->RotateLocalX(25);
		}
	}
					  break;
	case Third_Person: {
		if (myEngine->KeyHit(Key_C)) {
			camera->SetLocalPosition(0, 30, -50);
			camera->ResetOrientation();
			camera->RotateLocalX(25);
		}
	}
					   break;
	case Surveillance: {
		camera->LookAt(hover);	
		if (myEngine->KeyHit(Key_C)) {
			camera->SetLocalPosition(-100, 150, -10);
		}
	}
					   break;
	}
}

void updateStageStates(I3DEngine* myEngine, CPlayer& p1, CEnemy aEnemies[],
	IModel* aWaypoints[], IModel* aWalls[], IModel* aIsles[], IModel* aTanks[], IModel* aCheckpoints[], IModel* cross,
	int tankRadius, int currentCheckpoint, int& stateGameTemp, int& stateGame,
	float dt, float& collisionTimer, float healthDecrease, float& crossTimer, float crossY) {
	if (stateGame == Stage_0 || stateGame == Stage_1 ||
		stateGame == Stage_2 || stateGame == Stage_3 ||
		stateGame == Stage_4)
	{
		const int kNumEnemies = 5;
		const int kNumIsles = 80;
		const int kNumWalls = 72;
		const int kNumTanks = 36;
		const int kNumWaypoints = 10;
		//Player Movement
		p1.Movement(dt);
		p1.Boost(dt);

		//Enemy Movement
		for (int i = 0; i < kNumEnemies; i++) {
			aEnemies[i].Movement(aWaypoints, collisionTimer, dt);
		}

		//Player Dies
		if (p1.GetHealth() < 0) {
			p1.GetModel()->SetSkin("ui_backdrop.jpg");
			stateGame = Complete;
		}

		//Collision with Isles
		for (int i = 0; i < kNumIsles; i++)
		{
			if (sphere2boxCollisionDetection(p1.GetModel(), aIsles[i], p1.GetRadius(), dt)) {
				p1.CollisionResolution(dt);
			}
		}

		//Collision with Walls
		for (int i = 0; i < kNumWalls; i++)
		{
			if (sphere2boxCollisionDetection(p1.GetModel(), aWalls[i], p1.GetRadius(), dt)) {
				p1.CollisionResolution(dt);
			}
		}

		//Collision with Tanks
		for (int i = 0; i < kNumTanks; i++) {
			if (sphere2sphereCollisionDetection(p1.GetModel(), aTanks[i], p1.GetRadius(), tankRadius, collisionTimer)) {
				p1.CollisionResolution(dt);
			}
		}

		//Collision with checkpoints' legs
		if (collisionDetectionCheckpointLegs(p1.GetModel(), aCheckpoints[0], p1.GetRadius(), collisionTimer, horizontal)) {
			p1.CollisionResolution(dt);
		}
		if (collisionDetectionCheckpointLegs(p1.GetModel(), aCheckpoints[1], p1.GetRadius(), collisionTimer, vertical)) {
			p1.CollisionResolution(dt);
		}
		if (collisionDetectionCheckpointLegs(p1.GetModel(), aCheckpoints[2], p1.GetRadius(), collisionTimer, horizontal)) {
			p1.CollisionResolution(dt);
		}
		if (collisionDetectionCheckpointLegs(p1.GetModel(), aCheckpoints[3], p1.GetRadius(), collisionTimer, vertical)) {
			p1.CollisionResolution(dt);
		}

		//PvE Collision Detection
		for (int i = 0; i < kNumEnemies; i++) {
			if (sphere2sphereCollisionDetection(p1.GetModel(), aEnemies[i].GetModel(), p1.GetRadius(), aEnemies[i].GetRadius(), collisionTimer)) {
				aEnemies[i].SetCollision(true);
				p1.SetHealth(p1.GetHealth() - healthDecrease);
			}
		}

		//Smooth Collision Resolution
		for (int i = 0; i < kNumEnemies; i++) {
			aEnemies[i].CollisionResolution(p1.GetModel(), dt);
		}

		//Reset Location to last checkpoint (useful if player gets stuck)
		if (myEngine->KeyHit(Key_R) && currentCheckpoint > 0) {
			p1.GetModel()->SetLocalPosition(aCheckpoints[currentCheckpoint - 1]->GetLocalX(), 0, aCheckpoints[currentCheckpoint - 1]->GetLocalZ());
			p1.SetSpeed(0);
		}

		//Checkpoint X symbol
		//Detect stateGame change and display the X model accordingly
		if (stateGame != stateGameTemp && currentCheckpoint > 0) {
			cross->AttachToParent(aCheckpoints[currentCheckpoint - 1]);
			cross->SetLocalY(crossY);
			crossTimer += dt;
			if (crossTimer > 1) {
				cross->SetLocalY(-crossY);
				stateGameTemp = stateGame;
				crossTimer = 0;
			}
		}
	}
}

//----//
//MAIN//
//----//
void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("\Media");

	/**** Set up your scene here ****/

	//General Variables

	int stateGame = Demo;
	int stateSkin = 0;
	int stateGameTemp = Demo;
	int stateCamera = Free_Moving;
	int currentCheckpoint = 0;
	int currentLap = 1;
	int maxLaps = 4;
	float cameraSpeed = 100;
	float checkpointRadius = 3;
	float startTimer = 4;
	float collisionTimer = 0;
	float tankRadius = 5;
	float healthDecrease = 0.2;
	float crossY = 10;
	float crossTimer = 0;
	float interSpeed = 35;
	const int kNumIsles = 80;
	const int kNumWalls = 72;
	const int kNumTanks = 36;
	const int kNumCheckpoints = 4;
	const int kNumWaypoints = 10;
	const int initialCheckpoint = 1;
	const int kNumEnemies = 5;
	const int kNumParticles = 4;
	float particleVelocity = 50;
	float particleGravity = -50;

	//arrays
	IModel* aIsles[kNumIsles];
	IModel* aWalls[kNumWalls];
	IModel* aTanks[kNumTanks];
	IModel* aCheckpoints[kNumCheckpoints];
	IModel* aWaypoints[kNumWaypoints];

	//meshes
	IMesh* skyBoxMesh = myEngine->LoadMesh("Skybox 07.x");
	IMesh* groundMesh = myEngine->LoadMesh("Ground.x");
	IMesh* checkpointMesh = myEngine->LoadMesh("Checkpoint.x");
	IMesh* isleMesh = myEngine->LoadMesh("IsleStraight.x");
	IMesh* wallMesh = myEngine->LoadMesh("Wall.x");
	IMesh* hoverMesh = myEngine->LoadMesh("race2.x");
	IMesh* tankMesh = myEngine->LoadMesh("TankSmall2.x");
	IMesh* tribuneMesh = myEngine->LoadMesh("Tribune1.x");
	IMesh* crossMesh = myEngine->LoadMesh("Cross.x");
	IMesh* flareMesh = myEngine->LoadMesh("Flare.x");
	IMesh* interstellarMesh = myEngine->LoadMesh("Interstellar.x");

	//models
	IModel* skyBox = skyBoxMesh->CreateModel(0, -960, 0);
	IModel* ground = groundMesh->CreateModel();
	IModel* interstellar = interstellarMesh->CreateModel();
	IModel* dummy = flareMesh->CreateModel(90, -2, 90);
	IModel* cross = crossMesh->CreateModel(0,-5,0);
	cross->Scale(0.2);
	interstellar->AttachToParent(dummy);
	interstellar->SetLocalPosition(-70, 30, 0);
	interstellar->RotateZ(-40);
	
	setupScene(isleMesh, wallMesh, tankMesh, checkpointMesh, flareMesh, aIsles, aWalls, aTanks, aCheckpoints, aWaypoints);

	//sprite
	ISprite* backdrop = myEngine->CreateSprite("backdrop.jpg", 0, 650, 0);

	//camera
	ICamera* camera = myEngine->CreateCamera(kFPS , 0, 100, -150);
	camera->SetMovementSpeed(cameraSpeed);
	camera->SetRotationSpeed(cameraSpeed);
	camera->RotateLocalX(25);

	//text
	IFont* Font = myEngine->LoadFont("Arial", 40);

	//Objects
	CPlayer p1(hoverMesh, myEngine);
	CEnemy aEnemies[kNumEnemies];
	for (int i = 0; i < kNumEnemies; i++) {
		aEnemies[i] = { hoverMesh, myEngine, "RedGlow.jpg" };
	}

	aEnemies[0].GetModel()->SetPosition(-10, 0, 30);	
	aEnemies[1].GetModel()->SetPosition(0, 0, 50);
	aEnemies[2].GetModel()->SetPosition(10, 0, 30);
	aEnemies[3].GetModel()->SetPosition(10, 0, 50);
	aEnemies[4].GetModel()->SetPosition(-10, 0, 50);
	
	//particles
	IModel* aParticles[4];
	float aParticleVelocity[4];
	float aParticleGravity[4];
	for (int i = 0; i < kNumParticles; i++) {
		aParticleVelocity[i] = 50;
		aParticleGravity[i] = -80;
	}
	aParticles[0] = flareMesh->CreateModel(28, 0, 42);
	aParticles[1] = flareMesh->CreateModel(28, 0, 152);
	aParticles[2] = flareMesh->CreateModel(138, 0, 42);
	aParticles[3] = flareMesh->CreateModel(138, 0, 152);


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		//Draw Scene
		myEngine->DrawScene();

		float dt = myEngine->Timer();
		collisionTimer += dt;

		stringstream sTimer;
		displayText(Font, currentCheckpoint, currentLap, p1);
		
		//Moving bombs (decoration)
		for (int i = 0; i < kNumParticles; i++) {
			aParticles[i]->MoveY(aParticleVelocity[i] * dt);
			aParticleVelocity[i] = aParticleVelocity[i] + aParticleGravity[i] * dt;
			aParticles[i]->LookAt(camera);
			if (aParticles[i]->GetY() < 0) {
				aParticleVelocity[i] = 50;
			}
		}
		
		//Hovers hovering... if that makes sense
		p1.Wobble(dt);
		for (int i = 0; i < kNumEnemies; i++) {
			aEnemies[i].Wobble(dt);
		}

		//Interstellar patrol
		dummy->RotateLocalY(interSpeed * dt);

		//-----------------------//
		//----Main Game State----//
		//-----------------------//
		switch (stateGame)
		{
		case Demo: {
			//Display Welcome Message
			Font->Draw("Welcome, Press Space to start\nControls\nWASD - Move\nSpacebar - Boost\nR - Reset Position\nQ - Change Skin", 250, 250);

			changeSkin(myEngine, p1, stateSkin);
			
			if (myEngine->KeyHit(Key_Space)) {
				stateGame = Count_Down;
			}
		}
				   break;
		case Count_Down: {
			//Count to 3
			sTimer << "Time to start: " << int(startTimer);
			Font->Draw(sTimer.str(), 250, 250);
			startTimer -= dt;
			if (startTimer < 1) {
				stateGame = Stage_0;
			}
		}
					break;
		case Stage_0: {
			//Display "GO!"
			if (startTimer > 0) {
				startTimer -= dt;
				Font->Draw("GO!", 250, 250);
			}
			
			//Checkpoint			
			if (sphere2sphereCollisionDetection(p1.GetModel(), aCheckpoints[0], p1.GetRadius(), checkpointRadius, collisionTimer)) {
				startTimer = -1; //HotFix to a bug: "GO" reappeared after checkpoint 4.
				stateGame = Stage_1;
				currentCheckpoint = initialCheckpoint;
			}
		}
					  break;
		case Stage_1: {		
			//Checkpoint
			if (sphere2sphereCollisionDetection(p1.GetModel(), aCheckpoints[1], p1.GetRadius(), checkpointRadius, collisionTimer)) {
				stateGame = Stage_2;
				currentCheckpoint++;
			}
		}
					  break;
		case Stage_2: {
			//Checkpoint
			if (sphere2sphereCollisionDetection(p1.GetModel(), aCheckpoints[2], p1.GetRadius(), checkpointRadius, collisionTimer)) {
				stateGame = Stage_3;
				currentCheckpoint++;
			}
		}
					  break;
		case Stage_3: {
			//Checkpoint
			if (sphere2sphereCollisionDetection(p1.GetModel(), aCheckpoints[3], p1.GetRadius(), checkpointRadius, collisionTimer)) {
				stateGame = Stage_4;
				currentCheckpoint++;
			}
		}
					  break;
		case Stage_4: {
			currentLap++;
			for (int i = 0; i < kNumCheckpoints; i++) {
				aCheckpoints[i]->MoveLocalX(rand() % 11 + (-5));
			}
			stateGame = Stage_0;

			if (currentLap == maxLaps) {
				stateGame = Complete;
				currentLap--; //To fix bug displaying 4/3 Laps
			}
		}
					   break;
		case Complete: {
			Font->Draw("Game Over, Thank you for playing!!\nPress Enter to Restart or Escape top Exit", 250, 250);
			if (myEngine->KeyHit(Key_Return)) {
				restartGame(p1, aEnemies, aCheckpoints, cross, currentCheckpoint, currentLap, stateGame, startTimer, crossY);
			}
		}
					   break;
		}	//End Of Main Switch

		
		//Updates that happen in all "active" states
		//Created to make code smaller by writing code 1 time instead of once every stage
		updateStageStates(myEngine, p1, aEnemies, aWaypoints, aWalls, aIsles, aTanks, aCheckpoints, cross, tankRadius, currentCheckpoint,
			stateGameTemp, stateGame, dt, collisionTimer, healthDecrease, crossTimer, crossY);
	
		cameraFSM(myEngine, camera, p1.GetModel(), stateCamera);
	
		if (myEngine->KeyHit(Key_Escape)) {
			myEngine->Stop();
		}

	} //end of main loop

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}