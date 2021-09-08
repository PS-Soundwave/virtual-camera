#include <array>

#include "CVirtualCamera.h"

constexpr std::array<AMOVIESETUP_MEDIATYPE, 1> mediaTypes{
    {
        CVirtualCameraPin::MEDIA_TYPE,
        CVirtualCameraPin::MEDIA_SUBTYPE
    }
};

std::wstring pinName(CVirtualCamera::PIN_NAME); // NOLINT(cert-err58-cpp)

const std::array<AMOVIESETUP_PIN, 1> pins{
    {
        &pinName[0], // Name (Unused)
        FALSE, // Not rendered (never true for an output)
        TRUE, // Is output
        FALSE, // Cannot have zero of this pin
        FALSE, // Cannot have more than one of this pin
        &GUID_NULL, // Unused
        nullptr, // Unused
        1, // Number of types
        mediaTypes.data()
    }
};

constexpr AMOVIESETUP_FILTER filter{
    &CVirtualCamera::CLSID_VIRTUAL_CAMERA,
    CVirtualCamera::NAME,
    MERIT_DO_NOT_USE, // Is not of merit, i.e. is not a filter that should be exposed as mid-chain in compositing software
    1, // Number of pins
    pins.data()
};

CFactoryTemplate g_Templates[1] = {
    {
        CVirtualCamera::NAME,
        &CVirtualCamera::CLSID_VIRTUAL_CAMERA,
        CVirtualCamera::CreateInstance,
        nullptr, // DLL init function
        &filter
    }
};

int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

STDAPI RegisterServer(BOOL reg) {
    HRESULT hr = S_OK;

    hr = AMovieDllRegisterServer2(reg);
    if (FAILED(hr)) {
        return hr;
    }

    IFilterMapper2 *fm;
    hr = CoCreateInstance(CLSID_FilterMapper2, nullptr, CLSCTX_INPROC_SERVER, IID_IFilterMapper2, reinterpret_cast<void**>(&fm));
    if (FAILED(hr)) {
        return hr;
    }

    if (reg) {
        REGFILTER2 rf;
        rf.dwVersion = 1;
        rf.dwMerit = filter.dwMerit;
        rf.cPins = filter.nPins;
        rf.rgPins = pins.data();

        hr = fm->RegisterFilter(CVirtualCamera::CLSID_VIRTUAL_CAMERA, CVirtualCamera::NAME, nullptr, &CLSID_VideoInputDeviceCategory, nullptr, &rf);
    }
    else {
        hr = fm->UnregisterFilter(&CLSID_VideoInputDeviceCategory, nullptr, CVirtualCamera::CLSID_VIRTUAL_CAMERA);
    }
    if (FAILED(hr)) {
        return hr;
    }

    fm->Release();

    return hr;
}

STDAPI DllRegisterServer() {
    return RegisterServer(TRUE);
}

STDAPI DllUnregisterServer() {
    return RegisterServer(FALSE);
}

extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  dwReason, LPVOID lpReserved)
{
    return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}
