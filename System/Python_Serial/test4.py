from tkinter import *
import datetime
import tkinter
import time

class Application(tkinter.Frame):
    def __init__(self, master):
        super().__init__(master)
        self.timer = None
        self.master = master
        self.master.title("서귀포산업과학고등학교 발명반 홍경민")
        self.pack(fill='both', expand=True)

        lab = Label(root)
        lab.config(text = "스마트팜 모니터링", fg = 'green')
        lab.configure(font = ("고딕", 30))
        lab.place(x = 790, y = 60)

        temp = Label(root)
        temp.config(text = "온도")
        temp.configure(font = ("고딕", 20))
        temp.place(x = 795, y = 250)

        Humidity = Label(root)
        Humidity.config(text = "습도")
        Humidity.configure(font = ("고딕", 20))
        Humidity.place(x = 795, y = 400)

        soil = Label(root)
        soil.config(text = "토양 수분")
        soil.configure(font = ("고딕", 20))
        soil.place(x = 765, y = 550)

        btn = Button(root)
        btn.config(width = 10, height = 1)
        btn.configure(font = ("고딕", 20))
        btn.config(text = "모터 작동")
        btn.place(x = 750, y = 700)


        btn2 = Button(root)
        btn2.config(width = 10, height = 1)
        btn2.configure(font = ("고딕", 20))
        btn2.config(text = "모터 정지")
        btn2.place(x = 985, y = 700)

        now = time.strftime("%Y년 %m월 %d일 %H시 %M분 %S초")
        self.label = tkinter.Label(self, text=str(now))
        self.label.pack(padx=10, pady=10)

        self.startTimer()

    def startTimer(self, *_):
        self.tiktok()

    def tiktok(self):
        now = time.strftime("%Y년 %m월 %d일   %H:%M:%S")
        self.label.configure(font = ("고딕", 15))
        self.label.config(text=str(now))
        self.timer = self.after(1000, self.tiktok)

root = tkinter.Tk()
root.geometry("1024x768+100+100")
app = Application(root)
app.mainloop()