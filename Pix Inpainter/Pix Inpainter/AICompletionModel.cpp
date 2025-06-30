#include "AICompletionModel.h"
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QBuffer>

const QString SERVER_BASE_URL = "http://localhost:5000/";

namespace paint
{
    AICompletionModel::AICompletionModel(QObject* parent)
        : QObject(parent)
        , m_networkManager(new QNetworkAccessManager(this))
        , m_currentReply(nullptr)
        , m_currentProcessingModelKey("")
        , m_currentPostprocessValue(0)
        , m_totalComparisonRequests(0)
    {
        initializeModels();
    }

    AICompletionModel::~AICompletionModel()
    {
        if (m_currentReply)
        {
            m_currentReply->abort();
            m_currentReply->deleteLater();
        }
        for (const auto& req : std::as_const(m_activeComparisonRequests))
        {
            if (req.reply) {
                req.reply->abort();
                req.reply->deleteLater();
            }
        }
    }

    void AICompletionModel::initializeModels()
    {
        QNetworkRequest request(QUrl(SERVER_BASE_URL + "models"));
        QNetworkReply* reply = m_networkManager->get(request);

        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            if (reply->error() != QNetworkReply::NoError) {
                emit statusChanged("Failed to fetch models: " + reply->errorString());
                reply->deleteLater();
                return;
            }

            const QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            if (!doc.isObject()) {
                emit statusChanged("Invalid response from model server.");
                reply->deleteLater();
                return;
            }

            QJsonObject obj = doc.object();
            QJsonArray keysArray = obj.value("available_models").toArray();

            m_modelKeys.clear();
            m_modelNames.clear();

            for (const auto& val : keysArray) {
                QString key = val.toString();
                m_modelKeys.append(key);
                m_modelNames.append(key);
            }

            emit statusChanged("Models initialized.");
            reply->deleteLater();

            emit modelsInitialized(m_modelNames, m_modelKeys);
        });
    }

    void AICompletionModel::reinitializeModels()
    {
        m_comparisonPixmaps.clear();
        
        initializeModels();
    }

    void AICompletionModel::setImageData(const QByteArray& pngData)
    {
        m_imageData = pngData;

        if (!m_imageData.isEmpty())
        {
            m_previewPixmap.loadFromData(m_imageData);
        }
        else
        {
            m_previewPixmap = QPixmap();
        }

        emit imageDataChanged(m_previewPixmap);

        m_resultPixmap = QPixmap();
        m_comparisonPixmaps.clear();

        emit statusChanged("Ready to process");
    }

    QByteArray AICompletionModel::imageData() const
    {
        return m_imageData;
    }

    QPixmap AICompletionModel::previewPixmap() const
    {
        return m_previewPixmap;
    }

    QPixmap AICompletionModel::resultPixmap() const
    {
        return m_resultPixmap;
    }

    QPixmap AICompletionModel::comparisonPixmap(const QString& modelKey) const
    {
        return m_comparisonPixmaps.value(modelKey, QPixmap());
    }

    void AICompletionModel::processImage(const QString& modelKey, int postprocessValue)
    {
        if (m_imageData.isEmpty())
        {
            emit statusChanged("No image data to process.");
            emit processingFinished(false, QPixmap());
            return;
        }

        if (m_currentReply && m_currentReply->isRunning())
        {
            emit statusChanged("Another process is already running.");
            return;
        }

        emit processingStarted();
        emit statusChanged("Processing with model: " + modelKey + ", Postprocess: " + QString::number(postprocessValue));

        m_currentProcessingModelKey = modelKey;
        m_currentPostprocessValue = postprocessValue;

        QNetworkRequest request(buildRequestUrl("process", modelKey, postprocessValue));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");

        if (m_currentReply)
        {
            m_currentReply->deleteLater();
        }
        m_currentReply = m_networkManager->post(request, m_imageData);

        connect(m_currentReply, &QNetworkReply::finished, this, &AICompletionModel::handleNetworkReply);
        connect(m_currentReply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError code) {
            if (m_currentReply && m_currentReply->error() != QNetworkReply::NoError && m_currentReply->error() != QNetworkReply::OperationCanceledError)
            {
                emit statusChanged("Network error: " + m_currentReply->errorString());
                emit processingFinished(false, QPixmap());
                m_currentReply->deleteLater();
                m_currentReply = nullptr;
            }
        });
    }

    void AICompletionModel::handleNetworkReply()
    {
        if (!m_currentReply) return;

        bool success = false;
        QPixmap receivedPixmap;

        if (m_currentReply->error() == QNetworkReply::NoError)
        {
            QByteArray responseData = m_currentReply->readAll();
            if (receivedPixmap.loadFromData(responseData, "PNG"))
            {
                m_resultPixmap = receivedPixmap;
                success = true;
                emit statusChanged("Image processed successfully with " + m_currentProcessingModelKey + ".");
            }
            else
            {
                m_resultPixmap = QPixmap();
                emit statusChanged("Failed to parse processed image from server response.");
            }
        }
        else
        {
            m_resultPixmap = QPixmap();
            emit statusChanged("Processing error: " + m_currentReply->errorString());
        }

        emit processingFinished(success, m_resultPixmap);
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
        m_currentProcessingModelKey = "";
        m_currentPostprocessValue = 0;
    }

    void AICompletionModel::compareModels(const QList<QPair<QString, int>>& modelsToCompare)
    {
        if (m_imageData.isEmpty())
        {
            emit statusChanged("No image data for comparison.");
            emit allComparisonsFinished();
            return;
        }

        for (const auto& req : std::as_const(m_activeComparisonRequests))
        {
            if (req.reply && req.reply->isRunning())
            {
                req.reply->abort();
            }
            if (req.reply) req.reply->deleteLater();
        }
        m_activeComparisonRequests.clear();
        m_comparisonPixmaps.clear();

        if (modelsToCompare.isEmpty())
        {
            emit statusChanged("No models selected for comparison.");
            emit allComparisonsFinished();
            return;
        }

        QStringList modelKeysOnly;
        for (const auto& pair : modelsToCompare) modelKeysOnly << pair.first;
        emit comparisonStarted(modelKeysOnly);

        m_totalComparisonRequests = modelsToCompare.size();

        for (const auto& modelPair : modelsToCompare)
        {
            const QString& modelKey = modelPair.first;
            int postprocessValue = modelPair.second;

            emit statusChanged("Starting comparison for model: " + modelKey + ", Postprocess: " + QString::number(postprocessValue));

            QNetworkRequest request(buildRequestUrl("compare", modelKey, postprocessValue));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");

            QNetworkReply* reply = m_networkManager->post(request, m_imageData);

            m_activeComparisonRequests.append({ modelKey, postprocessValue, reply });

            connect(reply, &QNetworkReply::finished, this, &AICompletionModel::handleComparisonReply);
            connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError code) {
                for (int i = 0; i < m_activeComparisonRequests.size(); ++i) {
                    if (m_activeComparisonRequests[i].reply == reply)
                    {
                        if (reply->error() != QNetworkReply::NoError && reply->error() != QNetworkReply::OperationCanceledError)
                        {
                            emit statusChanged("Network error for " + m_activeComparisonRequests[i].modelKey + ": " + reply->errorString());
                            emit comparisonFinished(m_activeComparisonRequests[i].modelKey, false, QPixmap());
                            m_activeComparisonRequests.removeAt(i);
                        }
                        break;
                    }
                }
                reply->deleteLater();
                if (m_activeComparisonRequests.isEmpty() && m_totalComparisonRequests > 0)
                {
                    m_totalComparisonRequests = 0;
                    emit allComparisonsFinished();
                }
            });
        }
    }

    void AICompletionModel::handleComparisonReply()
    {
        QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
        if (!reply) return;

        QString modelKeyForResult;
        bool foundRequest = false;

        for (int i = 0; i < m_activeComparisonRequests.size(); ++i)
        {
            if (m_activeComparisonRequests[i].reply == reply)
            {
                modelKeyForResult = m_activeComparisonRequests[i].modelKey;
                m_activeComparisonRequests.removeAt(i);
                foundRequest = true;
                break;
            }
        }

        if (!foundRequest)
        {
            reply->deleteLater();
            return;
        }

        bool success = false;
        QPixmap receivedPixmap;

        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray responseData = reply->readAll();
            if (receivedPixmap.loadFromData(responseData, "PNG"))
            {
                m_comparisonPixmaps[modelKeyForResult] = receivedPixmap;
                success = true;
                emit statusChanged("Comparison for " + modelKeyForResult + " successful.");
            }
            else
            {
                emit statusChanged("Failed to parse comparison image for " + modelKeyForResult + ".");
            }
        }
        else
        {
            emit statusChanged("Comparison error for " + modelKeyForResult + ": " + reply->errorString());
        }

        emit comparisonFinished(modelKeyForResult, success, receivedPixmap);
        reply->deleteLater();

        if (m_activeComparisonRequests.isEmpty() && m_totalComparisonRequests > 0)
        {
            m_totalComparisonRequests = 0;
            emit allComparisonsFinished();
        }
    }

    QStringList AICompletionModel::modelNames() const
    {
        return m_modelNames;
    }

    QStringList AICompletionModel::modelKeys() const
    {
        return m_modelKeys;
    }

    QUrl AICompletionModel::buildRequestUrl(const QString& endpoint, const QString& modelKey, int postprocessValue) const
    {
        QUrl url(SERVER_BASE_URL + endpoint);
        QUrlQuery query;
        if (!modelKey.isEmpty()) {
            query.addQueryItem("model_id", modelKey);
        }
        if (postprocessValue > 0)
        {
            query.addQueryItem("postprocess_value", QString::number(postprocessValue));
        }
        url.setQuery(query);
        return url;
    }
}