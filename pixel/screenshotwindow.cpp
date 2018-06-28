#include "screenshotwindow.h"
#include "ui_screenshotwindow.h"

ScreenshotWindow::ScreenshotWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenshotWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

ScreenshotWindow::~ScreenshotWindow()
{
    delete ui;
}

void ScreenshotWindow::takeScreen(QScreen* screen) {
    mode = Screenshot;
    screenshot = screen->grabWindow(0);

    this->show();
    this->setGeometry(screen->geometry());
    this->setFixedSize(screen->geometry().size());

    emit taken(screenshot);
}

void ScreenshotWindow::animateScreen(QRect animateTo) {
    this->clearMask();

    QRect coordinates;
    coordinates.setTopLeft(this->mapFromGlobal(animateTo.topLeft()));
    coordinates.setSize(animateTo.size());

    animation = new tVariantAnimation();
    animation->setStartValue(QRect(0, 0, this->width(), this->height()));
    animation->setEndValue(coordinates);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(500);
    connect(animation, SIGNAL(valueChanged(QVariant)), this, SLOT(repaint()));
    connect(animation, SIGNAL(finished()), this, SLOT(close()));
    animation->start();

    colAnimation = new tVariantAnimation();
    colAnimation->setStartValue(255);
    colAnimation->setEndValue(0);
    colAnimation->setEasingCurve(QEasingCurve::OutCubic);
    colAnimation->setDuration(500);
    colAnimation->start();
}

void ScreenshotWindow::paintEvent(QPaintEvent *event) {
    QPainter p(this);

    switch (mode) {
        case Countdown: {
            p.setPen(Qt::transparent);
            p.setBrush(Qt::transparent);
            p.drawRect(0, 0, this->width(), this->height());

            p.setPen(Qt::black);
            p.setPen(Qt::white);

            QFont font = this->font();
            font.setPointSize(50);
            p.setFont(font);

            QFontMetrics metrics(font);
            QRect rect(this->width() - 30 - metrics.width(QString::number(countdownRemaining)), 30, metrics.width(QString::number(countdownRemaining)), metrics.ascent());
            p.drawText(rect, QString::number(countdownRemaining));

            QRegion r(rect);
            this->setMask(r);
            break;
        }
        case Screenshot: {
            if (animation != nullptr) {
                p.setPen(Qt::transparent);
                p.setBrush(QColor(0, 0, 0, colAnimation->currentValue().toInt()));
                p.drawRect(0, 0, this->width(), this->height());

                p.drawPixmap(animation->currentValue().toRect(), screenshot);
            }
            break;
        }
    }
}

void ScreenshotWindow::countdown(int seconds, QScreen* screen) {
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    mode = Countdown;
    countdownRemaining = seconds;

    this->show();

    if (screen == nullptr) {
        screen = QApplication::primaryScreen();
    }
    this->setGeometry(screen->geometry());
    this->setFixedSize(screen->geometry().size());

    QEventLoop* loop = new QEventLoop;

    QTimer* timer = new QTimer();
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, [=] {
        countdownRemaining--;
        if (countdownRemaining == 0) {
            timer->stop();
            timer->deleteLater();
            loop->exit();
            loop->deleteLater();
            this->hide();
        } else {
            this->repaint();
        }
    });
    timer->start();

    loop->exec();
}
