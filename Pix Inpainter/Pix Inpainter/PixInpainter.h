#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PixInpainter.h"

#include "AICompletionWidget.h"
#include "AICompletionController.h"
#include "AICompletionModel.h"
#include "PaintWidget.h"

#include <QActionGroup>
#include <QToolButton>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>
#include <QAction>
#include <QString> 
#include <QPixmap>
#include <QColor>
#include <QIcon>

class PixInpainter : public QMainWindow
{
    Q_OBJECT

public:
    static constexpr int ICON_SIZE = 28;
    static constexpr int TOOL_BUTTON_SIZE = 36;
    static constexpr int COLOR_BUTTON_SIZE = 36;

    static constexpr int LAYOUT_SPACING = 6;
    static constexpr int GRID_LAYOUT_SPACING = 6;
    static constexpr int HBOX_SPACING = 8;

    static constexpr int LAYOUT_MARGIN = 4;
    static constexpr int COLOR_LAYOUT_MARGIN_V = 2;
    static constexpr int COLOR_LAYOUT_MARGIN_H = 4;

    static constexpr int FONT_SIZE_INCREASE = 1;
    static constexpr int COMBO_MIN_HEIGHT = 26;

    static constexpr int COLOR_BUTTON_BORDER_WIDTH = 2;
    static constexpr int COLOR_BUTTON_BORDER_RADIUS = 5;

    static constexpr int PEN_SIZE_SMALL = 1;
    static constexpr int PEN_SIZE_MEDIUM = 2;
    static constexpr int PEN_SIZE_LARGE = 4;

public:
    PixInpainter(QWidget *parent = nullptr);
    ~PixInpainter();

    PixInpainter(const PixInpainter&) = delete;
    PixInpainter& operator=(const PixInpainter&) = delete;

    PixInpainter(PixInpainter&&) = delete;
    PixInpainter& operator=(PixInpainter&&) = delete;

private slots:
    void handleColorPicked(const QColor& color, bool leftButton);

    void onPenToolSelected();
    void onEraserToolSelected();
    void onFillToolSelected();
    void onEyedropperToolSelected();
    void onLineToolSelected();
    void onRectangleToolSelected();
    void onEllipseToolSelected();
    void onTriangleToolSelected();

    void resetZoom();
    void clearCanvas();
    void loadImageFromFile();

    void toggleGrid(bool show);
    void updateGridSize(int size);

    void showAICompletionWidget();
    void copyCanvas();
    void pasteCanvas();

    void zoomIn();
    void zoomOut();
    void saveImageToFile();

    void onUndo();
    void onRedo();

    void updatePenSize(int index);

    void onResultImageAppliedToCanvas(const QPixmap& image);

private:
    void setupMainUI();
    void setupMenus();
    void setupToolbar();

    void setupDrawingTools(QToolBar* toolbar);
    void setupUtilityTools(QToolBar* toolbar);

    void setupColorAndPenSizeControls(QToolBar* toolbar);
    void setupViewControls(QToolBar* toolbar);
    void setupAiGenControl(QToolBar* toolbar);

    QToolButton* createToolButton(const QIcon& icon, const QString& text);
    void connectToolActions();

private:
    Ui::PixInpainterClass m_ui;
    paint::PaintWidget* m_paintWidget;
    paint::AICompletionWidget* m_aiCompletionWidget;

    QPushButton* m_leftColorBtn;
    QPushButton* m_rightColorBtn;

    QActionGroup* m_toolGroup;
    QAction* m_penAction;
    QAction* m_eraserAction;
    QAction* m_fillAction;
    QAction* m_eyedropperAction;
    QAction* m_lineAction;
    QAction* m_rectangleAction;
    QAction* m_ellipseAction;
    QAction* m_triangleAction;
    QAction* m_previousToolAction;

    QAction* m_AICompletionAction;
    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;

    QCheckBox* m_gridCheckBox;
    QComboBox* m_gridSizeComboBox;

    QComboBox* m_penSizeComboBox;
    int m_currentPenSize = PEN_SIZE_MEDIUM;

    paint::PaintController* m_paintController;
    paint::ICanvasModelPtr m_canvasModel;

    paint::AICompletionModel* m_aiCompletionModel;
    paint::AICompletionController* m_aiCompletionController;
};

