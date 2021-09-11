#include <array>

#include "CVirtualCamera2.h"

HINSTANCE instance = nullptr;

constexpr std::array<REGPINTYPES, 1> mediaTypes{
    {
        &MEDIATYPE_Video,
        &MEDIASUBTYPE_ARGB32
    }
};

constexpr GUID id = {0x3CBE29AF, 0x374E, 0x4931, {0xB4, 0x3E, 0xCD, 0xAB, 0xEB, 0x66, 0x91, 0x74}};

std::wstring pinName(L"Output"); // NOLINT(cert-err58-cpp)

const std::array<REGFILTERPINS, 1> pins{
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

HRESULT RegisterServerKeys(BOOL reg) {
    HRESULT hr = S_OK;

    wchar_t cls[CHARS_IN_GUID];

    hr = StringFromGUID2(id, cls, CHARS_IN_GUID);

    if (FAILED(hr)) {
        return hr;
    }

    wchar_t key_name[MAX_PATH];

    hr = StringCchPrintfW(key_name, MAX_PATH, L"CLSID\\%ws", cls);

    if (FAILED(hr)) {
        return hr;
    }

    if (reg) {
        HKEY key;
        if(RegCreateKeyW(HKEY_CLASSES_ROOT, key_name, &key) != ERROR_SUCCESS) {
            return E_UNEXPECTED;
        }

        if(RegSetValueW(key, nullptr, REG_SZ, L"", sizeof(L"")) != ERROR_SUCCESS) {
            RegCloseKey(key);
            return E_UNEXPECTED;
        }

        HKEY subkey;
        if(RegCreateKeyW(key, L"InprocServer32", &subkey) != ERROR_SUCCESS) {
            RegCloseKey(key);
            return E_UNEXPECTED;
        }

        wchar_t filename[MAX_PATH];

        GetModuleFileNameW(instance, filename, MAX_PATH);

        if(RegSetValueW(subkey, nullptr, REG_SZ, filename, sizeof(filename)) != ERROR_SUCCESS) {
            RegCloseKey(subkey);
            RegCloseKey(key);
            return E_UNEXPECTED;
        }

        constexpr const wchar_t value[] = L"Both";
        if(RegSetValueExW(subkey, L"ThreadingModel", 0, REG_SZ, reinterpret_cast<const BYTE *>(value), sizeof(value))) {
            RegCloseKey(subkey);
            RegCloseKey(key);
            return E_UNEXPECTED;
        }

        RegCloseKey(subkey);
        RegCloseKey(key);
    }
    else {
        if(RegDeleteTreeW(HKEY_CLASSES_ROOT, key_name) != ERROR_SUCCESS) {
            return E_UNEXPECTED;
        }
    }

    return hr;
}

HRESULT RegisterServer(BOOL reg) {
    HRESULT hr = S_OK;

    if (reg) {
        hr = RegisterServerKeys(reg);
    }

    if (FAILED(hr)) {
        return hr;
    }

    CoInitialize(nullptr);

    IFilterMapper2 *fm;
    hr = CoCreateInstance(CLSID_FilterMapper2, nullptr, CLSCTX_INPROC_SERVER, IID_IFilterMapper2, reinterpret_cast<void**>(&fm));

    if (FAILED(hr)) {
        return hr;
    }

    if (reg) {
        REGFILTER2 rf;
        rf.dwVersion = 1;
        rf.dwMerit = MERIT_DO_NOT_USE;
        rf.cPins = 1;
        rf.rgPins = pins.data();

        hr = fm->RegisterFilter({0x3CBE29AF, 0x374E, 0x4931, {0xB4, 0x3E, 0xCD, 0xAB, 0xEB, 0x66, 0x91, 0x74}}, L"libdshowcapture-Based Virtual Camera", nullptr, &CLSID_VideoInputDeviceCategory, nullptr, &rf);
    }
    else {
        hr = fm->UnregisterFilter(&CLSID_VideoInputDeviceCategory, nullptr, {0x3CBE29AF, 0x374E, 0x4931, {0xB4, 0x3E, 0xCD, 0xAB, 0xEB, 0x66, 0x91, 0x74}});
    }
    if (FAILED(hr)) {
        return hr;
    }

    fm->Release();

    CoFreeUnusedLibraries();
    CoUninitialize();

    if (!reg) {
        hr = RegisterServerKeys(reg);
    }

    return hr;
}

[[maybe_unused]] HRESULT WINAPI DllRegisterServer() {
    return RegisterServer(TRUE);
}

[[maybe_unused]] HRESULT WINAPI DllUnregisterServer() {
    return RegisterServer(FALSE);
}

class VirtualCameraFactory : IClassFactory {
public:
    inline static volatile LONG locks{0};
    volatile LONG refs{0};

    VirtualCameraFactory();

    VirtualCameraFactory(const VirtualCameraFactory &o) = delete;
    VirtualCameraFactory(VirtualCameraFactory &&o) = delete;

    virtual ~VirtualCameraFactory() = default;

    VirtualCameraFactory& operator=(const VirtualCameraFactory &o) = delete;
    VirtualCameraFactory& operator=(VirtualCameraFactory &&o) = delete;

    STDMETHODIMP QueryInterface(REFIID riid, void **ppv) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    STDMETHODIMP CreateInstance(IUnknown *pUnk, REFIID riid, void **ppv) override;
    STDMETHODIMP LockServer(BOOL lock) override;
};

VirtualCameraFactory::VirtualCameraFactory() {
    VirtualCameraFactory::AddRef();
}

STDMETHODIMP VirtualCameraFactory::QueryInterface(REFIID riid, void **ppv) {
    if (ppv == nullptr) {
        return E_POINTER;
    }

    *ppv = nullptr;

    if (riid != IID_IUnknown && riid != IID_IClassFactory) {
        return E_NOINTERFACE;
    }

    *ppv = this;

    return S_OK;
}

STDMETHODIMP_(ULONG) VirtualCameraFactory::AddRef() {
    return InterlockedIncrement(&refs);
}

STDMETHODIMP_(ULONG) VirtualCameraFactory::Release() {
    long refCount = InterlockedDecrement(&refs);

    if (refCount == 0) {
        delete this;
    }

    return refCount;
}

STDMETHODIMP VirtualCameraFactory::CreateInstance(IUnknown *pUnk, REFIID riid, void **ppv) {
    if (ppv == nullptr) {
        return E_POINTER;
    }

    *ppv = nullptr;

    if (pUnk != nullptr) {
        return CLASS_E_NOAGGREGATION; // Aggregation requires specific IUnknown behavior that we don't implement
    }

    *ppv = new CVirtualCamera2();

    return S_OK;
}

STDMETHODIMP VirtualCameraFactory::LockServer(BOOL lock) {
    if (lock) {
        InterlockedIncrement(&locks);
    }
    else {
        InterlockedDecrement(&locks);
    }

    return S_OK;
}

[[maybe_unused]] HRESULT WINAPI DllCanUnloadNow() {
    if (InterlockedOr(&VirtualCameraFactory::locks, 0) == 0) {
        return S_OK;
    }

    return S_FALSE;
}

[[maybe_unused]] HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv) {
    if (ppv == nullptr) {
        return E_POINTER;
    }

    *ppv = nullptr;

    if (rclsid != id) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }
    if (riid != IID_IUnknown && riid != IID_IClassFactory) {
        return E_NOINTERFACE;
    }

    *ppv = new VirtualCameraFactory();

    return S_OK;
}

[[maybe_unused]] BOOL APIENTRY DllMain([[maybe_unused]] HINSTANCE hModule, [[maybe_unused]] DWORD dwReason, [[maybe_unused]] LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        InterlockedExchange(&VirtualCameraFactory::locks, 0);

        instance = hModule;
    }

    return TRUE;
}
