#ifndef IR_H
#define IR_H

#include <QObject>
#include <QString>
#include <QVariant>

#include "../remote-software/sources/integrations/integration.h"
#include "../remote-software/sources/integrations/integrationinterface.h"

class IR : public Integration, IntegrationInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "YIO.IntegrationInterface" FILE "ir.json")
    Q_INTERFACES(IntegrationInterface)

public:
    explicit IR();

    Q_INVOKABLE void initialize	    (int integrationId, const QVariantMap& config, QObject *entities, QObject *notifications, QObject* api, QObject *configObj) override;
    Q_INVOKABLE void connect	      () override;
    Q_INVOKABLE void disconnect	    () override;

signals:
    void notify();

public slots:
     void sendCommand               (const QString& type, const QString& id, const QString& command, const QVariant& param) override;

private:
    void updateEntity               (const QString& entity_id, const QVariantMap& attr) override;

    EntitiesInterface*              m_entities;
    NotificationsInterface*         m_notifications;
    YioAPIInterface*                m_api;
    ConfigInterface*                m_config;

};

#endif // IR_H
