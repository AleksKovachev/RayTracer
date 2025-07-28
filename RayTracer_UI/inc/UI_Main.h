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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    QGridLayout *viewerLayout;
    QPushButton *prevImgBtn;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *nextImgBtn;
    QPushButton *zoomResetBtn;
    QSpacerItem *horizontalSpacer;
    QPushButton *openImgBtn;
    QVBoxLayout *viewportLayout;
    ImageViewer *qViewport;
    QPushButton *renderBtn;
    QVBoxLayout *camPostLayout;
    QGroupBox *cameraBox;
    QGridLayout *gridLayout_3;
    QSpinBox *frameNumSpin;
    QLabel *xActionLabel;
    SmartDoubleSpinBox *camZActionSpin;
    QLabel *camActionLabel;
    QLabel *frameNumLabel;
    QLabel *xInitialLabel;
    SmartDoubleSpinBox *camYInitialSpin;
    QLabel *zActionLabel;
    QLabel *yInitialLabel;
    SmartDoubleSpinBox *camXActionSpin;
    QComboBox *camActionCombo;
    SmartDoubleSpinBox *camXInitialSpin;
    SmartDoubleSpinBox *camYActionSpin;
    QLabel *yActionLabel;
    QLabel *zInitialLabel;
    SmartDoubleSpinBox *camZInitialSpin;
    QLabel *camInitialPosLabel;
    QPushButton *sceneInitialCamPosBtn;
    QPushButton *applyAntialiasBtn;
    QSpacerItem *verticalSpacer;
    QFormLayout *mainSettingsLayout;
    QLabel *resWidthLabel;
    QSpinBox *resWidthSpin;
    QLabel *resHeightLabel;
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
    QLabel *bucketSizeLabel;
    QSpinBox *bucketSizeSpin;
    QLabel *accTreeMaxDepthLabel;
    QSpinBox *accTreeMaxDepthSpin;
    QLabel *maxAABBTriCountLabel;
    QSpinBox *maxAABBTriCountSpin;
    QLabel *rendNameLabel;
    QLineEdit *rendNameEntry;
    QPushButton *overrideBGColorBtn;
    QToolButton *colorPickerBtn;
    QPushButton *ignoreBackfaceBtn;
    QPushButton *enableGIBtn;
    QLabel *giSamplesLabel;
    QSpinBox *giSamplesSpin;
    QHBoxLayout *saveDirLayout;
    QPushButton *saveDirBtn;
    QLineEdit *saveDirEntry;
    QHBoxLayout *sceneFileLayout;
    QPushButton *sceneFileBtn;
    QLineEdit *sceneFileEntry;
    QMenuBar *menubar;
    QMenu *menuMenu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RayTracer)
    {
        if (RayTracer->objectName().isEmpty())
            RayTracer->setObjectName("RayTracer");
        RayTracer->resize(1350, 760);
        RayTracer->setMinimumSize(QSize(1350, 760));
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
        viewerLayout = new QGridLayout();
        viewerLayout->setObjectName("viewerLayout");
        viewerLayout->setContentsMargins(-1, -1, 10, -1);
        prevImgBtn = new QPushButton(centralwidget);
        prevImgBtn->setObjectName("prevImgBtn");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::GoPrevious));
        prevImgBtn->setIcon(icon1);
        prevImgBtn->setFlat(true);

        viewerLayout->addWidget(prevImgBtn, 3, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        viewerLayout->addItem(horizontalSpacer_2, 3, 5, 1, 1);

        nextImgBtn = new QPushButton(centralwidget);
        nextImgBtn->setObjectName("nextImgBtn");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::GoNext));
        nextImgBtn->setIcon(icon2);
        nextImgBtn->setFlat(true);

        viewerLayout->addWidget(nextImgBtn, 3, 3, 1, 1);

        zoomResetBtn = new QPushButton(centralwidget);
        zoomResetBtn->setObjectName("zoomResetBtn");
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::ViewFullscreen));
        zoomResetBtn->setIcon(icon3);
        zoomResetBtn->setFlat(true);

        viewerLayout->addWidget(zoomResetBtn, 3, 4, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        viewerLayout->addItem(horizontalSpacer, 3, 1, 1, 1);

        openImgBtn = new QPushButton(centralwidget);
        openImgBtn->setObjectName("openImgBtn");
        openImgBtn->setFocusPolicy(Qt::FocusPolicy::WheelFocus);
        openImgBtn->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        openImgBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));

        viewerLayout->addWidget(openImgBtn, 3, 0, 1, 1);

        viewportLayout = new QVBoxLayout();
        viewportLayout->setObjectName("viewportLayout");
        qViewport = new ImageViewer(centralwidget);
        qViewport->setObjectName("qViewport");
        qViewport->setFrameShape(QFrame::Shape::Box);

        viewportLayout->addWidget(qViewport);


        viewerLayout->addLayout(viewportLayout, 0, 0, 1, 7);

        renderBtn = new QPushButton(centralwidget);
        renderBtn->setObjectName("renderBtn");
        renderBtn->setMinimumSize(QSize(75, 0));
        renderBtn->setStyleSheet(QString::fromUtf8("padding: 2px 8px 2px 8px;"));

        viewerLayout->addWidget(renderBtn, 3, 6, 1, 1);

        viewerLayout->setRowStretch(0, 1);

        gridLayout_2->addLayout(viewerLayout, 2, 1, 6, 5);

        camPostLayout = new QVBoxLayout();
        camPostLayout->setObjectName("camPostLayout");
        cameraBox = new QGroupBox(centralwidget);
        cameraBox->setObjectName("cameraBox");
        gridLayout_3 = new QGridLayout(cameraBox);
        gridLayout_3->setObjectName("gridLayout_3");
        frameNumSpin = new QSpinBox(cameraBox);
        frameNumSpin->setObjectName("frameNumSpin");
        frameNumSpin->setMaximumSize(QSize(64, 16777215));
        frameNumSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        frameNumSpin->setMinimum(1);
        frameNumSpin->setMaximum(999999999);
        frameNumSpin->setValue(30);

        gridLayout_3->addWidget(frameNumSpin, 7, 3, 1, 1);

        xActionLabel = new QLabel(cameraBox);
        xActionLabel->setObjectName("xActionLabel");

        gridLayout_3->addWidget(xActionLabel, 6, 0, 1, 1);

        camZActionSpin = new SmartDoubleSpinBox(cameraBox);
        camZActionSpin->setObjectName("camZActionSpin");
        camZActionSpin->setMaximumSize(QSize(64, 16777215));
        camZActionSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camZActionSpin->setDecimals(9);
        camZActionSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camZActionSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);

        gridLayout_3->addWidget(camZActionSpin, 6, 5, 1, 1);

        camActionLabel = new QLabel(cameraBox);
        camActionLabel->setObjectName("camActionLabel");
        camActionLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        gridLayout_3->addWidget(camActionLabel, 5, 0, 1, 6);

        frameNumLabel = new QLabel(cameraBox);
        frameNumLabel->setObjectName("frameNumLabel");

        gridLayout_3->addWidget(frameNumLabel, 7, 0, 1, 3);

        xInitialLabel = new QLabel(cameraBox);
        xInitialLabel->setObjectName("xInitialLabel");

        gridLayout_3->addWidget(xInitialLabel, 4, 0, 1, 1);

        camYInitialSpin = new SmartDoubleSpinBox(cameraBox);
        camYInitialSpin->setObjectName("camYInitialSpin");
        camYInitialSpin->setEnabled(false);
        camYInitialSpin->setMaximumSize(QSize(64, 16777215));
        camYInitialSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camYInitialSpin->setDecimals(9);
        camYInitialSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camYInitialSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);
        camYInitialSpin->setValue(1.000000000000000);

        gridLayout_3->addWidget(camYInitialSpin, 4, 3, 1, 1);

        zActionLabel = new QLabel(cameraBox);
        zActionLabel->setObjectName("zActionLabel");

        gridLayout_3->addWidget(zActionLabel, 6, 4, 1, 1);

        yInitialLabel = new QLabel(cameraBox);
        yInitialLabel->setObjectName("yInitialLabel");

        gridLayout_3->addWidget(yInitialLabel, 4, 2, 1, 1);

        camXActionSpin = new SmartDoubleSpinBox(cameraBox);
        camXActionSpin->setObjectName("camXActionSpin");
        camXActionSpin->setMaximumSize(QSize(64, 16777215));
        camXActionSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camXActionSpin->setDecimals(9);
        camXActionSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camXActionSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);
        camXActionSpin->setValue(1.000000000000000);

        gridLayout_3->addWidget(camXActionSpin, 6, 1, 1, 1);

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

        camXInitialSpin = new SmartDoubleSpinBox(cameraBox);
        camXInitialSpin->setObjectName("camXInitialSpin");
        camXInitialSpin->setEnabled(false);
        camXInitialSpin->setMaximumSize(QSize(64, 16777215));
        camXInitialSpin->setFrame(true);
        camXInitialSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camXInitialSpin->setDecimals(9);
        camXInitialSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camXInitialSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);
        camXInitialSpin->setValue(1.000000000000000);

        gridLayout_3->addWidget(camXInitialSpin, 4, 1, 1, 1);

        camYActionSpin = new SmartDoubleSpinBox(cameraBox);
        camYActionSpin->setObjectName("camYActionSpin");
        camYActionSpin->setMaximumSize(QSize(64, 16777215));
        camYActionSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camYActionSpin->setDecimals(9);
        camYActionSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camYActionSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);
        camYActionSpin->setValue(1.000000000000000);

        gridLayout_3->addWidget(camYActionSpin, 6, 3, 1, 1);

        yActionLabel = new QLabel(cameraBox);
        yActionLabel->setObjectName("yActionLabel");

        gridLayout_3->addWidget(yActionLabel, 6, 2, 1, 1);

        zInitialLabel = new QLabel(cameraBox);
        zInitialLabel->setObjectName("zInitialLabel");

        gridLayout_3->addWidget(zInitialLabel, 4, 4, 1, 1);

        camZInitialSpin = new SmartDoubleSpinBox(cameraBox);
        camZInitialSpin->setObjectName("camZInitialSpin");
        camZInitialSpin->setEnabled(false);
        camZInitialSpin->setMaximumSize(QSize(64, 16777215));
        camZInitialSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        camZInitialSpin->setDecimals(9);
        camZInitialSpin->setMinimum(-10000000000000000303786028427003666890752.000000000000000);
        camZInitialSpin->setMaximum(10000000000000000303786028427003666890752.000000000000000);
        camZInitialSpin->setValue(1.000000000000000);

        gridLayout_3->addWidget(camZInitialSpin, 4, 5, 1, 1);

        camInitialPosLabel = new QLabel(cameraBox);
        camInitialPosLabel->setObjectName("camInitialPosLabel");
        camInitialPosLabel->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        camInitialPosLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        gridLayout_3->addWidget(camInitialPosLabel, 3, 0, 1, 3);

        sceneInitialCamPosBtn = new QPushButton(cameraBox);
        sceneInitialCamPosBtn->setObjectName("sceneInitialCamPosBtn");
        sceneInitialCamPosBtn->setMaximumSize(QSize(100, 16777215));
        sceneInitialCamPosBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));
        sceneInitialCamPosBtn->setCheckable(true);
        sceneInitialCamPosBtn->setChecked(true);

        gridLayout_3->addWidget(sceneInitialCamPosBtn, 3, 3, 1, 3);


        camPostLayout->addWidget(cameraBox);

        applyAntialiasBtn = new QPushButton(centralwidget);
        applyAntialiasBtn->setObjectName("applyAntialiasBtn");

        camPostLayout->addWidget(applyAntialiasBtn);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        camPostLayout->addItem(verticalSpacer);


        gridLayout_2->addLayout(camPostLayout, 7, 0, 1, 1);

        mainSettingsLayout = new QFormLayout();
        mainSettingsLayout->setObjectName("mainSettingsLayout");
        mainSettingsLayout->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy::AllNonFixedFieldsGrow);
        mainSettingsLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::DontWrapRows);
        mainSettingsLayout->setHorizontalSpacing(20);
        mainSettingsLayout->setVerticalSpacing(6);
        mainSettingsLayout->setContentsMargins(10, -1, -1, -1);
        resWidthLabel = new QLabel(centralwidget);
        resWidthLabel->setObjectName("resWidthLabel");

        mainSettingsLayout->setWidget(0, QFormLayout::LabelRole, resWidthLabel);

        resWidthSpin = new QSpinBox(centralwidget);
        resWidthSpin->setObjectName("resWidthSpin");
        resWidthSpin->setMaximumSize(QSize(150, 16777215));
        resWidthSpin->setWrapping(false);
        resWidthSpin->setFrame(false);
        resWidthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        resWidthSpin->setProperty("showGroupSeparator", QVariant(false));
        resWidthSpin->setMinimum(2);
        resWidthSpin->setMaximum(20000);
        resWidthSpin->setValue(1920);
        resWidthSpin->setDisplayIntegerBase(10);

        mainSettingsLayout->setWidget(0, QFormLayout::FieldRole, resWidthSpin);

        resHeightLabel = new QLabel(centralwidget);
        resHeightLabel->setObjectName("resHeightLabel");

        mainSettingsLayout->setWidget(1, QFormLayout::LabelRole, resHeightLabel);

        resHeightSpin = new QSpinBox(centralwidget);
        resHeightSpin->setObjectName("resHeightSpin");
        resHeightSpin->setMaximumSize(QSize(150, 16777215));
        resHeightSpin->setWrapping(false);
        resHeightSpin->setFrame(false);
        resHeightSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        resHeightSpin->setProperty("showGroupSeparator", QVariant(false));
        resHeightSpin->setMinimum(2);
        resHeightSpin->setMaximum(20000);
        resHeightSpin->setValue(1080);
        resHeightSpin->setDisplayIntegerBase(10);

        mainSettingsLayout->setWidget(1, QFormLayout::FieldRole, resHeightSpin);

        colorDepthLabel = new QLabel(centralwidget);
        colorDepthLabel->setObjectName("colorDepthLabel");

        mainSettingsLayout->setWidget(2, QFormLayout::LabelRole, colorDepthLabel);

        colorDepthSpin = new QSpinBox(centralwidget);
        colorDepthSpin->setObjectName("colorDepthSpin");
        colorDepthSpin->setMaximumSize(QSize(150, 16777215));
        colorDepthSpin->setWrapping(false);
        colorDepthSpin->setFrame(false);
        colorDepthSpin->setReadOnly(true);
        colorDepthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        colorDepthSpin->setProperty("showGroupSeparator", QVariant(false));
        colorDepthSpin->setMinimum(2);
        colorDepthSpin->setMaximum(24);
        colorDepthSpin->setValue(8);
        colorDepthSpin->setDisplayIntegerBase(10);

        mainSettingsLayout->setWidget(2, QFormLayout::FieldRole, colorDepthSpin);

        shadowBiasLabel = new QLabel(centralwidget);
        shadowBiasLabel->setObjectName("shadowBiasLabel");

        mainSettingsLayout->setWidget(3, QFormLayout::LabelRole, shadowBiasLabel);

        shadowBiasSpin = new SmartDoubleSpinBox(centralwidget);
        shadowBiasSpin->setObjectName("shadowBiasSpin");
        shadowBiasSpin->setMaximumSize(QSize(150, 16777215));
        shadowBiasSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        shadowBiasSpin->setDecimals(9);
        shadowBiasSpin->setMaximum(1.000000000000000);
        shadowBiasSpin->setSingleStep(0.010000000000000);
        shadowBiasSpin->setValue(0.100000000000000);

        mainSettingsLayout->setWidget(3, QFormLayout::FieldRole, shadowBiasSpin);

        refractBiasLabel = new QLabel(centralwidget);
        refractBiasLabel->setObjectName("refractBiasLabel");

        mainSettingsLayout->setWidget(4, QFormLayout::LabelRole, refractBiasLabel);

        refractBiasSpin = new SmartDoubleSpinBox(centralwidget);
        refractBiasSpin->setObjectName("refractBiasSpin");
        refractBiasSpin->setMaximumSize(QSize(150, 16777215));
        refractBiasSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        refractBiasSpin->setDecimals(9);
        refractBiasSpin->setMaximum(1.000000000000000);
        refractBiasSpin->setSingleStep(0.010000000000000);
        refractBiasSpin->setValue(0.001000000000000);

        mainSettingsLayout->setWidget(4, QFormLayout::FieldRole, refractBiasSpin);

        pathDepthLabel = new QLabel(centralwidget);
        pathDepthLabel->setObjectName("pathDepthLabel");

        mainSettingsLayout->setWidget(5, QFormLayout::LabelRole, pathDepthLabel);

        pathDepthSpin = new QSpinBox(centralwidget);
        pathDepthSpin->setObjectName("pathDepthSpin");
        pathDepthSpin->setMaximumSize(QSize(150, 16777215));
        pathDepthSpin->setWrapping(false);
        pathDepthSpin->setFrame(false);
        pathDepthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        pathDepthSpin->setProperty("showGroupSeparator", QVariant(false));
        pathDepthSpin->setMinimum(2);
        pathDepthSpin->setMaximum(100);
        pathDepthSpin->setValue(5);
        pathDepthSpin->setDisplayIntegerBase(10);

        mainSettingsLayout->setWidget(5, QFormLayout::FieldRole, pathDepthSpin);

        renderModeLabel = new QLabel(centralwidget);
        renderModeLabel->setObjectName("renderModeLabel");

        mainSettingsLayout->setWidget(6, QFormLayout::LabelRole, renderModeLabel);

        renderModeCombo = new QComboBox(centralwidget);
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->setObjectName("renderModeCombo");
        renderModeCombo->setMaximumSize(QSize(150, 16777215));
        renderModeCombo->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        renderModeCombo->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-top: 1px;\n"
"padding-bottom: 1px;"));

        mainSettingsLayout->setWidget(6, QFormLayout::FieldRole, renderModeCombo);

        bucketSizeLabel = new QLabel(centralwidget);
        bucketSizeLabel->setObjectName("bucketSizeLabel");

        mainSettingsLayout->setWidget(7, QFormLayout::LabelRole, bucketSizeLabel);

        bucketSizeSpin = new QSpinBox(centralwidget);
        bucketSizeSpin->setObjectName("bucketSizeSpin");
        bucketSizeSpin->setMaximumSize(QSize(150, 16777215));
        bucketSizeSpin->setWrapping(false);
        bucketSizeSpin->setFrame(false);
        bucketSizeSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        bucketSizeSpin->setProperty("showGroupSeparator", QVariant(false));
        bucketSizeSpin->setMinimum(2);
        bucketSizeSpin->setMaximum(1000);
        bucketSizeSpin->setValue(24);
        bucketSizeSpin->setDisplayIntegerBase(10);

        mainSettingsLayout->setWidget(7, QFormLayout::FieldRole, bucketSizeSpin);

        accTreeMaxDepthLabel = new QLabel(centralwidget);
        accTreeMaxDepthLabel->setObjectName("accTreeMaxDepthLabel");

        mainSettingsLayout->setWidget(8, QFormLayout::LabelRole, accTreeMaxDepthLabel);

        accTreeMaxDepthSpin = new QSpinBox(centralwidget);
        accTreeMaxDepthSpin->setObjectName("accTreeMaxDepthSpin");
        accTreeMaxDepthSpin->setMaximumSize(QSize(150, 16777215));
        accTreeMaxDepthSpin->setWrapping(false);
        accTreeMaxDepthSpin->setFrame(false);
        accTreeMaxDepthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        accTreeMaxDepthSpin->setProperty("showGroupSeparator", QVariant(false));
        accTreeMaxDepthSpin->setMinimum(1);
        accTreeMaxDepthSpin->setMaximum(30);
        accTreeMaxDepthSpin->setValue(15);
        accTreeMaxDepthSpin->setDisplayIntegerBase(10);

        mainSettingsLayout->setWidget(8, QFormLayout::FieldRole, accTreeMaxDepthSpin);

        maxAABBTriCountLabel = new QLabel(centralwidget);
        maxAABBTriCountLabel->setObjectName("maxAABBTriCountLabel");

        mainSettingsLayout->setWidget(9, QFormLayout::LabelRole, maxAABBTriCountLabel);

        maxAABBTriCountSpin = new QSpinBox(centralwidget);
        maxAABBTriCountSpin->setObjectName("maxAABBTriCountSpin");
        maxAABBTriCountSpin->setMaximumSize(QSize(150, 16777215));
        maxAABBTriCountSpin->setWrapping(false);
        maxAABBTriCountSpin->setFrame(false);
        maxAABBTriCountSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        maxAABBTriCountSpin->setProperty("showGroupSeparator", QVariant(false));
        maxAABBTriCountSpin->setMinimum(1);
        maxAABBTriCountSpin->setMaximum(50);
        maxAABBTriCountSpin->setValue(10);
        maxAABBTriCountSpin->setDisplayIntegerBase(10);

        mainSettingsLayout->setWidget(9, QFormLayout::FieldRole, maxAABBTriCountSpin);

        rendNameLabel = new QLabel(centralwidget);
        rendNameLabel->setObjectName("rendNameLabel");

        mainSettingsLayout->setWidget(10, QFormLayout::LabelRole, rendNameLabel);

        rendNameEntry = new QLineEdit(centralwidget);
        rendNameEntry->setObjectName("rendNameEntry");
        rendNameEntry->setMaximumSize(QSize(150, 16777215));

        mainSettingsLayout->setWidget(10, QFormLayout::FieldRole, rendNameEntry);

        overrideBGColorBtn = new QPushButton(centralwidget);
        overrideBGColorBtn->setObjectName("overrideBGColorBtn");
        overrideBGColorBtn->setMaximumSize(QSize(150, 16777215));
        overrideBGColorBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));
        overrideBGColorBtn->setCheckable(true);
        overrideBGColorBtn->setChecked(false);
        overrideBGColorBtn->setFlat(false);

        mainSettingsLayout->setWidget(11, QFormLayout::LabelRole, overrideBGColorBtn);

        colorPickerBtn = new QToolButton(centralwidget);
        colorPickerBtn->setObjectName("colorPickerBtn");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(colorPickerBtn->sizePolicy().hasHeightForWidth());
        colorPickerBtn->setSizePolicy(sizePolicy);
        colorPickerBtn->setMaximumSize(QSize(150, 16777215));
        colorPickerBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        colorPickerBtn->setAutoFillBackground(false);
        colorPickerBtn->setStyleSheet(QString::fromUtf8(""));
        colorPickerBtn->setCheckable(false);
        colorPickerBtn->setChecked(false);
        colorPickerBtn->setAutoRepeat(false);
        colorPickerBtn->setAutoExclusive(false);
        colorPickerBtn->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
        colorPickerBtn->setAutoRaise(false);
        colorPickerBtn->setArrowType(Qt::ArrowType::NoArrow);

        mainSettingsLayout->setWidget(11, QFormLayout::FieldRole, colorPickerBtn);

        ignoreBackfaceBtn = new QPushButton(centralwidget);
        ignoreBackfaceBtn->setObjectName("ignoreBackfaceBtn");
        ignoreBackfaceBtn->setMaximumSize(QSize(100, 16777215));
        ignoreBackfaceBtn->setStyleSheet(QString::fromUtf8("padding-left: 8px;\n"
"padding-right: 8px;\n"
"padding-top: 2px;\n"
"padding-bottom: 2px;"));
        ignoreBackfaceBtn->setCheckable(true);
        ignoreBackfaceBtn->setChecked(true);
        ignoreBackfaceBtn->setFlat(false);

        mainSettingsLayout->setWidget(14, QFormLayout::LabelRole, ignoreBackfaceBtn);

        enableGIBtn = new QPushButton(centralwidget);
        enableGIBtn->setObjectName("enableGIBtn");
        enableGIBtn->setEnabled(true);
        enableGIBtn->setCheckable(true);

        mainSettingsLayout->setWidget(12, QFormLayout::LabelRole, enableGIBtn);

        giSamplesLabel = new QLabel(centralwidget);
        giSamplesLabel->setObjectName("giSamplesLabel");
        giSamplesLabel->setEnabled(false);

        mainSettingsLayout->setWidget(13, QFormLayout::LabelRole, giSamplesLabel);

        giSamplesSpin = new QSpinBox(centralwidget);
        giSamplesSpin->setObjectName("giSamplesSpin");
        giSamplesSpin->setEnabled(false);
        giSamplesSpin->setMaximumSize(QSize(100, 16777215));
        giSamplesSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        giSamplesSpin->setMinimum(1);
        giSamplesSpin->setMaximum(10000);
        giSamplesSpin->setValue(64);

        mainSettingsLayout->setWidget(13, QFormLayout::FieldRole, giSamplesSpin);


        gridLayout_2->addLayout(mainSettingsLayout, 2, 0, 4, 1);

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

        saveDirLayout->addWidget(saveDirEntry);


        gridLayout_2->addLayout(saveDirLayout, 1, 0, 1, 6);

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

        sceneFileLayout->addWidget(sceneFileEntry);


        gridLayout_2->addLayout(sceneFileLayout, 0, 0, 1, 6);

        gridLayout_2->setRowStretch(7, 1);
        gridLayout_2->setColumnStretch(1, 1);
        RayTracer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RayTracer);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1350, 21));
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

        camActionCombo->setCurrentIndex(0);
        renderModeCombo->setCurrentIndex(0);
        overrideBGColorBtn->setDefault(false);


        QMetaObject::connectSlotsByName(RayTracer);
    } // setupUi

    void retranslateUi(QMainWindow *RayTracer)
    {
        RayTracer->setWindowTitle(QCoreApplication::translate("RayTracer", "RayTracer", nullptr));
        actionRender->setText(QCoreApplication::translate("RayTracer", "Render", nullptr));
        actionExit->setText(QCoreApplication::translate("RayTracer", "Exit", nullptr));
        prevImgBtn->setText(QString());
        nextImgBtn->setText(QString());
        zoomResetBtn->setText(QString());
        openImgBtn->setText(QCoreApplication::translate("RayTracer", "Open Image", nullptr));
        renderBtn->setText(QCoreApplication::translate("RayTracer", "Reneder", nullptr));
        cameraBox->setTitle(QCoreApplication::translate("RayTracer", "Camera Settings", nullptr));
        xActionLabel->setText(QCoreApplication::translate("RayTracer", "X:", nullptr));
        camActionLabel->setText(QCoreApplication::translate("RayTracer", "Move Offset", nullptr));
        frameNumLabel->setText(QCoreApplication::translate("RayTracer", "Number of frames:", nullptr));
        xInitialLabel->setText(QCoreApplication::translate("RayTracer", "X:", nullptr));
        zActionLabel->setText(QCoreApplication::translate("RayTracer", "Z:", nullptr));
        yInitialLabel->setText(QCoreApplication::translate("RayTracer", "Y:", nullptr));
        camActionCombo->setItemText(0, QCoreApplication::translate("RayTracer", "Static Image", nullptr));
        camActionCombo->setItemText(1, QCoreApplication::translate("RayTracer", "Move Animation", nullptr));
        camActionCombo->setItemText(2, QCoreApplication::translate("RayTracer", "Orbit Animation", nullptr));

        yActionLabel->setText(QCoreApplication::translate("RayTracer", "Y:", nullptr));
        zInitialLabel->setText(QCoreApplication::translate("RayTracer", "Z:", nullptr));
        camInitialPosLabel->setText(QCoreApplication::translate("RayTracer", "Initial Position", nullptr));
        sceneInitialCamPosBtn->setText(QCoreApplication::translate("RayTracer", "Scene Position", nullptr));
        applyAntialiasBtn->setText(QCoreApplication::translate("RayTracer", "Apply Antialiasing", nullptr));
#if QT_CONFIG(tooltip)
        resWidthLabel->setToolTip(QCoreApplication::translate("RayTracer", "The width of the resulting image in pixels.", nullptr));
#endif // QT_CONFIG(tooltip)
        resWidthLabel->setText(QCoreApplication::translate("RayTracer", "Width", nullptr));
#if QT_CONFIG(tooltip)
        resWidthSpin->setToolTip(QCoreApplication::translate("RayTracer", "The width of the resulting image in pixels.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        resHeightLabel->setToolTip(QCoreApplication::translate("RayTracer", "The height of the resulting image in pixels.", nullptr));
#endif // QT_CONFIG(tooltip)
        resHeightLabel->setText(QCoreApplication::translate("RayTracer", "Height", nullptr));
#if QT_CONFIG(tooltip)
        resHeightSpin->setToolTip(QCoreApplication::translate("RayTracer", "The height of the resulting image in pixels.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        colorDepthLabel->setToolTip(QCoreApplication::translate("RayTracer", "Color precision. The higher the color depth - the more color nuances will be produced. Defaults to 8-bit color, which makes each color component have a maximum value of 255, for a total of 16,777,216 colors.", nullptr));
#endif // QT_CONFIG(tooltip)
        colorDepthLabel->setText(QCoreApplication::translate("RayTracer", "Color Depth", nullptr));
#if QT_CONFIG(tooltip)
        colorDepthSpin->setToolTip(QCoreApplication::translate("RayTracer", "Color precision. The higher the color depth - the more color nuances will be produced. Defaults to 8-bit color, which makes each color component have a maximum value of 255, for a total of 16,777,216 colors. Currently not set up.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        shadowBiasLabel->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for shadow rays. Use for correcting shadow artifacts when rendering diffuse objects.", nullptr));
#endif // QT_CONFIG(tooltip)
        shadowBiasLabel->setText(QCoreApplication::translate("RayTracer", "Shadow Bias", nullptr));
#if QT_CONFIG(tooltip)
        shadowBiasSpin->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for shadow rays. Use for correcting shadow artifacts when rendering diffuse objects.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        refractBiasLabel->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for refraction rays. Use for correcting refraction artifacts when rendering semi-transparent objects.", nullptr));
#endif // QT_CONFIG(tooltip)
        refractBiasLabel->setText(QCoreApplication::translate("RayTracer", "Refraction Bias", nullptr));
#if QT_CONFIG(tooltip)
        refractBiasSpin->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for refraction rays. Use for correcting refraction artifacts when rendering semi-transparent objects.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        pathDepthLabel->setToolTip(QCoreApplication::translate("RayTracer", "The number of times a ray can be reflected or refracted before it stops calculating.", nullptr));
#endif // QT_CONFIG(tooltip)
        pathDepthLabel->setText(QCoreApplication::translate("RayTracer", "Path Depth", nullptr));
#if QT_CONFIG(tooltip)
        pathDepthSpin->setToolTip(QCoreApplication::translate("RayTracer", "The number of times a ray can be reflected or refracted before it stops calculating.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        renderModeLabel->setToolTip(QCoreApplication::translate("RayTracer", "The number of times a ray can be reflected or refracted before it stops calculating.", nullptr));
#endif // QT_CONFIG(tooltip)
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
#if QT_CONFIG(tooltip)
        bucketSizeLabel->setToolTip(QCoreApplication::translate("RayTracer", "The size of each render bucket. Defaults to 24x24 pixels.", nullptr));
#endif // QT_CONFIG(tooltip)
        bucketSizeLabel->setText(QCoreApplication::translate("RayTracer", "Bucket Size", nullptr));
#if QT_CONFIG(tooltip)
        bucketSizeSpin->setToolTip(QCoreApplication::translate("RayTracer", "The size of each render bucket. Defaults to 24x24 pixels.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        accTreeMaxDepthLabel->setToolTip(QCoreApplication::translate("RayTracer", "The maximum depth for building an acceleration tree (KD or BVH). Defaults to 15.", nullptr));
#endif // QT_CONFIG(tooltip)
        accTreeMaxDepthLabel->setText(QCoreApplication::translate("RayTracer", "Build Max Depth", nullptr));
#if QT_CONFIG(tooltip)
        accTreeMaxDepthSpin->setToolTip(QCoreApplication::translate("RayTracer", "The maximum depth for building an acceleration tree (KD or BVH). Defaults to 15.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        maxAABBTriCountLabel->setToolTip(QCoreApplication::translate("RayTracer", "The maximum number of triangles a single node of the acceleration tree can hold. Defaults to 10.", nullptr));
#endif // QT_CONFIG(tooltip)
        maxAABBTriCountLabel->setText(QCoreApplication::translate("RayTracer", "Build Max Primitives", nullptr));
#if QT_CONFIG(tooltip)
        maxAABBTriCountSpin->setToolTip(QCoreApplication::translate("RayTracer", "The maximum number of triangles a single node of the acceleration tree can hold. Defaults to 10.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        rendNameLabel->setToolTip(QCoreApplication::translate("RayTracer", "The name of the rendered image. If not set, it will use the name of the scene file.", nullptr));
#endif // QT_CONFIG(tooltip)
        rendNameLabel->setText(QCoreApplication::translate("RayTracer", "Render Name", nullptr));
#if QT_CONFIG(tooltip)
        rendNameEntry->setToolTip(QCoreApplication::translate("RayTracer", "The name of the rendered image. If not set, it will use the name of the scene file.", nullptr));
#endif // QT_CONFIG(tooltip)
        rendNameEntry->setText(QString());
        rendNameEntry->setPlaceholderText(QCoreApplication::translate("RayTracer", "<Default>", nullptr));
#if QT_CONFIG(tooltip)
        overrideBGColorBtn->setToolTip(QCoreApplication::translate("RayTracer", "Use custom color for the background instead of the one specified in the scene file (if at all).", nullptr));
#endif // QT_CONFIG(tooltip)
        overrideBGColorBtn->setText(QCoreApplication::translate("RayTracer", "Custom BG Color", nullptr));
#if QT_CONFIG(tooltip)
        colorPickerBtn->setToolTip(QCoreApplication::translate("RayTracer", "Click to select a color", nullptr));
#endif // QT_CONFIG(tooltip)
        colorPickerBtn->setText(QString());
#if QT_CONFIG(tooltip)
        ignoreBackfaceBtn->setToolTip(QCoreApplication::translate("RayTracer", "Should the back faces of the triangles be rendered or ignored.", nullptr));
#endif // QT_CONFIG(tooltip)
        ignoreBackfaceBtn->setText(QCoreApplication::translate("RayTracer", "Ignore Backface", nullptr));
#if QT_CONFIG(tooltip)
        enableGIBtn->setToolTip(QCoreApplication::translate("RayTracer", "Render with Global Illumination.", nullptr));
#endif // QT_CONFIG(tooltip)
        enableGIBtn->setText(QCoreApplication::translate("RayTracer", "Enable GI", nullptr));
        giSamplesLabel->setText(QCoreApplication::translate("RayTracer", "GI samples", nullptr));
#if QT_CONFIG(tooltip)
        saveDirBtn->setToolTip(QCoreApplication::translate("RayTracer", "Choose a directory to output the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
        saveDirBtn->setText(QCoreApplication::translate("RayTracer", "Render To...", nullptr));
#if QT_CONFIG(tooltip)
        saveDirEntry->setToolTip(QCoreApplication::translate("RayTracer", "The directory to output the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
        saveDirEntry->setText(QCoreApplication::translate("RayTracer", "./renders", nullptr));
        saveDirEntry->setPlaceholderText(QString());
#if QT_CONFIG(tooltip)
        sceneFileBtn->setToolTip(QCoreApplication::translate("RayTracer", "Choose a scene file to be processed and rendered.", nullptr));
#endif // QT_CONFIG(tooltip)
        sceneFileBtn->setText(QCoreApplication::translate("RayTracer", "Scene File...", nullptr));
#if QT_CONFIG(tooltip)
        sceneFileEntry->setToolTip(QCoreApplication::translate("RayTracer", "The scene file to be processed and rendered.", nullptr));
#endif // QT_CONFIG(tooltip)
        sceneFileEntry->setText(QString());
        sceneFileEntry->setPlaceholderText(QCoreApplication::translate("RayTracer", "<Scene File>", nullptr));
        menuMenu->setTitle(QCoreApplication::translate("RayTracer", "Menu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RayTracer: public Ui_RayTracer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_H
