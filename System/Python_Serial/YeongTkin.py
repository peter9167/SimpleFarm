from tkinter import *
import tkinter as tk
import time
import datetime
import serial
import threading
import continuous_threading

ser = serial.Serial('COM4', 9600)
val1 = 0

root = tk.Tk()
root.geometry("1280x720")

index = []

def Pump_On():
    print("펌프 동작...")
    ser.write(b'0')

def Pump_Off():
    print("펌프 정지...")
    ser.write(b'1')

def LED_On():
    print("LED 동작...")
    ser.write(b'2')

def LED_Off():
    print("LED 정지...")
    ser.write(b'3')

def FAN_On():
    print("FAN 동작...")
    ser.write(b'4')

def FAN_Off():
    print("FAN 정지...")
    ser.write(b'5')

class Application(tk.Frame):

    def __init__(self, master):
        super().__init__(master)
        self.timer = None
        self.master = master
        self.master.title("스마트팜 모니터링 제어장치")
        self.pack(fill='both', expand=True)

        # 자세한 버튼 설명 : https://076923.github.io/posts/Python-tkinter-3/
        btn = Button(root)
        btn.config(width = 8, height = 4)
        btn.configure(font = ("굴림체", 20))
        btn.config(text = "펌프 동작", overrelief="solid", activebackground="green", activeforeground="white", cursor="cross_reverse")
        btn.config(repeatdelay=1000, repeatinterval=100, command=Pump_On)
        btn.place(x = 490, y = 220)


        btn2 = Button(root)
        btn2.config(width = 8, height = 4)
        btn2.configure(font = ("굴림체", 20))
        btn2.config(text = "펌프 정지", overrelief="solid", activebackground="green", activeforeground="white", cursor="cross_reverse")
        btn2.config(repeatdelay=1000, repeatinterval=100, command=Pump_Off)
        btn2.place(x = 665, y = 220)

        btn3 = Button(root)
        btn3.config(width = 8, height = 4)
        btn3.configure(font = ("굴림체", 20))
        btn3.config(text = "LED 동작", overrelief="solid", activebackground="green", activeforeground="white", cursor="cross_reverse")
        btn3.config(repeatdelay=1000, repeatinterval=100, command=LED_On)
        btn3.place(x = 840, y = 220)


        btn4 = Button(root)
        btn4.config(width = 8, height = 4)
        btn4.configure(font = ("굴림체", 20))
        btn4.config(text = "LED 정지", overrelief="solid", activebackground="green", activeforeground="white", cursor="cross_reverse")
        btn4.config(repeatdelay=1000, repeatinterval=100, command=LED_Off)
        btn4.place(x = 1015, y = 220)

        btn5 = Button(root)
        btn5.config(width = 8, height = 4)
        btn5.configure(font = ("굴림체", 20))
        btn5.config(text = "FAN 동작", overrelief="solid", activebackground="green", activeforeground="white", cursor="cross_reverse")
        btn5.config(repeatdelay=1000, repeatinterval=100, command=FAN_On)
        btn5.place(x = 490, y = 400)


        btn6 = Button(root)
        btn6.config(width = 8, height = 4)
        btn6.configure(font = ("굴림체", 20))
        btn6.config(text = "FAN 정지", overrelief="solid", activebackground="green", activeforeground="white", cursor="cross_reverse")
        btn6.config(repeatdelay=1000, repeatinterval=100, command=FAN_Off)
        btn6.place(x = 665, y = 400)

        btn7 = Button(root)
        btn7.config(width = 8, height = 4)
        btn7.configure(font = ("굴림체", 20))
        btn7.config(text = "", overrelief="solid", activebackground="green", activeforeground="white", cursor="cross_reverse")
        btn7.config(repeatdelay=1000, repeatinterval=100)
        btn7.place(x = 840, y = 400)


        btn8 = Button(root)
        btn8.config(width = 8, height = 4)
        btn8.configure(font = ("굴림체", 20))
        btn8.config(text = "", overrelief="solid", activebackground="green", activeforeground="white", cursor="cross_reverse")
        btn8.config(repeatdelay=1000, repeatinterval=100)
        btn8.place(x = 1015, y = 400)

        now = time.strftime("%Y년 %m월 %d일 %H시 %M분 %S초")
        self.label = tk.Label(self, text=str(now))
        self.label.pack(padx=10, pady=10)

        self.startTimer()
    
    def readserial():
        global val1
        ser_bytes = ser.readline()
        ser_bytes = ser_bytes.decode("utf-8")
        print(ser_bytes.rstrip())
        val1 = ser_bytes
        index.append(val1)

        if len(index) == 1:
            display1 = tk.Label(root, text=index[0], font = ("굴림체", "28")).place(x=150, y=220) # 온도 값 출력
        elif len(index) == 2:
            # display1 = tk.Label(root,text=index[0]).place(x=10,y=10)
            display2 = tk.Label(root, text=index[1], font = ("굴림체", "28")).place(x=150, y=350) # 습도 값 출력


        if len(index) == 2:
            # print("Done")
            index.clear()

        time.sleep(0)

    t1 = continuous_threading.PeriodicThread(0, readserial)
    t1.start()

    def startTimer(self, *_):
        self.tiktok()

    def tiktok(self):
        now = time.strftime("%Y년 %m월 %d일   %H:%M:%S")
        self.label.configure(font = ("굴림체", 20))
        self.label.config(text=str(now))
        self.timer = self.after(1000, self.tiktok)

name = tk.Label(root,  height=2, text="스마트팜 모니터링 제어장치", font = ("굴림체", "40"))
name.pack()

app = Application(root)
app.mainloop()