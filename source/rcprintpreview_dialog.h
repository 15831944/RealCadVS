#ifndef RCPRINTPREVIEW_DIALOG_H
#define RCPRINTPREVIEW_DIALOG_H

#include <QDialog>
#include <QtPrintSupport/qtprintsupportglobal.h>

class RCPrintPreviewMainWindow;
class QPrintPreviewWidget;
class QPrinter;
class QAction;
class QActionGroup;
class QPrintDialog;
class QToolBar;
class QComboBox;
class QCheckBox;

class RCPrintPreview_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit RCPrintPreview_Dialog(QPrinter * printer=0, QWidget *parent = 0, Qt::WindowFlags flags=0);
    ~RCPrintPreview_Dialog();

    QPrinter* printer();
    void setupActions(QWidget *parent=0);

    RCPrintPreviewMainWindow* mMainWindow;
    QPrintPreviewWidget* mPreview;
    QToolBar *mToolbar;
    QPrintDialog *mPrintDialog;
    QPrinter *mPrinter;
    int defaultPrinterRes;
    double defaultPrintScale;
    bool mOwnPrinter;

    // actions

    QActionGroup *mPrinterGroup;
    QAction *mPrintAction;
    QAction *mPrintSetupAction;
    QAction *mPageSetupAction;
    QAction *mPrintPdfAction;

    QActionGroup* mOrientationGroup;
    QAction *mPortraitAction;
    QAction *mLandscapeAction;

    QActionGroup* mColourGroup;
    QAction *mColourAction;
    QAction *mBlackWhiteAction;
    QAction *mGreyScaleAction;

    QComboBox *mPdfRes;
    QComboBox *mPrintScale;
    QComboBox *mPaperTypes;

    QActionGroup* mPageZoomGroup;
    QAction *mFitPageAction;
    QAction *mFitWidthAction;
    QAction *mZoomInAction;
    QAction *mZoomOutAction;

    QAction *mReverseWhiteAction;
    QCheckBox *mReverseWhite;
    QCheckBox *mUseLastPrinter;
    QCheckBox *mkeepPreviewOpen;

    bool lastPdfState;

#if MAJOR_VERSION > 7
    int getPdfRes();
#endif

protected:

signals:
    void paintRequested(QPrinter *printer);

public slots:
    void setPortraitOrientation();
    void setLandscapeOrientation();
    void previewChanged();
    void print();
    void printToPdf();
    void pdfTriggered(bool checked);
    void printSetup();
    void pageSetup();
    void FitPage();
    void FitPageWidth();
    void ZoomPageIn();
    void ZoomPageOut();
    void setPrintColour();
    void setPrintBlackWhite();
    void setPrintGreyScale();
    void setPaperType(const QString & text);
    void setPrintScale(const QString & text);
    void printScaleEdited(const QString & text);
#if MAJOR_VERSION > 7
    void reverseBWTriggered(bool state);
    void useLastPrinterTriggered(int state);
    void pdfResChanged(int res);
    void keepOpenTriggered(int state);
#endif

    //void doPaintRequested(QPrinter *printer);
};

#endif // RCPRINTPREVIEW_DIALOG_H
