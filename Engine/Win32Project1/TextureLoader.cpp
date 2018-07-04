#include "stdafx.h"
#include "TextureLoader.h"


TextureLoader::TextureLoader()
{
}


TextureLoader::~TextureLoader()
{
}

HRESULT TextureLoader::LoadTexture(WCHAR* filename, ID3D11Device* dev, ID3D11ShaderResourceView** texture)
{
	 return D3DX11CreateShaderResourceViewFromFile(dev, filename, NULL, NULL, texture, NULL);
}
