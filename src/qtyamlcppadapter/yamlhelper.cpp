/*
    Copyright 2023 by Ioan Calin Borcoman <iborco@gmail.com>
*/

#include "yamlhelper.h"

#include "yaml-cpp/yaml.h" // IWYU pragma: keep

#include <QDebug>

namespace qtyamlcppadapter {

void emitYaml(YAML::Emitter &out, const char *key, const QString &value)
{
    out << YAML::Key << key << YAML::Value << value.toStdString();
}

void emitYaml(YAML::Emitter &out, const char *key, bool value)
{
    out << YAML::Key << key << YAML::Value << value;
}

void emitYaml(YAML::Emitter &out, const char *key, quint64 value)
{
    out << YAML::Key << key << YAML::Value << value;
}

void emitYaml(YAML::Emitter &out, const char *key, int value)
{
    out << YAML::Key << key << YAML::Value << value;
}

void emitYaml(YAML::Emitter &out, const char *key, const QStringList &list, bool compact)
{
    if (!list.isEmpty()) {
        out << YAML::Key << key << YAML::Value;
        if (compact) out << YAML::Flow;
        out << YAML::BeginSeq;
        foreach(QString value, list)
            if (!value.isEmpty())
                out << value.toStdString();
        out << YAML::EndSeq << YAML::Auto;
    }
}

void emitYaml(YAML::Emitter &out, const char *key, const QDate &value)
{
    out << YAML::Key << key << YAML::Value << value.toString(Qt::ISODate).toStdString();
}

void emitYaml(YAML::Emitter &out, const char *key, const QDateTime &value)
{
    out << YAML::Key << key << YAML::Value << value.toUTC().toString(Qt::ISODate).toCFString();
}

void emitYamlLiteral(YAML::Emitter &out, const char *key, const QString &value)
{
    if (value.isEmpty())
        return;

    out << YAML::Key << key << YAML::Literal << value.toStdString() << YAML::Auto;
}

bool boolFromYaml(const YAML::Node &node, const char *key, bool defaultValue)
{
    return node[key] ? node[key].as<bool>() : defaultValue;
}

int intFromYaml(const YAML::Node &node, const char *key, int defaultValue)
{
    return node[key] ? node[key].as<int>() : defaultValue;
}

quint64 quint64FromYaml(const YAML::Node &node, const char *key, quint64 defaultValue)
{
    return node[key] ? node[key].as<quint64>() : defaultValue;
}

QString stringFromYaml(const YAML::Node &node, const char *key, const QString &defaultValue)
{
    return node[key] ? QString::fromStdString(node[key].as<std::string>()) : defaultValue;
}

QStringList stringListFromYaml(const YAML::Node &node, const char *key, bool *needsSave, const QStringList &defaultValue)
{
    if (!node[key]) {
        return defaultValue;
    }

    QStringList string_list;

    auto nodes_sequence = node[key];

    if (node[key].IsSequence()) {
            for (YAML::const_iterator it = nodes_sequence.begin(); it != nodes_sequence.end(); ++it) {
                string_list << QString::fromStdString(it->as<std::string>()).trimmed();
            }
    } else if (needsSave) {
        // scalar fallback is valid only if needsSave is not null
        if (node[key].IsScalar()) {
            auto scalar = QString::fromStdString(node[key].as<std::string>()).trimmed();
            if (!scalar.isEmpty()) {
                for (const auto& value: scalar.split(',')) {
                    string_list << value.trimmed();
                }
                *needsSave = true;
            }
        } else {
            throw QString("node %1 is neither scalar, nor list").arg(key);
        }
    } else {
        throw QString("node %1 not a list").arg(key);
    }

    return string_list;
}

QDateTime dateTimeFromYaml(const YAML::Node &node, const char *key, const QDateTime &defaultValue)
{
    if (!node[key])
        return defaultValue;

    auto repr = QString::fromStdString(node[key].as<std::string>());
    return QDateTime::fromString(repr, Qt::ISODate);
}

QDate dateFromYaml(const YAML::Node &node, const char *key, const QDate &defaultValue)
{
    if (!node[key])
        return defaultValue;

    auto repr = QString::fromStdString(node[key].as<std::string>());
    return QDate::fromString(repr, Qt::ISODate);
}

}
