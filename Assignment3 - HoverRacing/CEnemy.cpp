//Floris Alexandrou
#include "CEnemy.h"
#include <ctime>

bool enemy2WaypointCollisionDetection(IModel* Sphere, IModel* Box, float sphereRadius, float& collisionTimer, int alignment)
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

	if (alignment == horizontal) {
		//Code from lecture slides
		if (p1x < p2x + 20 && p1x > p2x - 20 &&
			p1y < p2y + sphereRadius && p1y > p2y - sphereRadius &&
			p1z < p2z + sphereRadius && p1z > p2z - sphereRadius)
		{
			collision = true;
		}
	}
	else if (alignment == vertical) {
		//Code from lecture slides
		if (p1x < p2x + sphereRadius && p1x > p2x - sphereRadius &&
			p1y < p2y + sphereRadius && p1y > p2y - sphereRadius &&
			p1z < p2z + 20 && p1z > p2z - 20)
		{
			collision = true;
		}
	}
	return collision;
}

void CEnemy::Movement(IModel* aWaypoints[], float collisionTimer, float m_dt){

	if (m_speed < m_speedMax)
		m_speed += m_acceleration;

	m_hover->MoveLocalZ(m_speed*m_dt);
		//EnemyWaypoint FSM
		switch (m_stateWaypoint)
		{
			srand(static_cast<unsigned int>(clock()));
		case Waypoint_1:{	
			if (enemy2WaypointCollisionDetection(m_hover, aWaypoints[0], m_radius, collisionTimer, horizontal)) {
				m_stateWaypointNext = Waypoint_2;
				m_stateWaypoint = enemyTurn;
			}
		}
			break;
		case Waypoint_2: {
			if (enemy2WaypointCollisionDetection(m_hover, aWaypoints[1], m_radius, collisionTimer, vertical)) {
				m_stateWaypointNext = Waypoint_3;
				m_stateWaypoint = enemyTurn;
			}
		}
			break;
		case Waypoint_3: {
			if (enemy2WaypointCollisionDetection(m_hover, aWaypoints[2], m_radius, collisionTimer, horizontal)) {
				m_stateWaypointNext = Waypoint_4;
				m_stateWaypoint = enemyTurn;
			}
		}
			break;
		case Waypoint_4: {
			if (enemy2WaypointCollisionDetection(m_hover, aWaypoints[3], m_radius, collisionTimer, vertical)) {
				m_stateWaypointNext = Waypoint_1;
				m_stateWaypoint = enemyTurn;
			}
		}
			break;
		case enemyTurn:{
			m_turnAngle += 350 * m_dt;
			m_hover->RotateLocalY(m_turnAngle * m_dt);	
			m_speed -=  1.7 * (float(rand()) / (float(RAND_MAX) + 1.0));
			if (m_turnAngle >= 250) {
				m_stateWaypoint = m_stateWaypointNext;
				m_turnAngle = 0;
			}
		}
			break;
		}
}


void CEnemy::CollisionResolution(IModel* p1, float m_dt) {
	float moveXMax = 50;
	if (m_collision) {
		if (p1->GetLocalX() < m_hover->GetLocalX()) {
			if (m_moveX < moveXMax) {
				m_moveX += 1;
				p1->MoveLocalX(-m_moveX / 2 * m_dt);
				m_hover->MoveLocalX(m_moveX * m_dt);
			}
			else {
				m_moveX = 0;
				m_collision = false;
			}
		}
		else {
			if (m_moveX > -moveXMax) {
				m_moveX -= 1;
				p1->MoveLocalX(-m_moveX / 2 * m_dt);
				m_hover->MoveLocalX(m_moveX * m_dt);
			}
			else {
				m_moveX = 0;
				m_collision = false;
			}
		}
	}
}

void CEnemy::SetCollision(bool collision) {
	m_collision = collision;
}

void CEnemy::SetWaypoint(int waypoint) {
	m_stateWaypoint = waypoint;
}

void CEnemy::SetSpeed(int speed) {
	m_speed = speed;
}

void CEnemy::ResetTurningAngle() {
	m_turnAngle = 0;
}

CEnemy::CEnemy()
{
}

CEnemy::CEnemy(IMesh* hoverMesh, I3DEngine* myEngine, string skin) {
	m_hover = hoverMesh->CreateModel();
	m_hover->SetSkin(skin);
}

CEnemy::~CEnemy()
{
}