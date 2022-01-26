import serial
import time

# 시리얼 포트와 baud rate 지정
ser = serial.Serial('COM3', 9600)
time.sleep(2)  # 접속 대기


def flash():

    while(True):
        cmd = input('\n0.LED 끄기 1.LED 켜기 2.종료: 0/1/2 를 입력하세요. ')

        if cmd == "0":
            print("LED 끄기...")
            ser.write(b'L')
        elif cmd == "1":
            print("LED 켜기...")
            ser.write(b'H')
        elif cmd == "2":
            ser.write(b'L')      # LED 끄고 종료
            print("프로그램 종료")
            break
        else:
            print("입력 오류. 0/1/2 를 입력하세요.")


flash()
ser.close()