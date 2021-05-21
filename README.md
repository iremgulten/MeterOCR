# MeterOCR
OCR Tabanlı Sayac Okuma Sistemi <br>
Proje kapsamında C++ ve OpenCV kullanılmıştır. <br> <br>

Projenin gerçekleştirimi Visual Studio 2019 IDE'sinde yapılmış, programlama dili olarak C++
kullanılmış ve görüntü işleme kısımları için OpenCV kütüphanesi projeye dahil edilmiştir.
Projede sayacın rakamlar olduğu bölümünü image olarak alıp rakamlarını tespit ederek sayının okunması
gerçekleştirilmektedir. <br> Sayaçtaki karakter sayısı projenin başında define olarak tanımlanmıştır.
Kullanıcıdan öncelikle dosya yolu istenmektedir. Ardından okunacak sayacın tür bilgisi (dijital 
veya analog) istenmektedir. Bu bilgiler doğrultusunda image'nin bütününe filtreleme işlemleri
uygulanmış ardından dışta olabilecek mesafelerin en aza indirgenmesi için image'ye dıştan kırpma işlemi 
yapılmıştır. Genel kırpma işleminin ardından kullanıcıdan alınan karakter sayısı ele alınarak image'nin 
karakter sayısı kadar bölünerek sadece rakamların tutulması sağlanmıştır. Ardından tutulacak image'lerde
sadece rakamın kalması için sağdan, soldan, aşağıdan ve yukarıdan siyah alanları kırpma işlemi 
gerçekleştirilmiştir. Ardından bir dizide tutulan bu rakamlar sayacın dijital veya analog olmasına göre 
farklı işlemlere tabii tutulmuştur. Dijital sayaçlardan okunan rakamlar öncelikle yatayda 9 ve dikeyde 9 olmak
üzere toplam 81 parçaya bölünmüş ve bu parçalardan kendim belirlediğim parçaların siyah veya beyaz olmasına 
göre, oluşturduğum rakamların prototipi ile karşılaştırma yapılıp ona göre rakamlar tespit edilmiştir.
Analog sayaçlardan okuma gerçekleştirilirken rakamların her biri 3 parçaya bölünmektedir. Bu parça 
image'lerin tutulduğu matrisin her bir satırına bakılarak o satırdaki renk belirlenip arrayde 
tutulmaktadır. Daha sonra renk geçişi olan satırlardan yola çıkılarak rakamın o parçasının renk 
sıralaması tutulmuştur. Ardından her bir rakamın bu renk sıralamaları bir matriste tutulmuş ve yine 
rakamların prototipleri ile karşılaştırılıp, rakamların tespiti gerçekleştirilmiştir. Rakamların 
tespiti sırasında sayısı tespit edilemeyen sayılar için birden fazla filtreleme işlemleri sırayla 
gerçekleştirilmiştir. En son tespit edilen sayı konsol ekranında gösterilmiştir. 
Analog sayaçlar için arkası kırmızı olan rakamda görüntü işleme sırasında piksel bozuklukları
olduğu için tespiti mümkün olmamıştır ve tespit edilemeyen rakamlar yerine default olarak 0 yazılmıştır.
