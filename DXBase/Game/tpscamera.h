#ifndef _TPSCAMERA_H_
#define _TPSCAMERA_H_
#include "camera.h"
//a basic Third person camera which follows a give GameObject at a displacement of _dpos

class TPSCamera : public Camera
{
public:
	TPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, Vector3 _up, Vector3 _dpos);
	virtual ~TPSCamera();

	virtual void Tick(GameData* GD);

protected:
	GameObject*	m_targetObject;
	Vector3	m_dpos;
};

#endif