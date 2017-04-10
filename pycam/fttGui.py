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
import matplotlib.lines as mlines

from fttMainWindow import Ui_MainWindow  # pyuic5 generated file
from fttServo import Servo as servo
from fttMirror import Mirror as mirror

import time
import logging

logging.basicConfig(filename='./../log/example.log',level=logging.DEBUG)
image_path = "./../data/"

fiber_loc = (256,256)
def setFiberLoc(self, x, y):
        fiber_loc = (x, y)

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
        self.axes_err = fig.add_axes([0.075, 0.05, 0.775, 0.1], axis_bgcolor='k') # error plot
        self.axes_ftt = fig.add_axes([0.075, 0.175, 0.775, 0.675], frame_on=True)  # FTT image frame
        self.axes_vsum = fig.add_axes([0.075, 0.875, 0.775, 0.1], axis_bgcolor='k')  # vertical summaiton
        self.axes_hsum = fig.add_axes([0.875, 0.175, 0.1, 0.675], axis_bgcolor='k')  # horizontal summation
        # The image, plot random data for initialization
        self.ftt_image = self.axes_ftt.imshow(np.random.rand(512,512), cmap='gray', interpolation='none')
        self.axes_ftt.set_axis_off()
        self.axes_ftt.set_aspect('auto')
        self.axes_ftt.hold(False)

        # Summation plots, generate axes and plot random data for initialization
        self.xlims = np.linspace(0, 511, 512)
        self.ylims = np.linspace(0, 511, 512)
        self.errlims = np.linspace(0, 99, 100)

        self.axes_vsum.relim()
        self.axes_vsum.autoscale_view()
        self.axes_hsum.relim()
        self.axes_hsum.autoscale_view()
        self.axes_err.relim()
        self.axes_err.autoscale_view()

        # error buffer
        self.errs = np.zeros((100,2))

        self.vsum_lines, = self.axes_vsum.plot(self.ylims, np.random.rand(512,1), color='w', linewidth=0.5)
        self.hsum_lines, = self.axes_hsum.plot(np.random.rand(512,1), self.xlims, color='w', linewidth=0.5)
        self.xerr_lines, = self.axes_err.plot(self.errlims, self.errs[:,0], color='r') 
        self.yerr_lines, = self.axes_err.plot(self.errlims, self.errs[:,1], color='y')

        # Initialize fiber marker list and add to axes
        self.marker_lines = []
        ratio = (512/12.0, 512/12.0)
        x = np.array([[-ratio[0]*1.5, -ratio[0]*0.5], [ratio[0]*0.5, ratio[0]*1.5], [0.0, 0.0], [0.0, 0.0]]) + fiber_loc
        y = np.array([[0.0, 0.0], [0.0, 0.0], [-ratio[1]*1.5, -ratio[1]*0.5], [ratio[1]*0.5, ratio[1]*1.5]]) + fiber_loc

        for ind in range(len(x)):
            self.marker_lines.append(mlines.Line2D(x[ind], y[ind], color='g', linewidth=2.0)) #.set_data(x[ind], y[ind])

        for line in self.marker_lines:
            self.axes_ftt.add_line(line)

        cid = self.mpl_connect('button_press_event', self.on_press)


    #
    # Update fiber markers for new array size
    def setFiberMarker(self):
        # Settings for updated fiber markers
        # Markers are 1/12 the dimension, offset by half that ratio
        ratio = (ImageCanvas.iDims['h_dim']/12.0, ImageCanvas.iDims['v_dim']/12.0)

        x = np.array([[-ratio[0]*1.5, -ratio[0]*0.5], [ratio[0]*0.5, ratio[0]*1.5], [0.0, 0.0], [0.0, 0.0]]) + fiber_loc
        y = np.array([[0.0, 0.0], [0.0, 0.0], [-ratio[1]*1.5, -ratio[1]*0.5], [ratio[1]*0.5, ratio[1]*1.5]]) + fiber_loc

        for ind in range(len(x)):
            self.marker_lines[ind].set_data(x[ind], y[ind])

    #
    # Image click event
    def on_press(self, event):
        self.imageClick.emit(event.xdata, event.ydata)

    #
    # Update the figure
    def updateFig(self, buffer, vals=None):
        """
        Update the figure with the supplied buffer and values if provided
        Reshape the linear buffer, show image and summations
        """
        self.ftt_image.set_data(buffer.reshape(ImageCanvas.iDims['v_dim'], ImageCanvas.iDims['h_dim']))
        self.ftt_image.set_clim(vmin=buffer.min(), vmax=buffer.max())

        # Mean in horizontal direction
        self.hsum_lines.set_data(np.mean(buffer.reshape(ImageCanvas.iDims['v_dim'],	
                                                        ImageCanvas.iDims['h_dim']),  axis=1)[::-1], self.ylims)
        self.axes_hsum.relim()
        self.axes_hsum.autoscale_view()
        lab = self.axes_hsum.get_xticks()
        self.axes_hsum.set_xticklabels([lab[0], lab[1], lab[2]], rotation=75)

        # Mean in vertical direction
        self.vsum_lines.set_data(self.xlims, np.mean(buffer.reshape(ImageCanvas.iDims['v_dim'],
                                                                ImageCanvas.iDims['h_dim']), axis=0))
        self.axes_vsum.relim()
        self.axes_vsum.autoscale_view()

        # Update error buffer
        if vals is not None:
            self.errs[0,:] = vals
            self.errs = np.roll(self.errs, -1, axis=0)
            self.xerr_lines.set_data(self.errlims, self.errs[:,0])
            self.yerr_lines.set_data(self.errlims, self.errs[:,1])
            self.axes_err.relim()
            self.axes_err.autoscale_view()

        # self.axes_ftt.add_line(self.fiber_marker)
        self.draw()

    def setDimensions(self, iDims):
        """
        Update image dimensions for the figure display, then update axes
        """
        ImageCanvas.iDims = iDims
        self.setAxes()
        self.setFiberMarker()

    def setAxes(self):
        """
        Update axes based on image dimensions
        """
        self.xlims = np.linspace(0, ImageCanvas.iDims['h_dim'] - 1,ImageCanvas.iDims['h_dim'])
        self.ylims = np.linspace(0, ImageCanvas.iDims['v_dim'] - 1,ImageCanvas.iDims['v_dim'])

        # self.axes_ftt.set_xbound(0, ImageCanvas.iDims['h_dim']-1)
        # self.axes_ftt.set_ybound(0, ImageCanvas.iDims['v_dim']-1)
        # extent = (0, ImageCanvas.iDims['h_dim'] - 1, ImageCanvas.iDims['v_dim'] - 1, 0)
        extent = (ImageCanvas.iDims['h_start'], ImageCanvas.iDims['h_end'], 
                  ImageCanvas.iDims['v_start'], ImageCanvas.iDims['v_end'], )
        self.ftt_image.set_extent(extent)

        self.axes_hsum.set_ylim(0, ImageCanvas.iDims['v_dim'] - 1)
        self.axes_hsum.locator_params(axis='x', nbins=3)
        self.axes_hsum.set_yticklabels([], visible=False)

        # self.axes_vsum.set_xlim(0, 511)
        self.axes_vsum.set_xlim(0, ImageCanvas.iDims['h_dim'] - 1)
        self.axes_vsum.locator_params(axis='y', nbins=3)
        self.axes_vsum.set_xticks([])
        self.axes_vsum.set_xticklabels([], visible=False)


class Worker(QtCore.QThread):
    
    def __init__(self, imageDisp, parent=None):
        super(Worker, self).__init__()
        imageDisp.imageClick.connect(self.imageClicked)

        self.timeout = QtCore.QTimer()
        self.timeout.setSingleShot(True)
        self.counter = 0

    def run(self):
        self.counter = 0
        print("Click star")
        print("Click fiber")

    def imageClicked(self, x, y):
        self.counter += 1
        print("X: {}\tY: {}".format(x, y))
        if self.counter > 1:
            print("quitting")
            self.exit()


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
    def __init__(self, mainwindow, camera, mir_win, servo_win):
        super(AppWindow, self).__init__()
        self.setupUi(mainwindow)
        self.camera = camera
        self.mir_win = mir_win
        self.servo_win = servo_win
        self.servo_win.reInitVals(self.camera.getGainX(), 
                                  self.camera.getRotation())

        # Initialize logger
        self.logger = logging.getLogger('')
        self.logging = False

        self.imageDim = self.camera.getImageDimension()
        self.expProp = self.camera.getExposureProp()
        self.readProp = self.camera.getReadProp()
        self.timingProp = self.camera.getTimingProp()
        self.shutterProp = self.camera.getShutterProp()
        self.tempProp = self.camera.getTempProp()
        self.gain = self.camera.getGainX()
        self.coords = self.camera.getTargetCoords()

        # Timers for updates
        self.cam_timer = QtCore.QTimer()
        self.cam_timer.setInterval(500)  # [ms]
        self.temp_timer = QtCore.QTimer()
        self.temp_timer.setInterval(5000)  # [ms]

        # Make set point same as initialized value, to avoid confusion
        self.spb_SetPoint.setValue(self.tempProp['set_point'])

        # Time format for logging purposes
        self.timeFormat = '%Y-%m-%dz%H-%M-%S'

        # Instantiate class, add to gui, initialize display
        self.imageDisp = ImageCanvas()
        toolbar = NavigationToolbar(self.imageDisp, self.mpl_window, coordinates=True)
        self.mpl_vl.addWidget(self.imageDisp)
        self.mpl_vl.addWidget(toolbar)
        self.imageDisp.setDimensions(self.imageDim)

        self.connectSlots()
        # Click setframe to initialize data array in cython
        self.btn_SetFrame.click()
        self.temp_timer.start()

    def onFiberClicked(self):
        a = Worker(self.imageDisp, self.mir_win)
        a.start()


    def connectSlots(self):
        self.btn_ToggleCam.clicked.connect(self.btnToggleCamClicked)
        self.btn_SetFrame.clicked.connect(self.btnSetFrameClicked)
        self.btn_SetExp.clicked.connect(self.setExposureProp)
        self.btn_SetTemp.clicked.connect(self.setTempProp) 
        self.rad_ToggleServo.toggled.connect(self.radToggleServoClicked)
        self.imageDisp.imageClick.connect(self.imageClicked)
        self.cam_timer.timeout.connect(self.updateFig)
        self.temp_timer.timeout.connect(self.updateTemp)

        #
        # Buttons in the mirror window
        self.btn_Mirror.clicked.connect(self.mir_win.show)
        self.mir_win.btn_OnFiber.clicked.connect(self.onFiberClicked)

        #
        # Buttons in the servo window
        self.btn_Gain.clicked.connect(self.btnGainClicked)
        self.servo_win.buttonBox.accepted.connect(self.acceptGain)
        self.servo_win.buttonBox.button(QtWidgets.QDialogButtonBox.Apply).clicked.connect(self.acceptGain)

    def acceptGain(self):
        self.camera.setGain(self.servo_win.gains, self.servo_win.rot)
        self.logUpdate("Setting gain parameters: {} {}".format(self.gain, time.strftime(self.timeFormat,time.gmtime())))
        # self.logUpdate("Target coordinates set to: [{}, {}] {}".format(self.coords[0],self.coords[1],
        #                                                     time.strftime(self.timeFormat,time.gmtime())))

    def btnGainClicked(self):
        self.servo_win.reInitVals(self.camera.getGainX(), self.camera.getRotation())
        self.servo_win.show()
        # self.coords = self.camera.setTargetCoords(self.coords[0], self.coords[1])


    def btnToggleCamClicked(self):
        if  self.camera.running(): 
            self.camera.stop()
            self.cam_timer.stop()
            self.logUpdate("Exposure sequence stopped {}".format(time.strftime(self.timeFormat,time.gmtime())))
            self.btn_ToggleCam.setText('Start')
        elif not self.camera.running():
            if self.logging: 
                log_file = image_path+time.strftime(self.timeFormat, time.gmtime())
                self.camera.start(1, log_file)
                self.logUpdate("Data log at {} with image dim {},{}".format(log_file, self.imageDim['h_dim'],
                                                                                      self.imageDim['v_dim']))
            else:
                self.camera.start(1)

            self.cam_timer.start()
            self.logUpdate("Exposure sequence started {}".format(time.strftime(self.timeFormat,time.gmtime())))

            self.btn_ToggleCam.setText('Stop')


    def radToggleServoClicked(self):
        servoState = self.camera.setServoState(self.rad_ToggleServo.isChecked())
        self.logUpdate("Servo state set {} {}".format(servoState, time.strftime(self.timeFormat,time.gmtime())))
        	
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

        self.logUpdate("Setting frame parameters: {} {}".format(self.imageDim, 
                                                            time.strftime(self.timeFormat,time.gmtime())))


    def imageClicked(self, targx, targy):
        self.coords = (targx, targy)
        self.spb_XFib.setValue(targx)
        self.spb_YFib.setValue(targy)


    def updateFig(self):
        # Update image dispaly with newest data array, fires with timer timeout
        if self.rad_ToggleServo.isChecked():
            self.imageDisp.updateFig(self.camera.data()[0:self.imageDim['size']], self.camera.servoData())
        else:
            self.imageDisp.updateFig(self.camera.data()[0:self.imageDim['size']])


    def setTempProp(self):
        # Set internal temp dict, pass to camera, camera return actual values
        self.tempProp['set_point'] = self.spb_SetPoint.value()
        self.tempProp = self.camera.setTempProp(self.tempProp)
        self.logUpdate("Temp setpoint: {} {}".format(self.tempProp['set_point'],time.strftime(self.timeFormat,time.gmtime())))


    def updateTemp(self):
        # Update temp dict, set appropriate values
        self.tempProp = self.camera.getTempArray()
        self.spb_ArrayTemp.setValue(self.tempProp['array_temp'])
        self.edt_TempStatus.setText(AppWindow.d_temp[str(self.tempProp['cooler_state'])])
        self.logTemp("Camera temp: {} {}".format(self.tempProp['array_temp'],
                                time.strftime(self.timeFormat,time.gmtime())))
  
    def setExposureProp(self):
        self.expProp['exp_time'] = self.spb_ExpTime.value()
        self.expProp['em_gain'] = self.spb_EMGain.value()

        # Pass internal exp. dict to camrea, camera returns actual exp. settings
        self.expProp = self.camera.setExposureProp(self.expProp)
        self.spb_ExpTime.setValue(self.expProp['exp_time'])
        self.spb_EMGain.setValue(self.expProp['em_gain'])
        self.logUpdate("Exp time: {}, Cycle time: {} EMGain: {}, {}".format(self.expProp['exp_time'],
                                               self.expProp['kinetic_time'], self.expProp['em_gain'],
                                               time.strftime(self.timeFormat,time.gmtime())))

    
    def logUpdate(self, value):
        self.logUpdateText.appendPlainText(str(value))
        logging.info(value)

    
    def logTemp(self, value):
        #
        # TODO: replace logging with appropriate database call
        logging.info(value)

    
    # def closeEvent(self, event):
    #     event.ignore()
    #     self.close_application()

    
    # def close_application(self):
    #     print("closing")


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    name = "FTT"
    camera = pycamera.PyCamera(name, False, temp=17)
    serv = servo(MainWindow)
    mir = mirror(MainWindow)
    # imageDisp = ImageCanvas(MainWindow)
    ui = AppWindow(MainWindow, camera, mir, serv)
    MainWindow.show()
    sys.exit(app.exec_())

