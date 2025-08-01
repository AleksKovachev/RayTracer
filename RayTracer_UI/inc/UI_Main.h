/********************************************************************************
** Form generated from reading UI file 'RayTracer_responsive.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "CustomWidgets.h"

QT_BEGIN_NAMESPACE

class Ui_RayTracer
{
public:
    QAction *actionRender;
    QAction *actionExit;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *sceneFileLayout;
    QPushButton *sceneFileBtn;
    QLineEdit *sceneFileEntry;
    QHBoxLayout *saveDirLayout;
    QPushButton *saveDirBtn;
    QLineEdit *saveDirEntry;
    QScrollArea *settingsScrollArea;
    QWidget *scrollAreaContents;
    QVBoxLayout *verticalLayout;
    QFrame *scrollSettingsFrame;
    QFormLayout *formLayout;
    QSpinBox *resWidthSpin;
    QSpinBox *resHeightSpin;
    QLabel *colorDepthLabel;
    QSpinBox *colorDepthSpin;
    QLabel *shadowBiasLabel;
    SmartDoubleSpinBox *shadowBiasSpin;
    QLabel *refractBiasLabel;
    SmartDoubleSpinBox *refractBiasSpin;
    QLabel *pathDepthLabel;
    QSpinBox *pathDepthSpin;
    QLabel *renderModeLabel;
    QComboBox *renderModeCombo;
    QSpinBox *bucketSizeSpin;
    QLabel *accTreeMaxDepthLabel;
    QSpinBox *accTreeMaxDepthSpin;
    QLabel *maxAABBTriCountLabel;
    QSpinBox *maxAABBTriCountSpin;
    QLabel *rendNameLabel;
    QLineEdit *rendNameEntry;
    QPushButton *overrideBGColorBtn;
    QToolButton *colorPickerBtn;
    QPushButton *enableGIBtn;
    QLabel *giSamplesLabel;
    QSpinBox *giSamplesSpin;
    QPushButton *ignoreBackfaceBtn;
    QGroupBox *cameraBox;
    QGridLayout *gridLayout_3;
    QComboBox *camActionCombo;
    QLabel *camInitialPosLabel;
    QPushButton *sceneInitialCamPosBtn;
    QLabel *xInitialLabel;
    SmartDoubleSpinBox *camXInitialSpin;
    QLabel *yInitialLabel;
    SmartDoubleSpinBox *camYInitialSpin;
    QLabel *zInitialLabel;
    SmartDoubleSpinBox *camZInitialSpin;
    QLabel *camActionLabel;
    QLabel *xActionLabel;
    SmartDoubleSpinBox *camXActionSpin;
    QLabel *yActionLabel;
    SmartDoubleSpinBox *camYActionSpin;
    QLabel *zActionLabel;
    SmartDoubleSpinBox *camZActionSpin;
    QLabel *frameNumLabel;
    QSpinBox *frameNumSpin;
    QPushButton *applyAntialiasBtn;
    QSpacerItem *verticalSpacer;
    QPushButton *sRGBBtn;
    QPushButton *custBucketSizeBtn;
    QPushButton *custWidthBtn;
    QPushButton *custHeightBtn;
    QLabel *antialiasLabel;
    QComboBox *antialiasCombo;
    QSpinBox *SSAASpin;
    QGridLayout *viewerLayout;
    QVBoxLayout *viewportLayout;
    ImageViewer *qViewport;
    QPushButton *openImgBtn;
    QSpacerItem *horizontalSpacer;
    QPushButton *prevImgBtn;
    QPushButton *nextImgBtn;
    QPushButton *zoomResetBtn;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *renderBtn;
    QMenuBar *menubar;
    QMenu *menuMenu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RayTracer)
    {
        if (RayTracer->objectName().isEmpty())
            RayTracer->setObjectName("RayTracer");
        RayTracer->resize(1280, 720);
        RayTracer->setMinimumSize(QSize(1280, 720));
        RayTracer->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../../../../../../.designer/backup/icons/RayTracing.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        RayTracer->setWindowIcon(icon);
        actionRender = new QAction(RayTracer);
        actionRender->setObjectName("actionRender");
        actionExit = new QAction(RayTracer);
        actionExit->setObjectName("actionExit");
        centralwidget = new QWidget(RayTracer);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        sceneFileLayout = new QHBoxLayout();
        sceneFileLayout->setObjectName("sceneFileLayout");
        sceneFileLayout->setContentsMargins(10, -1, 10, -1);
        sceneFileBtn = new QPushButton(centralwidget);
        sceneFileBtn->setObjectName("sceneFileBtn");
        sceneFileBtn->setMaximumSize(QSize(16777215, 24));

        sceneFileLayout->addWidget(sceneFileBtn);

        sceneFileEntry = new QLineEdit(centralwidget);
        sceneFileEntry->setObjectName("sceneFileEntry");
        sceneFileEntry->setMaximumSize(QSize(16777215, 21));
        sceneFileEntry->setFrame(false);

        sceneFileLayout->addWidget(sceneFileEntry);


        gridLayout_2->addLayout(sceneFileLayout, 0, 0, 1, 6);

        saveDirLayout = new QHBoxLayout();
        saveDirLayout->setObjectName("saveDirLayout");
        saveDirLayout->setContentsMargins(10, -1, 10, -1);
        saveDirBtn = new QPushButton(centralwidget);
        saveDirBtn->setObjectName("saveDirBtn");
        saveDirBtn->setMaximumSize(QSize(16777215, 24));

        saveDirLayout->addWidget(saveDirBtn);

        saveDirEntry = new QLineEdit(centralwidget);
        saveDirEntry->setObjectName("saveDirEntry");
        saveDirEntry->setMaximumSize(QSize(16777215, 21));
        saveDirEntry->setFrame(false);

        saveDirLayout->addWidget(saveDirEntry);


        gridLayout_2->addLayout(saveDirLayout, 1, 0, 1, 6);

        settingsScrollArea = new QScrollArea(centralwidget);
        settingsScrollArea->setObjectName("settingsScrollArea");
        settingsScrollArea->setMinimumSize(QSize(270, 0));
        settingsScrollArea->setMaximumSize(QSize(270, 16777215));
        settingsScrollArea->setAutoFillBackground(true);
        settingsScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        settingsScrollArea->setFrameShadow(QFrame::Shadow::Plain);
        settingsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        settingsScrollArea->setWidgetResizable(true);
        scrollAreaContents = new QWidget();
        scrollAreaContents->setObjectName("scrollAreaContents");
        scrollAreaContents->setGeometry(QRect(0, -123, 312, 755));
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollAreaContents->sizePolicy().hasHeightForWidth());
        scrollAreaContents->setSizePolicy(sizePolicy);
        scrollAreaContents->setAutoFillBackground(true);
        verticalLayout = new QVBoxLayout(scrollAreaContents);
        verticalLayout->setObjectName("verticalLayout");
        scrollSettingsFrame = new QFrame(scrollAreaContents);
        scrollSettingsFrame->setObjectName("scrollSettingsFrame");
        scrollSettingsFrame->setMinimumSize(QSize(0, 0));
        scrollSettingsFrame->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        scrollSettingsFrame->setFrameShape(QFrame::Shape::NoFrame);
        scrollSettingsFrame->setFrameShadow(QFrame::Shadow::Raised);
        scrollSettingsFrame->setLineWidth(1);
        scrollSettingsFrame->setMidLineWidth(0);
        formLayout = new QFormLayout(scrollSettingsFrame);
        formLayout->setObjectName("formLayout");
        resWidthSpin = new QSpinBox(scrollSettingsFrame);
        resWidthSpin->setObjectName("resWidthSpin");
        resWidthSpin->setEnabled(false);
        resWidthSpin->setMinimumSize(QSize(0, 0));
        resWidthSpin->setMaximumSize(QSize(110, 16777215));
        resWidthSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        resWidthSpin->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
        resWidthSpin->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        resWidthSpin->setWrapping(false);
        resWidthSpin->setFrame(false);
        resWidthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        resWidthSpin->setProperty("showGroupSeparator", QVariant(false));
        resWidthSpin->setMinimum(2);
        resWidthSpin->setMaximum(20000);
        resWidthSpin->setValue(1920);
        resWidthSpin->setDisplayIntegerBase(10);

        formLayout->setWidget(0, QFormLayout::FieldRole, resWidthSpin);

        resHeightSpin = new QSpinBox(scrollSettingsFrame);
        resHeightSpin->setObjectName("resHeightSpin");
        resHeightSpin->setEnabled(false);
        resHeightSpin->setMinimumSize(QSize(0, 0));
        resHeightSpin->setMaximumSize(QSize(110, 16777215));
        resHeightSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        resHeightSpin->setWrapping(false);
        resHeightSpin->setFrame(false);
        resHeightSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        resHeightSpin->setProperty("showGroupSeparator", QVariant(false));
        resHeightSpin->setMinimum(2);
        resHeightSpin->setMaximum(20000);
        resHeightSpin->setValue(1080);
        resHeightSpin->setDisplayIntegerBase(10);

        formLayout->setWidget(1, QFormLayout::FieldRole, resHeightSpin);

        colorDepthLabel = new QLabel(scrollSettingsFrame);
        colorDepthLabel->setObjectName("colorDepthLabel");

        formLayout->setWidget(2, QFormLayout::LabelRole, colorDepthLabel);

        colorDepthSpin = new QSpinBox(scrollSettingsFrame);
        colorDepthSpin->setObjectName("colorDepthSpin");
        colorDepthSpin->setMinimumSize(QSize(0, 0));
        colorDepthSpin->setMaximumSize(QSize(110, 16777215));
        colorDepthSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        colorDepthSpin->setWrapping(false);
        colorDepthSpin->setFrame(false);
        colorDepthSpin->setReadOnly(true);
        colorDepthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        colorDepthSpin->setProperty("showGroupSeparator", QVariant(false));
        colorDepthSpin->setMinimum(2);
        colorDepthSpin->setMaximum(24);
        colorDepthSpin->setValue(8);
        colorDepthSpin->setDisplayIntegerBase(10);

        formLayout->setWidget(2, QFormLayout::FieldRole, colorDepthSpin);

        shadowBiasLabel = new QLabel(scrollSettingsFrame);
        shadowBiasLabel->setObjectName("shadowBiasLabel");

        formLayout->setWidget(3, QFormLayout::LabelRole, shadowBiasLabel);

        shadowBiasSpin = new SmartDoubleSpinBox(scrollSettingsFrame);
        shadowBiasSpin->setObjectName("shadowBiasSpin");
        shadowBiasSpin->setMinimumSize(QSize(0, 0));
        shadowBiasSpin->setMaximumSize(QSize(110, 16777215));
        shadowBiasSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        shadowBiasSpin->setFrame(false);
        shadowBiasSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        shadowBiasSpin->setDecimals(9);
        shadowBiasSpin->setMaximum(1.000000000000000);
        shadowBiasSpin->setSingleStep(0.010000000000000);
        shadowBiasSpin->setValue(0.100000000000000);

        formLayout->setWidget(3, QFormLayout::FieldRole, shadowBiasSpin);

        refractBiasLabel = new QLabel(scrollSettingsFrame);
        refractBiasLabel->setObjectName("refractBiasLabel");

        formLayout->setWidget(4, QFormLayout::LabelRole, refractBiasLabel);

        refractBiasSpin = new SmartDoubleSpinBox(scrollSettingsFrame);
        refractBiasSpin->setObjectName("refractBiasSpin");
        refractBiasSpin->setMinimumSize(QSize(0, 0));
        refractBiasSpin->setMaximumSize(QSize(110, 16777215));
        refractBiasSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        refractBiasSpin->setFrame(false);
        refractBiasSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        refractBiasSpin->setDecimals(9);
        refractBiasSpin->setMaximum(1.000000000000000);
        refractBiasSpin->setSingleStep(0.010000000000000);
        refractBiasSpin->setValue(0.001000000000000);

        formLayout->setWidget(4, QFormLayout::FieldRole, refractBiasSpin);

        pathDepthLabel = new QLabel(scrollSettingsFrame);
        pathDepthLabel->setObjectName("pathDepthLabel");

        formLayout->setWidget(5, QFormLayout::LabelRole, pathDepthLabel);

        pathDepthSpin = new QSpinBox(scrollSettingsFrame);
        pathDepthSpin->setObjectName("pathDepthSpin");
        pathDepthSpin->setMinimumSize(QSize(0, 0));
        pathDepthSpin->setMaximumSize(QSize(110, 16777215));
        pathDepthSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        pathDepthSpin->setWrapping(false);
        pathDepthSpin->setFrame(false);
        pathDepthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        pathDepthSpin->setProperty("showGroupSeparator", QVariant(false));
        pathDepthSpin->setMinimum(2);
        pathDepthSpin->setMaximum(100);
        pathDepthSpin->setValue(5);
        pathDepthSpin->setDisplayIntegerBase(10);

        formLayout->setWidget(5, QFormLayout::FieldRole, pathDepthSpin);

        renderModeLabel = new QLabel(scrollSettingsFrame);
        renderModeLabel->setObjectName("renderModeLabel");

        formLayout->setWidget(6, QFormLayout::LabelRole, renderModeLabel);

        renderModeCombo = new QComboBox(scrollSettingsFrame);
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->setObjectName("renderModeCombo");
        renderModeCombo->setMinimumSize(QSize(0, 0));
        renderModeCombo->setMaximumSize(QSize(110, 16777215));
        renderModeCombo->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        renderModeCombo->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        renderModeCombo->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-top: 1px;\n"
"padding-bottom: 1px;"));

        formLayout->setWidget(6, QFormLayout::FieldRole, renderModeCombo);

        bucketSizeSpin = new QSpinBox(scrollSettingsFrame);
        bucketSizeSpin->setObjectName("bucketSizeSpin");
        bucketSizeSpin->setEnabled(false);
        bucketSizeSpin->setMinimumSize(QSize(0, 0));
        bucketSizeSpin->setMaximumSize(QSize(110, 16777215));
        bucketSizeSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        bucketSizeSpin->setWrapping(false);
        bucketSizeSpin->setFrame(false);
        bucketSizeSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        bucketSizeSpin->setProperty("showGroupSeparator", QVariant(false));
        bucketSizeSpin->setMinimum(2);
        bucketSizeSpin->setMaximum(1000);
        bucketSizeSpin->setValue(24);
        bucketSizeSpin->setDisplayIntegerBase(10);

        formLayout->setWidget(7, QFormLayout::FieldRole, bucketSizeSpin);

        accTreeMaxDepthLabel = new QLabel(scrollSettingsFrame);
        accTreeMaxDepthLabel->setObjectName("accTreeMaxDepthLabel");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(accTreeMaxDepthLabel->sizePolicy().hasHeightForWidth());
        accTreeMaxDepthLabel->setSizePolicy(sizePolicy1);

        formLayout->setWidget(8, QFormLayout::LabelRole, accTreeMaxDepthLabel);

        accTreeMaxDepthSpin = new QSpinBox(scrollSettingsFrame);
        accTreeMaxDepthSpin->setObjectName("accTreeMaxDepthSpin");
        accTreeMaxDepthSpin->setMinimumSize(QSize(0, 0));
        accTreeMaxDepthSpin->setMaximumSize(QSize(110, 16777215));
        accTreeMaxDepthSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        accTreeMaxDepthSpin->setWrapping(false);
        accTreeMaxDepthSpin->setFrame(false);
        accTreeMaxDepthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        accTreeMaxDepthSpin->setProperty("showGroupSeparator", QVariant(false));
        accTreeMaxDepthSpin->setMinimum(1);
        accTreeMaxDepthSpin->setMaximum(30);
        accTreeMaxDepthSpin->setValue(15);
        accTreeMaxDepthSpin->setDisplayIntegerBase(10);

        formLayout->setWidget(8, QFormLayout::FieldRole, accTreeMaxDepthSpin);

        maxAABBTriCountLabel = new QLabel(scrollSettingsFrame);
        maxAABBTriCountLabel->setObjectName("maxAABBTriCountLabel");

        formLayout->setWidget(9, QFormLayout::LabelRole, maxAABBTriCountLabel);

        maxAABBTriCountSpin = new QSpinBox(scrollSettingsFrame);
        maxAABBTriCountSpin->setObjectName("maxAABBTriCountSpin");
        maxAABBTriCountSpin->setMinimumSize(QSize(0, 0));
        maxAABBTriCountSpin->setMaximumSize(QSize(110, 16777215));
        maxAABBTriCountSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        maxAABBTriCountSpin->setWrapping(false);
        maxAABBTriCountSpin->setFrame(false);
        maxAABBTriCountSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        maxAABBTriCountSpin->setProperty("showGroupSeparator", QVariant(false));
        maxAABBTriCountSpin->setMinimum(1);
        maxAABBTriCountSpin->setMaximum(50);
        maxAABBTriCountSpin->setValue(10);
        maxAABBTriCountSpin->setDisplayIntegerBase(10);

        formLayout->setWidget(9, QFormLayout::FieldRole, maxAABBTriCountSpin);

        rendNameLabel = new QLabel(scrollSettingsFrame);
        rendNameLabel->setObjectName("rendNameLabel");

        formLayout->setWidget(10, QFormLayout::LabelRole, rendNameLabel);

        rendNameEntry = new QLineEdit(scrollSettingsFrame);
        rendNameEntry->setObjectName("rendNameEntry");
        rendNameEntry->setMinimumSize(QSize(0, 0));
        rendNameEntry->setMaximumSize(QSize(110, 16777215));
        rendNameEntry->setStyleSheet(QString::fromUtf8(""));
        rendNameEntry->setFrame(false);

        formLayout->setWidget(10, QFormLayout::FieldRole, rendNameEntry);

        overrideBGColorBtn = new QPushButton(scrollSettingsFrame);
        overrideBGColorBtn->setObjectName("overrideBGColorBtn");
        overrideBGColorBtn->setMaximumSize(QSize(150, 16777215));
        overrideBGColorBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));
        overrideBGColorBtn->setCheckable(true);
        overrideBGColorBtn->setChecked(false);
        overrideBGColorBtn->setFlat(false);

        formLayout->setWidget(12, QFormLayout::LabelRole, overrideBGColorBtn);

        colorPickerBtn = new QToolButton(scrollSettingsFrame);
        colorPickerBtn->setObjectName("colorPickerBtn");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(colorPickerBtn->sizePolicy().hasHeightForWidth());
        colorPickerBtn->setSizePolicy(sizePolicy2);
        colorPickerBtn->setMinimumSize(QSize(0, 0));
        colorPickerBtn->setMaximumSize(QSize(110, 16777215));
        colorPickerBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        colorPickerBtn->setAutoFillBackground(false);
        colorPickerBtn->setStyleSheet(QString::fromUtf8("QLineEdit, QSpinBox, QDoubleSpinBox {\n"
"    border: 1px solid palette(mid);\n"
"    border-radius: 4px;\n"
"    padding: 1px 5px 1px 5px; /* top, right, bottom, left */\n"
"    background: palette(base);\n"
"    selection-background-color: palette(highlight);\n"
"    selection-color: palette(highlightedText);\n"
"}\n"
"\n"
"QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {\n"
"    border: 1px solid palette(highlight);\n"
"}"));
        colorPickerBtn->setCheckable(false);
        colorPickerBtn->setChecked(false);
        colorPickerBtn->setAutoRepeat(false);
        colorPickerBtn->setAutoExclusive(false);
        colorPickerBtn->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
        colorPickerBtn->setAutoRaise(false);
        colorPickerBtn->setArrowType(Qt::ArrowType::NoArrow);

        formLayout->setWidget(12, QFormLayout::FieldRole, colorPickerBtn);

        enableGIBtn = new QPushButton(scrollSettingsFrame);
        enableGIBtn->setObjectName("enableGIBtn");
        enableGIBtn->setEnabled(false);
        enableGIBtn->setCheckable(true);

        formLayout->setWidget(15, QFormLayout::LabelRole, enableGIBtn);

        giSamplesLabel = new QLabel(scrollSettingsFrame);
        giSamplesLabel->setObjectName("giSamplesLabel");
        giSamplesLabel->setEnabled(false);

        formLayout->setWidget(16, QFormLayout::LabelRole, giSamplesLabel);

        giSamplesSpin = new QSpinBox(scrollSettingsFrame);
        giSamplesSpin->setObjectName("giSamplesSpin");
        giSamplesSpin->setEnabled(false);
        giSamplesSpin->setMinimumSize(QSize(110, 0));
        giSamplesSpin->setMaximumSize(QSize(110, 16777215));
        giSamplesSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        giSamplesSpin->setStyleSheet(QString::fromUtf8(""));
        giSamplesSpin->setFrame(false);
        giSamplesSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        giSamplesSpin->setMinimum(1);
        giSamplesSpin->setMaximum(10000);
        giSamplesSpin->setValue(64);

        formLayout->setWidget(16, QFormLayout::FieldRole, giSamplesSpin);

        ignoreBackfaceBtn = new QPushButton(scrollSettingsFrame);
        ignoreBackfaceBtn->setObjectName("ignoreBackfaceBtn");
        ignoreBackfaceBtn->setMaximumSize(QSize(100, 16777215));
        ignoreBackfaceBtn->setAutoFillBackground(false);
        ignoreBackfaceBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));
        ignoreBackfaceBtn->setCheckable(true);
        ignoreBackfaceBtn->setChecked(true);
        ignoreBackfaceBtn->setFlat(false);

        formLayout->setWidget(17, QFormLayout::LabelRole, ignoreBackfaceBtn);

        cameraBox = new QGroupBox(scrollSettingsFrame);
        cameraBox->setObjectName("cameraBox");
        gridLayout_3 = new QGridLayout(cameraBox);
        gridLayout_3->setObjectName("gridLayout_3");
        camActionCombo = new QComboBox(cameraBox);
        camActionCombo->addItem(QString());
        camActionCombo->addItem(QString());
        camActionCombo->addItem(QString());
        camActionCombo->setObjectName("camActionCombo");
        camActionCombo->setMaximumSize(QSize(120, 16777215));
        camActionCombo->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-top: 1px;\n"
"padding-bottom: 1px;"));

        gridLayout_3->addWidget(camActionCombo, 0, 0, 1, 4);

        camInitialPosLabel = new QLabel(cameraBox);
        camInitialPosLabel->setObjectName("camInitialPosLabel");
        camInitialPosLabel->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        camInitialPosLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        gridLayout_3->addWidget(camInitialPosLabel, 1, 0, 1, 3);

        sceneInitialCamPosBtn = new QPushButton(cameraBox);
        sceneInitialCamPosBtn->setObjectName("sceneInitialCamPosBtn");
        sceneInitialCamPosBtn->setMaximumSize(QSize(100, 16777215));
        sceneInitialCamPosBtn->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        sceneInitialCamPosBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-top: 2px;\n"
"padding-right: 8px;\n"
"padding-bottom: 2px;"));
        sceneInitialCamPosBtn->setCheckable(true);
        sceneInitialCamPosBtn->setChecked(true);

        gridLayout_3->addWidget(sceneInitialCamPosBtn, 1, 3, 1, 3);

        xInitialLabel = new QLabel(cameraBox);
        xInitialLabel->setObjectName("xInitialLabel");

        gridLayout_3->addWidget(xInitialLabel, 2, 0, 1, 1);

        camXInitialSpin = new SmartDoubleSpinBox(cameraBox);
        camXInitialSpin->setObjectName("camXInitialSpin");
        camXInitialSpin->setEnabled(false);
        camXInitialSpin->setMaximumSize(QSize(64, 16777215));
        camXInitialSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        camXInitialSpin->setFrame(false);
        camXInitialSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camXInitialSpin->setDecimals(9);
        camXInitialSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camXInitialSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);
        camXInitialSpin->setValue(1.000000000000000);

        gridLayout_3->addWidget(camXInitialSpin, 2, 1, 1, 1);

        yInitialLabel = new QLabel(cameraBox);
        yInitialLabel->setObjectName("yInitialLabel");

        gridLayout_3->addWidget(yInitialLabel, 2, 2, 1, 1);

        camYInitialSpin = new SmartDoubleSpinBox(cameraBox);
        camYInitialSpin->setObjectName("camYInitialSpin");
        camYInitialSpin->setEnabled(false);
        camYInitialSpin->setMaximumSize(QSize(64, 16777215));
        camYInitialSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        camYInitialSpin->setFrame(false);
        camYInitialSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camYInitialSpin->setDecimals(9);
        camYInitialSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camYInitialSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);
        camYInitialSpin->setValue(1.000000000000000);

        gridLayout_3->addWidget(camYInitialSpin, 2, 3, 1, 1);

        zInitialLabel = new QLabel(cameraBox);
        zInitialLabel->setObjectName("zInitialLabel");

        gridLayout_3->addWidget(zInitialLabel, 2, 4, 1, 1);

        camZInitialSpin = new SmartDoubleSpinBox(cameraBox);
        camZInitialSpin->setObjectName("camZInitialSpin");
        camZInitialSpin->setEnabled(false);
        camZInitialSpin->setMaximumSize(QSize(64, 16777215));
        camZInitialSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        camZInitialSpin->setFrame(false);
        camZInitialSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camZInitialSpin->setDecimals(9);
        camZInitialSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camZInitialSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);
        camZInitialSpin->setValue(1.000000000000000);

        gridLayout_3->addWidget(camZInitialSpin, 2, 5, 1, 1);

        camActionLabel = new QLabel(cameraBox);
        camActionLabel->setObjectName("camActionLabel");
        camActionLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        gridLayout_3->addWidget(camActionLabel, 3, 0, 1, 6);

        xActionLabel = new QLabel(cameraBox);
        xActionLabel->setObjectName("xActionLabel");

        gridLayout_3->addWidget(xActionLabel, 4, 0, 1, 1);

        camXActionSpin = new SmartDoubleSpinBox(cameraBox);
        camXActionSpin->setObjectName("camXActionSpin");
        camXActionSpin->setMaximumSize(QSize(64, 16777215));
        camXActionSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        camXActionSpin->setFrame(false);
        camXActionSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camXActionSpin->setDecimals(9);
        camXActionSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camXActionSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);
        camXActionSpin->setValue(1.000000000000000);

        gridLayout_3->addWidget(camXActionSpin, 4, 1, 1, 1);

        yActionLabel = new QLabel(cameraBox);
        yActionLabel->setObjectName("yActionLabel");

        gridLayout_3->addWidget(yActionLabel, 4, 2, 1, 1);

        camYActionSpin = new SmartDoubleSpinBox(cameraBox);
        camYActionSpin->setObjectName("camYActionSpin");
        camYActionSpin->setMaximumSize(QSize(64, 16777215));
        camYActionSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        camYActionSpin->setFrame(false);
        camYActionSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camYActionSpin->setDecimals(9);
        camYActionSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camYActionSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);
        camYActionSpin->setValue(1.000000000000000);

        gridLayout_3->addWidget(camYActionSpin, 4, 3, 1, 1);

        zActionLabel = new QLabel(cameraBox);
        zActionLabel->setObjectName("zActionLabel");

        gridLayout_3->addWidget(zActionLabel, 4, 4, 1, 1);

        camZActionSpin = new SmartDoubleSpinBox(cameraBox);
        camZActionSpin->setObjectName("camZActionSpin");
        camZActionSpin->setMaximumSize(QSize(64, 16777215));
        camZActionSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        camZActionSpin->setFrame(false);
        camZActionSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camZActionSpin->setDecimals(9);
        camZActionSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camZActionSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);

        gridLayout_3->addWidget(camZActionSpin, 4, 5, 1, 1);

        frameNumLabel = new QLabel(cameraBox);
        frameNumLabel->setObjectName("frameNumLabel");

        gridLayout_3->addWidget(frameNumLabel, 5, 0, 1, 2);

        frameNumSpin = new QSpinBox(cameraBox);
        frameNumSpin->setObjectName("frameNumSpin");
        frameNumSpin->setMaximumSize(QSize(64, 16777215));
        frameNumSpin->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        frameNumSpin->setFrame(false);
        frameNumSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        frameNumSpin->setMinimum(1);
        frameNumSpin->setMaximum(999999999);
        frameNumSpin->setValue(30);

        gridLayout_3->addWidget(frameNumSpin, 5, 3, 1, 1);


        formLayout->setWidget(19, QFormLayout::SpanningRole, cameraBox);

        applyAntialiasBtn = new QPushButton(scrollSettingsFrame);
        applyAntialiasBtn->setObjectName("applyAntialiasBtn");
        applyAntialiasBtn->setEnabled(false);
        applyAntialiasBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));

        formLayout->setWidget(20, QFormLayout::LabelRole, applyAntialiasBtn);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        formLayout->setItem(21, QFormLayout::LabelRole, verticalSpacer);

        sRGBBtn = new QPushButton(scrollSettingsFrame);
        sRGBBtn->setObjectName("sRGBBtn");
        sRGBBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));
        sRGBBtn->setCheckable(true);
        sRGBBtn->setChecked(false);

        formLayout->setWidget(18, QFormLayout::LabelRole, sRGBBtn);

        custBucketSizeBtn = new QPushButton(scrollSettingsFrame);
        custBucketSizeBtn->setObjectName("custBucketSizeBtn");
        custBucketSizeBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));
        custBucketSizeBtn->setCheckable(true);

        formLayout->setWidget(7, QFormLayout::LabelRole, custBucketSizeBtn);

        custWidthBtn = new QPushButton(scrollSettingsFrame);
        custWidthBtn->setObjectName("custWidthBtn");
        custWidthBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));
        custWidthBtn->setCheckable(true);

        formLayout->setWidget(0, QFormLayout::LabelRole, custWidthBtn);

        custHeightBtn = new QPushButton(scrollSettingsFrame);
        custHeightBtn->setObjectName("custHeightBtn");
        custHeightBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));
        custHeightBtn->setCheckable(true);

        formLayout->setWidget(1, QFormLayout::LabelRole, custHeightBtn);

        antialiasLabel = new QLabel(scrollSettingsFrame);
        antialiasLabel->setObjectName("antialiasLabel");

        formLayout->setWidget(13, QFormLayout::LabelRole, antialiasLabel);

        antialiasCombo = new QComboBox(scrollSettingsFrame);
        antialiasCombo->addItem(QString());
        antialiasCombo->addItem(QString());
        antialiasCombo->addItem(QString());
        antialiasCombo->setObjectName("antialiasCombo");
        antialiasCombo->setMaximumSize(QSize(110, 16777215));
        antialiasCombo->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        antialiasCombo->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-top: 1px;\n"
"padding-bottom: 1px;"));

        formLayout->setWidget(13, QFormLayout::FieldRole, antialiasCombo);

        SSAASpin = new QSpinBox(scrollSettingsFrame);
        SSAASpin->setObjectName("SSAASpin");
        SSAASpin->setEnabled(false);
        SSAASpin->setMaximumSize(QSize(110, 16777215));
        SSAASpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        SSAASpin->setMinimum(1);
        SSAASpin->setMaximum(10);
        SSAASpin->setValue(1);
        SSAASpin->setDisplayIntegerBase(10);

        formLayout->setWidget(14, QFormLayout::FieldRole, SSAASpin);


        verticalLayout->addWidget(scrollSettingsFrame);

        settingsScrollArea->setWidget(scrollAreaContents);

        gridLayout_2->addWidget(settingsScrollArea, 2, 0, 1, 1);

        viewerLayout = new QGridLayout();
        viewerLayout->setObjectName("viewerLayout");
        viewerLayout->setContentsMargins(-1, -1, 10, -1);
        viewportLayout = new QVBoxLayout();
        viewportLayout->setObjectName("viewportLayout");
        qViewport = new ImageViewer(centralwidget);
        qViewport->setObjectName("qViewport");
        qViewport->setFrameShape(QFrame::Shape::Box);

        viewportLayout->addWidget(qViewport);


        viewerLayout->addLayout(viewportLayout, 0, 0, 1, 7);

        openImgBtn = new QPushButton(centralwidget);
        openImgBtn->setObjectName("openImgBtn");
        openImgBtn->setFocusPolicy(Qt::FocusPolicy::WheelFocus);
        openImgBtn->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        openImgBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));

        viewerLayout->addWidget(openImgBtn, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        viewerLayout->addItem(horizontalSpacer, 1, 1, 1, 1);

        prevImgBtn = new QPushButton(centralwidget);
        prevImgBtn->setObjectName("prevImgBtn");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::GoPrevious));
        prevImgBtn->setIcon(icon1);
        prevImgBtn->setFlat(true);

        viewerLayout->addWidget(prevImgBtn, 1, 2, 1, 1);

        nextImgBtn = new QPushButton(centralwidget);
        nextImgBtn->setObjectName("nextImgBtn");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::GoNext));
        nextImgBtn->setIcon(icon2);
        nextImgBtn->setFlat(true);

        viewerLayout->addWidget(nextImgBtn, 1, 3, 1, 1);

        zoomResetBtn = new QPushButton(centralwidget);
        zoomResetBtn->setObjectName("zoomResetBtn");
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::ViewFullscreen));
        zoomResetBtn->setIcon(icon3);
        zoomResetBtn->setFlat(true);

        viewerLayout->addWidget(zoomResetBtn, 1, 4, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        viewerLayout->addItem(horizontalSpacer_2, 1, 5, 1, 1);

        renderBtn = new QPushButton(centralwidget);
        renderBtn->setObjectName("renderBtn");
        renderBtn->setMinimumSize(QSize(75, 0));
        renderBtn->setStyleSheet(QString::fromUtf8("padding: 2px 8px 2px 8px;"));

        viewerLayout->addWidget(renderBtn, 1, 6, 1, 1);

        viewerLayout->setRowStretch(0, 1);

        gridLayout_2->addLayout(viewerLayout, 2, 1, 2, 5);

        gridLayout_2->setColumnStretch(1, 1);
        RayTracer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RayTracer);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 21));
        menuMenu = new QMenu(menubar);
        menuMenu->setObjectName("menuMenu");
        RayTracer->setMenuBar(menubar);
        statusbar = new QStatusBar(RayTracer);
        statusbar->setObjectName("statusbar");
        RayTracer->setStatusBar(statusbar);

        menubar->addAction(menuMenu->menuAction());
        menuMenu->addAction(actionRender);
        menuMenu->addSeparator();
        menuMenu->addAction(actionExit);

        retranslateUi(RayTracer);

        renderModeCombo->setCurrentIndex(0);
        overrideBGColorBtn->setDefault(false);
        camActionCombo->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(RayTracer);
    } // setupUi

    void retranslateUi(QMainWindow *RayTracer)
    {
        RayTracer->setWindowTitle(QCoreApplication::translate("RayTracer", "RayTracer", nullptr));
        actionRender->setText(QCoreApplication::translate("RayTracer", "Render", nullptr));
        actionExit->setText(QCoreApplication::translate("RayTracer", "Exit", nullptr));
#if QT_CONFIG(tooltip)
        sceneFileBtn->setToolTip(QCoreApplication::translate("RayTracer", "Choose a scene file to be processed and rendered.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        sceneFileBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Choose a scene file to be processed and rendered.", nullptr));
#endif // QT_CONFIG(statustip)
        sceneFileBtn->setText(QCoreApplication::translate("RayTracer", "Scene File...", nullptr));
#if QT_CONFIG(tooltip)
        sceneFileEntry->setToolTip(QCoreApplication::translate("RayTracer", "The scene file to be processed and rendered.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        sceneFileEntry->setStatusTip(QCoreApplication::translate("RayTracer", "The scene file to be processed and rendered.", nullptr));
#endif // QT_CONFIG(statustip)
        sceneFileEntry->setText(QString());
        sceneFileEntry->setPlaceholderText(QCoreApplication::translate("RayTracer", "<Scene File>", nullptr));
#if QT_CONFIG(tooltip)
        saveDirBtn->setToolTip(QCoreApplication::translate("RayTracer", "Choose a directory to output the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        saveDirBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Choose a directory to output the rendered image.", nullptr));
#endif // QT_CONFIG(statustip)
        saveDirBtn->setText(QCoreApplication::translate("RayTracer", "Render To...", nullptr));
#if QT_CONFIG(tooltip)
        saveDirEntry->setToolTip(QCoreApplication::translate("RayTracer", "The directory to output the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        saveDirEntry->setStatusTip(QCoreApplication::translate("RayTracer", "The directory to output the rendered image.", nullptr));
#endif // QT_CONFIG(statustip)
        saveDirEntry->setText(QCoreApplication::translate("RayTracer", "./renders", nullptr));
        saveDirEntry->setPlaceholderText(QString());
#if QT_CONFIG(tooltip)
        resWidthSpin->setToolTip(QCoreApplication::translate("RayTracer", "The width of the resulting image in pixels.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        resWidthSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The width of the resulting image in pixels.", nullptr));
#endif // QT_CONFIG(statustip)
        resWidthSpin->setSuffix(QCoreApplication::translate("RayTracer", "px", nullptr));
        resWidthSpin->setPrefix(QString());
#if QT_CONFIG(tooltip)
        resHeightSpin->setToolTip(QCoreApplication::translate("RayTracer", "The height of the resulting image in pixels.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        resHeightSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The height of the resulting image in pixels.", nullptr));
#endif // QT_CONFIG(statustip)
        resHeightSpin->setSuffix(QCoreApplication::translate("RayTracer", "px", nullptr));
#if QT_CONFIG(tooltip)
        colorDepthLabel->setToolTip(QCoreApplication::translate("RayTracer", "Color precision. The higher the color depth - the more color nuances will be produced. Defaults to 8-bit color, which makes each color component have a maximum value of 255, for a total of 16,777,216 colors.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        colorDepthLabel->setStatusTip(QCoreApplication::translate("RayTracer", "Color precision. The higher the color depth - the more color nuances will be produced. Defaults to 8-bit color, which makes each color component have a maximum value of 255, for a total of 16,777,216 colors.", nullptr));
#endif // QT_CONFIG(statustip)
        colorDepthLabel->setText(QCoreApplication::translate("RayTracer", "Color Depth", nullptr));
#if QT_CONFIG(tooltip)
        colorDepthSpin->setToolTip(QCoreApplication::translate("RayTracer", "Color precision. The higher the color depth - the more color nuances will be produced. Defaults to 8-bit color, which makes each color component have a maximum value of 255, for a total of 16,777,216 colors. Currently not set up.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        colorDepthSpin->setStatusTip(QCoreApplication::translate("RayTracer", "Color precision. The higher the color depth - the more color nuances will be produced. Defaults to 8-bit color, which makes each color component have a maximum value of 255, for a total of 16,777,216 colors. Currently not set up.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        shadowBiasLabel->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for shadow rays. Use for correcting shadow artifacts when rendering diffuse objects.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        shadowBiasLabel->setStatusTip(QCoreApplication::translate("RayTracer", "Precision fix for shadow rays. Use for correcting shadow artifacts when rendering diffuse objects.", nullptr));
#endif // QT_CONFIG(statustip)
        shadowBiasLabel->setText(QCoreApplication::translate("RayTracer", "Shadow Bias", nullptr));
#if QT_CONFIG(tooltip)
        shadowBiasSpin->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for shadow rays. Use for correcting shadow artifacts when rendering diffuse objects.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        shadowBiasSpin->setStatusTip(QCoreApplication::translate("RayTracer", "Precision fix for shadow rays. Use for correcting shadow artifacts when rendering diffuse objects.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        refractBiasLabel->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for refraction rays. Use for correcting refraction artifacts when rendering semi-transparent objects.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        refractBiasLabel->setStatusTip(QCoreApplication::translate("RayTracer", "Precision fix for refraction rays. Use for correcting refraction artifacts when rendering semi-transparent objects.", nullptr));
#endif // QT_CONFIG(statustip)
        refractBiasLabel->setText(QCoreApplication::translate("RayTracer", "Refraction Bias", nullptr));
#if QT_CONFIG(tooltip)
        refractBiasSpin->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for refraction rays. Use for correcting refraction artifacts when rendering semi-transparent objects.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        refractBiasSpin->setStatusTip(QCoreApplication::translate("RayTracer", "Precision fix for refraction rays. Use for correcting refraction artifacts when rendering semi-transparent objects.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        pathDepthLabel->setToolTip(QCoreApplication::translate("RayTracer", "The number of times a ray can be reflected or refracted before it stops calculating.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pathDepthLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The number of times a ray can be reflected or refracted before it stops calculating.", nullptr));
#endif // QT_CONFIG(statustip)
        pathDepthLabel->setText(QCoreApplication::translate("RayTracer", "Path Depth", nullptr));
#if QT_CONFIG(tooltip)
        pathDepthSpin->setToolTip(QCoreApplication::translate("RayTracer", "The number of times a ray can be reflected or refracted before it stops calculating.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        pathDepthSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The number of times a ray can be reflected or refracted before it stops calculating.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        renderModeLabel->setToolTip(QCoreApplication::translate("RayTracer", "The number of times a ray can be reflected or refracted before it stops calculating.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        renderModeLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The number of times a ray can be reflected or refracted before it stops calculating.", nullptr));
#endif // QT_CONFIG(statustip)
        renderModeLabel->setText(QCoreApplication::translate("RayTracer", "Render Mode", nullptr));
        renderModeCombo->setItemText(0, QCoreApplication::translate("RayTracer", "Material", nullptr));
        renderModeCombo->setItemText(1, QCoreApplication::translate("RayTracer", "Random Mesh Color", nullptr));
        renderModeCombo->setItemText(2, QCoreApplication::translate("RayTracer", "Random Triangle Color", nullptr));
        renderModeCombo->setItemText(3, QCoreApplication::translate("RayTracer", "Barycentric Coordinates", nullptr));
        renderModeCombo->setItemText(4, QCoreApplication::translate("RayTracer", "Normals", nullptr));
        renderModeCombo->setItemText(5, QCoreApplication::translate("RayTracer", "Shaded Normals", nullptr));

#if QT_CONFIG(tooltip)
        renderModeCombo->setToolTip(QCoreApplication::translate("RayTracer", "Choose how the rendered image would look like.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        renderModeCombo->setStatusTip(QCoreApplication::translate("RayTracer", "Choose how the rendered image would look like.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        bucketSizeSpin->setToolTip(QCoreApplication::translate("RayTracer", "The size of each render bucket. Defaults to 24x24 pixels.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        bucketSizeSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The size of each render bucket. Defaults to 24x24 pixels.", nullptr));
#endif // QT_CONFIG(statustip)
        bucketSizeSpin->setSuffix(QCoreApplication::translate("RayTracer", "px", nullptr));
#if QT_CONFIG(tooltip)
        accTreeMaxDepthLabel->setToolTip(QCoreApplication::translate("RayTracer", "The maximum depth for building an acceleration tree (KD or BVH). Defaults to 15.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        accTreeMaxDepthLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The maximum depth for building an acceleration tree (KD or BVH). Defaults to 15.", nullptr));
#endif // QT_CONFIG(statustip)
        accTreeMaxDepthLabel->setText(QCoreApplication::translate("RayTracer", "Build Max Depth", nullptr));
#if QT_CONFIG(tooltip)
        accTreeMaxDepthSpin->setToolTip(QCoreApplication::translate("RayTracer", "The maximum depth for building an acceleration tree (KD or BVH). Defaults to 15.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        accTreeMaxDepthSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The maximum depth for building an acceleration tree (KD or BVH). Defaults to 15.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        maxAABBTriCountLabel->setToolTip(QCoreApplication::translate("RayTracer", "The maximum number of triangles a single node of the acceleration tree can hold. Defaults to 10.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        maxAABBTriCountLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The maximum number of triangles a single node of the acceleration tree can hold. Defaults to 10.", nullptr));
#endif // QT_CONFIG(statustip)
        maxAABBTriCountLabel->setText(QCoreApplication::translate("RayTracer", "Build Max Primitives", nullptr));
#if QT_CONFIG(tooltip)
        maxAABBTriCountSpin->setToolTip(QCoreApplication::translate("RayTracer", "The maximum number of triangles a single node of the acceleration tree can hold. Defaults to 10.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        maxAABBTriCountSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The maximum number of triangles a single node of the acceleration tree can hold. Defaults to 10.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        rendNameLabel->setToolTip(QCoreApplication::translate("RayTracer", "The name of the rendered image. If not set, it will use the name of the scene file.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        rendNameLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The name of the rendered image. If not set, it will use the name of the scene file.", nullptr));
#endif // QT_CONFIG(statustip)
        rendNameLabel->setText(QCoreApplication::translate("RayTracer", "Render Name", nullptr));
#if QT_CONFIG(tooltip)
        rendNameEntry->setToolTip(QCoreApplication::translate("RayTracer", "The name of the rendered image. If not set, it will use the name of the scene file.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        rendNameEntry->setStatusTip(QCoreApplication::translate("RayTracer", "The name of the rendered image. If not set, it will use the name of the scene file.", nullptr));
#endif // QT_CONFIG(statustip)
        rendNameEntry->setText(QString());
        rendNameEntry->setPlaceholderText(QCoreApplication::translate("RayTracer", "<Default>", nullptr));
#if QT_CONFIG(tooltip)
        overrideBGColorBtn->setToolTip(QCoreApplication::translate("RayTracer", "Use custom color for the background instead of the one specified in the scene file (if at all).", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        overrideBGColorBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Use custom color for the background instead of the one specified in the scene file (if at all).", nullptr));
#endif // QT_CONFIG(statustip)
        overrideBGColorBtn->setText(QCoreApplication::translate("RayTracer", "Set BG Color", nullptr));
#if QT_CONFIG(tooltip)
        colorPickerBtn->setToolTip(QCoreApplication::translate("RayTracer", "Click to select a color", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        colorPickerBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Click to select a color", nullptr));
#endif // QT_CONFIG(statustip)
        colorPickerBtn->setText(QString());
#if QT_CONFIG(tooltip)
        enableGIBtn->setToolTip(QCoreApplication::translate("RayTracer", "Render with Global Illumination.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        enableGIBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Render with Global Illumination.", nullptr));
#endif // QT_CONFIG(statustip)
        enableGIBtn->setText(QCoreApplication::translate("RayTracer", "Enable GI", nullptr));
#if QT_CONFIG(tooltip)
        giSamplesLabel->setToolTip(QCoreApplication::translate("RayTracer", "The number of global illumination samples. Higher numbers result in slower rendering.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        giSamplesLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The number of global illumination samples. Higher numbers result in slower rendering.", nullptr));
#endif // QT_CONFIG(statustip)
        giSamplesLabel->setText(QCoreApplication::translate("RayTracer", "GI samples", nullptr));
#if QT_CONFIG(tooltip)
        giSamplesSpin->setToolTip(QCoreApplication::translate("RayTracer", "The number of global illumination samples. Higher numbers result in slower rendering.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        giSamplesSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The number of global illumination samples. Higher numbers result in slower rendering.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        ignoreBackfaceBtn->setToolTip(QCoreApplication::translate("RayTracer", "Should the back faces of the triangles be rendered or ignored.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        ignoreBackfaceBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Should the back faces of the triangles be rendered or ignored.", nullptr));
#endif // QT_CONFIG(statustip)
        ignoreBackfaceBtn->setText(QCoreApplication::translate("RayTracer", "Ignore Backface", nullptr));
#if QT_CONFIG(tooltip)
        cameraBox->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        cameraBox->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        cameraBox->setTitle(QCoreApplication::translate("RayTracer", "Camera Settings", nullptr));
        camActionCombo->setItemText(0, QCoreApplication::translate("RayTracer", "Static Image", nullptr));
        camActionCombo->setItemText(1, QCoreApplication::translate("RayTracer", "Move Animation", nullptr));
        camActionCombo->setItemText(2, QCoreApplication::translate("RayTracer", "Orbit Animation", nullptr));

#if QT_CONFIG(tooltip)
        camInitialPosLabel->setToolTip(QCoreApplication::translate("RayTracer", "The initial position of the camera.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        camInitialPosLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The initial position of the camera.", nullptr));
#endif // QT_CONFIG(statustip)
        camInitialPosLabel->setText(QCoreApplication::translate("RayTracer", "Initial Position", nullptr));
#if QT_CONFIG(tooltip)
        sceneInitialCamPosBtn->setToolTip(QCoreApplication::translate("RayTracer", "Wheter to use the camera position in the scene file or the one you specify below.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        sceneInitialCamPosBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Wheter to use the camera position in the scene file or the one you specify below.", nullptr));
#endif // QT_CONFIG(statustip)
        sceneInitialCamPosBtn->setText(QCoreApplication::translate("RayTracer", "Scene Position", nullptr));
#if QT_CONFIG(tooltip)
        xInitialLabel->setToolTip(QCoreApplication::translate("RayTracer", "The camera position on the X axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        xInitialLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The camera position on the X axis.", nullptr));
#endif // QT_CONFIG(statustip)
        xInitialLabel->setText(QCoreApplication::translate("RayTracer", "X:", nullptr));
#if QT_CONFIG(tooltip)
        camXInitialSpin->setToolTip(QCoreApplication::translate("RayTracer", "The camera position on the X axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        camXInitialSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The camera position on the X axis.", nullptr));
#endif // QT_CONFIG(statustip)
        camXInitialSpin->setSuffix(QCoreApplication::translate("RayTracer", "m", nullptr));
#if QT_CONFIG(tooltip)
        yInitialLabel->setToolTip(QCoreApplication::translate("RayTracer", "The camera position on the Y axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        yInitialLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The camera position on the Y axis.", nullptr));
#endif // QT_CONFIG(statustip)
        yInitialLabel->setText(QCoreApplication::translate("RayTracer", "Y:", nullptr));
#if QT_CONFIG(tooltip)
        camYInitialSpin->setToolTip(QCoreApplication::translate("RayTracer", "The camera position on the Y axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        camYInitialSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The camera position on the Y axis.", nullptr));
#endif // QT_CONFIG(statustip)
        camYInitialSpin->setSuffix(QCoreApplication::translate("RayTracer", "m", nullptr));
#if QT_CONFIG(tooltip)
        zInitialLabel->setToolTip(QCoreApplication::translate("RayTracer", "The camera position on the Z axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        zInitialLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The camera position on the Z axis.", nullptr));
#endif // QT_CONFIG(statustip)
        zInitialLabel->setText(QCoreApplication::translate("RayTracer", "Z:", nullptr));
#if QT_CONFIG(tooltip)
        camZInitialSpin->setToolTip(QCoreApplication::translate("RayTracer", "The camera position on the Z axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        camZInitialSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The camera position on the Z axis.", nullptr));
#endif // QT_CONFIG(statustip)
        camZInitialSpin->setSuffix(QCoreApplication::translate("RayTracer", "m", nullptr));
#if QT_CONFIG(tooltip)
        camActionLabel->setToolTip(QCoreApplication::translate("RayTracer", "The distance the camera should move on each frame and on which ases.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        camActionLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The distance the camera should move on each frame and on which ases.", nullptr));
#endif // QT_CONFIG(statustip)
        camActionLabel->setText(QCoreApplication::translate("RayTracer", "Move Offset", nullptr));
#if QT_CONFIG(tooltip)
        xActionLabel->setToolTip(QCoreApplication::translate("RayTracer", "The camera offset on the X axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        xActionLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The camera offset on the X axis.", nullptr));
#endif // QT_CONFIG(statustip)
        xActionLabel->setText(QCoreApplication::translate("RayTracer", "X:", nullptr));
#if QT_CONFIG(tooltip)
        camXActionSpin->setToolTip(QCoreApplication::translate("RayTracer", "The camera offset on the X axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        camXActionSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The camera offset on the X axis.", nullptr));
#endif // QT_CONFIG(statustip)
        camXActionSpin->setSuffix(QCoreApplication::translate("RayTracer", "\302\260", nullptr));
#if QT_CONFIG(tooltip)
        yActionLabel->setToolTip(QCoreApplication::translate("RayTracer", "The camera offset on the Y axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        yActionLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The camera offset on the Y axis.", nullptr));
#endif // QT_CONFIG(statustip)
        yActionLabel->setText(QCoreApplication::translate("RayTracer", "Y:", nullptr));
#if QT_CONFIG(tooltip)
        camYActionSpin->setToolTip(QCoreApplication::translate("RayTracer", "The camera offset on the Y axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        camYActionSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The camera offset on the Y axis.", nullptr));
#endif // QT_CONFIG(statustip)
        camYActionSpin->setSuffix(QCoreApplication::translate("RayTracer", "\302\260", nullptr));
#if QT_CONFIG(tooltip)
        zActionLabel->setToolTip(QCoreApplication::translate("RayTracer", "The camera offset on the Z axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        zActionLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The camera offset on the Z axis.", nullptr));
#endif // QT_CONFIG(statustip)
        zActionLabel->setText(QCoreApplication::translate("RayTracer", "Z:", nullptr));
#if QT_CONFIG(tooltip)
        camZActionSpin->setToolTip(QCoreApplication::translate("RayTracer", "The camera offset on the Z axis.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        camZActionSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The camera offset on the Z axis.", nullptr));
#endif // QT_CONFIG(statustip)
        camZActionSpin->setSuffix(QCoreApplication::translate("RayTracer", "\302\260", nullptr));
#if QT_CONFIG(tooltip)
        frameNumLabel->setToolTip(QCoreApplication::translate("RayTracer", "The number of frames to render.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        frameNumLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The number of frames to render.", nullptr));
#endif // QT_CONFIG(statustip)
        frameNumLabel->setText(QCoreApplication::translate("RayTracer", "Frames:", nullptr));
#if QT_CONFIG(tooltip)
        frameNumSpin->setToolTip(QCoreApplication::translate("RayTracer", "The number of frames to render.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        frameNumSpin->setStatusTip(QCoreApplication::translate("RayTracer", "The number of frames to render.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        applyAntialiasBtn->setToolTip(QCoreApplication::translate("RayTracer", "Apply antialiasing to the opened image.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        applyAntialiasBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Apply antialiasing to the opened image.", nullptr));
#endif // QT_CONFIG(statustip)
        applyAntialiasBtn->setText(QCoreApplication::translate("RayTracer", "Apply Antialiasing", nullptr));
#if QT_CONFIG(tooltip)
        sRGBBtn->setToolTip(QCoreApplication::translate("RayTracer", "Whether to save the rendered image in sRGB instead of linear space.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        sRGBBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Whether to save the rendered image in sRGB instead of linear space.", nullptr));
#endif // QT_CONFIG(statustip)
        sRGBBtn->setText(QCoreApplication::translate("RayTracer", "Output sRGB", nullptr));
#if QT_CONFIG(tooltip)
        custBucketSizeBtn->setToolTip(QCoreApplication::translate("RayTracer", "Whether to use a custom value for the bucket size. Defaults to the value specified in the scene file or 24 if it's not set.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        custBucketSizeBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Whether to use a custom value for the bucket size. Defaults to the value specified in the scene file or 24 if it's not set.", nullptr));
#endif // QT_CONFIG(statustip)
        custBucketSizeBtn->setText(QCoreApplication::translate("RayTracer", "Set Bucket Size", nullptr));
#if QT_CONFIG(tooltip)
        custWidthBtn->setToolTip(QCoreApplication::translate("RayTracer", "Whether to use a custom value for the width of the rendered image. Defaults to the value specified in the scene file.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        custWidthBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Whether to use a custom value for the width of the rendered image. Defaults to the value specified in the scene file.", nullptr));
#endif // QT_CONFIG(statustip)
        custWidthBtn->setText(QCoreApplication::translate("RayTracer", "Set Width", nullptr));
#if QT_CONFIG(tooltip)
        custHeightBtn->setToolTip(QCoreApplication::translate("RayTracer", "Whether to use a custom value for the height of the rendered image. Defaults to the value specified in the scene file.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        custHeightBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Whether to use a custom value for the height of the rendered image. Defaults to the value specified in the scene file.", nullptr));
#endif // QT_CONFIG(statustip)
        custHeightBtn->setText(QCoreApplication::translate("RayTracer", "Set Height", nullptr));
#if QT_CONFIG(tooltip)
        antialiasLabel->setToolTip(QCoreApplication::translate("RayTracer", "The antialiasing mode for the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        antialiasLabel->setStatusTip(QCoreApplication::translate("RayTracer", "The antialiasing mode for the rendered image.", nullptr));
#endif // QT_CONFIG(statustip)
        antialiasLabel->setText(QCoreApplication::translate("RayTracer", "Antialiasing", nullptr));
        antialiasCombo->setItemText(0, QCoreApplication::translate("RayTracer", "No Antialiasing", nullptr));
        antialiasCombo->setItemText(1, QCoreApplication::translate("RayTracer", "FXAA", nullptr));
        antialiasCombo->setItemText(2, QCoreApplication::translate("RayTracer", "SSAA", nullptr));

#if QT_CONFIG(tooltip)
        antialiasCombo->setToolTip(QCoreApplication::translate("RayTracer", "The antialiasing mode for the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        antialiasCombo->setStatusTip(QCoreApplication::translate("RayTracer", "The antialiasing mode for the rendered image.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        SSAASpin->setToolTip(QCoreApplication::translate("RayTracer", "Supersampling Anti-Aliasing (SSAA) - each level subdivides further and that many rays are generated. Then all values are combined for the final value. Level one subdivides the pixel into 4 sub-pixels, level 2 - 9 sub-pixels, etc.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        SSAASpin->setStatusTip(QCoreApplication::translate("RayTracer", "Supersampling Anti-Aliasing (SSAA) - each level subdivides further and that many rays are generated. Then all values are combined for the final value. Level one subdivides the pixel into 4 sub-pixels, level 2 - 9 sub-pixels, etc.", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        openImgBtn->setToolTip(QCoreApplication::translate("RayTracer", "Browse to an image to open in the viewer.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        openImgBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Browse to an image to open in the viewer.", nullptr));
#endif // QT_CONFIG(statustip)
        openImgBtn->setText(QCoreApplication::translate("RayTracer", "Open Image", nullptr));
#if QT_CONFIG(tooltip)
        prevImgBtn->setToolTip(QCoreApplication::translate("RayTracer", "Previous image. Shortcut \",\"", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        prevImgBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Previous image. Shortcut \",\"", nullptr));
#endif // QT_CONFIG(statustip)
        prevImgBtn->setText(QString());
#if QT_CONFIG(tooltip)
        nextImgBtn->setToolTip(QCoreApplication::translate("RayTracer", "Next image. Shortcut \".\"", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        nextImgBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Next image. Shortcut \".\"", nullptr));
#endif // QT_CONFIG(statustip)
        nextImgBtn->setText(QString());
#if QT_CONFIG(tooltip)
        zoomResetBtn->setToolTip(QCoreApplication::translate("RayTracer", "Fit image to viewer size (double click)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        zoomResetBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Fit image to viewer size. Shortcut double click", nullptr));
#endif // QT_CONFIG(statustip)
        zoomResetBtn->setText(QString());
#if QT_CONFIG(tooltip)
        renderBtn->setToolTip(QCoreApplication::translate("RayTracer", "Render using the chosen settings.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        renderBtn->setStatusTip(QCoreApplication::translate("RayTracer", "Render using the chosen settings.", nullptr));
#endif // QT_CONFIG(statustip)
        renderBtn->setText(QCoreApplication::translate("RayTracer", "Reneder", nullptr));
        menuMenu->setTitle(QCoreApplication::translate("RayTracer", "Menu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RayTracer: public Ui_RayTracer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_H
