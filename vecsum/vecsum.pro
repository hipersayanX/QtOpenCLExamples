# QtOpenCLExamples, Qt 5 + OpenCL 1.2 usage examples.
# Copyright (C) 2015  Gonzalo Exequiel Pedone
#
# QtOpenCLExamples is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# QtOpenCLExamples is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with QtOpenCLExamples. If not, see <http://www.gnu.org/licenses/>.
#
# Email   : hipersayan DOT x AT gmail DOT com
# Web-Site: http://github.com/hipersayanX/QtOpenCLExamples

exists(commons.pri): include(commons.pri)
else: exists(../commons.pri): include(../commons.pri)

LIBS += -lOpenCL

QT += core
QT -= gui

TARGET = vecsum
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

DISTFILES += \
    vecsum.cl

RESOURCES += \
    vecsum.qrc
