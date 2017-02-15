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
	imageClick = QtCore.pyqtSignal(float, float)
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
		self.ftt_image = self.axes_ftt.imshow(np.random.rand(512,512), 
											  cmap='gray', 
											  interpolation='none')
		self.axes_ftt.set_axis_off()
		self.axes_ftt.set_aspect('auto')
		self.axes_ftt.hold(False)

		# Summation plots, generate axes and plot random data for initialization
		self.xlims = np.linspace(0, 511, 512)
		self.ylims = np.linspace(0, 511, 512)
		self.axes_vsum.relim()
		self.axes_vsum.autoscale_view()


		self.axes_hsum.relim()
		self.axes_hsum.autoscale_view()

		self.vsum_lines, = self.axes_vsum.plot(self.ylims, np.random.rand(512,1), color='w', linewidth=0.5)
		self.hsum_lines, = self.axes_hsum.plot(np.random.rand(512,1), self.xlims, color='w', linewidth=0.5)

		cid = self.mpl_connect('button_press_event', self.on_press)


	#
	# 
	def on_press(self, event):
		self.imageClick.emit(event.xdata, event.ydata)


	def updateFig(self, buffer):
		"""
		Reshape the linear buffer, show image and summations
		"""
		self.ftt_image.set_data(buffer.reshape(ImageCanvas.iDims['v_dim'],
											ImageCanvas.iDims['h_dim']))
		self.ftt_image.set_clim(vmin=buffer.min(), vmax=buffer.max())

		# Summation in horizontal direction
		self.hsum_lines.set_data(np.mean(buffer.reshape(ImageCanvas.iDims['v_dim'],	
														ImageCanvas.iDims['h_dim']), 
										 axis=1)[::-1], self.ylims)
		self.axes_hsum.relim()
		self.axes_hsum.autoscale_view()
		lab = self.axes_hsum.get_xticks()
		self.axes_hsum.set_xticklabels([lab[0], lab[1], lab[2]], rotation=75)

		# Summation in vertical direction
		self.vsum_lines.set_data(self.xlims, np.mean(buffer.reshape(ImageCanvas.iDims['v_dim'],
																	ImageCanvas.iDims['h_dim']), 
													 axis=0))
		self.axes_vsum.relim()
		self.axes_vsum.autoscale_view()
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

		# self.axes_ftt.set_xbound(0, ImageCanvas.iDims['h_dim']-1)
		# self.axes_ftt.set_ybound(0, ImageCanvas.iDims['v_dim']-1)
		extent = (0, ImageCanvas.iDims['h_dim'] - 1, ImageCanvas.iDims['v_dim'] - 1, 0)
		self.ftt_image.set_extent(extent)


		self.axes_hsum.set_ylim(0, ImageCanvas.iDims['v_dim'] - 1)
		self.axes_hsum.locator_params(axis='x', nbins=3)
		self.axes_hsum.set_yticklabels([], visible=False)

		# self.axes_vsum.set_xlim(0, 511)
		self.axes_vsum.set_xlim(0, ImageCanvas.iDims['h_dim'] - 1)
		self.axes_vsum.locator_params(axis='y', nbins=3)
		self.axes_vsum.set_xticks([])
		self.axes_vsum.set_xticklabels([], visible=False)


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
		self.client = pycamera.SClient(conn_type='Single')
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
		self.camera = pycamera.PyCamera(name, False, temp=17)
		self.imageDim = self.camera.getImageDimension()
		self.expProp = self.camera.getExposureProp()
		self.readProp = self.camera.getReadProp()
		self.timingProp = self.camera.getTimingProp()
		self.shutterProp = self.camera.getShutterProp()
		self.tempProp = self.camera.getTempProp()
		self.gain = self.camera.getGainX()

		# Timers for updates
		self.cam_timer = QtCore.QTimer()
		self.cam_timer.setInterval(500)  # [ms]
		self.temp_timer = QtCore.QTimer()
		self.temp_timer.setInterval(5000)  # [ms]

		# Make set point same as initialized value, to avoid confusion
		self.spb_SetPoint.setValue(self.tempProp['set_point'])

		# Time format for logging purposes
		self.timeFormat = '%Y-%m-%dz%H:%M:%S'

		# Instantiate class, add to gui, initialize display
		self.imageDisp = ImageCanvas(parent=self.mpl_window)
		toolbar = NavigationToolbar(self.imageDisp, self.mpl_window, coordinates=True)
		self.mpl_vl.addWidget(self.imageDisp)
		self.mpl_vl.addWidget(toolbar)
		self.imageDisp.setDimensions(self.imageDim)

		self.connectSlots()
		# Click setframe to initialize data array in cython to avoid 
		# self.btn_SetFrame.click()
		self.temp_timer.start()

	def connectSlots(self):
		self.btn_FullFrame.clicked.connect(self.btnFullFrameClicked)
		self.btn_SubFrame.clicked.connect(self.btnSubFrameClicked)
		self.btn_Closed.clicked.connect(self.btnClosedClicked)
		self.btn_Abort.clicked.connect(self.btnAbortClicked)
		self.btn_SetFrame.clicked.connect(self.btnSetFrameClicked)
		self.btn_SetExp.clicked.connect(self.setExposureProp)
		self.btn_SetTemp.clicked.connect(self.setTempProp)
		self.btn_Gain.clicked.connect(self.btnGainClicked)
		self.imageDisp.imageClick.connect(self.imageClicked)
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
		if  self.camera.running(): 
			self.logUpdate("Exposure sequence already STARTED")
		else:
			self.camera.start(servo = True)
			self.cam_timer.start()
			self.logUpdate("Sub frame started {}".format(time.strftime(self.timeFormat,time.gmtime())))

	def btnAbortClicked(self):
		if self.camera.running():
			self.camera.stop()
			self.cam_timer.stop()
			self.logUpdate("Exposure sequence stopped {}".format(time.strftime(self.timeFormat,time.gmtime())))
		else: self.logUpdate("Exposure sequence already STOPPED")
	
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

	def btnGainClicked(self):
		self.gain['kp'] = self.spb_P.value()
		self.gain['ki'] = self.spb_I.value()
		self.gain['kd'] = self.spb_D.value()
		self.gain['dt'] = self.expProp['exp_time']

		self.gain = self.camera.setGain(self.gain, self.spb_Rotation.value())
		# print("{} {} {}".format(self.gain['kp'], self.gain['ki'], self.gain['kd']))

	def imageClicked(self, targx, targy):
		x, y = self.camera.setTargetCoords(targx, targy)
		self.spb_XFib.setValue(x)
		self.spb_YFib.setValue(y)

	def updateFig(self):
		# Update image dispaly with newest data array, fires with timer timeeout
		self.imageDisp.updateFig(self.camera.data()[0:self.imageDim['size']])

	def setTempProp(self):
		# Set internal temp dict, pass to camera, camera return actual values
		self.tempProp['set_point'] = self.spb_SetPoint.value()
		self.tempProp = self.camera.setTempProp(self.tempProp)

	def updateTemp(self):
		# Update temp dict, set appropriate values
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
	
