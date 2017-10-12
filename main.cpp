/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>
#include <QDesktopWidget>
#include <QStyle>

#ifndef QT_NO_OPENGL
#include "mainwidget.h"
#endif

QImage image;

bool loadMap(QString imagePath);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("TP2 - Game loop et timers");
    app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
    QRect windowSize = QApplication::desktop()->screenGeometry();
    const int frameSpacing = 10,
              frameHeight = QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight) + frameSpacing,
              frameWidth = frameSpacing * 2;
    int height = windowSize.height() / 2 - frameHeight,
        width = windowSize.width() / 2 - frameWidth;
    QString imagePath;
    QFileDialog *dialog = new QFileDialog();
    dialog->setGeometry(QRect(windowSize.width() / 4, windowSize.height() / 4, windowSize.width() / 2, windowSize.height() / 2));
    do {
        imagePath = dialog->getOpenFileName(
            0,
            QObject::tr("Pick an image for your heightmap"),
            "",
            QObject::tr("PNG (*.png);; JPEG (*.jpg *.jpeg)")
        );
    } while(!loadMap(imagePath));
    MainWidget  window1(0, image, 1),
                window2(0, image, 10),
                window3(0, image, 100),
                window4(0, image, 1000);
    window1.setWindowTitle("1 FPS");
    window1.resize(width, height);
    window1.move(0, 0);
    window1.show();
    window2.setWindowTitle("10 FPS");
    window2.resize(width, height);
    window2.move(windowSize.width() / 2, 0);
    window2.show();
    window3.setWindowTitle("100 FPS");
    window3.resize(width, height);
    window3.move(0, windowSize.height() / 2);
    window3.show();
    window4.setWindowTitle("1000 FPS");
    window4.resize(width, height);
    window4.move(windowSize.width() / 2, windowSize.height() / 2);
    window4.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}

bool loadMap(QString imagePath)
{
    if(image.load(imagePath)) {
        return true;
    }
    std::cerr << "Load \"" << imagePath.toStdString() << "\" failed." << std::endl;
    return false;
}
