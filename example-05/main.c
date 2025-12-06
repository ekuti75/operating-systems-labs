#include <windows.h>
#include <stdio.h>

DWORD WINAPI thread_function(LPVOID lpParam) {
    // Ana thread'den gelen veriyi (integer) alır
    int *data = (int *)lpParam;
    
    // Thread ID'sini al (Windows API'si)
    DWORD thread_id = GetCurrentThreadId();
    
    printf("Thread (ID: %lu): Baslatiliyor. Gelen veri: %d\n", thread_id, *data);
    
    // 2 saniye bekle
    Sleep(2000); // Milisaniye cinsinden

    printf("Thread (ID: %lu): Calisma tamamlandi.\n", thread_id);
    
    return 0; // Thread sonlanır
}

int main() {
    HANDLE hThread; // Thread handle'ını tutacak değişken
    DWORD thread_id;
    int value = 100;

    printf("Ana Süreç: Yeni bir thread olusturuluyor...\n");
    
    // Thread oluşturma
    hThread = CreateThread(
        NULL,           // Thread güvenliği
        0,              // Başlangıç yığını boyutu
        thread_function,// Thread'in çalıştıracağı fonksiyon
        &value,         // Fonksiyona aktarılacak argüman
        0,              // Oluşturma bayrakları (0 = hemen çalıştır)
        &thread_id      // Thread ID'sini alan değişken
    );

    if (hThread == NULL) {
        perror("Thread olusturma basarisiz");
        return 1;
    }

    printf("Ana Süreç: Thread'in calismasi bekleniyor (WaitForSingleObject).\n");

    // KRİTİK ADIM: Ana thread, thread'in bitmesini bekler (pthread_join eşdeğeri)
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread); // Handle'ı kapat

    printf("Ana Süreç: Thread basariyla tamamlandi. Cikis yapiliyor.\n");
    return 0;
}