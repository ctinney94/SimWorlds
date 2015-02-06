#ifndef _SPIRAL_H_
#define _SPIRAL_H_
#include "VBCube.h"
//create a twisted cube VBGO

class Spiral : public VBCube
{
public:
	Spiral(){};
protected:
	virtual void Transform()
	{
		for (int i = 0; i<m_numPrims * 3; i++)
		{
			Vector3 vertPos = m_vertices[i].Pos;

			Matrix rotMat = Matrix::CreateFromYawPitchRoll(XM_PI*(vertPos.y + 5.0f) / 20.0f, 0.0f, 0.0f);
			
			Vector3 newPos = Vector3::Transform(vertPos, rotMat);

			m_vertices[i].Pos = newPos;
		}
	}
};

#endif