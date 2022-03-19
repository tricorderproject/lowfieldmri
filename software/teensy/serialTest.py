# serialTest.py


import serial
import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack

s = serial.Serial("/dev/ttyACM0", 115200)

#s.write(b"Hello World")

#for a in range(0, 100):

def collectAndDrawData():
    # Send the command to read on the ADC
    s.write(b"r")

    # Wait for 'data'
    for i in range(0, 50000):
        line = s.readline()
        line = line.decode('UTF-8').strip()    
        if (line.startswith("DATA:")):
            break

    data = []
    for i in range(0, 50000):
        line = s.readline()   # read a '\n' terminated line
        line = line.decode('UTF-8').strip()    

        value = -1
        try:
            value = int(line)        
            value = value - 32768

            # Clipping
            if (value > 32768):
                value = 32768
            if (value < -32768):
                value = -32768

            data.append(value)
        except ValueError:
            # do nothing
            value = -1


    for i in range(0, len(data)):
        print(str(i) + "\t" + str(data[i]))
    
    y = data
    x = range(len(y))
    yf = scipy.fftpack.fft(y)

    yf = yf[:int(len(yf)/2)]
    xf = range(len(yf))

    #xf = np.linspace(0.0, 1.0/(2.0*T), N//2)

    

    plt.clf()
    #plt.plot(data)
    plt.subplot(2, 1, 1)
    plt.plot(x, y)    
    plt.draw()
    plt.subplot(2, 1, 2)
    plt.plot(xf, yf)
    plt.draw()
    #plt.pause(0.001)
    plt.pause(0.5)


#
#   Main Program
#
while (True):
    #input("")
    name = input("Press Enter to continue...")
    print("name: " + str(name))
    collectAndDrawData()