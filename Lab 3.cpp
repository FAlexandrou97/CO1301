// Lab 3.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{
	//PART I: Scene (executed only once)

	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	// Camera, meshes, models for:
	// (a) Floor
	// (b) Grid
	// (c) Cube
	ICamera* camera = myEngine->CreateCamera();
	IMesh* FloorMesh = myEngine->LoadMesh("Floor.x");
	IModel* FloorModel = FloorMesh->CreateModel(0, 0, 0);
	/*
	IMesh* GridMesh = myEngine->LoadMesh("Grid.x");
	IModel* GridModel = GridMesh->CreateModel(0, 1, 0);
	IMesh* CubeMesh = myEngine->LoadMesh("Cube.x");
	IModel* CubeModel = CubeMesh->CreateModel(0, 5, 0);
	*/
	IMesh* ArrowMesh = myEngine->LoadMesh("Arrow.x");
	IModel* ArrowModel = ArrowMesh->CreateModel(0,5,0);

	//PART II: Code which is executed over and over again.

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		// Camera motion:

		// Cube motion:

		// User Controls:
		/*Close the game when hit Escape.*/
		if (myEngine->KeyHit(Key_Escape)) {
			myEngine->Stop();
		}


		myEngine->StartMouseCapture();	//Switch off the mouse cursor and send all mouse information to the engine
		if (myEngine->KeyHeld(Key_Control)) {
			myEngine->StopMouseCapture();
		}

		int mouseMoveX = myEngine->GetMouseMovementX();
		int mouseMoveY = myEngine->GetMouseMovementY();
		ArrowModel->RotateY(mouseMoveX * -0.1);
		ArrowModel->RotateX(mouseMoveY * -0.1);

		if (myEngine->KeyHeld(Key_W)) {
			ArrowModel->MoveZ(0.1);
		}

		if (myEngine->KeyHeld(Key_S)) {
			ArrowModel->MoveZ(-0.1);
		}

		if (myEngine->KeyHeld(Key_D)) {
			ArrowModel->MoveX(0.1);
		}

		if (myEngine->KeyHeld(Key_A)) {
			ArrowModel->MoveX(-0.1);
		}

		if (myEngine->KeyHeld(Key_Q)) {
			ArrowModel->MoveY(0.1);
		}

		if (myEngine->KeyHeld(Key_E)) {
			ArrowModel->MoveY(-0.1);
		}
	
		
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
