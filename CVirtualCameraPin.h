#ifndef CVIRTUALCAMERAPIN_H
#define CVIRTUALCAMERAPIN_H

#include <cstdint>
#include <array>

#include "baseclasses/streams.h"

class CVirtualCameraPin : public CSourceStream, public IKsPropertySet, public IAMStreamConfig {
public:
    static constexpr const GUID* const MEDIA_TYPE = &MEDIATYPE_Video;
    static constexpr const GUID* const MEDIA_SUBTYPE = &MEDIASUBTYPE_ARGB32;

    CVirtualCameraPin(TCHAR *pObjectName, HRESULT *phr, CSource *pms, LPCWSTR pName);
    CVirtualCameraPin(const CVirtualCameraPin &o) = delete;
    CVirtualCameraPin(CVirtualCameraPin &&o) = delete;

    ~CVirtualCameraPin() override = default;

    CVirtualCameraPin& operator=(const CVirtualCameraPin &o) = delete;
    CVirtualCameraPin& operator=(CVirtualCameraPin &&o) = delete;

    HRESULT GetMediaType(CMediaType *pMediaType) override;
    HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *ppropInputRequest) override;
    HRESULT FillBuffer(IMediaSample *pSamp) override;

    // Must be implemented to expose interfaces
    HRESULT NonDelegatingQueryInterface(REFIID riid, void **ppv) override;

    // IUnknown (sub-interface of both interfaces)
    HRESULT QueryInterface(REFIID riid, void **ppv) override;
    ULONG AddRef() override;
    ULONG Release() override;

    // IKsPropertySet
    // Has to at least expose that there is a capture device pin, or else some consumers will not take the source at
    // face value and refuse to work with it
    HRESULT Get(REFGUID guidPropSet, DWORD dwPropId, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned) override;
    HRESULT QuerySupported(REFGUID guidPropSet, DWORD dwPropID, DWORD *pTypeSupport) override;
    HRESULT Set(REFGUID guidPropSet, DWORD dwPropId, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData) override;

    // IAMStreamConfig
    // Not strictly necessary, but OBS will give up on the source if it doesn't at least respond to an attempt to adjust
    // its options.
    HRESULT SetFormat(AM_MEDIA_TYPE *pmt) override;
    HRESULT GetFormat(AM_MEDIA_TYPE **ppmt) override;
    HRESULT GetNumberOfCapabilities(int *piCount, int *piSize) override;
    HRESULT GetStreamCaps(int iIndex, AM_MEDIA_TYPE **pmt, BYTE *pSCC) override;

private:
    static constexpr long DEPTH = 4; // Bit depth in bytes
    static constexpr long FRAME_WIDTH = 1920;
    static constexpr long FRAME_HEIGHT = 1080;
    static constexpr long FRAME_SIZE = FRAME_HEIGHT * FRAME_WIDTH * DEPTH;
    static constexpr long FPS = 30;
    static constexpr long BITRATE = FRAME_SIZE * FPS * 8;
    static constexpr long FRAME_TIME = 166667; // Frame length in 100s of nanoseconds (60fps)
    static constexpr long RESOLUTION = 12000; // Shouldn't matter. Photo quality.

    std::array<uint8_t, FRAME_SIZE> frame{};
    REFERENCE_TIME now = 0;
};

#endif
