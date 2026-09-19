#pragma once

#include "ZoomableImageWidget.h"

#include <QCloseEvent>
#include <QPushButton>
#include <QGridLayout>
#include <QStringList>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QWidget>
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <QMap>

namespace paint
{
    class AICompletionWidget : public QWidget
    {
        Q_OBJECT

    public:
        AICompletionWidget(QWidget* parent = nullptr);
        ~AICompletionWidget();

        AICompletionWidget(const AICompletionWidget&) = delete;
        AICompletionWidget& operator=(const AICompletionWidget&) = delete;

        AICompletionWidget(AICompletionWidget&&) = delete;
        AICompletionWidget& operator=(AICompletionWidget&&) = delete;


        void displayPreviewImage(const QPixmap& pixmap);
        void displayResultImage(const QPixmap& pixmap);
        void displayComparisonImage(const QString& modelKey, const QPixmap& pixmap);
        void setStatusText(const QString& text);
        void setProcessButtonEnabled(bool enabled);
        void setCompareButtonEnabled(bool enabled);
        void resetResults();
        void populateModels(const QStringList& modelNames, const QStringList& modelKeys);

        QString getSelectedModelKey() const;
        QStringList getSelectedComparisonModelKeys() const;

        int getProcessTabPostprocessValue() const;
        QList<QPair<QString, int>> getCompareTabSelectedModelsPostprocessValues() const;

    signals:
        void processImageRequested();
        void compareModelsRequested();
        void modelSelectionChanged();
        void applyResultToCanvasRequested(const QPixmap& resultImage);
        void previewImageChanged(const QPixmap& newPreviewImage);

    protected:
        void closeEvent(QCloseEvent* event) override;

    private slots:
        void onProcessButtonClicked();
        void onCompareButtonClicked();
        void onModelCheckboxToggled();
        void onApplyResultButtonClicked();
        void onCopyResultToPreviewButtonClicked();

    private:
        void setupUI();
        void setupProcessingTab();
        void setupComparisonTab();

        QTabWidget* m_tabWidget;
        ZoomableImageWidget* m_previewWidget;
        ZoomableImageWidget* m_resultWidget;

        QPushButton* m_processButton;
        QPushButton* m_compareButton;
        QPushButton* m_applyResultButton;
        QPushButton* m_copyResultToPreviewButton;

        QComboBox* m_modelSelector;
        QLabel* m_statusLabel;

        QCheckBox* m_processPostprocessCheckbox;
        QSpinBox* m_processPostprocessSpinBox;

        QMap<QString, QCheckBox*> m_modelCheckboxes;
        QMap<QString, QCheckBox*> m_comparePostprocessCheckboxes;
        QMap<QString, QSpinBox*>  m_comparePostprocessSpinboxes;

        ZoomableImageWidget* m_comparisonPreviewWidget;
        QMap<QString, ZoomableImageWidget*> m_comparisonWidgets;

        QVBoxLayout* m_modelsListLayout;
        QVBoxLayout* m_compResultsLayout;

        QStringList m_modelNames;
        QStringList m_modelKeys;
    };
}