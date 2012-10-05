/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: http://www.qt-project.org/
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**************************************************************************/

#include "qmlconsoleproxymodel.h"
#include "qmlconsoleitemmodel.h"

namespace QmlJSTools {
namespace Internal {

QmlConsoleProxyModel::QmlConsoleProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_filter(QmlConsoleItem::DefaultTypes)
{
}

void QmlConsoleProxyModel::setShowLogs(bool show)
{
    m_filter = show ? m_filter | QmlConsoleItem::DebugType : m_filter & ~QmlConsoleItem::DebugType;
    setFilterRegExp(QString());
}

void QmlConsoleProxyModel::setShowWarnings(bool show)
{
    m_filter = show ? m_filter | QmlConsoleItem::WarningType
                    : m_filter & ~QmlConsoleItem::WarningType;
    setFilterRegExp(QString());
}

void QmlConsoleProxyModel::setShowErrors(bool show)
{
    m_filter = show ? m_filter | QmlConsoleItem::ErrorType : m_filter & ~QmlConsoleItem::ErrorType;
    setFilterRegExp(QString());
}

void QmlConsoleProxyModel::selectEditableRow(const QModelIndex &index,
                           QItemSelectionModel::SelectionFlags command)
{
    emit setCurrentIndex(mapFromSource(index), command);
}

bool QmlConsoleProxyModel::filterAcceptsRow(int sourceRow,
         const QModelIndex &sourceParent) const
 {
     QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
     return m_filter.testFlag((QmlConsoleItem::ItemType)sourceModel()->data(
                                  index, QmlConsoleItemModel::TypeRole).toInt());
 }

void QmlConsoleProxyModel::onRowsInserted(const QModelIndex &index, int start, int end)
{
    int rowIndex = end;
    do {
        if (filterAcceptsRow(rowIndex, index)) {
            emit scrollToBottom();
            break;
        }
    } while (--rowIndex >= start);
}

} // Internal
} // QmlJSTools