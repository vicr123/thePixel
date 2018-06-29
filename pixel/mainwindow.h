#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QStackedWidget>
#include <QLabel>
#include <QWindow>
#include <QThread>
#include <QSpinBox>
#include <QPushButton>
#include <QClipboard>
#include <ttoast.h>
#include <QMessageBox>
#include <QPluginLoader>
#include <QListWidget>
#include "screenshotwindow.h"
#include "interfaces.h"

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

        void on_backButton_clicked();

        void on_copyButton_clicked();

        void on_saveButton_clicked();

        void on_publishButton_clicked();

        void on_backButton_2_clicked();

        void on_publishItems_currentRowChanged(int currentRow);

    signals:
        void newPixmap(QPixmap px);

    private:
        Ui::MainWindow *ui;

        ScreenshotWindow* s = nullptr;
        QPixmap currentPixmap;

        void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
