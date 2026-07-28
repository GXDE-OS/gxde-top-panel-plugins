//
// Created by ragdoll on 2021/5/18.
//

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include "QQLyricFetcher.h"

QString QQLyricFetcher::searchUrl("https://c.y.qq.com/soso/fcgi-bin/client_search_cp?p=1&n=2&w=%1&format=json");
QString QQLyricFetcher::lyricUrl("https://c.y.qq.com/lyric/fcgi-bin/fcg_query_lyric_new.fcg?songmid=%1&musicid=%2&format=json&nobase64=1");

void QQLyricFetcher::requestForLyric(QString title, QString artist, QString album) {
    QNetworkRequest request;
    QString s = title + ' ' + artist;
    if (album != title) {
        s += ' ' + album;
    }
    QUrl url(searchUrl.arg(s));
    request.setUrl(url);
    this->naManager->get(request);
}

QQLyricFetcher::QQLyricFetcher() {
    this->naManager = new QNetworkAccessManager(this);
    connect(this->naManager, &QNetworkAccessManager::finished, this, &QQLyricFetcher::songGetFinished);
}


void QQLyricFetcher::songGetFinished(QNetworkReply *reply) {
    std::cout << reply->request().url().toString().toStdString() << std::endl;
    // 网络错误检查
    if (reply->error() != QNetworkReply::NoError) {
        std::cerr << "[QQLyricFetcher] Network error: "
                  << reply->errorString().toStdString() << std::endl;
        reply->deleteLater(); // 修复内存泄漏
        return;
    }

    // 解析 JSON
    QByteArray body = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        std::cerr << "[QQLyricFetcher] JSON parse error: "
                  << parseError.errorString().toStdString() << std::endl;
        reply->deleteLater();
        return;
    }

    QJsonObject jsonResponse = jsonDoc.object();
    if (jsonResponse.isEmpty()) {
        reply->deleteLater();
        return;
    }

    // 搜索 API 分支
    if (reply->request().url().toString().startsWith("https://c.y.qq.com/soso/fcgi-bin/client_search_cp")) {
        int code = jsonResponse["code"].toInt();
        std::cout << "code: " << code << std::endl;

        if (code == 0) {
            QJsonValue dataVal = jsonResponse.value("data");
            if (!dataVal.isObject()) {
                std::cerr << "[QQLyricFetcher] 'data' is missing or not an object" << std::endl;
                reply->deleteLater();
                return;
            }

            QJsonValue songVal = dataVal.toObject().value("song");
            if (!songVal.isObject()) {
                std::cerr << "[QQLyricFetcher] 'song' is missing or not an object" << std::endl;
                reply->deleteLater();
                return;
            }

            QJsonValue listVal = songVal.toObject().value("list");
            if (!listVal.isArray()) {
                std::cerr << "[QQLyricFetcher] 'list' is missing or not an array" << std::endl;
                reply->deleteLater();
                return;
            }

            QJsonArray songArray = listVal.toArray();
            if (songArray.isEmpty()) {
                std::cerr << "[QQLyricFetcher] Song list is empty" << std::endl;
                reply->deleteLater();
                return;
            }

            QJsonObject firstSong = songArray[0].toObject();
            QString songmid = firstSong.value("songmid").toString();
            QString songid = firstSong.value("songid").toString();

            if (songmid.isEmpty()) {
                std::cerr << "[QQLyricFetcher] songmid is empty" << std::endl;
                reply->deleteLater();
                return;
            }

            std::cout << "songmid: " << songmid.toStdString() << std::endl;

            // 请求歌词
            QUrl url(lyricUrl.arg(songmid, songid));
            QNetworkRequest request(url);
            request.setRawHeader("Referer", "https://y.qq.com/portal/player.html");
            this->naManager->get(request);
        }
    }
    // 歌词 API 分支
    else {
        int code = jsonResponse["code"].toInt();
        std::cout << "code: " << code << std::endl;

        if (code == 0) {
            // 检查 lyric 字段是否存在且为字符串
            QJsonValue lyricVal = jsonResponse.value("lyric");
            if (lyricVal.isString()) {
                QString lyric = lyricVal.toString();
                if (!lyric.isEmpty()) {
                    MLyric formattedLyric(lyric);
                    Q_EMIT lyricFetched(formattedLyric);
                }
            } else {
                std::cerr << "[QQLyricFetcher] 'lyric' field is not a string" << std::endl;
            }
        }
    }
    
    reply->deleteLater(); // 统一释放资源
}
