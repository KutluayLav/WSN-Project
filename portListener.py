import serial
import time
import requests
import sys  # sys modülünü ekleyin

# Seri port bağlantısını yapılandırın
ser = serial.Serial('COM5', 9600)  # Port adını ve baud rate'i uygun şekilde ayarlayın

try:
    while True:
        # Seri porttan veri oku
        data = ser.readline().decode('utf-8').strip()

        # Okunan veriyi işle
        if data and data not in "6000" and len(data) == 6:
            dB = data[4:6]
            heat = data[0:2]
            humadity = data[2:4]
            # Veriyi GET isteğiyle gönder
            url = f'GET https://api.thingspeak.com/update?api_key=3KBN6FW51521S7A2&field1={dB}&field2={heat}&field3={humadity}'
            response = requests.get(url)

            if response.status_code == 200:
                print("GET isteği başarıyla gönderildi.")
            else:
                print(f"Hata oluştu. Sunucu cevap kodu: {response.status_code}")

        # 2 saniye bekle
        time.sleep(2)
except:
    print("")
