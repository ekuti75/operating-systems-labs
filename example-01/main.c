#include <unistd.h> 

int main() {
    // write(dosya_tanıtıcısı, veri_arabelleği, veri_uzunluğu)
    // 1, standart çıktı (terminal) anlamına gelir.
    const char *message = "Hello Operating Systems Lab!\n";
    write(1, message, 29); 
    return 0;
}