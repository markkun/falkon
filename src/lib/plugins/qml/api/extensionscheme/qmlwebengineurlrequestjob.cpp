/* ============================================================
* Falkon - Qt web browser
* Copyright (C) 2018 Anmol Gautam <tarptaeya@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "qmlwebengineurlrequestjob.h"
#include <QBuffer>
#include <QVariantMap>
#include <qztools.h>

QmlWebEngineUrlRequestJob::QmlWebEngineUrlRequestJob(QWebEngineUrlRequestJob *job, QObject *parent)
    : QObject(parent)
    , m_job(job)
{
}

/**
 * @brief Fails the request with the error
 * @param error
 */
void QmlWebEngineUrlRequestJob::fail(QmlWebEngineUrlRequestJob::Error error)
{
    if (!m_job) {
        return;
    }
    m_job->fail(QWebEngineUrlRequestJob::Error(error));
}

/**
 * @brief Redirects the request to the url
 * @param urlString, represents the url to which the request is to be redirected
 */
void QmlWebEngineUrlRequestJob::redirect(const QString &urlString)
{
    if (!m_job) {
        return;
    }
    return m_job->redirect(QUrl::fromEncoded(urlString.toUtf8()));
}

/**
 * @brief Replies to the request
 * @param A JavaScript object containing
 *        - content: String representing the reply data
 *        - contentType: String representing the contentType of reply data
 */
void QmlWebEngineUrlRequestJob::reply(const QVariantMap &map)
{
    if (!m_job) {
        return;
    }
    if (!map.contains(QSL("content")) || !map.contains(QSL("contentType"))) {
        qWarning() << "Unable to call" << __FUNCTION__ << ": invalid parameters";
        return;
    }
    QByteArray content = map.value(QSL("content")).toString().toUtf8();
    QByteArray contentType = map.value(QSL("contentType")).toString().toUtf8();
    QBuffer *buffer = new QBuffer();
    buffer->open(QIODevice::ReadWrite);
    buffer->write(content);
    buffer->seek(0);
    m_job->reply(contentType, buffer);
}

QString QmlWebEngineUrlRequestJob::initiator() const
{
    if (!m_job) {
        return QString();
    }
    return QString::fromUtf8(m_job->initiator().toEncoded());
}

QString QmlWebEngineUrlRequestJob::requestUrl() const
{
    if (!m_job) {
        return QString();
    }
    return QString::fromUtf8(m_job->requestUrl().toEncoded());
}

QString QmlWebEngineUrlRequestJob::requestMethod() const
{
    if (!m_job) {
        return QString();
    }
    return QString::fromUtf8(m_job->requestMethod());
}