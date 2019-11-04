#ifndef IR_H
#define IR_H

#include <QObject>
#include <QString>
#include <QVariant>

#include "../remote-software/sources/integrations/integration.h"
#include "../remote-software/sources/integrations/integrationinterface.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// IR FACTORY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IR : public IntegrationInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "YIO.IntegrationInterface" FILE "ir.json")
    Q_INTERFACES(IntegrationInterface)

public:
    explicit IR() {}

    void create          (const QVariantMap& config, QObject *entities, QObject *notifications, QObject* api, QObject *configObj) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// IR BASE CLASS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class IRBase : public Integration
{
    Q_OBJECT

public:
    explicit IRBase();

    Q_INVOKABLE void setup   	    (const QVariantMap& config, QObject *entities, QObject *notifications, QObject* api, QObject *configObj);
    Q_INVOKABLE void connect	    ();
    Q_INVOKABLE void disconnect	    ();

signals:
    void notify();

public slots:
     void sendCommand               (const QString& type, const QString& id, const QString& command, const QVariant& param);

private:
    void updateEntity               (const QString& entity_id, const QVariantMap& attr);
    QString findIRCode              (const QString& feature, QVariantList& list);

    EntitiesInterface*              m_entities;
    NotificationsInterface*         m_notifications;
    YioAPIInterface*                m_api;
    ConfigInterface*                m_config;

};

#endif // IR_H
