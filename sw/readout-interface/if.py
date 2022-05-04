import serial
import matplotlib.pyplot as plot
import numpy as np

ser = serial.Serial("COM7")

while True:
    rx_val = int(ser.readline().decode('utf-8').rstrip())
    rx_array = []
    
    if rx_val == 65535:
        for i in range(0, 1023):
            rx_val = int(ser.readline().decode('utf-8').rstrip())
            rx_array.append(rx_val)
            
        plot.plot(rx_array)
        plot.draw()
        plot.pause(0.01)
        plot.cla()