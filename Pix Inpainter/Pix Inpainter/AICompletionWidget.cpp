#include "AICompletionWidget.h"
#include "ZoomableImageWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QSplitter>

namespace paint
{
    AICompletionWidget::AICompletionWidget(QWidget* parent)
        : QWidget(parent, Qt::Window)
        , m_tabWidget(new QTabWidget)
        , m_previewWidget(new ZoomableImageWidget)
        , m_resultWidget(new ZoomableImageWidget)
        , m_comparisonPreviewWidget(new ZoomableImageWidget)
        , m_processButton(new QPushButton("Generate"))
        , m_compareButton(new QPushButton("Compare Models"))
        , m_applyResultButton(new QPushButton("Apply to Canvas"))
        , m_copyResultToPreviewButton(new QPushButton("Use as Preview"))
        , m_modelSelector(new QComboBox)
        , m_statusLabel(new QLabel)
        , m_processPostprocessCheckbox(new QCheckBox("Use Postprocess"))
        , m_processPostprocessSpinBox(new QSpinBox())
        , m_modelsListLayout(new QVBoxLayout)
        , m_compResultsLayout(new QVBoxLayout)
    {
        setupUI();
        m_applyResultButton->setEnabled(false);
        m_copyResultToPreviewButton->setEnabled(false);

        m_processPostprocessSpinBox->setRange(1, 10);
        m_processPostprocessSpinBox->setValue(1);
        m_processPostprocessSpinBox->setEnabled(m_processPostprocessCheckbox->isChecked());
        connect(m_processPostprocessCheckbox, &QCheckBox::toggled, m_processPostprocessSpinBox, &QSpinBox::setEnabled);
    }

    AICompletionWidget::~AICompletionWidget() = default;

    void AICompletionWidget::setupUI()
    {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        setupProcessingTab();
        setupComparisonTab();

        mainLayout->addWidget(m_tabWidget);
        mainLayout->addWidget(m_statusLabel);

        setLayout(mainLayout);

        m_statusLabel->setText("Ready");
        setMinimumSize(1000, 700);
        setWindowTitle("Ai Completion Tool");
    }

    void AICompletionWidget::setupProcessingTab()
    {
        QWidget* processingTab = new QWidget;
        QVBoxLayout* tabLayout = new QVBoxLayout(processingTab);

        QHBoxLayout* imageDisplayLayout = new QHBoxLayout();

        QVBoxLayout* previewVLayout = new QVBoxLayout();
        previewVLayout->addWidget(new QLabel("Preview:"));
        previewVLayout->addWidget(m_previewWidget);
        imageDisplayLayout->addLayout(previewVLayout);

        QVBoxLayout* resultVLayout = new QVBoxLayout();
        resultVLayout->addWidget(new QLabel("Result:"));
        resultVLayout->addWidget(m_resultWidget);
        imageDisplayLayout->addLayout(resultVLayout);

        tabLayout->addLayout(imageDisplayLayout);

        QGroupBox* processingControlsGroup = new QGroupBox("Controls");
        QVBoxLayout* groupLayout = new QVBoxLayout(processingControlsGroup);

        QHBoxLayout* modelAndProcessLayout = new QHBoxLayout();
        modelAndProcessLayout->addWidget(new QLabel("Model:"));
        modelAndProcessLayout->addWidget(m_modelSelector);
        modelAndProcessLayout->addWidget(m_processButton);
        modelAndProcessLayout->addStretch();
        groupLayout->addLayout(modelAndProcessLayout);

        QHBoxLayout* postprocessLayout = new QHBoxLayout();
        postprocessLayout->addWidget(m_processPostprocessCheckbox);
        postprocessLayout->addWidget(m_processPostprocessSpinBox);
        postprocessLayout->addStretch();
        groupLayout->addLayout(postprocessLayout);

        QHBoxLayout* actionButtonsLayout = new QHBoxLayout();
        actionButtonsLayout->addWidget(m_copyResultToPreviewButton);
        actionButtonsLayout->addWidget(m_applyResultButton);
        actionButtonsLayout->addStretch();
        groupLayout->addLayout(actionButtonsLayout);

        tabLayout->addWidget(processingControlsGroup);

        m_tabWidget->addTab(processingTab, "Ai Completion");

        connect(m_processButton, &QPushButton::clicked, this, &AICompletionWidget::onProcessButtonClicked);
        connect(m_copyResultToPreviewButton, &QPushButton::clicked, this, &AICompletionWidget::onCopyResultToPreviewButtonClicked);
        connect(m_applyResultButton, &QPushButton::clicked, this, &AICompletionWidget::onApplyResultButtonClicked);
        connect(m_modelSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AICompletionWidget::modelSelectionChanged);
    }

    void AICompletionWidget::setupComparisonTab()
    {
        QWidget* comparisonTab = new QWidget;
        QVBoxLayout* mainTabLayout = new QVBoxLayout(comparisonTab);

        QSplitter* splitter = new QSplitter(Qt::Horizontal);

        QGroupBox* modelSelectionGroup = new QGroupBox("Select Models & Postprocess Options");
        QVBoxLayout* selectionGroupLayout = new QVBoxLayout(modelSelectionGroup);

        QGroupBox* previewGroup = new QGroupBox("Preview");
        QVBoxLayout* previewLayout = new QVBoxLayout(previewGroup);
        previewLayout->addWidget(m_comparisonPreviewWidget);
        m_comparisonPreviewWidget->setMinimumHeight(150);
        selectionGroupLayout->addWidget(previewGroup);

        QScrollArea* modelScrollArea = new QScrollArea();
        modelScrollArea->setWidgetResizable(true);
        modelScrollArea->setFrameShape(QFrame::StyledPanel);

        QWidget* modelScrollWidget = new QWidget();
        modelScrollWidget->setLayout(m_modelsListLayout);
        m_modelsListLayout->setAlignment(Qt::AlignTop);
        modelScrollArea->setWidget(modelScrollWidget);

        selectionGroupLayout->addWidget(modelScrollArea);
        splitter->addWidget(modelSelectionGroup);

        QGroupBox* resultsGroup = new QGroupBox("Comparison Results");
        QVBoxLayout* resultsGroupLayout = new QVBoxLayout(resultsGroup);

        QScrollArea* resultsScrollArea = new QScrollArea();
        resultsScrollArea->setWidgetResizable(true);
        resultsScrollArea->setFrameShape(QFrame::StyledPanel);

        QWidget* resultsScrollWidget = new QWidget();
        resultsScrollWidget->setLayout(m_compResultsLayout);
        m_compResultsLayout->setAlignment(Qt::AlignTop);
        resultsScrollArea->setWidget(resultsScrollWidget);

        resultsGroupLayout->addWidget(resultsScrollArea);
        splitter->addWidget(resultsGroup);

        QList<int> sizes;
        sizes << 300 << 500;
        splitter->setSizes(sizes);

        mainTabLayout->addWidget(splitter);

        QHBoxLayout* compareButtonLayout = new QHBoxLayout();
        compareButtonLayout->addStretch();
        compareButtonLayout->addWidget(m_compareButton);
        compareButtonLayout->addStretch();
        mainTabLayout->addLayout(compareButtonLayout);

        m_tabWidget->addTab(comparisonTab, "Compare");

        connect(m_compareButton, &QPushButton::clicked, this, &AICompletionWidget::onCompareButtonClicked);
    }

    void AICompletionWidget::displayPreviewImage(const QPixmap& pixmap)
    {
        m_previewWidget->setPixmap(pixmap);
        m_comparisonPreviewWidget->setPixmap(pixmap);
    }

    void AICompletionWidget::displayResultImage(const QPixmap& pixmap)
    {
        m_resultWidget->setPixmap(pixmap);
        bool isValidPixmap = !pixmap.isNull();
        m_applyResultButton->setEnabled(isValidPixmap);
        m_copyResultToPreviewButton->setEnabled(isValidPixmap);
    }

    void AICompletionWidget::displayComparisonImage(const QString& modelKey, const QPixmap& pixmap)
    {
        if (m_comparisonWidgets.contains(modelKey))
        {
            m_comparisonWidgets[modelKey]->setPixmap(pixmap);
        }
    }

    void AICompletionWidget::setStatusText(const QString& text)
    {
        m_statusLabel->setText(text);
    }

    void AICompletionWidget::setProcessButtonEnabled(bool enabled)
    {
        m_processButton->setEnabled(enabled);
    }

    void AICompletionWidget::setCompareButtonEnabled(bool enabled)
    {
        m_compareButton->setEnabled(enabled);
    }

    void AICompletionWidget::resetResults()
    {
        m_resultWidget->setPixmap(QPixmap());
        m_applyResultButton->setEnabled(false);
        m_copyResultToPreviewButton->setEnabled(false);
        m_processPostprocessCheckbox->setChecked(false);

        for (ZoomableImageWidget* ziw : m_comparisonWidgets)
        {
            if (ziw) ziw->setPixmap(QPixmap());
        }
    }

    void AICompletionWidget::populateModels(const QStringList& modelNames, const QStringList& modelKeys)
    {
        m_modelNames = modelNames;
        m_modelKeys = modelKeys;

        m_modelSelector->clear();
        m_modelSelector->addItems(modelNames);
        
        m_modelSelector->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        m_modelSelector->updateGeometry();

        for (QCheckBox* checkbox : std::as_const(m_modelCheckboxes)) {
            if (checkbox) delete checkbox;
        }
        m_modelCheckboxes.clear();

        for (QCheckBox* checkbox : std::as_const(m_comparePostprocessCheckboxes)) {
            if (checkbox) delete checkbox;
        }
        m_comparePostprocessCheckboxes.clear();

        for (QSpinBox* spinbox : std::as_const(m_comparePostprocessSpinboxes)) {
            if (spinbox) delete spinbox;
        }
        m_comparePostprocessSpinboxes.clear();

        for (ZoomableImageWidget* widget : std::as_const(m_comparisonWidgets)) {
            if (widget) delete widget;
        }
        m_comparisonWidgets.clear();

        QLayoutItem* item;
        if (m_modelsListLayout) {
            while ((item = m_modelsListLayout->takeAt(0)) != nullptr) {
                if (item->widget()) delete item->widget();
                delete item;
            }
        }
        if (m_compResultsLayout) {
            while ((item = m_compResultsLayout->takeAt(0)) != nullptr) {
                if (item->widget()) delete item->widget();
                delete item;
            }
        }

        m_modelsListLayout->addSpacing(5);

        for (int i = 0; i < modelNames.size(); ++i)
        {
            const QString& modelKey = modelKeys[i];
            const QString& modelName = modelNames[i];

            QFrame* modelFrame = new QFrame();
            modelFrame->setFrameShape(QFrame::StyledPanel);
            modelFrame->setFrameShadow(QFrame::Raised);
            modelFrame->setStyleSheet("QFrame { background-color: rgba(240, 240, 240, 120); border-radius: 5px; margin: 2px; }");

            QVBoxLayout* frameLayout = new QVBoxLayout(modelFrame);
            frameLayout->setContentsMargins(8, 8, 8, 8);
            frameLayout->setSpacing(4);

            QCheckBox* mainCheckbox = new QCheckBox(modelName);
            QFont font = mainCheckbox->font();
            font.setBold(true);
            mainCheckbox->setFont(font);
            m_modelCheckboxes[modelKey] = mainCheckbox;
            frameLayout->addWidget(mainCheckbox);

            QWidget* postprocessContainer = new QWidget();
            QHBoxLayout* postprocessLayout = new QHBoxLayout(postprocessContainer);
            postprocessLayout->setContentsMargins(20, 0, 0, 0);
            postprocessLayout->setSpacing(8);

            QCheckBox* postprocessCb = new QCheckBox("Postprocess");
            m_comparePostprocessCheckboxes[modelKey] = postprocessCb;
            postprocessLayout->addWidget(postprocessCb);

            QSpinBox* postprocessSb = new QSpinBox();
            postprocessSb->setRange(1, 10);
            postprocessSb->setValue(1);
            postprocessSb->setEnabled(postprocessCb->isChecked());
            connect(postprocessCb, &QCheckBox::toggled, postprocessSb, &QSpinBox::setEnabled);
            m_comparePostprocessSpinboxes[modelKey] = postprocessSb;
            postprocessLayout->addWidget(postprocessSb);

            postprocessLayout->addStretch(1);
            frameLayout->addWidget(postprocessContainer);

            m_modelsListLayout->addWidget(modelFrame);

            m_modelsListLayout->addSpacing(4);

            connect(mainCheckbox, &QCheckBox::toggled, this, &AICompletionWidget::onModelCheckboxToggled);

            ZoomableImageWidget* ziw = new ZoomableImageWidget;
            m_comparisonWidgets[modelKey] = ziw;
        }

        m_modelsListLayout->addStretch();

        onModelCheckboxToggled();
    }

    QString AICompletionWidget::getSelectedModelKey() const
    {
        int index = m_modelSelector->currentIndex();
        return (index >= 0 && index < m_modelKeys.size()) ? m_modelKeys[index] : QString();
    }

    int AICompletionWidget::getProcessTabPostprocessValue() const
    {
        if (m_processPostprocessCheckbox && m_processPostprocessCheckbox->isChecked() && m_processPostprocessSpinBox) {
            return m_processPostprocessSpinBox->value();
        }
        return 0;
    }

    QList<QPair<QString, int>> AICompletionWidget::getCompareTabSelectedModelsPostprocessValues() const
    {
        QList<QPair<QString, int>> values;
        QStringList selectedKeys = getSelectedComparisonModelKeys();

        for (const QString& key : selectedKeys) {
            int postprocessValue = 0;
            if (m_comparePostprocessCheckboxes.contains(key) &&
                m_comparePostprocessCheckboxes.value(key)->isChecked() &&
                m_comparePostprocessSpinboxes.contains(key)) {
                postprocessValue = m_comparePostprocessSpinboxes.value(key)->value();
            }
            values.append(qMakePair(key, postprocessValue));
        }
        return values;
    }

    QStringList AICompletionWidget::getSelectedComparisonModelKeys() const
    {
        QStringList keys;
        for (const QString& key : std::as_const(m_modelKeys))
        {
            auto it = m_modelCheckboxes.find(key);
            if (it != m_modelCheckboxes.end() && it.value()->isChecked())
            {
                keys << key;
            }
        }
        return keys;
    }

    void AICompletionWidget::closeEvent(QCloseEvent* event)
    {
        event->accept();
    }

    void AICompletionWidget::onProcessButtonClicked()
    {
        emit processImageRequested();
    }

    void AICompletionWidget::onCompareButtonClicked()
    {
        emit compareModelsRequested();
    }

    void AICompletionWidget::onApplyResultButtonClicked()
    {
        if (m_resultWidget && !m_resultWidget->pixmap().isNull())
        {
            emit applyResultToCanvasRequested(m_resultWidget->pixmap());
        }
    }

    void AICompletionWidget::onCopyResultToPreviewButtonClicked()
    {
        if (m_resultWidget && !m_resultWidget->pixmap().isNull() && m_previewWidget)
        {
            QPixmap resultPixmap = m_resultWidget->pixmap();
            m_previewWidget->setPixmap(resultPixmap);
            emit previewImageChanged(resultPixmap);
        }
    }

    void AICompletionWidget::onModelCheckboxToggled()
    {
        emit modelSelectionChanged();

        QLayoutItem* item;
        while ((item = m_compResultsLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                QList<ZoomableImageWidget*> childZiws = item->widget()->findChildren<ZoomableImageWidget*>();
                for (ZoomableImageWidget* ziw : childZiws) {
                    ziw->setParent(nullptr);
                }
                delete item->widget();
            }
            delete item;
        }

        QWidget* gridContainer = new QWidget();
        QGridLayout* gridLayout = new QGridLayout(gridContainer);
        gridLayout->setSpacing(10);

        QList<QString> selectedKeys;
        for (const QString& key : std::as_const(m_modelKeys)) {
            auto checkboxIt = m_modelCheckboxes.find(key);
            if (checkboxIt != m_modelCheckboxes.end() && checkboxIt.value()->isChecked()) {
                selectedKeys.append(key);
            }
        }
        
        const int columns = 2;
        
        int row = 0;
        int col = 0;
        
        for (const QString& key : selectedKeys)
        {
            auto ziwIt = m_comparisonWidgets.find(key);
            if (ziwIt != m_comparisonWidgets.end()) {
                ZoomableImageWidget* currentZiw = ziwIt.value();

                QFrame* resultFrame = new QFrame();
                resultFrame->setFrameShape(QFrame::StyledPanel);

                QVBoxLayout* resultItemLayout = new QVBoxLayout(resultFrame);
                resultItemLayout->setContentsMargins(5, 5, 5, 5);

                int modelIndex = m_modelKeys.indexOf(key);
                QString name = (modelIndex != -1 && modelIndex < m_modelNames.size()) ? m_modelNames[modelIndex] : key;

                QLabel* nameLabel = new QLabel(name);
                nameLabel->setAlignment(Qt::AlignCenter);
                QFont font = nameLabel->font();
                font.setBold(true);
                nameLabel->setFont(font);

                resultItemLayout->addWidget(nameLabel);
                resultItemLayout->addWidget(currentZiw);

                currentZiw->setMinimumSize(200, 200);

                gridLayout->addWidget(resultFrame, row, col);
                
                col++;
                if (col >= columns) {
                    col = 0;
                    row++;
                }
            }
        }

        m_compResultsLayout->addWidget(gridContainer);
        m_compResultsLayout->addStretch();
    }
}