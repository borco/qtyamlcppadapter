/*
    Copyright 2023 by Ioan Calin Borcoman <iborco@gmail.com>
*/

#pragma once

#include <QDateTime>
#include <QStringList>

#include "iborcocpputils/enumhelper.h"

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
void emitYaml(YAML::Emitter& out, const char* key, const QDate& value);
void emitYaml(YAML::Emitter& out, const char* key, const QDateTime& value);
void emitYamlLiteral(YAML::Emitter& out, const char* key, const QString& value);

bool boolFromYaml(const YAML::Node& node, const char* key, bool defaultValue = false);
int intFromYaml(const YAML::Node& node, const char* key, int defaultValue = 0);
quint64 quint64FromYaml(const YAML::Node &node, const char *key, quint64 defaultValue = 0UL);
QString stringFromYaml(const YAML::Node& node, const char* key, const QString& defaultValue = QString());
QStringList stringListFromYaml(const YAML::Node& node, const char* key, bool *needsSave = nullptr, const QStringList& defaultValue = QStringList());
QDate dateFromYaml(const YAML::Node& node, const char* key, const QDate& defaultValue = QDate());
QDateTime dateTimeFromYaml(const YAML::Node& node, const char* key, const QDateTime& defaultValue = QDateTime());

template <typename E>
E enumFromYaml(const YAML::Node &node, const char *key, E defaultValue) {
    auto repr = stringFromYaml(node, key, iborcocpputils::toString(defaultValue));
    auto optional_value = iborcocpputils::fromString<E>(repr);
    return optional_value ? optional_value.value() : defaultValue;
}

template <typename E>
void emitYaml(YAML::Emitter& out, const char* key, E value) {
    emitYaml(out, key, iborcocpputils::toString(value));
}

}
