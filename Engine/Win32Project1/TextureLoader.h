#pragma once
#include <d3d11.h>
#include <d3dx11tex.h>

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();
	HRESULT LoadTexture(WCHAR* filename, ID3D11Device* dev, ID3D11ShaderResourceView** texture);
};

