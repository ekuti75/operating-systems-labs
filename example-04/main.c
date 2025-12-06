#include <windows.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 256
#define MESSAGE "Hello from Parent via Windows Pipe!"

int main(void) {
    HANDLE hReadPipe, hWritePipe;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD dwWritten, dwRead;
    char buffer[BUFSIZE];

    // 1. Boruyu oluştur
    if (!CreatePipe(&hReadPipe, &hWritePipe, NULL, 0)) {
        printf("Parent: CreatePipe failed.\n");
        return 1;
    }

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // Borunun okuma ucunu child process'in standart girişine yönlendir
    si.hStdInput = hReadPipe;
    si.dwFlags |= STARTF_USESTDHANDLES;
    
    ZeroMemory(&pi, sizeof(pi));

    // 2. Çocuk Süreci oluştur
    TCHAR command[] = TEXT("cmd.exe /c more"); // Çocuk, borudan okumak için 'more' komutunu kullanacak

    if (!CreateProcess(
        NULL, command, NULL, NULL, TRUE, // TRUE: Handle'ları kalıtım yoluyla aktar (Çok Önemli!)
        0, NULL, NULL, &si, &pi
    )) {
        printf("Parent: CreateProcess failed (%lu).\n", GetLastError());
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return 1;
    }

    // 3. Ebeveyn: Borunun okuma ucunu kapat (artık gerekli değil)
    CloseHandle(hReadPipe);

    // 4. Ebeveyn: Boruya yaz
    printf("Parent: Sending message to child...\n");
    if (!WriteFile(hWritePipe, MESSAGE, strlen(MESSAGE), &dwWritten, NULL)) {
        printf("Parent: WriteFile failed.\n");
    }

    // 5. Borunun yazma ucunu kapat (veri akışını sonlandırır)
    CloseHandle(hWritePipe);

    // 6. Çocuğun bitmesini bekle
    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("Parent: IPC complete.\n");
    return 0;
}