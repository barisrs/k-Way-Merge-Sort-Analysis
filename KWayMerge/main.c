#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Rastgele dizi oluşturma fonksiyonu
void diziOlustur(int* arr, int N) {
    int i, j, temp;
    // Diziyi 1'den N'ye kadar sıralı olarak doldur
    for (i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    // Diziyi rastgele karıştırma işlemi 
    for (i = N-1; i > 0; i--) {
        j = rand() % (i + 1);  // 0 ile i arasında rastgele bir indeks
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// k-way merge sort fonksiyonu
void kWayMergeSort(int* arr, int* temp, int left, int right, int k) {
    if (left >= right) return;  // Eğer sadece bir eleman kaldıysa, sıralama yapmaya gerek yok
    int i, j, parcaBoyutu0;
    int size = right - left + 1;
    int parcaBoyutu = size / k;
    int kalanEleman = size % k;
    int baslangiclar[k + 1];

    // Parçaların başlangıç ve bitiş indekslerini hesapla
    int start = left;
    for (i = 0; i < k; i++) {
        baslangiclar[i] = start;
        parcaBoyutu0 = parcaBoyutu + (i < kalanEleman ? 1 : 0);
        start += parcaBoyutu0;
    }
    baslangiclar[k] = right + 1;

    // Her parçayı kendi içinde sıralamak için recursive fonksiyon
    for (int i = 0; i < k; i++) {
        if (baslangiclar[i] < baslangiclar[i + 1]) {
            kWayMergeSort(arr, temp, baslangiclar[i], baslangiclar[i + 1] - 1, k);
        }
    }

    // Kıyaslama ve sıralama işlemi (merge adımı)
    int indeksler[k];  // Her parçanın güncel indeksini tutar
    int aktifParcaSayisi = k;  // Hangi parçaların hâlâ eleman içerdiğini kontrol etmek için

    for (i = 0; i < k; i++) {
        indeksler[i] = baslangiclar[i];
    }

    int idx = left;  // Geçici diziye kopyalama için
    while (idx <= right && aktifParcaSayisi > 0) {
        int minIndex = -1;
        
        // Minimum elemanı bulmak için tüm parçaları kontrol et
        for (i = 0; i < k; i++) {
            if (indeksler[i] < baslangiclar[i + 1]) {  // Parça tükenmediyse
                if (minIndex == -1 || arr[indeksler[i]] < arr[indeksler[minIndex]]) {
                    minIndex = i;
                }
            }
        }

        // Eğer tüm parçalar tükenmişse aktifParcaSayisi'ni sıfıra ayarla
        if (minIndex == -1) {
            aktifParcaSayisi = 0;
        } else {
            temp[idx++] = arr[indeksler[minIndex]];
            indeksler[minIndex]++;
            
            // Eğer bir parça tükenmişse aktifParcaSayisi'ni azalt
            if (indeksler[minIndex] >= baslangiclar[minIndex + 1]) {
                aktifParcaSayisi--;
            }
        }
    }

    // Geçici diziyi orijinal diziye kopyala
    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

int main() {
    int N, k, i, j;

    // Kullanıcıdan dizinin boyutu alınır
    printf("Dizinin eleman sayısını (N) girin: ");
    scanf("%d", &N);

    if (N < 1) {  // Eğer N geçerli değilse
        printf("Geçersiz N değeri! N pozitif bir sayı olmalıdır.\n");
        return -1;
    }

    srand(time(NULL));

    // Bellek oluşturma
    int* arr = malloc(N * sizeof(int));  // Dizi için bellek
    int* temp = malloc(N * sizeof(int)); // Geçici dizi için bellek
    if (!arr || !temp) {
        printf("Bellek Hatası.\n");
        return -1;
    }

    // Rastgele diziyi oluştur
    diziOlustur(arr, N);

    // Orijinal diziyi gormek için yorum satırından çıkarabilirsiniz.
    /*
    printf("Orijinal dizi:\n");
    for (i = 0; i < N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    */

    // k için 2'den 10'a kadar değerler üzerinde sıralama işlemi
    for (k = 2; k <= 10; k++) {
        // Orijinal diziyi kopyalama
        int* arr_copy = (int*)malloc(N * sizeof(int));
        if (!arr_copy) {
            printf("Bellek Hatası.\n");
            return -1;
        }
        for (i = 0; i < N; i++) {
            arr_copy[i] = arr[i];
        }

        // Zaman ölçümüne başla
        clock_t basla = clock();

        // k-way merge sort işlemi
        kWayMergeSort(arr_copy, temp, 0, N - 1, k);

        // Zamanı bitir
        clock_t bitis = clock();

        // Zaman hesaplama
        double sure = ((double)(bitis - basla)) / (CLOCKS_PER_SEC / 1000);

        printf("k = %d'nin işlem süresi: %.3f milisaniye\n", k, sure);

        // Sıralanmış diziyi Gormek isterseniz,yorum satırından çıkabilirsiniz.
        /*
        printf("Sıralanmış dizi (k = %d):\n", k);
        for (i = 0; i < N; i++) {
            printf("%d ", arr_copy[i]);
        }
        printf("\n");
        */

        // Belleği serbest bırak
        free(arr_copy);
    }

    // Belleği serbest bırak
    free(arr);
    free(temp);

    return 0;
}
