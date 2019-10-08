#include <QtDebug>
#include <QJsonDocument>
#include <QJsonArray>

#include "ir.h"
#include "../remote-software/sources/entities/entity.h"

void IR::initialize(int integrationId, const QVariantMap& config, QObject* entities, QObject* notifications, QObject* api, QObject* configObj)
{
    setIntegrationId(integrationId);

    for (QVariantMap::const_iterator iter = config.begin(); iter != config.end(); ++iter) {
        if (iter.key() == "type")
            setType(iter.value().toString());
        else if (iter.key() == "friendly_name")
            setFriendlyName(iter.value().toString());
    }
    
    m_entities = qobject_cast<EntitiesInterface *>(entities);
    m_notifications = qobject_cast<NotificationsInterface *>(notifications);
    m_api = qobject_cast<YioAPIInterface *>(api);
    m_config = qobject_cast<ConfigInterface *>(configObj);
}

void HomeAssistant::connect()
{
    setState(CONNECTING);
}

void HomeAssistant::disconnect()
{
    setState(DISCONNECTED);
}

void HomeAssistant::sendCommand(const QString& type, const QString& entity_id, const QString& command, const QVariant& param)
{
}