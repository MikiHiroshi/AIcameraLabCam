//This code is for M5StickC of LabCam with reference to the article by no研@nnn112358.
//https://qiita.com/nnn112358/items/5efd926fea20cd6c2c43
//Special thanks to ano研 @nnn112358.


#include <M5StickC.h>
#include <WiFi.h>
#include <ssl_client.h>
#include <WiFiClientSecure.h>

const char* ssid = "YOUR_WIFI_SSID";   //INPUT your wifi ssid
const char* passwd = "YOUR_WIFI_PASSWORD";  //INPUT your wifi password
const char* token = "YOUR_LINE_NOTIFY_TOKEN";  //INPUT your LINE notify token
const char* host = "notify-api.line.me";

HardwareSerial serial_ext(2);

void setup_wifi() ;
void setup() {
  M5.begin();
  setup_wifi();
  send("WIFI connected");
  M5.Lcd.setRotation(1);
  M5.Lcd.setCursor(2, 4, 2);
  M5.Lcd.println("Wifi_Connected");
  serial_ext.begin(115200, SERIAL_8N1, 32, 33);
}

void loop() {
  M5.update();
  delay(500);
  if (serial_ext.available()>0) {
    String str = serial_ext.readStringUntil('\n');
    int data = str.toInt();
    Serial.print("data:");
    Serial.write(data);
    if (data == 1) {
    M5.Lcd.print('1');
    send("画像1が現れた。");  // 画像1認識時のメッセージを書く.
  } else if (data == 2) {
    M5.Lcd.print('2');
    send("画像2が現れた。");  // 画像2認識時のメッセージを書く.
  } else if (data == 3) {
    M5.Lcd.print('3');
    send("画像3が現れた。");  // 画像3認識時のメッセージを書く.
    }
  }
  vTaskDelay(1000 / portTICK_RATE_MS);
}


 /* Wifiに接続する */
void setup_wifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


 /* LINE Notifyに送る */
void send(String message) {
  WiFiClientSecure client;
  Serial.println("Try");
  //LineのAPIサーバに接続
  if (!client.connect(host, 443)) {
    Serial.println("Connection failed");
    return;
  }
  Serial.println("Connected");
  //リクエストを送信
  String query = String("message=") + message;
  String request = String("") +
               "POST /api/notify HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + token + "\r\n" +
               "Content-Length: " + String(query.length()) +  "\r\n" + 
               "Content-Type: application/x-www-form-urlencoded\r\n\r\n" +
                query + "\r\n";
  client.print(request);

  //受信終了まで待つ 
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      break;
    }
  }

  String line = client.readStringUntil('\n');
  Serial.println(line);
}


