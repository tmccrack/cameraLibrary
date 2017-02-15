import sys
from pycamera import SClient

from PyQt5 import QtCore, QtWidgets
import numpy as np

import time

class Mirror(QtWidgets.QDialog):
	"""
	Dialog window for manually moving mirror
	"""
	def __init__(self, parent=None):
		super(Mirror, self).__init__()
		# Set up the window
		self.setWindowTitle("Mirror control")
		self.btn_Ch0 = QtWidgets.QPushButton("Ch. 0", parent=self)
		self.btn_Ch0Plus = QtWidgets.QPushButton("+", parent=self)
		self.btn_Ch0Minus = QtWidgets.QPushButton("-", parent=self)
		self.btn_Ch1 = QtWidgets.QPushButton("Ch. 1", parent=self)
		self.btn_Ch1Plus = QtWidgets.QPushButton("+", parent=self)
		self.btn_Ch1Minus = QtWidgets.QPushButton("-", parent=self)
		self.spb_Ch0 = QtWidgets.QDoubleSpinBox(self)
		self.spb_Ch0.setRange(0.0, 10.0)
		self.spb_Ch0.setDecimals(3)
		self.spb_Ch1 = QtWidgets.QDoubleSpinBox(self)
		self.spb_Ch1.setRange(0.0, 10.0)
		self.spb_Ch1.setDecimals(3)
		gl = QtWidgets.QGridLayout(self)
		gl.addWidget(self.spb_Ch0, 0, 0)
		gl.addWidget(self.spb_Ch1, 0, 1)
		gl.addWidget(self.btn_Ch0Plus, 1, 0)
		gl.addWidget(self.btn_Ch0, 2, 0)
		gl.addWidget(self.btn_Ch0Minus, 3, 0)
		gl.addWidget(self.btn_Ch1Plus, 1, 1)
		gl.addWidget(self.btn_Ch1, 2, 1)
		gl.addWidget(self.btn_Ch1Minus, 3, 1)

		# Create client
		self.client = SClient(conn_type='Single')
		self.connectSlots()

	def connectSlots(self):
		self.btn_Ch0.clicked.connect(self.moveMirror)
		self.btn_Ch1.clicked.connect(self.moveMirror)
		self.btn_Ch0Plus.clicked.connect(self.jogUpCh0)
		self.btn_Ch0Minus.clicked.connect(self.jogDownCh0)
		self.btn_Ch1Plus.clicked.connect(self.jogUpCh1)
		self.btn_Ch1Minus.clicked.connect(self.jogDownCh1)

	def moveMirror(self):
		if self.client.sendData(self.spb_Ch0.value(),
								self.spb_Ch1.value()):
			self.spb_Ch0.setValue(self.spb_Ch0.value())
			self.spb_Ch1.setValue(self.spb_Ch1.value())
		else:
			print("Connection error")
			# Data didn't transmit, try to update, likely a connection issue
			#try:
			#	arr = self.client.getData()
			#		self.spb_Ch0.setValue(arr[0])
			#		self.spb_Ch1.setValue(arr[1])


	def jogUpCh0(self):
		self.spb_Ch0.setValue(self.spb_Ch0.value() + 0.01)
		self.moveMirror()

	def jogUpCh1(self):
		self.spb_Ch1.setValue(self.spb_Ch1.value() + 0.01)
		self.moveMirror()

	def jogDownCh0(self):
		self.spb_Ch0.setValue(self.spb_Ch0.value() - 0.01)
		self.moveMirror()

	def jogDownCh1(self):
		self.spb_Ch1.setValue(self.spb_Ch1.value() - 0.01)
		self.moveMirror()


if __name__ == '__main__':
	app = QtWidgets.QApplication(sys.argv)
	mir = Mirror()
	mir.show()
	sys.exit(app.exec_())
	
