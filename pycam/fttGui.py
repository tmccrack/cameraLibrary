import sys
import pycamera

from PyQt5 import QtCore, QtWidgets

import matplotlib
matplotlib.use('Qt5Agg')
import numpy as np

from matplotlib.backends.backend_qt5agg import (
	FigureCanvasQTAgg as FigureCanvas,
	NavigationToolbar2QT as NavigationToolbar)

from matplotlib.figure import Figure
from matplotlib.image import AxesImage
from fttMainWindow import Ui_MainWindow  # pyuic5 generated file

import time


class ImageCanvas(FigureCanvas):
	"""
	FigureCanvasQtAgg that is ultimately a QWidget
	"""
	iDims = {}
	def __init__(self, parent=None):
		fig = Figure()
		super(ImageCanvas, self).__init__(fig)
		matplotlib.rcParams.update({'font.size': 10})

		# Generate axes on the figure
		self.axes_err = fig.add_axes(				 
									 [0.075, 0.05, 0.775, 0.1], 
									 axis_bgcolor='k') # error plot
		self.axes_ftt = fig.add_axes(
									 [0.075, 0.175, 0.775, 0.675], 
									 frame_on=True)  # FTT image frame
		self.axes_vsum = fig.add_axes(
									  [0.075, 0.875, 0.775, 0.1], 
									  axis_bgcolor='k')  # vertical summaiton
		self.axes_hsum = fig.add_axes(
									  [0.875, 0.175, 0.1, 0.675], 
									  axis_bgcolor='k')  # horizontal summation

		# The image, plot random data for initialization
		self.ftt_image = self.axes_ftt.imshow(np.random.rand(50,50), 
											  cmap='gray', 
											  interpolation='none')
		self.axes_ftt.set_axis_off()
		self.axes_ftt.set_aspect('auto')
		self.axes_ftt.hold(False)

		# Summation plots, generate axes and plot random data for initialization
		self.xlims = np.linspace(0, 49, 50)
		self.ylims = np.linspace(0, 49, 50)
		self.axes_vsum.relim()
		self.axes_vsum.autoscale_view()
		# self.axes_vsum.set_autoscaley_on(True)

		self.axes_hsum.relim()
		self.axes_hsum.autoscale_view()
		# self.axes_hsum.set_autoscalex_on(True)

		self.vsum_lines, = self.axes_vsum.plot(self.ylims, np.random.rand(50,1), color='r', linewidth=0.5)
		self.hsum_lines, = self.axes_hsum.plot(np.random.rand(50,1), self.xlims, color='g', linewidth=0.5)
		# self.axes_hsum.set_autoscalex_on(True)



	def updateFig(self, buffer):
		"""
		Reshape the linear buffer, show image and summations
		"""
		self.ftt_image.set_data(buffer.reshape(ImageCanvas.iDims['v_dim'],
											ImageCanvas.iDims['h_dim']))
		# self.ftt_image.set_data(np.random.rand(ImageCanvas.iDims['h_dim'],
		# 									   ImageCanvas.iDims['v_dim']))
		# self.axes_ftt.imshow(buffer.reshape(ImageCanvas.iDims['h_dim'],
		# 									ImageCanvas.iDims['v_dim']),
		# 							aspect='auto', 
		# 							interpolation='none', 
		# 							cmap='gray')
		self.ftt_image.set_clim(vmin=buffer.min(), vmax=buffer.max())
		self.hsum_lines.set_data(np.mean(buffer.reshape(ImageCanvas.iDims['v_dim'],
														ImageCanvas.iDims['h_dim']), 
										 axis=1), self.ylims)
		self.axes_hsum.relim()
		self.axes_hsum.autoscale_view()
		lab = self.axes_hsum.get_xticks()
		self.axes_hsum.set_xticklabels([lab[0], lab[1], lab[2]], rotation=75)

		# self.axes_vsum.plot(self.ylims, np.sum(buffer.reshape(
		# 								ImageCanvas.iDims['h_dim'],
		# 								ImageCanvas.iDims['v_dim']), 
		# 							axis=0), 
		# 						color='w', 
		# 						linewidth=0.5)
		self.vsum_lines.set_data(self.xlims, np.mean(buffer.reshape(ImageCanvas.iDims['v_dim'],
																	ImageCanvas.iDims['h_dim']), 
													 axis=0))
		self.axes_vsum.relim()
		self.axes_vsum.autoscale_view()
		# self.axes_hsum.plot(np.sum(buffer.reshape(
		# 								ImageCanvas.iDims['h_dim'],
		# 								ImageCanvas.iDims['v_dim']), 
		# 							axis=1), 
		# 						self.xlims, 
		# 						color='w', 
		# 						linewidth=0.5)
		# self.axes_err.plot(np.linspace(0,49,50), np.random.rand(50,1), 'g')
		self.draw()


	def setDimensions(self, iDims):
		"""
		Update image dimensions for the figure display, then update axes
		"""
		ImageCanvas.iDims = iDims
		self.setAxes()

	def setAxes(self):
		"""
		Update axes based on image dimensions
		"""
		self.xlims = np.linspace(0, ImageCanvas.iDims['h_dim'] - 1,
									ImageCanvas.iDims['h_dim'])
		self.ylims = np.linspace(0, ImageCanvas.iDims['v_dim'] - 1,
									ImageCanvas.iDims['v_dim'])

		# self.axes_hsum.set_ylim(0, 511)
		self.axes_hsum.set_ylim(0, ImageCanvas.iDims['v_dim'] - 1)
		self.axes_hsum.locator_params(axis='x', nbins=3)
		self.axes_hsum.set_yticklabels([], visible=False)

		# self.axes_vsum.set_xlim(0, 511)
		self.axes_vsum.set_xlim(0, ImageCanvas.iDims['h_dim'] - 1)
		self.axes_vsum.locator_params(axis='y', nbins=3)
		self.axes_vsum.set_xticks([])
		self.axes_vsum.set_xticklabels([], visible=False)


class AppWindow(Ui_MainWindow):
	"""
	Main window class, inherits Ui
	"""
	d_temp = {'20034': 'Off',
			  '20035': 'Not stable',
			  '20036': 'Stable',
			  '20037': 'Not reached',
			  '20040': 'Drift',
			  '20072': 'Acquiring',
			  '0': 'Fake cam'}

	def __init__(self, mainwindow):
		self.setupUi(mainwindow)

		# Initialize camera, data buffer, and data timer
		name = "FTT"
		self.camera = pycamera.PyCamera(name, True, temp=17)
		self.imageDim = self.camera.getImageDimension()
		self.expProp = self.camera.getExposureProp()
		self.tempProp = self.camera.getTempProp()
		# self.controlProp = self.camera.getControlProps()
		# self.buffer = np.empty((262144,1), dtype='int')

		# Timers for updates
		self.cam_timer = QtCore.QTimer()
		self.cam_timer.setInterval(100)  # [ms]
		self.temp_timer = QtCore.QTimer()
		self.temp_timer.setInterval(5000)  # [ms]

		# Make set point same as initialized value, to avoid confusion
		self.spb_SetPoint.setValue(self.tempProp['set_point'])


		# input()


		# Time format for logging purposes
		self.timeFormat = '%Y-%m-%dz%H:%M:%S'

		# fig = Figure(edgecolor='k')
		self.imageDisp = ImageCanvas(parent=self.mpl_window)
		toolbar = NavigationToolbar(self.imageDisp, self.mpl_window, coordinates=True)
		self.mpl_vl.addWidget(self.imageDisp)
		self.mpl_vl.addWidget(toolbar)
		self.imageDisp.setDimensions(self.imageDim)
		# self.imageDisp.updateFig(n)

		self.connectSlots()
		self.temp_timer.start()

	def connectSlots(self):
		self.btn_FullFrame.clicked.connect(self.btnFullFrameClicked)
		self.btn_SubFrame.clicked.connect(self.btnSubFrameClicked)
		self.btn_Closed.clicked.connect(self.btnClosedClicked)
		self.btn_Abort.clicked.connect(self.btnAbortClicked)
		self.btn_SetFrame.clicked.connect(self.btnSetFrameClicked)
		self.btn_SetExp.clicked.connect(self.setExposureProp)
		self.btn_SetTemp.clicked.connect(self.setTempProp)
		self.cam_timer.timeout.connect(self.updateFig)
		self.temp_timer.timeout.connect(self.updateTemp)


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

	def btnSubFrameClicked(self):
		if  self.camera.running(): 
			self.logUpdate("Exposure sequence already STARTED")
		else:
			self.camera.start()
			self.cam_timer.start()
			self.logUpdate("Sub frame started {}".format(time.strftime(self.timeFormat,time.gmtime())))

	def btnClosedClicked(self):
		pass

	def btnAbortClicked(self):
		if self.camera.running():
			self.camera.stop()
			self.cam_timer.stop()
			self.logUpdate("Exposure sequence stopped {}".format(time.strftime(self.timeFormat,time.gmtime())))
		else: self.logUpdate("Exposure sequence already STOPPED")


	def updateFig(self):
		self.imageDisp.updateFig(self.camera.data()[0:self.imageDim['size']])

	def btnSetFrameClicked(self):
		# Update internal dimension dict
		self.imageDim['h_start'] = self.spb_XOffs.value()
		self.imageDim['v_start'] = self.spb_YOffs.value()
		self.imageDim['h_end'] = self.spb_XOffs.value() + self.spb_XDim.value() - 1
		self.imageDim['v_end'] = self.spb_YOffs.value() + self.spb_YDim.value() - 1
		self.imageDim['v_bin'] = self.spb_XBin.value()
		self.imageDim['h_bin'] = self.spb_YBin.value()
		#
		# TODO: Check binning
		#

		# Pass internal dimension dict to camera, camera returns actual image dimensions
		self.imageDim = self.camera.setImageDimension(self.imageDim)
		self.imageDisp.setDimensions(self.imageDim)

		# Display updates
		self.logUpdate("Setting frame parameters")
		self.logUpdate(self.imageDim)

	def setTempProp(self):
		self.tempProp['set_point'] = self.spb_SetPoint.value()
		self.tempProp = self.camera.setTempProp(self.tempProp)

	def updateTemp(self):
		self.tempProp = self.camera.getTempArray()
		self.spb_ArrayTemp.setValue(self.tempProp['array_temp'])
		self.edt_TempStatus.setText(AppWindow.d_temp[str(self.tempProp['cooler_state'])])


	def setExposureProp(self):
		self.expProp['exp_time'] = self.spb_ExpTime.value()
		self.expProp['em_gain'] = self.spb_EMGain.value()
		# Pass internal exp. dict to camrea, camera returns actual exp. settings
		self.expProp = self.camera.setExposureProp(self.expProp)
		self.spb_ExpTime.setValue(self.expProp['exp_time'])
		self.spb_EMGain.setValue(self.expProp['em_gain'])

	def logUpdate(self, value):
		self.logUpdateText.appendPlainText(str(value))


if __name__ == '__main__':
	app = QtWidgets.QApplication(sys.argv)
	MainWindow = QtWidgets.QMainWindow()
	ui = AppWindow(MainWindow)
	MainWindow.show()
	sys.exit(app.exec_())
	


# def setupImageDisplay(self):
	# 	fig = Figure(edgecolor='k')
	# 	matplotlib.rcParams.update({'font.size': 10})

	# 	self.axes_err = fig.add_axes([0.075, 0.05, 0.775, 0.1], axis_bgcolor='k')  # error plot
	# 	self.axes_ftt = fig.add_axes([0.075, 0.175, 0.775, 0.675], frame_on=True)  # FTT image frame
	# 	self.axes_vhist = fig.add_axes([0.075, 0.875, 0.775, 0.1], axis_bgcolor='k')  # vertical histogrom
	# 	self.axes_hhist = fig.add_axes([0.875, 0.175, 0.1, 0.675], axis_bgcolor='k')  # horizontal histogram

	# 	self.buffer = np.empty((self.imageDim['size'], 1), dtype='int')
	
