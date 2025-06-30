#include "AICompletionController.h"
#include "AICompletionModel.h"
#include "AICompletionWidget.h"
#include <QBuffer>
#include <QPair>

namespace paint
{

    AICompletionController::AICompletionController(QObject* parent, AICompletionModel* model)
        : QObject(parent)
        , m_model(nullptr)
        , m_view(nullptr)
    {
        setModel(model);
    }

    AICompletionController::~AICompletionController()
    {
        disconnectSignals();
    }

    void AICompletionController::setModel(AICompletionModel* model)
    {
        if (m_model)
        {
            disconnect(m_model, &AICompletionModel::imageDataChanged, this, &AICompletionController::onImageDataChanged);
            disconnect(m_model, &AICompletionModel::processingStarted, this, &AICompletionController::onProcessingStarted);
            disconnect(m_model, &AICompletionModel::processingFinished, this, &AICompletionController::onProcessingFinished);
            disconnect(m_model, &AICompletionModel::comparisonStarted, this, &AICompletionController::onComparisonStarted);
            disconnect(m_model, &AICompletionModel::comparisonFinished, this, &AICompletionController::onComparisonFinished);
            disconnect(m_model, &AICompletionModel::allComparisonsFinished, this, &AICompletionController::onAllComparisonsFinished);
            disconnect(m_model, &AICompletionModel::statusChanged, this, &AICompletionController::onStatusChanged);
        }

        m_model = model;

        if (m_model)
        {
            connect(m_model, &AICompletionModel::imageDataChanged, this, &AICompletionController::onImageDataChanged);
            connect(m_model, &AICompletionModel::processingStarted, this, &AICompletionController::onProcessingStarted);
            connect(m_model, &AICompletionModel::processingFinished, this, &AICompletionController::onProcessingFinished);
            connect(m_model, &AICompletionModel::comparisonStarted, this, &AICompletionController::onComparisonStarted);
            connect(m_model, &AICompletionModel::comparisonFinished, this, &AICompletionController::onComparisonFinished);
            connect(m_model, &AICompletionModel::allComparisonsFinished, this, &AICompletionController::onAllComparisonsFinished);
            connect(m_model, &AICompletionModel::statusChanged, this, &AICompletionController::onStatusChanged);

            if (m_view && m_model)
            {
                m_view->populateModels(m_model->modelNames(), m_model->modelKeys());
                if (!m_model->previewPixmap().isNull())
                {
                    m_view->displayPreviewImage(m_model->previewPixmap());
                }
            }
        }
    }

    void AICompletionController::setView(AICompletionWidget* view)
    {
        disconnectSignals();
        m_view = view;
        connectSignals();

        if (m_view && m_model)
        {
            m_view->populateModels(m_model->modelNames(), m_model->modelKeys());
            if (!m_model->previewPixmap().isNull())
            {
                m_view->displayPreviewImage(m_model->previewPixmap());
            }
            if (!m_model->resultPixmap().isNull())
            {
                m_view->displayResultImage(m_model->resultPixmap());
            }
        }
    }

    void AICompletionController::setImageData(const QByteArray& pngData)
    {
        if (m_model)
        {
            m_model->setImageData(pngData);
        }
    }

    AICompletionWidget* AICompletionController::view() const
    {
        return m_view;
    }

    void AICompletionController::connectSignals()
    {
        if (m_view)
        {
            connect(m_view, &AICompletionWidget::processImageRequested, this, &AICompletionController::onProcessImageRequested);
            connect(m_view, &AICompletionWidget::compareModelsRequested, this, &AICompletionController::onCompareModelsRequested);
            connect(m_view, &AICompletionWidget::modelSelectionChanged, this, &AICompletionController::onModelSelectionChanged);
            connect(m_view, &AICompletionWidget::previewImageChanged, this, &AICompletionController::onPreviewImageUpdated);
        }
        
        if (m_model)
        {
            connect(m_model, &AICompletionModel::modelsInitialized, this, &AICompletionController::onModelsInitialized);
        }
    }

    void AICompletionController::disconnectSignals()
    {
        if (m_view)
        {
            disconnect(m_view, &AICompletionWidget::processImageRequested, this, &AICompletionController::onProcessImageRequested);
            disconnect(m_view, &AICompletionWidget::compareModelsRequested, this, &AICompletionController::onCompareModelsRequested);
            disconnect(m_view, &AICompletionWidget::modelSelectionChanged, this, &AICompletionController::onModelSelectionChanged);
            disconnect(m_view, &AICompletionWidget::previewImageChanged, this, &AICompletionController::onPreviewImageUpdated);
        }
    }

    void AICompletionController::onProcessImageRequested()
    {
        if (m_model && m_view)
        {
            QString modelKey = m_view->getSelectedModelKey();
            if (!modelKey.isEmpty())
            {
                int postprocessValue = m_view->getProcessTabPostprocessValue();
                m_model->processImage(modelKey, postprocessValue);
            }
            else
            {
                m_view->setStatusText("No model selected for processing.");
            }
        }
    }

    void AICompletionController::onCompareModelsRequested()
    {
        if (m_model && m_view)
        {
            QList<QPair<QString, int>> modelsToCompare = m_view->getCompareTabSelectedModelsPostprocessValues();

            if (!modelsToCompare.isEmpty())
            {
                m_model->compareModels(modelsToCompare);
            }
            else
            {
                m_view->setStatusText("No models selected for comparison.");
            }
        }
    }

    void AICompletionController::onModelSelectionChanged()
    {
    }

    void AICompletionController::onImageDataChanged(const QPixmap& pixmap)
    {
        if (m_view)
        {
            m_view->displayPreviewImage(pixmap);
            m_view->resetResults();
        }
    }

    void AICompletionController::onProcessingStarted()
    {
        if (m_view)
        {
            m_view->setStatusText("Processing image...");
            m_view->setProcessButtonEnabled(false);
            m_view->setCompareButtonEnabled(false);
        }
    }

    void AICompletionController::onProcessingFinished(bool success, const QPixmap& resultPixmap)
    {
        if (m_view)
        {
            if (success)
            {
                m_view->displayResultImage(resultPixmap);
                m_view->setStatusText("Processing finished successfully.");
            }
            else
            {
                m_view->displayResultImage(QPixmap());
                m_view->setStatusText("Processing failed.");
            }
            m_view->setProcessButtonEnabled(true);
            m_view->setCompareButtonEnabled(true);
        }
    }

    void AICompletionController::onComparisonStarted(const QStringList& modelKeys)
    {
        if (m_view)
        {
            m_view->setStatusText("Starting comparison for models: " + modelKeys.join(", "));
            m_view->setProcessButtonEnabled(false);
            m_view->setCompareButtonEnabled(false);
            if (m_model)
            {
                for (const QString& key : m_model->modelKeys())
                {
                    m_view->displayComparisonImage(key, QPixmap());
                }
            }
        }
    }

    void AICompletionController::onComparisonFinished(const QString& modelKey, bool success, const QPixmap& resultPixmap)
    {
        if (m_view)
        {
            if (success)
            {
                m_view->displayComparisonImage(modelKey, resultPixmap);
                m_view->setStatusText(QString("Comparison finished for model: %1.").arg(modelKey));
            }
            else
            {
                m_view->displayComparisonImage(modelKey, QPixmap());
                m_view->setStatusText(QString("Comparison failed for model: %1.").arg(modelKey));
            }
        }
    }

    void AICompletionController::onAllComparisonsFinished()
    {
        if (m_view)
        {
            m_view->setStatusText("All comparisons finished.");
            m_view->setProcessButtonEnabled(true);
            m_view->setCompareButtonEnabled(true);
        }
    }

    void AICompletionController::onStatusChanged(const QString& message)
    {
        if (m_view)
        {
            m_view->setStatusText(message);
        }
    }

    void AICompletionController::onPreviewImageUpdated(const QPixmap& newPreviewImage)
    {
        if (m_model && !newPreviewImage.isNull())
        {
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            newPreviewImage.save(&buffer, "PNG");
            buffer.close();

            m_model->setImageData(byteArray);
        }
    }

    void AICompletionController::onModelsInitialized(const QStringList& modelNames, const QStringList& modelKeys)
    {
        if (m_view) {
            m_view->populateModels(modelNames, modelKeys);
        }
    }
}