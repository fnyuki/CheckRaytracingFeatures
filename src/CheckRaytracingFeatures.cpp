#include "windows.h"
#include "d3d12.h"
#include "dxgi1_6.h"
#include "string"
#include "unordered_map"

int main()
{
	auto hr = 0;
	// UINT dxgiFactoryFlags = 0;
	IDXGIFactory7* factory = nullptr;
	IDXGIAdapter1* adapter = nullptr;
	ID3D12Device6* device = nullptr;
	CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));
	factory->Release();
}
