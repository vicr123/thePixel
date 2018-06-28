#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QStackedWidget>
#include <QLabel>
#include <QWindow>
#include <QThread>
#include <QSpinBox>
#include "screenshotwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_takeButton_clicked();

        void updateScreenshotLabel();

    private:
        Ui::MainWindow *ui;

        ScreenshotWindow* s;
        QPixmap currentPixmap;

        void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
