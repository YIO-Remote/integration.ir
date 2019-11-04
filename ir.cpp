#include <QtDebug>
#include <QJsonDocument>
#include <QJsonArray>

#include "ir.h"
#include "../remote-software/sources/entities/entity.h"
#include "../remote-software/sources/entities/remote.h"

void IR::create(const QVariantMap &config, QObject *entities, QObject *notifications, QObject *api, QObject *configObj)
{
    QMap<QObject *, QVariant> returnData;

    IRBase* ir = new IRBase();
    ir->setup(QVariantMap(), entities, notifications, api, configObj);

    QVariantMap d ;
    d.insert("id", "ir");
    d.insert("friendly_name", "IR");
    d.insert("type", config.value("type").toString());
    returnData.insert(ir, d);

    emit createDone(returnData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// IR BASE CLASS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IRBase::IRBase()
{
}

void IRBase::setup(const QVariantMap& config, QObject* entities, QObject* notifications, QObject* api, QObject* configObj)
{
    setFriendlyName("IR");
    setIntegrationId("ir");

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
        QString IRcommand = findIRCode(command, commands);

        QVariantMap msg;
        msg.insert("type", QVariant("dock"));
        msg.insert("command", QVariant("ir_send"));
        msg.insert("code", IRcommand);
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
