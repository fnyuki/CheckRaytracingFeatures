#include <string>

#include "CheckRaytracingFeatures.h"

int main(int argc, const char* argv[])
{
	HRESULT								hr			= 0;
	IDXGIFactory7*						factory		= nullptr;
	IDXGIAdapter1*						adapter		= nullptr;
	ID3D12Device6*						device		= nullptr;
	D3D12_FEATURE_DATA_D3D12_OPTIONS5	featureData	= {};

	if (!CreateFactory(hr, factory))
	{
		goto DISPOSE;
	}

	if (!CreateAdapter(factory, hr, adapter))
	{
		goto DISPOSE;
	}

	if (!CreateDevice(adapter, hr, device))
	{
		goto DISPOSE;
	}

	if (!CheckRaytracingFeatureSupport(device, hr, featureData))
	{
		goto DISPOSE;
	}

DISPOSE:
	SafeRelease(device);
	SafeRelease(adapter);
	SafeRelease(factory);

	if (SUCCEEDED(hr))
	{
		std::string tierMessage;
		tierMessage += "Raytracing tier: ";
		tierMessage += GetRaytracingTierString(featureData.RaytracingTier);
		ShowDialog("Success", tierMessage);
	}
	else
	{
		std::string errorMessage;
		errorMessage += "An error was occured.\n";
		errorMessage += GetErrorMessage(hr);
		ShowDialog("Error", errorMessage);
	}

	return (int)hr;
}

bool CreateFactory(HRESULT& hr, IDXGIFactory7*& factory)
{
	hr = CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));
	return SUCCEEDED(hr);
}

bool CreateAdapter(IDXGIFactory7* const factory, HRESULT& hr, IDXGIAdapter1*& adapter)
{
	DXGI_ADAPTER_DESC1 desc = {};
	for (auto adapterIndex = 0; factory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex)
	{
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}
		hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(hr))
		{
			return true;
		}
		SafeRelease(adapter);
	}
	return false;
}

bool CreateDevice(IDXGIAdapter1* const adapter, HRESULT& hr, ID3D12Device6*& device)
{
	hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
	return SUCCEEDED(hr);
}

bool CheckRaytracingFeatureSupport(ID3D12Device6* const device, HRESULT& hr, D3D12_FEATURE_DATA_D3D12_OPTIONS5& featureData)
{
	hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureData, sizeof(featureData));
	return SUCCEEDED(hr);
}