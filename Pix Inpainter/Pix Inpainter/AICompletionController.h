#pragma once

#include "AICompletionWidget.h"
#include "AICompletionModel.h"

#include <QByteArray>
#include <QObject>
#include <QPixmap>

namespace paint
{
    class AICompletionController : public QObject
    {
        Q_OBJECT

    public:
        explicit AICompletionController(QObject* parent = nullptr, AICompletionModel* model = nullptr);
        ~AICompletionController();

        AICompletionController(const AICompletionController&) = delete;
        AICompletionController& operator=(const AICompletionController&) = delete;

        AICompletionController(AICompletionController&&) = delete;
        AICompletionController& operator=(AICompletionController&&) = delete;

        void setModel(AICompletionModel* model);
        void setView(AICompletionWidget* view);
        void setImageData(const QByteArray& pngData);
        AICompletionWidget* view() const;

    private slots:
        void onProcessImageRequested();
        void onCompareModelsRequested();
        void onModelSelectionChanged();

        void onImageDataChanged(const QPixmap& pixmap);
        void onProcessingStarted();
        void onProcessingFinished(bool success, const QPixmap& resultPixmap);
        void onComparisonStarted(const QStringList& modelKeys);
        void onComparisonFinished(const QString& modelKey, bool success, const QPixmap& resultPixmap);
        void onAllComparisonsFinished();
        void onStatusChanged(const QString& message);
        void onPreviewImageUpdated(const QPixmap& newPreviewImage);
        void onModelsInitialized(const QStringList& modelNames, const QStringList& modelKeys);

    private:
        void connectSignals();
        void disconnectSignals();

        AICompletionModel* m_model;
        AICompletionWidget* m_view;
    };

}

