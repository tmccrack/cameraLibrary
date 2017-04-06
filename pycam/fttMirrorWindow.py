# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file './../src/fttMirrorWindow.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(190, 170)
        self.verticalLayoutWidget = QtWidgets.QWidget(Dialog)
        self.verticalLayoutWidget.setGeometry(QtCore.QRect(10, 10, 171, 151))
        self.verticalLayoutWidget.setObjectName("verticalLayoutWidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.verticalLayoutWidget)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.gridLayout = QtWidgets.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        self.lbl_Ch1 = QtWidgets.QLabel(self.verticalLayoutWidget)
        self.lbl_Ch1.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_Ch1.setObjectName("lbl_Ch1")
        self.gridLayout.addWidget(self.lbl_Ch1, 2, 1, 1, 1)
        self.btn_Ch1Minus = QtWidgets.QPushButton(self.verticalLayoutWidget)
        self.btn_Ch1Minus.setAutoRepeat(True)
        self.btn_Ch1Minus.setObjectName("btn_Ch1Minus")
        self.gridLayout.addWidget(self.btn_Ch1Minus, 3, 1, 1, 1)
        self.btn_Ch0Minus = QtWidgets.QPushButton(self.verticalLayoutWidget)
        self.btn_Ch0Minus.setAutoRepeat(True)
        self.btn_Ch0Minus.setObjectName("btn_Ch0Minus")
        self.gridLayout.addWidget(self.btn_Ch0Minus, 3, 0, 1, 1)
        self.btn_Ch1Plus = QtWidgets.QPushButton(self.verticalLayoutWidget)
        self.btn_Ch1Plus.setAutoRepeat(True)
        self.btn_Ch1Plus.setObjectName("btn_Ch1Plus")
        self.gridLayout.addWidget(self.btn_Ch1Plus, 1, 1, 1, 1)
        self.lbl_Ch0 = QtWidgets.QLabel(self.verticalLayoutWidget)
        self.lbl_Ch0.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_Ch0.setObjectName("lbl_Ch0")
        self.gridLayout.addWidget(self.lbl_Ch0, 2, 0, 1, 1)
        self.btn_Ch0Plus = QtWidgets.QPushButton(self.verticalLayoutWidget)
        self.btn_Ch0Plus.setAutoRepeat(True)
        self.btn_Ch0Plus.setObjectName("btn_Ch0Plus")
        self.gridLayout.addWidget(self.btn_Ch0Plus, 1, 0, 1, 1)
        self.spb_Ch1 = QtWidgets.QDoubleSpinBox(self.verticalLayoutWidget)
        self.spb_Ch1.setDecimals(3)
        self.spb_Ch1.setMaximum(5.0)
        self.spb_Ch1.setSingleStep(0.01)
        self.spb_Ch1.setObjectName("spb_Ch1")
        self.gridLayout.addWidget(self.spb_Ch1, 0, 1, 1, 1)
        self.spb_Ch0 = QtWidgets.QDoubleSpinBox(self.verticalLayoutWidget)
        self.spb_Ch0.setDecimals(3)
        self.spb_Ch0.setMaximum(5.0)
        self.spb_Ch0.setSingleStep(0.01)
        self.spb_Ch0.setObjectName("spb_Ch0")
        self.gridLayout.addWidget(self.spb_Ch0, 0, 0, 1, 1)
        self.verticalLayout.addLayout(self.gridLayout)
        spacerItem = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem)
        self.line = QtWidgets.QFrame(self.verticalLayoutWidget)
        self.line.setFrameShape(QtWidgets.QFrame.HLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.verticalLayout.addWidget(self.line)
        spacerItem1 = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem1)
        self.btn_OnFiber = QtWidgets.QPushButton(self.verticalLayoutWidget)
        self.btn_OnFiber.setObjectName("btn_OnFiber")
        self.verticalLayout.addWidget(self.btn_OnFiber)

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Mirror control"))
        self.lbl_Ch1.setText(_translate("Dialog", "Ch. 1"))
        self.btn_Ch1Minus.setText(_translate("Dialog", "-"))
        self.btn_Ch0Minus.setText(_translate("Dialog", "-"))
        self.btn_Ch1Plus.setText(_translate("Dialog", "+"))
        self.lbl_Ch0.setText(_translate("Dialog", "Ch. 0"))
        self.btn_Ch0Plus.setText(_translate("Dialog", "+"))
        self.btn_OnFiber.setText(_translate("Dialog", "Center"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.show()
    sys.exit(app.exec_())

