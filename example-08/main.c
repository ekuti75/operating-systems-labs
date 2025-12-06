#include <windows.h>
#include <stdio.h>

// Senkronizasyon Yapıları
CRITICAL_SECTION buffer_lock;
CONDITION_VARIABLE can_produce; // Üretici için koşul: Arabellek boş mu?
CONDITION_VARIABLE can_consume; // Tüketici için koşul: Arabellek dolu mu?

// Paylaşılan Kaynak (Arabellek)
int buffer_item = 0;
int is_empty = 1; // Başlangıçta arabellek boş

// Üretici Thread
DWORD WINAPI producer(LPVOID lpParam) {
    EnterCriticalSection(&buffer_lock); // Kritik bölgeye gir

    // Arabellek doluysa bekleyin (Tüketici boşaltana kadar)
    while (is_empty == 0) {
        printf("Producer: Buffer dolu, bekliyor...\n");
        SleepConditionVariableCS(&can_produce, &buffer_lock, INFINITE);
    }

    // Üretim yap
    buffer_item = 42;
    is_empty = 0;
    printf("Producer: Urun (42) uretildi.\n");

    LeaveCriticalSection(&buffer_lock); // Kritik bölgeden çık

    // Tüketiciyi uyandır
    WakeConditionVariable(&can_consume);

    return 0;
}

// Tüketici Thread
DWORD WINAPI consumer(LPVOID lpParam) {
    EnterCriticalSection(&buffer_lock); // Kritik bölgeye gir

    // Arabellek boşsa bekleyin (Üretici doldurana kadar)
    while (is_empty == 1) {
        printf("Consumer: Buffer bos, bekliyor...\n");
        SleepConditionVariableCS(&can_consume, &buffer_lock, INFINITE);
    }

    // Tüketim yap
    printf("Consumer: Urun (%d) tüketildi.\n", buffer_item);
    buffer_item = 0;
    is_empty = 1;

    LeaveCriticalSection(&buffer_lock); // Kritik bölgeden çık

    // Üreticiyi uyandır
    WakeConditionVariable(&can_produce);

    return 0;
}

int main() {
    HANDLE hProducer, hConsumer;

    // Senkronizasyon yapılarını başlat
    InitializeCriticalSection(&buffer_lock);
    InitializeConditionVariable(&can_produce);
    InitializeConditionVariable(&can_consume);

    // Thread'leri oluştur
    hProducer = CreateThread(NULL, 0, producer, NULL, 0, NULL);
    hConsumer = CreateThread(NULL, 0, consumer, NULL, 0, NULL);

    // Thread'lerin bitmesini bekle
    HANDLE threads[] = {hProducer, hConsumer};
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    // Kaynakları temizle
    CloseHandle(hProducer);
    CloseHandle(hConsumer);
    DeleteCriticalSection(&buffer_lock);

    printf("\nProgram tamamlandi.\n");
    return 0;
}