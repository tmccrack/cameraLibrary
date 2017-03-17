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
        MainWindow.resize(830, 914)
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
        self.radioButton = QtWidgets.QRadioButton(self.centralWidget)
        self.radioButton.setObjectName("radioButton")
        self.button_grid.addWidget(self.radioButton, 5, 0, 1, 1)
        self.button_vl.addLayout(self.button_grid)
        self.groupBoxExposure = QtWidgets.QGroupBox(self.centralWidget)
        self.groupBoxExposure.setObjectName("groupBoxExposure")
        self.gridLayout = QtWidgets.QGridLayout(self.groupBoxExposure)
        self.gridLayout.setContentsMargins(11, 11, 11, 11)
        self.gridLayout.setSpacing(6)
        self.gridLayout.setObjectName("gridLayout")
        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem, 0, 0, 1, 1)
        self.spb_EMGain = QtWidgets.QSpinBox(self.groupBoxExposure)
        self.spb_EMGain.setMinimum(1)
        self.spb_EMGain.setMaximum(300)
        self.spb_EMGain.setObjectName("spb_EMGain")
        self.gridLayout.addWidget(self.spb_EMGain, 2, 3, 1, 1)
        self.lbl_ExpTime = QtWidgets.QLabel(self.groupBoxExposure)
        self.lbl_ExpTime.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_ExpTime.setObjectName("lbl_ExpTime")
        self.gridLayout.addWidget(self.lbl_ExpTime, 0, 1, 1, 1)
        spacerItem1 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem1, 2, 0, 1, 1)
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
        self.spb_XDim = QtWidgets.QSpinBox(self.groupBoxFrame)
        self.spb_XDim.setMinimum(1)
        self.spb_XDim.setMaximum(512)
        self.spb_XDim.setProperty("value", 512)
        self.spb_XDim.setObjectName("spb_XDim")
        self.glFrame.addWidget(self.spb_XDim, 1, 1, 1, 1)
        self.lbl_Dimension = QtWidgets.QLabel(self.groupBoxFrame)
        self.lbl_Dimension.setObjectName("lbl_Dimension")
        self.glFrame.addWidget(self.lbl_Dimension, 1, 0, 1, 1)
        self.lbl_Binning = QtWidgets.QLabel(self.groupBoxFrame)
        self.lbl_Binning.setObjectName("lbl_Binning")
        self.glFrame.addWidget(self.lbl_Binning, 3, 0, 1, 1)
        self.lbl_X = QtWidgets.QLabel(self.groupBoxFrame)
        self.lbl_X.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_X.setObjectName("lbl_X")
        self.glFrame.addWidget(self.lbl_X, 0, 1, 1, 1)
        self.spb_YDim = QtWidgets.QSpinBox(self.groupBoxFrame)
        self.spb_YDim.setMinimum(1)
        self.spb_YDim.setMaximum(512)
        self.spb_YDim.setProperty("value", 512)
        self.spb_YDim.setObjectName("spb_YDim")
        self.glFrame.addWidget(self.spb_YDim, 1, 2, 1, 1)
        self.spb_XOffs = QtWidgets.QSpinBox(self.groupBoxFrame)
        self.spb_XOffs.setMinimum(1)
        self.spb_XOffs.setMaximum(512)
        self.spb_XOffs.setObjectName("spb_XOffs")
        self.glFrame.addWidget(self.spb_XOffs, 2, 1, 1, 1)
        self.spb_YOffs = QtWidgets.QSpinBox(self.groupBoxFrame)
        self.spb_YOffs.setMinimum(1)
        self.spb_YOffs.setMaximum(512)
        self.spb_YOffs.setObjectName("spb_YOffs")
        self.glFrame.addWidget(self.spb_YOffs, 2, 2, 1, 1)
        self.spb_XBin = QtWidgets.QSpinBox(self.groupBoxFrame)
        self.spb_XBin.setMinimum(1)
        self.spb_XBin.setMaximum(512)
        self.spb_XBin.setObjectName("spb_XBin")
        self.glFrame.addWidget(self.spb_XBin, 3, 1, 1, 1)
        self.lbl_Offset = QtWidgets.QLabel(self.groupBoxFrame)
        self.lbl_Offset.setObjectName("lbl_Offset")
        self.glFrame.addWidget(self.lbl_Offset, 2, 0, 1, 1)
        self.lbl_Y = QtWidgets.QLabel(self.groupBoxFrame)
        self.lbl_Y.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_Y.setObjectName("lbl_Y")
        self.glFrame.addWidget(self.lbl_Y, 0, 2, 1, 1)
        self.spb_YBin = QtWidgets.QSpinBox(self.groupBoxFrame)
        self.spb_YBin.setMinimum(1)
        self.spb_YBin.setMaximum(512)
        self.spb_YBin.setObjectName("spb_YBin")
        self.glFrame.addWidget(self.spb_YBin, 3, 2, 1, 1)
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
        spacerItem2 = QtWidgets.QSpacerItem(18, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout_2.addItem(spacerItem2, 1, 0, 1, 1)
        self.lbl_SetPoint = QtWidgets.QLabel(self.groupBoxTemp)
        self.lbl_SetPoint.setObjectName("lbl_SetPoint")
        self.gridLayout_2.addWidget(self.lbl_SetPoint, 0, 1, 1, 1)
        self.lbl_ArrayTemp = QtWidgets.QLabel(self.groupBoxTemp)
        self.lbl_ArrayTemp.setObjectName("lbl_ArrayTemp")
        self.gridLayout_2.addWidget(self.lbl_ArrayTemp, 1, 1, 1, 1)
        spacerItem3 = QtWidgets.QSpacerItem(18, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout_2.addItem(spacerItem3, 0, 0, 1, 1)
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
        spacerItem4 = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout_2.addItem(spacerItem4, 2, 0, 1, 1)
        self.btn_SetTemp = QtWidgets.QPushButton(self.groupBoxTemp)
        self.btn_SetTemp.setObjectName("btn_SetTemp")
        self.gridLayout_2.addWidget(self.btn_SetTemp, 3, 0, 1, 3)
        self.button_vl.addWidget(self.groupBoxTemp)
        self.groupBoxControls = QtWidgets.QGroupBox(self.centralWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.groupBoxControls.sizePolicy().hasHeightForWidth())
        self.groupBoxControls.setSizePolicy(sizePolicy)
        self.groupBoxControls.setObjectName("groupBoxControls")
        self.gridLayout_3 = QtWidgets.QGridLayout(self.groupBoxControls)
        self.gridLayout_3.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_3.setSpacing(6)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.spb_P = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_P.setDecimals(4)
        self.spb_P.setMinimum(-100.0)
        self.spb_P.setMaximum(100.0)
        self.spb_P.setSingleStep(0.01)
        self.spb_P.setObjectName("spb_P")
        self.gridLayout_3.addWidget(self.spb_P, 0, 2, 1, 1)
        self.label_FiberLoc = QtWidgets.QLabel(self.groupBoxControls)
        self.label_FiberLoc.setAlignment(QtCore.Qt.AlignCenter)
        self.label_FiberLoc.setObjectName("label_FiberLoc")
        self.gridLayout_3.addWidget(self.label_FiberLoc, 4, 0, 1, 1)
        self.label_I = QtWidgets.QLabel(self.groupBoxControls)
        self.label_I.setAlignment(QtCore.Qt.AlignCenter)
        self.label_I.setObjectName("label_I")
        self.gridLayout_3.addWidget(self.label_I, 1, 1, 1, 1)
        self.lbl_Rotation = QtWidgets.QLabel(self.groupBoxControls)
        self.lbl_Rotation.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_Rotation.setObjectName("lbl_Rotation")
        self.gridLayout_3.addWidget(self.lbl_Rotation, 3, 1, 1, 1)
        self.spb_I = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_I.setDecimals(4)
        self.spb_I.setMinimum(-100.0)
        self.spb_I.setSingleStep(0.01)
        self.spb_I.setObjectName("spb_I")
        self.gridLayout_3.addWidget(self.spb_I, 1, 2, 1, 1)
        self.label_P = QtWidgets.QLabel(self.groupBoxControls)
        self.label_P.setAlignment(QtCore.Qt.AlignCenter)
        self.label_P.setObjectName("label_P")
        self.gridLayout_3.addWidget(self.label_P, 0, 1, 1, 1)
        self.spb_Rotation = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_Rotation.setDecimals(1)
        self.spb_Rotation.setMinimum(-360.0)
        self.spb_Rotation.setMaximum(360.0)
        self.spb_Rotation.setObjectName("spb_Rotation")
        self.gridLayout_3.addWidget(self.spb_Rotation, 3, 2, 1, 1)
        self.label_D = QtWidgets.QLabel(self.groupBoxControls)
        self.label_D.setAlignment(QtCore.Qt.AlignCenter)
        self.label_D.setObjectName("label_D")
        self.gridLayout_3.addWidget(self.label_D, 2, 1, 1, 1)
        self.spb_D = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_D.setDecimals(4)
        self.spb_D.setMinimum(-100.0)
        self.spb_D.setMaximum(100.0)
        self.spb_D.setSingleStep(0.01)
        self.spb_D.setObjectName("spb_D")
        self.gridLayout_3.addWidget(self.spb_D, 2, 2, 1, 1)
        self.btn_Gain = QtWidgets.QPushButton(self.groupBoxControls)
        self.btn_Gain.setObjectName("btn_Gain")
        self.gridLayout_3.addWidget(self.btn_Gain, 0, 0, 1, 1)
        self.spb_XFib = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_XFib.setButtonSymbols(QtWidgets.QAbstractSpinBox.NoButtons)
        self.spb_XFib.setDecimals(3)
        self.spb_XFib.setMaximum(512.0)
        self.spb_XFib.setObjectName("spb_XFib")
        self.gridLayout_3.addWidget(self.spb_XFib, 4, 1, 1, 1)
        self.spb_YFib = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_YFib.setButtonSymbols(QtWidgets.QAbstractSpinBox.NoButtons)
        self.spb_YFib.setDecimals(3)
        self.spb_YFib.setMaximum(512.0)
        self.spb_YFib.setObjectName("spb_YFib")
        self.gridLayout_3.addWidget(self.spb_YFib, 4, 2, 1, 1)
        self.button_vl.addWidget(self.groupBoxControls)
        self.logUpdateText = QtWidgets.QPlainTextEdit(self.centralWidget)
        self.logUpdateText.setObjectName("logUpdateText")
        self.button_vl.addWidget(self.logUpdateText)
        self.horizontalLayout.addLayout(self.button_vl)
        MainWindow.setCentralWidget(self.centralWidget)
        self.menuBar = QtWidgets.QMenuBar(MainWindow)
        self.menuBar.setGeometry(QtCore.QRect(0, 0, 830, 26))
        self.menuBar.setObjectName("menuBar")
        MainWindow.setMenuBar(self.menuBar)
        self.mainToolBar = QtWidgets.QToolBar(MainWindow)
        self.mainToolBar.setObjectName("mainToolBar")
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.mainToolBar)
        self.statusBar = QtWidgets.QStatusBar(MainWindow)
        self.statusBar.setObjectName("statusBar")
        MainWindow.setStatusBar(self.statusBar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "EXPRES FTT"))
        self.btn_ToggleCam.setToolTip(_translate("MainWindow", "Start full frame exposures"))
        self.btn_ToggleCam.setText(_translate("MainWindow", "Start"))
        self.radioButton.setText(_translate("MainWindow", "Servo"))
        self.groupBoxExposure.setTitle(_translate("MainWindow", "Exposure Parameters"))
        self.spb_EMGain.setToolTip(_translate("MainWindow", "<html><head/><body><p>Electron-multiplying gain. Range of valid values are 1-300.</p></body></html>"))
        self.lbl_ExpTime.setText(_translate("MainWindow", "Exp. Time [s]"))
        self.lbl_EMGain.setText(_translate("MainWindow", "EM Gain"))
        self.btn_SetExp.setText(_translate("MainWindow", "Set Exposure"))
        self.spb_ExpTime.setToolTip(_translate("MainWindow", "<html><head/><body><p>Integration time in milliseconds. Value will update with actual time.</p></body></html>"))
        self.groupBoxFrame.setTitle(_translate("MainWindow", "Frame Parameters"))
        self.spb_XDim.setToolTip(_translate("MainWindow", "Horizontal dimension"))
        self.lbl_Dimension.setText(_translate("MainWindow", "Dimension"))
        self.lbl_Binning.setText(_translate("MainWindow", "Binning"))
        self.lbl_X.setText(_translate("MainWindow", "X"))
        self.spb_YDim.setToolTip(_translate("MainWindow", "Vertical dimension"))
        self.spb_XOffs.setToolTip(_translate("MainWindow", "ROI horizontal offset"))
        self.spb_YOffs.setToolTip(_translate("MainWindow", "ROI vertical offset"))
        self.spb_XBin.setToolTip(_translate("MainWindow", "Horizontal pixel binning"))
        self.lbl_Offset.setText(_translate("MainWindow", "Offset"))
        self.lbl_Y.setText(_translate("MainWindow", "Y"))
        self.spb_YBin.setToolTip(_translate("MainWindow", "Vertical pixel binning"))
        self.btn_SetFrame.setToolTip(_translate("MainWindow", "Update camera frame"))
        self.btn_SetFrame.setText(_translate("MainWindow", "Set Frame"))
        self.groupBoxTemp.setTitle(_translate("MainWindow", "Temperature"))
        self.lbl_SetPoint.setText(_translate("MainWindow", "Set Point"))
        self.lbl_ArrayTemp.setText(_translate("MainWindow", "Array Temp."))
        self.spb_ArrayTemp.setToolTip(_translate("MainWindow", "<html><head/><body><p>Actual array temperature in C.</p></body></html>"))
        self.lbl_TempStatus.setText(_translate("MainWindow", "Status"))
        self.spb_SetPoint.setToolTip(_translate("MainWindow", "<html><head/><body><p>Target array temperature in C.</p></body></html>"))
        self.btn_SetTemp.setText(_translate("MainWindow", "Set Temp"))
        self.groupBoxControls.setTitle(_translate("MainWindow", "Control Parameters"))
        self.label_FiberLoc.setText(_translate("MainWindow", "Fiber loc."))
        self.label_I.setText(_translate("MainWindow", "I"))
        self.lbl_Rotation.setText(_translate("MainWindow", "Rot."))
        self.label_P.setText(_translate("MainWindow", "P"))
        self.label_D.setText(_translate("MainWindow", "D"))
        self.btn_Gain.setText(_translate("MainWindow", "Set Gain"))
        self.spb_XFib.setToolTip(_translate("MainWindow", "Target horizontal location for control algorithm"))
        self.spb_YFib.setToolTip(_translate("MainWindow", "Target vertical location for control algorithm"))

