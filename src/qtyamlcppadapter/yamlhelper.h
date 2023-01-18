/*
    Copyright 2023 by Ioan Calin Borcoman <iborco@gmail.com>
*/

#pragma once

#include <QDateTime>
#include <QStringList>

#include "enumhelper.h"

namespace YAML {
class Emitter;
class Node;
}

namespace qtyamlcppadapter {

void emitYaml(YAML::Emitter& out, const char* key, const QString& value);
void emitYaml(YAML::Emitter& out, const char* key, bool value);
void emitYaml(YAML::Emitter& out, const char* key, quint64 value);
void emitYaml(YAML::Emitter& out, const char* key, int value);
void emitYaml(YAML::Emitter& out, const char* key, const QStringList& list, bool compact = true);
void emitYamlLiteral(YAML::Emitter& out, const char* key, const QString& value);

bool boolFromYaml(const YAML::Node& node, const char* key, bool defaultValue = false);
int intFromYaml(const YAML::Node& node, const char* key, int defaultValue = 0);
quint64 quint64FromYaml(const YAML::Node &node, const char *key, quint64 defaultValue = 0UL);
QString stringFromYaml(const YAML::Node& node, const char* key, const QString& defaultValue = QString());
QStringList stringListFromYaml(const YAML::Node& node, const char* key, bool *needsSave = nullptr, const QStringList& defaultValue = QStringList());
QDateTime dateTimeFromYaml(const YAML::Node& node, const char* key, const QDateTime& defaultValue = QDateTime());

template <typename E>
E enumFromYaml(const YAML::Node &node, const char *key, E defaultValue) {
    auto repr = stringFromYaml(node, key, toString(defaultValue));
    auto optional_value = fromString<E>(repr);
    return optional_value ? optional_value.value() : defaultValue;
}

}
