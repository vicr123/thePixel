#ifndef SCREENSHOTWINDOW_H
#define SCREENSHOTWINDOW_H

#include <QWidget>
#include <QScreen>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QFileDialog>
#include <tvariantanimation.h>

namespace Ui {
    class ScreenshotWindow;
}

class ScreenshotWindow : public QWidget
{
        Q_OBJECT

        enum WindowMode {
            None,
            Screenshot,
            Countdown
        };

    public:
        explicit ScreenshotWindow(QWidget *parent = 0);
        ~ScreenshotWindow();

    public slots:
        void countdown(int seconds, QScreen* screen = nullptr);

        void takeScreen(QScreen* screen);

        void animateScreen(QRect animateTo);

    signals:
        void taken(QPixmap px);

    private:
        Ui::ScreenshotWindow *ui;

        QPixmap screenshot;
        tVariantAnimation *animation = nullptr, *colAnimation = nullptr;
        int countdownRemaining;

        WindowMode mode = None;

        void paintEvent(QPaintEvent* event);
};

#endif // SCREENSHOTWINDOW_H
