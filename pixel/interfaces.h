#ifndef INTERFACES_H
#define INTERFACES_H

#include <QString>
#include <QWidget>
#include <QPixmap>

class PublishInterface {
    public:
        virtual ~PublishInterface() {}

        virtual QString serviceName() = 0;
        virtual QWidget* widget() = 0;

        virtual void setPixmap(QPixmap px) = 0;
};

#define PublishIterface_iid "org.thesuite.thePixel.PublishInterface"
Q_DECLARE_INTERFACE(PublishInterface, PublishIterface_iid)

#endif // INTERFACES_H
