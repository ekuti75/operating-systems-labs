#include <windows.h>
#include <stdio.h>

int main(void) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si)); 
    si.cb = sizeof(si);          
    ZeroMemory(&pi, sizeof(pi)); 

    TCHAR command[] = TEXT("cmd.exe"); 

    if (!CreateProcess(
        NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi
    )) {
        printf("Parent: Failed to create child process.\n");
        return 1;
    }

    // Ebeveyn Süreç (Parent Process)
    printf("Parent Process (PID %lu): Child Process (PID %lu) started.\n", 
            GetCurrentProcessId(), pi.dwProcessId);

    // KRİTİK ADIM: Ebeveyn, Çocuk sürecin bitmesini bekler.
    printf("Parent Process: Waiting for the child to terminate...\n");

    // Sınırsız süre (INFINITE) bekler. Çocuk süreç sonlandığında döner.
    WaitForSingleObject(pi.hProcess, INFINITE);

    printf("Parent Process: Child process terminated. Continuing execution.\n");

    // Açık handle'ları kapat
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}