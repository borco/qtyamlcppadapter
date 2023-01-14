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

quint64 fileSizeFromYaml(const YAML::Node &node, const char *key, quint64 defaultValue)
{
    try {
        return quint64FromYaml(node, key, defaultValue);
    } catch (...) {
        return parsedFileSize(stringFromYaml(node, key));
    }
}

quint64 durationFromYaml(const YAML::Node &node, const char *key, quint64 defaultValue)
{
    try {
        return quint64FromYaml(node, key, defaultValue);
    } catch (...) {
        return parsedDuration(stringFromYaml(node, key));
    }
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

QString formattedDuration(quint64 duration)
{
    if (duration == 0) {
        return QString();
    }

    const quint64 minute = 60;
    const quint64 hour = 60 * minute;

    quint64 hours = duration / hour;
    quint64 minutes = (duration % hour) / minute;
    quint64 seconds = duration % minute;

    QStringList repr;

    if (hours) {
        repr.append(QString("%1h").arg(hours));
    }
    if (minutes) {
        repr.append(QString("%1m").arg(minutes));
    }
    if (seconds && hours == 0) {
        repr.append(QString("%1s").arg(seconds));
    }

    return repr.join(" ");
}

quint64 parsedDuration(const QString &value)
{
    auto tokens = value.split(" ");
    quint64 duration = 0UL;
    const quint64 minute = 60UL;
    const quint64 hour = 60 * minute;

    for (const auto& token: tokens) {
        if (token.length() >= 1) {
            auto suffix = token.right(1);
            auto value = token.first(token.length() - 1).toULongLong();

            if (suffix == "h") {
                duration += value * hour;
            } else if (suffix == "m") {
                duration += value * minute;
            } else if (suffix == "s") {
                duration += value;
            }
        }
    }

//    qDebug() << "Parsed string duration:" << value << "->" << duration << "s";

    return duration;
}

QString formattedFileSize(quint64 size)
{
    if (size == 0) {
        return QString();
    }

    const char* suffix[]{"B", "KB", "MB", "GB", "TB", "PB", "EB"};
    const quint64 base{1000UL}; // use 1000 to get same results as macOS Finder
    int i{0};
    double mantissa = size;
    for (; std::ceil(mantissa) >= base && i < sizeof(suffix); mantissa /= base, ++i) { }
    mantissa = std::ceil(mantissa * 100.) / 100.;
    return QString("%1 %2").arg(mantissa, 0, 'f', 2).arg(suffix[i]);
}

quint64 parsedFileSize(const QString &value)
{
    auto values = value.split(" ");
    auto raw_size = values[0].toDouble();
    QString suffix = values.length() > 1 ? values[1] : "B";
    quint64 size {0UL};
    const quint64 base {1000UL}; // use 1000 to get same results as macOS Finder

    if (suffix == "B") {
        size = raw_size;
    } else if (suffix == "KB") {
        size = raw_size * base;
    } else if (suffix == "MB") {
        size = raw_size *  base * base;
    } else if (suffix == "GB") {
        size = raw_size *  base * base * base;
    } else if (suffix == "TB") {
        size = raw_size *  base * base * base * base;
    } else if (suffix == "PB") {
        size = raw_size *  base * base * base * base * base;
    } else if (suffix == "EB") {
        size = raw_size *  base * base * base * base * base * base;
    }

//    qDebug() << "Parsed file size:" << value << "->" << size;

    return size;
}

}
