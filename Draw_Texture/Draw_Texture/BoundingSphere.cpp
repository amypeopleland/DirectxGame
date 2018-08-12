#include "BoundingSphere.h"


BoundingSphere::BoundingSphere(IDirect3DDevice9* _bDevice)
{
	p_Device = _bDevice;
	SphereMesh = NULL;
	v_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	f_r = 0.0f;
	boundingFlag = false;
}
//
//BoundingSphere::~BoundingSphere()
//{
//	Release<ID3DXMesh*>(SphereMesh);
//}

bool BoundingSphere::CreatBoundingSphere(ID3DXMesh* mesh)
{
	//计算绑定盒
	HRESULT hr = 0;

	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&v_pos,
		&f_r);

	mesh->UnlockVertexBuffer();

	if( FAILED(hr) )
		return false;

 	D3DXCreateSphere(							
 		p_Device,
 		f_r,
 		25,
 		25,
 		&SphereMesh,
 		0);
}

void BoundingSphere::RenderBoundingSphere()
{	
	D3DMATERIAL9 Color =WHITE_MTRL;	   //指定绘制的外接体的材质
	if (boundingFlag)
	{
		Color = RED_MTRL;
	}
	Color.Diffuse.a = 0.10f; 			//指定绘制的外接体的透明度



	p_Device->SetMaterial(&Color);								
	p_Device->SetTexture(0, 0); 

	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	SphereMesh->DrawSubset(0);	

	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void BoundingSphere::setBoundingPos(D3DXVECTOR3 _v)
 {
 	v_pos = _v;
 }

void BoundingSphere::setBoundingPos(float _x,float _y,float _z)
{
	v_pos.x = _x;  
	v_pos.y = _y;
	v_pos.z = _z;
}

bool BoundingSphere::computeBounding(BoundingSphere* _boundingSphere)
{
	boundingFlag = false;
	
	_boundingSphere->boundingFlag = false;

	float _boundingDis = f_r + _boundingSphere->f_r;
	D3DXVECTOR3 _dis = v_pos - _boundingSphere ->v_pos;
	

	if(D3DXVec3Length(&_dis) < _boundingDis)
	{
		boundingFlag = true;
		_boundingSphere->boundingFlag = true;
		return true;
	}
	//*/
	return false;
}