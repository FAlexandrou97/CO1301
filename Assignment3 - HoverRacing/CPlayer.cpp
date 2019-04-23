//Floris Alexandrou
#include "CPlayer.h"


void CPlayer::Movement(float m_dt){
	m_hover->MoveLocalZ(m_speed*m_dt);

	if (m_myEngine->KeyHeld(Key_W) && m_speed < m_speedMax) {
		m_speed += m_acceleration;
	}

	if (m_myEngine->KeyHeld(Key_S) && m_speed > -m_speedMax / 2) {
		m_speed -= m_accelReverse;
	}

	if (m_myEngine->KeyHeld(Key_D)) {
		m_hover->RotateLocalY(m_rotationRate *m_dt);

	}

	if (m_myEngine->KeyHeld(Key_A)) {
		m_hover->RotateLocalY(-m_rotationRate * m_dt);

	}

	//Lose speed due to friction
	if (m_speed > 0)
		m_speed -= 0.2;
	else
		m_speed += 0.4;
}

void CPlayer::Boost(float m_dt) {
	if (m_myEngine->KeyHeld(Key_Space) && m_boostMeter > m_boostMeterMin) {
		m_acceleration = m_accelBoost;
		m_speedMax = m_speedBoostMax;
		m_boostMeter -= 0.2;
	}
	else {
		m_acceleration = m_accelInit;
		m_speedMax = m_speedMaxInit;
		if (m_boostMeter < m_boostMeterMax)
			m_boostMeter += 0.1;
	}
}

void CPlayer::CollisionResolution(float m_dt) {
	if (m_speed > 70)
		m_hp -= 5;
	else if (m_speed > 40)
		m_hp -= 3;
	else if (m_speed < -40)
		m_hp -= 3;
	else
		m_hp -= 0.1;
	float tempSpeed = m_speed;
	//Resolution while hover moves forward
	if (m_speed > 0) {
		while (m_speed > -tempSpeed) {
			m_speed -= 4;
			m_hover->MoveLocalZ(m_speed*m_dt);
		}
	}
	else
		//Resolution while hover in reverse
	{
		while (m_speed < -tempSpeed) {
			m_speed += 4;
			m_hover->MoveLocalZ(m_speed*m_dt);
		}
	}
}

float CPlayer::GetSpeed() {
	return m_speed;
}

void CPlayer::SetSpeed(float speed) {
	m_speed = speed;
}

float CPlayer::GetBoostMeter() {
	return m_boostMeter;
}

float CPlayer::GetHealth() {
	return m_hp;
}

void CPlayer::SetHealth(float hp) {
	m_hp = hp;
}

CPlayer::CPlayer()
{
}

CPlayer::CPlayer(IMesh* hoverMesh, I3DEngine* myEngine) {
	m_hover = hoverMesh->CreateModel(0, 0, 30);
	m_myEngine = myEngine;
}

CPlayer::~CPlayer()
{
}