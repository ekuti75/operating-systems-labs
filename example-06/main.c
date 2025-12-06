#include <windows.h>
#include <stdio.h>

#define THREAD_COUNT 2
#define INCREMENT_COUNT 100000

// Ortak Kaynak
long long global_counter = 0;

// Mutex Tanımlama (Global'de olmalı)
HANDLE hMutex; 

// Thread'in çalıştıracağı fonksiyon
DWORD WINAPI thread_function(LPVOID lpParam) {
    DWORD thread_id = GetCurrentThreadId();

    for (int i = 0; i < INCREMENT_COUNT; i++) {
        // Mutex'i kilitle: Kritik bölgeye girmeye çalış
        WaitForSingleObject(hMutex, INFINITE);

        // KRİTİK BÖLGE: Ortak kaynağa erişim
        global_counter++;

        // Mutex'i serbest bırak: Kritik bölgeden çık
        ReleaseMutex(hMutex);
    }

    printf("Thread %lu finished incrementing.\n", thread_id);
    return 0;
}

int main() {
    HANDLE hThreads[THREAD_COUNT];
    DWORD thread_ids[THREAD_COUNT];

    // 1. Mutex'i Oluşturma
    hMutex = CreateMutex(
        NULL,           // Güvenlik nitelikleri
        FALSE,          // Başlangıçta sahip değil (kilitsiz)
        NULL            // Mutex adı (İsimsiz)
    );

    if (hMutex == NULL) {
        printf("CreateMutex failed (%lu)\n", GetLastError());
        return 1;
    }

    // 2. Thread'leri Oluşturma
    printf("Creating %d threads to access the counter...\n", THREAD_COUNT);
    for (int i = 0; i < THREAD_COUNT; i++) {
        hThreads[i] = CreateThread(
            NULL, 0, thread_function, NULL, 0, &thread_ids[i]
        );
        if (hThreads[i] == NULL) {
            printf("CreateThread failed for thread %d (%lu)\n", i, GetLastError());
            return 1;
        }
    }

    // 3. Thread'lerin Bitmesini Bekleme
    WaitForMultipleObjects(THREAD_COUNT, hThreads, TRUE, INFINITE);

    // 4. Sonuçları Görüntüleme
    printf("\nSynchronization complete.\n");
    printf("Expected Counter Value: %lld\n", (long long)THREAD_COUNT * INCREMENT_COUNT);
    printf("Actual Counter Value: %lld\n", global_counter);

    // 5. Mutex ve Thread Handle'larını Kapatma
    CloseHandle(hMutex);
    for (int i = 0; i < THREAD_COUNT; i++) {
        CloseHandle(hThreads[i]);
    }

    return 0;
}