# serialTest.py


import serial

s = serial.Serial("/dev/ttyACM0", 115200)

s.write(b"Hello World")

for i in range(0, 1000):
    line = s.readline()   # read a '\n' terminated line
    print(line)
