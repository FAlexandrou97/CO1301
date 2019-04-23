#pragma once
#include "CHover.h"
class CPlayer :
	public CHover
{
private:
	float m_acceleration = 0.9;
	float m_accelReverse = 0.7;
	float m_rotationRate = 100;
	float const m_speedMaxInit = 100;
	float const m_accelInit = 0.9;
	float m_boostMeter = 100;
	float m_boostMeterMin = 0;
	float m_boostMeterMax = 100;
	float m_accelBoost = m_acceleration * 2;
	float m_speedBoostMax = m_speedMaxInit * 1.4;
	float m_speedMax = 100;
	float m_hp = 100;
public:
	//Movement of the player, WASD hotkeys
	void Movement(float m_dt);
	//Hover "Nitro", with great speed comes with great responsibility!!
	void Boost(float m_dt);
	//Collision Resolution with all stationary objects
	void CollisionResolution(float m_dt);
	float GetSpeed();
	void SetSpeed(float speed);
	float GetBoostMeter();
	float GetHealth();
	void SetHealth(float hp);

	//Constructors
	CPlayer();
	CPlayer(IMesh* hoverMesh, I3DEngine* myEngine);
	~CPlayer();
};