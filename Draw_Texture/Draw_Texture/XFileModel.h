//#include "d3dUtility.h"
#include "D3DUT.h"
#include <vector>



class XFileModel
{
public:
	XFileModel(IDirect3DDevice9* _p_Device);
	~XFileModel( );

	HRESULT LoadXFileModelFromFile( char* XFilename );
	HRESULT RenderXFlieModel();
	HRESULT Release();
	ID3DXMesh* getModelMesh();

private:
	IDirect3DDevice9*				p_Device ;				//…Ë±∏÷∏’Î
	ID3DXMesh*                      model_Mesh ;			//
	DWORD							d_num_Mtrls;			//
	D3DMATERIAL9*					model_Mtrls;			//
	LPDIRECT3DTEXTURE9*				model_Textures;			//

	
};