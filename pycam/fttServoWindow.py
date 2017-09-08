# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file './../src/fttServoWindow.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(280, 270)
        Dialog.setMinimumSize(QtCore.QSize(280, 260))
        self.verticalLayoutWidget = QtWidgets.QWidget(Dialog)
        self.verticalLayoutWidget.setGeometry(QtCore.QRect(10, 10, 261, 253))
        self.verticalLayoutWidget.setObjectName("verticalLayoutWidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.verticalLayoutWidget)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.groupBoxControls = QtWidgets.QGroupBox(self.verticalLayoutWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.groupBoxControls.sizePolicy().hasHeightForWidth())
        self.groupBoxControls.setSizePolicy(sizePolicy)
        self.groupBoxControls.setTitle("")
        self.groupBoxControls.setFlat(True)
        self.groupBoxControls.setObjectName("groupBoxControls")
        self.gridLayout_3 = QtWidgets.QGridLayout(self.groupBoxControls)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.label_P = QtWidgets.QLabel(self.groupBoxControls)
        self.label_P.setAlignment(QtCore.Qt.AlignCenter)
        self.label_P.setObjectName("label_P")
        self.gridLayout_3.addWidget(self.label_P, 0, 1, 1, 1)
        self.label_D = QtWidgets.QLabel(self.groupBoxControls)
        self.label_D.setAlignment(QtCore.Qt.AlignCenter)
        self.label_D.setObjectName("label_D")
        self.gridLayout_3.addWidget(self.label_D, 2, 1, 1, 1)
        self.spb_I = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_I.setDecimals(4)
        self.spb_I.setMinimum(-100.0)
        self.spb_I.setSingleStep(0.01)
        self.spb_I.setObjectName("spb_I")
        self.gridLayout_3.addWidget(self.spb_I, 1, 2, 1, 1)
        self.lbl_Rotation = QtWidgets.QLabel(self.groupBoxControls)
        self.lbl_Rotation.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_Rotation.setObjectName("lbl_Rotation")
        self.gridLayout_3.addWidget(self.lbl_Rotation, 3, 1, 1, 1)
        self.spb_Rotation = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_Rotation.setEnabled(True)
        self.spb_Rotation.setDecimals(1)
        self.spb_Rotation.setMinimum(-360.0)
        self.spb_Rotation.setMaximum(360.0)
        self.spb_Rotation.setObjectName("spb_Rotation")
        self.gridLayout_3.addWidget(self.spb_Rotation, 3, 2, 1, 1)
        self.label_TargetrLoc = QtWidgets.QLabel(self.groupBoxControls)
        self.label_TargetrLoc.setAlignment(QtCore.Qt.AlignCenter)
        self.label_TargetrLoc.setObjectName("label_TargetrLoc")
        self.gridLayout_3.addWidget(self.label_TargetrLoc, 6, 0, 1, 1)
        self.combo_Alg = QtWidgets.QComboBox(self.groupBoxControls)
        self.combo_Alg.setCurrentText("")
        self.combo_Alg.setObjectName("combo_Alg")
        self.gridLayout_3.addWidget(self.combo_Alg, 7, 1, 1, 2)
        self.spb_XTarg = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_XTarg.setEnabled(True)
        self.spb_XTarg.setButtonSymbols(QtWidgets.QAbstractSpinBox.UpDownArrows)
        self.spb_XTarg.setDecimals(3)
        self.spb_XTarg.setMaximum(512.0)
        self.spb_XTarg.setSingleStep(0.5)
        self.spb_XTarg.setObjectName("spb_XTarg")
        self.gridLayout_3.addWidget(self.spb_XTarg, 6, 1, 1, 1)
        self.spb_YTarg = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_YTarg.setEnabled(True)
        self.spb_YTarg.setButtonSymbols(QtWidgets.QAbstractSpinBox.UpDownArrows)
        self.spb_YTarg.setDecimals(3)
        self.spb_YTarg.setMaximum(512.0)
        self.spb_YTarg.setSingleStep(0.5)
        self.spb_YTarg.setObjectName("spb_YTarg")
        self.gridLayout_3.addWidget(self.spb_YTarg, 6, 2, 1, 1)
        self.spb_D = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_D.setEnabled(True)
        self.spb_D.setDecimals(4)
        self.spb_D.setMinimum(-100.0)
        self.spb_D.setMaximum(100.0)
        self.spb_D.setSingleStep(0.01)
        self.spb_D.setObjectName("spb_D")
        self.gridLayout_3.addWidget(self.spb_D, 2, 2, 1, 1)
        self.spb_P = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_P.setEnabled(True)
        self.spb_P.setDecimals(4)
        self.spb_P.setMinimum(-100.0)
        self.spb_P.setMaximum(100.0)
        self.spb_P.setSingleStep(0.01)
        self.spb_P.setObjectName("spb_P")
        self.gridLayout_3.addWidget(self.spb_P, 0, 2, 1, 1)
        self.spb_Background = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_Background.setDecimals(1)
        self.spb_Background.setMinimum(-1.0)
        self.spb_Background.setMaximum(1000.0)
        self.spb_Background.setSingleStep(5.0)
        self.spb_Background.setProperty("value", -1.0)
        self.spb_Background.setObjectName("spb_Background")
        self.gridLayout_3.addWidget(self.spb_Background, 4, 2, 1, 1)
        self.lbl_Background = QtWidgets.QLabel(self.groupBoxControls)
        self.lbl_Background.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_Background.setObjectName("lbl_Background")
        self.gridLayout_3.addWidget(self.lbl_Background, 4, 1, 1, 1)
        self.label_I = QtWidgets.QLabel(self.groupBoxControls)
        self.label_I.setAlignment(QtCore.Qt.AlignCenter)
        self.label_I.setObjectName("label_I")
        self.gridLayout_3.addWidget(self.label_I, 1, 1, 1, 1)
        self.spb_LeakyFactor = QtWidgets.QDoubleSpinBox(self.groupBoxControls)
        self.spb_LeakyFactor.setMaximum(1.0)
        self.spb_LeakyFactor.setSingleStep(0.1)
        self.spb_LeakyFactor.setObjectName("spb_LeakyFactor")
        self.gridLayout_3.addWidget(self.spb_LeakyFactor, 5, 2, 1, 1)
        self.label = QtWidgets.QLabel(self.groupBoxControls)
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName("label")
        self.gridLayout_3.addWidget(self.label, 5, 1, 1, 1)
        self.verticalLayout.addWidget(self.groupBoxControls)
        self.buttonBox = QtWidgets.QDialogButtonBox(self.verticalLayoutWidget)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Apply|QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.verticalLayout.addWidget(self.buttonBox)

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)
        Dialog.setTabOrder(self.spb_P, self.spb_I)
        Dialog.setTabOrder(self.spb_I, self.spb_D)
        Dialog.setTabOrder(self.spb_D, self.spb_Rotation)
        Dialog.setTabOrder(self.spb_Rotation, self.spb_YTarg)
        Dialog.setTabOrder(self.spb_YTarg, self.spb_XTarg)
        Dialog.setTabOrder(self.spb_XTarg, self.combo_Alg)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Servo parameters"))
        self.label_P.setText(_translate("Dialog", "P"))
        self.label_D.setText(_translate("Dialog", "D"))
        self.lbl_Rotation.setText(_translate("Dialog", "Rot."))
        self.label_TargetrLoc.setText(_translate("Dialog", "Target loc."))
        self.spb_XTarg.setToolTip(_translate("Dialog", "Target horizontal location for control algorithm"))
        self.spb_YTarg.setToolTip(_translate("Dialog", "Target vertical location for control algorithm"))
        self.lbl_Background.setText(_translate("Dialog", "Background"))
        self.label_I.setText(_translate("Dialog", "I"))
        self.label.setText(_translate("Dialog", "Leaky factor"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.show()
    sys.exit(app.exec_())

