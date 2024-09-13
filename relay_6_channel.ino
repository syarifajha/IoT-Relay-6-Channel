// Library
#include <ESP8266WiFi.h>       // Mengimport Library ESP8266WiFi
#include <FirebaseESP8266.h>  // Mengimport Library Firebase ESP8266

// Mendefinisikan SSID dan kata sandi Wi-Fi
const char* ssid = "KHUSUS GAME";  // Nama jaringan Wi-Fi (SSID)
const char* password = "Simbadda#22";  // Kata sandi jaringan Wi-Fi

// Konfigurasi Firebase
const char* firebaseHost = " ";   // Host Firebase
const char* firebaseAuth = " ";                 // Auth Firebase

// Membuat server Wi-Fi pada port 80
WiFiServer server(80);  // Membuat server Wi-Fi pada port 80

// Membuat variabel header
String header;

// Mendefinisikan variabel dan status output
String output1State = "off";  // Status output 1 (relay 1)
String output2State = "off";  // Status output 2 (relay 2)
String output3State = "off";  // Status output 3 (relay 3)
String output4State = "off";  // Status output 4 (relay 4)
String output5State = "off";  // Status output 5 (relay 5)
String output6State = "off";  // Status output 6 (relay 6)

const int relay1 = D0;  // Pin GPIO untuk relay 1
const int relay2 = D1;  // Pin GPIO untuk relay 2
const int relay3 = D2;  // Pin GPIO untuk relay 3
const int relay4 = D3;  // Pin GPIO untuk relay 4
const int relay5 = D4;  // Pin GPIO untuk relay 5
const int relay6 = D5;  // Pin GPIO untuk relay 6


unsigned long currentTime = millis();  // Waktu saat ini dalam milidetik
unsigned long previousTime = 0;  // Waktu sebelumnya dalam milidetik

const long timeoutTime = 2000;  // Waktu batas (dalam milidetik) untuk timeout

FirebaseData firebaseData;      // Objek FirebaseData untuk koneksi ke Firebase

void setup() {
  Serial.begin(115200);  // Memulai komunikasi serial pada baud rate 115200

  pinMode(relay1, OUTPUT);  // Mengatur pin relay 1 sebagai output
  pinMode(relay2, OUTPUT);  // Mengatur pin relay 2 sebagai output
  pinMode(relay3, OUTPUT);  // Mengatur pin relay 3 sebagai output
  pinMode(relay4, OUTPUT);  // Mengatur pin relay 4 sebagai output
  pinMode(relay5, OUTPUT);  // Mengatur pin relay 5 sebagai output
  pinMode(relay6, OUTPUT);  // Mengatur pin relay 6 sebagai output

  digitalWrite(relay1, HIGH);  // Mematikan relay 1 (output 1)
  digitalWrite(relay2, HIGH);  // Mematikan relay 2 (output 2)
  digitalWrite(relay3, HIGH);  // Mematikan relay 3 (output 3)
  digitalWrite(relay4, HIGH);  // Mematikan relay 4 (output 4)
  digitalWrite(relay5, HIGH);  // Mematikan relay 5 (output 5)
  digitalWrite(relay6, HIGH);  // Mematikan relay 6 (output 6)


  Serial.print("Connecting to ");
  Serial.println(ssid); // Menampilkan SSID (nama jaringan) yang akan dihubungkan

  WiFi.begin(ssid, password); // Memulai proses koneksi ke jaringan WiFi dengan menggunakan SSID dan password

  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // Menunggu selama 0.5 detik
    Serial.print("."); // Menampilkan tanda titik untuk menunjukkan bahwa koneksi belum terhubung
  }

  Serial.println(""); // Mencetak baris kosong
  Serial.println("WiFi connected."); // Menampilkan pesan bahwa koneksi WiFi berhasil terhubung
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Menampilkan alamat IP yang diberikan oleh router WiFi
  server.begin(); // Memulai server web

  // Menginisialisasi koneksi ke Firebase
  Firebase.begin(firebaseHost, firebaseAuth);
}

void loop() {
  WiFiClient client = server.available(); // Mengecek apakah ada klien yang terhubung ke server

  if (client) { // Jika ada klien yang terhubung
    Serial.println("New Client."); // Menampilkan pesan bahwa klien baru terhubung
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime; // Menginisialisasi waktu saat ini dan waktu sebelumnya sebagai waktu sekarang
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // Selama klien terhubung dan waktu saat ini dikurangi waktu sebelumnya tidak melebihi batas waktu tertentu
      currentTime = millis(); // Mendapatkan waktu saat ini
      if (client.available()) { // Jika ada data yang tersedia dari klien
        char c = client.read(); // Membaca karakter yang dikirim oleh klien
        Serial.write(c); // Menampilkan karakter yang diterima ke Serial Monitor
        header += c; // Menambahkan karakter ke dalam header
        if (c == '\n') { // Jika karakter yang diterima adalah baris baru
          // Proses analisis header HTTP dan mengirimkan respons
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK"); // Mengirimkan status OK ke klien
            client.println("Content-type:text/html"); // Menentukan tipe konten sebagai teks HTML
            client.println("Connection: close"); // Menutup koneksi setelah respons dikirim
            client.println(); // Mencetak baris kosong sebagai pemisah antara header dan konten

            // Mengatur status relay berdasarkan permintaan GET
            if (header.indexOf("GET /16/on") >= 0) {
              // Jika permintaan GET mengandung "GET /16/on"
              Serial.println("GPIO 16 on");
              output1State = "on";
              digitalWrite(relay1, LOW);
            } else if (header.indexOf("GET /16/off") >= 0) {
              // Jika permintaan GET mengandung "GET /16/off"
              Serial.println("GPIO 16 off");
              output1State = "off";
              digitalWrite(relay1, HIGH);
            } else if (header.indexOf("GET /5/on") >= 0) {
              // Jika permintaan GET mengandung "GET /5/on"
              Serial.println("GPIO 5 on");
              output2State = "on";
              digitalWrite(relay2, LOW);
            } else if (header.indexOf("GET /5/off") >= 0) {
              // Jika permintaan GET mengandung "GET /5/off"
              Serial.println("GPIO 5 off");
              output2State = "off";
              digitalWrite(relay2, HIGH);
            } else if (header.indexOf("GET /4/on") >= 0) {
              // Jika permintaan GET mengandung "GET /4/on"
              Serial.println("GPIO 4 on");
              output3State = "on";
              digitalWrite(relay3, LOW);
            } else if (header.indexOf("GET /4/off") >= 0) {
              // Jika permintaan GET mengandung "GET /4/off"
              Serial.println("GPIO 4 off");
              output3State = "off";
              digitalWrite(relay3, HIGH);
            } else if (header.indexOf("GET /0/on") >= 0) {
              // Jika permintaan GET mengandung "GET /0/on"
              Serial.println("GPIO 0 on");
              output4State = "on";
              digitalWrite(relay4, LOW);
            } else if (header.indexOf("GET /0/off") >= 0) {
              // Jika permintaan GET mengandung "GET /0/off"
              Serial.println("GPIO 0 off");
              output4State = "off";
              digitalWrite(relay4, HIGH);
            } else if (header.indexOf("GET /2/on") >= 0) {
              // Jika permintaan GET mengandung "GET /2/on"
              Serial.println("GPIO 2 on");
              output5State = "on";
              digitalWrite(relay5, LOW);
            } else if (header.indexOf("GET /2/off") >= 0) {
              // Jika permintaan GET mengandung "GET /2/off"
              Serial.println("GPIO 2 off");
              output5State = "off";
              digitalWrite(relay5, HIGH);
            } else if (header.indexOf("GET /14/on") >= 0) {
              // Jika permintaan GET mengandung "GET /14/on"
              Serial.println("GPIO 14 on");
              output6State = "on";
              digitalWrite(relay6, LOW);
            } else if (header.indexOf("GET /14/off") >= 0) {
              // Jika permintaan GET mengandung "GET /14/off"
              Serial.println("GPIO 14 off");
              output6State = "off";
              digitalWrite(relay6, HIGH);
            }
            
            // Tampilkan halaman Web HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            // CSS untuk mengatur tampilan tombol on/off
            // Warna latar belakang dan ukuran font sesuai dengan preferensi Anda.
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: blue; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Header halaman web
            client.println("<body><h1>Simulasi ESP8266 Web Server Relay</h1>");
            
            // Menampilkan status dan tombol ON/OFF untuk Relay 1
            client.println("<p>Relay 1 " + output1State + "</p>");
            // Jika output1State mati, tampilkan tombol ON
            if (output1State == "off") {
              client.println("<p><a href=\"/16/on\"><button class=\"button\">Nyala</button></a></p>");
            } else {
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">Mati</button></a></p>");
              }
              
            // Menampilkan status dan tombol ON/OFF untuk Relay 2
            client.println("<p>Relay 2 " + output2State + "</p>");
            // Jika output2State mati, tampilkan tombol ON
            if (output2State == "off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">Nyala</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">Mati</button></a></p>");
              }
              
            // Menampilkan status dan tombol ON/OFF untuk Relay 3
            client.println("<p>Relay 3 " + output3State + "</p>");
            // Jika output3State mati, tampilkan tombol ON
            if (output3State == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">Nyala</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">Mati</button></a></p>");
              }
              
            // Menampilkan status dan tombol ON/OFF untuk Relay 4
            client.println("<p>Relay 4 " + output4State + "</p>");
            // Jika output4State mati, tampilkan tombol ON
            if (output4State == "off") {
              client.println("<p><a href=\"/0/on\"><button class=\"button\">Nyala</button></a></p>");
            } else {
              client.println("<p><a href=\"/0/off\"><button class=\"button button2\">Mati</button></a></p>");
            }
            
            // Menampilkan status dan tombol ON/OFF untuk Relay 5
            client.println("<p>Relay 5 " + output5State + "</p>");
            // Jika output5State mati, tampilkan tombol ON
            if (output5State == "off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">Nyala</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">Mati</button></a></p>");
            }
            
            // Menampilkan status dan tombol ON/OFF untuk Relay 6
            client.println("<p>Relay 6 " + output6State + "</p>");
            // Jika output6State mati, tampilkan tombol ON
            if (output6State == "off") {
              client.println("<p><a href=\"/14/on\"><button class=\"button\">Nyala</button></a></p>");
            } else {
              client.println("<p><a href=\"/14/off\"><button class=\"button button2\">Mati</button></a></p>");
            }
            
             // Tag penutup halaman HTML
            client.println("</body></html>");

            // Respon HTTP diakhiri dengan baris kosong
            client.println();
            
            // Keluar dari loop while
            break;
          } else { // Jika ada baris baru, hapus currentLine
            currentLine = "";
            }
        } else if (c != '\r') {  // Jika karakter bukan karakter carriage return,
          currentLine += c;      // tambahkan ke currentLine
        }
      }
    }
    // Menghapus variabel header
    header = "";
      
    // Menutup koneksi
    client.stop();  // Hentikan koneksi klien.

    Serial.println("Client disconnected."); // Cetak pesan yang menunjukkan bahwa klien terputus.
    Serial.println("");

    // Mengupdate status relay di Firebase
    Firebase.setString(firebaseData, "/Relay 1", output1State);   // Perbarui status Relay 1 di Firebase.
    Firebase.setString(firebaseData, "/Relay 2", output2State);   // Perbarui status Relay 2 di Firebase.
    Firebase.setString(firebaseData, "/Relay 3", output3State);   // Perbarui status Relay 3 di Firebase.
    Firebase.setString(firebaseData, "/Relay 4", output4State);   // Perbarui status Relay 4 di Firebase.
    Firebase.setString(firebaseData, "/Relay 5", output5State);   // Perbarui status Relay 5 di Firebase.
    Firebase.setString(firebaseData, "/Relay 6", output6State);   // Perbarui status Relay 6 di Firebase.

    if (firebaseData.dataAvailable())     // Memeriksa apakah data tersedia dalam objek firebaseData.
    {
      Serial.println("Selamat data berhasil dikirim ke Firebase !");   // Jika data tersedia, mencetak pesan ke Serial Monitor.
    }
    else {
      Serial.println("Maaf gagal mengirim data ke Firebase !");  // Jika data tidak tersedia, mencetak pesan ke Serial Monitor.
    }
  }
}