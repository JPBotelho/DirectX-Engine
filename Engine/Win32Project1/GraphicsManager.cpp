#include "stdafx.h"
#include "GraphicsManager.h"
#include "WindowManager.h"
#include "LogManager.h"
#include "ModelClass.h"
#include "Camera.h"
#include "TextureLoader.h"


#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")


#include <D3DCompiler.h>




LogManager *logger = new LogManager();

IDXGISwapChain *swapchain;  // the pointer to the swap chain interface
ID3D11Device *dev;          // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;// the pointer to our Direct3D device context

ID3D11RenderTargetView *backbuffer;

int width;
int height;
float deltaTime;
float timeElapsed;

ID3D11VertexShader *vertShader;
ID3D11PixelShader *pixelShader;

struct VERTEX
{
	FLOAT X, Y, Z;      // position
	D3DXCOLOR Color;    // color
};

ID3D11Buffer *constBuffer;
ID3D11InputLayout *pLayout;

ID3D10Blob *vertLog = nullptr;
ID3D10Blob *pixelLog = nullptr;

ID3D11ShaderResourceView* texture = 0;

D3D11_INPUT_ELEMENT_DESC ied[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD0", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

__declspec(align(16))
struct CameraConstants {
	float dTime;
	float time;
	float pad[2];
	D3DXMATRIX projectionMatrix;
};

CameraConstants constants;
Camera* camera = new Camera();
D3DXMATRIX projMatrix, viewMatrix, worldMatrix;


GraphicsManager::GraphicsManager(float x, float y)
{
	width = x;
	height = y;
}


GraphicsManager::~GraphicsManager()
{
}

#pragma region Initialize

HRESULT GraphicsManager::InitializeGraphics(HWND hWnd)
{
	HRESULT action = InitD3D(hWnd);
	if (FAILED(action))
	{
		return action;
	}

	action = InitShaders(0);
	if (FAILED(action))
	{
		return action;
	}

	ModelClass* model = new ModelClass;
	action = model->InitializeBuffers(dev, devcon, logger);
	if (FAILED(action))
	{
		return action;
	}
	
	action = CreateConstBuffer();
	if (FAILED(action))
	{
		return action;
	}

	action = SetTexture();
	if (FAILED(action))
	{
		return action;
	}

	return action;
}

// this function initializes and prepares Direct3D for use
HRESULT GraphicsManager::InitD3D(HWND hWnd)
{
	logger->Open();
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = 1;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

															// create a device, device context and swap chain using the information in the scd struct
	HRESULT createDevice = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	if (FAILED(createDevice))
	{
		return createDevice;
	}

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	HRESULT bufferCreated = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(bufferCreated))
	{
		return bufferCreated;
	}

	// use the back buffer address to create the render target
	HRESULT createTarget = dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	if (FAILED(createTarget))
	{
		return createTarget;
	}

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 1920;
	viewport.Height = 1080;

	devcon->RSSetViewports(1, &viewport);

	return S_OK;
}

HRESULT GraphicsManager::InitShaders(bool clearLog)
{
	if (clearLog)
		logger->Open();
	logger->Append("Compiling shaders...\n");

	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", D3D10_SHADER_DEBUG, 0, 0, &VS, &vertLog, 0);
	D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", D3D10_SHADER_DEBUG, 0, 0, &PS, &pixelLog, 0);
	
	if (vertLog != nullptr || pixelLog != nullptr)
	{
		logger->Append("\nShader compilation failed: \n");
		LPVOID vertPtr = vertLog->GetBufferPointer();

		logger->Append((char*)vertPtr);
		return E_ABORT;
	}
	else if (vertLog == nullptr && pixelLog == nullptr)
	{
		logger->Append("Shader compilation successful.");
	}


	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &vertShader);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pixelShader);
	devcon->VSSetShader(vertShader, 0, 0);
	devcon->PSSetShader(pixelShader, 0, 0);
	dev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);

	return S_OK;
}

#pragma endregion Initialize

void GraphicsManager::RenderFrame()
{
	UpdateConstBuffer();
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(255, 255, 255, 0));

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//devcon->DrawIndexed(60,0, 0);
	devcon->Draw(60, 0);

	//Present(0,0) for no VSync
	swapchain->Present(1, 0);
}

void GraphicsManager::EndD3D()
{
	swapchain->SetFullscreenState(FALSE, NULL);
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
	vertShader->Release();
	pixelShader->Release();
	if(texture != nullptr)
		texture->Release();
	if(vertLog != nullptr)
		vertLog->Release();
	if(pixelLog != nullptr)
		pixelLog->Release();
	if(pLayout != nullptr)
		pLayout->Release();
}

#pragma region Constants

HRESULT GraphicsManager::SetTexture()
{
	TextureLoader* t = new TextureLoader();
	HRESULT action = t->LoadTexture(L"seafloor.dds", dev, &texture);

	if (FAILED(action))
	{
		logger->Append("\n Texture not found!");
		return action;
	}

	ID3D11SamplerState* sampleState;
	D3D11_SAMPLER_DESC samplerDesc;


	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	action = dev->CreateSamplerState(&samplerDesc, &sampleState);
	if (FAILED(action))
	{
		logger->Append("\n Texture not found!");
		return action;
	}

	devcon->PSSetShaderResources(0, 1, &texture);
	devcon->PSSetSamplers(0, 1, &sampleState);

	return action;
}

HRESULT GraphicsManager::CreateConstBuffer()
{
	static_assert (sizeof(CameraConstants) % 16 == 0, "Size is not correct");
	
	camera->GetProjectionMatrix(&projMatrix);
	camera->GetViewMatrix(&viewMatrix);
	camera->GetWorldMatrix(&worldMatrix);

	worldMatrix *= viewMatrix;
	worldMatrix *= projMatrix;

	constants.projectionMatrix = worldMatrix;

	D3D11_BUFFER_DESC cbDesc;

	cbDesc.ByteWidth = sizeof(CameraConstants);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &constants;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT res = dev->CreateBuffer(&cbDesc, &InitData, &constBuffer);
	if (FAILED(res))
	{
		logger->Append("\nERROR: Creating constant buffer failed." );
	}
	devcon->VSSetConstantBuffers(0, 1, &constBuffer);
	//devcon->PSSetConstantBuffers(0, 1, &constBuffer);

	return res;
}

void GraphicsManager::UpdateConstBuffer()
{
	constants.dTime = 100;
	constants.time = 100;

	camera->GetProjectionMatrix(&projMatrix);
	camera->GetViewMatrix(&viewMatrix);
	camera->GetWorldMatrix(&worldMatrix);
	worldMatrix *= viewMatrix;
	worldMatrix *= projMatrix;

	constants.projectionMatrix = worldMatrix;


	D3D11_MAPPED_SUBRESOURCE resource;
	devcon->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &constants, sizeof(constants));
	devcon->Unmap(constBuffer, 0);
}

#pragma endregion Constants

#pragma region Inputs

void GraphicsManager::MoveCamera(D3DXVECTOR3 direction)
{
	camera->AddPosition(direction);
}

#pragma endregion Inputs