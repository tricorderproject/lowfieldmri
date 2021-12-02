# serialTest.py


import serial
import matplotlib.pyplot as plt

s = serial.Serial("/dev/ttyACM0", 115200)

s.write(b"Hello World")

for a in range(0, 100):
    # Wait for 'data'
    for i in range(0, 100000):
        line = s.readline()
        line = line.decode('UTF-8').strip()    
        if (line.startswith("DATA:")):
            break

    data = []
    for i in range(0, 100000):
        line = s.readline()   # read a '\n' terminated line
        line = line.decode('UTF-8').strip()    

        value = -1
        try:
            value = int(line)        
            data.append(value)
        except ValueError:
            # do nothing
            value = -1


    for i in range(0, len(data)):
        print(str(i) + "\t" + str(data[i]))

    plt.clf()
    plt.plot(data)
    plt.draw()
    plt.pause(0.001)