#include "ncwin.h"
#include "rcprintpreview_dialog.h"
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QDialog>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QPrintPreviewWidget>
#include <QPageSetupDialog>
#include <QPrintDialog>
#include <QPrinterInfo>
#include <QPageSize>
#include <QtPrintSupport/qtprintsupportglobal.h>
#include <QPrinter>
#include <Qtoolbar>
#include <QAction>
#include <QActionGroup>
#include <QComboBox>
#include <QPageSize>
#include <QTimer>

class RCPrintPreviewMainWindow : public QMainWindow
{
public:
    RCPrintPreviewMainWindow(QWidget *parent) : QMainWindow(parent) {}
    QMenu *createPopupMenu() { return 0; }
};


class printScaleValidator : public QDoubleValidator
{
public:
    printScaleValidator(QObject* parent)
        : QDoubleValidator(parent) {}
    printScaleValidator(qreal bottom, qreal top, int decimals, QObject *parent)
        : QDoubleValidator(bottom, top, decimals, parent) {}

    State validate(QString &input, int &pos) const
    {
        int i;
        RCCHAR string[300],*eptr;
        double x;
        State state;

        strcpy(string,input.data());

        for (i = 0 ; DefaultPlotScales[i].label != 0 ; i++)
        {
            if (_stricmp(string,DefaultPlotScales[i].label.data()) == 0)
            {
                return Acceptable;
            }
        }

        if (DefaultPlotScales[i].label == 0)
        {
            ResourceString rs1(NCDIALOG+1);
            ResourceString rs2(NCDIALOG+2);
            /*
           if (string[0] == '1' && string[1] == ':')
             x = strtod(string+2,&eptr);
           else
             x = strtod(string,&eptr);


           if (*eptr != 0)
           {
                cadwindow->MessageBox(rs1.gets(),"",MB_ICONSTOP);
                return Invalid;
           }
           if (x < 1.0E-10 || x > 1.0E10)
           {
                //cadwindow->MessageBox(rs2.gets(),"",MB_ICONSTOP);
                return Invalid;
           }
           */
            bool replacePercent = false;
            bool replaceSemiColon = false;
            if (input.endsWith(QLatin1Char('%'))) {
                input = input.left(input.length() - 1);
                replacePercent = true;
            }
            if (input.endsWith(QLatin1Char(':'))) {
                input = input.left(input.length() - 1);
                replaceSemiColon = true;
            }
            state = QDoubleValidator::validate(input, pos);
            if (replacePercent)
                input += QLatin1Char('%');
            if (replaceSemiColon)
            {
                input += QLatin1Char(':');
                return Acceptable;
            }
            const int num_size = 4;
            if (state == Intermediate) {
                int i = input.indexOf(QLocale::system().decimalPoint());
                if ((i == -1 && input.size() > num_size)
                    || (i != -1 && i > num_size))
                    return Invalid;
            }
        }
        return state;
    }
};

static inline void RC_setupActionIcon(QAction *action, QLatin1String name)
{

    QLatin1String imagePrefix(":/images/");
    QIcon icon;
    icon.addFile(imagePrefix + name + QLatin1String(".svg"), QSize(32, 32));
    action->setIcon(icon);

}

RCPrintPreview_Dialog::RCPrintPreview_Dialog(QPrinter *printer, QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent,flags), mPrintDialog(0), mOwnPrinter(false)
{
    mPdfRes=0;
    // create the print preview widget
    if(printer && printer->isValid())
    {
        qDebug() << "printer name : " << printer->printerName();
        qDebug() << "printer output format : " << printer->outputFormat();
        qDebug() << "printer orientation :" << printer->orientation();
        mOwnPrinter = false;
        mPrinter = printer;
        defaultPrinterRes = mPrinter->resolution();
        defaultPrintScale = v.getreal("pp::printscale");
        mPreview = new QPrintPreviewWidget(mPrinter, this);
        qDebug() << "preview orientation :" << mPreview->orientation();
        qDebug() << "printer orientation :" << printer->orientation();
    }
    else
    {
        qDebug() << "printer name : " << printer->printerName();
        qDebug() << "printer output format : " << printer->outputFormat();
        qDebug() << "printer orientation :" << printer->orientation();
        mOwnPrinter = true;
        mPrinter = new QPrinter;
        mPreview = new QPrintPreviewWidget(mPrinter, this);
        qDebug() << "preview orientation :" << mPreview->orientation();
        qDebug() << "printer orientation :" << printer->orientation();
    }
    //QObject::connect(mPreview, SIGNAL(paintRequested(QPrinter*)), this, SIGNAL(paintRequested(QPrinter*)));
    QObject::connect(mPreview, SIGNAL(paintRequested(QPrinter*)), this , SIGNAL(paintRequested(QPrinter*)));
    QObject::connect(mPreview, SIGNAL(previewChanged()), this, SLOT(previewChanged()));
    qDebug() << "printer orientation :" << mPrinter->orientation();

    // create the main window
    mMainWindow = new RCPrintPreviewMainWindow(this);
    // QMainWindows are always created as top levels, force it to be a
    // plain widget
    mMainWindow->setParent(this, Qt::Widget);
    mToolbar = new QToolBar(mMainWindow);
    mToolbar->setObjectName("PrintPreviewToolBar");

    qDebug() << "printer orientation :" << mPrinter->orientation();

    // create the pdf resolution combo box
#if MAJOR_VERSION > 7
    QStringList pdfResItems;
    pdfResItems << "300" << "600" << "1200" << "2400";
    mPdfRes = new QComboBox(this);
    mPdfRes->setEditable(false);
    //mPdfRes->setMaxCount(10);
    mPdfRes->addItems(pdfResItems);
    mPdfRes->setMaxVisibleItems(10);
    if(0 && v.getinteger("pr::pdfresolution") >= 0)
        mPdfRes->setCurrentIndex(v.getinteger("pr::pdfresolution"));
    else
        mPdfRes->setCurrentIndex(2);
    //
    mPdfRes->setItemData(0,300);
    mPdfRes->setItemData(1,600);
    mPdfRes->setItemData(2,1200);
    mPdfRes->setItemData(3,2400);
    //
    QObject::connect(mPdfRes,SIGNAL(currentIndexChanged(int)),this,SLOT(pdfResChanged(int)));
#else
    mPdfRes=0;
#endif
    setupActions(mToolbar);
    qDebug() << "printer orientation :" << mPrinter->orientation();
    mToolbar->addAction(mPrintAction);
    mToolbar->addAction(mPrintSetupAction);
    mToolbar->addAction(mPageSetupAction);
#if MAJOR_VERSION > 7
    mToolbar->addSeparator();
    mToolbar->addAction(mPrintPdfAction);
    mToolbar->addWidget(mPdfRes);
    mToolbar->addSeparator();
#else
    mToolbar->addAction(mPrintPdfAction);
    mToolbar->addSeparator();
#endif
    mToolbar->addAction(mColourAction);
    mToolbar->addAction(mBlackWhiteAction);
    mToolbar->addAction(mGreyScaleAction);
    mToolbar->addSeparator();
    mToolbar->addAction(mFitPageAction);
    mToolbar->addAction(mFitWidthAction);
    mToolbar->addAction(mZoomInAction);
    mToolbar->addAction(mZoomOutAction);
    mToolbar->addSeparator();
    mToolbar->addAction(mPortraitAction);
    mToolbar->addAction(mLandscapeAction);
    mToolbar->addSeparator();

    qDebug() << "printer orientation :" << mPrinter->orientation();
    mPaperTypes = new QComboBox(this);
    mPaperTypes->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    mToolbar->addWidget(mPaperTypes);
    QPrinterInfo printerInfo(*mPrinter);
    QList<QPageSize> pageSizes = printerInfo.supportedPageSizes();
    if(pageSizes.size() == 0)
    {
        for(int i=0; i<View2d::nsizes; i++)
        {
            if(View2d::heights[i] > View2d::widths[i])
                mPaperTypes->addItem(View2d::sizes[i],QSizeF(View2d::widths[i],View2d::heights[i]));
            else
                mPaperTypes->addItem(View2d::sizes[i],QSizeF(View2d::heights[i],View2d::widths[i]));
        }
    }
    else
    {
        for(int i=0; i<pageSizes.size(); i++)
            mPaperTypes->addItem(pageSizes.at(i).name(),pageSizes.at(i).size(QPageSize::Millimeter));
        mPaperTypes->addItem("Custom");
    }
    qDebug() << "printer orientation :" << mPrinter->orientation();
    int index = mPaperTypes->findText("Custom");
    QSizeF size;
    if(v.getreal("cd::nonstdheight") > v.getreal("cd::nonstdwidth"))
        size = QSizeF(v.getreal("cd::nonstdwidth"),v.getreal("cd::nonstdheight"));
    else
        size = QSizeF(v.getreal("cd::nonstdheight"),v.getreal("cd::nonstdwidth"));
    QString customName = QString("Custom (%1mm x %2mm)").arg(size.width()).arg(size.height());
    mPaperTypes->setItemText(index,customName);
    mPaperTypes->setItemData(index,size);
    qDebug() << "init paper name : " << mPrinter->paperName();
    qDebug() << "pp::papername : " << QString(v.getstring("pp::papername"));
    //mPaperTypes->setCurrentText("A4");
    qDebug() << "printer orientation :" << mPrinter->orientation();
    mPaperTypes->setCurrentText(mPrinter->paperName());
    qDebug() << "printer orientation :" << mPrinter->orientation();
    //QObject::connect(mPaperTypes,SIGNAL(activated(const QString&)),this,SLOT(setPaperType(const QString&)));
    QObject::connect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));

    qDebug() << "printer orientation :" << mPrinter->orientation();

    mPrintScale = new QComboBox(this);
    mPrintScale->setEditable(true);
    // add the special Fit plot scale
    mPrintScale->addItem("Fit to Paper");
    // using the RealCAD default plot scale table
    for (int i = 0 ; DefaultPlotScales[i].label != 0 ; i++)
        mPrintScale->addItem(DefaultPlotScales[i].label);

    qDebug() << "printer orientation :" << mPrinter->orientation();

    mToolbar->addWidget(mPrintScale);
    QObject::connect(mPrintScale,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPrintScale(const QString&)));
    QObject::connect(mPrintScale,SIGNAL(editTextChanged(const QString&)),this,SLOT(printScaleEdited(const QString&)));
    qDebug() << "printer orientation :" << mPrinter->orientation();

#if MAJOR_VERSION > 7
    // v8 enhancement p1 option for reverse printing of 100% white colours when the screen background is black
    mToolbar->addSeparator();

    // tri state check box with an icon
    /*
    mReverseWhite = new QCheckBox(this);
    mReverseWhite->setTristate(true);
    mReverseWhite->setIcon(QIcon(QLatin1String("://images/white-to-black-icon.svg")));
    mReverseWhite->setToolTip("<b>Invert White</b><hr>Prints white lines as black");
    reverseBWTriggered(v.getinteger("pl::reverseblackwhite"));
    connect(mReverseWhite,SIGNAL(stateChanged(int)),this,SLOT(reverseBWTriggered(int)));
    mToolbar->addWidget(mReverseWhite);
    mToolbar->addSeparator();
    */

    // checkbox replaced by an action button
    mReverseWhiteAction = new QAction("Invert White",mToolbar);
    mReverseWhiteAction->setToolTip("<b>Invert White</b><hr>Prints white lines as black");
    RC_setupActionIcon(mReverseWhiteAction, QLatin1String("white-to-black-icon"));
    mReverseWhiteAction->setCheckable(true);
    connect(mReverseWhiteAction,SIGNAL(triggered(bool)),this,SLOT(reverseBWTriggered(bool)));
    mReverseWhiteAction->setChecked(v.getinteger("pl::reverseblackwhite"));
    mToolbar->addAction(mReverseWhiteAction);
    mToolbar->addSeparator();

    // v8 enhacement p3 proved a check box for using the last printer
    mUseLastPrinter = new QCheckBox(this);
    mUseLastPrinter->setText("Use last printer");
    mUseLastPrinter->setChecked(v.getinteger("pr::uselastprinter"));
    connect(mUseLastPrinter,SIGNAL(stateChanged(int)),this,SLOT(useLastPrinterTriggered(int)));
    mToolbar->addWidget(mUseLastPrinter);

    // v8 enhancement: add a check box to keep the dialog open after printing
    mkeepPreviewOpen = new QCheckBox(this);
    mkeepPreviewOpen->setText("Keep preview open");
    mkeepPreviewOpen->setChecked(v.getinteger("pr::uselastprinter"));
    connect(mkeepPreviewOpen,SIGNAL(stateChanged(int)),this,SLOT(keepOpenTriggered(int)));
    mToolbar->addWidget(mkeepPreviewOpen);

    mToolbar->addSeparator();
#endif

    mMainWindow->addToolBar(mToolbar);

    mMainWindow->setCentralWidget(mPreview);

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(mMainWindow);
    topLayout->setMargin(0);
    this->setLayout(topLayout);

    qDebug() << "printer orientation :" << mPrinter->orientation();
    // set the initial format
    // assuming pdf is not the default format
    lastPdfState = false;
#if MAJOR_VERSION > 7
    mPdfRes->setEnabled(false);
#endif
    if (!mPrinter->isValid() || mPrinter->outputFormat() != QPrinter::NativeFormat)
    {
        lastPdfState = true;
        mPrintPdfAction->setChecked(true);
#if MAJOR_VERSION > 7
        mPdfRes->setEnabled(true);
        //mPrinter->setResolution(getPdfRes());
        //v.setreal("pp::printscale",defaultPrintScale*(((double)mPrinter->resolution())/((double)defaultPrinterRes)));
#endif
        mPageSetupAction->setEnabled(false);
        mPrintSetupAction->setEnabled(false);
        mPrinter->setOutputFileName(QString(db.gettitle())+QString(_RCT(".pdf")));
    }

    qDebug() << "printer orientation :" << mPrinter->orientation();
    // set the initial colour
    if(v.getinteger("pl::blackandwhite") == 0)
    {
        mColourAction->setChecked(true);
        mPrinter->setColorMode(QPrinter::Color);
    }
    else if(v.getinteger("pl::blackandwhite") == 1)
    {
        mBlackWhiteAction->setChecked(true);
        mPrinter->setColorMode(QPrinter::GrayScale);
    }
    else if(v.getinteger("pl::blackandwhite") == 2)
    {
        mGreyScaleAction->setChecked(true);
        mPrinter->setColorMode(QPrinter::GrayScale);
    }

    // set the initial paper scale
    double printscale = v.getreal("pr::printscale");
    if(fabs(printscale) < 0.00001)
    {
        v.setstring("pr::scalename","Fit to Paper");
        // diff to 7.5 added at 7.6
        //v.setreal("pr::printscale",1.0); // 7.6
        mPrintScale->setCurrentIndex(0);
    }
    else
    {
        QString scalename(MatchPlotScale(printscale));
        mPrintScale->setCurrentText(scalename);
    }

    // set initial paper name and size
    // assuming that PDF mode is not the default
    qDebug() << "printer orientation :" << mPrinter->orientation();
    QString currentPaperName(v.getstring("pp::papername"));
    if(currentPaperName.contains("V"))
        currentPaperName.remove("V");
    index = mPaperTypes->findText(currentPaperName,Qt::MatchContains);
    if(index < 0) {
        if (!lastPdfState)
        {
            QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
            mPrintPdfAction->setChecked(true);
            mPrintSetupAction->setEnabled(false);
            mPageSetupAction->setEnabled(false);
            QPageSize lastPageSize = mPrinter->pageLayout().pageSize();
            QPageLayout::Orientation lastOrientation = mPrinter->pageLayout().orientation();
            mPrinter->setOutputFormat(QPrinter::PdfFormat);
    #if MAJOR_VERSION > 7
            mPdfRes->setEnabled(true);
            if(v.getinteger("pr::pdfresolution") >= 0)
                mPdfRes->setCurrentIndex(v.getinteger("pr::pdfresolution"));
            else
                mPdfRes->setCurrentIndex(2);
            //mPrinter->setResolution(getPdfRes());
            settings.setValue("Strings/pr::lastprinter","InternalPDF");
    #else
            mPrinter->setResolution(1200);
    #endif
            if(!mPrinter->setPageSize(lastPageSize))
            {
                qDebug() << "printer paper name" << mPrinter->paperName();
            }
            mPrinter->setPageOrientation(lastOrientation);
            mPrinter->setOutputFileName(QString(db.gettitle())+QString(_RCT(".pdf")));
            lastPdfState = true;
            qDebug() << "pagesize paper name" << lastPageSize.name();
            qDebug() << "printer page name" << mPrinter->pageLayout().pageSize().name();

            QObject::disconnect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));

            mPaperTypes->clear();
            for(int i=0; i<View2d::nsizes; i++)
            {
                if(View2d::heights[i] > View2d::widths[i])
                    mPaperTypes->addItem(View2d::sizes[i],QSizeF(View2d::widths[i],View2d::heights[i]));
                else
                    mPaperTypes->addItem(View2d::sizes[i],QSizeF(View2d::heights[i],View2d::widths[i]));
            }

            QObject::connect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));
        }
        //index = mPaperTypes->findText("A4",Qt::MatchContains);
    }
    index = mPaperTypes->findText(currentPaperName,Qt::MatchContains);
    if(index < 0) {
        index = mPaperTypes->findText("A4",Qt::MatchContains);
    }
    qDebug() << "current text : " << mPaperTypes->itemText(index);
    //mPaperTypes->setCurrentIndex(index);
    mPaperTypes->setCurrentText(mPaperTypes->itemText(index));
    setPaperType(mPaperTypes->itemText(index));
    qDebug() << "printer paper name :" << mPrinter->paperName();
    qDebug() << "mPaperTypes current : " << mPaperTypes->currentText();
    mPrinter->setPaperName(mPaperTypes->currentText());
    qDebug() << "printer paper name :" << mPrinter->paperName();
    qDebug() << "printer orientation :" << mPrinter->orientation();

    // set initial paper orientation
    //mPreview->setOrientation(mPrinter->orientation());
    if(QString(v.getstring("pp::orientation")) == "portrait")
        setPortraitOrientation();
    else
        setLandscapeOrientation();
    mPreview->fitInView();
    //QTimer::singleShot(200, this, SLOT(fitInView()));

#if MAJOR_VERSION > 7
    // v8 enhancement p1 option for reverse printing of 100% white colours when the screen background is black
    //mReverseWhite->setCheckState((Qt::CheckState)v.getinteger("pl::reverseblackwhite"));
    mReverseWhiteAction->setChecked(v.getinteger("pl::reverseblackwhite"));
#endif

    //v.setinteger("pr::patternfilllod",8);
}

RCPrintPreview_Dialog::~RCPrintPreview_Dialog()
{
    if (mOwnPrinter)
        delete mPrinter;
}

void RCPrintPreview_Dialog::setupActions(QWidget *parent)
{
    // Print actions
    mPrinterGroup = new QActionGroup(parent);
    mPrintAction = new QAction(mPrinterGroup);
    //mPrintAction = mPrinterGroup->addAction("File Print");
    mPrintSetupAction = mPrinterGroup->addAction("Printer Setup");
    mPageSetupAction = mPrinterGroup->addAction("Paper Setup");
    mPrintPdfAction = mPrinterGroup->addAction("PDF mode");
    mPrintPdfAction->setCheckable(true);
    bool lastPdfState = false;

    mPrintAction->setToolTip(QString("<b>File Print</b><hr>Send the preview to the current printer"));
    mPrintSetupAction->setToolTip("<b>Printer Setup</b><hr>Set or change the printer and properties");
    mPageSetupAction->setToolTip("<b>Paper Setup</b><hr>Set or change the sheet properties");
    mPrintPdfAction->setToolTip("<b>Switch to PDF mode</b><hr>Click the Print button to save the PDF file");

    RC_setupActionIcon(mPrintAction, QLatin1String("print"));
    RC_setupActionIcon(mPrintSetupAction, QLatin1String("PRINT-printer-setup"));
    RC_setupActionIcon(mPageSetupAction, QLatin1String("PRINT-paper-setup"));
    RC_setupActionIcon(mPrintPdfAction, QLatin1String("PRINT-output-to-pdf"));
    QObject::connect(mPrintAction, SIGNAL(triggered(bool)), this, SLOT(print()));
    QObject::connect(mPrintSetupAction, SIGNAL(triggered(bool)), this, SLOT(printSetup()));
    QObject::connect(mPageSetupAction, SIGNAL(triggered(bool)), this, SLOT(pageSetup()));
    //QObject::connect(mPrintPdfAction, SIGNAL(triggered(bool)), this, SLOT(printToPdf()));
    //QObject::connect(mPrintPdfAction, SIGNAL(toggled(bool)), this, SLOT(pdfToggled(bool)));
    QObject::connect(mPrintPdfAction, SIGNAL(triggered(bool)), this, SLOT(pdfTriggered(bool)));

    // Portrait/Landscape
    mOrientationGroup = new QActionGroup(parent);
    mOrientationGroup->setExclusive(true);
    mPortraitAction = mOrientationGroup->addAction(QApplication::translate("RCPrintPreviewDialog", "Portrait"));
    mLandscapeAction = mOrientationGroup->addAction(QApplication::translate("RCPrintPreviewDialog", "Landscape"));

    mPortraitAction->setToolTip("<b>Portrait</b><hr>Print in vertical orientation");
    mLandscapeAction->setToolTip("<b>Landscape</b><hr>Print in horizontal orientation");

    mPortraitAction->setCheckable(true);
    mLandscapeAction->setCheckable(true);
    RC_setupActionIcon(mPortraitAction, QLatin1String("PRINT-portrait"));
    RC_setupActionIcon(mLandscapeAction, QLatin1String("PRINT-landscape"));
    QObject::connect(mPortraitAction, SIGNAL(triggered(bool)), this, SLOT(setPortraitOrientation()));
    QObject::connect(mLandscapeAction, SIGNAL(triggered(bool)), this, SLOT(setLandscapeOrientation()));

    // preview page zoom
    mPageZoomGroup = new QActionGroup(parent);
    mFitPageAction = mPageZoomGroup->addAction(QApplication::translate("RCPrintPreviewDialog", "<b>Fit Page</b><hr>Show the entire page on screen"));
    mFitWidthAction = mPageZoomGroup->addAction(QApplication::translate("RCPrintPreviewDialog", "<b>Fit Page Width</b><hr>Fit page width to the screen"));
    mZoomInAction = mPageZoomGroup->addAction(QApplication::translate("RCPrintPreviewDialog", "<b>Zoom Page in</b><hr>Zoom in to see more detail"));
    mZoomOutAction = mPageZoomGroup->addAction(QApplication::translate("RCPrintPreviewDialog", "<b>Zoom Page Out</b><hr>Zoom out to see more of the drawing"));
    RC_setupActionIcon(mFitPageAction, QLatin1String("PRINT-fit-page"));
    RC_setupActionIcon(mFitWidthAction, QLatin1String("PRINT-fit-page-width"));
    RC_setupActionIcon(mZoomInAction, QLatin1String("PRINT-zoom-page-in"));
    RC_setupActionIcon(mZoomOutAction, QLatin1String("PRINT-zoom-page-out"));
    QObject::connect(mFitPageAction, SIGNAL(triggered(bool)), this, SLOT(FitPage()));
    QObject::connect(mFitWidthAction, SIGNAL(triggered(bool)), this, SLOT(FitPageWidth()));
    QObject::connect(mZoomInAction, SIGNAL(triggered(bool)), this, SLOT(ZoomPageIn()));
    QObject::connect(mZoomOutAction, SIGNAL(triggered(bool)), this, SLOT(ZoomPageOut()));

    // colour settings
    mColourGroup = new QActionGroup(parent);
    mColourGroup->setExclusive(true);
    mBlackWhiteAction = mColourGroup->addAction(QApplication::translate("RCPrintPreviewDialog", "<b>Print B&W</b><hr>Print in Black and White"));
    mGreyScaleAction = mColourGroup->addAction(QApplication::translate("RCPrintPreviewDialog", "<b>Print Grey Scale</b><hr>Print colours as shades of grey"));
    mColourAction = mColourGroup->addAction(QApplication::translate("RCPrintPreviewDialog", "<b>Print Colour</b><hr>Print in full colour"));
    mBlackWhiteAction->setCheckable(true);
    mGreyScaleAction->setCheckable(true);
    mColourAction->setCheckable(true);
    RC_setupActionIcon(mBlackWhiteAction, QLatin1String("PRINT-print-black-and-white"));
    RC_setupActionIcon(mGreyScaleAction, QLatin1String("PRINT-print-grey-scale"));
    RC_setupActionIcon(mColourAction, QLatin1String("PRINT-print-colour"));
    QObject::connect(mBlackWhiteAction, SIGNAL(triggered(bool)), this, SLOT(setPrintBlackWhite()));
    QObject::connect(mGreyScaleAction, SIGNAL(triggered(bool)), this, SLOT(setPrintGreyScale()));
    QObject::connect(mColourAction, SIGNAL(triggered(bool)), this, SLOT(setPrintColour()));

    // Initial state:
    v.setinteger("pl::blackandwhite",0);
    mColourAction->setChecked(true);
    mPreview->setOrientation(mPrinter->orientation());
    mLandscapeAction->setChecked((mPrinter->orientation()==QPrinter::Portrait) ? QPrinter::Portrait: QPrinter::Landscape);
    mPreview->fitInView();
}

void RCPrintPreview_Dialog::pageSetup()
{
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    //mPrinter->setOrientation(mPreview->orientation());
    QPageSetupDialog pageSetup(mPrinter,this);
    if (pageSetup.exec() == QDialog::Accepted)
    {
        // reload the page sizes just incase the user created some more
        // we only come here with native printers
        qDebug() << "pageSetup : " << pageSetup.printer()->printerName();
        QPrinterInfo printerInfo(*mPrinter);
        qDebug() << "last printer : " << mPrinter->printerName();
        settings.setValue("Strings/pr::lastprinter",mPrinter->printerName());
        //QPageSize lastPageSize = mPrinter->pageLayout().pageSize();
        QList<QPageSize> pageSizes = printerInfo.supportedPageSizes();
        //QObject::disconnect(mPaperTypes,SIGNAL(activated(const QString&)),this,SLOT(setPaperType(const QString&)));
        QObject::disconnect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));
        mPaperTypes->clear();
        for(int i=0; i<pageSizes.size(); i++)
            mPaperTypes->addItem(pageSizes.at(i).name(),pageSizes.at(i).size(QPageSize::Millimeter));
//#ifdef _MAC
        mPaperTypes->addItem("Custom",QSizeF(View2d::heights[View2d::nsizes-1],View2d::widths[View2d::nsizes-1]));
//#endif
        //QObject::connect(mPaperTypes,SIGNAL(activated(const QString&)),this,SLOT(setPaperType(const QString&)));
        QObject::connect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));
        //mPrinter->setPageSize(mPrinter->pageLayout().pageSize());
        //mPaperTypes->setCurrentText(mPrinter->pageLayout().pageSize().name());

        // update possible orientation changes
        qDebug() << "pagesetup " << pageSetup.printer()->orientation();
        qDebug() << "page layout orientation" << mPrinter->pageLayout().orientation();
        qDebug() << "printer orientation" << mPrinter->orientation();
        qDebug() << "page layout paperName" << mPrinter->pageLayout().pageSize().name();
        qDebug() << "paperName" << mPrinter->paperName();
        qDebug() << "paperRect" << mPrinter->paperRect();
        qDebug() << "paperSize" << mPrinter->paperSize();
        qDebug() << "pageRect" << mPrinter->pageRect();
        qDebug() << "pageSizeMM" << mPrinter->pageSizeMM();
        qDebug() << "pageRect" << mPrinter->pageRect();
        qDebug() << "pageSize" << mPrinter->pageSize();
        qDebug() << "page Width" << mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width();
        qDebug() << "page height" << mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height();
        qDebug() << "psgesize id :" << mPrinter->pageLayout().pageSize().id();

        v.setreal("pp:paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
        v.setreal("pp:paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
        v.setstring("pp:papername",mPrinter->paperName().data());

        if(mPrinter->pageLayout().pageSize().id() == QPageSize::Custom)
        {
            v.setreal("cd::nonstdwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
            v.setreal("cd::nonstdheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());

            bool found=false;
            for(int i=0; i<pageSizes.size(); i++)
            {
                qDebug() << "current pageSize : " << pageSizes[i].size(QPageSize::Millimeter);
                qDebug() << "test pageSize    : " << mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter);
                if((fabs(pageSizes[i].size(QPageSize::Millimeter).width() - mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width()) < 1.0)
                   &&
                   (fabs(pageSizes[i].size(QPageSize::Millimeter).height() - mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height()) < 1.0))
                {
                    //mPrinter->setPaperName(pageSizes[i].name());
                    mPrinter->setPageSize(pageSizes[i]);
                    found=true;
                    break;
                }
            }
            if(!found)
            {
                qDebug() << "size : " << mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter);
                //mPrinter->setPaperName(QString("Custom (%1mm x %2mm)").arg(v.getreal("cd::nonstdwidth")).arg(v.getreal("cd::nonstdheight")));
                //QString customName = QString("Custom (%1mm x %2mm)").arg(v.getreal("cd::nonstdwidth")).arg(v.getreal("cd::nonstdheight"));
                QPageSize pageSize(mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter),QPageSize::Millimeter);
                mPrinter->setPageSize(pageSize);
                //mPrinter->setPaperName(customName);
                int index = mPaperTypes->findText("Custom",Qt::MatchContains);
                //mPaperTypes->setItemText(index,customName);
                mPaperTypes->setItemText(index,mPrinter->pageLayout().pageSize().name());
                mPaperTypes->setItemData(index,pageSize.size(QPageSize::Millimeter));
                //mPaperTypes->setCurrentIndex(index);
                //mPaperTypes->setCurrentText(customName);
                mPaperTypes->setCurrentText(mPrinter->pageLayout().pageSize().name());
            }
            else
                mPaperTypes->setCurrentText(mPrinter->pageLayout().pageSize().name());

            if(v.getreal("cd::nonstdheight") > v.getreal("cd::nonstdwidth"))
                mPrinter->setPageOrientation(QPageLayout::Portrait);
            else
                mPrinter->setPageOrientation(QPageLayout::Landscape);
            v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());
        }
        else
            mPaperTypes->setCurrentText(mPrinter->pageLayout().pageSize().name());


        if (mPreview->orientation() == QPrinter::Portrait)
            setPortraitOrientation();
        else
            setLandscapeOrientation();

    }
}

void RCPrintPreview_Dialog::previewChanged()
{

    if (mPreview->orientation() == QPrinter::Portrait)
        mPortraitAction->setChecked(true);
    else
        mLandscapeAction->setChecked(true);

}

void RCPrintPreview_Dialog::setPortraitOrientation()
{
    //mPrinter->setOrientation(QPrinter::Portrait);
    v.setstring("pp::orientation","portrait");
    v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
    v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
    v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());

    mPrinter->setPageOrientation(QPageLayout::Portrait);
    mPreview->setPortraitOrientation();

    if(mPrinter->outputFormat() == QPrinter::PdfFormat)
    {
        //QObject::disconnect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));

        int index = mPaperTypes->currentIndex();
        if(index < View2d::nsizes -1 && !mPaperTypes->itemText(index).contains("V"))
        {
            index++;
            mPaperTypes->setCurrentIndex(index);
            v.setstring("pp::papername",mPaperTypes->currentText().data());
        }

        //QObject::connect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));

    }
}

void RCPrintPreview_Dialog::setLandscapeOrientation()
{
    //mPrinter->setOrientation(QPrinter::Landscape);
    v.setstring("pp::orientation","landscape");
    v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
    v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
    v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());

    mPrinter->setPageOrientation(QPageLayout::Landscape);
    mPreview->setLandscapeOrientation();

    if(mPrinter->outputFormat() == QPrinter::PdfFormat)
    {
        //QObject::disconnect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));

        int index = mPaperTypes->currentIndex();
        if(index < View2d::nsizes -1 && mPaperTypes->itemText(index).contains("V"))
        {
            index--;
            mPaperTypes->setCurrentIndex(index);
            v.setstring("pp::papername",mPaperTypes->currentText().data());
        }

        //QObject::connect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));
    }
}

void RCPrintPreview_Dialog::printSetup()
{
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    if (mPrinter->outputFormat() != QPrinter::NativeFormat) {
        QString title = QCoreApplication::translate("RCPrintPreview_Dialog", "Save to PDF");
        QString suffix = QLatin1String(".pdf");
        QString fileName = QFileDialog::getSaveFileName(this, title, mPrinter->outputFileName(),
                                                        QLatin1Char('*') + suffix);
        if (!fileName.isEmpty()) {
            if (QFileInfo(fileName).suffix().isEmpty())
                fileName.append(suffix);
            mPrinter->setOutputFileName(fileName);
        }

        if(mColourAction->isChecked())
            mPrinter->setColorMode(QPrinter::Color);
        else if(mBlackWhiteAction->isChecked())
            mPrinter->setColorMode(QPrinter::GrayScale);
        else if(mGreyScaleAction->isChecked())
            mPrinter->setColorMode(QPrinter::GrayScale);
        mPaperTypes->setCurrentText(mPrinter->paperName());
        if (mPreview->orientation() == QPrinter::Portrait) {
            setPortraitOrientation();
        }else {
            setLandscapeOrientation();
        }

        if (!mPrinter->outputFileName().isEmpty())
            mPreview->print();

        // Enhancement for v8: don't close the dialog after printing
#if MAJOR_VERSION < 8
        accept();
#endif
    if(mkeepPreviewOpen->checkState() == 0)
        accept();
    else
        return;
    }
#endif

    if(mColourAction->isChecked())
        mPrinter->setColorMode(QPrinter::Color);
    else if(mBlackWhiteAction->isChecked())
        mPrinter->setColorMode(QPrinter::GrayScale);
    else if(mGreyScaleAction->isChecked())
        mPrinter->setColorMode(QPrinter::GrayScale);

    if (!mPrintDialog)
        mPrintDialog = new QPrintDialog(mPrinter, this);
    if (mPrintDialog->exec() == QDialog::Accepted)
    {
        mPaperTypes->setCurrentText(mPrinter->paperName());
        if (mPreview->orientation() == QPrinter::Portrait)
        {
            setPortraitOrientation();
        }else {
            setLandscapeOrientation();
        }

        mPreview->print();
        // Enhancement for v8: don't close the dialog after printing
#if MAJOR_VERSION < 8
        accept();
#else
    if(mkeepPreviewOpen->checkState() == 0)
        accept();
#endif
    }
    qDebug() << "last printer : " << mPrinter->printerName();
    settings.setValue("Strings/pr::lastprinter",mPrinter->printerName());
}

void RCPrintPreview_Dialog::printToPdf()
{
    qDebug() << "paper name : " << mPrinter->paperName();
    qDebug() << "resolution : " << mPrinter->resolution();

    QPrinter::OutputFormat lastFormat = mPrinter->outputFormat();
    QString lastPrinterName = mPrinter->printerName();
    QPageSize lastPageSize = mPrinter->pageLayout().pageSize();

    mPrinter->setOutputFormat(QPrinter::PdfFormat);
    mPrinter->setPageSize(lastPageSize);
    mPrinter->setOutputFileName(QString(db.gettitle())+QString(_RCT(".pdf")));

    print();

    mPrinter->setOutputFormat(lastFormat);
    mPrinter->setPageSize(lastPageSize);
    mPrinter->setPrinterName(lastPrinterName);
    mPrinter->setOutputFileName(QString());
    qDebug() << "paper name : " << mPrinter->paperName();
}

void RCPrintPreview_Dialog::pdfTriggered(bool checked)
{
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

    if(!lastPdfState)
    {
        mPrintPdfAction->setChecked(true);
        mPrintSetupAction->setEnabled(false);
        mPageSetupAction->setEnabled(false);
        QPageSize lastPageSize = mPrinter->pageLayout().pageSize();
        QPageLayout::Orientation lastOrientation = mPrinter->pageLayout().orientation();
        mPrinter->setOutputFormat(QPrinter::PdfFormat);
#if MAJOR_VERSION > 7
        mPdfRes->setEnabled(true);
        //mPrinter->setResolution(getPdfRes());
        settings.setValue("Strings/pr::lastprinter","InternalPDF");
        //v.setreal("pp::printscale",defaultPrintScale*(((double)mPrinter->resolution())/((double)defaultPrinterRes)));
#else
        mPrinter->setResolution(1200);
#endif
        if(!mPrinter->setPageSize(lastPageSize))
        {
            qDebug() << "printer paper name" << mPrinter->paperName();
        }
        mPrinter->setPageOrientation(lastOrientation);
        mPrinter->setOutputFileName(QString(db.gettitle())+QString(_RCT(".pdf")));
        lastPdfState = true;
        qDebug() << "pagesize paper name" << lastPageSize.name();
        qDebug() << "printer page name" << mPrinter->pageLayout().pageSize().name();

        QObject::disconnect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));

        mPaperTypes->clear();
        for(int i=0; i<View2d::nsizes; i++)
        {
            if(View2d::heights[i] > View2d::widths[i])
                mPaperTypes->addItem(View2d::sizes[i],QSizeF(View2d::widths[i],View2d::heights[i]));
            else
                mPaperTypes->addItem(View2d::sizes[i],QSizeF(View2d::heights[i],View2d::widths[i]));
        }

        QObject::connect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));
        // testing
        int index = mPaperTypes->findText("Custom");
        QSizeF size;
        if(v.getreal("cd::nonstdheight") > v.getreal("cd::nonstdwidth"))
            size = QSizeF(v.getreal("cd::nonstdwidth"),v.getreal("cd::nonstdheight"));
        else
            size = QSizeF(v.getreal("cd::nonstdheight"),v.getreal("cd::nonstdwidth"));
        QString customName = QString("Custom (%1mm x %2mm)").arg(size.width()).arg(size.height());
        mPaperTypes->setItemText(index,customName);
        mPaperTypes->setItemData(index,size);
        //
        mPrinter->setPageOrientation(lastOrientation);
        mPrinter->setPageSize(lastPageSize);
        qDebug() << "printer paper name" << mPrinter->pageLayout().pageSize().name();
        qDebug() << "printer page orientation" << mPrinter->pageLayout().orientation();
        qDebug() << "printer page size" << mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter);
        QString sizeName(mPrinter->paperName());
        if(lastOrientation==QPageLayout::Portrait)
            sizeName += "V";
        index = mPaperTypes->findText(sizeName);
        //index = mPaperTypes->findText(QString(mPrinter->paperName()));
        if(index >= 0)
        {
            if(mPrinter->orientation() == QPrinter::Portrait)
            {
                v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());
            }
            else
            {
                v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());
            }
            qDebug() << "pp::paperwidth" << v.getreal("pp::paperwidth");
            qDebug() << "pp::paperheight" << v.getreal("pp::paperheight");
            qDebug() << "pp::papername" <<  QString(v.getstring("pp::papername"));

            //mPaperTypes->setItemText(index,mPrinter->pageLayout().pageSize().name());
            mPaperTypes->setItemText(index,sizeName);
            mPaperTypes->setItemData(index,mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter));
            mPaperTypes->setCurrentIndex(index);
        }
        else
        {
            index = View2d::matchPaperName(mPrinter->paperName().data());
            if(index >= 0)
            {
                if(mPrinter->pageLayout().orientation() == QPageLayout::Portrait)
                    index++;
                if(index >= View2d::nsizes)
                    index = View2d::nsizes-1;

                if(index == View2d::nsizes-1)
                {
                    //v.setreal("cd::nonstdwidth",View2d::widths[index]);
                    //v.setreal("cd::nonstdheight",View2d::heights[index]);
                    if(mPrinter->orientation() == QPrinter::Portrait)
                    {
                        v.setreal("cd::nonstdwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                        v.setreal("cd::nonstdheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                        v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                        v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                        v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());
                    }
                    else
                    {
                        v.setreal("cd::nonstdwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                        v.setreal("cd::nonstdheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                        v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                        v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                        v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());
                    }

                    mPaperTypes->setItemText(index,mPrinter->pageLayout().pageSize().name());
                    mPaperTypes->setItemData(index,mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter));
                    mPaperTypes->setCurrentIndex(index);
                }
                else
                {
                    //v.setreal("pp::paperwidth",View2d::widths[index]);
                    //v.setreal("pp::paperheight",View2d::heights[index]);
                    if(mPrinter->orientation() == QPrinter::Portrait)
                    {
                        v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                        v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                        v.setstring("pp::papername",View2d::sizes[index].data());
                    }
                    else
                    {
                        v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                        v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                        v.setstring("pp::papername",View2d::sizes[index].data());
                    }

                    mPaperTypes->setItemText(index,View2d::sizes[index]);
                    mPaperTypes->setItemData(index,mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter));
                    mPaperTypes->setCurrentIndex(index);
                }
            }
            else
            {
                QSizeF size = lastPageSize.size(QPageSize::Millimeter);
                QString customName = QString("Custom (%1mm x %2mm)").arg(size.width()).arg(size.height());
                int index = mPaperTypes->findText("Custom",Qt::MatchContains);
                if(index >= 0)
                {
                    mPaperTypes->setItemText(index,customName);
                    mPaperTypes->setItemData(index,size);
                }
                else
                    mPaperTypes->addItem(customName,size);

                if(mPrinter->orientation() == QPrinter::Portrait)
                {
                    v.setreal("pp::paperwidth",size.width());
                    v.setreal("pp::paperheight",size.height());
                    v.setreal("cd::nonstdwidth",size.width());
                    v.setreal("cd::nonstdheight",size.height());
                    v.setstring("pp::papername",customName.data());
                }
                else
                {
                    v.setreal("pp::paperwidth",size.height());
                    v.setreal("pp::paperheight",size.width());
                    v.setreal("cd::nonstdwidth",size.height());
                    v.setreal("cd::nonstdheight",size.width());
                    v.setstring("pp::papername",customName.data());
                }

                mPaperTypes->setCurrentText(customName);
            }
        }

        //QObject::connect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));
    }
    else
    {
        mPrintPdfAction->setChecked(false);
        mPrintSetupAction->setEnabled(true);
        mPageSetupAction->setEnabled(true);
#if MAJOR_VERSION > 7
        mPdfRes->setEnabled(false);
        //QList<int> templist = mPrinter->supportedResolutions();
        //mPrinter->setResolution(defaultPrinterRes);
        //v.setreal("pp::printscale",defaultPrintScale);
#endif
        QPageSize lastPageSize = mPrinter->pageLayout().pageSize();
        QPageLayout::Orientation lastOrientation = mPrinter->pageLayout().orientation();
        mPrinter->setOutputFormat(QPrinter::NativeFormat);
        mPrinter->setPageSize(lastPageSize);
        mPrinter->setPageOrientation(lastOrientation);
        mPrinter->setOutputFileName(QString());
        qDebug() << "printer name : " << mPrinter->printerName();
        settings.setValue("Strings/pr::lastprinter",mPrinter->printerName());

        lastPdfState = false;

        qDebug() << "pagesize paper name" << lastPageSize.name();
        qDebug() << "printer page name" << mPrinter->pageLayout().pageSize().name();

        //QObject::disconnect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));

        mPaperTypes->clear();
        QPrinterInfo printerInfo(*mPrinter);
        QList<QPageSize> pageSizes = printerInfo.supportedPageSizes();
        for(int i=0; i<pageSizes.size(); i++)
        {
            mPaperTypes->addItem(pageSizes.at(i).name(),pageSizes.at(i).size(QPageSize::Millimeter));
        }

        mPrinter->setPageOrientation(lastOrientation);
        mPrinter->setPageSize(lastPageSize);
        qDebug() << "pagesize paper name" << lastPageSize.name();
        qDebug() << "printer page name" << mPrinter->pageLayout().pageSize().name();

        int index = mPaperTypes->findText(mPrinter->pageLayout().pageSize().name());
        if(index < 0)
        {
            QSizeF size = lastPageSize.size(QPageSize::Millimeter);
            QString customName = QString("Custom (%1mm x %2mm)").arg(size.width()).arg(size.height());
            int index = mPaperTypes->findText("Custom",Qt::MatchContains);
            if(index >= 0)
            {
                mPaperTypes->setItemText(index,customName);
                mPaperTypes->setItemData(index,size);
            }
            else
                mPaperTypes->addItem(customName,size);

            if(mPrinter->orientation() == QPrinter::Portrait)
            {
                v.setreal("pp::paperwidth",size.width());
                v.setreal("pp::paperheight",size.height());
                v.setreal("cd::nonstdwidth",size.width());
                v.setreal("cd::nonstdheight",size.height());
                v.setstring("pp::papername",customName.data());
            }
            else
            {
                v.setreal("pp::paperwidth",size.height());
                v.setreal("pp::paperheight",size.width());
                v.setreal("cd::nonstdwidth",size.height());
                v.setreal("cd::nonstdheight",size.width());
                v.setstring("pp::papername",customName.data());
            }

            mPaperTypes->setCurrentText(customName);
        }
        else
        {
            if(mPrinter->orientation() == QPrinter::Portrait)
            {
                v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());
            }
            else
            {
                v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());
            }

            mPaperTypes->setCurrentText(mPrinter->pageLayout().pageSize().name());
        }

        //QObject::connect(mPaperTypes,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(setPaperType(const QString&)));
    }
}

void RCPrintPreview_Dialog::print()
{
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

    qDebug() << "paper name : " << mPrinter->paperName();
    qDebug() << "page paper name : " << mPrinter->pageLayout().pageSize().name();
    qDebug() << "output file name : " << mPrinter->outputFileName();
    qDebug() << "printer name : " << mPrinter->printerName();

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    if (mPrinter->outputFormat() != QPrinter::NativeFormat)
    {
        QString title = QCoreApplication::translate("RCPrintPreview_Dialog", "Save to PDF");
        QString suffix = QLatin1String(".pdf");
        QString fileName,deffilename(mPrinter->outputFileName());
        qDebug() << "resolution : " << mPrinter->resolution();

        // get the last pdf file path
        deffilename = settings.value("Strings/pr::lastpdffilepath",QString(home.getpublicfilename(""))).toString();
        deffilename += deffilename.endsWith(QDir::separator()/*"/"*/) ? mPrinter->outputFileName() : QDir::separator()/*"/"*/ + mPrinter->outputFileName();

        QFileDialog  FileDialog(this, title, deffilename,QLatin1Char('*') + suffix);
        //FileDialog.setOption(QFileDialog::DontUseNativeDialog);
        FileDialog.setAcceptMode(QFileDialog::AcceptSave);

        int status = FileDialog.exec();

        //if (!fileName.isEmpty()) {
        if (status)
        {
            if(FileDialog.selectedFiles().size() > 0)
                fileName = FileDialog.selectedFiles().at(0);
            if (QFileInfo(fileName).suffix().isEmpty())
                fileName.append(suffix);
            mPrinter->setOutputFileName(fileName);
        }
        else
        {
#if MAJOR_VERSION < 8
            reject();
#else
            if(mkeepPreviewOpen->checkState() == 0)
                reject();
#endif
            return;
        }

        if(mColourAction->isChecked())
            mPrinter->setColorMode(QPrinter::Color);
        else if(mBlackWhiteAction->isChecked())
            mPrinter->setColorMode(QPrinter::GrayScale);
        else if(mGreyScaleAction->isChecked())
            mPrinter->setColorMode(QPrinter::GrayScale);

        qDebug() << "pp::papername : " << QString(v.getstring("pp::papername"));

        mPrinter->setPageOrientation(QString(v.getstring("pp::orientation")) == "portrait" ? QPageLayout::Portrait : QPageLayout::Landscape );

        mPaperTypes->setCurrentText(mPrinter->paperName());

        if (mPreview->orientation() == QPrinter::Portrait) {
            setPortraitOrientation();
        }else {
            setLandscapeOrientation();
        }

        qDebug() << "paper name : " << mPrinter->paperName();
        if (!mPrinter->outputFileName().isEmpty())
        {

            QFileInfo fileInfo(fileName);
            QString lastFilePath = fileInfo.path();
            // set the last pdf file path
            settings.setValue("Strings/pr::lastpdffilepath",lastFilePath);
            mPreview->print();
        }



        // Enhancement for v8: don't close the dialog after printing
#if MAJOR_VERSION < 8
        accept();
#else
        if(mkeepPreviewOpen->checkState() == 0)
            accept();
        else
            return;
#endif
    }
#endif
    if(mColourAction->isChecked())
        mPrinter->setColorMode(QPrinter::Color);
    else if(mBlackWhiteAction->isChecked())
        mPrinter->setColorMode(QPrinter::GrayScale);
    else if(mGreyScaleAction->isChecked())
        mPrinter->setColorMode(QPrinter::GrayScale);

    qDebug() << "pp::papername : " << QString(v.getstring("pp::papername"));
    mPaperTypes->setCurrentText(mPrinter->paperName());
    if (mPreview->orientation() == QPrinter::Portrait) {
        setPortraitOrientation();
    }else {
        setLandscapeOrientation();
    }

    qDebug() << "pp::papername : " << QString(v.getstring("pp::papername"));
    //v.setinteger("pr::patternfilllod",0);
    mPreview->print();
    //v.setinteger("pr::patternfilllod",8);

    // Enhancement for v8: don't close the dialog after printing
#if MAJOR_VERSION < 8
    accept();
#else
    if(mkeepPreviewOpen->checkState() == 0)
        accept();
#endif
}

void RCPrintPreview_Dialog::FitPage()
{
    mPreview->fitToWidth();
    mPreview->fitInView();
}

void RCPrintPreview_Dialog::FitPageWidth()
{
    mPreview->fitToWidth();
}

void RCPrintPreview_Dialog::ZoomPageIn()
{
    mPreview->zoomIn();
}

void RCPrintPreview_Dialog::ZoomPageOut()
{
    mPreview->zoomOut();
}

void RCPrintPreview_Dialog::setPrintColour()
{
    v.setinteger("pl::blackandwhite",0);
    mPrinter->setColorMode(QPrinter::Color);
    mColourAction->setChecked(true);

    if (mPreview->orientation() == QPrinter::Portrait)
        setPortraitOrientation();
    else
        setLandscapeOrientation();

}

void RCPrintPreview_Dialog::setPrintBlackWhite()
{
    v.setinteger("pl::blackandwhite",1);
    mPrinter->setColorMode(QPrinter::GrayScale);
    mBlackWhiteAction->setChecked(true);

    if (mPreview->orientation() == QPrinter::Portrait)
        setPortraitOrientation();
    else
        setLandscapeOrientation();

}

void RCPrintPreview_Dialog::setPrintGreyScale()
{
    v.setinteger("pl::blackandwhite",2);
    mPrinter->setColorMode(QPrinter::GrayScale);
    mGreyScaleAction->setChecked(true);
    if (mPreview->orientation() == QPrinter::Portrait)
        setPortraitOrientation();
    else
        setLandscapeOrientation();
}

void RCPrintPreview_Dialog::setPaperType(const QString & text)
{
#if 1
    int index = mPaperTypes->currentIndex();
    if(index >= 0)
    {
        QSizeF size = mPaperTypes->itemData(index).toSizeF();
        QString name = mPaperTypes->itemText(index);
        QPageSize pageSize(size,QPageSize::Millimeter,name);
        mPrinter->setPageSize(pageSize);

        if(mPrinter->orientation() == QPrinter::Portrait)
        {
            v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
            v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
            v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());
        }
        else
        {
            v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
            v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
            v.setstring("pp::papername",mPrinter->pageLayout().pageSize().name().data());
        }

        if(mPrinter->outputFormat() == QPrinter::PdfFormat)
        {
            if( ! name.contains("Custom"))
            {
                // force the orientation
                if(name.contains("V"))
                    setPortraitOrientation();
                else
                    setLandscapeOrientation();
            }
            else
            {
                // use the current orientation
                //if (mPreview->orientation() == QPrinter::Portrait)
                if (mPrinter->orientation() == QPrinter::Portrait)
                    setPortraitOrientation();
                else
                    setLandscapeOrientation();
            }
        }
        else
        {
            // use the current orientation
            //if (mPreview->orientation() == QPrinter::Portrait)
            if (mPrinter->orientation() == QPrinter::Portrait)
                setPortraitOrientation();
            else
                setLandscapeOrientation();
        }

        qDebug() << "paper name" << mPrinter->paperName();
        qDebug() << "paper width" << mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width();
        qDebug() << "paper height" << mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height();
        qDebug() << "page orientation" << mPrinter->pageLayout().orientation();
        qDebug() << "paper orientation" << mPrinter->orientation();
        qDebug() << "";
        //v.setreal("pp:paperwidth",);
    }
#endif
#if 0
    if(mPrinter->printerName() == "" || mPrinter->outputFormat() == QPrinter::PdfFormat)
    {
        int index = View2d::matchPaperName((RCCHAR*)text.data());
        if(text.contains("V"))
            mPrinter->setPageOrientation(QPageLayout::Portrait);
        else
            mPrinter->setPageOrientation(QPageLayout::Landscape);
        QSizeF sizef;
        // pages are always defined in portrait orientation
        sizef.setWidth(View2d::heights[index]);
        sizef.setHeight(View2d::widths[index]);
        QString pageName = text;
        if(text.toLower().contains("custom",Qt::CaseInsensitive))
        {
            pageName = "";//QString("Custom (%1mm x %2mm)").arg(View2d::widths[index]).arg(View2d::heights[index]);
            // pages are always defined in portrait orientation
            // assume user defined orientation is size dependent
            if(v.getreal("cd::nonstdheight") > v.getreal("cd::nonstdwidth"))
            {
                sizef.setHeight(v.getreal("cd::nonstdheight"));
                sizef.setWidth(v.getreal("cd::nonstdwidth"));
            }
            else
            {
                sizef.setHeight(v.getreal("cd::nonstdwidth"));
                sizef.setWidth(v.getreal("cd::nonstdheight"));
            }
        }
        QPageSize pageSize(sizef,QPageSize::Millimeter,pageName,QPageSize::ExactMatch);
        mPrinter->setPageSize(pageSize);
        if(text.toLower().contains("custom",Qt::CaseInsensitive))
        {
            index = mPaperTypes->currentIndex();
            mPaperTypes->setItemText(index,mPrinter->paperName());
            mPaperTypes->setItemData(index,mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter));
             // assume user defined orientation is size dependent
            if(v.getreal("cd::nonstdheight") > v.getreal("cd::nonstdwidth"))
                mPrinter->setPageOrientation(QPageLayout::Portrait);
            else
                mPrinter->setPageOrientation(QPageLayout::Landscape);
        }
        else
        {
            int index = View2d::matchPaperName((RCCHAR*)text.data());
            if(index >= 0)
            {
                v.setstring("pp::papername",View2d::sizes[index].data());
                if(index < View2d::nsizes -1)
                {
                    v.setreal("pp::paperwidth",View2d::widths[index]);
                    v.setreal("pp::paperheight",View2d::heights[index]);
                }
                else
                {
                    v.setreal("pp::paperwidth",v.getreal("cd::nonstdwidth"));
                    v.setreal("pp::paperheight",v.getreal("cd::nonstdheight"));
                }
            }
        }
    }
    else
    {
        if(text.toLower().contains("custom"))
        {
            QString customName;
            if(mPrinter->pageLayout().pageSize().id() == QPageSize::Custom)
            {
                qDebug() << "custom name : " << mPrinter->pageLayout().pageSize().name();
                qDebug() << "custom id   : " << mPrinter->pageLayout().pageSize().id();
                v.setreal("cd::nonstdwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                v.setreal("cd::nonstdheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
                v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
                customName = QString("Custom (%1mm x %2mm)").arg(v.getreal("cd::nonstdwidth")).arg(v.getreal("cd::nonstdheight"));
                mPrinter->setPaperName(customName);
            }
            else
            {
                qDebug() << "custom name : " << mPrinter->pageLayout().pageSize().name();
                qDebug() << "custom id   : " << mPrinter->pageLayout().pageSize().id();
                QSizeF sizef;
                // pages are always defined in portrait orientation
                sizef.setWidth(View2d::heights[View2d::nsizes-1]);
                sizef.setHeight(View2d::widths[View2d::nsizes-1]);
                QString pageName = text;
                if(text.toLower().contains("custom",Qt::CaseInsensitive))
                {
                    pageName = "";//QString("Custom (%1mm x %2mm)").arg(View2d::widths[index]).arg(View2d::heights[index]);
                    // pages are always defined in portrait orientation
                    // assume user defined orientation is size dependent
                    if(v.getreal("cd::nonstdheight") > v.getreal("cd::nonstdwidth"))
                    {
                        sizef.setHeight(v.getreal("cd::nonstdheight"));
                        sizef.setWidth(v.getreal("cd::nonstdwidth"));
                    }
                    else
                    {
                        sizef.setHeight(v.getreal("cd::nonstdwidth"));
                        sizef.setWidth(v.getreal("cd::nonstdheight"));
                    }
                }
                QPageSize pageSize(sizef,QPageSize::Millimeter,pageName,QPageSize::ExactMatch);
                mPrinter->setPageSize(pageSize);
                customName = pageSize.name();
            }
            int index = mPaperTypes->findText("Custom",Qt::MatchContains);
            mPaperTypes->setItemText(index,customName);
            mPaperTypes->setItemData(index,mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter));
            mPaperTypes->setCurrentIndex(index);
            if(v.getreal("cd::nonstdheight") > v.getreal("cd::nonstdwidth"))
                mPrinter->setPageOrientation(QPageLayout::Portrait);
            else
                mPrinter->setPageOrientation(QPageLayout::Landscape);
            v.setstring("pp::papername",mPrinter->paperName().data());
        }
        else
        {
            mPrinter->setPaperName(text);
            v.setreal("pp::paperwidth",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width());
            v.setreal("pp::paperheight",mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height());
            v.setstring("pp::papername",mPrinter->paperName().data());
        }
    }
    qDebug() << "paper name" << mPrinter->paperName();
    qDebug() << "paper width" << mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).width();
    qDebug() << "paper height" << mPrinter->pageLayout().pageSize().size(QPageSize::Millimeter).height();
    qDebug() << "page orientation" << mPrinter->pageLayout().orientation();
    qDebug() << "paper orientation" << mPrinter->orientation();

    if (mPreview->orientation() == QPrinter::Portrait)
        setPortraitOrientation();
    else
        setLandscapeOrientation();
#endif


}

void RCPrintPreview_Dialog::setPrintScale(const QString & text)
{
    // calculate the print scale and redraw the preview
    qDebug() << "scale text : " << text;
    int i;
    RCCHAR string[300],*eptr;
    double x,scale=1.0;

    if(mPrintScale->currentIndex() == 0)
    {
        // fit to paper scaling
        scale = 0.0;
        scale = 1.0;
    }
    else
    {
        // other scale values
        strcpy(string,text.data());

        for (i = 0 ; DefaultPlotScales[i].label != 0 ; i++)
        {
            if (_stricmp(string,DefaultPlotScales[i].label.data()) == 0)
            {
                scale = DefaultPlotScales[i].scale;
                break;
            }
        }

        if (DefaultPlotScales[i].label == 0)
        {
            ResourceString rs1(NCDIALOG+1);
            ResourceString rs2(NCDIALOG+2);

           if(text.contains(":"))
           {
               bool ok1=false,ok2=false;
               QStringList strings = text.split(":");
               if(strings.size() == 1 && strings.at(0) == ":")
                   ok1=false;
               else
               {
                   QString left = strings.at(0);
                   QString right = strings.at(1);
                   x = right.toDouble(&ok1) / left.toDouble(&ok2);
               }
               if(!ok1 || !ok2)
                   cadwindow->MessageBox(rs1.gets(),"",MB_ICONSTOP);
           }
           else if(text.endsWith('%'))
           {
               bool ok=false;
               QString string = text.left(text.length()-1);
               x = string.toDouble(&ok) / 100.0;
               if(!ok)
                   cadwindow->MessageBox(rs1.gets(),"",MB_ICONSTOP);
           }
           else
           {
               bool ok=false;
               x = text.toDouble(&ok);
               if(!ok)
                   cadwindow->MessageBox(rs1.gets(),"",MB_ICONSTOP);
           }
           if (x < 1.0E-10 || x > 1.0E10)
           {
                cadwindow->MessageBox(rs2.gets(),"",MB_ICONSTOP);
           }
           scale = x;
        }
    }
    qDebug() << "scale value      : " << scale;
    qDebug() << "preview scale value : " << v.getreal("pr::printscale");
    v.setstring("pr::scalename",(RCCHAR*)text.data());
    v.setreal("pr::printscale",scale);
    if(0  & lastPdfState) {
        v.setreal("pp::printscale",scale*(((double)mPrinter->resolution())/((double)defaultPrinterRes)));
    }
    else {
        v.setreal("pp::printscale",scale);
    }
    v.setreal("vw::plotscale",scale);
    qDebug() << "pr scale name : " << QString(v.getstring("pr::scalename"));


    if (mPreview->orientation() == QPrinter::Portrait)
        setPortraitOrientation();
    else
        setLandscapeOrientation();

}

void RCPrintPreview_Dialog::printScaleEdited(const QString & text)
{

}

#if MAJOR_VERSION > 7
void RCPrintPreview_Dialog::reverseBWTriggered(bool state)
{
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    v.setinteger("pl::reverseblackwhite",(int)state);
    settings.setValue("Integers/pl::reverseblackwhite",(int)state);
    settings.sync();

    /*
    switch((int)state)
    {
    case 0:
    case 1:
        //mReverseWhite->setText("Reverse Black/White");
        mReverseWhite->setToolTip("<b>Invert White</b><hr>Prints white entities as black");
        break;
    case 2:
    default:
        //mReverseWhite->setText("Auto reverse Black/White");
        mReverseWhite->setToolTip("<b>Auto Invert White</b><hr>Automatically prints white entities as black");
        break;
    }
    */

    if (mPreview->orientation() == QPrinter::Portrait)
        setPortraitOrientation();
    else
        setLandscapeOrientation();

}
/*
void RCPrintPreview_Dialog::doPaintRequested(QPrinter *printer)
{
    qDebug() << "in doPaintRequested";
}
*/
int RCPrintPreview_Dialog::getPdfRes()
{
    return mPdfRes->itemData(mPdfRes->currentIndex()).toInt();
}

void RCPrintPreview_Dialog::pdfResChanged(int res)
{
    if(mPrinter && mPrinter->outputFormat() != QPrinter::NativeFormat)
    {
        QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
        //mPrinter->setResolution(getPdfRes());
        v.setinteger("pr::pdfresolution",res);
        settings.setValue("Integers/pr::pdfresolution",res);
        //v.setreal("pp::printscale",defaultPrintScale*(((double)mPrinter->resolution())/((double)defaultPrinterRes)));

        if(QString(v.getstring("pp::orientation")) == "portrait")
            setPortraitOrientation();
        else
            setLandscapeOrientation();
        //mPreview->fitInView();
    }
}

void RCPrintPreview_Dialog::useLastPrinterTriggered(int state)
{
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    v.setinteger("pr::uselastprinter",(state == 2 ? 1 : 0));
    settings.setValue("Integers/pr::uselastprinter",(state == 2 ? 1 : 0));
}

void RCPrintPreview_Dialog::keepOpenTriggered(int state)
{
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    v.setinteger("pr::keeppreviewopen",(state == 2 ? 1 : 0));
    settings.setValue("Integers/pr::keeppreviewopen",(state == 2 ? 1 : 0));
}
#endif
