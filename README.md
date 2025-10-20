# 📚 Kütüphane Yönetim Sistemi – C Programlama Projesi

Bu proje, **C programlama dili** kullanılarak geliştirilen dosya tabanlı bir **kütüphane yönetim sistemidir.**  
Kullanıcı girişi, kitap listeleme, kitap ödünç alma/iade etme ve rol bazlı yetkilendirme (admin, personel, öğrenci) gibi temel işlevleri destekler.

> Dosya işlemleri, kullanıcı oturumu yönetimi ve rol bazlı yetkilendirme mantığı ile gerçek dünya uygulamalarına yönelik pratik bir örnektir.

---

## 🎯 Projenin Amacı

- C dilinde **dosya işlemleri** (file I/O) pratiği yapmak  
- Kullanıcı rolleri ile **yetki kontrolü** sağlamak  
- Terminal tabanlı gerçekçi bir sistem tasarımı gerçekleştirmek  
- Veritabanı kullanılmadan **kalıcı veri yönetimi** gerçekleştirmek  
- Yazılım mühendisliği portföyü için işlevsel bir uygulama geliştirmek

---

## 🛠️ Kullanılan Teknolojiler

| Teknoloji | Açıklama |
|-----------|----------|
| Dil       | C (ANSI C Standardı) |
| Platform  | macOS / Linux / Windows (GCC uyumlu) |
| Arayüz    | Komut satırı (CLI) |
| Veritabanı| `users.txt` ve `book.txt` üzerinden metin tabanlı saklama |

---

## 📁 Proje Yapısı

📦 kutuphane-yonetim
├── main.c               # Ana kaynak kodu
├── users.txt            # Kullanıcı verileri
├── book.txt             # Kitap verileri
└── README.md            # Proje dokümantasyonu

---

## 👤 Kullanıcı Rolleri

| Rol        | Yetkiler |
|------------|----------|
| `admin`    | Tüm kullanıcıları ve kitapları görüntüleyebilir, yeni kitap ekleyebilir |
| `personel` | Kitap ödünç alabilir ve iade edebilir |
| `ogrenci`  | Kitap ödünç alabilir ve iade edebilir |

---

## 🗃️ Veri Formatı

### 🔸 `users.txt` (Kullanıcılar)

txt
ID KullanıcıAdı Şifre Rol KitapID1 KitapID2 KitapID3
2 beyza 12345 ogrenci 3 3 -1

🔸 book.txt (Kitaplar)
ID KitapAdı Yazar Tür AlındıMı AlanKullanıcıID TeslimTarihi
3 suç_ve_ceza dostoyevski roman 1 5 08/06/2025


🚀 Derleme ve Çalıştırma

GCC (Linux/macOS):
gcc main.c -o kutuphane
./kutuphane

Windows (MinGW):
gcc main.c -o kutuphane.exe
kutuphane.exe


✅ Uygulama Özellikleri
	•	Giriş sistemi ve kullanıcı doğrulama
	•	Rol bazlı yetkilendirme
	•	Kitap listeleme
	•	Kitap ödünç alma ve iade etme
	•	Dosya bazlı veri kaydı
	•	Modüler ve geliştirilebilir kaynak kodu


  🧠 Kazanımlar
	•	C dilinde fopen, fscanf, fprintf, feof gibi temel fonksiyonların etkin kullanımı
	•	Gerçek dünya problemlerinin terminal tabanlı çözümleri
	•	Kullanıcı oturumu ve sistem yönetimi simülasyonu
	•	Yapılandırılmış, anlaşılır C kodu geliştirme pratiği

