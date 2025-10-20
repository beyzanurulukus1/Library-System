#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define MAX_USERS 100
#define MAX_KITAP 500
struct kullanici
{
    int id;
    char username[50];
    char sifre[20];
    char rol[10];
    int oduncKitaplar[3]; // En fazla 3 kitap alabilir kuralı içindir. Burada idleri girilerek ödünç verilecek. max 3 id girişi olabilir. bostakilere -1 konulabilir
};

struct kitap
{
    int id;
    char kitapAdi[50];
    char yazar[50];
    char kategori[20];
    int durum;         // Bellek tasarrufu için 0-1 girilir. 0 rafta 1 ödünç alınmış kitap olur.
    int oduncKullanan; // Buraya bellek tasarrufu açısından kullanıcıların idleri girilir.
    char date[11];     // gg/aa/yyyy şeklinde son teslim tarihi ekrana basılır.
};

int loadUsers(struct kullanici kullanicilar[]) // Kullanıcıları dosyaya yükler.
{
    FILE *f = fopen("users.txt", "r");
    if (f == NULL)
    {
        printf("users.txt dosyası açılamadı.\n");
        return 0;
    }

    int count = 0;
    while (fscanf(f, "%d %s %s %s %d %d %d",
                  &kullanicilar[count].id,
                  kullanicilar[count].username,
                  kullanicilar[count].sifre,
                  kullanicilar[count].rol,
                  &kullanicilar[count].oduncKitaplar[0],
                  &kullanicilar[count].oduncKitaplar[1],
                  &kullanicilar[count].oduncKitaplar[2]) == 7)
    {

        count++;
        if (count >= MAX_USERS)
        {
            printf("Maksimum kullanıcı sayısına ulaşıldı.\n");
            break;
        }
    }

    fclose(f);
    return count;
}

int girisYap(struct kullanici kullanicilar[], int kullaniciSayisi)
{ // Kullanıcıların giriş fonksiyonu.
    char kullaniciAdi[30];
    char kullaniciSifre[15];

    printf("kullanici adinizi giriniz:\n");
    scanf("%s", kullaniciAdi);
    printf("sifrenizi giriniz:\n");
    scanf("%s", kullaniciSifre);

    for (int i = 0; i < kullaniciSayisi; i++)
    {
        if (strcmp(kullaniciAdi, kullanicilar[i].username) == 0 &&
            strcmp(kullaniciSifre, kullanicilar[i].sifre) == 0)
        {
            printf("Giris Basarili!!");
            return i;
        }
    }
    printf("Kullanici adi veya sifre yanlıs. Lutfen tekrar deneyiniz.\n");
    return -1;
}


// Fonksiyon bildirmeleri yapılan yer.
void adminMenusu(struct kullanici kullanicilar[], int *kullaniciSayisi, struct kitap kitaplar[], int *kitapSayisi);
void kullaniciEkle(struct kullanici kullanicilar[], int *kullaniciSayisi);
void kitapEkle(struct kitap kitaplar[], int *kitapSayisi);
void kitapSil(struct kitap kitaplar[], int *kitapSayisi);
void kullaniciListesiniGoster(struct kullanici kullanicilar[], int kullaniciSayisi);
void kitapListesiniGoster(struct kitap kitaplar[], int kitapSayisi);
void kullaniciMenusu(struct kullanici *aktifKullanici,
                     struct kullanici kullanicilar[], int *kullaniciSayisi,
                     struct kitap kitaplar[], int *kitapSayisi);
void kitaplariAra(struct kitap kitaplar[], int kitapSayisi);
void kucukHarfCevir(char *str);
void kitapOduncAl(struct kullanici *aktifKullanici, struct kullanici kullanicilar[], int kullaniciSayisi, struct kitap kitaplar[], int kitapSayisi);
void kitapIadeEt(struct kullanici *aktifKullanici, struct kullanici kullanicilar[], int kullaniciSayisi, struct kitap kitaplar[], int kitapSayisi);
void kullaniciOduncKitaplariGoster(struct kullanici *aktifKullanici, struct kitap kitaplar[], int kitapSayisi);
void gecikmeleriKontrolEt(struct kitap kitaplar[], int kitapSayisi, struct kullanici kullanicilar[], int kullaniciSayisi);

int loadBooks(struct kitap kitaplar[]) // Kitapları dosyaya yükler.
{
    FILE *f = fopen("book.txt", "r");
    if (f == NULL)
    {
        printf("book.txt dosyası bulunamadı, yeni bir dosya oluşturulacak.\n");
        return 0;
    }

    int count = 0;

    // Kitap bilgilerini ayırarak okumak için tampon
    char line[256];
    char kitapAdiTmp[50], yazarTmp[50], kategoriTmp[20], dateTmp[11];

    while (fgets(line, sizeof(line), f) != NULL && count < MAX_KITAP)
    {

        line[strcspn(line, "\n")] = '\0';

        if (sscanf(line, "%d %s %s %s %d %d %s",
                   &kitaplar[count].id,
                   kitapAdiTmp,
                   yazarTmp,
                   kategoriTmp,
                   &kitaplar[count].durum,
                   &kitaplar[count].oduncKullanan,
                   dateTmp) == 7)
        {

            // Kitap adı, yazar ve kategori için _ işaretlerini boşluğa çevir
            for (int i = 0; i < strlen(kitapAdiTmp); i++)
            {
                if (kitapAdiTmp[i] == '_')
                    kitapAdiTmp[i] = ' ';
            }
            strcpy(kitaplar[count].kitapAdi, kitapAdiTmp);

            for (int i = 0; i < strlen(yazarTmp); i++)
            {
                if (yazarTmp[i] == '_')
                    yazarTmp[i] = ' ';
            }
            strcpy(kitaplar[count].yazar, yazarTmp);

            for (int i = 0; i < strlen(kategoriTmp); i++)
            {
                if (kategoriTmp[i] == '_')
                    kategoriTmp[i] = ' ';
            }
            strcpy(kitaplar[count].kategori, kategoriTmp);

            strcpy(kitaplar[count].date, dateTmp);

            count++;
        }
    }

    fclose(f);
    printf("Toplam %d kitap yüklendi.\n", count);
    return count;
}

int main()
{ // Ana fonksiyondur.
    struct kitap kitaplar[MAX_KITAP];
    int kitapSayisi = loadBooks(kitaplar);

    struct kullanici kullanicilar[MAX_USERS];
    int kullaniciSayisi = loadUsers(kullanicilar);

    int kullaniciIndex = -1;
    while (kullaniciIndex == -1)
    {
        kullaniciIndex = girisYap(kullanicilar, kullaniciSayisi);
    }
    if (strcmp(kullanicilar[kullaniciIndex].rol, "admin") == 0) // Kullanıcıyı rolüne göre menüsüne gönderirir.
    { 
        adminMenusu(kullanicilar, &kullaniciSayisi, kitaplar, &kitapSayisi);
    }
    else
    {
        kullaniciMenusu(&kullanicilar[kullaniciIndex], kullanicilar, &kullaniciSayisi, kitaplar, &kitapSayisi);
    }
}

void adminMenusu(struct kullanici kullanicilar[], int *kullaniciSayisi, struct kitap kitaplar[], int *kitapSayisi)
{ // Admin menüsü.
    int secim;
    do
    {
        printf("\n--- Admin Menüsü ---\n");
        printf("1. Yeni kullanıcı ekle\n");
        printf("2. (Kitap ekle)\n");
        printf("3. (Kitap sil)\n");
        printf("4. (Gecikmeleri kontrol et)\n");
        printf("5. (Kullanıcı listesini görüntüle)\n");
        printf("6. (Kitap Listesini görüntüle)\n");
        printf("7. Çıkış\n");
        printf("Seçiminiz: ");
        scanf("%d", &secim);

        switch (secim)
        {
        case 1:
            kullaniciEkle(kullanicilar, kullaniciSayisi);
            break;
        case 2:
            kitapEkle(kitaplar, kitapSayisi);
            break;
        case 3:
            kitapSil(kitaplar, kitapSayisi);
            break;
        case 5:
            kullaniciListesiniGoster(kullanicilar, *kullaniciSayisi);
            break;
        case 6:
            kitapListesiniGoster(kitaplar, *kitapSayisi);
            break;
        case 7:
            printf("Admin çıkış yaptı.\n");
            break;
        default:
            printf("Bu seçenek henüz aktif değil veya geçersiz.\n");
        }
    } while (secim != 7);
}

void kullaniciEkle(struct kullanici kullanicilar[], int *kullaniciSayisi)
{ // Kullanıcı ekleme fonksiyonudur
    struct kullanici yeni;
    yeni.id = *kullaniciSayisi + 1;
    printf("Kullanıcı adı girin: ");
    scanf("%s", yeni.username);
    for (int i = 0; i < *kullaniciSayisi; i++)
    {
        if (strcmp(kullanicilar[i].username, yeni.username) == 0)
        {
            printf("Bu kullanıcı adı başka kullanıcıya ait! Lütfen başka bir kullanıcı adı giriniz\n");
            return; // Ekleme işlemini iptal etme
        }
    }

    printf("Şifre girin: ");
    scanf("%s", yeni.sifre);

    printf("Rol girin (ogrenci/personel): ");
    scanf("%s", yeni.rol);
    for (int i = 0; i < 3; i++)
    {
        yeni.oduncKitaplar[i] = -1;
    }

    FILE *f = fopen("users.txt", "a"); // Kullanıcıyı 'append' modunda dosyaya ekler.
    if (f == NULL)
    {
        printf("users.txt dosyası açılamadı!\n");
        return;
    }
    fprintf(f, "%d %s %s %s %d %d %d\n",
            yeni.id,
            yeni.username,
            yeni.sifre,
            yeni.rol,
            yeni.oduncKitaplar[0],
            yeni.oduncKitaplar[1],
            yeni.oduncKitaplar[2]);
    fclose(f);
    kullanicilar[*kullaniciSayisi] = yeni;
    (*kullaniciSayisi)++;
    printf("Kullanıcı başarıyla eklendi!\n");
}

void kitapEkle(struct kitap kitaplar[], int *kitapSayisi)
{ // Kitap ekleme fonksiyonudur.
    if (*kitapSayisi >= MAX_KITAP)
    { // Kitap sayısı maximum sayı olan 500'ü geçmiş mi kontrolü.
        printf("Maksimum kitap sayısına ulaşıldı!\n");
        return;
    }

    struct kitap yeni;

    // En büyük mevcut ID'yi bulma ve sonrasında ona bir ekleyerek yeni kitaba id atamadır.
    int maxID = 0;
    for (int i = 0; i < *kitapSayisi; i++)
    {
        if (kitaplar[i].id > maxID)
            maxID = kitaplar[i].id;
    }
    yeni.id = maxID + 1;

    while (getchar() != '\n'); // Temizleme işlemi

    printf("Eklemek istediginiz kitabın ismini giriniz: ");
    fgets(yeni.kitapAdi, sizeof(yeni.kitapAdi), stdin);
    yeni.kitapAdi[strcspn(yeni.kitapAdi, "\n")] = '\0';

    printf("Eklemek istediğiniz kitabın yazarının ismini giriniz: ");
    fgets(yeni.yazar, sizeof(yeni.yazar), stdin);
    yeni.yazar[strcspn(yeni.yazar, "\n")] = '\0';

    printf("Eklemek istediğiniz kitabın hangi kategoriye ait olduğunu giriniz: ");
    fgets(yeni.kategori, sizeof(yeni.kategori), stdin);
    yeni.kategori[strcspn(yeni.kategori, "\n")] = '\0';

    yeni.durum = 0;          // 0 = rafta
    yeni.oduncKullanan = -1; // Hiç kimse ödünç almamış
    strcpy(yeni.date, "-");  // Henüz tarih yok

    // Dosyaya yazılacak geçici diziler oluşturma
    char kitapAdiTmp[50], yazarTmp[50], kategoriTmp[20];

    // Boşlukları _ ile değiştirme
    strcpy(kitapAdiTmp, yeni.kitapAdi);
    strcpy(yazarTmp, yeni.yazar);
    strcpy(kategoriTmp, yeni.kategori);

    for (int i = 0; i < strlen(kitapAdiTmp); i++)
    {
        if (kitapAdiTmp[i] == ' ')
            kitapAdiTmp[i] = '_';
    }

    for (int i = 0; i < strlen(yazarTmp); i++)
    {
        if (yazarTmp[i] == ' ')
            yazarTmp[i] = '_';
    }

    for (int i = 0; i < strlen(kategoriTmp); i++)
    {
        if (kategoriTmp[i] == ' ')
            kategoriTmp[i] = '_';
    }

    // Kitabı dosyaya ekle 'a' (append) moduyla açıldı
    FILE *f = fopen("book.txt", "a");
    if (f == NULL)
    {
        printf("book.txt açılamadı!\n");
        return;
    }
    fprintf(f, "%d %s %s %s %d %d %s\n",
            yeni.id,
            kitapAdiTmp,
            yazarTmp,
            kategoriTmp,
            yeni.durum,
            yeni.oduncKullanan,
            yeni.date);
    fclose(f);

    // Kitabı bellekteki diziye ekle
    kitaplar[*kitapSayisi] = yeni;
    (*kitapSayisi)++;

    printf("Kitap başarıyla eklendi! (ID: %d)\n", yeni.id);
}

void kitapSil(struct kitap kitaplar[], int *kitapSayisi)
{ // Kitap silme fonksiyonu
    int silinecekID;
    int bulunanIndex = -1;

    printf("Silmek istediğiniz kitabın id'sini giriniz: ");
    scanf("%d", &silinecekID);

    // Silinecek kitabı bulma
    for (int i = 0; i < *kitapSayisi; i++)
    {
        if (kitaplar[i].id == silinecekID)
        {
            bulunanIndex = i;
            break;
        }
    }

    if (bulunanIndex == -1)
    { 
        printf("Aradığınız id'ye sahip bir kitap bulunamadı!\n");
        return;
    }

    printf("Kitap bulundu!! Id=>%d-%s\n", kitaplar[bulunanIndex].id, kitaplar[bulunanIndex].kitapAdi);

    // Kitabı diziden kaldırma ve sonrasında onun alt sırasındaki kitapları bir üste kaydırma.
    for (int i = bulunanIndex; i < *kitapSayisi - 1; i++)
    {
        kitaplar[i] = kitaplar[i + 1];
    }
    (*kitapSayisi)--;

    // Silinen kitaptan sonraki tüm kitapların id'lerini bir azaltma
    for (int i = 0; i < *kitapSayisi; i++)
    {
        if (kitaplar[i].id > silinecekID)
        {
            kitaplar[i].id--;
        }
    }

    // Değişiklikleri dosyaya kaydetme
    FILE *f = fopen("book.txt", "w");
    if (f == NULL)
    {
        printf("book.txt dosyası yazmak için açılamadı!\n");
        return;
    }

    for (int i = 0; i < *kitapSayisi; i++)
    {
        // Boşlukları _ ile değiştirme
        char kitapAdiTmp[50], yazarTmp[50], kategoriTmp[20];

        strcpy(kitapAdiTmp, kitaplar[i].kitapAdi);
        strcpy(yazarTmp, kitaplar[i].yazar);
        strcpy(kategoriTmp, kitaplar[i].kategori);

        for (int j = 0; j < strlen(kitapAdiTmp); j++)
        {
            if (kitapAdiTmp[j] == ' ')
                kitapAdiTmp[j] = '_';
        }

        for (int j = 0; j < strlen(yazarTmp); j++)
        {
            if (yazarTmp[j] == ' ')
                yazarTmp[j] = '_';
        }

        for (int j = 0; j < strlen(kategoriTmp); j++)
        {
            if (kategoriTmp[j] == ' ')
                kategoriTmp[j] = '_';
        }

        fprintf(f, "%d %s %s %s %d %d %s\n",
                kitaplar[i].id,
                kitapAdiTmp,
                yazarTmp,
                kategoriTmp,
                kitaplar[i].durum,
                kitaplar[i].oduncKullanan,
                kitaplar[i].date);
    }

    fclose(f);

    printf("İstediğiniz kitap başarıyla silindi ve ID'ler güncellendi!\n");
}

void gecikmeleriKontrolEt(struct kitap kitaplar[], int kitapSayisi, struct kullanici kullanicilar[], int kullaniciSayisi)
{ // Eğer kullanıcılar kitapları teslim tarihini geciktirdiyse görüntülemeye yönlendirir.
    time_t now = time(NULL);
    struct tm *bugun = localtime(&now);

    int gecikenVar = 0;
    for (int i = 0; i < kitapSayisi; i++)
    {
        if (kitaplar[i].durum == 1 && strcmp(kitaplar[i].date, "-") != 0)
        {
            struct tm teslimTarihi = {0};
            strptime(kitaplar[i].date, "%d/%m/%Y", &teslimTarihi);
            // Tarihi time_t formatına çevir
            time_t teslim = mktime(&teslimTarihi);

            if (difftime(teslim, now) < 0)
            { // teslim < bugün ise şart sağlanır
                gecikenVar = 1;
                printf("\nGEÇ KALINMIŞ KİTAP:\n");
                printf("ID: %d\n", kitaplar[i].id);
                printf("Kitap ismi: %s\n", kitaplar[i].kitapAdi);
                printf("Yazar: %s\n", kitaplar[i].yazar);
                printf("Kategori: %s\n", kitaplar[i].kategori);
                printf("Teslim tarihi: %s\n", kitaplar[i].date);
                printf("Ödünç alan kullanıcı ID: %d\n", kitaplar[i].oduncKullanan);
                // Kullanıcı adını göster
                for (int k = 0; k < kullaniciSayisi; k++)
                {
                    if (kullanicilar[k].id == kitaplar[i].oduncKullanan)
                    {
                        printf("Kullanıcı adı: %s\n", kullanicilar[k].username);
                        break;
                    }
                }
            }
        }
    }
    if (!gecikenVar)
    { // Eğer şart sağlanmıyorsa ekrana uyarı verir.
        printf("Şu anda gecikmiş kitap bulunmamaktadır.\n");
    }
}

void kullaniciListesiniGoster(struct kullanici kullanicilar[], int kullaniciSayisi)
{ // Eklenen tüm kullanıcıları görünntüleme.
    printf("<--Kullanıcı Listesi-->\n");
    for (int i = 0; i < (kullaniciSayisi); i++)
    {
        printf("ID:%d\n", kullanicilar[i].id);
        printf("Kullanıcı Adı:%s\n", kullanicilar[i].username);
        printf("Rol:%s\n", kullanicilar[i].rol);
        printf("Odunc Aldigi Ktaplar:");
        for (int j = 0; j < 3; j++)
        {
            if (kullanicilar[i].oduncKitaplar[j] != -1)
            {
                printf("%d\n", kullanicilar[i].oduncKitaplar[j]);
            }
        }
    }
}
void kitapListesiniGoster(struct kitap kitaplar[], int kitapSayisi)
{ // Eklenen tüm kitapları ve bilgilerini en güncel haliyle ekrana yazdırma
    printf("<--Kitap Listesi-->\n");
    printf("Toplam kitap sayısı: %d\n", kitapSayisi);

    for (int i = 0; i < kitapSayisi; i++)
    {
        printf("ID:%d\n", kitaplar[i].id);
        printf("Kitap Ismi:%s\n", kitaplar[i].kitapAdi);
        printf("Yazar:%s\n", kitaplar[i].yazar);
        printf("Kategori:%s\n", kitaplar[i].kategori);
        printf("Durum:%s\n", kitaplar[i].durum == 0 ? "Rafta" : "Ödünç alınmış");
        if (kitaplar[i].durum == 1)
        {
            printf("Ödünç alan kullanıcı ID:%d\n", kitaplar[i].oduncKullanan);
            printf("Son teslim tarihi:%s\n", kitaplar[i].date);
        }
    }
}
void kullaniciMenusu(
    // Kullanıcı menüsü fonskiyonudur.
    struct kullanici *aktifKullanici,
    struct kullanici kullanicilar[], int *kullaniciSayisi,
    struct kitap kitaplar[], int *kitapSayisi)
{
    int secim;
    do
    {
        printf("\n--- Kullanıcı Menüsü ---\n");
        printf("1. Kitap ara (isim, yazar veya kategoriye göre)\n"); // <-- yeni
        printf("2. Kitap ödünç al\n");
        printf("3. Kitap iade et\n");
        printf("4. Ödünç alınan kitapları göster\n");
        printf("5. Çıkış yap\n");
        printf("Seçiminiz: ");
        scanf("%d", &secim);

        switch (secim)
        {
        case 1:
            kitaplariAra(kitaplar, *kitapSayisi);
            break;
        case 2:
            kitapOduncAl(aktifKullanici, kullanicilar, *kullaniciSayisi, kitaplar, *kitapSayisi);
            break;
        case 3:
            kitapIadeEt(aktifKullanici, kullanicilar, *kullaniciSayisi, kitaplar, *kitapSayisi);
            break;
        case 4:
            kullaniciOduncKitaplariGoster(aktifKullanici, kitaplar, *kitapSayisi);
            break;
        case 5:
            printf("Çıkış yapılmıştır!!\n");
            break;
        default:
            printf("Geçersiz seçim! Lütfen tekrar deneyin.\n");
        }
    } while (secim != 5);
}

void kucukHarfCevir(char *str)
{ // Kullanıcı kitapları ararken dosyadaki büyük-küçük harf duyarlığını ortadan kaldırır.
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void kitaplariAra(struct kitap kitaplar[], int kitapSayisi)
{ // Kullanıcı kitapları istediği şekilde arayabilir.
    int secim;
    char aranan[50];
    printf("Kitabı nasıl aramak istediğinizi seçiniz:\n");
    printf("1-->İsmine göre arama\n");
    printf("2-->Kategoriye göre arama\n");
    printf("3--> Yazara göre arama\n");
    printf("SEçiminiz:");
    scanf("%d", &secim);
    getchar();

    printf("Aramak için karakter giriniz:");
    fgets(aranan, sizeof(aranan), stdin);
    aranan[strcspn(aranan, "\n")] = '\0';
    kucukHarfCevir(aranan);

    int bulundu = 0;
    for (int i = 0; i < kitapSayisi; i++)
    {
        char tmp[50];
        if (secim == 1)
        {
            strcpy(tmp, kitaplar[i].kitapAdi);
        }
        else if (secim == 2)
        {
            strcpy(tmp, kitaplar[i].kategori);
        }
        else if (secim == 3)
        {
            strcpy(tmp, kitaplar[i].yazar);
        }
        else
        {
            printf("Gecersiz seçim");
            return;
        }
        kucukHarfCevir(tmp);

        if (strstr(tmp, aranan) != NULL) 
        { // Eğer kullanıcı aradığı şekliyle yanlış yazıdysa o yazılana yakın özellikteki kitapları da görüntüler
            bulundu = 1;
            printf("Aradığınız kitap bulundu!\n");
            printf("ID:%d\n", kitaplar[i].id);
            printf("Kitap ismi:%s\n", kitaplar[i].kitapAdi);
            printf("Yazar:%s\n", kitaplar[i].yazar);
            printf("Kategori:%s\n", kitaplar[i].kategori);
            printf("Durum:%s\n", kitaplar[i].durum == 0 ? "Rafta" : "Ödünç alınmış");
            if (kitaplar[i].durum == 1)
            {
                printf("Ödünç kullanan id:%d\n", kitaplar[i].oduncKullanan);
                printf("Son teslim tarihi:%s\n", kitaplar[i].date);
            }
        }
    }
    if (!bulundu)
    {
        printf("Aradığınız kriterde kitap bulunamadı!\n");
    }
}
void kitapOduncAl(struct kullanici *aktifKullanici, struct kullanici kullanicilar[], int kullaniciSayisi, struct kitap kitaplar[], int kitapSayisi)
{ // Kullanıcıya kitap ödünç verme sistemidir.
    int kitapId;
    printf("Ödünç almak istediğiniz kitabın id'sini giriniz:");
    scanf("%d", &kitapId);

    int kitapIndex = -1;
    for (int i = 0; i < kitapSayisi; i++)
    {
        if (kitaplar[i].id == kitapId)
        {
            kitapIndex = i;
            break;
        }
    }
    if (kitapIndex == -1)
    {
        printf("Bu id'ye sahip kitap bulunamadı.\n");
        return;
    }
    if (kitaplar[kitapIndex].durum == 1)
    {
        printf("Bu kitap zaten ödünç alınmış.\n");
        return;
    }
    int bosYer = -1;
    for (int i = 0; i < 3; i++)
    {
        if ((*aktifKullanici).oduncKitaplar[i] == -1)
        {
            bosYer = i;
            break;
        }
    }
    if (bosYer == -1)
    {
        printf("Alabileceğiniz maximum kitap sayısına ulaştınız. Önce kitap iade edin.\n");
        return;
    }
     // Kitabın son teslim tarihini hesaplama
    time_t now = time(NULL);
    now += 15 * 24 * 60 * 60;
    struct tm *teslim = localtime(&now);
    strftime(kitaplar[kitapIndex].date, sizeof(kitaplar[kitapIndex].date), "%d/%m/%Y", teslim);

    kitaplar[kitapIndex].durum = 1;
    kitaplar[kitapIndex].oduncKullanan = (*aktifKullanici).id;
    (*aktifKullanici).oduncKitaplar[bosYer] = kitapId;
    printf("Kitap başarıyla ödünç alındı! Teslim tarihi: %s\n", kitaplar[kitapIndex].date);

    // Kitapları dosyaya yazma
    FILE *fkitapFile = fopen("book.txt", "w");
    if (fkitapFile == NULL)
    {
        printf("book.txt dosyası açılamadı!\n");
    }
    else
    {
        for (int i = 0; i < kitapSayisi; i++)
        {
            // Boşlukları _ ile değiştirerek yazma
            char kitapAdiTmp[50], yazarTmp[50], kategoriTmp[20];
            strcpy(kitapAdiTmp, kitaplar[i].kitapAdi);
            strcpy(yazarTmp, kitaplar[i].yazar);
            strcpy(kategoriTmp, kitaplar[i].kategori);

            for (int j = 0; j < strlen(kitapAdiTmp); j++)
            {
                if (kitapAdiTmp[j] == ' ')
                    kitapAdiTmp[j] = '_';
            }
            for (int j = 0; j < strlen(yazarTmp); j++)
            {
                if (yazarTmp[j] == ' ')
                    yazarTmp[j] = '_';
            }
            for (int j = 0; j < strlen(kategoriTmp); j++)
            {
                if (kategoriTmp[j] == ' ')
                    kategoriTmp[j] = '_';
            }

            fprintf(fkitapFile, "%d %s %s %s %d %d %s\n",
                    kitaplar[i].id,
                    kitapAdiTmp,
                    yazarTmp,
                    kategoriTmp,
                    kitaplar[i].durum,
                    kitaplar[i].oduncKullanan,
                    kitaplar[i].date);
        }
        fclose(fkitapFile);
    }

    // Kullanıcı dosyasını güncelleme
    FILE *fkullaniciFile = fopen("users.txt", "w");
    if (fkullaniciFile == NULL)
    {
        printf("users.txt dosyası açılamadı!\n");
    }
    else
    {
        for (int i = 0; i < kullaniciSayisi; i++)
        {
            fprintf(fkullaniciFile, "%d %s %s %s %d %d %d\n",
                    kullanicilar[i].id,
                    kullanicilar[i].username,
                    kullanicilar[i].sifre,
                    kullanicilar[i].rol,
                    kullanicilar[i].oduncKitaplar[0],
                    kullanicilar[i].oduncKitaplar[1],
                    kullanicilar[i].oduncKitaplar[2]);
        }
        fclose(fkullaniciFile);
    }
}
void kitapIadeEt(struct kullanici *aktifKullanici, struct kullanici kullanicilar[], int kullaniciSayisi, struct kitap kitaplar[], int kitapSayisi)
{ // Kullancının ödünç aldığı kitabı iade etme fonksiyonu
    int kitapId;
    printf("İade etmek istediğiniz kitabın id'sini giriniz:");
    scanf("%d", &kitapId);

    int kitapIndex = -1;
    for (int i = 0; i < kitapSayisi; i++)
    {
        if (kitaplar[i].id == kitapId)
        {
            kitapIndex = i;
            break;
        }
    }
    if (kitapIndex == -1)
    {
        printf("Aradığınız id'ye sahip kitap bulunamadı.\n");
        return;
    }
    for (int i = 0; i < 3; i++)
    {
        if ((*aktifKullanici).oduncKitaplar[i] == kitapId)
        {
            (*aktifKullanici).oduncKitaplar[i] = -1;
            break;
        }
    }

    kitaplar[kitapIndex].durum = 0; // Durumunu ödünç hal olan '1' den raf hali olan '0'a çevirme
    kitaplar[kitapIndex].oduncKullanan = -1; // Ödünç alan kullanıcının id'si yerine -1 koyma
    strcpy(kitaplar[kitapIndex].date, "-"); // Son teslim tarihi yerine '-' koyma

    printf("Kitap başarıyla iade edildi!\n");

    // Kitap dosyasını güncelleme
    FILE *fkitap = fopen("book.txt", "w");
    if (fkitap == NULL)
    {
        printf("book.txt dosyası açılamadı!\n");
    }
    else
    {
        char kitapAdiTmp[50], yazarTmp[50], kategoriTmp[20];
        for (int i = 0; i < kitapSayisi; i++)
        {
            strcpy(kitapAdiTmp, kitaplar[i].kitapAdi);
            strcpy(yazarTmp, kitaplar[i].yazar);
            strcpy(kategoriTmp, kitaplar[i].kategori);

            for (int j = 0; j < strlen(kitapAdiTmp); j++)
            {
                if (kitapAdiTmp[j] == ' ')
                    kitapAdiTmp[j] = '_';
            }
            for (int j = 0; j < strlen(yazarTmp); j++)
            {
                if (yazarTmp[j] == ' ')
                    yazarTmp[j] = '_';
            }
            for (int j = 0; j < strlen(kategoriTmp); j++)
            {
                if (kategoriTmp[j] == ' ')
                    kategoriTmp[j] = '_';
            }

            fprintf(fkitap, "%d %s %s %s %d %d %s\n",
                    kitaplar[i].id,
                    kitapAdiTmp,
                    yazarTmp,
                    kategoriTmp,
                    kitaplar[i].durum,
                    kitaplar[i].oduncKullanan,
                    kitaplar[i].date);
        }
        fclose(fkitap);
    }

    // Kullanıcı dosyasını güncelleme
    FILE *fkullanici = fopen("users.txt", "w");
    if (fkullanici == NULL)
    {
        printf("users.txt dosyası açılamadı!\n");
    }
    else
    {
        for (int i = 0; i < kullaniciSayisi; i++)
        {
            fprintf(fkullanici, "%d %s %s %s %d %d %d\n",
                    kullanicilar[i].id,
                    kullanicilar[i].username,
                    kullanicilar[i].sifre,
                    kullanicilar[i].rol,
                    kullanicilar[i].oduncKitaplar[0],
                    kullanicilar[i].oduncKitaplar[1],
                    kullanicilar[i].oduncKitaplar[2]);
        }
        fclose(fkullanici);
    }
}
void kullaniciOduncKitaplariGoster(struct kullanici *aktifKullanici, struct kitap kitaplar[], int kitapSayisi)
{ // Kullanıcı ödünç aldığı kitapları görüntüleme bölümü
    int kitapSayac = 0;
    printf("<--Ödünç alınan Kitaplar-->\n");
    for (int i = 0; i < 3; i++)
    {
        int oduncId = (*aktifKullanici).oduncKitaplar[i];
        if (oduncId != -1)
        {
            for (int j = 0; j < kitapSayisi; j++)
            {
                if (kitaplar[j].id == oduncId)
                {
                    kitapSayac++;
                    printf("\n%d. Kitap\n", kitapSayac);
                    printf("ID:%d\n", kitaplar[j].id);
                    printf("Kitap ismi:%s\n", kitaplar[j].kitapAdi);
                    printf("Yazar:%s\n", kitaplar[j].yazar);
                    printf("Kategori:%s\n", kitaplar[j].kategori);
                    printf("Son teslim tarihi:%s\n", kitaplar[j].date);
                }
            }
        }
    }
    if (kitapSayac == 0)
    {
        printf("Şuanda ödünç aldığınız kitap bulunmamaktadır");
    }
}