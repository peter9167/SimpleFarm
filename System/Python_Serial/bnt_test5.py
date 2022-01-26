import tkinter

window=tkinter.Tk()
window.title("YUN DAE HEE")
window.geometry("640x400+100+100")
window.resizable(False, False)

count=0

def countUP():
    global count
    count +=1
    label.config(text=str(count))

label = tkinter.Label(window, text="0")
label.pack()

# 자세한 버튼 설명은 https://076923.github.io/posts/Python-tkinter-3/
# repeatdelay : 버튼이 눌러진 상태에서 command 실행까지의 대기 시간
# repeatinterval : 버튼이 눌러진 상태에서 command 실행의 반복 시간

button = tkinter.Button(window, overrelief="solid", width=15, command=countUP, repeatdelay=100, repeatinterval=100)
button.pack()

window.mainloop()