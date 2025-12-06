#include <windows.h>
#include <stdio.h>

#define THREAD_COUNT 5
#define MAX_THREADS_ALLOWED 2 // Kaynağa aynı anda erişebilecek maksimum thread sayısı

// Semaphore Tanımlama
HANDLE hSemaphore; 

// Thread'in çalıştıracağı fonksiyon
DWORD WINAPI thread_function(LPVOID lpParam) {
    DWORD thread_id = GetCurrentThreadId();

    printf("Thread %lu: Resource'a erisim izni bekliyor...\n", thread_id);

    // Semaphore'u kilitle: Kaynağa erişim hakkı talep et
    WaitForSingleObject(hSemaphore, INFINITE);

    // KRİTİK BÖLGE: Kaynağı kullanma simülasyonu
    printf("Thread %lu: Kaynaga girdi. (Kullanimda)\n", thread_id);
    Sleep(2000); // 2 saniye kaynağı kullanma simülasyonu
    printf("Thread %lu: Kaynagi kullandi ve cikiyor.\n", thread_id);

    // Semaphore'u serbest bırak: Kullanım hakkını iade et
    ReleaseSemaphore(hSemaphore, 1, NULL); // 1, serbest bırakılacak sayıyı belirtir

    return 0;
}

int main() {
    HANDLE hThreads[THREAD_COUNT];
    DWORD thread_ids[THREAD_COUNT];

    // 1. Semaphore'u Oluşturma
    hSemaphore = CreateSemaphore(
        NULL,                       // Güvenlik nitelikleri
        MAX_THREADS_ALLOWED,        // Başlangıç Sayısı (Aynı anda kaç thread girebilir)
        MAX_THREADS_ALLOWED,        // Maksimum Sayı
        NULL                        // Semaphore adı (İsimsiz)
    );

    if (hSemaphore == NULL) {
        printf("CreateSemaphore failed (%lu)\n", GetLastError());
        return 1;
    }

    // 2. Thread'leri Oluşturma
    printf("Toplam %d thread olusturuluyor, ancak sadece %d tanesi ayni anda calisacak.\n", THREAD_COUNT, MAX_THREADS_ALLOWED);
    for (int i = 0; i < THREAD_COUNT; i++) {
        hThreads[i] = CreateThread(
            NULL, 0, thread_function, NULL, 0, &thread_ids[i]
        );
    }

    // 3. Thread'lerin Bitmesini Bekleme
    WaitForMultipleObjects(THREAD_COUNT, hThreads, TRUE, INFINITE);

    // 4. Semaphore ve Thread Handle'larını Kapatma
    CloseHandle(hSemaphore);
    for (int i = 0; i < THREAD_COUNT; i++) {
        CloseHandle(hThreads[i]);
    }

    printf("\nButun threadler tamamlandi. Cikis.\n");
    return 0;
}