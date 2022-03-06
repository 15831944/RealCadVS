#ifndef RCFigureBrowser_H
#define RCFigureBrowser_H

#include "ncwin.h"

#include <QDialog>
#include <QFileSystemModel>
#include <QtConcurrent>
class QGraphicsItem;
class QGraphicsView;

class QFileSystemModel;

class BaseWidget : public QWidget
{
Q_OBJECT
public:
    BaseWidget(QWidget *browserWidget, QWidget *parent = 0);
    ~BaseWidget();
    bool isStarted() { return thumbnail->isStarted(); }
    bool isRunning() { return thumbnail->isRunning(); }
    void waitforFinished() { return thumbnail->waitForFinished(); }

    QWidget *browser;
    QGridLayout *grid;
    QList<QGraphicsView *> views;

public Q_SLOTS:

    void getImages(const QStringList &imageFiles);
    void showImage(int num);
    void finished();
private:
    QFutureWatcher<QPixmap> *thumbnail;
};

class previewData : public QObject
{
    Q_OBJECT
public:
    RCHWND mhwnd;
    QGraphicsScene *mscene;
    QGraphicsItem *mgitem;
    Point meye,mref,mup;
    int mpersp;
    RCCHAR mfigurename[500];
    previewData()
    {
        mscene = 0;
        mgitem = 0;
        mhwnd  = 0;
        meye   = Point();
        mref   = Point();
        mup    = Point();
        mpersp = 0;
        strcpy(mfigurename,"");
    }
    previewData(RCHWND hwnd,Point eye,Point ref,Point up,int persp,RCCHAR *figurename)
    {
        mscene = 0;
        mgitem = 0;
        mhwnd  = hwnd;
        meye   = eye;
        mref   = ref;
        mup    = up;
        mpersp = persp;
        strcpy(mfigurename,figurename);
    }
    previewData(const previewData &o)
    {
        mscene = o.mscene;
        mgitem = o.mgitem;
        mhwnd  = o.mhwnd;
        meye   = o.meye;
        mref   = o.mref;
        mup    = o.mup;
        mpersp = o.mpersp;
        strcpy(mfigurename,o.mfigurename);
    }
    previewData operator=(const previewData &o)
    {
        mscene = o.mscene;
        mgitem = o.mgitem;
        mhwnd  = o.mhwnd;
        meye   = o.meye;
        mref   = o.mref;
        mup    = o.mup;
        mpersp = o.mpersp;
        strcpy(mfigurename,o.mfigurename);
        return *this;
    }
    ~previewData()
    {
        mscene=0;
        mgitem=0;
    }

public slots:
     void handleFinished();
};

namespace Ui {
class RCFigureBrowser;
}

class RCFigureBrowser : public QDialog
{
    Q_OBJECT

    QMenu *m_FileTreeMenu;
    QAction *Act_ShowHidden;
    void CreateContextMenu();
    QTimer *drawtimer;
    QTimer *stoptimer;
    QPoint mFirstPos;
    QProgressDialog *thumbProgressDlg;
    Point mPmin,mPmax;

public:
    enum settingType { none, figures, bitmaps };

    explicit RCFigureBrowser(QWidget *parent = 0);
    explicit RCFigureBrowser(QString rootdir, QString file, QStringList recent, QString filters="*.cad;*.jpg;*.jpeg;*.png;", QString title="Select Figure", QString prefix="Browser", RCFigureBrowser::settingType type=RCFigureBrowser::none, QWidget *parent=0);
    ~RCFigureBrowser();

    bool m_Interrupted;
    bool m_initializing;
    bool m_insertingFolderCB;
    RCCHAR m_filter[300];
    RCCHAR m_rootDir[300];
    RCCHAR m_SelectedFileName[300];
    RCCHAR m_currentDisplayFolder[300];
    QFileSystemModel m_FileSystemModel;
    QGraphicsItem *selectedItem;
    QGraphicsView *selectedView;
    QString m_Prefix;
    QStringList m_RecentPaths;
    QString m_Title;

    QString fixSeperators(const QString & string);
    QString fixFileName(const QString & string);
    void ShowSettingsGroup(settingType type);
    QStringList GetRecentPaths();
    RCCHAR *GetFileName();
    RCCHAR *GetCurrentPath();
    int GetCurrentFileIndex();
    void PreviewImage(QWidget *widget, RCCHAR *filepath );
    void PreviewImageEx(QWidget *widget, RCCHAR *filepath );
    void PreviewImageEx(const QString &filepath);
    void PreviewFigure(RCHWND hwnd,Point eye,Point ref,Point up,int persp,RCCHAR *figurename);
    void PreviewFigure(RCHWND hwnd,RCCHAR *figurename);
    void PreviewFigureEx(RCHWND hwnd,RCCHAR *figurename);
    void PreviewFigureEx(const QString &figurename);
    void PreviewFigureMTController(RCHWND hwnd,Point eye,Point ref,Point up,int persp,RCCHAR *figurename);
    void PreviewFigureGL(RCHWND hwnd,Point eye,Point ref,Point up,int persp,RCCHAR *figurename);
    //
    QString GetPreviewCacheName(const QString &figurePath);
    void CreateOneFigurePreviewFile(const QString &figurePath, int width=100, int height=100);
    void CreateFigurePreviewFiles(const QStringList &figurePaths, int width=100, int height=100);
    void CreateOneImagePreviewFile(const QString &imagePath, int width=100, int height=100);
    void CreateImagePreviewFiles(const QStringList &imagesPaths, int width=100, int height=100);
    QPixmap GetOnePreviewImage(const QString &previewFileName, int width=100, int height=100);
    void GetPreviewPage(int page);
    void ShowImageListWithProgress();

    //QGraphicsItem * PreviewFigureMTWorker(QGraphicsScene *scene,Point eye,Point ref,Point up,int persp,RCCHAR *figurename);
    double FigureXScale();
    double FigureYScale();
    double FigureZScale();
    double BitmapWidth();
    double BitmapHeight();
    RCCHAR* GetPrefix();
    bool GetFigureOriginalStyle();
    bool GetFigureOriginalColour();
    bool GetFigureOriginalLayer();
    bool GetFigureOriginalWeight();
    bool GetFigureOrientToView();
    bool GetBitmapOriginalShape();
    bool GetBitmapLinkFile();
    void clearPreview(QWidget *widget);
    void initGrid();

    // v8 enhancements
    bool m_UseSize;
    Point mLibrarySize;
    double FigureXSize();
    double FigureYSize();
    double FigureZSize();
    void GetFigureExtents(RCCHAR *filepath, Point &pmin, Point &pmax);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void showEvent(QShowEvent * event);
    void resizeEvent(QResizeEvent *e);
    void moveEvent(QMoveEvent *e);

signals:

public slots:
    void OnInitDialog();

    void accept();

    void SetFileNames();

    void UpdateImageList();

    void UpdateImageListT();

    void SetFolderTree(RCCHAR * rootDir);

    void selectFigureListItem(RCCHAR *text);

    void selectFigureListItem(QWidget *item);

    void SetCurrentFolder(RCCHAR *text);

    void SetFigureXScale(double scale);
    void SetFigureYScale(double scale);
    void SetFigureZScale(double scale);

    void SetBitmapWidth(double width);
    void SetBitmapHeight(double height);

    void SetTitle(RCCHAR *title);
    void SetFilter(RCCHAR *filter);
    void SetRootDir(RCCHAR *rootdir);
    void SyncFolderTree(RCCHAR *selDir);
    void SetSelFileName(RCCHAR *filename);
    void SetRecentPaths(QStringList paths);
    void SetPrefix(RCCHAR *prefix);

    void SetFigureOriginalStyle(bool OnOff);
    void SetFigureOriginalColour(bool OnOff);
    void SetFigureOriginalLayer(bool OnOff);
    void SetFigureOriginalWeight(bool OnOff);
    void SetFigureOrientToView(bool OnOff);
    void SetBitmapOriginalShape(bool OnOff);
    void SetBitmapLinkFile(bool OnOff);

    void SetShowHidden();

    void handleFinished();
    void interrupted()
    {
        m_Interrupted = true;
        program->setinterrupt(true);
    }

    void ShowPreviewFiles();

    // v8 enhancements
    void SetFigureXSize(double size);
    void SetFigureYSize(double size);
    void SetFigureZSize(double size);
    void SetSizeChecked(bool checked);
    bool SizeIsChecked();

private slots:
    void on_IDC_FILENAME_COMBO_currentIndexChanged(const QString &arg1);

    void on_IDC_FOLDER_COMBO_currentIndexChanged(const QString &arg1);

    void on_IDC_BROWSE_BTN_clicked();

    void on_IDC_FILENAME_COMBO_currentTextChanged(const QString &arg1);

    void on_IDC_FOLDER_COMBO_currentTextChanged(const QString &arg1);

    void on_IDC_FOLDER_TREE_pressed(const QModelIndex &index);

    void on_IDC_FOLDER_TREE_customContextMenuRequested(const QPoint &pos);

    void on_originalLayer_clicked();

    void on_originalColour_clicked();

    void on_originalStyle_clicked();

    void on_originalWeight_clicked();

    void updateGrid();

    void on_RCFigureBrowser_accepted();

    void getFirstPos();

    void onIdle();

    void on_IDC_FOLDER_COMBO_activated(const QString &arg1);

    void on_IDC_FILENAME_COMBO_activated(const QString &arg1);

    void on_SizeCheckBox_clicked();

    void on_ScaleCheckBox_clicked();

    void on_SizeCheckBox_clicked(bool checked);

    void on_XSize_valueChanged(double arg1);

    void on_YSize_valueChanged(double arg1);

    void on_ZSize_valueChanged(double arg1);

    void on_XScale_valueChanged(double arg1);

    void on_YScale_valueChanged(double arg1);

    void on_ZScale_valueChanged(double arg1);

    void on_ScaleCheckBox_clicked(bool checked);

private:
    Ui::RCFigureBrowser *ui;
};

#endif // RCFigureBrowser_H
