#ifndef _VB_PLANE_H_
#define _VB_PLANE_H_
#include "VBGO.h"
#include "vertex.h"
#include "drawdata.h"
#include "GameData.h"

#include <vector>

class VBPlane : public VBGO
{
public:
	VBPlane(){};
	virtual ~VBPlane(){};
	//{ delete[] m_vertices;};

	//initialize the Vertex and Index buffers for the cube
	void init(int _size, ID3D11Device* _GD);

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see Spiral, SpikedVB and Pillow
	void Transform();


	int m_size;
	int m_numVertices;
	myVertex* m_vertices;
};

#endif