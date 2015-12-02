
/****************************************************************************
  
 Copyright (c) 2015, Hans Robeers
 All rights reserved.
 
 BSD 2-Clause License
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
   * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   
   * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
   
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
****************************************************************************/

#include "stlexport.hpp"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>

#include "jenson.h"
#include "foil.h"
#include "string/json_utils.hpp"
#include "version.h"

#include <future>

using namespace web;
using namespace foillogic;
using namespace jenson;
using namespace hrlib;

StlExport::StlExport(const Version *version, QObject *parent) :
  QObject(parent),
  _manager(new QNetworkAccessManager()),
  _fileName("finfoil"), // TODO use real fileName if possible
  _messageName("finfoil")
{
    QString versionSuffix = "_v";
    versionSuffix.append(QString::number(version->Major()));
    versionSuffix.append(".");
    versionSuffix.append(QString::number(version->Minor()));

    _fileName.append(versionSuffix);
    _fileName.append(".foil");

    _messageName.append(versionSuffix);
    _messageName.append(".html");
}

QNetworkReply* StlExport::getMessage()
{
    QUrl url("http://127.0.0.1:4000/messages/" + _messageName);
    QNetworkRequest request(url);

    connect(_manager.get(), &QNetworkAccessManager::finished, this, &StlExport::finished);

    return _manager->get(request);
}

QNetworkReply* StlExport::generateSTL(const Foil *foil)
{
    QUrl url("http://127.0.0.1:4000/stl/" + _fileName);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/foil");

    connect(_manager.get(), &QNetworkAccessManager::finished, this, &StlExport::finished);

    // Serialize foil object
    QJsonDocument json(JenSON::serialize(foil));
    // Trim json floats
    std::string long_utf8 = json.toJson(QJsonDocument::Compact).toStdString();
    std::string short_utf8 = trim_json_floats(long_utf8);

    return _manager->post(request, QByteArray(short_utf8.c_str()));
}

QNetworkReply *StlExport::getSTL(const QByteArray &stlReply)
{
    QUrl url("http://127.0.0.1:4000/" + QString::fromUtf8(stlReply));
    QNetworkRequest request(url);

    connect(_manager.get(), &QNetworkAccessManager::finished, this, &StlExport::finished);

    return _manager->get(request);
}

StlExport::~StlExport() { }