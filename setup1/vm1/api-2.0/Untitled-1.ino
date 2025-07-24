#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

String tag;
String ip = "100.26.43.95";
String Bearer = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjE2NTQyMDg3NTMsInVzZXJuYW1lIjoiVXNlcjEiLCJvcmdOYW1lIjoiT3JnMSIsImlhdCI6MTY1NDE3Mjc1M30.AX-KQJKoVfoN3QBFKKbpBmJkcGoIScR4NPf7KQa7Z6o";
String KARTLAR[6] = {"21414892131", "1712916546", "1452751199", "161104110185", "511813114", "9414924020"};

//#define WLAN_SSID "COMUC24"
//#define WLAN_PASSWORD  "Ceng1881."
#define WLAN_SSID "Çağlayan İphone'u"
#define WLAN_PASSWORD  "beceriksiz"

HTTPClient http;
WiFiClient client;

void WiFi_Setup(){
  delay(100);
  Serial.println();
  Serial.print(WLAN_SSID);
  Serial.print(" agina baglaniliyor");
  delay(100);
  
  WiFi.begin(WLAN_SSID, WLAN_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print(WLAN_SSID);
  Serial.print(" agina baglandi\n");
  Serial.println("IP adresi: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  delay(1000);
  WiFi_Setup();
  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Yeni kullanıcı oluşturmak istiyor musunuz?(Y/N): ");
  while(Serial.available() == 0){}
  String cevap = Serial.readString();
  cevap = cevap.substring(0,cevap.length()-1);
  if(cevap.equals("Y")){
    Serial.println("-----------------------REGISTER USER-----------------------");
    Serial.println("ENTER USER NAME: ");
    while(Serial.available() == 0){}
    String userName = Serial.readString();
    userName = userName.substring(0,userName.length()-1);
    
    Serial.println("ENTER ORGANIZATION NAME(Org1): ");
    while(Serial.available() == 0){}
    String orgName = Serial.readString();
    orgName = orgName.substring(0,orgName.length()-1);

    RegisterUser(userName, orgName);
    Serial.println("-----------------------END OF REGISTER USER-----------------------\n");
  }
  
  Serial.println("Lutfen kart okutun.");
}

void loop() {
  
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {
    if ( ! rfid.PICC_IsNewCardPresent())
      return;
    if (rfid.PICC_ReadCardSerial()) {
      for (byte i = 0; i < 4; i++) {
        tag += rfid.uid.uidByte[i];
      }

      if (tag.equals(KARTLAR[0])) {//anahtarlık - Register User
          Serial.println("-----------------------REGISTER USER-----------------------");
          Serial.println("ENTER USER NAME: ");
          while(Serial.available() == 0){}
          String userName = Serial.readString();
          userName = userName.substring(0,userName.length()-1);
          
          Serial.println("ENTER ORGANIZATION NAME(Org1): ");
          while(Serial.available() == 0){}
          String orgName = Serial.readString();
          orgName = orgName.substring(0,orgName.length()-1);
  
          RegisterUser(userName, orgName);
      
          Serial.println("-----------------------END OF REGISTER USER-----------------------\n");
          Serial.println("Lütfen kart okutun.");
      } else if (tag.equals(KARTLAR[1])) {//beyaz kart - Create Product
          Serial.println("-----------------------CREATE PRODUCT-----------------------");
          Serial.println("ENTER PRODUCT ID: ");
          while(Serial.available() == 0){}
          String productID = Serial.readString();
          productID = productID.substring(0,productID.length()-1);
          
          Serial.println("ENTER PRODUCT NAME: ");
          while(Serial.available() == 0){}
          String productName = Serial.readString();
          productName = productName.substring(0,productName.length()-1);
          
          Serial.println("ENTER PRODUCT CLASS: ");
          while(Serial.available() == 0){}
          String productClass = Serial.readString();
          productClass = productClass.substring(0,productClass.length()-1);
          
          Serial.println("ENTER PRODUCER: ");
          while(Serial.available() == 0){}
          String producer = Serial.readString();
          producer = producer.substring(0,producer.length()-1);
  
          CreateProduct(productID, productName, productClass, producer);
          
          Serial.println("-----------------------END OF CREATE PRODUCT-----------------------\n");
          Serial.println("Lütfen kart okutun.");
      } else if (tag.equals(KARTLAR[2])) {//ziraat kredi kartı - Change Product Status
          Serial.println("-----------------------CHANGE PRODUCT STATUS-----------------------");
          Serial.println("ENTER PRODUCT ID: ");
          while(Serial.available() == 0){}
          String productID = Serial.readString();
          productID = productID.substring(0,productID.length()-1);
          
          Serial.println("ENTER TRANSPORTER: ");
          while(Serial.available() == 0){}
          String productStatus = Serial.readString();
          productStatus = productStatus.substring(0,productStatus.length()-1);
  
          ChangeProductStatus(productID, productStatus);
      
          Serial.println("-----------------------END OF CHANGE PRODUCT STATUS-----------------------\n");
          Serial.println("Lütfen kart okutun.");
      } else if (tag.equals(KARTLAR[3])) {//okul ziraat kartı - Query Product
          Serial.println("-----------------------QUERY PRODUCT-----------------------");
          Serial.println("ENTER PRODUCT ID: ");
          while(Serial.available() == 0){}
          String productID = Serial.readString();
          productID = productID.substring(0,productID.length()-1);
  
          QueryProduct(productID);
      
          Serial.println("-----------------------END OF QUERY PRODUCT-----------------------\n");
          Serial.println("Lütfen kart okutun.");
      } else if (tag.equals(KARTLAR[4])) {//öğrenci kartı
          Serial.println("-----------------------QUERY ALL PRODUCT-----------------------");
  
          QueryAllProduct();
          Serial.println("-----------------------END OF QUERY ALL PRODUCT-----------------------\n");
          Serial.println("Lütfen kart okutun.");
      } else if (tag.equals(KARTLAR[5])){
          Serial.println("-----------------------GET HISTORY FOR ASSET-----------------------");
          
          Serial.println("ENTER PRODUCT ID: ");
          while(Serial.available() == 0){}
          String productID = Serial.readString();
          productID = productID.substring(0,productID.length()-1);
          
          GetHistoryForAsset(productID);
          Serial.println("-----------------------END OF GET HISTORY FOR ASSET-----------------------\n");
          Serial.println("Lütfen kart okutun.");
      }
      
      tag = "";
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
}

void RegisterUser(String userName, String orgName) {
  StaticJsonBuffer<500> jsonBuffer;
  String reqData = "{\r\n    \"username\":\"" + userName + "\",\r\n    \"orgName\":\"" + orgName + "\"\r\n}";
  JsonObject& json = jsonBuffer.parseObject(reqData);
  
  Serial.print("[HTTP] begin...\n");
  http.begin(client, "http://"+ip+":4000/users"); //HTTP
  http.addHeader("Content-Type", "application/json");

  String jsonData;
  json.printTo(jsonData);
  
  Serial.print("[HTTP] POST...\n");

  int httpCode = http.POST(jsonData);

  if (httpCode > 0) {
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
      Serial.println("received payload:\n<<");
      Serial.println(payload);
      Serial.println(">>");
      int count = payload.length() + 1;
      char charArray[count];
      payload.toCharArray(charArray, count);
      char *temp = strtok(charArray, "\"");
      count = 0;
      while(temp != NULL){
        if(count == 9){
          Bearer = temp;
          break;
        }
        temp = strtok(NULL,"\"");
        count++;
      }
      //Serial.println("Bearer: " + Bearer);
    }
    
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  delay(500);
}

void CreateProduct(String productID, String productName, String productClass, String producer) {
  StaticJsonBuffer<500> jsonBuffer;
  String reqData = "{\r\n    \"fcn\": \"createProduct\",\r\n    \"peers\": [\r\n        \"peer0.org1.example.com\",\r\n        \"peer0.org2.example.com\",\r\n        \"peer0.org3.example.com\"\r\n    ],\r\n    \"chaincodeName\": \"example\",\r\n    \"channelName\": \"mychannel\",\r\n    \"args\": [\r\n        \"" + productID + "\",\r\n        \"" + productName + "\",\r\n        \"" + productClass + "\",\r\n        \"" + producer + "\"\r\n    ]\r\n}";
  JsonObject& json = jsonBuffer.parseObject(reqData);
  Serial.print("[HTTP] begin...\n");
  http.begin(client, "http://"+ip+":4000/channels/mychannel/chaincodes/example"); //HTTP
  http.addHeader("Authorization", "Bearer " + Bearer + "");
  http.addHeader("Content-Type", "application/json");

  String jsonData;
  json.printTo(jsonData);
  
  Serial.print("[HTTP] POST...\n");

  int httpCode = http.POST(jsonData);


  if (httpCode > 0) {
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
      Serial.println("received payload:\n<<");
      Serial.println(payload);
      Serial.println(">>");
    }
    
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  delay(500);
}

void ChangeProductStatus(String productID, String productStatus) {
  StaticJsonBuffer<500> jsonBuffer;
  String reqData = "{\r\n    \"fcn\": \"changeProductStatus\",\r\n    \"peers\": [\r\n        \"peer0.org1.example.com\",\r\n        \"peer0.org2.example.com\",\r\n        \"peer0.org3.example.com\"\r\n    ],\r\n    \"chaincodeName\": \"example\",\r\n    \"channelName\": \"mychannel\",\r\n    \"args\": [\r\n        \"" + productID + "\",\r\n        \"" + productStatus + "\"]\r\n}";
  JsonObject& json = jsonBuffer.parseObject(reqData);
  Serial.print("[HTTP] begin...\n");
  http.begin(client, "http://"+ip+":4000/channels/mychannel/chaincodes/example"); //HTTP
  http.addHeader("Authorization", "Bearer " + Bearer + "");
  http.addHeader("Content-Type", "application/json");

  String jsonData;
  json.printTo(jsonData);
  
  Serial.print("[HTTP] POST...\n");

  int httpCode = http.POST(jsonData);


  if (httpCode > 0) {
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
      Serial.println("received payload:\n<<");
      Serial.println(payload);
      Serial.println(">>");
    }
    
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  delay(500);
}

void QueryProduct(String productID){
  Serial.print("[HTTP] begin...\n");
  http.begin(client, "http://"+ip+":4000/channels/mychannel/chaincodes/example?fcn=queryProduct&peers=['peer0.org1.example.com','peer0.org2.example.com','peer0.org3.example.com']&args=['"+productID+"']"); //HTTP
  http.addHeader("Authorization", "Bearer " + Bearer + "");
  http.addHeader("Content-Type", "application/json");
  
  Serial.print("[HTTP] GET...\n");
  digitalWrite(LED_BUILTIN, LOW);

  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
      //Serial.println("received payload:\n<<");
      //Serial.println(payload);
      PrintJsonProduct(payload, productID);
      //Serial.println(">>");
    }
    
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  delay(500);
}

void QueryAllProduct(){
  Serial.print("[HTTP] begin...\n");
  http.begin(client, "http://"+ip+":4000/channels/mychannel/chaincodes/example?fcn=queryAllProduct&peers=['peer0.org1.example.com','peer0.org2.example.com','peer0.org3.example.com']&args=['PRODUCT1']"); //HTTP
  http.addHeader("Authorization", "Bearer " + Bearer + "");
  http.addHeader("Content-Type", "application/json");
  
  Serial.print("[HTTP] GET...\n");
  digitalWrite(LED_BUILTIN, LOW);

  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
      //Serial.println("received payload:\n<<");
      //Serial.println(payload);
      PrintJsonAllProduct(payload);
      //Serial.println(">>");
    }
    
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  delay(500);
}

void GetHistoryForAsset(String productID){
  Serial.print("[HTTP] begin...\n");
  http.begin(client, "http://"+ip+":4000/channels/mychannel/chaincodes/example?fcn=getHistoryForAsset&args=['"+productID+"']&peer=peer0.org1.example.com"); //HTTP
  http.addHeader("Authorization", "Bearer " + Bearer + "");
  http.addHeader("Content-Type", "application/json");
  
  Serial.print("[HTTP] GET...\n");
  digitalWrite(LED_BUILTIN, LOW);

  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
      //Serial.println("received payload:\n<<");
      //Serial.println(payload);
      PrintJsonProductHistory(payload,productID);
      //Serial.println(">>");
    }
    
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  delay(500);
}

void PrintJsonAllProduct(String veri){
  DynamicJsonBuffer doc;

  JsonObject& obj = doc.parseObject(veri);

  if (!obj.success()) {
    Serial.print("parseObject() failed: ");
    return;
  }

  for (JsonObject& result_item : obj["result"].as<JsonArray>()) {
  
    const char* result_item_Key = result_item["Key"]; // "PRODUCT1", "PRODUCT10", "PRODUCT11", "PRODUCT12", ...
  
    JsonObject& result_item_Record = result_item["Record"];
    const char* result_item_Record_producer = result_item_Record["producer"]; // "MSI", "RAMPAGE", "FABER ...
    const char* result_item_Record_producercheckoutdate = result_item_Record["producercheckoutdate"];
    const char* result_item_Record_productclass = result_item_Record["productclass"]; // "Teknoloji", ...
    const char* result_item_Record_productiondate = result_item_Record["productiondate"]; // "10-23-2021 ...
    const char* result_item_Record_productname = result_item_Record["productname"]; // "Bilgisayar", ...
    const char* result_item_Record_status = result_item_Record["status"]; // "Depoda", "Uretildi", ...
    const char* result_item_Record_transporter = result_item_Record["transporter"]; // "MNG KARGO", "n/a", ...
    const char* result_item_Record_transportercheckoutdate = result_item_Record["transportercheckoutdate"];
    const char* result_item_Record_transporterentrydate = result_item_Record["transporterentrydate"];
    const char* result_item_Record_warehouse = result_item_Record["warehouse"]; // "KOCAELI DEPO", "n/a", ...
    const char* result_item_Record_warehousecheckoutdate = result_item_Record["warehousecheckoutdate"];
    const char* result_item_Record_warehouseentrydate = result_item_Record["warehouseentrydate"];
  
    Serial.println("----------------------------------" + String(result_item_Key) + "----------------------------------");
    Serial.println("PRODUCER: \t\t\t" + String(result_item_Record_producer));
    Serial.println("PRODUCER CHECK OUT DATE: \t" + String(result_item_Record_producercheckoutdate));
    Serial.println("PRODUCT CLASS: \t\t\t" + String(result_item_Record_productclass));
    Serial.println("PRODUCTION DATE: \t\t" + String(result_item_Record_productiondate));
    Serial.println("PRODUCT NAME: \t\t\t" + String(result_item_Record_productname));
    Serial.println("PRODUCT STATUS: \t\t" + String(result_item_Record_status));
    Serial.println("TRANSPORTER: \t\t\t" + String(result_item_Record_transporter));
    Serial.println("TRANSPORTER CHECK OUT DATE: \t" + String(result_item_Record_transportercheckoutdate));
    Serial.println("TRANSPORTER ENTRY DATE: \t" + String(result_item_Record_transporterentrydate));
    Serial.println("WAREHOUSE: \t\t\t" + String(result_item_Record_warehouse));
    Serial.println("WAREHOUSE CHECK OUT DATE: \t" + String(result_item_Record_warehousecheckoutdate));
    Serial.println("WAREHOUSE ENTRY DATE: \t\t" + String(result_item_Record_warehouseentrydate) + "\n---------------------------------------------------------------------------");
    
  }
}

void PrintJsonProductHistory(String veri, String productID){
  DynamicJsonBuffer doc;

  JsonObject& obj = doc.parseObject(veri);

  if (!obj.success()) {
    Serial.print("parseObject() failed: ");
    return;
  }
  Serial.println("---------------------------------------------------------------------------");
  for (JsonObject& result_item : obj["result"].as<JsonArray>()) {
  
    //const char* result_item_Key = result_item["Key"]; // "PRODUCT1", "PRODUCT10", "PRODUCT11", "PRODUCT12", ...
  
    JsonObject& result_item_Record = result_item["Value"];
    const char* result_item_Record_producer = result_item_Record["producer"]; // "MSI", "RAMPAGE", "FABER ...
    const char* result_item_Record_producercheckoutdate = result_item_Record["producercheckoutdate"];
    const char* result_item_Record_productclass = result_item_Record["productclass"]; // "Teknoloji", ...
    const char* result_item_Record_productiondate = result_item_Record["productiondate"]; // "10-23-2021 ...
    const char* result_item_Record_productname = result_item_Record["productname"]; // "Bilgisayar", ...
    const char* result_item_Record_status = result_item_Record["status"]; // "Depoda", "Uretildi", ...
    const char* result_item_Record_transporter = result_item_Record["transporter"]; // "MNG KARGO", "n/a", ...
    const char* result_item_Record_transportercheckoutdate = result_item_Record["transportercheckoutdate"];
    const char* result_item_Record_transporterentrydate = result_item_Record["transporterentrydate"];
    const char* result_item_Record_warehouse = result_item_Record["warehouse"]; // "KOCAELI DEPO", "n/a", ...
    const char* result_item_Record_warehousecheckoutdate = result_item_Record["warehousecheckoutdate"];
    const char* result_item_Record_warehouseentrydate = result_item_Record["warehouseentrydate"];
  
    Serial.println("PRODUCER: \t\t\t" + String(result_item_Record_producer));
    Serial.println("PRODUCER CHECK OUT DATE: \t" + String(result_item_Record_producercheckoutdate));
    Serial.println("PRODUCT CLASS: \t\t\t" + String(result_item_Record_productclass));
    Serial.println("PRODUCTION DATE: \t\t" + String(result_item_Record_productiondate));
    Serial.println("PRODUCT NAME: \t\t\t" + String(result_item_Record_productname));
    Serial.println("PRODUCT STATUS: \t\t" + String(result_item_Record_status));
    Serial.println("TRANSPORTER: \t\t\t" + String(result_item_Record_transporter));
    Serial.println("TRANSPORTER CHECK OUT DATE: \t" + String(result_item_Record_transportercheckoutdate));
    Serial.println("TRANSPORTER ENTRY DATE: \t" + String(result_item_Record_transporterentrydate));
    Serial.println("WAREHOUSE: \t\t\t" + String(result_item_Record_warehouse));
    Serial.println("WAREHOUSE CHECK OUT DATE: \t" + String(result_item_Record_warehousecheckoutdate));
    Serial.println("WAREHOUSE ENTRY DATE: \t\t" + String(result_item_Record_warehouseentrydate) + "\n---------------------------------------------------------------------------");
    
  }
}

void PrintJsonProduct(String veri, String key){
  DynamicJsonBuffer doc;

  JsonObject& obj = doc.parseObject(veri);

  if (!obj.success()) {
    Serial.print("parseObject() failed: ");
    return;
  }

  JsonObject& result_item = obj["result"];

  const char* result_item_Record_producer = result_item["producer"]; // "MSI", "RAMPAGE", "FABER ...
  const char* result_item_Record_producercheckoutdate = result_item["producercheckoutdate"];
  const char* result_item_Record_productclass = result_item["productclass"]; // "Teknoloji", ...
  const char* result_item_Record_productiondate = result_item["productiondate"]; // "10-23-2021 ...
  const char* result_item_Record_productname = result_item["productname"]; // "Bilgisayar", ...
  const char* result_item_Record_status = result_item["status"]; // "Depoda", "Uretildi", ...
  const char* result_item_Record_transporter = result_item["transporter"]; // "MNG KARGO", "n/a", ...
  const char* result_item_Record_transportercheckoutdate = result_item["transportercheckoutdate"];
  const char* result_item_Record_transporterentrydate = result_item["transporterentrydate"];
  const char* result_item_Record_warehouse = result_item["warehouse"]; // "KOCAELI DEPO", "n/a", ...
  const char* result_item_Record_warehousecheckoutdate = result_item["warehousecheckoutdate"];
  const char* result_item_Record_warehouseentrydate = result_item["warehouseentrydate"];

  Serial.println("----------------------------------" + key + "----------------------------------");
  Serial.println("PRODUCER: \t\t\t" + String(result_item_Record_producer));
  Serial.println("PRODUCER CHECK OUT DATE: \t" + String(result_item_Record_producercheckoutdate));
  Serial.println("PRODUCT CLASS: \t\t\t" + String(result_item_Record_productclass));
  Serial.println("PRODUCTION DATE: \t\t" + String(result_item_Record_productiondate));
  Serial.println("PRODUCT NAME: \t\t\t" + String(result_item_Record_productname));
  Serial.println("PRODUCT STATUS: \t\t" + String(result_item_Record_status));
  Serial.println("TRANSPORTER: \t\t\t" + String(result_item_Record_transporter));
  Serial.println("TRANSPORTER CHECK OUT DATE: \t" + String(result_item_Record_transportercheckoutdate));
  Serial.println("TRANSPORTER ENTRY DATE: \t" + String(result_item_Record_transporterentrydate));
  Serial.println("WAREHOUSE: \t\t\t" + String(result_item_Record_warehouse));
  Serial.println("WAREHOUSE CHECK OUT DATE: \t" + String(result_item_Record_warehousecheckoutdate));
  Serial.println("WAREHOUSE ENTRY DATE: \t\t" + String(result_item_Record_warehouseentrydate) + "\n---------------------------------------------------------------------------");
}