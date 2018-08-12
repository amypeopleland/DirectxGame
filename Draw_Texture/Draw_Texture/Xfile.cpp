#include "Xfile.h"


Xfile::Xfile()
{

}

Xfile::~Xfile()
{
	Mesh->Release();

	for(int i = 0; i < Textures.size(); i++)
	{
		if (Textures[i] != NULL)
		{
			Textures[i]->Release();
		}
	}
	Textures.clear();

}

bool	Xfile::CreateBuffer(IDirect3DDevice9* _device, char* _path)
{

	HRESULT hr = 0;

	//
	// Load the XFile data.
	//

	ID3DXBuffer* adjBuffer  = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls   = 0;

	hr = D3DXLoadMeshFromX(  
		_path,
		D3DXMESH_MANAGED,
		_device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&Mesh);

	if(FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	//
	// Extract the materials, and load textures.
	//

	if( mtrlBuffer != 0 && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for(int i = 0; i < numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != 0 )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					_device,
					mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				Textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back( 0 );
			}
		}
	}
	mtrlBuffer->Release();
	

	if(FAILED(hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}

	//
	// Set texture filters.
	//

	_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);


	return true;
}

void	Xfile::Render(IDirect3DDevice9* _device)
{
	_device->SetTransform( D3DTS_WORLD, &m_Transform);
	for(int i = 0; i < Mtrls.size(); i++)
	{
		_device->SetMaterial( &Mtrls[i] );
		_device->SetTexture(0, Textures[i]);
		Mesh->DrawSubset(i);
	}	
}

//*/