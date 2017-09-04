from PyQt5 import QtCore, QtWidgets
from ftt.ftt_temp_window import Ui_Dialog as tempWindow  ## pyuic5 generated file

class Temp(QtWidgets.QDialog, tempWindow):
    """
    Dialog window for servo parameters
    Inherits ui object
    """
    # gains = {}
    # rot = 0.0
    def __init__(self, parent=None,):
        super(Temp, self).__init__()
        self.setupUi(self)

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    temp = Temp()  # The dialog
    temp.show()
    sys.exit(app.exec_())