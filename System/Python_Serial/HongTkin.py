import sys
from tkinter import *
import tkinter as tk
import time
import serial
import threading
import continuous_threading

ser = serial.Serial('COM9', 9600)
val1 = 0

index = []


def readserial():
    global val1
    ser_bytes = ser.readline()
    ser_bytes = ser_bytes.decode("utf-8")
    print(ser_bytes.rstrip())
    val1 = ser_bytes
    index.append(val1)


    if len(index) == 1:
        display1 = tk.Label(root, text=index[0], font = ("굴림체", "28")).place(x=100, y=290)
    elif len(index) == 2:
        # display1 = tk.Label(root,text=`index[0]).place(x=10,y=10)
        display2 = tk.Label(root, text=index[1], font = ("굴림체", "28")).place(x=500, y=40)



    if len(index) == 2:
        # print("Done")
        index.clear()

    time.sleep(0)



t1 = continuous_threading.PeriodicThread(0, readserial)

root = tk.Tk()
root.geometry("1920x1080")

#화면 구성
name = tk.Label(root,  height=2, text="스마트팜", font = ("굴림체", "40"))
name.pack()



#w = tk.Label(root, text="Hum.").place(x=10, y=10)
#w1 = tk.Label(root, text="Temp.").place(x=10, y=40)
t1.start()
# t1.join()

root.mainloop()