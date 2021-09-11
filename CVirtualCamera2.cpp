#include "CVirtualCamera2.h"

CVirtualCamera2::CVirtualCamera2() : DShow::OutputFilter(DShow::VideoFormat::YUY2, 1920, 1080, 333333) {
    start = CreateEventW(nullptr, true, false, nullptr);
    stop = CreateEventW(nullptr, true, false, nullptr);

    thread = std::thread([this] {
        long FRAME_WIDTH = 1920;

        std::vector<uint8_t> frame(1920 * 1080 * 4);

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

        HANDLE objs[2]{start, stop};
        if(WaitForMultipleObjects(2, objs, false, INFINITE) != WAIT_OBJECT_0) {
            return;
        }

        LARGE_INTEGER timestamp;
        LARGE_INTEGER frequency_counts;

        QueryPerformanceCounter(&timestamp);
        QueryPerformanceFrequency(&frequency_counts);

        auto frequency = static_cast<double>(frequency_counts.QuadPart);
        double counts = timestamp.QuadPart;
        counts *= 10000000.0; // Convert to 100ns periods

        auto time = static_cast<uint64_t>(counts / frequency);

        uint64_t stream_time;

        if (clock != nullptr) {
            REFERENCE_TIME ref_time;
            HRESULT hr = clock->GetTime(&ref_time);
            if (!FAILED(hr)) {
                stream_time = ref_time;
            }
            else {
                stream_time = time;
            }
        }
        else {
            stream_time = time;
        }

        while (WaitForSingleObject(stop, 0) == WAIT_TIMEOUT) {
            uint8_t* buf;
            if (LockSampleData(&buf)) {
                memcpy(buf, frame.data(), 1920 * 1080 * 16 / 8);

                UnlockSampleData(stream_time, stream_time + 333333); // NOLINT (erroneous narrowing warning)
            }

            uint64_t now = time;
            time += 333333;

            while (now < time) {
                QueryPerformanceCounter(&timestamp);

                counts = timestamp.QuadPart;
                counts *= 10000000.0; // Convert to 100ns periods

                now = static_cast<uint64_t>(counts / frequency);

                Sleep(0);
            }

            stream_time += 333333;
        }
    });

    CVirtualCamera2::AddRef();
}

STDMETHODIMP CVirtualCamera2::Pause() {
    HRESULT hr = DShow::OutputFilter::Pause();
    if (FAILED(hr)) {
        return hr;
    }

    SetEvent(start);

    return S_OK;
}

CVirtualCamera2::~CVirtualCamera2() {
    SetEvent(stop);
    thread.join();
}
