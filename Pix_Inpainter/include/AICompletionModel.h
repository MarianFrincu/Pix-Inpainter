#pragma once
#include <QObject>
#include <QPixmap>
#include <QByteArray>
#include <QStringList>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPair>

namespace paint
{
    class AICompletionModel : public QObject
    {
        Q_OBJECT

    public:
        explicit AICompletionModel(QObject* parent = nullptr);
        ~AICompletionModel();

        AICompletionModel(const AICompletionModel&) = delete;
        AICompletionModel& operator=(const AICompletionModel&) = delete;

        AICompletionModel(AICompletionModel&&) = delete;
        AICompletionModel& operator=(AICompletionModel&&) = delete;

        void setImageData(const QByteArray& pngData);
        QByteArray imageData() const;
        QPixmap previewPixmap() const;
        QPixmap resultPixmap() const;
        QPixmap comparisonPixmap(const QString& modelKey) const;

        void processImage(const QString& modelKey, int postprocessValue);
        void compareModels(const QList<QPair<QString, int>>& modelsToCompare);
        void reinitializeModels();

        QStringList modelNames() const;
        QStringList modelKeys() const;

    signals:
        void imageDataChanged(const QPixmap& pixmap);
        void processingStarted();
        void processingFinished(bool success, const QPixmap& resultPixmap);
        void comparisonStarted(const QStringList& modelKeys);
        void comparisonFinished(const QString& modelKey, bool success, const QPixmap& resultPixmap);
        void allComparisonsFinished();
        void statusChanged(const QString& message);
        void modelsInitialized(const QStringList& modelNames, const QStringList& modelKeys);

    private slots:
        void handleNetworkReply();
        void handleComparisonReply();

    private:
        void initializeModels();
        QUrl buildRequestUrl(const QString& endpoint, const QString& modelKey, int postprocessValue) const;

        QByteArray m_imageData;
        QPixmap m_previewPixmap;
        QPixmap m_resultPixmap;
        QNetworkAccessManager* m_networkManager;

        QNetworkReply* m_currentReply;
        QString m_currentProcessingModelKey;
        int m_currentPostprocessValue;

        struct ComparisonRequest {
            QString modelKey;
            int postprocessValue;
            QNetworkReply* reply;
        };
        QList<ComparisonRequest> m_activeComparisonRequests;
        int m_totalComparisonRequests;

        QStringList m_modelNames;
        QStringList m_modelKeys;

        QMap<QString, QPixmap> m_comparisonPixmaps;
    };
}
