#ifndef IMGURUI_H
#define IMGURUI_H

#include <QStackedWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QClipboard>
#include <ttoast.h>

#define IMGUR_CLIENT_ID "9e004b6fa4218f7"

namespace Ui {
    class ImgurUI;
}

class ImgurUI : public QStackedWidget
{
        Q_OBJECT

    public:
        explicit ImgurUI(QWidget *parent = 0);
        ~ImgurUI();

        void setPixmap(QPixmap px);

    private slots:

        void on_publishButton_clicked();

        void on_copyButton_clicked();

    private:
        Ui::ImgurUI *ui;

        QPixmap px;
        QNetworkAccessManager mgr;
};

#endif // IMGURUI_H
