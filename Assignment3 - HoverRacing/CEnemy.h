#pragma once
#include "CHover.h"

#define Waypoint_1 1
#define Waypoint_2 2
#define Waypoint_3 3
#define Waypoint_4 4
#define enemyTurn 6
#define enemyBrake 7
#define vertical 0
#define horizontal 1

class CEnemy :
	public CHover
{
private:
	int m_stateWaypoint = Waypoint_1;
	int m_stateWaypointNext = Waypoint_1;
	float m_turnAngle = 0;
	float m_speedMax = 90;
	float m_acceleration = 0.8;
	float m_moveX = 0;
	bool m_collision = false;
public:
	//Requires a boolean variable to use in CollisionResolution
	void SetCollision(bool collision);
	//Enemy Movement... AI would've been better
	void Movement(IModel* aWaypoints[], float collisionTimer, float m_dt);
	//Collision Resolution with player
	void CollisionResolution(IModel* p1, float m_dt);
	//Sets the current waypoint of the enemy, useful in game Restarts
	void SetWaypoint(int waypoint);
	void SetSpeed(int speed);
	void ResetTurningAngle();
	CEnemy();
	CEnemy(IMesh* hoverMesh, I3DEngine* myEngine, string skin);
	~CEnemy();
};