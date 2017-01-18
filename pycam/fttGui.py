import sys
import pycamera

from PyQt5 import QtCore, QtWidgets

import matplotlib
matplotlib.use('Qt5Agg')
import numpy as np

from matplotlib.backends.backend_qt5agg import (
	FigureCanvasQTAgg as FigureCanvas,
	NavigationToolbar2QT as NavigationToolbar)

from matplotlib.figure import Figure, SubplotParams
from fttMainWindow import Ui_MainWindow  # pyuic5 generated file


class AppWindow(Ui_MainWindow):
	"""
	Main window class, inherits Ui
	"""
	def __init__(self, mainwindow):
		self.setupUi(mainwindow)

		# Initialize camera, data buffer, and data timer
		name = "FTT"
		self.camera = pycamera.PyCamera(name.encode('utf-8'), False)
		self.buffer = np.empty((262144,1), dtype='int')
		self.cam_timer = QtCore.QTimer()
		self.cam_timer.setInterval(1000)  # [ms]

		self.connectSlots()

		#
		# Setup image display for FTT camera
		#
		self.fig = Figure(edgecolor='k')
		matplotlib.rcParams.update({'font.size': 10})

		self.axes_err = self.fig.add_axes([0.075, 0.05, 0.775, 0.1], axis_bgcolor='k')  # error plot
		self.axes_ftt = self.fig.add_axes([0.075, 0.175, 0.775, 0.675], frame_on=True)  # FTT image frame
		self.axes_vhist = self.fig.add_axes([0.075, 0.875, 0.775, 0.1], axis_bgcolor='k')  # vertical histogrom

		self.axes_hhist = self.fig.add_axes([0.875, 0.175, 0.1, 0.675], axis_bgcolor='k')  # horizontal histogram

		self.buffer = self.buffer.reshape(512,512)
		self.xlimits = np.linspace(0, self.buffer.shape[1]-1, self.buffer.shape[1])
		self.ylimits = np.linspace(0, self.buffer.shape[0]-1, self.buffer.shape[0])
		self.buffer.reshape(262144,1)

		# The image
		self.ftt_image = self.axes_ftt.imshow(self.buffer, aspect='auto', interpolation='none', cmap='gray')
		self.axes_ftt.set_axis_off()

		# The error plots
		self.axes_err.plot(self.xlimits, np.random.rand(512,1), color='r')
		self.axes_err.plot(self.xlimits, np.random.rand(512,1), color='g')
		self.axes_err.locator_params(axis='y', nbins=3)
		self.axes_err.autoscale('tight')

		# Vertical histogram
		# TODO: check it is the correct axis
		self.axes_vhist.plot(self.xlimits, np.mean(self.buffer, axis=1), color='w', linewidth=0.5)
		self.axes_vhist.set_xlim(0, self.buffer.shape[1]-1)
		self.axes_vhist.locator_params(axis='y', nbins=3)
		self.axes_vhist.set_xticks([])
		self.axes_vhist.set_xticklabels([], visible=False)

		# Right histogram
		# TODO: check it is the correct axis
		self.axes_hhist.plot(np.mean(self.buffer, axis=0), self.ylimits, color='w', linewidth=0.5)
		self.axes_hhist.set_ylim(0, self.buffer.shape[0]-1)
		self.axes_hhist.locator_params(axis='x', nbins=3)
		self.axes_hhist.set_yticklabels([], visible=False)

		self.addMpl(self.fig)

	def updateDisplay(self):
		# Get camera data with preallocated buffer
		self.camera.Data(self.buffer)
		self.axes_ftt.imshow(self.buffer.reshape(512,512), interpolation='none', aspect='auto', cmap='gray')
		self.axes_vhist.plot(self.xlimits, np.mean(self.buffer, axis=1), color='w', linewidth=0.5)
		self.axes_hhist.plot(np.mean(self.buffer, axis=0), self.ylimits, color='w', linewidth=0.5)
		self.canvas.draw()

	def addMpl(self, fig):
		self.canvas = FigureCanvas(fig)
		self.mpl_vl.addWidget(self.canvas)
		self.canvas.draw()

		# Add toolbar that shows coordinates mpl_window as parent 
		self.toolbar = NavigationToolbar(self.canvas, 
				self.mpl_window, coordinates=True)
		self.mpl_vl.addWidget(self.toolbar)

	# def rmMpl(self):
	# 	self.mpl_vl.removeWidget(self.canvas)
	# 	self.canvas.close()
	# 	self.mpl_vl.removeWidget(self.toolbar)
	# 	self.toolbar.close()

	def connectSlots(self):
		self.btn_closed.clicked.connect(self.btnClosedClicked)
		self.btn_full_frame.clicked.connect(self.btnFullFrameClicked)
		self.btn_set_roi.clicked.connect(self.btnSetROIClicked)
		self.btn_sub_frame.clicked.connect(self.btnSubFrameClicked)
		self.btn_abort.clicked.connect(self.btnAbortClicked)
		self.cam_timer.timeout.connect(self.updateDisplay)

	def btnClosedClicked(self):
		pass

	def btnFullFrameClicked(self):
		val = self.camera.Start()
		print("{}".format(val))
		self.cam_timer.start()

	def btnSubFrameClicked(self):
		pass

	def btnAbortClicked(self):
		val = self.camera.Stop()
		print("{}".format(val))
		self.cam_timer.stop()

	def btnSetROIClicked(self):
		pass


if __name__ == '__main__':
	app = QtWidgets.QApplication(sys.argv)
	MainWindow = QtWidgets.QMainWindow()
	ui = AppWindow(MainWindow)
	MainWindow.show()
	sys.exit(app.exec_())
	

