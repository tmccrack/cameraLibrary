from PyQt5 import QtCore, QtWidgets
from fttServoWindow import Ui_Dialog as servoWindow  ## pyuic5 generated file

class Servo(QtWidgets.QDialog, servoWindow):
    """
    Dialog window for servo parameters
    Inherits ui object
    """
    # gains = {}
    # rot = 0.0
    def __init__(self, parent=None, gain_dict=None, rot=None, targ=None, bg=None):
        super(Servo, self).__init__()
        self.setupUi(self)
        if gain_dict is None:
            gain_dict = {'kp': 0.0,
                         'ki': 0.0,
                         'kd': 0.0,
                         'dt': 0.0}
        if rot is None: rot = 0.0
        if targ is None: targ = (0, 0)
        if bg is None: bg = 0.0 
        self.gains = gain_dict
        self.rot = rot
        self.targ = targ
        # Initialize values
        self.spb_P.setValue(self.gains['kp'])
        self.spb_I.setValue(self.gains['ki'])
        self.spb_D.setValue(self.gains['kd'])
        self.spb_Rotation.setValue(self.rot)
        self.combo_Alg.addItems(["Center-of-Mass", "Quadrant", "Guassian"])
        self.spb_XTarg.setValue(self.targ[0])
        self.spb_YTarg.setValue(self.targ[1])
        self.spb_Background.setValue(bg)
        # Connect slots
        self.buttonBox.rejected.connect(self.close)
        self.buttonBox.accepted.connect(self.updateVals)
        self.buttonBox.accepted.connect(self.close)
        self.buttonBox.button(QtWidgets.QDialogButtonBox.Apply).clicked.connect(self.updateVals)
        self.spb_P.valueChanged.connect(self.updateVals)
        self.spb_I.valueChanged.connect(self.updateVals)
        self.spb_D.valueChanged.connect(self.updateVals)
        self.spb_Rotation.valueChanged.connect(self.updateVals)

    def updateVals(self):
        self.gains['kp'] = self.spb_P.value()
        self.gains['ki'] = self.spb_I.value()
        self.gains['kd'] = self.spb_D.value()
        self.targ = (self.spb_XTarg.value(), self.spb_YTarg.value())
        self.rot = self.spb_Rotation.value()


    def reInitVals(self, gain_dict=None, rot=None, targ=None, bg=None):
        if gain_dict is not None: self.gains = gain_dict
        if rot is not None: self.rot = rot
        if targ is not None: self.targ = targ
        if bg is not None: self.spb_Background.setValue(bg)
        self.spb_P.setValue(self.gains['kp'])
        self.spb_I.setValue(self.gains['ki'])
        self.spb_D.setValue(self.gains['kd'])
        self.spb_Rotation.setValue(self.rot)
        self.spb_XTarg.setValue(self.targ[0])
        self.spb_YTarg.setValue(self.targ[1])

    def keyPressEvent(self, event):
        pass
        # if type(event) == Qt



if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    serv = Servo()  # The dialog
    serv.show()
    sys.exit(app.exec_())