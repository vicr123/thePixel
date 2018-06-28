#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    s = new ScreenshotWindow();
    connect(s, &ScreenshotWindow::taken, [=](QPixmap px) {
        this->show();
        ui->stackedWidget->setCurrentIndex(1);

        currentPixmap = px;
        QPixmap labelPixmap = currentPixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel->setPixmap(labelPixmap);

        QRect animateTo;
        animateTo.setLeft(this->geometry().left() + (ui->imageLabel->width() / 2 - labelPixmap.width() / 2));
        animateTo.setTop(this->geometry().top() + ui->imageLabel->geometry().top() + (ui->imageLabel->height() / 2 - labelPixmap.height() / 2));
        animateTo.setSize(labelPixmap.size());

        s->animateScreen(animateTo);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_takeButton_clicked()
{
    this->hide();
    if (ui->screenshotDelay->value() > 0) {
        s->countdown(ui->screenshotDelay->value(), this->windowHandle()->screen());
    }

    QThread::msleep(100);
    s->takeScreen(this->windowHandle()->screen());
}

void MainWindow::updateScreenshotLabel() {
    QPixmap labelPixmap = currentPixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(labelPixmap);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    updateScreenshotLabel();
}
