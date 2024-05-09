#include <stdio.h>
#include <string.h>

struct sirket {
    char ad[20];
    char soyad[20];
    char otobus[2];
    int koltukNo;
};

int YerKontrol(char otobus, int hedefKoltuk);
void Biletiptal(const char *isim);
void otobusDurumu(const char *sirketAdi);

int main() {
    char kullanici[] = "Mahmut";
    int kullanicisifresi = 1234;
    char kullanicigirisi[50];
    int sifregirisi;
    int secenek, devam = 1;

    printf("kullanici: ");
    scanf("%s", kullanicigirisi);

    printf("sifre: ");
    scanf("%d", &sifregirisi);

    if (strcmp(kullanici, kullanicigirisi) == 0 && kullanicisifresi == sifregirisi) {
        printf("--------------------Otobus Rezervasyon Sistemine Hos Geldiniz--------------------\n");
        while (devam) {
            printf("1-Bilet satin almak icin 1'e basiniz\n");
            printf("2-Bileti iptal etmek icin 2'ye basiniz\n");
            printf("3-Otobus ve koltuk durumunu kontrol etmek icin 3'e basiniz\n");
            printf("4-Cikis\n");
            scanf("%d", &secenek);

            if (secenek == 1) {
                struct sirket bir;

                printf("Ad: ");
                scanf("%s", bir.ad);
                printf("Soyad: ");
                scanf("%s", bir.soyad);
                printf("Otobus: ");
                scanf("%s", bir.otobus);
                printf("Koltuk No: ");
                scanf("%d", &bir.koltukNo);

                int kontrol = YerKontrol(bir.otobus, bir.koltukNo);
                if (kontrol == 1) {
                    FILE *biletliste = fopen("BiletListe.txt", "a");

                    fprintf(biletliste, "%s %s %s %d\n", bir.ad, bir.soyad, bir.otobus, bir.koltukNo);

                    fclose(biletliste);
                } else {
                    printf("Bu koltuk dolu, Tekrar deneyin.\n");
                }
            }

            if (secenek == 2) {
                char ad1[20];

                printf("Adiniz giriniz: ");
                scanf("%s", ad1);

                Biletiptal(ad1);
            }

            if (secenek == 3) {
                    char otoad[2];
                printf("Otobus sirketinin adini giriniz: ");
                scanf("%s", otoad);

                otobusDurumu(otoad);
            }

            if (secenek == 4) {
                devam = 0; 
            }
        }
    } else {
        printf("Girilen bilgiler yanlis. Lutfen tekrar deneyiniz\n");
    }

    return 0;
}

int YerKontrol(char otobus, int hedefKoltuk) {
    FILE *dosya = fopen("ABC.txt", "r");
    FILE *geciciDosya = fopen("gecici.txt", "w");

    if (dosya == NULL || geciciDosya == NULL) {
        printf("Dosya acilamadi!\n");
        return -1;
    }

    char sirket[2]; // "A", "B", "C"
    int koltukNo, dolu, flag = 0;

    // Dosyadan okuma ve yazma işlemi
    while (fscanf(dosya, "%s %d %d", sirket, &koltukNo, &dolu) == 3) {
        if (koltukNo == hedefKoltuk && dolu == 0) {
            fprintf(geciciDosya, "%s %d %d\n", sirket, koltukNo, 1);
            flag = 1;
        } else {
            fprintf(geciciDosya, "%s %d %d\n", sirket, koltukNo, dolu);
        }
    }

    fclose(dosya);
    fclose(geciciDosya);

    remove("ABC.txt"); // Eski dosyayı silme
    rename("gecici.txt", "ABC.txt"); // Yeni dosyanın adını değiştirme

    if (flag == 1)
        return 1;
    else
        return 0;
}

void Biletiptal(const char *isim) {
    FILE *dosya = fopen("BiletListe.txt", "r");
    FILE *geciciDosya = fopen("gecici1.txt", "w");

    struct sirket bilet;
    int koltuk1 = 0;
    char sirket1[2];

    while (fscanf(dosya, "%s %s %s %d", bilet.ad, bilet.soyad, bilet.otobus, &bilet.koltukNo) == 4) {
        if (strcmp(isim, bilet.ad) != 0) {
            fprintf(geciciDosya, "%s %s %s %d\n", bilet.ad, bilet.soyad, bilet.otobus, bilet.koltukNo);
        } else {
            koltuk1 = bilet.koltukNo;
            strcpy(sirket1, bilet.otobus);
        }
    }

    fclose(dosya);
    fclose(geciciDosya);

    remove("BiletListe.txt"); 
    rename("gecici1.txt", "BiletListe.txt"); 

    FILE *dosya1 = fopen("ABC.txt", "r");
    FILE *geciciDosya1 = fopen("gecici.txt", "w");

    char sirket[2]; // "A", "B", "C"
    int koltukNo, dolu;

    while (fscanf(dosya1, "%s %d %d", sirket, &koltukNo, &dolu) == 3) {
        if (strcmp(sirket1, sirket) == 0 && koltuk1 == koltukNo) {
            fprintf(geciciDosya1, "%s %d %d\n", sirket, koltukNo, 0);
        } else {
            fprintf(geciciDosya1, "%s %d %d\n", sirket, koltukNo, dolu);
        }
    }

    fclose(dosya1);
    fclose(geciciDosya1);

    remove("ABC.txt");
    rename("gecici.txt", "ABC.txt");

    printf("%s kullanicinin bileti iptal edilmistir\n", isim);
}

void otobusDurumu(const char *sirketAdi){
    FILE *dosya = fopen("ABC.txt", "r");

    int sayac = 0;
    char sirket[2]; // "A", "B", "C"
    int koltukNo, dolu;

    while(fscanf(dosya, "%s %d %d\n", sirket, &koltukNo, &dolu) == 3){
        if(strcmp(sirketAdi, sirket) == 0 && dolu == 1)
            sayac++;
    }

    fclose(dosya);

    if(strcmp(sirketAdi, "A") == 0 ){
        printf("A otobus ankara-istanbul istikametinde saat 14.00'da %d kisiyle yolculuk gerceklesicektir.\n",sayac);
        printf("%d kisilik bos koltuk mevcuttur\n",20-sayac);
    }

    if(strcmp(sirketAdi, "B") == 0 ){
        printf("B otobus Ankara-izmir istikametinde saat 14.00'da %d kisiyle yolculuk gerceklesicektir.\n",sayac);
        printf("%d kisilik bos koltuk mevcuttur\n",20-sayac);
    }

    if(strcmp(sirketAdi, "C") == 0 ){
        printf("C otobus Kahramanmaras-Bursa istikametinde saat 14.00'da %d kisiyle yolculuk gerceklesicektir.\n",sayac);
        printf("%d kisilik bos koltuk mevcuttur\n",20-sayac);
    }
}
