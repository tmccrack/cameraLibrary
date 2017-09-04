from PyQt5 import QtCore, QtWidgets
from ftt.ftt_image_logger_window import Ui_Dialog as logWindow  ## pyuic5 generated file

class Logger(QtWidgets.QDialog, logWindow):

    def __init__(self, parent=None, intv=1, state=True):
        super(Logger, self).__init__()
        self.setupUi(self)
        if intv is None:
            intv = 1
        self.initVals(intv, state)
        self.buttonBox.rejected.connect(self.close)
        self.buttonBox.accepted.connect(self.updateVals)
        self.buttonBox.accepted.connect(self.close)

    def updateVals(self):
        self.intv = self.spb_Interval.value()
        self.state = self.rad_Logging.isChecked()

    def initVals(self, intv, state):
        self.intv = intv
        self.state = state
        self.spb_Interval.setValue(self.intv)
        self.rad_Logging.setChecked(state)

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    log = Logger()
    log.show()
    sys.exit(app.exec_())


