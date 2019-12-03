import serial,time,winsound
arduino=serial.Serial('COM3', 9600)
while True:
    time.sleep(2)
    tempBas=arduino.readline()
    humBas=arduino.readline()
    temperatura=float(tempBas.decode("utf-8"))
    humedad=float(humBas.decode("utf-8"))
    print(f"Temp: {temperatura}")
    print(f"Hum: {humedad}")
    if temperatura >=30:
        arduino.write(b'1')
        print('\a')
        time.sleep(1)
    else:
        arduino.write(b'0')
        time.sleep(1)
arduino.close()