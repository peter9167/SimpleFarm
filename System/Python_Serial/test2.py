import sys                              # 모름 
from PyQt5.QtWidgets import *           # PyQt5 사용하기 위한 것
from PyQt5 import uic                   # ui 파일을 직접 이용하기 위한 것.
import serial.tools.list_ports
form_class = uic.loadUiType("qt07.ui")[0]

# 콤보 박스 및  listWidget (listView )의  

nnum = 0                            # 여기에 선언하면 전역변수라는 

class MyWindow(QMainWindow, form_class):
    
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.pushButton.clicked.connect(self.btn_clicked)
        self.pushButton_1.clicked.connect(self.btn1_clicked)


        #self.pushButton.setText("hoho")  # 
    def btn_clicked(self):
        global nnum                 # 이렇게 하면 전역 변수를 사용함.
        nnum =  nnum+1;
        self.listWidget.addItem("Text add " + str(nnum))
        self.comboBox.addItem("COM"+str(nnum))

    def btn1_clicked(self):
        self.comboBox.clear()           # 찾기 버튼 두 번 이상 눌렀을 때 대비용
        ports = serial.tools.list_ports.comports()
        
        available_ports = []
        for p in ports:
            available_ports.append(p.device)    
        if len(available_ports) > 0:
            available_ports.sort()                  # 이 부분이 있어야..
            for  aa in available_ports:
                self.comboBox.addItem(aa)
        

if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWindow = MyWindow()
    myWindow.show()
    app.exec_()