#include <QtDebug>
#include <QJsonDocument>
#include <QJsonArray>

#include "ir.h"
#include "../remote-software/sources/entities/entity.h"
#include "../remote-software/sources/entities/remote.h"

QMap<QObject *, QVariant> IR::create(const QVariantMap &config, QObject *entities, QObject *notifications, QObject *api, QObject *configObj)
{
    QMap<QObject *, QVariant> returnData;

    QVariantList data;
    QString mdns;

    for (QVariantMap::const_iterator iter = config.begin(); iter != config.end(); ++iter) {
        if (iter.key() == "mdns") {
            mdns = iter.value().toString();
        } else if (iter.key() == "data") {
            data = iter.value().toList();
        }
    }

    for (int i=0; i<data.length(); i++)
    {
        IRBase* ir = new IRBase();
        ir->setup(data[i].toMap(), entities, notifications, api, configObj);

        QVariantMap d = data[i].toMap();
        d.insert("mdns", mdns);
        returnData.insert(ir, d);
    }

    return returnData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// IR BASE CLASS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IRBase::IRBase()
{
}

void IRBase::setup(const QVariantMap& config, QObject* entities, QObject* notifications, QObject* api, QObject* configObj)
{
    for (QVariantMap::const_iterator iter = config.begin(); iter != config.end(); ++iter) {
        if (iter.key() == "friendly_name")
            setFriendlyName(iter.value().toString());
        else if (iter.key() == "id")
            setIntegrationId(iter.value().toString());
    }
    
    m_entities = qobject_cast<EntitiesInterface *>(entities);
    m_notifications = qobject_cast<NotificationsInterface *>(notifications);
    m_api = qobject_cast<YioAPIInterface *>(api);
    m_config = qobject_cast<ConfigInterface *>(configObj);

    connect();
}

void IRBase::connect()
{
    setState(CONNECTED);
}

void IRBase::disconnect()
{
    setState(DISCONNECTED);
}

void IRBase::sendCommand(const QString& type, const QString& entity_id, const QString& command, const QVariant& param)
{
    if (type == "remote") {
         Remote* entity = (Remote*)m_entities->get(entity_id);
         QVariantList commands = entity->commands();
         QString command = findIRCode(command, commands);

         QVariantMap msg;
         msg.insert("type", QVariant("dock"));
         msg.insert("command", QVariant("ir_send"));
         msg.insert("code", command);
         QJsonDocument doc = QJsonDocument::fromVariant(msg);
         QString message = doc.toJson(QJsonDocument::JsonFormat::Compact);

        if (command != "") {
            // send the message through the websocket api
            m_api->sendMessage(message);
        }
    }
}

void IRBase::updateEntity(const QString& entity_id, const QVariantMap& attr)
{
    // not used for IR entites. IR entities are not updated
}

QString IRBase::findIRCode(const QString &feature, QVariantList& list)
{
    QString r = "";

    for (int i = 0; i < list.length(); i++) {
        QVariantMap map =  list[i].toMap();

        if (map.value("button_map").toString() == feature) {
            r = map.value("code").toString();
        }
    }

    return r;
}
