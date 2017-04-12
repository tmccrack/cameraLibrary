import socket
from PyQt5 import QtCore, QtWidgets
import numpy as np
import struct
from fttMirrorWindow import Ui_Dialog as mirrorWindow  ## pyuic5 generated file


import time

class SClient():
    """
    Socket class for connecting to mirror server.
    Uses the closed loop mode which holds the socket open for updates.
    Closing and reopening the socket fails.
    """
    def __init__(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self, host, port):
        try:
            self.client.connect((host, port))
            mess = b'Closed'
            suc = self.client.sendall(b'06')
            suc = self.client.sendall(mess)
            print("connected")
        except OSError as exception:
            print("cannot connect")


    def close(self):
        self.client.shutdown(socket.SHUT_RDWR)
        self.client.close()
        print("closed")
        self.client

    def sendUpdates(self, x, y):
        data = (str(x) + ';' + str(y)).encode('utf-8')
        # Server expects two bytes
        if len(str(len(data)).encode('utf-8')) < 2:
            mess = ('0' + str(len(data))).encode('utf-8')
        else:
            mess = str(len(data)).encode('utf-8')

        suc = self.client.sendall(mess)
        suc = self.client.sendall(data)


class Mirror(QtWidgets.QDialog, mirrorWindow):
    """
    Dialog window for manually moving mirror
    """
    def __init__(self, parent=None, mpl_window=None):
        super(Mirror, self).__init__()
        self.setupUi(self)
        self.host = 'localhost'
        self.port = 6666
        self.mpl_window = mpl_window
        self.connectSlots()
        self.getValues()

    def connectSlots(self):
        self.spb_Ch0.valueChanged.connect(self.moveMirror)
        self.spb_Ch1.valueChanged.connect(self.moveMirror)
        self.btn_Ch0Plus.clicked.connect(self.jogUpCh0)
        self.btn_Ch0Minus.clicked.connect(self.jogDownCh0)
        self.btn_Ch1Plus.clicked.connect(self.jogUpCh1)
        self.btn_Ch1Minus.clicked.connect(self.jogDownCh1)
        
    def keyPressEvent(self, qKeyEvent):
        # Ignore enters so they do not increment channels
        if qKeyEvent.key() == QtCore.Qt.Key_Return:
            pass

    def moveMirror(self):
        self.sendSingleUpdate(self.spb_Ch0.value(), self.spb_Ch1.value())

    def jogUpCh0(self):
        self.spb_Ch0.setValue(self.spb_Ch0.value() + 0.01)

    def jogUpCh1(self):
        self.spb_Ch1.setValue(self.spb_Ch1.value() + 0.01)

    def jogDownCh0(self):
        self.spb_Ch0.setValue(self.spb_Ch0.value() - 0.01)

    def jogDownCh1(self):
        self.spb_Ch1.setValue(self.spb_Ch1.value() - 0.01)

    def sendSingleUpdate(self, x, y):
        """
        Function to send single updates to mirror server.
        Python does not hold on to the socket after it is closed.
        """
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((self.host, self.port))
        mess = b'Single'
        suc = client.sendall(b'06')
        suc = client.sendall(mess)
        data = (str(x) + ';' + str(y)).encode('utf-8')
        # Server expects two bytes
        if len(str(len(data)).encode('utf-8')) < 2:
            mess = ('0' + str(len(data))).encode('utf-8')
        else:
            mess = str(len(data)).encode('utf-8')

        suc = client.sendall(mess)
        suc = client.sendall(data)
        client.close()

    def getValues(self):
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((self.host, self.port))
        mess = b'Values'
        suc = client.sendall(b'06')
        suc = client.sendall(mess)
        data = client.recv(2)
        data = client.recv(int.from_bytes(data, byteorder='little')).decode('utf-8').split(';')
        client.close()
        self.spb_Ch0.setValue(float(data[0]))
        self.spb_Ch1.setValue(float(data[1]))


    # def closeEvent(self, event):
    #     print("closing")


if __name__ == '__main__':
    import sys
    app = QtWidgets.QApplication(sys.argv)
    mir = Mirror()
    mir.show()
    sys.exit(app.exec_())
    
