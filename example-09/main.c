#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_READERS 5
#define NUM_WRITERS 2
#define SLEEP_TIME 1000 // 1 saniye

// Senkronizasyon Yapıları
HANDLE rw_mutex; // Yazma erişimini kontrol eden Semaphore (Mutex gibi davranır)
HANDLE read_count_mutex; // Okuyucu sayısını (read_count) koruyan Mutex
int read_count = 0; // Aktif okuyucu sayısı

// Ortak Kaynak
int shared_data = 0;

// Okuyucu Thread
DWORD WINAPI reader(LPVOID lpParam) {
    int reader_id = *(int*)lpParam;

    // Okuyucu sayacını güncellemek için Mutex'i kilitle
    WaitForSingleObject(read_count_mutex, INFINITE);
    read_count++;

    // Eğer bu ilk okuyucuysa, yazıcıları engellemek için rw_mutex'i kilitle
    if (read_count == 1) {
        WaitForSingleObject(rw_mutex, INFINITE); // Yazıcıların girmesini engeller
    }
    ReleaseMutex(read_count_mutex); // Mutex'i serbest bırak

    // OKUMA KRİTİK BÖLGESİ
    printf("Reader %d: Reading data (Value: %d)\n", reader_id, shared_data);
    Sleep(SLEEP_TIME); 

    // Okuyucu sayacını güncellemek için Mutex'i kilitle
    WaitForSingleObject(read_count_mutex, INFINITE);
    read_count--;

    // Eğer bu son okuyucuysa, yazıcının girmesine izin vermek için rw_mutex'i serbest bırak
    if (read_count == 0) {
        ReleaseMutex(rw_mutex); // Yazıcıların girmesine izin verir
    }
    ReleaseMutex(read_count_mutex); // Mutex'i serbest bırak

    return 0;
}

// Yazıcı Thread
DWORD WINAPI writer(LPVOID lpParam) {
    int writer_id = *(int*)lpParam;

    // Yazma erişimi için rw_mutex'i kilitle
    WaitForSingleObject(rw_mutex, INFINITE);

    // YAZMA KRİTİK BÖLGESİ
    shared_data++;
    printf("Writer %d: Writing data (New Value: %d)\n", writer_id, shared_data);
    Sleep(SLEEP_TIME); 

    // rw_mutex'i serbest bırak
    ReleaseMutex(rw_mutex);

    return 0;
}

int main() {
    // Handle'lar ve ID'ler
    HANDLE hReaders[NUM_READERS];
    HANDLE hWriters[NUM_WRITERS];
    int *reader_ids[NUM_READERS];
    int *writer_ids[NUM_WRITERS];

    // Mutex'leri oluştur
    rw_mutex = CreateMutex(NULL, FALSE, NULL);
    read_count_mutex = CreateMutex(NULL, FALSE, NULL);

    if (rw_mutex == NULL || read_count_mutex == NULL) {
        printf("Mutex creation failed (%lu)\n", GetLastError());
        return 1;
    }

    printf("Starting Readers-Writers Simulation...\n");

    // Thread'leri oluştur
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = (int*)malloc(sizeof(int));
        *reader_ids[i] = i + 1;
        hReaders[i] = CreateThread(NULL, 0, reader, reader_ids[i], 0, NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = (int*)malloc(sizeof(int));
        *writer_ids[i] = i + 1;
        hWriters[i] = CreateThread(NULL, 0, writer, writer_ids[i], 0, NULL);
    }

    // Tüm Thread'lerin bitmesini bekle
    HANDLE all_threads[NUM_READERS + NUM_WRITERS];
    for (int i = 0; i < NUM_READERS; i++) all_threads[i] = hReaders[i];
    for (int i = 0; i < NUM_WRITERS; i++) all_threads[NUM_READERS + i] = hWriters[i];

    WaitForMultipleObjects(NUM_READERS + NUM_WRITERS, all_threads, TRUE, INFINITE);

    // Kaynakları temizle
    CloseHandle(rw_mutex);
    CloseHandle(read_count_mutex);
    for (int i = 0; i < NUM_READERS; i++) { CloseHandle(hReaders[i]); free(reader_ids[i]); }
    for (int i = 0; i < NUM_WRITERS; i++) { CloseHandle(hWriters[i]); free(writer_ids[i]); }

    printf("\nSimulation complete. Final shared data value: %d\n", shared_data);
    return 0;
}