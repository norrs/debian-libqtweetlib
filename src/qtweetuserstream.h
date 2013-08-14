/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef QTWEETUSERSTREAM_H
#define QTWEETUSERSTREAM_H

#include <QObject>
#include <QNetworkReply>
#include "qtweetlib_global.h"

#ifdef STREAM_LOGGER
    #include <QFile>
#endif

class QNetworkAccessManager;
class QNetworkReply;
class OAuthTwitter;
class QAuthenticator;
class QTimer;
class QTweetStatus;
class QTweetDMStatus;

/**
 *   Class for fetching user stream
 */
class QTWEETLIBSHARED_EXPORT QTweetUserStream : public QObject
{
    Q_OBJECT
public:
    QTweetUserStream(QObject *parent = 0);
    void setOAuthTwitter(OAuthTwitter* oauthTwitter);
    OAuthTwitter* oauthTwitter() const;

signals:
    /**
     *   Emits stream elements
     */
    void stream(const QByteArray& );
    /**
     *   Emits tweets (parsed) elements from stream
     */
    void statusesStream(const QTweetStatus& status);
    /**
     *   Emits friends list of id of authenticated user.
     *   Emited immediately after connecting to the user stream.
     *   If there is no reconnect it won't be emited again.
     */
    void friendsList(const QList<qint64> friends);
    /**
     *   Emits direct message when is arrived in the stream
     */
    void directMessageStream(const QTweetDMStatus& directMessage);
    /**
     *   Emits deletion of status in the stream
     */
    void deleteStatusStream(qint64 id, qint64 userid);
    /**
     *  Emited when user stream is reconnected after failure
     *  Usefull when user stream connection fails to fetch missed tweets with REST API
     */
    void reconnected();
    /**
     * Emited when user stream doesn't connect and backoff timer reaches maximum value (300 seconds)
     * Usefull when users stream fails to revert to REST API
     */
    void failureConnect();

public slots:
    void startFetching();

private slots:
    void replyFinished();
    void replyReadyRead();
    void replyTimeout();
    void parsingFinished(const QVariant& json, bool ok, const QString& errorMsg);

private:
    void parseStream(const QByteArray& );
    void parseFriendsList(const QVariantMap& streamObject);
    void parseDirectMessage(const QVariantMap& streamObject);
    void parseDeleteStatus(const QVariantMap& streamObject);

    QByteArray m_cachedResponse;
    OAuthTwitter *m_oauthTwitter;
    QNetworkReply *m_reply;
    QTimer *m_backofftimer;
    QTimer *m_timeoutTimer;
    bool m_streamTryingReconnect;

#ifdef STREAM_LOGGER
    QFile m_streamLog;
#endif
};

#endif // QTWEETUSERSTREAM_H
