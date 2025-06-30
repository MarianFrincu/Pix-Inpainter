#include "PixInpainter.h"

#include <QColorDialog>
#include <QImageReader>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QStatusBar>
#include <QClipboard>
#include <QGroupBox>
#include <QMenuBar>
#include <QBuffer>
#include <QLabel>

PixInpainter::PixInpainter(QWidget *parent)
    : QMainWindow(parent)
{
    m_ui.setupUi(this);

    setupMainUI();
    setupToolbar();
    setupMenus();

    connect(m_paintWidget, &paint::PaintWidget::colorPicked,
        this, &PixInpainter::handleColorPicked);

    if (m_penAction)
    {
        m_penAction->trigger();
    }
}

PixInpainter::~PixInpainter()
{}

void PixInpainter::setupMainUI()
{
    m_canvasModel = paint::ICanvasModel::create(256, 256);

    m_paintWidget = new paint::PaintWidget(this, 256, 256);

    m_paintController = new paint::PaintController(this, m_canvasModel);

    m_paintWidget->setController(m_paintController);

    connect(m_paintWidget, &paint::PaintWidget::zoomChanged,
        this, [this](qreal zoomLevel) {
            statusBar()->showMessage(QString("Zoom level: %1%").arg(zoomLevel * 100, 0, 'f', 0), 2000);
        });

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(m_paintWidget);
    scrollArea->setWidgetResizable(false);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setBackgroundRole(QPalette::Dark);

    setCentralWidget(scrollArea);
    resize(1080, 720);
    setWindowTitle("Pix Inpainter");

    m_toolGroup = new QActionGroup(this);
    m_toolGroup->setExclusive(true);

    statusBar()->showMessage("Ready", 3000);
}

void PixInpainter::setupMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));

    QAction* openAction = fileMenu->addAction(tr("Open..."));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &PixInpainter::loadImageFromFile);

    QAction* saveAction = fileMenu->addAction(tr("Save..."));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &PixInpainter::saveImageToFile);

    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction(tr("Exit"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu* editMenu = menuBar()->addMenu(tr("Edit"));

    QAction* undoAction = editMenu->addAction(tr("Undo"));
    undoAction->setIcon(QIcon(":/icons/Undo.png"));
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &PixInpainter::onUndo);

    QAction* redoAction = editMenu->addAction(tr("Redo"));
    redoAction->setIcon(QIcon(":/icons/Redo.png"));
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &PixInpainter::onRedo);

    QAction* copyAction = editMenu->addAction(tr("Copy"));
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, this, &PixInpainter::copyCanvas);

    QAction* pasteAction = editMenu->addAction(tr("Paste"));
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, &QAction::triggered, this, &PixInpainter::pasteCanvas);

    editMenu->addSeparator();
    QAction* clearAction = editMenu->addAction(tr("Clear Canvas"));
    connect(clearAction, &QAction::triggered, this, &PixInpainter::clearCanvas);

    QMenu* viewMenu = menuBar()->addMenu(tr("View"));

    if (m_zoomInAction)
    {
        viewMenu->addAction(m_zoomInAction);
    }

    if (m_zoomOutAction)
    {
        viewMenu->addAction(m_zoomOutAction);
    }

    QAction* resetZoomAction = viewMenu->addAction(tr("Reset Zoom"));
    resetZoomAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    connect(resetZoomAction, &QAction::triggered, this, &PixInpainter::resetZoom);

    QAction* toggleGridAction = viewMenu->addAction(tr("Show Grid"));
    toggleGridAction->setCheckable(true);
    toggleGridAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    connect(toggleGridAction, &QAction::toggled, this, &PixInpainter::toggleGrid);

    QMenu* toolsMenu = menuBar()->addMenu(tr("Tools"));

    if (m_penAction)
    {
        m_penAction->setShortcut(QKeySequence(Qt::Key_P));
        toolsMenu->addAction(m_penAction);
    }

    if (m_eraserAction)
    {
        m_eraserAction->setShortcut(QKeySequence(Qt::Key_E));
        toolsMenu->addAction(m_eraserAction);
    }

    if (m_fillAction)
    {
        m_fillAction->setShortcut(QKeySequence(Qt::Key_F));
        toolsMenu->addAction(m_fillAction);
    }

    if (m_eyedropperAction)
    {
        m_eyedropperAction->setShortcut(QKeySequence(Qt::Key_I));
        toolsMenu->addAction(m_eyedropperAction);
    }

    if (m_lineAction)
    {
        m_lineAction->setShortcut(QKeySequence(Qt::Key_L));
        toolsMenu->addAction(m_lineAction);
    }

    if (m_rectangleAction)
    {
        m_rectangleAction->setShortcut(QKeySequence(Qt::Key_R));
        toolsMenu->addAction(m_rectangleAction);
    }

    if (m_ellipseAction)
    {
        m_ellipseAction->setShortcut(QKeySequence(Qt::Key_C));
        toolsMenu->addAction(m_ellipseAction);
    }

    if (m_triangleAction)
    {
        m_triangleAction->setShortcut(QKeySequence(Qt::Key_T));
        toolsMenu->addAction(m_triangleAction);
    }
}

void PixInpainter::setupToolbar()
{
    QToolBar* toolbar = addToolBar("Toolbar");
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(ICON_SIZE, ICON_SIZE));

    setupUtilityTools(toolbar);
    toolbar->addSeparator();

    setupDrawingTools(toolbar);
    toolbar->addSeparator();

    setupColorAndPenSizeControls(toolbar);
    toolbar->addSeparator();

    setupViewControls(toolbar);

    toolbar->addSeparator();

    setupAiGenControl(toolbar);

    connectToolActions();
}

void PixInpainter::setupDrawingTools(QToolBar* toolbar)
{
    QWidget* drawingToolsContainer = new QWidget(this);
    QGridLayout* gridLayout = new QGridLayout(drawingToolsContainer);
    gridLayout->setSpacing(GRID_LAYOUT_SPACING);
    gridLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN, LAYOUT_MARGIN);

    QToolButton* penButton = createToolButton(
        QIcon(":/icons/Pen.png"),
        "Pen"
    );
    m_penAction = penButton->defaultAction();
    m_penAction->setChecked(true);

    QToolButton* eraserButton = createToolButton(
        QIcon(":/icons/Eraser.png"),
        "Eraser"
    );
    m_eraserAction = eraserButton->defaultAction();

    QToolButton* fillButton = createToolButton(
        QIcon(":/icons/Fill.png"),
        "Fill"
    );
    m_fillAction = fillButton->defaultAction();

    QToolButton* eyedropperButton = createToolButton(
        QIcon(":/icons/Eyedropper.png"),
        "Eyedropper"
    );
    m_eyedropperAction = eyedropperButton->defaultAction();

    QToolButton* lineButton = createToolButton(
        QIcon(":/icons/Line.png"),
        "Line"
    );
    m_lineAction = lineButton->defaultAction();

    QToolButton* rectangleButton = createToolButton(
        QIcon(":/icons/Rectangle.png"),
        "Rectangle"
    );
    m_rectangleAction = rectangleButton->defaultAction();

    QToolButton* ellipseButton = createToolButton(
        QIcon(":/icons/Elipse.png"),
        "Ellipse"
    );
    m_ellipseAction = ellipseButton->defaultAction();

    QToolButton* triangleButton = createToolButton(
        QIcon(":/icons/Triangle.png"),
        "Triangle"
    );
    m_triangleAction = triangleButton->defaultAction();

    gridLayout->addWidget(penButton, 0, 0);
    gridLayout->addWidget(eraserButton, 1, 0);
    gridLayout->addWidget(fillButton, 0, 1);
    gridLayout->addWidget(eyedropperButton, 1, 1);
    gridLayout->addWidget(lineButton, 0, 2);
    gridLayout->addWidget(triangleButton, 1, 2);
    gridLayout->addWidget(rectangleButton, 0, 3);
    gridLayout->addWidget(ellipseButton, 1, 3);

    toolbar->addWidget(drawingToolsContainer);
}

QToolButton* PixInpainter::createToolButton(const QIcon& icon, const QString& text)
{
    QToolButton* button = new QToolButton(this);
    QAction* action = new QAction(icon, text, this);
    action->setCheckable(true);
    m_toolGroup->addAction(action);

    button->setDefaultAction(action);
    button->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    button->setFixedSize(TOOL_BUTTON_SIZE, TOOL_BUTTON_SIZE);
    button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    return button;
}

void PixInpainter::setupUtilityTools(QToolBar* toolbar)
{
    QWidget* utilityContainer = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(utilityContainer);
    mainLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN / 2,
        LAYOUT_MARGIN, LAYOUT_MARGIN / 2);
    mainLayout->setSpacing(HBOX_SPACING);

    QGroupBox* canvasGroup = new QGroupBox("Canvas", utilityContainer);
    canvasGroup->setStyleSheet("QGroupBox { font-weight: bold; }");
    QHBoxLayout* historyLayout = new QHBoxLayout(canvasGroup);
    historyLayout->setContentsMargins(8, 12, 8, 8);
    historyLayout->setSpacing(4);

    QToolButton* undoButton = new QToolButton(canvasGroup);
    undoButton->setIcon(QIcon(":/icons/Undo.png"));
    undoButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    undoButton->setToolTip("Undo");
    undoButton->setFixedSize(TOOL_BUTTON_SIZE, TOOL_BUTTON_SIZE);
    connect(undoButton, &QToolButton::clicked, this, &PixInpainter::onUndo);

    QToolButton* redoButton = new QToolButton(canvasGroup);
    redoButton->setIcon(QIcon(":/icons/Redo.png"));
    redoButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    redoButton->setToolTip("Redo");
    redoButton->setFixedSize(TOOL_BUTTON_SIZE, TOOL_BUTTON_SIZE);
    connect(redoButton, &QToolButton::clicked, this, &PixInpainter::onRedo);

    QPushButton* clearCanvasButton = new QPushButton("Clear", canvasGroup);
    QFont font;
    font.setPointSize(font.pointSize() + FONT_SIZE_INCREASE - 1);
    clearCanvasButton->setFont(font);
    clearCanvasButton->setToolTip("Clear Canvas");
    connect(clearCanvasButton, &QPushButton::clicked, this, &PixInpainter::clearCanvas);

    historyLayout->addWidget(undoButton);
    historyLayout->addWidget(redoButton);
    historyLayout->addWidget(clearCanvasButton);

    mainLayout->addWidget(canvasGroup);

    toolbar->addWidget(utilityContainer);
}

void PixInpainter::setupColorAndPenSizeControls(QToolBar* toolbar)
{
    QWidget* colorContainer = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(colorContainer);
    layout->setContentsMargins(COLOR_LAYOUT_MARGIN_H, COLOR_LAYOUT_MARGIN_V,
        COLOR_LAYOUT_MARGIN_H, COLOR_LAYOUT_MARGIN_V);
    layout->setSpacing(HBOX_SPACING);

    m_leftColorBtn = new QPushButton(this);
    m_leftColorBtn->setFixedSize(COLOR_BUTTON_SIZE, COLOR_BUTTON_SIZE);
    m_leftColorBtn->setStyleSheet(QString("background-color: black; border: %1px solid gray; border-radius: %2px;")
        .arg(COLOR_BUTTON_BORDER_WIDTH)
        .arg(COLOR_BUTTON_BORDER_RADIUS));

    connect(m_leftColorBtn, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(Qt::black, this, "Choose primary color");
        if (color.isValid()) {
            m_paintWidget->changePrimaryColor(color);
            m_leftColorBtn->setStyleSheet(QString("background-color: %1; border: %2px solid gray; border-radius: %3px;")
                .arg(color.name())
                .arg(COLOR_BUTTON_BORDER_WIDTH)
                .arg(COLOR_BUTTON_BORDER_RADIUS));
            statusBar()->showMessage(QString("Primary color changed"), 2000);
        }
        });
    layout->addWidget(m_leftColorBtn);

    m_rightColorBtn = new QPushButton(this);
    m_rightColorBtn->setFixedSize(COLOR_BUTTON_SIZE, COLOR_BUTTON_SIZE);
    m_rightColorBtn->setStyleSheet(QString("background-color: white; border: %1px solid gray; border-radius: %2px;")
        .arg(COLOR_BUTTON_BORDER_WIDTH)
        .arg(COLOR_BUTTON_BORDER_RADIUS));

    connect(m_rightColorBtn, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(Qt::white, this, "Choose secondary color");
        if (color.isValid()) {
            m_paintWidget->changeSecondaryColor(color);
            m_rightColorBtn->setStyleSheet(QString("background-color: %1; border: %2px solid gray; border-radius: %3px;")
                .arg(color.name())
                .arg(COLOR_BUTTON_BORDER_WIDTH)
                .arg(COLOR_BUTTON_BORDER_RADIUS));
            statusBar()->showMessage(QString("Secondary color changed"), 2000);
        }
        });
    layout->addWidget(m_rightColorBtn);

    QLabel* penSizeLabel = new QLabel("Pen Size:", this);
    QFont font = penSizeLabel->font();
    font.setPointSize(font.pointSize() + FONT_SIZE_INCREASE);
    penSizeLabel->setFont(font);
    layout->addWidget(penSizeLabel);

    m_penSizeComboBox = new QComboBox(this);
    m_penSizeComboBox->setFont(font);
    m_penSizeComboBox->addItem("1", PEN_SIZE_SMALL);
    m_penSizeComboBox->addItem("2", PEN_SIZE_MEDIUM);
    m_penSizeComboBox->addItem("4", PEN_SIZE_LARGE);
    m_penSizeComboBox->setMinimumHeight(COMBO_MIN_HEIGHT);
    m_penSizeComboBox->setCurrentIndex(1);

    connect(m_penSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &PixInpainter::updatePenSize);

    layout->addWidget(m_penSizeComboBox);

    toolbar->addWidget(colorContainer);
}

void PixInpainter::setupViewControls(QToolBar* toolbar)
{
    QWidget* controlsContainer = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(controlsContainer);
    mainLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN / 2,
        LAYOUT_MARGIN, LAYOUT_MARGIN / 2);
    mainLayout->setSpacing(HBOX_SPACING);

    QGroupBox* zoomGroup = new QGroupBox("Zoom", controlsContainer);
    zoomGroup->setStyleSheet("QGroupBox { font-weight: bold; }");
    QHBoxLayout* zoomLayout = new QHBoxLayout(zoomGroup);
    zoomLayout->setContentsMargins(8, 12, 8, 8);
    zoomLayout->setSpacing(4);

    QToolButton* zoomInButton = new QToolButton(zoomGroup);
    zoomInButton->setIcon(QIcon(":/icons/ZoomIn.png"));
    zoomInButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    zoomInButton->setToolTip("Zoom In");
    zoomInButton->setFixedSize(TOOL_BUTTON_SIZE, TOOL_BUTTON_SIZE);
    connect(zoomInButton, &QToolButton::clicked, this, &PixInpainter::zoomIn);

    QToolButton* zoomOutButton = new QToolButton(zoomGroup);
    zoomOutButton->setIcon(QIcon(":/icons/ZoomOut.png"));
    zoomOutButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    zoomOutButton->setToolTip("Zoom Out");
    zoomOutButton->setFixedSize(TOOL_BUTTON_SIZE, TOOL_BUTTON_SIZE);
    connect(zoomOutButton, &QToolButton::clicked, this, &PixInpainter::zoomOut);

    QPushButton* resetZoomButton = new QPushButton("Reset", zoomGroup);
    QFont smallFont;
    smallFont.setPointSize(smallFont.pointSize() + FONT_SIZE_INCREASE - 1);
    resetZoomButton->setFont(smallFont);
    resetZoomButton->setToolTip("Reset Zoom");
    connect(resetZoomButton, &QPushButton::clicked, this, &PixInpainter::resetZoom);

    zoomLayout->addWidget(zoomInButton);
    zoomLayout->addWidget(zoomOutButton);
    zoomLayout->addWidget(resetZoomButton);

    mainLayout->addWidget(zoomGroup);

    QGroupBox* gridGroup = new QGroupBox("Grid", controlsContainer);
    gridGroup->setStyleSheet("QGroupBox { font-weight: bold; }");
    QHBoxLayout* gridLayout = new QHBoxLayout(gridGroup);
    gridLayout->setContentsMargins(8, 12, 8, 8);
    gridLayout->setSpacing(8);

    m_gridCheckBox = new QCheckBox("Show", this);
    m_gridCheckBox->setFont(smallFont);
    connect(m_gridCheckBox, &QCheckBox::toggled, this, &PixInpainter::toggleGrid);

    QLabel* gridSizeLabel = new QLabel("Size:", this);
    gridSizeLabel->setFont(smallFont);

    m_gridSizeComboBox = new QComboBox(this);
    m_gridSizeComboBox->setFont(smallFont);
    m_gridSizeComboBox->addItem("8", 8);
    m_gridSizeComboBox->addItem("16", 16);
    m_gridSizeComboBox->addItem("32", 32);
    m_gridSizeComboBox->addItem("64", 64);
    m_gridSizeComboBox->setMinimumHeight(COMBO_MIN_HEIGHT);
    m_gridSizeComboBox->setMaximumWidth(50);
    m_gridSizeComboBox->setCurrentIndex(0);

    connect(m_gridSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](int index) {
            int gridSize = m_gridSizeComboBox->itemData(index).toInt();
            updateGridSize(gridSize);
        });

    gridLayout->addWidget(m_gridCheckBox);
    gridLayout->addWidget(gridSizeLabel);
    gridLayout->addWidget(m_gridSizeComboBox);

    mainLayout->addWidget(gridGroup);

    toolbar->addWidget(controlsContainer);

    m_zoomInAction = new QAction(QIcon(":/icons/ZoomIn.png"), "Zoom In", this);
    m_zoomInAction->setShortcut(Qt::CTRL | Qt::Key_Plus);
    connect(m_zoomInAction, &QAction::triggered, this, &PixInpainter::zoomIn);

    m_zoomOutAction = new QAction(QIcon(":/icons/ZoomOut.png"), "Zoom Out", this);
    m_zoomOutAction->setShortcut(Qt::CTRL | Qt::Key_Minus);
    connect(m_zoomOutAction, &QAction::triggered, this, &PixInpainter::zoomOut);
}

void PixInpainter::setupAiGenControl(QToolBar* toolbar)
{
    QWidget* aiContainer = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(aiContainer);
    mainLayout->setContentsMargins(LAYOUT_MARGIN, LAYOUT_MARGIN / 2,
        LAYOUT_MARGIN, LAYOUT_MARGIN / 2);
    mainLayout->setSpacing(HBOX_SPACING);

    QGroupBox* group = new QGroupBox("AI Completion", aiContainer);
    group->setStyleSheet("QGroupBox { font-weight: bold; }");
    QHBoxLayout* layout = new QHBoxLayout(group);
    layout->setContentsMargins(8, 12, 8, 8);
    layout->setSpacing(4);

    QToolButton* aiButton = new QToolButton(group);
    aiButton->setIcon(QIcon(":/icons/AiGenerate.png"));
    aiButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    aiButton->setToolTip("AI Completion Tool");
    aiButton->setFixedSize(TOOL_BUTTON_SIZE, TOOL_BUTTON_SIZE);
    connect(aiButton, &QToolButton::clicked, this, &PixInpainter::showAICompletionWidget);

    layout->addWidget(aiButton);
    mainLayout->addWidget(group);
    toolbar->addWidget(aiContainer);
}

void PixInpainter::connectToolActions()
{
    connect(m_penAction, &QAction::triggered, this, &PixInpainter::onPenToolSelected);
    connect(m_eraserAction, &QAction::triggered, this, &PixInpainter::onEraserToolSelected);
    connect(m_fillAction, &QAction::triggered, this, &PixInpainter::onFillToolSelected);
    connect(m_eyedropperAction, &QAction::triggered, this, &PixInpainter::onEyedropperToolSelected);
    connect(m_lineAction, &QAction::triggered, this, &PixInpainter::onLineToolSelected);
    connect(m_rectangleAction, &QAction::triggered, this, &PixInpainter::onRectangleToolSelected);
    connect(m_ellipseAction, &QAction::triggered, this, &PixInpainter::onEllipseToolSelected);
    connect(m_triangleAction, &QAction::triggered, this, &PixInpainter::onTriangleToolSelected);
}

void PixInpainter::onPenToolSelected()
{
    m_previousToolAction = m_penAction;
    m_paintWidget->setTool(paint::Tool::Pen);
    m_paintController->setTool(paint::Tool::Pen);
    m_paintWidget->changePenSize(m_currentPenSize);
    statusBar()->showMessage("Pen tool selected", 2000);
}

void PixInpainter::onEraserToolSelected()
{
    m_previousToolAction = m_eraserAction;
    m_paintWidget->setTool(paint::Tool::Eraser);
    m_paintController->setTool(paint::Tool::Eraser);
    statusBar()->showMessage("Eraser tool selected", 2000);
}

void PixInpainter::onFillToolSelected()
{
    m_previousToolAction = m_fillAction;
    m_paintWidget->setTool(paint::Tool::Fill);
    m_paintController->setTool(paint::Tool::Fill);
    statusBar()->showMessage("Fill tool selected", 2000);
}

void PixInpainter::onEyedropperToolSelected()
{
    m_paintWidget->setTool(paint::Tool::Eyedropper);
    m_paintController->setTool(paint::Tool::Eyedropper);
    statusBar()->showMessage("Eyedropper tool selected", 2000);
}

void PixInpainter::onLineToolSelected()
{
    m_previousToolAction = m_lineAction;
    m_paintWidget->setTool(paint::Tool::Line);
    m_paintController->setTool(paint::Tool::Line);
    statusBar()->showMessage("Line tool selected", 2000);
}

void PixInpainter::onRectangleToolSelected()
{
    m_previousToolAction = m_rectangleAction;
    m_paintWidget->setTool(paint::Tool::Rectangle);
    m_paintController->setTool(paint::Tool::Rectangle);
    statusBar()->showMessage("Rectangle tool selected", 2000);
}

void PixInpainter::onEllipseToolSelected()
{
    m_previousToolAction = m_ellipseAction;
    m_paintWidget->setTool(paint::Tool::Ellipse);
    m_paintController->setTool(paint::Tool::Ellipse);
    statusBar()->showMessage("Ellipse tool selected", 2000);
}

void PixInpainter::onTriangleToolSelected()
{
    m_previousToolAction = m_triangleAction;
    m_paintWidget->setTool(paint::Tool::Triangle);
    m_paintController->setTool(paint::Tool::Triangle);
    statusBar()->showMessage("Triangle tool selected", 2000);
}

void PixInpainter::onUndo()
{
    m_paintWidget->undo();
    statusBar()->showMessage("Undone last action", 2000);
}

void PixInpainter::onRedo()
{
    m_paintWidget->redo();
    statusBar()->showMessage("Redone last action", 2000);
}

void PixInpainter::handleColorPicked(const QColor& color, bool leftButton)
{
    QString message;
    if (leftButton)
    {
        m_paintWidget->changePrimaryColor(color);
        m_leftColorBtn->setStyleSheet(QString("background-color: %1; border: %2px solid gray; border-radius: %3px;")
            .arg(color.name())
            .arg(COLOR_BUTTON_BORDER_WIDTH)
            .arg(COLOR_BUTTON_BORDER_RADIUS));
        message = "Primary color changed";
    }
    else
    {
        m_paintWidget->changeSecondaryColor(color);
        m_rightColorBtn->setStyleSheet(QString("background-color: %1; border: %2px solid gray; border-radius: %3px;")
            .arg(color.name())
            .arg(COLOR_BUTTON_BORDER_WIDTH)
            .arg(COLOR_BUTTON_BORDER_RADIUS));
        message = "Secondary color changed";
    }

    if (m_previousToolAction)
        m_previousToolAction->trigger();

    statusBar()->showMessage(QString(message), 2000);
}

void PixInpainter::resetZoom()
{
    if (m_paintWidget)
    {
        m_paintWidget->resetZoom();
    }
}

void PixInpainter::zoomIn()
{
    if (m_paintWidget)
    {
        m_paintWidget->zoomIn();
    }
}

void PixInpainter::zoomOut()
{
    if (m_paintWidget)
    {
        m_paintWidget->zoomOut();
    }
}

void PixInpainter::clearCanvas()
{
    m_paintWidget->clearCanvas();
    statusBar()->showMessage("Canvas cleared", 2000);
}

void PixInpainter::loadImageFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open Image",
        QString(),
        "Images (*.png *.jpg *.jpeg *.bmp *.gif)"
    );

    if (!fileName.isEmpty())
    {
        QImageReader reader(fileName);
        QImage image = reader.read();
        if (!image.isNull())
        {
            m_paintWidget->loadImage(image.convertToFormat(QImage::Format_ARGB32));
            statusBar()->showMessage(QString("Image loaded: %1").arg(QFileInfo(fileName).fileName()), 2000);
        }
        else
        {
            QMessageBox::warning(this, "Error", "Failed to load image: " + reader.errorString());
            statusBar()->showMessage("Failed to load image", 2000);
        }
    }
}

void PixInpainter::saveImageToFile()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Image",
        QString(),
        "PNG Images (*.png);;JPEG Images (*.jpg *.jpeg);;BMP Images (*.bmp)"
    );

    if (!fileName.isEmpty())
    {
        const QImage& canvasImage = m_paintWidget->getCanvasImage();
        if (!canvasImage.save(fileName))
        {
            QMessageBox::warning(this, "Error", "Failed to save image");
            statusBar()->showMessage("Failed to save image", 2000);
        }
        else
        {
            statusBar()->showMessage(QString("Image saved: %1").arg(QFileInfo(fileName).fileName()), 2000);
        }
    }
}

void PixInpainter::toggleGrid(bool show)
{
    m_paintWidget->toggleGrid(show);
    if (show)
    {
        int size = m_gridSizeComboBox ? m_gridSizeComboBox->currentData().toInt() : 8;
        statusBar()->showMessage(QString("Grid enabled (size: %1)").arg(size), 2000);
    }
    else
    {
        statusBar()->showMessage("Grid disabled", 2000);
    }
}

void PixInpainter::updateGridSize(int size)
{
    m_paintWidget->setGridSize(size);
    if (m_gridCheckBox && m_gridCheckBox->isChecked())
    {
        statusBar()->showMessage(QString("Grid size set to %1").arg(size), 2000);
    }
}

void PixInpainter::updatePenSize(int index)
{
    if (m_penSizeComboBox)
    {
        m_currentPenSize = m_penSizeComboBox->itemData(index).toInt();

        m_paintWidget->changePenSize(m_currentPenSize);

        if (m_eraserAction && m_eraserAction->isChecked())
        {
            m_paintWidget->updateToolCursor();
        }

        statusBar()->showMessage(QString("Pen size changed to %1").arg(m_currentPenSize), 2000);
    }
}

void PixInpainter::showAICompletionWidget()
{
    if (!m_aiCompletionModel)
    {
        m_aiCompletionModel = new paint::AICompletionModel(this);
        m_aiCompletionController = new paint::AICompletionController(this, m_aiCompletionModel);
    }
    else
    {
        m_aiCompletionModel->reinitializeModels();
    }

    if (!m_aiCompletionWidget)
    {
        m_aiCompletionWidget = new paint::AICompletionWidget(nullptr);
        m_aiCompletionWidget->setAttribute(Qt::WA_DeleteOnClose);

        connect(m_aiCompletionWidget, &QWidget::destroyed, this, [this]()
            {
                if (m_aiCompletionController)
                {
                    m_aiCompletionController->setView(nullptr);
                }
                m_aiCompletionWidget = nullptr;
            });

        connect(m_aiCompletionWidget, &paint::AICompletionWidget::applyResultToCanvasRequested,
            this, &PixInpainter::onResultImageAppliedToCanvas);

        if (m_aiCompletionController)
        {
            m_aiCompletionController->setView(m_aiCompletionWidget);
        }
    }
    else
    {
        if (m_aiCompletionController)
        {
            m_aiCompletionController->setView(m_aiCompletionWidget);
        }
    }

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    if (m_paintWidget)
    {
        m_paintWidget->getCanvasImage().save(&buffer, "PNG");
    }
    buffer.close();

    if (m_aiCompletionController)
    {
        m_aiCompletionController->setImageData(imageData);
    }

    m_aiCompletionWidget->show();
    m_aiCompletionWidget->activateWindow();
    m_aiCompletionWidget->raise();
}

void PixInpainter::onResultImageAppliedToCanvas(const QPixmap& pixmap)
{
    if (!pixmap.isNull() && m_paintWidget)
    {
        QImage image = pixmap.toImage();
        if (image.format() != QImage::Format_ARGB32)
        {
            image = image.convertToFormat(QImage::Format_ARGB32);
        }
        m_paintWidget->loadImage(image);
        statusBar()->showMessage("Result image applied to canvas.", 3000);
    }
}

void PixInpainter::copyCanvas()
{
    QClipboard* clipboard = QApplication::clipboard();
    const QImage& canvasImage = m_paintWidget->getCanvasImage();

    if (!canvasImage.isNull())
    {
        QImage copyImage = canvasImage.copy();
        clipboard->setImage(copyImage);

        statusBar()->showMessage("Image copied to clipboard", 2000);
    }
}

void PixInpainter::pasteCanvas()
{
    QClipboard* clipboard = QApplication::clipboard();
    const QImage image = clipboard->image();

    if (!image.isNull())
    {
        QImage convertedImage = image.convertToFormat(QImage::Format_ARGB32);
        m_paintWidget->loadImage(convertedImage);

        statusBar()->showMessage("Image pasted from clipboard", 2000);
    }
    else
    {
        statusBar()->showMessage("No image found in clipboard", 2000);
    }
}
