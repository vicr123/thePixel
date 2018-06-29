#ifndef IMGURPLUGIN_H
#define IMGURPLUGIN_H

#include "../../pixel/interfaces.h"
#include "imgurui.h"

class ImgurPlugin : public QObject, public PublishInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PublishIterface_iid FILE "metadata.json")
    Q_INTERFACES(PublishInterface)

    public:
        explicit ImgurPlugin(QObject* parent = nullptr);

        QString serviceName();
        QWidget* widget();

        void setPixmap(QPixmap px);

    private:
        ImgurUI* ui;
};

#endif // IMGURPLUGIN_H
