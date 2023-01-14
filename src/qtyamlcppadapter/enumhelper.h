/*
    Copyright 2023 by Ioan Calin Borcoman <iborco@gmail.com>
*/

#pragma once

#include <QDebug>
#include <QMetaEnum>
#include <QString>

namespace qtyamlcppadapter {
// Tool to convert enum values to/from QString
template <typename E>
std::optional<E> fromString(const QString &text) {
    bool ok;
    auto result = static_cast<E>(QMetaEnum::fromType<E>().keyToValue(text.toUtf8(), &ok));
    if (!ok) {
        if (!text.isEmpty()) {
            qDebug() << "Failed to convert enum" << text;
        }
        return {};
    }
    return result;
}

template <typename E>
QString toString(E value) {
    const int intValue = static_cast<int>(value);
    return QString::fromUtf8(QMetaEnum::fromType<E>().valueToKey(intValue));
}
}
