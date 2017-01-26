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

import time


class AppWindow(Ui_MainWindow):
	"""
	Main window class, inherits Ui
	"""
	def __init__(self, mainwindow):
		self.setupUi(mainwindow)

		# Initialize camera, data buffer, and data timer
		name = "FTT"
		self.camera = pycamera.PyCamera(name, False)
		self.imageDim = self.camera.getImageDimension()
		self.exposureProp = self.camera.getExposureSettings()
		# self.controlProp = self.camera.getControlProps()
		self.buffer = np.empty((262144,1), dtype='int')

		# Timer to grab camera data and variab
		self.cam_timer = QtCore.QTimer()
		self.cam_timer.setInterval(100)  # [ms]

		# Time format for logging purposes
		self.timeFormat = '%Y-%m-%dz%H:%M:%S'

		self.connectSlots()
		self.setupImageDisplay()

		#
		# Setup image display for FTT camera
		#
	def setupImageDisplay(self):
		fig = Figure(edgecolor='k')
		matplotlib.rcParams.update({'font.size': 10})

		self.axes_err = fig.add_axes([0.075, 0.05, 0.775, 0.1], axis_bgcolor='k')  # error plot
		self.axes_ftt = fig.add_axes([0.075, 0.175, 0.775, 0.675], frame_on=True)  # FTT image frame
		self.axes_vhist = fig.add_axes([0.075, 0.875, 0.775, 0.1], axis_bgcolor='k')  # vertical histogrom
		self.axes_hhist = fig.add_axes([0.875, 0.175, 0.1, 0.675], axis_bgcolor='k')  # horizontal histogram

		self.buffer = np.empty((self.imageDim['size'], 1), dtype='int')
		self.xlimits = np.linspace(
							0, self.camera.x_dim - 1, self.camera.x_dim)
		self.ylimits = np.linspace(
							0, self.camera.y_dim - 1, self.camera.y_dim)

		# The image
		self.axes_ftt.imshow(
					self.buffer.reshape(self.camera.x_dim, self.camera.y_dim),
					aspect='auto', interpolation='none', cmap='gray')
		self.axes_ftt.set_axis_off()

		# The error plots
		self.axes_err.plot(self.xlimits, np.random.rand(self.camera.x_dim,1), color='r')
		self.axes_err.plot(self.ylimits, np.random.rand(self.camera.y_dim,1), color='g')
		self.axes_err.locator_params(axis='y', nbins=3)
		self.axes_err.autoscale('tight')

		# Vertical histogram
		# TODO: check it is the correct axis
		self.axes_vhist.plot(self.xlimits, np.mean(
								self.buffer.reshape(self.camera.x_dim, self.camera.y_dim),
								axis=1), 
							color='w', linewidth=0.5)
		self.axes_vhist.set_xlim(0, self.camera.x_dim - 1)
		self.axes_vhist.locator_params(axis='y', nbins=3)
		self.axes_vhist.set_xticks([])
		self.axes_vhist.set_xticklabels([], visible=False)

		# Right histogram
		# TODO: check it is the correct axis
		self.hsum = self.axes_hhist.plot(np.mean(
								self.buffer.reshape(self.camera.x_dim, self.camera.y_dim), 
								axis=0),
							self.ylimits, color='w', linewidth=0.5)
		self.axes_hhist.set_ylim(0, self.camera.y_dim - 1)
		self.axes_hhist.locator_params(axis='x', nbins=3)
		self.axes_hhist.set_yticklabels([], visible=False)

		self.addMpl(fig)


	def updateDisplay(self):
		# Get camera data with preallocated buffer
		self.camera.data(self.buffer)
		# self.ftt_image.set_data( self.buffer.reshape(self.camera.x_dim, self.camera.y_dim ))
		self.axes_ftt.cla()
		self.axes_ftt.imshow(
					self.buffer.reshape(self.camera.x_dim, self.camera.y_dim),
					aspect='auto', interpolation='none', cmap='gray')
		
		self.axes_vhist.cla()
		self.axes_vhist.plot(self.ylimits, np.mean(self.buffer.reshape(
									self.camera.x_dim,
									self.camera.y_dim),
								axis=0), color='w', linewidth=0.5)
		self.axes_hhist.cla()
		self.axes_hhist.plot(np.mean(self.buffer.reshape(
									self.camera.x_dim,
									self.camera.y_dim), 
								axis=1), self.xlimits, color='w', linewidth=0.5)
		self.canvas.draw()

	def addMpl(self, fig):
		self.canvas = FigureCanvas(fig)
		self.mpl_vl.addWidget(self.canvas)
		self.canvas.draw()

		# Add toolbar that shows coordinates mpl_window as parent 
		self.toolbar = NavigationToolbar(self.canvas, 
				self.mpl_window, coordinates=True)
		self.mpl_vl.addWidget(self.toolbar)

	def rmMpl(self):
		self.mpl_vl.removeWidget(self.canvas)
		self.canvas.close()
		self.mpl_vl.removeWidget(self.toolbar)
		self.toolbar.close()

	def connectSlots(self):
		self.btn_FullFrame.clicked.connect(self.btnFullFrameClicked)
		self.btn_SubFrame.clicked.connect(self.btnSubFrameClicked)
		self.btn_Closed.clicked.connect(self.btnClosedClicked)
		self.btn_Abort.clicked.connect(self.btnAbortClicked)
		self.btn_SetFrame.clicked.connect(self.btnSetFrameClicked)
		self.cam_timer.timeout.connect(self.updateDisplay)


	def btnFullFrameClicked(self):
		if  self.camera.running(): 
			self.logUpdate("Exposure sequence already STARTED")
		else:
			# set frame size to full frame, click 'setFrame', start camera
			self.spb_XOffs.setValue(1)
			self.spb_YOffs.setValue(1)
			self.spb_XDim.setValue(512)
			self.spb_YDim.setValue(512)
			self.spb_XBin.setValue(1)
			self.spb_YBin.setValue(1)
			self.btn_SetFrame.click()
			self.camera.start()
			self.cam_timer.start()
			self.logUpdate("Full frame started {}".format(time.strftime(self.timeFormat,time.gmtime())))


	def btnClosedClicked(self):
		pass


	def btnSubFrameClicked(self):
		if  self.camera.running(): 
			self.logUpdate("Exposure sequence already STARTED")
		else:
			self.camera.start()
			self.cam_timer.start()
			self.logUpdate("Sub frame started {}".format(time.strftime(self.timeFormat,time.gmtime())))

	def btnAbortClicked(self):
		if self.camera.running():
			self.camera.stop()
			self.cam_timer.stop()
			self.logUpdate("Exposure sequence stopped {}".format(time.strftime(self.timeFormat,time.gmtime())))
		else: self.logUpdate("Exposure sequence already STOPPED")


	def btnSetFrameClicked(self):
		self.imageDim['v_start'] = self.spb_XOffs.value()
		self.imageDim['h_start'] = self.spb_YOffs.value()
		self.imageDim['v_end'] = self.spb_XOffs.value() + self.spb_XDim.value() - 1
		self.imageDim['h_end'] = self.spb_YOffs.value() + self.spb_YDim.value() - 1
		self.imageDim['v_bin'] = self.spb_XBin.value()
		self.imageDim['h_bin'] = self.spb_YBin.value()
		# self.imageDim['size'] = (self.spb_XDim.value() * self.spb_YDim.value() /
		# 				 (self.camera.s_imageDim['h_bin'] * self.camera.s_imageDim['v_bin']))
		
		# Program camera
		self.camera.setImageDimension(self.imageDim)
		self.imageDim = self.camera.getImageDimension()
		self.logUpdate("Setting frame parameters")
		self.logUpdate(self.imageDim)
		self.buffer = np.empty((self.imageDim['size'],1), dtype='int')

		self.rmMpl()
		self.setupImageDisplay()

	def logUpdate(self, value):
		self.logUpdateText.appendPlainText(str(value))


if __name__ == '__main__':
	app = QtWidgets.QApplication(sys.argv)
	MainWindow = QtWidgets.QMainWindow()
	ui = AppWindow(MainWindow)
	MainWindow.show()
	sys.exit(app.exec_())
	

