//Floris Alexandrou
// Caterpillar.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <iostream>
#include <time.h>
#include <sstream>
using namespace tle;

#define Ready 0
#define Firing 1
#define Contact 2
#define Over 3
#define Pause 4
#define Win 5

#define Healthy 0
#define Damaged 1
#define Critical 2
#define Dead 3


#define Moving_Right 0
#define Moving_Left 1
#define Moving_Down 2

//Function Declarations

void updateCamera(I3DEngine* myEngine, ICamera* camera, IModel* dummy, float camMaxY, float camMinY, float dummySpeed, float dt);

bool collisionDetection(IModel* Model1, IModel* Model2, float mRadius1, float mRadius2, float& collisionTimer);

void mushroomFSM(IModel* aMushrooms, IMesh* mushroomMesh, int& aStateShrooms, int& currentScore);

void deadSegmentFSM(IModel* aDeadSegments, IMesh* mushroomMesh, int& aStateDeadSegments, int& currentScore);

void segmentFSM(IModel* aSegments, IModel* aMushrooms[], IMesh* segmentMesh, IMesh* mushroomMesh, int aStateSegments, int& segmentKills, int& varNumShrooms, int& aStateShrooms, int& aStateDeadSegments, float mushroomSinkY, bool& segmentation, int& currentScore);

void arrowMarbleMovement(I3DEngine* myEngine, IModel* marble, IModel* marble2, IModel* marble3, IModel* dummyMarb, float marbleHandlingSpeed, float marbleRotationSpeed, float marbleMinX, float marbleMaxX,
	float& arrowAngle, float arrowMaxAngle, float arrowAngleInc, float dt, int StateGame, bool aBurst[]);

void segmentMovementFSM(IModel* aSegments, int& aStateWormDirection, float wormSpeed, float dt, float wormMaxX, float& getNextSquare, bool& abSeg2Shroom, int& previewsDirection);

void resetMarble(IModel* marble, IModel* dummyMarb, float arrowAngle);

void resetGame(IMesh* segmentMesh, IMesh* mushroomMesh, IModel* aMushrooms[], IModel* aSegments[], IModel* aDeadSegments[],
	int aStateSegments[], int aStateShrooms[], int aStateDeadSegments[], int aStateWormDirection[], bool abSeg2Shroom[], int kNumSegments, int kNumShrooms, int& varDeadSegments, float reverse);

void updateReadyAndFiring(I3DEngine* myEngine, IMesh* mushroomMesh, IMesh* segmentMesh, IModel* marble, IModel* marble2, IModel* marble3, IModel* dummyMarb,
	IModel* aMushrooms[], IModel* aSegments[], IModel* aDeadSegments[], IModel* aBarriers[],
	int aStateSegments[], int aStateWormDirection[], int aStateShrooms[], int aStateDeadSegments[],
	int& StateGame, int kNumSegments, int kNumShrooms, int previewsDirection, int& varDeadSegments,
	int& burstIterator, int& segmentKills, int& previewsState, int kNumBarriers, int& score, int indexOfSegmentation, int wormStart,
	float wormSpeed, float dt, float wormMaxX, float& getNextSquare, float gameLosingZ, float& marble2BounceRate,
	float& marble3BounceRate, float& arrowAngle, float marbleFiringSpeed, float mushroomSinkY, float mushroomSpeed,
	float marbleMinZ, float marbleMaxZ, float marbleMinX, float marbleMaxX, float marbleRadius, float barrierRadius,
	float shroomRadius, float segmentRadius, float collisionTimer,
	bool abSeg2Shroom[], bool aBurst[], bool segmentation);

void firingTransitions(IMesh* mushroomMesh, IMesh* segmentMesh, IModel* marble, IModel* aDeadSegments[], IModel* aMushrooms[], IModel* aSegments[], IModel* aBarriers[],
	int kNumShrooms, int& score, int& StateGame, int kNumSegments, int& segmentKills, int& varDeadSegments, int indexOfSegmentation, int kNumBarriers,
	int aStateShrooms[], int aStateSegments[], int aStateDeadSegments[],
	float marbleRadius, float shroomRadius, float segmentRadius, float collisionTimer, float mushroomSinkY, float barrierRadius, float& marbleBounceRate,
	float marbleMaxZ, float marbleMinZ, float marbleMaxX, float marbleMinX, bool segmentation);

void gameControls(I3DEngine* myEngine);

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("\Media");

	/**** Set up your scene here ****/

	//General Game Variables And Constants
	float camMaxY = 200;
	float camMinY = 20;
	float marbleMaxZ = 210;
	float marbleMinZ = 0;
	float marbleMaxX = 80;
	float marbleMinX = -80;
	float marbleHandlingSpeed = 50;
	float marbleFiringSpeed = 150;
	float marbleRotationSpeed = marbleFiringSpeed * 4;
	float marbleBounceRate = 0;
	float marble2BounceRate = 0;
	float marble3BounceRate = 0;
	float marbleRadius = 4;
	float shroomRadius = 4;
	float barrierRadius = 6;
	float segmentRadius = 6;
	float segment2MushroomRadius = 8;
	float arrowAngle = 0;
	float arrowAngleInc = 600;
	float arrowMaxAngle = 75;
	float dummyCamSpeed = 20;
	float mushroomSpeed = 3;
	float mushroomSinkY = -8;
	float barrierDistLeft = 0;
	float barrierDistRight = 0;
	float bushDist = -90;
	float timer = 0;
	float collisionTimer = 0;
	float wormSpeed = 30;
	float wormMaxX = 75;
	float getNextSquare = 0;
	float gameLosingZ = 10;
	float wormXAxis = -75;
	float reverse = 180;
	const float initialWormSpeed = 30;
	int segmentKills = 0;
	int varDeadSegments = 0;
	int collisionCounter = 0;
	int score = 0;
	int wave = 1;
	int waveX = 30;
	int waveY = 20;
	int scoreX = 30;
	int scoreY = 70;
	int fontCenterX = 250;
	int fontCenterY = 250;
	int previewsStateGame = Ready;
	int StateGame = Pause;
	int burstIterator = 0;
	int wormStart = 0;
	int indexOfSegmentation = 0;
	int previewsDirection = 0;
	const int kNumBushes = 10;
	const int kNumShrooms = 10;
	const int kNumBarriers = 22;
	const int kNumSegments = 10;
	const int maxShroomSize = 10;
	int aStateSegments[kNumSegments];
	int aStateShrooms[kNumShrooms];
	int aStateDeadSegments[kNumShrooms];
	int aStateWormDirection[kNumSegments];
	bool segmentation = false;
	bool abSeg2Shroom[kNumSegments];
	bool aBurst[2] = { false,false };
	

	//Text
	IFont* Font_CurrentScore = myEngine->LoadFont("Arial", 40);
	IFont* Font_Wave = myEngine->LoadFont("Arial", 40);
	IFont* Font_Welcome = myEngine->LoadFont("Arial", 40);
	IFont* Font_Win = myEngine->LoadFont("Arial", 40);
	IFont* Font_Over = myEngine->LoadFont("Arial", 40);


	//Meshes
	IMesh* groundMesh = myEngine->LoadMesh("Ground.x");
	IMesh* skyBoxMesh = myEngine->LoadMesh("Skybox_SciFi.x");
	IMesh* marbleMesh = myEngine->LoadMesh("Marble.x");
	IMesh* mushroomMesh = myEngine->LoadMesh("Mushroom.x");
	IMesh* arrowMesh = myEngine->LoadMesh("Arrow.x");
	IMesh* bushMesh = myEngine->LoadMesh("Bush.x");
	IMesh* dummyMesh = myEngine->LoadMesh("Dummy.x");
	IMesh* barrierMesh = myEngine->LoadMesh("Barrier.x");
	IMesh* segmentMesh = myEngine->LoadMesh("Segment.x");


	//Models
	IModel* ground = groundMesh->CreateModel();
	IModel* skyBox = skyBoxMesh->CreateModel(0, -1000, 0);
	IModel* marble = marbleMesh->CreateModel(0, 2, 0);
	IModel* marble2 = marbleMesh->CreateModel(0, 0, 0);
	IModel* marble3 = marbleMesh->CreateModel(0, 0, 0);
	IModel* arrow = arrowMesh->CreateModel(0, 2, 10);
	IModel* dummyCam = dummyMesh->CreateModel(0, 0, 100);
	IModel* dummyMarb = dummyMesh->CreateModel(0, 2, 0);
	IModel* aBarriers[kNumBarriers];
	IModel* aSegments[kNumSegments];
	IModel* aDeadSegments[maxShroomSize];

	//Place worm on the game.
	for (int i = 0; i < kNumSegments; i++) {
		aSegments[i] = segmentMesh->CreateModel(wormXAxis, 10, 200);
		wormXAxis += 10;
	}

	//Place 2 walls on the game.
	for (int i = 0; i < kNumBarriers; i++) {
		if (i < 11) {
			aBarriers[i] = barrierMesh->CreateModel(-83, 0, barrierDistLeft);
			barrierDistLeft += 18;
		}
		else
		{
			aBarriers[i] = barrierMesh->CreateModel(83, 0, barrierDistRight);
			barrierDistRight += 18;
		}
		//Yellow/black stripes on the first 4 barriers of each row
		if (i >= 4 && i <= 10 || i >= 15) {
			aBarriers[i]->SetSkin("barrier1a.bmp");
		}
	}

	//Place mushrooms on the game.
	IModel* aMushrooms[kNumShrooms];
	aMushrooms[0] = mushroomMesh->CreateModel(-55, 0, 190);
	aMushrooms[1] = mushroomMesh->CreateModel(55, 0, 190);
	aMushrooms[2] = mushroomMesh->CreateModel(15, 0, 160);
	aMushrooms[3] = mushroomMesh->CreateModel(-25, 0, 140);
	aMushrooms[4] = mushroomMesh->CreateModel(35, 0, 90);
	aMushrooms[5] = mushroomMesh->CreateModel(-65, 0, 90);
	aMushrooms[6] = mushroomMesh->CreateModel(75, 0, 70);
	aMushrooms[7] = mushroomMesh->CreateModel(-15, 0, 60);
	aMushrooms[8] = mushroomMesh->CreateModel(-45, 0, 40);
	aMushrooms[9] = mushroomMesh->CreateModel(45, 0, 20);


	//Spread Bushes, rotate and scale by 30 - 50
	IModel* aBushes[kNumBushes];
	for (int i = 0; i < kNumBushes; i++) {
		aBushes[i] = bushMesh->CreateModel(bushDist, 0, 210);
		aBushes[i]->Scale(rand() % 20 + 30);
		aBushes[i]->RotateY(bushDist);
		bushDist += 19.5;
	}

	//Model Manipulation 
	arrow->AttachToParent(dummyMarb);

	//Array Manipulation
	//Initialise Arrays
	for (int i = 0; i < kNumShrooms; i++) {
		aMushrooms[i]->RotateY(reverse);	//Rotate 180 degrees because mushroom colours appear better.
		aStateSegments[i] = Healthy;
		aStateShrooms[i] = Healthy;
		aStateDeadSegments[i] = Healthy;
		aStateWormDirection[i] = Moving_Right;
		abSeg2Shroom[i] = false;
	}

	//Camera
	ICamera* camera = myEngine->CreateCamera(kManual, 0, 75, -175);
	camera->RotateX(25);
	camera->AttachToParent(dummyCam);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		stringstream sCurrentScore;
		stringstream sWave;

		//Display Score
		sCurrentScore << "Score: " << score;
		Font_CurrentScore->Draw(sCurrentScore.str(), scoreX, scoreY);

		//Display Wave
		sWave << "Wave: " << wave;
		Font_Wave->Draw(sWave.str(), waveX, waveY);


		/**** Update your scene each frame here ****/

		//Get time since last frame
		float dt = myEngine->Timer();
		collisionTimer += dt;

		//*****	The 4 Game States *****//
		switch (StateGame)
		{
		case Ready: {
			//Update
			arrow->AttachToParent(marble);

			arrowMarbleMovement(myEngine, marble, marble2, marble3, dummyMarb, marbleHandlingSpeed, marbleRotationSpeed, marbleMinX, marbleMaxX, arrowAngle, arrowMaxAngle, arrowAngleInc, dt, StateGame, aBurst);

			//Transition
			if (myEngine->KeyHit(Key_Space)) {
				StateGame = Firing;
				//Variable to fix a bug when rotating the marble on collision with a barrier(when moving the arrow after the marble is shot).
				marbleBounceRate = arrowAngle;
			}
		}
					break;
		case Firing: {
			//Update
			arrow->AttachToParent(dummyMarb);
			marble->MoveLocalZ(marbleFiringSpeed*dt);

			arrowMarbleMovement(myEngine, marble, marble2, marble3, dummyMarb, marbleHandlingSpeed, marbleRotationSpeed, marbleMinX, marbleMaxX, arrowAngle, arrowMaxAngle, arrowAngleInc, dt, StateGame, aBurst);

			//Code which Transitions the game state from firing to contact
			firingTransitions(mushroomMesh, segmentMesh, marble, aDeadSegments, aMushrooms, aSegments, aBarriers, kNumShrooms, score, StateGame, kNumSegments, segmentKills, varDeadSegments,
				indexOfSegmentation, kNumBarriers, aStateShrooms, aStateSegments, aStateDeadSegments, marbleRadius, shroomRadius, segmentRadius, collisionTimer, mushroomSinkY, barrierRadius,
				marbleBounceRate, marbleMaxZ, marbleMinZ, marbleMaxX, marbleMinX, segmentation);
		}
					 break;
		
		case Contact: {
			//Update marble's position and orientation according to the arrow.
			resetMarble(marble, dummyMarb, arrowAngle);
			StateGame = Ready;
		}
					  break;
		case Over: {
			//When game is over, show a message and press R to restart or Escape to quit
			Font_Over->Draw("Game Over! Thank You For Playing!!\nPress R to reset or Escape to quit.", fontCenterX, fontCenterY);
			if (myEngine->KeyHit(Key_R)) {
				resetGame(segmentMesh, mushroomMesh, aMushrooms, aSegments, aDeadSegments, aStateSegments, aStateShrooms, aStateDeadSegments, aStateWormDirection, abSeg2Shroom, kNumSegments, kNumShrooms, varDeadSegments, reverse);
				resetMarble(marble, dummyMarb, arrowAngle);
				StateGame = Ready;
				wave = 0; score = 0; wormSpeed = initialWormSpeed; segmentKills = 0;

			}
		}
				   break;
		case Pause: {
			Font_Welcome->Draw("Press Enter To Play!\n\nWelcome to caterpillar\nInstructions: Simply destroy the worm to win! \nControls:\nSpacebar - Fire Marble \nZ,X Keys - Move Arrow\nFull Stop, Comma keys - Rotate Arrow\nEscape - Exit Game\nEnter - Pause/Show Help Screen", 350, 150);
			if (myEngine->KeyHit(Key_Return)) {
				StateGame = previewsStateGame;
			}
		}
					break;

		case Win: {
			//When game is won, reset scenery and increase worm speed
			Font_Win->Draw("Congratulations!! Press any key to go to the next wave!", fontCenterX, fontCenterY);
			if (myEngine->AnyKeyHit()) {
				resetGame(segmentMesh, mushroomMesh, aMushrooms, aSegments, aDeadSegments, aStateSegments, aStateShrooms, aStateDeadSegments, aStateWormDirection, abSeg2Shroom, kNumSegments, kNumShrooms, varDeadSegments, reverse);
				resetMarble(marble, dummyMarb, arrowAngle);
				wormSpeed += 5;
				wave++;
				segmentKills = 0;
				StateGame = Ready;
			}
		}
				  break;
		}


		//Function for both ready and firing states 
		updateReadyAndFiring(myEngine, mushroomMesh, segmentMesh, marble, marble2, marble3, dummyMarb, aMushrooms, aSegments, aDeadSegments, aBarriers, aStateSegments, aStateWormDirection,
			aStateShrooms, aStateDeadSegments, StateGame, kNumSegments, kNumShrooms, previewsDirection, varDeadSegments, burstIterator, segmentKills, previewsStateGame, kNumBarriers, score, indexOfSegmentation,
			wormStart, wormSpeed, dt, wormMaxX, getNextSquare, gameLosingZ, marble2BounceRate, marble3BounceRate, arrowAngle, marbleFiringSpeed, mushroomSinkY, mushroomSpeed, marbleMinZ,
			marbleMaxZ, marbleMinX, marbleMaxX, marbleRadius, barrierRadius, shroomRadius, segmentRadius, collisionTimer, abSeg2Shroom, aBurst, segmentation);
		
		updateCamera(myEngine, camera, dummyCam, camMaxY, camMinY, dummyCamSpeed, dt);
		gameControls(myEngine);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

//--------------------//
//FUNCTION DEFINITIONS//
//--------------------//

void updateCamera(I3DEngine* myEngine, ICamera* camera, IModel* dummy,
	float camMaxY, float camMinY, float dummySpeed, float dt) {
	if (myEngine->KeyHeld(Key_Up) && (camera->GetY()) <= camMaxY) {
		dummy->RotateX(dummySpeed*dt);
		dummy->MoveY(dummySpeed*dt);
	}

	if (myEngine->KeyHeld(Key_Down) && (camera->GetY()) >= camMinY) {
		dummy->RotateX(-dummySpeed * dt);
		dummy->MoveY(-dummySpeed * dt);
	}

	if (myEngine->KeyHit(Key_C)) {
		dummy->SetY(0);
		dummy->ResetOrientation();
	}
}

bool collisionDetection(IModel* Model1, IModel* Model2, float mRadius1, float mRadius2, float& collisionTimer)
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
		std::cout << "collision" << endl;
		collision = true;
		collisionTimer = 0;
	}
	return collision;
}



void mushroomFSM(IModel* aMushrooms, IMesh* mushroomMesh, int& aStateShrooms, int& currentScore) {
	switch (aStateShrooms)
	{
	case Healthy: {}
				  break;
	case Damaged: {
		aMushrooms->SetSkin("mushroom_hot.png");
	}
				  break;
	case Critical: {
		aMushrooms->SetSkin("mushroom_very_hot.png");
	}
				   break;
	case Dead: {
		currentScore += 10;
	}
			   break;
	}
}

void deadSegmentFSM(IModel* aDeadSegments, IMesh* mushroomMesh, int& aStateDeadSegments, int& currentScore) {
	switch (aStateDeadSegments)
	{
	case Healthy: {}
				  break;
	case Damaged: {
		aDeadSegments->SetSkin("mushroom_hot.png");
	}
				  break;
	case Critical: {
		aDeadSegments->SetSkin("mushroom_very_hot.png");
	}
				   break;
	case Dead: {
		currentScore += 10;
	}
			   break;
	}
}

void segmentFSM(IModel* aSegments, IModel* aDeadSegments[], IMesh* segmentMesh, IMesh* mushroomMesh,
	int aStateSegments, int& segmentKills, int& varDeadSegments, int& aStateShrooms, int& aStateDeadSegments, float mushroomSinkY, bool& segmentation, int& currentScore) {
	const int maxShroomSize = 10;
	switch (aStateSegments)
	{
	case Healthy: {}
				  break;
	case Damaged: {
		aSegments->SetSkin("wormskin_hot.jpg");
	}
				  break;
	case Critical: {
		aSegments->SetSkin("wormskin_very_hot.jpg");
	}
				   break;
	case Dead: {
		//Remove segment and place a Mushroom at its position
		float x = aSegments->GetX();
		float z = aSegments->GetZ();
		//Move segment underground instead of removing it, to eliminate model Null error on collisionDetection.
		aSegments->MoveY(-10);
		if (varDeadSegments < maxShroomSize) {
			aDeadSegments[varDeadSegments] = mushroomMesh->CreateModel(x, mushroomSinkY, z);
			varDeadSegments++;
			aStateDeadSegments = Healthy;
		}
		segmentKills++;
		currentScore += 30;
		segmentation = true;
	}
			   break;
	}
}

void arrowMarbleMovement(I3DEngine* myEngine, IModel* marble, IModel* marble2, IModel* marble3, IModel* dummyMarb,
	float marbleHandlingSpeed, float marbleRotationSpeed, float marbleMinX, float marbleMaxX,
	float& arrowAngle, float arrowMaxAngle, float arrowAngleInc, float dt, int StateGame, bool aBurst[]) {
	//"Update" code from Ready State
	if (StateGame == Ready) {
		//Move Marble Left and Right
		if (myEngine->KeyHeld(Key_Z) && marble->GetX() > marbleMinX) {
			marble->MoveX(-marbleHandlingSpeed * dt);
			if (!aBurst[0])
				marble2->MoveX(-marbleHandlingSpeed * dt);
			if (!aBurst[1])
				marble3->MoveX(-marbleHandlingSpeed * dt);
			dummyMarb->MoveX(-marbleHandlingSpeed * dt);
		}

		if (myEngine->KeyHeld(Key_X) && marble->GetX() < marbleMaxX) {
			marble->MoveX(marbleHandlingSpeed * dt);
			if (!aBurst[0])
				marble2->MoveX(marbleHandlingSpeed * dt);
			if (!aBurst[1])
				marble3->MoveX(marbleHandlingSpeed * dt);
			dummyMarb->MoveX(marbleHandlingSpeed * dt);
		}

		//Rotate the arrow Leftwards and Rightwards
		if (myEngine->KeyHeld(Key_Comma) && arrowAngle > -arrowMaxAngle) {
			marble->RotateY(-marbleRotationSpeed * dt);
			if (!aBurst[0])
				marble2->RotateY(-marbleRotationSpeed * dt);
			if (!aBurst[1])
				marble3->RotateY(-marbleRotationSpeed * dt);
			dummyMarb->RotateY(-marbleRotationSpeed * dt);
			arrowAngle -= arrowAngleInc * dt;
		}
		if (myEngine->KeyHeld(Key_Period) && arrowAngle < arrowMaxAngle) {
			marble->RotateY(marbleRotationSpeed * dt);
			if (!aBurst[0])
				marble2->RotateY(marbleRotationSpeed * dt);
			if (!aBurst[1])
				marble3->RotateY(marbleRotationSpeed * dt);
			dummyMarb->RotateY(marbleRotationSpeed * dt);
			arrowAngle += arrowAngleInc * dt;
		}
	}
	//"Update" code from Firing State
	else if (StateGame == Firing) {
		//Move Arrow When marble is fired
		if (myEngine->KeyHeld(Key_Z) && dummyMarb->GetX() > marbleMinX) {
			if (!aBurst[0])
				marble2->MoveX(-marbleHandlingSpeed * dt);
			if (!aBurst[1])
				marble3->MoveX(-marbleHandlingSpeed * dt);
			dummyMarb->MoveX(-marbleHandlingSpeed * dt);
		}

		if (myEngine->KeyHeld(Key_X) && dummyMarb->GetX() < marbleMaxX) {
			if (!aBurst[0])
				marble2->MoveX(marbleHandlingSpeed * dt);
			if (!aBurst[1])
				marble3->MoveX(marbleHandlingSpeed * dt);
			dummyMarb->MoveX(marbleHandlingSpeed * dt);
		}

		if (myEngine->KeyHeld(Key_Comma) && arrowAngle > -arrowMaxAngle) {
			if (!aBurst[0])
				marble2->RotateY(-marbleRotationSpeed * dt);
			if (!aBurst[1])
				marble3->RotateY(-marbleRotationSpeed * dt);
			dummyMarb->RotateY(-marbleRotationSpeed * dt);
			arrowAngle -= arrowAngleInc * dt;
		}
		if (myEngine->KeyHeld(Key_Period) && arrowAngle < arrowMaxAngle) {
			if (!aBurst[0])
				marble2->RotateY(marbleRotationSpeed * dt);
			if (!aBurst[1])
				marble3->RotateY(marbleRotationSpeed * dt);
			dummyMarb->RotateY(marbleRotationSpeed * dt);
			arrowAngle += arrowAngleInc * dt;
		}
	}
}

void segmentMovementFSM(IModel* aSegments, int& aStateWormDirection, float wormSpeed, float dt, float wormMaxX, float& getNextSquare, bool& abSeg2Shroom, int &nextDirection) {
	float nextSquare = 10;


	switch (aStateWormDirection)
	{
	case Moving_Right: {
		//Update
		aSegments->MoveLocalX(wormSpeed * dt);

		//Transitions
		if (aSegments->GetX() > wormMaxX) {
			aStateWormDirection = Moving_Down;
			getNextSquare = aSegments->GetZ() - nextSquare;
		}

		else if (abSeg2Shroom) {
			nextDirection = Moving_Left;
			aStateWormDirection = Moving_Down;
			getNextSquare = aSegments->GetZ() - nextSquare;
		}

	}
					   break;
	case Moving_Left: {
		//update
		aSegments->MoveLocalX(-wormSpeed * dt);
		//Transitions
		if (aSegments->GetX() < -wormMaxX) {
			aStateWormDirection = Moving_Down;
			getNextSquare = aSegments->GetZ() - nextSquare;
		}

		else if (abSeg2Shroom) {
			nextDirection = Moving_Right;
			aStateWormDirection = Moving_Down;
			getNextSquare = aSegments->GetZ() - nextSquare;
		}

	}
					  break;
	case Moving_Down: {
		//update
		aSegments->MoveLocalZ(-wormSpeed * dt);

		//transition
		if (aSegments->GetZ() < getNextSquare && aSegments->GetX() > wormMaxX)
			aStateWormDirection = Moving_Left;
		else if (aSegments->GetZ() < getNextSquare && aSegments->GetX() < -wormMaxX)
			aStateWormDirection = Moving_Right;

		//For collision with mushroom

		else if (aSegments->GetZ() <= getNextSquare && abSeg2Shroom) {
			aStateWormDirection = nextDirection;
			abSeg2Shroom = false;
		}
	}
					  break;
	}//switch
}

void resetMarble(IModel* marble, IModel* dummyMarb, float arrowAngle) {
	marble->SetPosition(dummyMarb->GetX(), dummyMarb->GetY(), dummyMarb->GetZ());
	marble->ResetOrientation();
	marble->RotateY(arrowAngle);
}

void resetGame(IMesh* segmentMesh, IMesh* mushroomMesh, IModel* aMushrooms[], IModel* aSegments[], IModel* aDeadSegments[],
	int aStateSegments[], int aStateShrooms[], int aStateDeadSegments[], int aStateWormDirection[], bool abSeg2Shroom[], int kNumSegments, int kNumShrooms, int& varDeadSegments,
	float reverse) {
	float mushroomSinkY = 8;
	float wormXAxis = -75;
	int ground = 0;

	//Place mushrooms on the game.
	for (int i = 0; i < kNumShrooms; i++) {
		if (aMushrooms[i]->GetY() < ground) {
			aMushrooms[i]->MoveY(mushroomSinkY);
			aMushrooms[i]->SetSkin("mushroom.png");
		}
	}

	//Delete any remaining segments
	for (int i = 0; i < kNumSegments; i++) {
		if (aSegments[i] != NULL) {
			segmentMesh->RemoveModel(aSegments[i]);
		}
	}

	//Place worm on the game.
	for (int i = 0; i < kNumSegments; i++) {
		aSegments[i] = segmentMesh->CreateModel(wormXAxis, 10, 200);
		wormXAxis += 10;
	}

	//Initialise Mushroom and Segment State to Healthy.
	for (int i = 0; i < kNumShrooms; i++) {
		aMushrooms[i]->RotateY(reverse);	//Because mushroom colours appear better.
		aStateSegments[i] = Healthy;
		aStateShrooms[i] = Healthy;
		aStateDeadSegments[i] = Healthy;
		aStateWormDirection[i] = Moving_Right;
		abSeg2Shroom[i] = false;
	}


	//Delete any new mushrooms from dead segments
	if (varDeadSegments > 0) {
		for (int i = 0; i < varDeadSegments; i++)
		{
			mushroomMesh->RemoveModel(aDeadSegments[i]);
		}
		varDeadSegments = 0;
	}
}

void updateReadyAndFiring(I3DEngine* myEngine, IMesh* mushroomMesh, IMesh* segmentMesh, IModel* marble, IModel* marble2, IModel* marble3, IModel* dummyMarb,
	IModel* aMushrooms[], IModel* aSegments[], IModel* aDeadSegments[], IModel* aBarriers[],
	int aStateSegments[], int aStateWormDirection[], int aStateShrooms[], int aStateDeadSegments[], 
	int& StateGame, int kNumSegments, int kNumShrooms, int previewsDirection, int& varDeadSegments,
	int& burstIterator, int& segmentKills, int& previewsStateGame, int kNumBarriers, int& score, int indexOfSegmentation, int wormStart,
	float wormSpeed, float dt, float wormMaxX, float& getNextSquare, float gameLosingZ, float& marble2BounceRate,
	float& marble3BounceRate, float& arrowAngle, float marbleFiringSpeed, float mushroomSinkY, float mushroomSpeed,
	float marbleMinZ, float marbleMaxZ, float marbleMinX, float marbleMaxX, float marbleRadius, float barrierRadius,
	float shroomRadius, float segmentRadius, float collisionTimer, 
	bool abSeg2Shroom[], bool aBurst[], bool segmentation) {
	if (StateGame == Ready || StateGame == Firing) {
		//Worm Movement
		for (int i = 0; i < kNumSegments; i++) {
			if (aStateSegments[i] != Dead) {
				segmentMovementFSM(aSegments[i], aStateWormDirection[i], wormSpeed, dt, wormMaxX, getNextSquare, abSeg2Shroom[i], previewsDirection);

				//Check if worm has reached the final square
				if (aSegments[i]->GetZ() < gameLosingZ) {
					StateGame = Over;
					//Turn all mushrooms to purple and purple hot accordingly
					for (int i = 0; i < kNumShrooms; i++) {
						if (aStateShrooms[i] == Healthy)
							aMushrooms[i]->SetSkin("purple_mushroom.png");
						else if (aStateShrooms[i] != Dead)
							aMushrooms[i]->SetSkin("purple_mushroom_hot.png");
					}

					for (int i = 0; i < varDeadSegments; i++) {
						if (aStateDeadSegments[i] == Healthy)
							aDeadSegments[i]->SetSkin("purple_mushroom.png");
						else if (aStateDeadSegments[i] != Dead)
							aDeadSegments[i]->SetSkin("purple_mushroom_hot.png");
					}
				}
			}
		}

		//Commented out because of a bug: Half the worm moves normally then the other half's z axis gradually goes beyond the given limit.

		/*

		//Collision between segments and mushrooms
		for (int i = 0; i < kNumSegments; i++) {
		if (aStateSegments[i] != Dead) {
		for (int j = 0; j < kNumShrooms; j++) {
		if (aStateShrooms[j] != Dead) {
		//if (collisionDetection(aSegments[i], aMushrooms[j], segment2MushroomRadius, shroomRadius, collisionTimer)) {

		//abSeg2Shroom[i] = true;
		}
		}
		}
		}
		}

		*/

		//Update Marble 2 and Marble 3
		if (myEngine->KeyHit(Key_Space) && StateGame == Firing)
		{
			aBurst[burstIterator] = true;
			if (burstIterator < 1) {
				marble2BounceRate = arrowAngle;
				burstIterator++;
			}
			else
				marble3BounceRate = arrowAngle;
		}

		//Move marble 2
		if (aBurst[0]) {
			marble2->MoveLocalZ(marbleFiringSpeed * dt);
		}
		//Move marble 3
		if (aBurst[1]) {
			marble3->MoveLocalZ(marbleFiringSpeed * dt);
		}

		//Slowly sink dead mushrooms
		for (int i = 0; i < kNumShrooms; i++) {
			if (aStateShrooms[i] == Dead) {
				if (aMushrooms[i]->GetY() > mushroomSinkY)
					aMushrooms[i]->MoveY(-mushroomSpeed * dt);
			}
		}

		//Slowly grow mushrooms from dead Segments
		for (int i = 0; i < varDeadSegments; i++) {
			if (aStateDeadSegments[i] != Dead) {
				if (aDeadSegments[i]->GetY() < 0)
					aDeadSegments[i]->MoveY(mushroomSpeed * dt);
			}
			//Slowly sink new dead mushrooms
			else if (aStateDeadSegments[i] == Dead) {
				if (aDeadSegments[i]->GetY() > mushroomSinkY)
					aDeadSegments[i]->MoveY(-mushroomSpeed * dt);
			}
		}

		//Transitions

		//Check if game is won
		if (segmentKills == kNumSegments) {
			marble->SetSkin("glass_green.jpg");
			marble2->SetSkin("glass_green.jpg");
			marble3->SetSkin("glass_green.jpg");
			StateGame = Win;
		}

		if (myEngine->KeyHit(Key_Return)) {
			previewsStateGame = StateGame;
			StateGame = Pause;
		}


		//Check if marbles 2,3 go out of bounds
		if (marble2->GetZ() < marbleMinZ || marble2->GetZ() > marbleMaxZ || marble2->GetX() < marbleMinX || marble2->GetX() > marbleMaxX) {
			aBurst[0] = false;
			if (burstIterator == 1)
				burstIterator--;
		}

		if (marble3->GetZ() < marbleMinZ || marble3->GetZ() > marbleMaxZ || marble3->GetX() < marbleMinX || marble3->GetX() > marbleMaxX) {
			aBurst[1] = false;
			if (burstIterator == 1)
				burstIterator--;
		}

		//Check for collision between marble 2,3 and wall.
		for (int i = 0; i < kNumBarriers; i++) {
			if (collisionDetection(marble2, aBarriers[i], marbleRadius, barrierRadius, collisionTimer)) {
				marble2->RotateY(-2 * marble2BounceRate);
				//To fix sphere wall to wall bug
				marble2BounceRate = 1 * -marble2BounceRate;
			}
			if (collisionDetection(marble3, aBarriers[i], marbleRadius, barrierRadius, collisionTimer)) {
				marble3->RotateY(-2 * marble3BounceRate);
				//To fix sphere wall to wall bug
				marble3BounceRate = 1 * -marble3BounceRate;
			}
		}

		//Check for collision between marble 2,3 and mushroom
		for (int i = 0; i < kNumShrooms; i++) {
			//If the mushroom is dead then skip collision detection.
			if (aStateShrooms[i] != Dead) {
				if (collisionDetection(marble2, aMushrooms[i], marbleRadius, shroomRadius, collisionTimer)) {
					aStateShrooms[i] += 1;
					mushroomFSM(aMushrooms[i], mushroomMesh, aStateShrooms[i], score);
					aBurst[0] = false;
					if (burstIterator == 1)
						burstIterator--;
				}
				if (collisionDetection(marble3, aMushrooms[i], marbleRadius, shroomRadius, collisionTimer)) {
					aStateShrooms[i] += 1;
					mushroomFSM(aMushrooms[i], mushroomMesh, aStateShrooms[i], score);
					aBurst[1] = false;
					if (burstIterator == 1)
						burstIterator--;
				}
			}
		}

		for (int i = 0; i < kNumSegments; i++) {
			//If the segment is dead then skip collision detection.
			if (aStateSegments[i] != Dead) {
				//Check for collision between marble 2,3 and worm
				if (collisionDetection(marble2, aSegments[i], marbleRadius, segmentRadius, collisionTimer)) {
					aStateSegments[i] += 1;
					segmentFSM(aSegments[i], aDeadSegments, segmentMesh, mushroomMesh, aStateSegments[i], segmentKills, varDeadSegments, aStateShrooms[i], aStateDeadSegments[i], mushroomSinkY, segmentation, score);
					aBurst[0] = false;
					if (burstIterator == 1)
						burstIterator--;
				}
				if (collisionDetection(marble3, aSegments[i], marbleRadius, segmentRadius, collisionTimer)) {
					aStateSegments[i] += 1;
					segmentFSM(aSegments[i], aDeadSegments, segmentMesh, mushroomMesh, aStateSegments[i], segmentKills, varDeadSegments, aStateShrooms[i], aStateDeadSegments[i], mushroomSinkY, segmentation, score);
					aBurst[1] = false;
					if (burstIterator == 1)
						burstIterator--;

				}
			}
			//Check for segmentation
			if (segmentation) {
				indexOfSegmentation = i - 1;
				segmentation = false;
			}
		}

		//Check for collision between marble 2,3 and dead Segments
		for (int i = 0; i < varDeadSegments; i++) {
			if (aStateDeadSegments[i] != Dead) {
				if (collisionDetection(marble2, aDeadSegments[i], marbleRadius, shroomRadius, collisionTimer)) {
					aStateDeadSegments[i] += 1;
					deadSegmentFSM(aDeadSegments[i], mushroomMesh, aStateDeadSegments[i], score);
					aBurst[0] = false;
					if (burstIterator == 1)
						burstIterator--;
				}
				if (collisionDetection(marble3, aDeadSegments[i], marbleRadius, shroomRadius, collisionTimer)) {
					aStateDeadSegments[i] += 1;
					deadSegmentFSM(aDeadSegments[i], mushroomMesh, aStateDeadSegments[i], score);
					aBurst[1] = false;
					if (burstIterator == 1)
						burstIterator--;
				}
			}
		}

		//Worm Segmentation
		if (indexOfSegmentation != 0 && indexOfSegmentation != 8) {
			for (int i = wormStart; i <= indexOfSegmentation; i++) {
				if (indexOfSegmentation < wormStart)
					wormStart = indexOfSegmentation + 2;
				else
					wormStart = 0;
				if ((indexOfSegmentation - wormStart > 1)) {
					if (aStateWormDirection[i] == Moving_Right)
						aStateWormDirection[i] = Moving_Left;
					else if (aStateWormDirection[i] == Moving_Left)
						aStateWormDirection[i] = Moving_Right;
					segmentMovementFSM(aSegments[i], aStateWormDirection[i], wormSpeed, dt, wormMaxX, getNextSquare, abSeg2Shroom[i], previewsDirection);
				}
			}
			indexOfSegmentation = 0;
		}

		//Similar to contact state
		if (!aBurst[0])
			resetMarble(marble2, dummyMarb, arrowAngle);
		if (!aBurst[1])
			resetMarble(marble3, dummyMarb, arrowAngle);
	}
}


void firingTransitions(IMesh* mushroomMesh, IMesh* segmentMesh, IModel* marble, IModel* aDeadSegments[], IModel* aMushrooms[], IModel* aSegments[], IModel* aBarriers[],
	int kNumShrooms, int& score, int& StateGame, int kNumSegments, int& segmentKills, int& varDeadSegments, int indexOfSegmentation, int kNumBarriers,
	int aStateShrooms[], int aStateSegments[], int aStateDeadSegments[],
	float marbleRadius, float shroomRadius, float segmentRadius, float collisionTimer, float mushroomSinkY, float barrierRadius, float& marbleBounceRate,
	float marbleMaxZ, float marbleMinZ, float marbleMaxX, float marbleMinX, bool segmentation) {
	//Check for collision between marble and mushroom
	for (int i = 0; i < kNumShrooms; i++) {
		//If the mushroom is dead then skip collision detection.
		if (aStateShrooms[i] != Dead) {
			if (collisionDetection(marble, aMushrooms[i], marbleRadius, shroomRadius, collisionTimer)) {
				aStateShrooms[i] += 1;
				mushroomFSM(aMushrooms[i], mushroomMesh, aStateShrooms[i], score);
				StateGame = Contact;
			}
		}
	}

	//Check for collision between marble and worm
	for (int i = 0; i < kNumSegments; i++) {
		//If the segment is dead then skip collision detection.
		if (aStateSegments[i] != Dead) {
			if (collisionDetection(marble, aSegments[i], marbleRadius, segmentRadius, collisionTimer)) {
				aStateSegments[i] += 1;
				segmentFSM(aSegments[i], aDeadSegments, segmentMesh, mushroomMesh, aStateSegments[i], segmentKills, varDeadSegments, aStateShrooms[i], aStateDeadSegments[i], mushroomSinkY, segmentation, score);
				StateGame = Contact;
			}
			//Check for segmentation
			if (segmentation) {
				indexOfSegmentation = i - 1;
				segmentation = false;
			}
		}
	}

	//Check for collision between marble and dead Segments
	for (int i = 0; i < varDeadSegments; i++) {
		if (aStateDeadSegments[i] != Dead) {
			if (collisionDetection(marble, aDeadSegments[i], marbleRadius, shroomRadius, collisionTimer)) {
				aStateDeadSegments[i] += 1;
				deadSegmentFSM(aDeadSegments[i], mushroomMesh, aStateDeadSegments[i], score);
				StateGame = Contact;
			}
		}
	}

	//Check for collision between marble and wall
	for (int i = 0; i < kNumBarriers; i++) {
		if (collisionDetection(marble, aBarriers[i], marbleRadius, barrierRadius, collisionTimer)) {
			marble->RotateY(-2 * marbleBounceRate);
			//To fix sphere wall to wall bug
			marbleBounceRate = 1 * -marbleBounceRate;
		}

	}

	//Check if Marble goes out of bounds
	if (marble->GetZ() < 0 || marble->GetZ() > marbleMaxZ || marble->GetX() < marbleMinX || marble->GetX() > marbleMaxX)
		StateGame = Contact;
}

void gameControls(I3DEngine* myEngine) {
	if (myEngine->KeyHit(Key_Escape))
		myEngine->Stop();
}