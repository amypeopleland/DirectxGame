#ifndef __XfileH__
#define __XfileH__

#include "D3DObject.h"
#include <vector>


class Xfile:public D3DObject
{
public:
	Xfile();
	~Xfile();

	bool		CreateBuffer(IDirect3DDevice9* _device) {return true;};
	bool		CreateBuffer(IDirect3DDevice9* _device, char* _path);
	void		Render(IDirect3DDevice9* _device);

protected:

private:
	ID3DXMesh*                      Mesh;
	std::vector<D3DMATERIAL9>       Mtrls;
	std::vector<IDirect3DTexture9*> Textures;

};

#endif