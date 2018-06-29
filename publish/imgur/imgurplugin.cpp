#include "imgurplugin.h"

ImgurPlugin::ImgurPlugin(QObject* parent) : QObject(parent)
{
    ui = new ImgurUI();
}

QString ImgurPlugin::serviceName() {
    return "Imgur";
}

QWidget* ImgurPlugin::widget() {
    return ui;
}

void ImgurPlugin::setPixmap(QPixmap px) {
    ui->setPixmap(px);
}
