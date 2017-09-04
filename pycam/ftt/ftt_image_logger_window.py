# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file './../src/fttLoggingWindow.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(174, 90)
        self.gridLayout = QtWidgets.QGridLayout(Dialog)
        self.gridLayout.setObjectName("gridLayout")
        self.lbl_LogIntvl = QtWidgets.QLabel(Dialog)
        self.lbl_LogIntvl.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_LogIntvl.setObjectName("lbl_LogIntvl")
        self.gridLayout.addWidget(self.lbl_LogIntvl, 1, 0, 1, 1)
        self.spb_Interval = QtWidgets.QSpinBox(Dialog)
        self.spb_Interval.setMaximum(1000)
        self.spb_Interval.setObjectName("spb_Interval")
        self.gridLayout.addWidget(self.spb_Interval, 1, 1, 1, 1)
        self.buttonBox = QtWidgets.QDialogButtonBox(Dialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.gridLayout.addWidget(self.buttonBox, 2, 0, 1, 2)
        self.lbl_Logging = QtWidgets.QLabel(Dialog)
        self.lbl_Logging.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_Logging.setObjectName("lbl_Logging")
        self.gridLayout.addWidget(self.lbl_Logging, 0, 0, 1, 1)
        self.rad_Logging = QtWidgets.QRadioButton(Dialog)
        self.rad_Logging.setText("")
        self.rad_Logging.setObjectName("rad_Logging")
        self.gridLayout.addWidget(self.rad_Logging, 0, 1, 1, 1)

        self.retranslateUi(Dialog)
        self.buttonBox.accepted.connect(Dialog.accept)
        self.buttonBox.rejected.connect(Dialog.reject)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Logger"))
        self.lbl_LogIntvl.setText(_translate("Dialog", "Interval"))
        self.lbl_Logging.setText(_translate("Dialog", "Logging"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.show()
    sys.exit(app.exec_())

