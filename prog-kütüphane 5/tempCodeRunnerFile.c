 time_t now=time(NULL);
    now+=15*24*60*60;

    struct tm *teslim=localtime(&now);
    strftime(kitaplar[kitapIndex].date, sizeof(kitaplar[kitapIndex].date), "%d/%m/%Y", teslim);

    kitaplar[kitapIndex].durum=1;
    kitaplar[kitapIndex].oduncKullanan=(*aktifKullanici).id;
    (*aktifKullanici).oduncKitaplar[bosYer]=kitapId;
    printf("Kitap başarıyla ödünç alındı! Teslim tarihi: %s\n", kitaplar[kitapIndex].date);
