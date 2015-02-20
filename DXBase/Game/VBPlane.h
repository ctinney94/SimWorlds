#ifndef _VB_PLANE_H_
#define _VB_PLANE_H_
#include "VBGO.h"
#include "vertex.h"
#include "drawdata.h"
#include "GameData.h"

class VBPlane : public VBGO
{
public:
	VBPlane();
	virtual ~VBPlane(){ delete[] m_vertices;};

	//initialise the Veretx and Index buffers for the cube
	void init(int _size, ID3D11Device* _GD);
	void Draw(DrawData* _DD);
	void Tick(GameData* _GD);

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see Spirla, SpikedVB and Pillow
	void Transform(GameData* _GD);

	int numVerts;
	int m_size;
	float changeMe;
	int m_numVertices;
	myVertex* m_vertices;
};

#endif