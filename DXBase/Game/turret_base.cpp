#include "Turret_base.h"
#include <dinput.h>
#include "gamedata.h"
//I suppose we should really call this the main player character

Turret_Base::Turret_Base(string _fileName, ID3D11Device* _pd3dDevice, MyEffectFactory* _EF) :CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_pos.y = 10.0f;
	m_scale = 2.0f * Vector3::One;

	m_fudge = Matrix::CreateRotationY(XM_PI);
}

Turret_Base::~Turret_Base()
{

}

void Turret_Base::Tick(GameData* _GD)
{
	switch (_GD->GS)
	{
	case GS_PLAY_MAIN_CAM:
	{

		if (abs(_GD->mouse->lX) > 0.01f || abs(_GD->mouse->lY) > 0.01f)
		{
			m_yaw = atan2f(_GD->mouse->lX, _GD->mouse->lY) - XM_PIDIV2;
		}
		m_pos.x += _GD->mouse->lX;
		m_pos.z += _GD->mouse->lY;
		break;
	}
	case GS_PLAY_TPS_CAM:
	{
		float rotSpeed = _GD->dt * 2.0f;
		if (_GD->keyboard[DIK_A] & 0x80)
		{
			m_yaw += rotSpeed;
		}
		if (_GD->keyboard[DIK_D] & 0x80)
		{
			m_yaw -= rotSpeed;
		}
		float speed = _GD->dt * 100.0f;

		Matrix rotMat = Matrix::CreateRotationY(m_yaw);
		Vector3 forward = Vector3::UnitX;

		if (_GD->keyboard[DIK_W] & 0x80)
		{
			m_pos += speed * Vector3::Transform(forward, rotMat);
		}

		if (_GD->keyboard[DIK_S] & 0x80)
		{
			m_pos -= speed * Vector3::Transform(forward, rotMat);
		}
		break;
	}
	}

	//Make sure we're still in the box!
	if (m_pos.x > 190.0f)
		m_pos.x = 190.0f;
	else if (m_pos.x < -190.0f)
		m_pos.x = -190.0f;

	if (m_pos.z > 190.0f)
		m_pos.z = 190.0f;
	else if (m_pos.z < -190.0f)
		m_pos.z = -190.0f;

	CMOGO::Tick(_GD);
}