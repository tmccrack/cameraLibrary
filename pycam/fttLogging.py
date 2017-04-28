from PyQt5 import QtCore, QtWidgets
from fttLoggingWindow import Ui_Dialog as logWindow  ## pyuic5 generated file

class Logger(QtWidgets.QDialog, logWindow):

    def __init__(self, parent=None, intv=None):
        super(Logger, self).__init__()
        self.setupUi(self)
        if intv is None:
            intv = 1
        self.initVals(intv)
        self.buttonBox.rejected.connect(self.close)
        self.buttonBox.accepted.connect(self.updateVals)
        self.buttonBox.accepted.connect(self.close)

    def updateVals(self):
        self.intv = self.spb_Interval.value()

    def initVals(self, intv):
        self.intv = intv
        self.spb_Interval.setValue(self.intv)

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    log = Logger()
    log.show()
    sys.exit(app.exec_())


