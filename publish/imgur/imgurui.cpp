#include "imgurui.h"
#include "ui_imgurui.h"

ImgurUI::ImgurUI(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::ImgurUI)
{
    ui->setupUi(this);
}

ImgurUI::~ImgurUI()
{
    delete ui;
}

void ImgurUI::setPixmap(QPixmap px) {
    this->px = px;
    this->setCurrentIndex(0);
    ui->titleBox->setText("");
    ui->nameBox->setText("");
    ui->descriptionBox->setPlainText("");
}

void ImgurUI::on_publishButton_clicked()
{
    this->setCurrentIndex(1);

    QBuffer* imageData = new QBuffer();
    imageData->open(QBuffer::ReadWrite);
    if (!px.save(imageData, "PNG")) {
        imageData->deleteLater();

        tToast* toast = new tToast();
        toast->setTitle(tr("Error"));
        toast->setText(tr("Couldn't post image to Imgur."));
        connect(toast, SIGNAL(dismissed()), toast, SLOT(deleteLater()));
        toast->show(this->window());
        return;
    }
    imageData->seek(0);

    QHttpMultiPart* data = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart image;
    image.setBodyDevice(imageData);
    image.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
    image.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"image\"; filename=\"screenshot.png\"");
    data->append(image);
    imageData->setParent(data);

    if (ui->nameBox->text() != "") {
        QHttpPart name;
        name.setBody(ui->nameBox->text().toUtf8());
        name.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"name\"");
        data->append(name);
    }

    if (ui->titleBox->text() != "") {
        QHttpPart title;
        title.setBody(ui->titleBox->text().toUtf8());
        title.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"title\"");
        data->append(title);
    }

    if (ui->descriptionBox->toPlainText() != "") {
        QHttpPart desc;
        desc.setBody(ui->descriptionBox->toPlainText().toUtf8());
        desc.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"description\"");
        data->append(desc);
    }

    QNetworkRequest request;
    request.setUrl(QUrl("https://api.imgur.com/3/image"));
    //request.setUrl(QUrl("http://localhost:8000"));
    request.setRawHeader("Authorization", QString("Client-ID ").append(IMGUR_CLIENT_ID).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=\"" + data->boundary() + "\"");
    QNetworkReply* reply = mgr.post(request, data);
    data->setParent(reply);
    connect(reply, &QNetworkReply::finished, [=] {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject dataObj = doc.object().value("data").toObject();
            ui->imageLink->setText("https://imgur.com/" + dataObj.value("id").toString());
            this->setCurrentIndex(2);
        } else {
            this->setCurrentIndex(0);

            qDebug() << reply->readAll();

            tToast* toast = new tToast();
            toast->setTitle(tr("Error"));
            toast->setText(tr("Couldn't post image to Imgur."));
            connect(toast, SIGNAL(dismissed()), toast, SLOT(deleteLater()));
            toast->show(this->window());
        }

        reply->deleteLater();
    });
}

void ImgurUI::on_copyButton_clicked()
{
    QApplication::clipboard()->setText(ui->imageLink->text());

    tToast* toast = new tToast();
    toast->setTitle(tr("Copied"));
    toast->setText(tr("The link was copied to the clipboard."));
    connect(toast, SIGNAL(dismissed()), toast, SLOT(deleteLater()));
    toast->show(this->window());
}
