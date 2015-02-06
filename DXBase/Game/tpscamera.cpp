#include "tpscamera.h"
//a basic Third person camera which follows a give GameObject at a displacement of _dpos

TPSCamera::TPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, Vector3 _up, Vector3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _target->GetPos(), _up)
{
	m_targetObject = _target;
	m_dpos = _dpos;
}

TPSCamera::~TPSCamera()
{

}

void TPSCamera::Tick(GameData* _GD)
{
	//Set up position of camera and target position of camera based on new position and orientation of target object
	m_target = m_targetObject->GetPos();

	Matrix rotMat = Matrix::CreateRotationY(m_targetObject->GetYaw());
	m_pos = m_target + Vector3::Transform(m_dpos, rotMat);

	//and then set up proj and view matrices
	Camera::Tick(_GD);
}