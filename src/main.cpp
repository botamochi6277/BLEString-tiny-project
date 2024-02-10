#include <Arduino.h>
#include <ArduinoBLE.h>
#define BLE_GATT_CPF_UNIT_UNITLESS 0x2700
#define BLE_GATT_CPF_FORMAT_UTF8S 0x19

class MyBLEService : public BLEService {
 private:
  const uint8_t string_format_[7] = {BLE_GATT_CPF_FORMAT_UTF8S,
                                     0,
                                     (uint8_t)BLE_GATT_CPF_UNIT_UNITLESS,
                                     (uint8_t)(BLE_GATT_CPF_UNIT_UNITLESS >> 8),
                                     0x01,
                                     0x00,
                                     0x00};

 public:
  BLEStringCharacteristic string_chr;

  MyBLEService();
};

MyBLEService::MyBLEService()
    : BLEService("4191d4c0-7477-5fd0-ce8b-0f205d3cf1ee"),
      string_chr("4191d4c1-7477-5fd0-ce8b-0f205d3cf1ee", BLERead | BLEWrite,
                 24) {
  this->addCharacteristic(this->string_chr);

  BLEDescriptor user_descriptor("2901", "message");
  this->string_chr.addDescriptor(user_descriptor);

  BLEDescriptor str_descriptor("2904", this->string_format_, 7);
  this->string_chr.addDescriptor(str_descriptor);
}

MyBLEService my_srv;

void setup() {
  Serial.begin(115200);

  for (size_t i = 0; i < 100; i++) {
    if (Serial) {
      break;
    }
    delay(10);
  }

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1) {
      delay(100);
    }
  }
  String ble_address = BLE.address();
  String local_name = "StringTester_" + ble_address;
  BLE.setDeviceName(local_name.c_str());
  BLE.setLocalName("StringTester");
  BLE.setAdvertisedService(my_srv);
  BLE.addService(my_srv);
  my_srv.string_chr.setValue("Hello!");
  // start advertising
  BLE.advertise();
}

void loop() {
  BLE.poll();
  delay(100);
}
