#!/usr/bin/env python

# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'gui.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

import resources
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import QPainter, QBrush, QPen
from PyQt4.QtCore import Qt
from PyQt4.QtCore import QObject, pyqtSignal, pyqtSlot

import rospy
from std_msgs.msg import String
try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_Form(object):
    def setupUi(self, Form):

        rospy.init_node('mines', anonymous=True)
        rospy.Subscriber("setMines", String, self.callback)
        self.sig = signals()
        self.sig.set.connect(self.setMine)
        Form.setObjectName(_fromUtf8("Form"))
        Form.resize(400, 400)
        Form.setMaximumSize(QtCore.QSize(400, 400))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/Images/icon.jpg")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Form.setWindowIcon(icon)
        Form.setStyleSheet(_fromUtf8("background-image: url(:/Images/grid);"))
        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(_translate("Form", "Minesweepers\'20", None))


    def callback(self,data):

        com = data.data
        global x,y,c
        c = int(com[0])
        x = int(com[com.index('X')+1:com.index('Y')])
        y = int(com[com.index('Y')+1:])
        self.sig.set.emit(x,y,c)



    def setMine(self,x,y,c):
        print x,y,c
        self.label = QtGui.QLabel(Form)
        self.label.setGeometry(QtCore.QRect(x*20 + 5, y*20 + 5,10 , 10))
        self.label.setObjectName(_fromUtf8("label"))

        if c==1:
            self.label.setStyleSheet('color: red')
            self.label.setText("X")
        elif c==0:
            self.label.setStyleSheet('color: blue')
            self.label.setText("X")

        self.label.show()
        app.processEvents()




class signals(QtCore.QObject):

    set = QtCore.pyqtSignal(int,int,int)



if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    Form = QtGui.QWidget()
    ui = Ui_Form()
    ui.setupUi(Form)
    Form.show()
    sys.exit(app.exec_())
