#include <windows.h>
#include <stdio.h>

int main(void) {
    STARTUPINFO si;         // Yeni sürecin (child process) başlatma bilgilerini tutar
    PROCESS_INFORMATION pi; // Yeni sürecin (child process) kimlik bilgilerini tutar

    ZeroMemory(&si, sizeof(si)); // si yapısını sıfırla
    si.cb = sizeof(si);          // si yapısının boyutunu ayarla
    ZeroMemory(&pi, sizeof(pi)); // pi yapısını sıfırla

    // Komut satırında çalıştırılacak programı belirtin (Örn: cmd.exe)
    TCHAR command[] = TEXT("cmd.exe"); 

    // Süreç oluşturma çağrısı
    if (!CreateProcess(
        NULL,        // Modül adı (NULL, command'de belirtildiği için)
        command,     // Çalıştırılacak komut satırı
        NULL,        // Süreç güvenliği
        NULL,        // İş parçacığı güvenliği
        FALSE,       // Kalıtım (Handle kalıtımı)
        0,           // Oluşturma bayrakları
        NULL,        // Ortam bloğu
        NULL,        // Başlangıç dizini
        &si,         // STARTUPINFO yapısı
        &pi          // PROCESS_INFORMATION yapısı
    )) {
        printf("CreateProcess Failed (%lu).\n", GetLastError());
        return 1;
    }

    // Ebeveyn Süreç (Parent Process)
    printf("Parent Process: Child Process ID is %lu\n", pi.dwProcessId);
    printf("Parent Process: My PID is %lu\n", GetCurrentProcessId());

    // Çocuk sürecin bitmesini bekle (isteğe bağlı)
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Açık handle'ları kapat
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}