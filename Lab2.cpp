// Lab 2.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{

	// PART I: Executed once.

	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	ICamera* camera = myEngine->CreateCamera(kFPS);

	IMesh* CubeMesh = myEngine->LoadMesh("Cube.x");
	IModel* CubeModel = CubeMesh->CreateModel(6,10,0);
	

	IMesh* GridMesh = myEngine->LoadMesh("Grid.x");
	IModel* GridModel = GridMesh->CreateModel();

	IMesh* FloorMesh = myEngine->LoadMesh("Floor.x");
	IModel* FloorModel = FloorMesh->CreateModel(0,-1,0);





	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();


		/**** Update your scene each frame here ****/
		CubeModel->RotateY(-3.0);
		
		/*Move the cube Left.	*/
		if (myEngine->KeyHeld(Key_A)) {

			CubeModel->MoveX(-0.2);
			/*Move cube in the middle when it gets out of range.  */
			float xpos = CubeModel->GetX();
			if (xpos < -40) {
				CubeModel->SetX(0);
			}
		}

		/*Move the cube Right.	*/
		if (myEngine->KeyHeld(Key_D)) {
			CubeModel->MoveX(0.2);

			/*Move cube in the middle when it gets out of range.  */
			float xpos = CubeModel->GetX();
			if (xpos > 40) {
				CubeModel->SetX(0);
			}
		}

		//Closed when Pressed Esc Button.
		if (myEngine->KeyHit(Key_Escape))
			{
			myEngine->Stop();
			}
	}

	// PART III: Code responsible for clearing up.
	

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
