import sys
import socket
from PyQt5 import QtCore, QtWidgets
import numpy as np

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

class Mirror(QtWidgets.QDialog):
    """
    Dialog window for manually moving mirror
    """
    def __init__(self, parent=None):
        super(Mirror, self).__init__()
        self.host = 'localhost'
        self.port = 6666

        # Set up the window
        self.setWindowTitle("Mirror control")
        self.lbl_Ch0 = QtWidgets.QLabel("Ch. 0", parent=self)
        self.lbl_Ch0.setAlignment(QtCore.Qt.AlignCenter)
        self.btn_Ch0Plus = QtWidgets.QPushButton("+", parent=self)
        self.btn_Ch0Minus = QtWidgets.QPushButton("-", parent=self)
        self.lbl_Ch1 = QtWidgets.QLabel("Ch. 1", parent=self)
        self.lbl_Ch1.setAlignment(QtCore.Qt.AlignCenter)
        self.btn_Ch1Plus = QtWidgets.QPushButton("+", parent=self)
        self.btn_Ch1Minus = QtWidgets.QPushButton("-", parent=self)
        self.spb_Ch0 = QtWidgets.QDoubleSpinBox(self)
        self.spb_Ch0.setRange(0.0, 5.0)
        self.spb_Ch0.setDecimals(3)
        self.spb_Ch1 = QtWidgets.QDoubleSpinBox(self)
        self.spb_Ch1.setRange(0.0, 5.0)
        self.spb_Ch1.setDecimals(3)
        gl = QtWidgets.QGridLayout(self)
        gl.addWidget(self.spb_Ch0, 0, 0)
        gl.addWidget(self.spb_Ch1, 0, 1)
        gl.addWidget(self.btn_Ch0Plus, 1, 0)
        gl.addWidget(self.lbl_Ch0, 2, 0)
        gl.addWidget(self.btn_Ch0Minus, 3, 0)
        gl.addWidget(self.btn_Ch1Plus, 1, 1)
        gl.addWidget(self.lbl_Ch1, 2, 1)
        gl.addWidget(self.btn_Ch1Minus, 3, 1)

        self.connectSlots()

    def connectSlots(self):
        self.spb_Ch0.valueChanged.connect(self.moveMirror)
        self.spb_Ch1.valueChanged.connect(self.moveMirror)
        self.btn_Ch0Plus.clicked.connect(self.jogUpCh0)
        self.btn_Ch0Minus.clicked.connect(self.jogDownCh0)
        self.btn_Ch1Plus.clicked.connect(self.jogUpCh1)
        self.btn_Ch1Minus.clicked.connect(self.jogDownCh1)

    def moveMirror(self):
        self.sendSingleUpdate(self.spb_Ch0.value(), self.spb_Ch1.value())

    def jogUpCh0(self):
        self.spb_Ch0.setValue(self.spb_Ch0.value() + 0.01)
        # self.moveMirror()

    def jogUpCh1(self):
        self.spb_Ch1.setValue(self.spb_Ch1.value() + 0.01)
        # self.moveMirror()
        # sendSingle('localhost', 6666, self.spb_Ch1.value(), self.spb_Ch1.value())

    def jogDownCh0(self):
        self.spb_Ch0.setValue(self.spb_Ch0.value() - 0.01)
        # self.moveMirror()

    def jogDownCh1(self):
        self.spb_Ch1.setValue(self.spb_Ch1.value() - 0.01)
        # self.moveMirror()

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


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    mir = Mirror()
    mir.show()
    sys.exit(app.exec_())
    
