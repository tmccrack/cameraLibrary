# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file './../src/fttTempWindow.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(212, 177)
        self.buttonBox = QtWidgets.QDialogButtonBox(Dialog)
        self.buttonBox.setGeometry(QtCore.QRect(-10, 140, 211, 32))
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.gridLayoutWidget = QtWidgets.QWidget(Dialog)
        self.gridLayoutWidget.setGeometry(QtCore.QRect(9, 9, 191, 121))
        self.gridLayoutWidget.setObjectName("gridLayoutWidget")
        self.gridLayout = QtWidgets.QGridLayout(self.gridLayoutWidget)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.spb_ArrayTemp = QtWidgets.QSpinBox(self.gridLayoutWidget)
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
        self.gridLayout.addWidget(self.spb_ArrayTemp, 1, 1, 1, 1)
        self.spb_SetPoint = QtWidgets.QSpinBox(self.gridLayoutWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.spb_SetPoint.sizePolicy().hasHeightForWidth())
        self.spb_SetPoint.setSizePolicy(sizePolicy)
        self.spb_SetPoint.setMinimum(-100)
        self.spb_SetPoint.setMaximum(30)
        self.spb_SetPoint.setObjectName("spb_SetPoint")
        self.gridLayout.addWidget(self.spb_SetPoint, 0, 1, 1, 1)
        self.lbl_ArrayTemp = QtWidgets.QLabel(self.gridLayoutWidget)
        self.lbl_ArrayTemp.setObjectName("lbl_ArrayTemp")
        self.gridLayout.addWidget(self.lbl_ArrayTemp, 1, 0, 1, 1)
        self.lbl_TempStatus = QtWidgets.QLabel(self.gridLayoutWidget)
        self.lbl_TempStatus.setObjectName("lbl_TempStatus")
        self.gridLayout.addWidget(self.lbl_TempStatus, 2, 0, 1, 1)
        self.edt_TempStatus = QtWidgets.QLineEdit(self.gridLayoutWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.edt_TempStatus.sizePolicy().hasHeightForWidth())
        self.edt_TempStatus.setSizePolicy(sizePolicy)
        self.edt_TempStatus.setReadOnly(True)
        self.edt_TempStatus.setObjectName("edt_TempStatus")
        self.gridLayout.addWidget(self.edt_TempStatus, 2, 1, 1, 1)
        self.lbl_SetPoint = QtWidgets.QLabel(self.gridLayoutWidget)
        self.lbl_SetPoint.setObjectName("lbl_SetPoint")
        self.gridLayout.addWidget(self.lbl_SetPoint, 0, 0, 1, 1)

        self.retranslateUi(Dialog)
        self.buttonBox.accepted.connect(Dialog.accept)
        self.buttonBox.rejected.connect(Dialog.reject)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Temperature"))
        self.spb_ArrayTemp.setToolTip(_translate("Dialog", "<html><head/><body><p>Actual array temperature in C.</p></body></html>"))
        self.spb_SetPoint.setToolTip(_translate("Dialog", "<html><head/><body><p>Target array temperature in C.</p></body></html>"))
        self.lbl_ArrayTemp.setText(_translate("Dialog", "Array Temp."))
        self.lbl_TempStatus.setText(_translate("Dialog", "Status"))
        self.lbl_SetPoint.setText(_translate("Dialog", "Set Point"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.show()
    sys.exit(app.exec_())

