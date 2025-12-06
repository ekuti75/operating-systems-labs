#include <windows.h>
#include <stdio.h>

// Kaynaklar (İki Mutex)
HANDLE ResourceA;
HANDLE ResourceB;

// Thread 1: Önce A'yı, sonra B'yi ister
DWORD WINAPI Thread1(LPVOID lpParam) {
    printf("Thread 1: Kaynak A'yı isteme.\n");
    WaitForSingleObject(ResourceA, INFINITE);
    printf("Thread 1: Kaynak A kilitlendi.\n");

    // Bir miktar bekleme, diğer thread'in A'yı kilitlediğinden emin olmak için
    Sleep(500); 

    printf("Thread 1: Kaynak B'yi isteme (Kilitlenme olası).\n");
    // Thread 2 bu noktada A'yı isterken B'yi tutuyor olabilir.
    WaitForSingleObject(ResourceB, INFINITE); 

    printf("Thread 1: Kaynak B kilitlendi. Basarili cikis.\n");

    // Kaynakları serbest bırak
    ReleaseMutex(ResourceB);
    ReleaseMutex(ResourceA);
    return 0;
}

// Thread 2: Önce B'yi, sonra A'yı ister (Deadlock oluşturur)
DWORD WINAPI Thread2(LPVOID lpParam) {
    printf("Thread 2: Kaynak B'yi isteme.\n");
    WaitForSingleObject(ResourceB, INFINITE);
    printf("Thread 2: Kaynak B kilitlendi.\n");

    // Bir miktar bekleme
    Sleep(500); 

    printf("Thread 2: Kaynak A'yı isteme (Deadlock olası).\n");
    // Thread 1 bu noktada B'yi isterken A'yı tutuyor olabilir.
    WaitForSingleObject(ResourceA, INFINITE); 

    printf("Thread 2: Kaynak A kilitlendi. Basarili cikis.\n");

    // Kaynakları serbest bırak
    ReleaseMutex(ResourceA);
    ReleaseMutex(ResourceB);
    return 0;
}

int main() {
    // Mutex'leri oluştur
    ResourceA = CreateMutex(NULL, FALSE, NULL);
    ResourceB = CreateMutex(NULL, FALSE, NULL);

    if (ResourceA == NULL || ResourceB == NULL) {
        printf("Mutex oluşturma hatası!\n");
        return 1;
    }

    // Thread'leri oluştur
    HANDLE hThread1 = CreateThread(NULL, 0, Thread1, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, Thread2, NULL, 0, NULL);

    // Thread'lerin bitmesini bekle (Ancak Deadlock olursa, burası hiç bitmez!)
    HANDLE threads[] = {hThread1, hThread2};
    printf("\nDeadlock simülasyonu başlatılıyor...\n");

    // Windows'ta timeout olmadan kilitlenme oluşursa program asılı kalır.
    WaitForMultipleObjects(2, threads, TRUE, 5000); // 5 saniye bekle

    DWORD wait_status = WaitForMultipleObjects(2, threads, TRUE, 0); // Durumu kontrol et

    if (wait_status == WAIT_TIMEOUT) {
        printf("\nDEADLOCK OLUSTU: Thread'ler 5 saniye icinde bitmedi.\n");
        // Gerçek bir uygulamada burada thread'ler sonlandırılmalıdır.
    } else {
        printf("\nSimulasyon basariyla tamamlandi (Deadlock olusmadi).\n");
    }

    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(ResourceA);
    CloseHandle(ResourceB);

    return 0;
}