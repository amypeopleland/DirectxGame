//#include "d3dUtility.h"
#include <math.h>
#include <vector>
#include "D3DUT.h"

class BoundingSphere
{
public:
	BoundingSphere();
	BoundingSphere(IDirect3DDevice9* _bDevice);
	~BoundingSphere();

	bool CreatBoundingSphere(ID3DXMesh* mesh);
	void RenderBoundingSphere();
	void setBoundingPos(D3DXVECTOR3 _v);
	void setBoundingPos(float _x,float _y,float _z);
	bool computeBounding(BoundingSphere *roleBoundingBox);
	bool isBounding();
private:
	bool boundingFlag;

	IDirect3DDevice9* p_Device ;

	ID3DXMesh* SphereMesh;

	float f_r;

	D3DXVECTOR3 v_pos;
};