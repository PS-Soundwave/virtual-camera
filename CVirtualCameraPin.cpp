#include "CVirtualCameraPin.h"

#include <cstring>

CVirtualCameraPin::CVirtualCameraPin(TCHAR *pObjectName, HRESULT *phr, CSource *pms, LPCWSTR pName) : CSourceStream(pObjectName, phr, pms, pName) {
    // Initialize frame
    // ARIB STD-B28 with +I based test pattern.
    // Color bars divided into 5 segments, with the top segment having 100% alpha, with the alpha decreasing by 25%
    // each segment going down.
    // Colors are full-scale RGB. +I color probably isn't even close to right.
    // Pixels are RGBA (NB: Actual color interpretation depends on consumer. VLC expects RGBA, but OBS expects BGRA)
    // Pixels are ordered bottom-up within the frame.

    // Pattern 1
    for (int a = 0; a < 5; a++) {
        int y_end = (a + 1) * 126;
        int alpha = static_cast<int>(255 * (4 - a) / 4.0);
        for (int y = a * 126; y < y_end; y++) {
            for (int x = 0; x < 1920; x++) {
                if (x < 240 || x >= 1680) {
                    // Gray
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 102;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 102;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 102;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = alpha;
                }
                else if (x < 445) {
                    // White
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = alpha;
                }
                else if (x < 651) {
                    // Yellow
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 0;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = alpha;
                }
                else if (x < 857) {
                    // Cyan
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 0;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = alpha;
                }
                else if (x < 1063) {
                    // Green
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 0;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 0;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = alpha;
                }
                else if (x < 1269) {
                    // Magenta
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 0;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = alpha;
                }
                else if (x < 1475) {
                    // Red
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 0;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 0;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = alpha;
                }
                else {
                    // Blue
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 0;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 0;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 192;
                    frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = alpha;
                }
            }
        }
    }

    // Pattern 2
    for (int y = 630; y < 720; y++) {
        for (int x = 0; x < 1920; x++) {
            if (x < 240) {
                // Cyan
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 0;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 255;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 255;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
            else if (x < 445) {
                // +I
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 104;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 42;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 0;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
            else if (x < 1680) {
                // White
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 192;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 192;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 192;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
            else {
                // Blue
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 0;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 0;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 255;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
        }
    }

    // Pattern 3
    for (int y = 720; y < 810; y++) {
        for (int x = 0; x < 1920; x++) {
            if (x < 240) {
                // Yellow
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 255;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 255;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 0;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
            else if (x < 1680) {
                // Ramp
                int value = static_cast<int>(((x - 240) / 1440.0) * 255);
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = value;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = value;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = value;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
            else {
                // Red
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 255;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 0;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 0;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
        }
    }

    // Pattern 4
    for (int y = 810; y < 1080; y++) {
        for (int x = 0; x < 1920; x++) {
            if (x < 240 || (x >= 1680)) {
                // Gray
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 38;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 38;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 38;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
            else if (x < 549 || (x >= 960 && x < 1268) || (x >= 1337 && x < 1405) || (x >= 1474 && x < 1680)) {
                // Black and Blacker-Than-Black
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 0;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 0;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 0;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
            else if (x < 960) {
                // White
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 255;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 255;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 255;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
            else if (x < 1337) {
                // +2% Black
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 5;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 5;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 5;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
            else {
                // +4% Black
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4] = 10;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 1] = 10;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 2] = 10;
                frame[(FRAME_WIDTH * (1079 - y) + x) * 4 + 3] = 255;
            }
        }
    }
}

HRESULT CVirtualCameraPin::GetMediaType(CMediaType *pMediaType) {
    pMediaType->InitMediaType();
    pMediaType->SetType(MEDIA_TYPE);
    pMediaType->SetSubtype(MEDIA_SUBTYPE);
    pMediaType->SetSampleSize(1920 * 1080 * 4);
    pMediaType->SetTemporalCompression(FALSE);
    pMediaType->SetFormatType(&FORMAT_VideoInfo);

    VIDEOINFOHEADER header;

    header.rcSource.left = 0; // Don't know what these rects are for. I guess some filters will clip with them?
    header.rcSource.top = 0;
    header.rcSource.right = FRAME_HEIGHT;
    header.rcSource.bottom = FRAME_WIDTH;
    header.rcTarget = header.rcSource;
    header.dwBitRate = BITRATE;
    header.dwBitErrorRate = 0;
    header.AvgTimePerFrame = FRAME_TIME; // Number of 100 nanosecond periods in a frame

    header.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    header.bmiHeader.biWidth = FRAME_WIDTH;
    header.bmiHeader.biHeight = FRAME_HEIGHT; // Negative values are not respected by all consumers. Do not use.
    header.bmiHeader.biPlanes = 1;
    header.bmiHeader.biBitCount = 4 * DEPTH; // Bit depth
    header.bmiHeader.biCompression = BI_RGB; // Uncompressed RGB
    header.bmiHeader.biSizeImage = 0;
    header.bmiHeader.biXPelsPerMeter = RESOLUTION;
    header.bmiHeader.biYPelsPerMeter = RESOLUTION;
    header.bmiHeader.biClrUsed = 0; // Palette info. Unused.
    header.bmiHeader.biClrImportant = 0; // Palette info. Unused.

    pMediaType->SetFormat(reinterpret_cast<BYTE*>(&header), sizeof(VIDEOINFOHEADER)); // Safe, header is copied

    return S_OK;
}

HRESULT CVirtualCameraPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *ppropInputRequest) {
    ppropInputRequest->cBuffers = 1;
    ppropInputRequest->cbBuffer = FRAME_SIZE;
    ppropInputRequest->cbAlign = 8; // Machine word alignment
    ppropInputRequest->cbPrefix = 0;

    ALLOCATOR_PROPERTIES actual_props;

    pAlloc->SetProperties(ppropInputRequest, &actual_props);

    return S_OK;
}

HRESULT CVirtualCameraPin::FillBuffer(IMediaSample *pSamp) {
    pSamp->SetActualDataLength(FRAME_SIZE);
    pSamp->SetDiscontinuity(FALSE); // Discontinuity marker for e.g. restarting of compressed stream
    pSamp->SetMediaTime(nullptr, nullptr); // Stream timestamp
    pSamp->SetPreroll(FALSE); // Pre-roll may be treated differently by downstream filters
    pSamp->SetSyncPoint(TRUE); // According to documentation, uncompressed video should have a sync point every frame

    REFERENCE_TIME end = now + FRAME_TIME;

    // Timestamp within the source. Doesn't seem to be used for much for live sources, but might as well set to realtime
    pSamp->SetMediaTime(&now, &end);

    // Realtime timestamp (for syncing sources). Also many consumers cannot infer when it should fetch the next frame
    // without this timestamp set
    pSamp->SetTime(&now, &end);
    now = end;

    BYTE* buffer;

    pSamp->GetPointer(&buffer);

    std::memcpy(buffer, frame.data(), FRAME_SIZE);

    return S_OK;
}

HRESULT CVirtualCameraPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_IKsPropertySet) {
        return GetInterface(static_cast<IKsPropertySet*>(this), ppv);
    }
    else if (riid == IID_IAMStreamConfig) {
        return GetInterface(static_cast<IAMStreamConfig*>(this), ppv);
    }

    return CSourceStream::NonDelegatingQueryInterface(riid, ppv);
}

HRESULT CVirtualCameraPin::QueryInterface(REFIID riid, void **ppv) {
    return GetOwner()->QueryInterface(riid, ppv);
}

ULONG CVirtualCameraPin::AddRef() {
    return GetOwner()->AddRef();
}

ULONG CVirtualCameraPin::Release() {
    return GetOwner()->Release();
}

HRESULT CVirtualCameraPin::Get(REFGUID guidPropSet, DWORD dwPropId, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned) {
    if (guidPropSet != AMPROPSETID_Pin) {
        return E_PROP_SET_UNSUPPORTED;
    }

    if (dwPropId != AMPROPERTY_PIN_CATEGORY) {
        return E_PROP_ID_UNSUPPORTED;
    }

    *pcbReturned = sizeof(GUID);

    if (pPropData == nullptr) {
        return S_OK;
    }

    *reinterpret_cast<GUID*>(pPropData) = PIN_CATEGORY_CAPTURE;

    return S_OK;
}

HRESULT CVirtualCameraPin::QuerySupported(REFGUID guidPropSet, DWORD dwPropId, DWORD *pTypeSupport) {
    if (guidPropSet != AMPROPSETID_Pin) {
        return E_PROP_SET_UNSUPPORTED;
    }

    if (dwPropId != AMPROPERTY_PIN_CATEGORY) {
        return E_PROP_ID_UNSUPPORTED;
    }

    *pTypeSupport = KSPROPERTY_SUPPORT_GET;

    return S_OK;
}

HRESULT CVirtualCameraPin::Set(REFGUID guidPropSet, DWORD dwPropId, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData) {
    return E_PROP_SET_UNSUPPORTED;
}

HRESULT CVirtualCameraPin::SetFormat(AM_MEDIA_TYPE *pmt) {
    return S_OK;
}

HRESULT CVirtualCameraPin::GetFormat(AM_MEDIA_TYPE **ppmt) {
    CMediaType mediaType;

    mediaType.InitMediaType();
    mediaType.SetType(MEDIA_TYPE);
    mediaType.SetSubtype(MEDIA_SUBTYPE);
    mediaType.SetSampleSize(1920 * 1080 * 4);
    mediaType.SetTemporalCompression(FALSE);
    mediaType.SetFormatType(&FORMAT_VideoInfo);

    VIDEOINFOHEADER header;

    header.rcSource.left = 0; // Don't know what these rects are for. I guess some filters will clip with them?
    header.rcSource.top = 0;
    header.rcSource.right = FRAME_HEIGHT;
    header.rcSource.bottom = FRAME_WIDTH;
    header.rcTarget = header.rcSource;
    header.dwBitRate = BITRATE;
    header.dwBitErrorRate = 0;
    header.AvgTimePerFrame = FRAME_TIME; // Number of 100 nanosecond periods in a frame

    header.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    header.bmiHeader.biWidth = FRAME_WIDTH;
    header.bmiHeader.biHeight = FRAME_HEIGHT; // Negative values are not respected by all consumers. Do not use.
    header.bmiHeader.biPlanes = 1;
    header.bmiHeader.biBitCount = 4 * DEPTH; // Bit depth
    header.bmiHeader.biCompression = BI_RGB; // Uncompressed RGB
    header.bmiHeader.biSizeImage = 0;
    header.bmiHeader.biXPelsPerMeter = RESOLUTION; // Shouldn't matter. Photo quality.
    header.bmiHeader.biYPelsPerMeter = RESOLUTION;
    header.bmiHeader.biClrUsed = 0; // Palette info. Unused.
    header.bmiHeader.biClrImportant = 0; // Palette info. Unused.

    mediaType.SetFormat(reinterpret_cast<BYTE*>(&header), sizeof(VIDEOINFOHEADER)); // Safe, header is copied

    *ppmt = CreateMediaType(&mediaType);

    return S_OK;
}

HRESULT CVirtualCameraPin::GetNumberOfCapabilities(int *piCount, int *piSize) {
    *piCount = 1;
    *piSize = sizeof(VIDEO_STREAM_CONFIG_CAPS);
    return S_OK;
}

HRESULT CVirtualCameraPin::GetStreamCaps(int iIndex, AM_MEDIA_TYPE **pmt, BYTE *pSCC) {
    CMediaType mediaType;

    mediaType.InitMediaType();
    mediaType.SetType(MEDIA_TYPE);
    mediaType.SetSubtype(MEDIA_SUBTYPE);
    mediaType.SetSampleSize(1920 * 1080 * 4);
    mediaType.SetTemporalCompression(FALSE);
    mediaType.SetFormatType(&FORMAT_VideoInfo);

    VIDEOINFOHEADER header;

    header.rcSource.left = 0; // Don't know what these rects are for. I guess some filters will clip with them?
    header.rcSource.top = 0;
    header.rcSource.right = FRAME_HEIGHT;
    header.rcSource.bottom = FRAME_WIDTH;
    header.rcTarget = header.rcSource;
    header.dwBitRate = BITRATE;
    header.dwBitErrorRate = 0;
    header.AvgTimePerFrame = FRAME_TIME; // Number of 100 nanosecond periods in a frame

    header.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    header.bmiHeader.biWidth = FRAME_WIDTH;
    header.bmiHeader.biHeight = FRAME_HEIGHT; // Negative values are not respected by all consumers. Do not use.
    header.bmiHeader.biPlanes = 1;
    header.bmiHeader.biBitCount = 4 * DEPTH; // Bit depth
    header.bmiHeader.biCompression = BI_RGB; // Uncompressed RGB
    header.bmiHeader.biSizeImage = 0;
    header.bmiHeader.biXPelsPerMeter = RESOLUTION;
    header.bmiHeader.biYPelsPerMeter = RESOLUTION;
    header.bmiHeader.biClrUsed = 0; // Palette info. Unused.
    header.bmiHeader.biClrImportant = 0; // Palette info. Unused.

    mediaType.SetFormat(reinterpret_cast<BYTE*>(&header), sizeof(VIDEOINFOHEADER)); // Safe, header is copied

    *pmt = CreateMediaType(&mediaType);

    auto *vc = reinterpret_cast<VIDEO_STREAM_CONFIG_CAPS*>(pSCC);

    vc->guid = FORMAT_VideoInfo;
    vc->VideoStandard = 0; // No applicable standard
    vc->InputSize.cx = FRAME_WIDTH;
    vc->InputSize.cy = FRAME_HEIGHT;
    vc->MinCroppingSize.cx = FRAME_WIDTH;
    vc->MinCroppingSize.cy = FRAME_HEIGHT;
    vc->MaxCroppingSize.cx = FRAME_WIDTH;
    vc->MaxCroppingSize.cy = FRAME_HEIGHT;
    vc->CropGranularityX = 0;
    vc->CropGranularityY = 0;
    vc->CropAlignX = 0;
    vc->CropAlignY = 0;
    vc->MinOutputSize.cx = FRAME_WIDTH;
    vc->MinOutputSize.cy = FRAME_HEIGHT;
    vc->MaxOutputSize.cx = FRAME_WIDTH;
    vc->MaxOutputSize.cy = FRAME_HEIGHT;
    vc->OutputGranularityX = 0;
    vc->OutputGranularityY = 0;
    vc->StretchTapsX = 0;
    vc->StretchTapsY = 0;
    vc->ShrinkTapsX = 0;
    vc->ShrinkTapsY = 0;
    vc->MinFrameInterval = FRAME_TIME;
    vc->MaxFrameInterval = FRAME_TIME;
    vc->MinBitsPerSecond = BITRATE;
    vc->MaxBitsPerSecond = BITRATE;

    return S_OK;
}
