#pragma once

#include <QStringList>

#include "yaml-cpp/emitter.h"

namespace qtyamlcppadapter {

QString formattedDuration(quint64 duration);
quint64 parsedDuration(const QString& value);

QString formattedFileSize(quint64 size);
quint64 parsedFileSize(const QString& size);

void emitYaml(YAML::Emitter& out, const char* key, const QString& value);
void emitYaml(YAML::Emitter& out, const char* key, bool value);
void emitYaml(YAML::Emitter& out, const char* key, quint64 value);
void emitYaml(YAML::Emitter& out, const char* key, int value);
void emitYaml(YAML::Emitter& out, const char* key, const QStringList& list, bool compact = true);
void emitYamlLiteral(YAML::Emitter& out, const char* key, const QString& value);

bool boolFromYaml(const YAML::Node& node, const char* key, bool defaultValue = false);
int intFromYaml(const YAML::Node& node, const char* key, int defaultValue = 0);
quint64 quint64FromYaml(const YAML::Node &node, const char *key, quint64 defaultValue = 0UL);
quint64 fileSizeFromYaml(const YAML::Node &node, const char *key, quint64 defaultValue = 0UL);
quint64 durationFromYaml(const YAML::Node &node, const char *key, quint64 defaultValue = 0UL);
QString stringFromYaml(const YAML::Node& node, const char* key, const QString& defaultValue = QString());
QStringList stringListFromYaml(const YAML::Node& node, const char* key, bool *needsSave = nullptr, const QStringList& defaultValue = QStringList());

}
