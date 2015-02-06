//base camera class
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "gameobject.h"

struct DrawData;

class Camera : public GameObject
{
public:
	Camera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, Vector3 _target, Vector3 _up);
	~Camera();

	virtual void Tick(GameData* _GD);

	virtual void Draw(DrawData* _DS);

	Matrix GetProj(){ return m_projMat; }
	Matrix GetView(){ return m_viewMat; }

	Vector3 GetTarget(){ return m_target; }
	Vector3 GetUp(){ return m_up; }

protected:

	Matrix m_projMat;
	Matrix m_viewMat;

	Vector3 m_target;
	Vector3 m_up;

	float m_fieldOfView;
	float m_aspectRatio;
	float m_nearPlaneDistance;
	float m_farPlaneDistance;

};


#endif