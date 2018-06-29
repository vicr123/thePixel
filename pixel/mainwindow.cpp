#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Load upload plugins
    QDir uploadPlugins("/usr/share/thepixel/publish");
    for (QString file : uploadPlugins.entryList(QDir::Files)) {
        QPluginLoader loader(uploadPlugins.absoluteFilePath(file));
        QObject* plugin = loader.instance();
        if (plugin != nullptr) {
            PublishInterface* p = qobject_cast<PublishInterface*>(plugin);
            if (p) {
                ui->publishItems->addItem(p->serviceName());
                ui->publishPages->addWidget(p->widget());
                connect(this, &MainWindow::newPixmap, [=](QPixmap px) {
                    p->setPixmap(px);
                });
            }
        }
    }

    if (ui->publishItems->count() == 0) {
        QWidget* w = new QWidget();
        QBoxLayout* l = new QBoxLayout(QBoxLayout::TopToBottom);
        w->setLayout(l);

        l->addStretch(0);

        QLabel* warningLabel = new QLabel();
        warningLabel->setText(tr("No publish providers"));
        warningLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QFont f = warningLabel->font();
        f.setPointSize(20);
        warningLabel->setFont(f);
        l->addWidget(warningLabel);

        QLabel* descLabel = new QLabel();
        descLabel->setText(tr("Go ahead and install one from your package manager"));
        descLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        l->addWidget(descLabel);

        l->addStretch(0);

        ui->publishPages->addWidget(w);
    }

    ui->backButton->click();
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

void MainWindow::on_backButton_clicked()
{
    if (s != nullptr) {
        s->deleteLater();
    }

    s = new ScreenshotWindow();
    connect(s, &ScreenshotWindow::taken, [=](QPixmap px) {
        this->show();
        ui->stackedWidget->setCurrentIndex(1);

        currentPixmap = px;
        newPixmap(px);
        QPixmap labelPixmap = currentPixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel->setPixmap(labelPixmap);

        QRect animateTo;
        animateTo.setLeft(this->geometry().left() + (ui->imageLabel->width() / 2 - labelPixmap.width() / 2));
        animateTo.setTop(this->geometry().top() + ui->imageLabel->geometry().top() + (ui->imageLabel->height() / 2 - labelPixmap.height() / 2));
        animateTo.setSize(labelPixmap.size());

        s->animateScreen(animateTo);
    });
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_copyButton_clicked()
{
    QApplication::clipboard()->setPixmap(this->currentPixmap);

    tToast* toast = new tToast();
    toast->setTitle(tr("Copied"));
    toast->setText(tr("This screenshot was copied to the clipboard."));
    connect(toast, SIGNAL(dismissed()), toast, SLOT(deleteLater()));
    toast->show(this);
}

void MainWindow::on_saveButton_clicked()
{
    QFileDialog* dialog = new QFileDialog();
    dialog->setWindowTitle(tr("Save Screenshot"));
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setNameFilters(QStringList() << "PNG images (*.png)");
    if (dialog->exec() == dialog->Accepted) {
        if (currentPixmap.save(dialog->selectedFiles().first())) {
            tToast* toast = new tToast();
            toast->setTitle(tr("Saved"));
            toast->setText(tr("This screenshot was saved to disk."));
            connect(toast, SIGNAL(dismissed()), toast, SLOT(deleteLater()));
            toast->show(this);
        } else {
            QMessageBox::critical(this, tr("Couldn't save screenshot"), tr("The screenshot was unable to be saved."), QMessageBox::Ok, QMessageBox::Ok);
        }
    }
}

void MainWindow::on_publishButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_backButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_publishItems_currentRowChanged(int currentRow)
{
    ui->publishPages->setCurrentIndex(currentRow);
}
