# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui/reflectometer/merging_refm.ui'
#
# Created: Mon Aug 22 07:08:25 2011
#      by: PyQt4 UI code generator 4.7.2
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1138, 980)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.pushButton = QtGui.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(10, 10, 115, 32))
        self.pushButton.setObjectName("pushButton")
        self.pushButton_2 = QtGui.QPushButton(self.centralwidget)
        self.pushButton_2.setGeometry(QtCore.QRect(130, 10, 161, 32))
        self.pushButton_2.setObjectName("pushButton_2")
        self.tableWidget = QtGui.QTableWidget(self.centralwidget)
        self.tableWidget.setGeometry(QtCore.QRect(20, 60, 1021, 291))
        self.tableWidget.setObjectName("tableWidget")
        self.tableWidget.setColumnCount(0)
        self.tableWidget.setRowCount(0)
        self.pushButton_3 = QtGui.QPushButton(self.centralwidget)
        self.pushButton_3.setGeometry(QtCore.QRect(870, 10, 171, 32))
        self.pushButton_3.setObjectName("pushButton_3")
        self.pushButton_4 = QtGui.QPushButton(self.centralwidget)
        self.pushButton_4.setGeometry(QtCore.QRect(420, 360, 621, 32))
        self.pushButton_4.setObjectName("pushButton_4")
        self.label_2 = QtGui.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(680, 400, 21, 16))
        self.label_2.setObjectName("label_2")
        self.groupBox = QtGui.QGroupBox(self.centralwidget)
        self.groupBox.setGeometry(QtCore.QRect(420, 410, 621, 121))
        self.groupBox.setObjectName("groupBox")
        self.horizontalLayoutWidget_2 = QtGui.QWidget(self.groupBox)
        self.horizontalLayoutWidget_2.setGeometry(QtCore.QRect(10, 30, 601, 41))
        self.horizontalLayoutWidget_2.setObjectName("horizontalLayoutWidget_2")
        self.horizontalLayout_2 = QtGui.QHBoxLayout(self.horizontalLayoutWidget_2)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.pushButton_6 = QtGui.QPushButton(self.horizontalLayoutWidget_2)
        self.pushButton_6.setObjectName("pushButton_6")
        self.horizontalLayout_2.addWidget(self.pushButton_6)
        self.pushButton_7 = QtGui.QPushButton(self.horizontalLayoutWidget_2)
        self.pushButton_7.setObjectName("pushButton_7")
        self.horizontalLayout_2.addWidget(self.pushButton_7)
        self.pushButton_8 = QtGui.QPushButton(self.horizontalLayoutWidget_2)
        self.pushButton_8.setObjectName("pushButton_8")
        self.horizontalLayout_2.addWidget(self.pushButton_8)
        self.horizontalLayoutWidget_3 = QtGui.QWidget(self.groupBox)
        self.horizontalLayoutWidget_3.setGeometry(QtCore.QRect(10, 70, 601, 41))
        self.horizontalLayoutWidget_3.setObjectName("horizontalLayoutWidget_3")
        self.horizontalLayout_3 = QtGui.QHBoxLayout(self.horizontalLayoutWidget_3)
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.pushButton_9 = QtGui.QPushButton(self.horizontalLayoutWidget_3)
        self.pushButton_9.setObjectName("pushButton_9")
        self.horizontalLayout_3.addWidget(self.pushButton_9)
        self.pushButton_10 = QtGui.QPushButton(self.horizontalLayoutWidget_3)
        self.pushButton_10.setObjectName("pushButton_10")
        self.horizontalLayout_3.addWidget(self.pushButton_10)
        self.pushButton_11 = QtGui.QPushButton(self.horizontalLayoutWidget_3)
        self.pushButton_11.setObjectName("pushButton_11")
        self.horizontalLayout_3.addWidget(self.pushButton_11)
        self.horizontalLayoutWidget = QtGui.QWidget(self.centralwidget)
        self.horizontalLayoutWidget.setGeometry(QtCore.QRect(320, 10, 521, 31))
        self.horizontalLayoutWidget.setObjectName("horizontalLayoutWidget")
        self.horizontalLayout = QtGui.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.label = QtGui.QLabel(self.horizontalLayoutWidget)
        self.label.setObjectName("label")
        self.horizontalLayout.addWidget(self.label)
        self.radioButton = QtGui.QRadioButton(self.horizontalLayoutWidget)
        self.radioButton.setChecked(True)
        self.radioButton.setObjectName("radioButton")
        self.horizontalLayout.addWidget(self.radioButton)
        self.radioButton_2 = QtGui.QRadioButton(self.horizontalLayoutWidget)
        self.radioButton_2.setEnabled(False)
        self.radioButton_2.setObjectName("radioButton_2")
        self.horizontalLayout.addWidget(self.radioButton_2)
        self.radioButton_3 = QtGui.QRadioButton(self.horizontalLayoutWidget)
        self.radioButton_3.setObjectName("radioButton_3")
        self.horizontalLayout.addWidget(self.radioButton_3)
        self.radioButton_4 = QtGui.QRadioButton(self.horizontalLayoutWidget)
        self.radioButton_4.setEnabled(False)
        self.radioButton_4.setObjectName("radioButton_4")
        self.horizontalLayout.addWidget(self.radioButton_4)
        self.groupBox_2 = QtGui.QGroupBox(self.centralwidget)
        self.groupBox_2.setGeometry(QtCore.QRect(20, 360, 281, 171))
        self.groupBox_2.setObjectName("groupBox_2")
        self.label_3 = QtGui.QLabel(self.groupBox_2)
        self.label_3.setGeometry(QtCore.QRect(10, 40, 62, 16))
        self.label_3.setObjectName("label_3")
        self.lineEdit = QtGui.QLineEdit(self.groupBox_2)
        self.lineEdit.setGeometry(QtCore.QRect(70, 40, 113, 22))
        self.lineEdit.setObjectName("lineEdit")
        self.label_4 = QtGui.QLabel(self.groupBox_2)
        self.label_4.setGeometry(QtCore.QRect(190, 40, 81, 16))
        self.label_4.setObjectName("label_4")
        self.label_5 = QtGui.QLabel(self.groupBox_2)
        self.label_5.setGeometry(QtCore.QRect(10, 80, 51, 20))
        self.label_5.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
        self.label_5.setObjectName("label_5")
        self.lineEdit_2 = QtGui.QLineEdit(self.groupBox_2)
        self.lineEdit_2.setGeometry(QtCore.QRect(70, 80, 113, 22))
        self.lineEdit_2.setObjectName("lineEdit_2")
        self.label_6 = QtGui.QLabel(self.groupBox_2)
        self.label_6.setGeometry(QtCore.QRect(190, 80, 81, 16))
        self.label_6.setObjectName("label_6")
        self.pushButton_5 = QtGui.QPushButton(self.groupBox_2)
        self.pushButton_5.setGeometry(QtCore.QRect(10, 120, 261, 32))
        self.pushButton_5.setObjectName("pushButton_5")
        self.label_7 = QtGui.QLabel(self.centralwidget)
        self.label_7.setGeometry(QtCore.QRect(340, 430, 31, 16))
        self.label_7.setObjectName("label_7")
        self.groupBox_3 = QtGui.QGroupBox(self.centralwidget)
        self.groupBox_3.setGeometry(QtCore.QRect(20, 550, 671, 361))
        self.groupBox_3.setObjectName("groupBox_3")
        self.label_8 = QtGui.QLabel(self.groupBox_3)
        self.label_8.setGeometry(QtCore.QRect(20, 40, 62, 16))
        self.label_8.setObjectName("label_8")
        self.pushButton_12 = QtGui.QPushButton(self.groupBox_3)
        self.pushButton_12.setGeometry(QtCore.QRect(70, 30, 591, 32))
        self.pushButton_12.setObjectName("pushButton_12")
        self.label_9 = QtGui.QLabel(self.groupBox_3)
        self.label_9.setGeometry(QtCore.QRect(20, 80, 111, 16))
        self.label_9.setObjectName("label_9")
        self.lineEdit_3 = QtGui.QLineEdit(self.groupBox_3)
        self.lineEdit_3.setGeometry(QtCore.QRect(120, 70, 531, 22))
        self.lineEdit_3.setObjectName("lineEdit_3")
        self.radioButton_5 = QtGui.QRadioButton(self.groupBox_3)
        self.radioButton_5.setGeometry(QtCore.QRect(20, 140, 103, 20))
        self.radioButton_5.setAutoExclusive(False)
        self.radioButton_5.setObjectName("radioButton_5")
        self.label_10 = QtGui.QLabel(self.groupBox_3)
        self.label_10.setEnabled(False)
        self.label_10.setGeometry(QtCore.QRect(160, 140, 111, 21))
        self.label_10.setObjectName("label_10")
        self.lineEdit_4 = QtGui.QLineEdit(self.groupBox_3)
        self.lineEdit_4.setEnabled(False)
        self.lineEdit_4.setGeometry(QtCore.QRect(200, 140, 451, 22))
        self.lineEdit_4.setObjectName("lineEdit_4")
        self.radioButton_6 = QtGui.QRadioButton(self.groupBox_3)
        self.radioButton_6.setGeometry(QtCore.QRect(20, 200, 241, 20))
        self.radioButton_6.setAutoExclusive(False)
        self.radioButton_6.setObjectName("radioButton_6")
        self.label_13 = QtGui.QLabel(self.groupBox_3)
        self.label_13.setGeometry(QtCore.QRect(20, 270, 81, 16))
        self.label_13.setObjectName("label_13")
        self.radioButton_8 = QtGui.QRadioButton(self.groupBox_3)
        self.radioButton_8.setGeometry(QtCore.QRect(320, 270, 251, 20))
        self.radioButton_8.setAutoExclusive(False)
        self.radioButton_8.setObjectName("radioButton_8")
        self.radioButton_7 = QtGui.QRadioButton(self.groupBox_3)
        self.radioButton_7.setGeometry(QtCore.QRect(110, 270, 171, 20))
        self.radioButton_7.setAutoExclusive(False)
        self.radioButton_7.setObjectName("radioButton_7")
        self.pushButton_13 = QtGui.QPushButton(self.groupBox_3)
        self.pushButton_13.setGeometry(QtCore.QRect(10, 310, 631, 32))
        self.pushButton_13.setObjectName("pushButton_13")
        self.label_12 = QtGui.QLabel(self.groupBox_3)
        self.label_12.setGeometry(QtCore.QRect(430, 200, 62, 21))
        self.label_12.setObjectName("label_12")
        self.lineEdit_5 = QtGui.QLineEdit(self.groupBox_3)
        self.lineEdit_5.setGeometry(QtCore.QRect(330, 200, 61, 22))
        self.lineEdit_5.setObjectName("lineEdit_5")
        self.lineEdit_6 = QtGui.QLineEdit(self.groupBox_3)
        self.lineEdit_6.setGeometry(QtCore.QRect(470, 200, 61, 22))
        self.lineEdit_6.setObjectName("lineEdit_6")
        self.label_11 = QtGui.QLabel(self.groupBox_3)
        self.label_11.setGeometry(QtCore.QRect(300, 200, 62, 21))
        self.label_11.setObjectName("label_11")
        self.groupBox_4 = QtGui.QGroupBox(self.centralwidget)
        self.groupBox_4.setGeometry(QtCore.QRect(710, 550, 331, 161))
        self.groupBox_4.setObjectName("groupBox_4")
        self.radioButton_9 = QtGui.QRadioButton(self.groupBox_4)
        self.radioButton_9.setGeometry(QtCore.QRect(20, 40, 211, 20))
        self.radioButton_9.setObjectName("radioButton_9")
        self.label_14 = QtGui.QLabel(self.groupBox_4)
        self.label_14.setGeometry(QtCore.QRect(60, 70, 201, 21))
        self.label_14.setObjectName("label_14")
        self.lineEdit_7 = QtGui.QLineEdit(self.groupBox_4)
        self.lineEdit_7.setGeometry(QtCore.QRect(270, 70, 41, 22))
        self.lineEdit_7.setObjectName("lineEdit_7")
        self.radioButton_10 = QtGui.QRadioButton(self.groupBox_4)
        self.radioButton_10.setGeometry(QtCore.QRect(20, 120, 311, 20))
        self.radioButton_10.setObjectName("radioButton_10")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1138, 22))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton.setText(QtGui.QApplication.translate("MainWindow", "Load table...", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_2.setText(QtGui.QApplication.translate("MainWindow", "Load reduce files...", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_3.setText(QtGui.QApplication.translate("MainWindow", "Save table...", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_4.setText(QtGui.QApplication.translate("MainWindow", "Automatic Stitching", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("MainWindow", "or", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("MainWindow", "Manual stitching of selected file(s)", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_6.setText(QtGui.QApplication.translate("MainWindow", "+100%", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_7.setText(QtGui.QApplication.translate("MainWindow", "+10%", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_8.setText(QtGui.QApplication.translate("MainWindow", "+1%", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_9.setText(QtGui.QApplication.translate("MainWindow", "-10%", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_10.setText(QtGui.QApplication.translate("MainWindow", "-100%", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_11.setText(QtGui.QApplication.translate("MainWindow", "-1%", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("MainWindow", "  Data spin state:  ", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton.setText(QtGui.QApplication.translate("MainWindow", "Off_Off", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_2.setText(QtGui.QApplication.translate("MainWindow", "Off_On", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_3.setText(QtGui.QApplication.translate("MainWindow", "On_Off", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_4.setText(QtGui.QApplication.translate("MainWindow", "On_On", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_2.setTitle(QtGui.QApplication.translate("MainWindow", "Critical edge", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("MainWindow", "From Q", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("MainWindow", "Angstroms", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("MainWindow", "    To Q", None, QtGui.QApplication.UnicodeUTF8))
        self.label_6.setText(QtGui.QApplication.translate("MainWindow", "Angstroms", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_5.setText(QtGui.QApplication.translate("MainWindow", "Calculate SF", None, QtGui.QApplication.UnicodeUTF8))
        self.label_7.setText(QtGui.QApplication.translate("MainWindow", "and", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_3.setTitle(QtGui.QApplication.translate("MainWindow", "Create output", None, QtGui.QApplication.UnicodeUTF8))
        self.label_8.setText(QtGui.QApplication.translate("MainWindow", "Folder", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_12.setText(QtGui.QApplication.translate("MainWindow", "~/results/", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setText(QtGui.QApplication.translate("MainWindow", "Base file name", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_5.setText(QtGui.QApplication.translate("MainWindow", "Email output", None, QtGui.QApplication.UnicodeUTF8))
        self.label_10.setText(QtGui.QApplication.translate("MainWindow", "email:", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_6.setText(QtGui.QApplication.translate("MainWindow", "Add resolution function to output", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setText(QtGui.QApplication.translate("MainWindow", "File format:", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_8.setText(QtGui.QApplication.translate("MainWindow", "Combined   [_scaled_combined.txt]", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_7.setText(QtGui.QApplication.translate("MainWindow", "Scaled     [_scaled.txt]", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_13.setText(QtGui.QApplication.translate("MainWindow", "Create output files", None, QtGui.QApplication.UnicodeUTF8))
        self.label_12.setText(QtGui.QApplication.translate("MainWindow", "dQ/Q", None, QtGui.QApplication.UnicodeUTF8))
        self.lineEdit_5.setText(QtGui.QApplication.translate("MainWindow", "0.0012", None, QtGui.QApplication.UnicodeUTF8))
        self.lineEdit_6.setText(QtGui.QApplication.translate("MainWindow", "0.02", None, QtGui.QApplication.UnicodeUTF8))
        self.label_11.setText(QtGui.QApplication.translate("MainWindow", "dQ0", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_4.setTitle(QtGui.QApplication.translate("MainWindow", "Settings", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_9.setText(QtGui.QApplication.translate("MainWindow", "Automatic loading cleaning", None, QtGui.QApplication.UnicodeUTF8))
        self.label_14.setText(QtGui.QApplication.translate("MainWindow", "% of Q to remove on both sides", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_10.setText(QtGui.QApplication.translate("MainWindow", "Use predefined SF values when loading table", None, QtGui.QApplication.UnicodeUTF8))

