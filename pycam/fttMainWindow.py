# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file './../src/fttMainWindow.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(916, 811)
        self.centralWidget = QtWidgets.QWidget(MainWindow)
        self.centralWidget.setObjectName("centralWidget")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.centralWidget)
        self.horizontalLayout.setContentsMargins(11, 11, 11, 11)
        self.horizontalLayout.setSpacing(6)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.mpl_window = QtWidgets.QWidget(self.centralWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.mpl_window.sizePolicy().hasHeightForWidth())
        self.mpl_window.setSizePolicy(sizePolicy)
        self.mpl_window.setMinimumSize(QtCore.QSize(512, 512))
        self.mpl_window.setObjectName("mpl_window")
        self.mpl_vl = QtWidgets.QVBoxLayout(self.mpl_window)
        self.mpl_vl.setContentsMargins(11, 11, 11, 11)
        self.mpl_vl.setSpacing(6)
        self.mpl_vl.setObjectName("mpl_vl")
        self.horizontalLayout.addWidget(self.mpl_window)
        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.button_vl = QtWidgets.QVBoxLayout()
        self.button_vl.setContentsMargins(11, 11, 11, 11)
        self.button_vl.setSpacing(6)
        self.button_vl.setObjectName("button_vl")
        self.button_grid = QtWidgets.QGridLayout()
        self.button_grid.setContentsMargins(11, 11, 11, 11)
        self.button_grid.setSpacing(6)
        self.button_grid.setObjectName("button_grid")
        self.btn_ToggleCam = QtWidgets.QPushButton(self.centralWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.btn_ToggleCam.sizePolicy().hasHeightForWidth())
        self.btn_ToggleCam.setSizePolicy(sizePolicy)
        self.btn_ToggleCam.setMaximumSize(QtCore.QSize(16777215, 25))
        self.btn_ToggleCam.setObjectName("btn_ToggleCam")
        self.button_grid.addWidget(self.btn_ToggleCam, 4, 0, 1, 1)
        self.rad_ToggleServo = QtWidgets.QRadioButton(self.centralWidget)
        self.rad_ToggleServo.setObjectName("rad_ToggleServo")
        self.button_grid.addWidget(self.rad_ToggleServo, 4, 1, 1, 1)
        self.button_vl.addLayout(self.button_grid)
        self.groupBoxExposure = QtWidgets.QGroupBox(self.centralWidget)
        self.groupBoxExposure.setObjectName("groupBoxExposure")
        self.gridLayout = QtWidgets.QGridLayout(self.groupBoxExposure)
        self.gridLayout.setContentsMargins(11, 11, 11, 11)
        self.gridLayout.setSpacing(6)
        self.gridLayout.setObjectName("gridLayout")
        spacerItem1 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem1, 0, 0, 1, 1)
        self.spb_EMGain = QtWidgets.QSpinBox(self.groupBoxExposure)
        self.spb_EMGain.setMinimum(1)
        self.spb_EMGain.setMaximum(300)
        self.spb_EMGain.setObjectName("spb_EMGain")
        self.gridLayout.addWidget(self.spb_EMGain, 2, 3, 1, 1)
        self.lbl_ExpTime = QtWidgets.QLabel(self.groupBoxExposure)
        self.lbl_ExpTime.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_ExpTime.setObjectName("lbl_ExpTime")
        self.gridLayout.addWidget(self.lbl_ExpTime, 0, 1, 1, 1)
        spacerItem2 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem2, 2, 0, 1, 1)
        self.lbl_EMGain = QtWidgets.QLabel(self.groupBoxExposure)
        self.lbl_EMGain.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_EMGain.setObjectName("lbl_EMGain")
        self.gridLayout.addWidget(self.lbl_EMGain, 2, 1, 1, 1)
        self.btn_SetExp = QtWidgets.QPushButton(self.groupBoxExposure)
        self.btn_SetExp.setObjectName("btn_SetExp")
        self.gridLayout.addWidget(self.btn_SetExp, 5, 0, 1, 4)
        self.spb_ExpTime = QtWidgets.QDoubleSpinBox(self.groupBoxExposure)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.spb_ExpTime.sizePolicy().hasHeightForWidth())
        self.spb_ExpTime.setSizePolicy(sizePolicy)
        self.spb_ExpTime.setMaximumSize(QtCore.QSize(16777215, 25))
        self.spb_ExpTime.setDecimals(3)
        self.spb_ExpTime.setMaximum(10.0)
        self.spb_ExpTime.setSingleStep(0.001)
        self.spb_ExpTime.setProperty("value", 0.1)
        self.spb_ExpTime.setObjectName("spb_ExpTime")
        self.gridLayout.addWidget(self.spb_ExpTime, 0, 3, 1, 1)
        self.button_vl.addWidget(self.groupBoxExposure)
        self.groupBoxFrame = QtWidgets.QGroupBox(self.centralWidget)
        self.groupBoxFrame.setObjectName("groupBoxFrame")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout(self.groupBoxFrame)
        self.verticalLayout_2.setContentsMargins(11, 11, 11, 11)
        self.verticalLayout_2.setSpacing(6)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.glFrame = QtWidgets.QGridLayout()
        self.glFrame.setContentsMargins(11, 11, 11, 11)
        self.glFrame.setSpacing(6)
        self.glFrame.setObjectName("glFrame")
        self.lbl_Offset = QtWidgets.QLabel(self.groupBoxFrame)
        self.lbl_Offset.setObjectName("lbl_Offset")
        self.glFrame.addWidget(self.lbl_Offset, 2, 0, 1, 1)
        self.lbl_Y = QtWidgets.QLabel(self.groupBoxFrame)
        self.lbl_Y.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_Y.setObjectName("lbl_Y")
        self.glFrame.addWidget(self.lbl_Y, 0, 2, 1, 1)
        self.spb_Dim = QtWidgets.QSpinBox(self.groupBoxFrame)
        self.spb_Dim.setMinimum(1)
        self.spb_Dim.setMaximum(512)
        self.spb_Dim.setProperty("value", 512)
        self.spb_Dim.setObjectName("spb_Dim")
        self.glFrame.addWidget(self.spb_Dim, 1, 1, 1, 2)
        self.lbl_Dimension = QtWidgets.QLabel(self.groupBoxFrame)
        self.lbl_Dimension.setObjectName("lbl_Dimension")
        self.glFrame.addWidget(self.lbl_Dimension, 1, 0, 1, 1)
        self.lbl_X = QtWidgets.QLabel(self.groupBoxFrame)
        self.lbl_X.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_X.setObjectName("lbl_X")
        self.glFrame.addWidget(self.lbl_X, 0, 1, 1, 1)
        self.spb_XCnt = QtWidgets.QSpinBox(self.groupBoxFrame)
        self.spb_XCnt.setMinimum(1)
        self.spb_XCnt.setMaximum(512)
        self.spb_XCnt.setObjectName("spb_XCnt")
        self.glFrame.addWidget(self.spb_XCnt, 2, 1, 1, 1)
        self.spb_YCnt = QtWidgets.QSpinBox(self.groupBoxFrame)
        self.spb_YCnt.setMinimum(1)
        self.spb_YCnt.setMaximum(512)
        self.spb_YCnt.setObjectName("spb_YCnt")
        self.glFrame.addWidget(self.spb_YCnt, 2, 2, 1, 1)
        self.verticalLayout_2.addLayout(self.glFrame)
        self.btn_SetFrame = QtWidgets.QPushButton(self.groupBoxFrame)
        self.btn_SetFrame.setObjectName("btn_SetFrame")
        self.verticalLayout_2.addWidget(self.btn_SetFrame)
        self.button_vl.addWidget(self.groupBoxFrame)
        self.groupBoxTemp = QtWidgets.QGroupBox(self.centralWidget)
        self.groupBoxTemp.setObjectName("groupBoxTemp")
        self.gridLayout_2 = QtWidgets.QGridLayout(self.groupBoxTemp)
        self.gridLayout_2.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_2.setSpacing(6)
        self.gridLayout_2.setObjectName("gridLayout_2")
        spacerItem3 = QtWidgets.QSpacerItem(18, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout_2.addItem(spacerItem3, 1, 0, 1, 1)
        self.lbl_SetPoint = QtWidgets.QLabel(self.groupBoxTemp)
        self.lbl_SetPoint.setObjectName("lbl_SetPoint")
        self.gridLayout_2.addWidget(self.lbl_SetPoint, 0, 1, 1, 1)
        self.lbl_ArrayTemp = QtWidgets.QLabel(self.groupBoxTemp)
        self.lbl_ArrayTemp.setObjectName("lbl_ArrayTemp")
        self.gridLayout_2.addWidget(self.lbl_ArrayTemp, 1, 1, 1, 1)
        spacerItem4 = QtWidgets.QSpacerItem(18, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout_2.addItem(spacerItem4, 0, 0, 1, 1)
        self.spb_ArrayTemp = QtWidgets.QSpinBox(self.groupBoxTemp)
        self.spb_ArrayTemp.setEnabled(False)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.spb_ArrayTemp.sizePolicy().hasHeightForWidth())
        self.spb_ArrayTemp.setSizePolicy(sizePolicy)
        self.spb_ArrayTemp.setButtonSymbols(QtWidgets.QAbstractSpinBox.NoButtons)
        self.spb_ArrayTemp.setMinimum(-273)
        self.spb_ArrayTemp.setMaximum(273)
        self.spb_ArrayTemp.setObjectName("spb_ArrayTemp")
        self.gridLayout_2.addWidget(self.spb_ArrayTemp, 1, 2, 1, 1)
        self.lbl_TempStatus = QtWidgets.QLabel(self.groupBoxTemp)
        self.lbl_TempStatus.setObjectName("lbl_TempStatus")
        self.gridLayout_2.addWidget(self.lbl_TempStatus, 2, 1, 1, 1)
        self.spb_SetPoint = QtWidgets.QSpinBox(self.groupBoxTemp)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.spb_SetPoint.sizePolicy().hasHeightForWidth())
        self.spb_SetPoint.setSizePolicy(sizePolicy)
        self.spb_SetPoint.setMinimum(-100)
        self.spb_SetPoint.setMaximum(30)
        self.spb_SetPoint.setObjectName("spb_SetPoint")
        self.gridLayout_2.addWidget(self.spb_SetPoint, 0, 2, 1, 1)
        self.edt_TempStatus = QtWidgets.QLineEdit(self.groupBoxTemp)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.edt_TempStatus.sizePolicy().hasHeightForWidth())
        self.edt_TempStatus.setSizePolicy(sizePolicy)
        self.edt_TempStatus.setReadOnly(True)
        self.edt_TempStatus.setObjectName("edt_TempStatus")
        self.gridLayout_2.addWidget(self.edt_TempStatus, 2, 2, 1, 1)
        spacerItem5 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout_2.addItem(spacerItem5, 2, 0, 1, 1)
        self.btn_SetTemp = QtWidgets.QPushButton(self.groupBoxTemp)
        self.btn_SetTemp.setObjectName("btn_SetTemp")
        self.gridLayout_2.addWidget(self.btn_SetTemp, 3, 0, 1, 3)
        self.button_vl.addWidget(self.groupBoxTemp)
        self.btn_Gain = QtWidgets.QPushButton(self.centralWidget)
        self.btn_Gain.setObjectName("btn_Gain")
        self.button_vl.addWidget(self.btn_Gain)
        self.btn_OnFiber = QtWidgets.QPushButton(self.centralWidget)
        self.btn_OnFiber.setObjectName("btn_OnFiber")
        self.button_vl.addWidget(self.btn_OnFiber)
        self.btn_Mirror = QtWidgets.QPushButton(self.centralWidget)
        self.btn_Mirror.setObjectName("btn_Mirror")
        self.button_vl.addWidget(self.btn_Mirror)
        self.logUpdateText = QtWidgets.QPlainTextEdit(self.centralWidget)
        self.logUpdateText.setObjectName("logUpdateText")
        self.button_vl.addWidget(self.logUpdateText)
        self.horizontalLayout.addLayout(self.button_vl)
        MainWindow.setCentralWidget(self.centralWidget)
        self.menuBar = QtWidgets.QMenuBar(MainWindow)
        self.menuBar.setGeometry(QtCore.QRect(0, 0, 916, 21))
        self.menuBar.setObjectName("menuBar")
        self.menuFile = QtWidgets.QMenu(self.menuBar)
        self.menuFile.setObjectName("menuFile")
        self.menuSettings = QtWidgets.QMenu(self.menuBar)
        self.menuSettings.setObjectName("menuSettings")
        self.menuLogging = QtWidgets.QMenu(self.menuSettings)
        self.menuLogging.setObjectName("menuLogging")
        self.menuAdvanced = QtWidgets.QMenu(self.menuBar)
        self.menuAdvanced.setObjectName("menuAdvanced")
        MainWindow.setMenuBar(self.menuBar)
        self.mainToolBar = QtWidgets.QToolBar(MainWindow)
        self.mainToolBar.setObjectName("mainToolBar")
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.mainToolBar)
        self.statusBar = QtWidgets.QStatusBar(MainWindow)
        self.statusBar.setObjectName("statusBar")
        MainWindow.setStatusBar(self.statusBar)
        self.actionClose = QtWidgets.QAction(MainWindow)
        self.actionClose.setObjectName("actionClose")
        self.actionServo = QtWidgets.QAction(MainWindow)
        self.actionServo.setObjectName("actionServo")
        self.actionMirror = QtWidgets.QAction(MainWindow)
        self.actionMirror.setObjectName("actionMirror")
        self.actionFrame = QtWidgets.QAction(MainWindow)
        self.actionFrame.setObjectName("actionFrame")
        self.actionTiming = QtWidgets.QAction(MainWindow)
        self.actionTiming.setObjectName("actionTiming")
        self.actionTemperature = QtWidgets.QAction(MainWindow)
        self.actionTemperature.setObjectName("actionTemperature")
        self.actionRead_mode = QtWidgets.QAction(MainWindow)
        self.actionRead_mode.setObjectName("actionRead_mode")
        self.actionTiming_2 = QtWidgets.QAction(MainWindow)
        self.actionTiming_2.setObjectName("actionTiming_2")
        self.actionLogEnable = QtWidgets.QAction(MainWindow)
        self.actionLogEnable.setCheckable(True)
        self.actionLogEnable.setObjectName("actionLogEnable")
        self.actionLogIntv = QtWidgets.QAction(MainWindow)
        self.actionLogIntv.setObjectName("actionLogIntv")
        self.actionFiberLocation = QtWidgets.QAction(MainWindow)
        self.actionFiberLocation.setObjectName("actionFiberLocation")
        self.menuFile.addAction(self.actionClose)
        self.menuLogging.addAction(self.actionLogEnable)
        self.menuLogging.addAction(self.actionLogIntv)
        self.menuSettings.addAction(self.menuLogging.menuAction())
        self.menuSettings.addAction(self.actionTemperature)
        self.menuSettings.addAction(self.actionFrame)
        self.menuSettings.addAction(self.actionServo)
        self.menuSettings.addAction(self.actionMirror)
        self.menuAdvanced.addAction(self.actionRead_mode)
        self.menuAdvanced.addAction(self.actionTiming_2)
        self.menuAdvanced.addAction(self.actionFiberLocation)
        self.menuBar.addAction(self.menuFile.menuAction())
        self.menuBar.addAction(self.menuSettings.menuAction())
        self.menuBar.addAction(self.menuAdvanced.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "EXPRES FTT"))
        self.btn_ToggleCam.setToolTip(_translate("MainWindow", "Start full frame exposures"))
        self.btn_ToggleCam.setText(_translate("MainWindow", "Start"))
        self.rad_ToggleServo.setText(_translate("MainWindow", "Servo"))
        self.groupBoxExposure.setTitle(_translate("MainWindow", "Exposure Parameters"))
        self.spb_EMGain.setToolTip(_translate("MainWindow", "<html><head/><body><p>Electron-multiplying gain. Range of valid values are 1-300.</p></body></html>"))
        self.lbl_ExpTime.setText(_translate("MainWindow", "Exp. Time [s]"))
        self.lbl_EMGain.setText(_translate("MainWindow", "EM Gain"))
        self.btn_SetExp.setText(_translate("MainWindow", "Set Exposure"))
        self.spb_ExpTime.setToolTip(_translate("MainWindow", "<html><head/><body><p>Integration time in milliseconds. Value will update with actual time.</p></body></html>"))
        self.groupBoxFrame.setTitle(_translate("MainWindow", "Frame Parameters"))
        self.lbl_Offset.setText(_translate("MainWindow", "Center"))
        self.lbl_Y.setText(_translate("MainWindow", "Y"))
        self.spb_Dim.setToolTip(_translate("MainWindow", "Horizontal dimension"))
        self.lbl_Dimension.setText(_translate("MainWindow", "Dimension"))
        self.lbl_X.setText(_translate("MainWindow", "X"))
        self.spb_XCnt.setToolTip(_translate("MainWindow", "ROI horizontal offset"))
        self.spb_YCnt.setToolTip(_translate("MainWindow", "ROI vertical offset"))
        self.btn_SetFrame.setToolTip(_translate("MainWindow", "Update camera frame"))
        self.btn_SetFrame.setText(_translate("MainWindow", "Set Frame"))
        self.groupBoxTemp.setTitle(_translate("MainWindow", "Temperature"))
        self.lbl_SetPoint.setText(_translate("MainWindow", "Set Point"))
        self.lbl_ArrayTemp.setText(_translate("MainWindow", "Array Temp."))
        self.spb_ArrayTemp.setToolTip(_translate("MainWindow", "<html><head/><body><p>Actual array temperature in C.</p></body></html>"))
        self.lbl_TempStatus.setText(_translate("MainWindow", "Status"))
        self.spb_SetPoint.setToolTip(_translate("MainWindow", "<html><head/><body><p>Target array temperature in C.</p></body></html>"))
        self.btn_SetTemp.setText(_translate("MainWindow", "Set Temp"))
        self.btn_Gain.setText(_translate("MainWindow", "Gain"))
        self.btn_OnFiber.setText(_translate("MainWindow", "Center"))
        self.btn_Mirror.setText(_translate("MainWindow", "Mirror"))
        self.menuFile.setTitle(_translate("MainWindow", "File"))
        self.menuSettings.setTitle(_translate("MainWindow", "Settings"))
        self.menuLogging.setTitle(_translate("MainWindow", "Logging"))
        self.menuAdvanced.setTitle(_translate("MainWindow", "Advanced"))
        self.actionClose.setText(_translate("MainWindow", "Close"))
        self.actionServo.setText(_translate("MainWindow", "Servo"))
        self.actionMirror.setText(_translate("MainWindow", "Mirror"))
        self.actionFrame.setText(_translate("MainWindow", "Frame"))
        self.actionTiming.setText(_translate("MainWindow", "Timing"))
        self.actionTemperature.setText(_translate("MainWindow", "Temperature"))
        self.actionRead_mode.setText(_translate("MainWindow", "Read mode"))
        self.actionTiming_2.setText(_translate("MainWindow", "Timing"))
        self.actionLogEnable.setText(_translate("MainWindow", "On"))
        self.actionLogIntv.setText(_translate("MainWindow", "Interval"))
        self.actionFiberLocation.setText(_translate("MainWindow", "Fiber location"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())

