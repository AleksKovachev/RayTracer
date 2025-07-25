/********************************************************************************
** Form generated from reading UI file 'RayTracer.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RayTracer
{
public:
    QAction *actionRender;
    QAction *actionExit;
    QWidget *centralwidget;
    QWidget *formLayoutWidget;
    QFormLayout *mainSettings;
    QLabel *resWidthLabel;
    QSpinBox *resWidthSpin;
    QLabel *resHeightLabel;
    QSpinBox *resHeightSpin;
    QLabel *colorDepthLabel;
    QSpinBox *colorDepthSpin;
    QLabel *shadowBiasLabel;
    QDoubleSpinBox *shadowBianSpin;
    QLabel *refractBiasLabel;
    QDoubleSpinBox *refractBianSpin;
    QLabel *pathDepthLabel_2;
    QSpinBox *pathDepthSpin;
    QLabel *renderModeLabel;
    QComboBox *renderModeCombo;
    QLabel *bucketSizeLabel;
    QSpinBox *resHeightSpin_2;
    QLabel *accTreeMaxDepthLabel;
    QSpinBox *accTreeMaxDepthSpin;
    QLabel *maxAABBTriCountLabel;
    QSpinBox *maxAABBTriCountSpin;
    QLabel *colorPreviewLabel;
    QToolButton *colorPickerBtn;
    QLabel *rendNameLabel;
    QLineEdit *rendNameEntry;
    QPushButton *ignoreBackfaceBtn;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *saveDirBtn;
    QLineEdit *saveDirEntry;
    QGraphicsView *graphicsView;
    QPushButton *renderBtn;
    QMenuBar *menubar;
    QMenu *menuMenu;
    QMenu *menuAbout;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RayTracer)
    {
        if (RayTracer->objectName().isEmpty())
            RayTracer->setObjectName("RayTracer");
        RayTracer->resize(1280, 720);
        RayTracer->setMinimumSize(QSize(800, 600));
        actionRender = new QAction(RayTracer);
        actionRender->setObjectName("actionRender");
        actionExit = new QAction(RayTracer);
        actionExit->setObjectName("actionExit");
        centralwidget = new QWidget(RayTracer);
        centralwidget->setObjectName("centralwidget");
        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(20, 40, 281, 351));
        mainSettings = new QFormLayout(formLayoutWidget);
        mainSettings->setObjectName("mainSettings");
        mainSettings->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy::AllNonFixedFieldsGrow);
        mainSettings->setHorizontalSpacing(20);
        mainSettings->setVerticalSpacing(6);
        mainSettings->setContentsMargins(0, 0, 0, 0);
        resWidthLabel = new QLabel(formLayoutWidget);
        resWidthLabel->setObjectName("resWidthLabel");

        mainSettings->setWidget(0, QFormLayout::LabelRole, resWidthLabel);

        resWidthSpin = new QSpinBox(formLayoutWidget);
        resWidthSpin->setObjectName("resWidthSpin");
        resWidthSpin->setWrapping(false);
        resWidthSpin->setFrame(false);
        resWidthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        resWidthSpin->setProperty("showGroupSeparator", QVariant(false));
        resWidthSpin->setMinimum(2);
        resWidthSpin->setMaximum(20000);
        resWidthSpin->setValue(1920);
        resWidthSpin->setDisplayIntegerBase(10);

        mainSettings->setWidget(0, QFormLayout::FieldRole, resWidthSpin);

        resHeightLabel = new QLabel(formLayoutWidget);
        resHeightLabel->setObjectName("resHeightLabel");

        mainSettings->setWidget(1, QFormLayout::LabelRole, resHeightLabel);

        resHeightSpin = new QSpinBox(formLayoutWidget);
        resHeightSpin->setObjectName("resHeightSpin");
        resHeightSpin->setWrapping(false);
        resHeightSpin->setFrame(false);
        resHeightSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        resHeightSpin->setProperty("showGroupSeparator", QVariant(false));
        resHeightSpin->setMinimum(2);
        resHeightSpin->setMaximum(20000);
        resHeightSpin->setValue(1080);
        resHeightSpin->setDisplayIntegerBase(10);

        mainSettings->setWidget(1, QFormLayout::FieldRole, resHeightSpin);

        colorDepthLabel = new QLabel(formLayoutWidget);
        colorDepthLabel->setObjectName("colorDepthLabel");

        mainSettings->setWidget(2, QFormLayout::LabelRole, colorDepthLabel);

        colorDepthSpin = new QSpinBox(formLayoutWidget);
        colorDepthSpin->setObjectName("colorDepthSpin");
        colorDepthSpin->setWrapping(false);
        colorDepthSpin->setFrame(false);
        colorDepthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        colorDepthSpin->setProperty("showGroupSeparator", QVariant(false));
        colorDepthSpin->setMinimum(2);
        colorDepthSpin->setMaximum(24);
        colorDepthSpin->setValue(8);
        colorDepthSpin->setDisplayIntegerBase(10);

        mainSettings->setWidget(2, QFormLayout::FieldRole, colorDepthSpin);

        shadowBiasLabel = new QLabel(formLayoutWidget);
        shadowBiasLabel->setObjectName("shadowBiasLabel");

        mainSettings->setWidget(3, QFormLayout::LabelRole, shadowBiasLabel);

        shadowBianSpin = new QDoubleSpinBox(formLayoutWidget);
        shadowBianSpin->setObjectName("shadowBianSpin");
        shadowBianSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        shadowBianSpin->setDecimals(4);
        shadowBianSpin->setMaximum(1.000000000000000);
        shadowBianSpin->setSingleStep(0.010000000000000);
        shadowBianSpin->setValue(0.100000000000000);

        mainSettings->setWidget(3, QFormLayout::FieldRole, shadowBianSpin);

        refractBiasLabel = new QLabel(formLayoutWidget);
        refractBiasLabel->setObjectName("refractBiasLabel");

        mainSettings->setWidget(4, QFormLayout::LabelRole, refractBiasLabel);

        refractBianSpin = new QDoubleSpinBox(formLayoutWidget);
        refractBianSpin->setObjectName("refractBianSpin");
        refractBianSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        refractBianSpin->setDecimals(4);
        refractBianSpin->setMaximum(1.000000000000000);
        refractBianSpin->setSingleStep(0.010000000000000);
        refractBianSpin->setValue(0.001000000000000);

        mainSettings->setWidget(4, QFormLayout::FieldRole, refractBianSpin);

        pathDepthLabel_2 = new QLabel(formLayoutWidget);
        pathDepthLabel_2->setObjectName("pathDepthLabel_2");

        mainSettings->setWidget(5, QFormLayout::LabelRole, pathDepthLabel_2);

        pathDepthSpin = new QSpinBox(formLayoutWidget);
        pathDepthSpin->setObjectName("pathDepthSpin");
        pathDepthSpin->setWrapping(false);
        pathDepthSpin->setFrame(false);
        pathDepthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        pathDepthSpin->setProperty("showGroupSeparator", QVariant(false));
        pathDepthSpin->setMinimum(2);
        pathDepthSpin->setMaximum(100);
        pathDepthSpin->setValue(8);
        pathDepthSpin->setDisplayIntegerBase(10);

        mainSettings->setWidget(5, QFormLayout::FieldRole, pathDepthSpin);

        renderModeLabel = new QLabel(formLayoutWidget);
        renderModeLabel->setObjectName("renderModeLabel");

        mainSettings->setWidget(6, QFormLayout::LabelRole, renderModeLabel);

        renderModeCombo = new QComboBox(formLayoutWidget);
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->addItem(QString());
        renderModeCombo->setObjectName("renderModeCombo");
        renderModeCombo->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);

        mainSettings->setWidget(6, QFormLayout::FieldRole, renderModeCombo);

        bucketSizeLabel = new QLabel(formLayoutWidget);
        bucketSizeLabel->setObjectName("bucketSizeLabel");

        mainSettings->setWidget(7, QFormLayout::LabelRole, bucketSizeLabel);

        resHeightSpin_2 = new QSpinBox(formLayoutWidget);
        resHeightSpin_2->setObjectName("resHeightSpin_2");
        resHeightSpin_2->setWrapping(false);
        resHeightSpin_2->setFrame(false);
        resHeightSpin_2->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        resHeightSpin_2->setProperty("showGroupSeparator", QVariant(false));
        resHeightSpin_2->setMinimum(2);
        resHeightSpin_2->setMaximum(1000);
        resHeightSpin_2->setValue(24);
        resHeightSpin_2->setDisplayIntegerBase(10);

        mainSettings->setWidget(7, QFormLayout::FieldRole, resHeightSpin_2);

        accTreeMaxDepthLabel = new QLabel(formLayoutWidget);
        accTreeMaxDepthLabel->setObjectName("accTreeMaxDepthLabel");

        mainSettings->setWidget(8, QFormLayout::LabelRole, accTreeMaxDepthLabel);

        accTreeMaxDepthSpin = new QSpinBox(formLayoutWidget);
        accTreeMaxDepthSpin->setObjectName("accTreeMaxDepthSpin");
        accTreeMaxDepthSpin->setWrapping(false);
        accTreeMaxDepthSpin->setFrame(false);
        accTreeMaxDepthSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        accTreeMaxDepthSpin->setProperty("showGroupSeparator", QVariant(false));
        accTreeMaxDepthSpin->setMinimum(1);
        accTreeMaxDepthSpin->setMaximum(30);
        accTreeMaxDepthSpin->setValue(15);
        accTreeMaxDepthSpin->setDisplayIntegerBase(10);

        mainSettings->setWidget(8, QFormLayout::FieldRole, accTreeMaxDepthSpin);

        maxAABBTriCountLabel = new QLabel(formLayoutWidget);
        maxAABBTriCountLabel->setObjectName("maxAABBTriCountLabel");

        mainSettings->setWidget(9, QFormLayout::LabelRole, maxAABBTriCountLabel);

        maxAABBTriCountSpin = new QSpinBox(formLayoutWidget);
        maxAABBTriCountSpin->setObjectName("maxAABBTriCountSpin");
        maxAABBTriCountSpin->setWrapping(false);
        maxAABBTriCountSpin->setFrame(false);
        maxAABBTriCountSpin->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        maxAABBTriCountSpin->setProperty("showGroupSeparator", QVariant(false));
        maxAABBTriCountSpin->setMinimum(1);
        maxAABBTriCountSpin->setMaximum(50);
        maxAABBTriCountSpin->setValue(10);
        maxAABBTriCountSpin->setDisplayIntegerBase(10);

        mainSettings->setWidget(9, QFormLayout::FieldRole, maxAABBTriCountSpin);

        colorPreviewLabel = new QLabel(formLayoutWidget);
        colorPreviewLabel->setObjectName("colorPreviewLabel");
        colorPreviewLabel->setMinimumSize(QSize(24, 24));
        colorPreviewLabel->setFrameShape(QFrame::Shape::NoFrame);

        mainSettings->setWidget(10, QFormLayout::LabelRole, colorPreviewLabel);

        colorPickerBtn = new QToolButton(formLayoutWidget);
        colorPickerBtn->setObjectName("colorPickerBtn");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(colorPickerBtn->sizePolicy().hasHeightForWidth());
        colorPickerBtn->setSizePolicy(sizePolicy);
        colorPickerBtn->setCheckable(false);
        colorPickerBtn->setChecked(false);
        colorPickerBtn->setAutoRepeat(false);
        colorPickerBtn->setAutoExclusive(false);
        colorPickerBtn->setAutoRaise(false);

        mainSettings->setWidget(10, QFormLayout::FieldRole, colorPickerBtn);

        rendNameLabel = new QLabel(formLayoutWidget);
        rendNameLabel->setObjectName("rendNameLabel");

        mainSettings->setWidget(11, QFormLayout::LabelRole, rendNameLabel);

        rendNameEntry = new QLineEdit(formLayoutWidget);
        rendNameEntry->setObjectName("rendNameEntry");

        mainSettings->setWidget(11, QFormLayout::FieldRole, rendNameEntry);

        ignoreBackfaceBtn = new QPushButton(formLayoutWidget);
        ignoreBackfaceBtn->setObjectName("ignoreBackfaceBtn");
        ignoreBackfaceBtn->setCheckable(true);
        ignoreBackfaceBtn->setChecked(true);

        mainSettings->setWidget(12, QFormLayout::FieldRole, ignoreBackfaceBtn);

        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(10, 0, 1241, 26));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        saveDirBtn = new QPushButton(horizontalLayoutWidget);
        saveDirBtn->setObjectName("saveDirBtn");

        horizontalLayout->addWidget(saveDirBtn);

        saveDirEntry = new QLineEdit(horizontalLayoutWidget);
        saveDirEntry->setObjectName("saveDirEntry");

        horizontalLayout->addWidget(saveDirEntry);

        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(330, 40, 920, 550));
        graphicsView->setFrameShape(QFrame::Shape::Box);
        renderBtn = new QPushButton(centralwidget);
        renderBtn->setObjectName("renderBtn");
        renderBtn->setGeometry(QRect(1177, 603, 71, 31));
        RayTracer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RayTracer);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 33));
        menuMenu = new QMenu(menubar);
        menuMenu->setObjectName("menuMenu");
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName("menuAbout");
        RayTracer->setMenuBar(menubar);
        statusbar = new QStatusBar(RayTracer);
        statusbar->setObjectName("statusbar");
        RayTracer->setStatusBar(statusbar);

        menubar->addAction(menuMenu->menuAction());
        menubar->addAction(menuAbout->menuAction());
        menuMenu->addAction(actionRender);
        menuMenu->addSeparator();
        menuMenu->addAction(actionExit);

        retranslateUi(RayTracer);

        renderModeCombo->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(RayTracer);
    } // setupUi

    void retranslateUi(QMainWindow *RayTracer)
    {
        RayTracer->setWindowTitle(QCoreApplication::translate("RayTracer", "RayTracer", nullptr));
        actionRender->setText(QCoreApplication::translate("RayTracer", "Render", nullptr));
        actionExit->setText(QCoreApplication::translate("RayTracer", "Exit", nullptr));
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
        colorDepthSpin->setToolTip(QCoreApplication::translate("RayTracer", "Color precision. The higher the color depth - the more color nuances will be produced. Defaults to 8-bit color, which makes each color component have a maximum value of 255, for a total of 16,777,216 colors.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        shadowBiasLabel->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for shadow rays. Use for correcting shadow artifacts when rendering diffuse objects.", nullptr));
#endif // QT_CONFIG(tooltip)
        shadowBiasLabel->setText(QCoreApplication::translate("RayTracer", "Shadow Bias", nullptr));
#if QT_CONFIG(tooltip)
        shadowBianSpin->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for shadow rays. Use for correcting shadow artifacts when rendering diffuse objects.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        refractBiasLabel->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for refraction rays. Use for correcting refraction artifacts when rendering semi-transparent objects.", nullptr));
#endif // QT_CONFIG(tooltip)
        refractBiasLabel->setText(QCoreApplication::translate("RayTracer", "Refraction Bias", nullptr));
#if QT_CONFIG(tooltip)
        refractBianSpin->setToolTip(QCoreApplication::translate("RayTracer", "Precision fix for refraction rays. Use for correcting refraction artifacts when rendering semi-transparent objects.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        pathDepthLabel_2->setToolTip(QCoreApplication::translate("RayTracer", "The number of times a ray can be reflected or refracted before it stops calculating.", nullptr));
#endif // QT_CONFIG(tooltip)
        pathDepthLabel_2->setText(QCoreApplication::translate("RayTracer", "Path Depth", nullptr));
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
        resHeightSpin_2->setToolTip(QCoreApplication::translate("RayTracer", "The size of each render bucket. Defaults to 24x24 pixels.", nullptr));
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
        colorPreviewLabel->setToolTip(QCoreApplication::translate("RayTracer", "The background color for the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
        colorPreviewLabel->setText(QCoreApplication::translate("RayTracer", "Background Color", nullptr));
#if QT_CONFIG(tooltip)
        colorPickerBtn->setToolTip(QCoreApplication::translate("RayTracer", "Click to select a color", nullptr));
#endif // QT_CONFIG(tooltip)
        colorPickerBtn->setText(QString());
#if QT_CONFIG(tooltip)
        rendNameLabel->setToolTip(QCoreApplication::translate("RayTracer", "The name of the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
        rendNameLabel->setText(QCoreApplication::translate("RayTracer", "Render Name", nullptr));
#if QT_CONFIG(tooltip)
        rendNameEntry->setToolTip(QCoreApplication::translate("RayTracer", "The name of the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
        rendNameEntry->setText(QCoreApplication::translate("RayTracer", "Render_01", nullptr));
#if QT_CONFIG(tooltip)
        ignoreBackfaceBtn->setToolTip(QCoreApplication::translate("RayTracer", "Should the back faces of the triangles be rendered or ignored.", nullptr));
#endif // QT_CONFIG(tooltip)
        ignoreBackfaceBtn->setText(QCoreApplication::translate("RayTracer", "Ignore Backface", nullptr));
#if QT_CONFIG(tooltip)
        saveDirBtn->setToolTip(QCoreApplication::translate("RayTracer", "Choose a directory to output the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
        saveDirBtn->setText(QCoreApplication::translate("RayTracer", "Render To...", nullptr));
#if QT_CONFIG(tooltip)
        saveDirEntry->setToolTip(QCoreApplication::translate("RayTracer", "The directory to output the rendered image.", nullptr));
#endif // QT_CONFIG(tooltip)
        saveDirEntry->setText(QCoreApplication::translate("RayTracer", "./renders", nullptr));
        renderBtn->setText(QCoreApplication::translate("RayTracer", "Reneder", nullptr));
        menuMenu->setTitle(QCoreApplication::translate("RayTracer", "Menu", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("RayTracer", "About...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RayTracer: public Ui_RayTracer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_H
