# uPIC46

uPIC46, **PIC18F46K22-I/P** tabanlı, tamamen THT (Through-Hole) komponentlerden oluşan açık kaynak bir geliştirme kartıdır. 

## Özellikler
- PIC18F46K22-I/P (40 pin DIP) desteği
- ICSP programlama header
- Kristal osilatör desteği (16 MHz)
- Tüm I/O pinleri header’lara çıkarılmıştır
- UART, I²C, SPI için ayrı header’lar
- Güç LED’i, kullanıcı LED’leri ve butonlar
- 5V/3.3V regülatör seçenekleri
- THT komponentler ile kolay montaj

## Klasör Yapısı
- `hardware/` → KiCad şematik, PCB dosyaları, gerberler
- `firmware/` → Örnek XC8 projeleri (Blink, UART Hello)
- `docs/images/` → Kart görselleri, render ve fotoğraflar
- `docs/pcb_spec/` → PCB özellikleri, üretim dosyaları

## Lisans
Bu proje **MIT Lisansı** altında yayınlanmıştır.
