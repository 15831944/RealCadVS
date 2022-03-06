#ifndef _QTDEFS_H_
#define _QTDEFS_H_

#define UNREFERENCED_PARAMETER(P) (P)

//#define _MAC
#undef _WIN32_WCE

#ifdef UNICODE
#pragma message("UNICODE is defined")
#else
#pragma message("UNICODE is not defined")
#endif

//#undef UNICODE
#define _USING_QTCHAR
#define _DEBUG_RCCHARS

// bit flags for View 3D Window types
#define RCSCENEVW   1
#define RCNORMLVW   2
#define RCWINTYPE   4
#define RCGLWINTYPE 8
#define RCV3DWINTYPE 16
#define RCV2DWINTYPE 32

// finished flag for a modeless dialog
#define RCDIALOG_NOT_FINISHED -1

#ifndef _USING_QT
#if defined(_WIN32) || defined(_WIN64)
#include "sys/types.h"
#include "sys/stat.h"
#include <stdarg.h>
#include <tchar.h>
#include <ctype.h>
#endif
#else
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#ifndef _MAC
#include <malloc.h>
#include <direct.h>
#include "Lmcons.h"
#else
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <exception>
#endif

//#include "ijl.h"

//#include <QtGui>
#include <QtGlobal>
#include <QObject>
#include <QApplication>
#include <QMdiArea>
#include <QDialog>
#include <QMdiSubWindow>
#include <QAbstractEventDispatcher>
#include <QDockWidget>
#include <QToolBar>
#include <QLabel>
#include <QLineEdit>
#include <QTranslator>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QLibrary>
#include <QMainWindow>
#include <QWidget>
#include <QWindow>
#include <QMessageBox>
#include <QSettings>
#include <QChar>
#include <QString>
#include <QTextstream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QtEndian>
#include <QPainter>
#include <QFont>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QBitmap>
#include <QImage>
#include <QLocale>
#include <QRegion>
#include <QScrollBar>
#include <QScrollArea>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QGLFormat>
#include <QGLWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QToolButton>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QFontInfo>
#include <QFontMetrics>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTimer>
#include <QSizeGrip>
#include <QToolTip>
#include <QtPrintSupport/QPrinterInfo>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewWidget>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPageSetupDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QListView>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableWidgetItem>
#include <QSlider>
#include <QShortcut>
#include <QFontDatabase>
#include <QByteArray>
#include <QDateTime>
#include <QClipboard>
#include <QMimeData>
#include <QImageWriter>
#include <QtOpenGL/QtOpenGl>
#include <QOpenGLWidget>
#include <QValidator>
#include <QNetworkAccessManager>

//#include "RCGraphicsView.h"
//#include "RCGraphicsScene.h"
//#include "RCView.h"
//#include "RCCentralWidget.h"
#define RCDBL_MAX DBL_MAX
#define RCDBL_MIN DBL_MIN
#define RCFLT_MAX FLT_MAX
#define RCFLT_MIN FLT_MIN
#define RCREAL_MAX DBL_MAX
#define RCREAL_MIN DBL_MIN

// QGraphicsItem zValue variables
#define QGMINZVALUE 0.0
//#define QGZVALINC 1.0E-10
#define QGZVALINC 0.0
#define QGDRAGZVALUE(a) (a+QGZVALINC)
extern qreal QGMAXZVALUE; // current max zValue

//#include "RCGraphicsItems.h"
#include "RCtoolbutton.h"
#include "rctoolbox.h"
#include "mainwindow.h"

extern QString elideTextMiddle(const QString inString, int elideWidth);

/// converts a RealCAD dialog control integer name to a QT string names
inline int RCnameToInt(const QString &name)
{
    return name.right(name.length()-1).toInt();
}

/// converts a Qt string name to a RealCad integer name
inline QString RCintToName(int iname)
{
    return QString("_%1").arg(iname);
}

/// returns the name string of a QGraphicsItem object
extern QString getItemTypeName(QGraphicsItem *gi);

class Button;

class mywidget : public QWidget
{
protected:
    void resizeEvent(QResizeEvent *);
public :
    explicit mywidget();
};

class StateMachine;
class Entity;

class GlobalEventHandler : public QObject
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};


/**
 * @brief The RCApplication class
 */
//class RCApplication : public QGuiApplication
class RCApplication : public QApplication
{
private:
    Q_OBJECT

    void cleanUpOnQuit();
public:
    QProcess *mSketchUpBrowser;
    QTimer *mIdleTimer;
    Qt::MouseButton mbtn;
    QPoint mlastMousPos;
    int mlastkey; // the last key pressed
    int mlastmodifiers; // the last modifier pressed
    bool eventLoopStarted;
    QNetworkReply *mlastWebPageReply;
    int mdialogFinishedFlag;
    QTimer *mFlashTimer;
    Entity *mFlashEntity;

public:
    explicit RCApplication(int &argc, char ** argv);
    ~RCApplication();

    QString getStyleSheetText(QString stylePath);
    bool notify(QObject * receiver, QEvent * e);
    bool event(QEvent * event);
    //int exec();
    StateMachine *getStateMachine();
private:
	void registerDialogType();
public slots:
    void onIdle();
    void webPageReplyFinished(QNetworkReply *reply);
    void checkautosavetimer();
    void dialogFinished(int result);
    void onReady3DWarehouseOutput();
    void onFinished3DWarehouse(int, QProcess::ExitStatus);
};

/**
 * @brief The RCEventLoop class
 */
class RCEventLoop : public QEventLoop
{
     Q_OBJECT

public:
    explicit RCEventLoop(QObject * parent = 0);
    const QMetaObject * metaObject();
    int exec(ProcessEventsFlags flags = AllEvents);
};

/*
class RCGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    RCGLWidget(QWidget *parent)
     : QGLWidget(parent) {}
    RCGLWidget(const QGLFormat & format, QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0)
     : QGLWidget(format, parent, shareWidget, f) { }

    void initializeGL()
    {
        qDebug() << "in initializeGL";

    }

    void resizeGL()
    {
        qDebug() << "in resizeGL";
    }

    void paintGL()
    {
        qDebug() << "in paintGL";
    }
};
*/

/**
 * @brief The RCMdiArea class
 */
class RCMdiArea : public QMdiArea
{
    Q_OBJECT

public:
    enum tilingMode { noTiling, QtTiling, horizontalTiling, verticalTiling };

    explicit RCMdiArea(QWidget *parent=0)
        : QMdiArea(parent)
    {}

    tilingMode tiledMode() { return mtiledMode; }
    void setTiledMode(RCMdiArea::tilingMode mode) { mtiledMode = mode; }

public slots:
    void removeSubWindow(QWidget * widget);
    void tileSubWindows();
    void tileVertically();
    void tileHorizontally();

private:
    tilingMode mtiledMode;

};

/**
 * @brief The RCMdiSubWindow class
 */
class RCMdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    RCMdiSubWindow(QWidget *parent=0, Qt::WindowFlags flags=0 )
        : QMdiSubWindow(parent,flags)
    {}

    virtual void resizeEvent(QResizeEvent * event);
    virtual void closeEvent(QCloseEvent * closeEvent);
    virtual void hideEvent(QHideEvent * hideEvent);
    virtual void moveEvent(QMoveEvent * moveEvent);

signals:
    void isClosing(void);

};

/**
 * @brief The RCstyleIndicator class
 * this class lives on the Style ToolBar and displayes a coloured line on a white BACKGROUND_BLUE
 * this shows the currently selected style
 */
class RCstyleIndicator : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent * event);
public:
    explicit RCstyleIndicator(QWidget *parent=0);

    ~RCstyleIndicator();
};

class RCCursor;
/**
 * @brief The RCCursor class
 * sub classes the QCursor class and adds the == operator
 * this compares the name of the cursor
 */
#if 1
class RCCursor
{
    QCursor mcursor;
public:
    QString m_name;
public:
    explicit RCCursor();
    explicit RCCursor(Qt::CursorShape shape);
    explicit RCCursor(QString bitmapName, QString maskName, int hotX = -1, int hotY = -1);
    explicit RCCursor(QString pixmapName, int hotX = -1, int hotY = -1);

    inline const RCCursor & operator= (const QCursor &other)
    { m_name = "";  mcursor = other; return *this; }
    inline const QCursor & operator= (const RCCursor &other)
    { return mcursor; }
    //inline RCCursor & operator= (const RCCursor &other)
    //{ m_name = other.m_name; QCursor::operator=(other); return *this; }
    friend bool operator==( RCCursor, RCCursor );
};
#else
class RCCursor : public QCursor
{
    QCursor mcursor;
public:
    QString m_name;
public:
    explicit RCCursor();
    explicit RCCursor(Qt::CursorShape shape);
    explicit RCCursor(QString bitmapName, QString maskName, int hotX = -1, int hotY = -1);
    explicit RCCursor(QString pixmapName, int hotX = -1, int hotY = -1);

    inline RCCursor & operator= (const QCursor &other)
    { m_name = "";  mcursor = other; return *this; }
    //inline RCCursor & operator= (const RCCursor &other)
    //{ m_name = other.m_name; QCursor::operator=(other); return *this; }
    friend bool operator==( RCCursor, RCCursor );
};
#endif

/**
 * @brief The RCColourEditValidator class
 */
class RCColourEditValidator : public QValidator
{
    Q_OBJECT

public:
    explicit RCColourEditValidator(QWidget *parent=0) : QValidator(parent){;}

     void fixup(QString & input) const;
     QValidator::State validate(QString & input, int & pos) const;
};

/**
 * @brief The RCWeightEditValidator class
 */
class RCWeightEditValidator : public QValidator
{
    Q_OBJECT

public:
    explicit RCWeightEditValidator(QWidget *parent=0) : QValidator(parent){;}

     void fixup(QString & input) const;
     QValidator::State validate(QString & input, int & pos) const;
};

/**
 * @brief The RCStyleEditValidator class
 */
class RCStyleEditValidator : public QValidator
{
    Q_OBJECT

public:
    explicit RCStyleEditValidator(QWidget *parent=0) : QValidator(parent){;}

     void fixup(QString & input) const;
     QValidator::State validate(QString & input, int & pos) const;
};

/**
 * @brief The RCLayerEditValidator class
 */
class RCLayerEditValidator : public QValidator
{
    Q_OBJECT

public:
    explicit RCLayerEditValidator(QWidget *parent=0) : QValidator(parent){;}

     void fixup(QString & input) const;
     QValidator::State validate(QString & input, int & pos) const;
};

/**
 * @brief The RCStyleEntryToolBar class
 */
class RCStyleEntryToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit RCStyleEntryToolBar(QWidget *parent=0) : QToolBar(parent){ ; }
    explicit RCStyleEntryToolBar(QString title, QWidget *parent=0) : QToolBar(title,parent){ ; }

    QCheckBox *mWeightCB;

public slots:
    void layerButtonPressed();
    void weightButtonPressed();
    void colourButtonPressed();
    void styleButtonPressed();
    void entityButtonPressed();
    void colourTextEdited(QString value);
    void colourTextEditFinished();
    void weightTextEdited(QString value);
    void weightTextEditFinished();
    void layerTextEdited(QString value);
    void layerTextEditFinished();
    void styleTextEdited(QString value);
    void styleTextEditFinished();
    void styleSetIndexChanged(int id);
    void weightDisplayToggled(bool checked);
    void styleDisplayChanged(bool checked);
    //void styleSetActivated(int id);
};

class RCCoordindateEntryValidator : public QValidator
{
    Q_OBJECT
public:
    explicit RCCoordindateEntryValidator(QWidget *parent=0) : QValidator(parent){;}

     void fixup(QString & input) const;
     QValidator::State validate(QString & input, int & pos) const;
};

class RCCoordindateEntryToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit RCCoordindateEntryToolBar(QWidget *parent=0) :QToolBar(parent){;}
    explicit RCCoordindateEntryToolBar(QString title, QWidget *parent=0) : QToolBar(title,parent){;}

protected:
    void mousePressEvent(QMouseEvent * event);

public slots:
    void editTextChanged(const QString & text);
};

class _rcgdiobj
{
public:
    enum _rcgditype { undefined=0, font, pen, brush, pixmap, bitmap, region };
public:
    _rcgdiobj() {;}
    virtual _rcgditype type();
    virtual ~_rcgdiobj() {;}
};
class _rcfont : public _rcgdiobj, public QFont
{
public:
    _rcfont() : _rcgdiobj(), QFont() { ;}
    _rcgditype type();
    virtual ~_rcfont() {;}
};

class _rcpen : public _rcgdiobj, public QPen
{
public:
    _rcpen() : _rcgdiobj(), QPen() {;}
    _rcgditype type(); //{ return _rcgditype::pen ; }
    virtual ~_rcpen() {;}
};
class _rcbrush : public _rcgdiobj, public QBrush
{
public:
    _rcbrush() : _rcgdiobj(), QBrush() {;}
    _rcgditype type();// { return _rcgditype::brush ; }
    virtual ~_rcbrush() {;}
};
class _rcpixmap : public _rcgdiobj, public QPixmap
{
public:
    _rcpixmap() : _rcgdiobj(), QPixmap() {;}
    _rcpixmap(int width,int height) : _rcgdiobj(), QPixmap(width,height) {;}
    _rcgditype type();// { return _rcgditype::pixmap ; }
    virtual ~_rcpixmap() {;}
};
class _rcbitmap : public _rcgdiobj, public QBitmap
{
public:
    _rcbitmap() : _rcgdiobj(), QBitmap() {;}
    _rcbitmap(int width,int height) : _rcgdiobj(), QBitmap(width,height) {;}
    _rcgditype type();// { return _rcgditype::pixmap ; }
    virtual ~_rcbitmap() {;}
};
class _rcrgn : public _rcgdiobj, public QRegion
{
public:
    _rcrgn() : _rcgdiobj(), QRegion() {;}
    _rcgditype type();// { return _rcgditype::region ; }
    virtual ~_rcrgn() {;}
};
typedef _rcgdiobj* RCHGDIOBJ;
typedef _rcfont* RCHFONT;
typedef _rcpen* RCHPEN;
typedef _rcbrush* RCHBRUSH;
typedef _rcpixmap* RCHBITMAP;
typedef _rcrgn* RCHRGN;

// endian conversion
#ifndef BIG_ENDIAN
#define BIG_ENDIAN 1
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 0
#endif
inline int isBigEndian(void)
{
    static const uint16_t m_endianCheck(0x00ff);
    return  (*((uint8_t*)&m_endianCheck) == 0x0) ?  BIG_ENDIAN : LITTLE_ENDIAN;
}
/*
Usage:
To convert from given endian to host, use:
host = endian(source, endian_of_source)
To convert from host endian to given endian, use:
output = endian(hostsource, endian_you_want_to_output)
*/
template <typename T>
T endian(T w, uint32_t endian)
{
    // this gets optimized out into if (endian == host_endian) return w;
    union { uint64_t quad; uint32_t islittle; } t;
    t.quad = 1;
    if (t.islittle ^ endian) return w;
    /*
    T r = 0;

    // decent compilers will unroll this (gcc)
    // or even convert straight into single bswap (clang)
    for (int i = 0; i < sizeof(r); i++) {
        r <<= 8;
        r |= w & 0xff;
        w >>= 8;
    }
    */
    T *r = &w;
    char *b = new char[sizeof(r)];
    b = (char*)r;
    int cnt = sizeof(r);
    for (int i = 0; i < sizeof(r); i++) {
        char tc = b[i];
        b[i] = b[cnt];
        b[cnt] = tc;
    }
    delete [] b;
    return *r;
};
/*
inline double endian(double w, uint32_t endian)
{
    // this gets optimized out into if (endian == host_endian) return w;
    union { uint64_t quad; uint32_t islittle; } t;
    t.quad = 1;
    if (t.islittle ^ endian) return w;

    double *r = &w;
    char *b = new char[sizeof(r)];
    b = (char*)r;
    int cnt = sizeof(r);
    for (int i = 0; i < sizeof(r); i++) {
        char tc = b[i];
        b[i] = b[cnt];
        b[cnt] = tc;
    }
    return *r;
}
*/
//

#ifdef _USING_QTCHAR

typedef void *PVOID;
#ifdef STRICT
typedef void *HANDLE;
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#else
typedef PVOID HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
typedef HANDLE *PHANDLE;

#ifdef _MAC
#ifndef PASCAL
#define PASCAL  __stdcall
#endif
#ifndef CDECL
#define CDECL _cdecl
#endif
#define CALLBACK    PASCAL
#define WINAPI      CDECL
#define WINAPIV     CDECL
#define APIENTRY    WINAPI
#define APIPRIVATE  CDECL
#ifdef _68K_
#define PASCAL      __pascal
//#else
//#define PASCAL
#endif
#else
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define CALLBACK    __stdcall
#define WINAPI      __stdcall
#define WINAPIV     __cdecl
#define APIENTRY    WINAPI
#define APIPRIVATE  __stdcall
#define PASCAL      __stdcall
#else
#define CALLBACK
#define WINAPI
#define WINAPIV
#define APIENTRY    WINAPI
#define APIPRIVATE
#define PASCAL      pascal
#endif
#endif

#define WM_USER                         0x0400

/* Pen Styles */
#define PS_SOLID            0
#define PS_DASH             1       /* -------  */
#define PS_DOT              2       /* .......  */
#define PS_DASHDOT          3       /* _._._._  */
#define PS_DASHDOTDOT       4       /* _.._.._  */
#define PS_NULL             5
#define PS_INSIDEFRAME      6
#define PS_USERSTYLE        7
#define PS_ALTERNATE        8
#define PS_STYLE_MASK       0x0000000F

/* Font Families */
#define FF_DONTCARE         (0<<4)  /* Don't care or don't know. */
#define FF_ROMAN            (1<<4)  /* Variable stroke width, serifed. */
                                    /* Times Roman, Century Schoolbook, etc. */
#define FF_SWISS            (2<<4)  /* Variable stroke width, sans-serifed. */
                                    /* Helvetica, Swiss, etc. */
#define FF_MODERN           (3<<4)  /* Constant stroke width, serifed or sans-serifed. */
                                    /* Pica, Elite, Courier, etc. */
#define FF_SCRIPT           (4<<4)  /* Cursive, etc. */
#define FF_DECORATIVE       (5<<4)  /* Old English, etc. */

#define DEFAULT_PITCH           0
#define FIXED_PITCH             1
#define VARIABLE_PITCH          2
#if(WINVER >= 0x0400)
#define MONO_FONT               8
#endif /* WINVER >= 0x0400 */

/* Local Memory Flags */
#define LMEM_FIXED          0x0000
#define LMEM_MOVEABLE       0x0002
#define LMEM_NOCOMPACT      0x0010
#define LMEM_NODISCARD      0x0020
#define LMEM_ZEROINIT       0x0040
#define LMEM_MODIFY         0x0080
#define LMEM_DISCARDABLE    0x0F00
#define LMEM_VALID_FLAGS    0x0F72
#define LMEM_INVALID_HANDLE 0x8000

#define LHND                (LMEM_MOVEABLE | LMEM_ZEROINIT)
#define LPTR                (LMEM_FIXED | LMEM_ZEROINIT)

#ifndef SNDMSG
#ifdef __cplusplus
#ifndef _MAC
#define SNDMSG ::SendMessage
#else
#define SNDMSG ::AfxSendMessage
#endif
#else
#ifndef _MAC
#define SNDMSG SendMessage
#else
#define SNDMSG AfxSendMessage
#endif
#endif
#endif // ifndef SNDMSG

#define VER_PLATFORM_WIN32s             0
#define VER_PLATFORM_WIN32_WINDOWS      1
#define VER_PLATFORM_WIN32_NT           2

#define QUERYESCSUPPORT              8
/* orientation selections */
#define DMORIENT_PORTRAIT   1
#define DMORIENT_LANDSCAPE  2

#define GETPRINTINGOFFSET            13
#define PHYSICALWIDTH   110 /* Physical Width in device units           */
#define PHYSICALHEIGHT  111 /* Physical Height in device units          */

/* File attribute constants */

#define _A_NORMAL       0x00    /* Normal file - No read/write restrictions */
#define _A_RDONLY       0x01    /* Read only file */
#define _A_HIDDEN       0x02    /* Hidden file */
#define _A_SYSTEM       0x04    /* System file */
#define _A_SUBDIR       0x10    /* Subdirectory */
#define _A_ARCH         0x20    /* Archive file */

#ifdef _MAC
extern unsigned char GetRValue(  unsigned long rgb );
extern unsigned char GetGValue(  unsigned long rgb );
extern unsigned char GetBValue(  unsigned long rgb );
#endif

#ifndef _FSIZE_T_DEFINED
typedef unsigned long _fsize_t; /* Could be 64 bits for Win32 */
#define _FSIZE_T_DEFINED
#endif

#ifndef _FINDDATA_T_DEFINED

#if !defined(WIN32) && !defined(WIN64)
typedef int32_t __time32_t;
typedef int32_t __int32;
typedef int64_t __time64_t;
typedef int64_t __int64;
#endif

#if !defined(_SIZE_T_DEFINED) && !defined(_SIZE_T)
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#define _SIZE_T
#endif

struct _finddata32_t {
        unsigned    attrib;
        __time32_t  time_create;    /* -1 for FAT file systems */
        __time32_t  time_access;    /* -1 for FAT file systems */
        __time32_t  time_write;
        _fsize_t    size;
        char        name[260];
};

struct _finddata32i64_t {
        unsigned    attrib;
        __time32_t  time_create;    /* -1 for FAT file systems */
        __time32_t  time_access;    /* -1 for FAT file systems */
        __time32_t  time_write;
        __int64     size;
        char        name[260];
};

struct _finddata64i32_t {
        unsigned    attrib;
        __time64_t  time_create;    /* -1 for FAT file systems */
        __time64_t  time_access;    /* -1 for FAT file systems */
        __time64_t  time_write;
        _fsize_t    size;
        char        name[260];
};

struct __finddata64_t {
        unsigned    attrib;
        __time64_t  time_create;    /* -1 for FAT file systems */
        __time64_t  time_access;    /* -1 for FAT file systems */
        __time64_t  time_write;
        __int64     size;
        char        name[260];
};

#ifdef _USE_32BIT_TIME_T
#define _finddata_t     _finddata32_t
#define _finddatai64_t  _finddata32i64_t

#define _findfirst      _findfirst32
#define _findnext       _findnext32
#define _findfirsti64   _findfirst32i64
#define _findnexti64     _findnext32i64

#else
#define _finddata_t     _finddata64i32_t
#define _finddatai64_t  __finddata64_t

#define _findfirst      _findfirst64i32
#define _findnext       _findnext64i32
#define _findfirsti64   _findfirst64
#define _findnexti64    _findnext64

#endif


#define _FINDDATA_T_DEFINED
#endif

#ifndef _WFINDDATA_T_DEFINED
#if 0
struct _wfinddata32_t {
        unsigned    attrib;
        __time32_t  time_create;    /* -1 for FAT file systems */
        __time32_t  time_access;    /* -1 for FAT file systems */
        __time32_t  time_write;
        _fsize_t    size;
        wchar_t     name[260];
};

struct _wfinddata32i64_t {
        unsigned    attrib;
        __time32_t  time_create;    /* -1 for FAT file systems */
        __time32_t  time_access;    /* -1 for FAT file systems */
        __time32_t  time_write;
        __int64     size;
        wchar_t     name[260];
};

struct _wfinddata64i32_t {
        unsigned    attrib;
        __time64_t  time_create;    /* -1 for FAT file systems */
        __time64_t  time_access;    /* -1 for FAT file systems */
        __time64_t  time_write;
        _fsize_t    size;
        wchar_t     name[260];
};

struct _wfinddata64_t {
        unsigned    attrib;
        __time64_t  time_create;    /* -1 for FAT file systems */
        __time64_t  time_access;    /* -1 for FAT file systems */
        __time64_t  time_write;
        __int64     size;
        wchar_t     name[260];
};
#endif
#ifdef _USE_32BIT_TIME_T
#define _wfinddata_t    _wfinddata32_t
#define _wfinddatai64_t _wfinddata32i64_t

#define _wfindfirst     _wfindfirst32
#define _wfindnext      _wfindnext32
#define _wfindfirsti64  _wfindfirst32i64
#define _wfindnexti64   _wfindnext32i64

#else
#define _wfinddata_t    _wfinddata64i32_t
#define _wfinddatai64_t _wfinddata64_t

#define _wfindfirst     _wfindfirst64i32
#define _wfindnext      _wfindnext64i32
#define _wfindfirsti64  _wfindfirst64
#define _wfindnexti64   _wfindnext64

#endif

#define _WFINDDATA_T_DEFINED
#endif

#define SYMBOL_CHARSET          2
typedef long HRESULT;
#ifndef S_OK
#define S_OK                            ((HRESULT)0L)
#endif
#define FreeProcInstance(lpProc) (lpProc)
#define MakeProcInstance(lpProc,hInstance) (lpProc)
#define GMEM_ZEROINIT       0x0040
#define GMEM_MOVEABLE       0x0002
#define GHND                (GMEM_MOVEABLE | GMEM_ZEROINIT)
#define MONITOR_DEFAULTTOPRIMARY    0x00000001
#define MAXLONG     0x7fffffff
#define TEXT _RCT
#define CSIDL_PERSONAL                  0x0005        // My Documents
#define CSIDL_MYDOCUMENTS               CSIDL_PERSONAL //  Personal was just a silly name for My Documents
#ifndef SUCCESS_SUCCESS
#define SUCCESS_SUCCESS                 0
#endif
#define ERROR_SUCCESS                    0L
//#define SPECSTRINGIZE( x ) #x
//#define __readableTo(extent) __declspec("SAL_readableTo("SPECSTRINGIZE(extent)")")
//#define __nullterminated __readableTo(sentinel(0))
#define __nullterminated
#define SOCK_STREAM     1               /* stream socket */
#define SOCKET_ERROR            (-1)
#define MAPI_LOGON_UI           0x00000001  /* Display logon UI					*/
#ifndef MAPI_ORIG				/* also defined in mapi.h */
#define MAPI_ORIG   0           /* Recipient is message originator          */
#define MAPI_TO     1           /* Recipient is a primary recipient         */
#define MAPI_CC     2           /* Recipient is a copy recipient            */
#define MAPI_BCC    3           /* Recipient is blind copy recipient        */
#define MAPI_P1		0x10000000	/* Recipient is a P1 resend recipient       */
#define MAPI_SUBMITTED 0x80000000 /* Recipient is already processed         */
/* #define MAPI_AUTHORIZE 4		   recipient is a CMC authorizing user		*/
/*#define MAPI_DISCRETE 0x10000000 Recipient is a P1 resend recipient       */
#endif
#define MAPI_INIT_VERSION				0
#define HKEY_CURRENT_USER                   (( HKEY ) (ULONG_PTR)((LONG)0x80000001) )
#define HKEY_LOCAL_MACHINE                  (( HKEY ) (ULONG_PTR)((LONG)0x80000002) )
#define HKEY_CLASSES_ROOT                   (( HKEY ) (ULONG_PTR)((LONG)0x80000000) )
#define REG_SZ                      ( 1 )   // Unicode nul terminated string
#define INADDR_NONE             0xffffffff
#if defined(_WIN64)
 typedef unsigned __int64 ULONG_PTR;
#else
 typedef unsigned long ULONG_PTR;
#endif
typedef unsigned short USHORT;
#define MAKEINTRESOURCE


#define TRUE    1
#define FALSE   0
#define huge
#define __in
#define __out
#define FAILED(Status)     ((HRESULT)(Status)<0)
#define SUCCEEDED(Status)     ((HRESULT)(Status) >= 0)

#define STGM_DIRECT             0x00000000L
#define STGM_READ               0x00000000L
#define STGM_SHARE_EXCLUSIVE    0x00000010L
#define STGM_READWRITE          0x00000002L
#define STGM_CREATE             0x00001000L
#define STGM_WRITE              0x00000001L

#define TA_CENTER                    6
#define TA_BOTTOM                    8

#define AF_INET         2               /* internetwork: UDP, TCP, etc. */

#define ODA_DRAWENTIRE  0x0001

#define WS_OVERLAPPED       0x00000000L
#define WS_CHILD            0x40000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_VSCROLL          0x00200000L
#define WS_TILED            WS_OVERLAPPED
#define WS_ICONIC           WS_MINIMIZE
#define WS_SIZEBOX          WS_THICKFRAME
#define WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW
#define WS_CAPTION          0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX    | \
                             WS_MAXIMIZEBOX)
#define WS_VISIBLE          0x10000000L
#define WS_DLGFRAME         0x00400000L
#define WS_EX_TOOLWINDOW        0x00000080L
#define WS_EX_LEFT              0x00000000L
#define WS_EX_LTRREADING        0x00000000L
#define WS_EX_RIGHTSCROLLBAR    0x00000000L
#define WS_CHILDWINDOW      (WS_CHILD)
#define WS_POPUP            0x80000000L
#define WS_BORDER           0x00800000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_POPUPWINDOW      (WS_POPUP          | \
                             WS_BORDER         | \
                             WS_SYSMENU)
#define WS_HSCROLL          0x00100000L
#define WS_DISABLED         0x08000000L

#define WM_PAINT            0x000F
#define WM_QUIT             0x0012
#define WM_RBUTTONDOWN      0x0204
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDBLCLK                0x0209
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_CHAR                         0x0102
#define WM_KEYUP                        0x0101
#define WM_KEYDOWN                      0x0100
#define WM_MOUSEMOVE                    0x0200
#define WM_ERASEBKGND                   0x0014
#define WM_NCLBUTTONDOWN                0x00A1
#define WM_NCHITTEST                    0x0084
#define WM_NCMOUSEMOVE                  0x00A0
#define WM_NCLBUTTONDBLCLK              0x00A3
#define WM_VSCROLL                      0x0115
#define WM_HSCROLL                      0x0114
#define WM_COMMAND                      0x0111
#define WM_TIMER                        0x0113
#define WM_SIZE                         0x0005
#define WM_MOVE                         0x0003
#define WM_GETMINMAXINFO                0x0024
#define WM_MENUSELECT                   0x011F
#define WM_USER                         0x0400
#define WM_QUERYENDSESSION              0x0011
#define WM_INITMENUPOPUP                0x0117
#define WM_CLOSE                        0x0010
#define WM_DROPFILES                    0x0233
#define WM_ENTERIDLE                    0x0121
#define WM_SETFOCUS                     0x0007
#define WM_WINDOWPOSCHANGING            0x0046
#define WM_DESTROY                      0x0002
#define WM_SYSCOMMAND                   0x0112
#define WM_INITMENU                     0x0116
#define WM_RBUTTONUP                    0x0205
#define WM_CTLCOLORBTN                  0x0135
#define WM_CTLCOLORSTATIC               0x0138
#define WM_CTLCOLORDLG                  0x0136
#define WM_INITDIALOG                   0x0110
#define WM_DRAWITEM                     0x002B
#define WM_NOTIFY                       0x004E
#define WM_KILLFOCUS                    0x0008
#define WM_EXITSIZEMOVE                 0x0232
#define WM_MOUSEFIRST                   0x0200

//#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
#define WM_MOUSEWHEEL                   0x020A
//#endif
//#if (_WIN32_WINNT >= 0x0500)
#define WM_XBUTTONDOWN                  0x020B
#define WM_XBUTTONUP                    0x020C
#define WM_XBUTTONDBLCLK                0x020D
//#endif
//#if (_WIN32_WINNT >= 0x0600)
#define WM_MOUSEHWHEEL                  0x020E
//#endif

//#if (_WIN32_WINNT >= 0x0600)
#define WM_MOUSELAST                    0x020E
//#elif (_WIN32_WINNT >= 0x0500)
//#define WM_MOUSELAST                    0x020D
//#elif (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
//#define WM_MOUSELAST                    0x020A
//#else
//#define WM_MOUSELAST                    0x0209
//#endif /* (_WIN32_WINNT >= 0x0600) */

#define WM_SETFONT                      0x0030
#define WM_NCACTIVATE                   0x0086
#define WM_GETTEXT                      0x000D
#define WM_SETTEXT                      0x000C

#define WH_MSGFILTER        (-1)

#define MF_DISABLED         0x00000002L
#define MF_BYPOSITION       0x00000400L
#define MF_SEPARATOR        0x00000800L
#define MF_STRING           0x00000000L
#define MF_CHECKED          0x00000008L
#define MF_UNCHECKED        0x00000000L

#define SC_SIZE         0xF000
#define SC_MOVE         0xF010

#define DS_MODALFRAME       0x80L   /* Can be combined with WS_CAPTION  */
#define DS_SETFONT          0x40L   /* User specified font for Dlg controls */
#define DS_3DLOOK           0x0004L
#define DS_CONTROL          0x0400L

#define ES_LEFT             0x0000L
#define ES_AUTOHSCROLL      0x0080L

#define BS_RADIOBUTTON      0x00000004L
#define BS_CHECKBOX         0x00000002L
#define BS_GROUPBOX         0x00000007L
#define BS_OWNERDRAW        0x0000000BL
#define BS_AUTOCHECKBOX     0x00000003L

#define CBS_DROPDOWNLIST      0x0003L

#define HWND_TOP        ((HWND)0)
#define HWND_BOTTOM     ((HWND)1)

#define PM_REMOVE           0x0001

#define BM_GETCHECK        0x00F0

#define SWP_NOZORDER        0x0004
#define SWP_NOMOVE          0x0002
#define SWP_NOSIZE          0x0001
#define SWP_FRAMECHANGED    0x0020  /* The frame changed: send WM_NCCALCSIZE */
#define SWP_SHOWWINDOW      0x0040
#define SWP_DRAWFRAME       SWP_FRAMECHANGED

#define SW_NORMAL           1
#define SW_SHOWNORMAL       1
#define SW_SHOW             5
#define SW_MAXIMIZE         3
#define SW_HIDE             0
#define SW_SHOWMINIMIZED    2

#define SB_HORZ             0
#define SB_VERT             1
#define SB_CTL              2
#define SB_BOTH             3

#define SB_LINEUP           0
#define SB_LINEDOWN         1
#define SB_PAGEDOWN         3
#define SB_PAGEUP           2
#define SB_THUMBPOSITION    4
#define SB_THUMBTRACK       5
#define SB_TOP              6
#define SB_BOTTOM           7
#define SB_ENDSCROLL        8

#define SM_CXSCREEN             0
#define SM_CYSCREEN             1
#define SM_CXMAXIMIZED          61
#define SM_CYMAXIMIZED          62
#define SM_CYCAPTION            4
#define SM_CXFRAME              32
#define SM_CYFRAME              33
#define SM_CYMINIMIZED          58
#define SM_CXDLGFRAME           7

#define SBS_VERT                    0x0001L

#define SBM_SETRANGE                0x00E2 /*not in win3.1 */

#define TPM_LEFTALIGN   0x0000L
#define TPM_LEFTBUTTON  0x0000L
#define TPM_RETURNCMD       0x0100L

#define PM_NOREMOVE         0x0000

#define GWL_STYLE           (-16)
#define GWL_EXSTYLE         (-20)
#define GWL_USERDATA        (-21)

#define GW_CHILD            5

#define BST_CHECKED        0x0001
#define BST_UNCHECKED      0x0000

#define BS_PUSHBUTTON       0x00000000L
#define BS_DEFPUSHBUTTON    0x00000001L
#define BS_LEFTTEXT         0x00000020L

#define CBS_DROPDOWN          0x0002L
#define CBS_SORT              0x0100L

#define SIZE_MAXIMIZED      2
/*
 * ShowWindow() Commands
 */
#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_NORMAL           1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_MAXIMIZE         3
#define SW_SHOWNOACTIVATE   4
#define SW_SHOW             5
#define SW_MINIMIZE         6
#define SW_SHOWMINNOACTIVE  7
#define SW_SHOWNA           8
#define SW_RESTORE          9
#define SW_SHOWDEFAULT      10
#define SW_FORCEMINIMIZE    11
#define SW_MAX              11

#define PD_RETURNDEFAULT               0x00000400
#define PD_PRINTSETUP                  0x00000040
#define PD_RETURNDC                    0x00000100
#define PD_NOSELECTION                 0x00000004
#define PD_NOPAGENUMS                  0x00000008
#define PD_ENABLEPRINTTEMPLATE         0x00004000
#define PD_ENABLEPRINTHOOK             0x00001000
#define PD_SELECTION                   0x00000001

#define EM_SETSEL               0x00B1

//#define IDC_SIZEWE          MAKEINTRESOURCE(32644)
#define IDC_SIZEWE            Qt::SizeHorCursor

#define MCI_STRING_OFFSET      512
#define MCI_OPEN                        0x0803
#define MCI_WAIT                        0x00000002L
#define MCI_OPEN_ELEMENT                0x00000200L
#define MCI_WINDOW                      0x0841
#define MCI_WHERE                       0x0843
#define MCI_DGV_WINDOW_HWND                 0x00010000L
#define MCI_DGV_WHERE_SOURCE                0x00020000L
#define MCI_PUT                         0x0842
#define MCI_DGV_RECT                        0x00010000L
#define MCI_DGV_PUT_SOURCE                  0x00020000L
#define MCI_DGV_PUT_DESTINATION             0x00040000L
#define MCI_SET                         0x080D
#define MCI_SET_ON                      0x00002000L
#define MCI_DGV_SET_SEEK_EXACTLY            0x00010000L
#define MCI_UPDATE                      0x0854
#define MCI_DGV_UPDATE_HDC                  0x00020000L
#define MCI_PLAY                        0x0806
#define MCI_DGV_PLAY_REVERSE                0x00020000L
#define MCI_STOP                        0x0808
#define MCI_SEEK                        0x0807
#define MCI_SEEK_TO_START               0x00000100L
#define MCI_PAUSE                       0x0809
#define MCI_STEP                        0x080E
#define MCI_DGV_STEP_FRAMES                 0x00020000L
#define MCI_STATUS_MODE                 0x00000004L
#define MCI_STATUS                      0x0814
#define MCI_STATUS_ITEM                 0x00000100L
#define MCI_MODE_STOP                   (MCI_STRING_OFFSET + 13)
#define MCI_MODE_PAUSE                  (MCI_STRING_OFFSET + 17)
#define MCI_DGV_STEP_REVERSE                0x00010000L
#define MCI_STATUS_POSITION             0x00000002L
#define MCI_CLOSE                       0x0804

#define TA_RIGHT                     2
#define TA_TOP                       0
#define TA_LEFT                      0

#define SEM_FAILCRITICALERRORS      0x0001
#define SEM_NOGPFAULTERRORBOX       0x0002
#define SEM_NOALIGNMENTFAULTEXCEPT  0x0004
#define SEM_NOOPENFILEERRORBOX      0x8000

#define NOTSRCCOPY          (DWORD)0x00330008 /* dest = (NOT source)             */
#define COLORONCOLOR                 3
#define HALFTONE                     4
#define HFILE_ERROR ((int)-1)
#define OF_EXIST            0x00004000
#define GGO_NATIVE         2
#define TT_PRIM_QSPLINE    2
#define DT_METAFILE         5   /* Metafile, VDM                    */
#define GCL_HICON           (-14)
#define HTCAPTION           2
#define FW_NORMAL               400
#define FW_BOLD             700
#define ANSI_CHARSET            0
#define OUT_TT_PRECIS           4
#define CLIP_DEFAULT_PRECIS     0
#define OUT_STROKE_PRECIS       3
#define CLIP_STROKE_PRECIS      2
#define PROOF_QUALITY           2
#define DEFAULT_QUALITY         0
#define FF_DONTCARE         (0<<4)  /* Don't care or don't know. */
#define DEFAULT_PITCH           0

#define HELP_CONTEXT      0x0001L  /* Display topic in ulTopic */
#define HELP_INDEX        0x0003L  /* Display index */
#define HH_DISPLAY_TOPIC        0x0000

#define CB_FINDSTRINGEXACT          0x0158
#define CB_DELETESTRING             0x0144
#define CB_SELECTSTRING             0x014D
#define CB_GETCURSEL                0x0147
#define CB_ERR              (-1)
#define CB_GETLBTEXT                0x0148
#define CB_ADDSTRING                0x0143
#define CB_GETCOUNT                 0x0146
#define CB_FINDSTRING               0x014C
#define CB_RESETCONTENT             0x014B
#define CB_SETDROPPEDWIDTH          0x0160
#define CB_SETITEMHEIGHT            0x0153
#define CB_SETCURSEL                0x014E


#define LB_GETCOUNT             0x018B
#define LB_ADDSTRING            0x0180
#define LB_GETTEXT              0x0189
#define LB_ERR              (-1)
#define LB_DELETESTRING         0x0182
#define LB_RESETCONTENT         0x0184
#define LB_GETCURSEL            0x0188
#define LB_FINDSTRING           0x018F
#define LB_GETSELCOUNT          0x0190
#define LB_GETSELITEMS          0x0191
#define LB_SELITEMRANGE         0x019B

#define CW_USEDEFAULT       ((int)0x80000000)

#define CBN_SELENDOK        9
#define CBN_CLOSEUP         8
#define CBN_SELCHANGE       5
#define CBN_SETFOCUS        3
#define CBN_KILLFOCUS       4

#define LBN_SELCHANGE       1
#define LBN_DBLCLK          2

#define BN_CLICKED          0

#define CF_TEXT             1
#define CF_BITMAP           2
#define CF_ENHMETAFILE      14

#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L
#define MB_ICONINFORMATION          MB_ICONASTERISK
#define MB_ICONSTOP                 MB_ICONHAND
#define MB_YESNO                    0x00000004L
#define MB_YESNOCANCEL              0x00000003L
#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_PRECOMPOSED            0x00000001  // use precomposed chars

#define IDOK                1
#define IDYES               6
#define IDNO                7
#define IDCANCEL            2

#define SRCCOPY           (DWORD)0x00CC0020 /* dest = source                   */

#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_RETURN         0x0D
#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_PRIOR          0x21
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_NEXT           0x22
#define VK_UP             0x26
#define VK_BACK           0x08
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_TAB            0x09
#define VK_ESCAPE         0x1B
#define VK_MENU           0x12
#define VK_F12            0x7B
#define VK_SPACE          0x20
#define FVIRTKEY  TRUE          /* Assumed to be == TRUE */

#define MK_LBUTTON          0x0001
#define MK_RBUTTON          0x0002
#define MK_SHIFT            0x0004
#define MK_CONTROL          0x0008
#define MK_MBUTTON          0x0010
#if(_WIN32_WINNT >= 0x0500)
#define MK_XBUTTON1         0x0020
#define MK_XBUTTON2         0x0040
#endif /* _WIN32_WINNT >= 0x0500 */

#define MF_ENABLED          0x00000000L
#define MF_GRAYED           0x00000001L
#define MF_BYCOMMAND        0x00000000L
#define MF_POPUP            0x00000010L

#define MSGF_DIALOGBOX      0
#define MSGF_MENU           2

#define CF_DIB              8

#define EN_SETFOCUS         0x0100
#define EN_KILLFOCUS        0x0200

#define OFN_SHOWHELP                 0x00000010
#define OFN_PATHMUSTEXIST            0x00000800
#define OFN_FILEMUSTEXIST            0x00001000
#define OFN_EXPLORER                 0x00080000     // new look commdlg
#define OFN_ENABLETEMPLATEHANDLE     0x00000080
#define OFN_ENABLEHOOK               0x00000020
#define OFN_SHOWHELP                 0x00000010
#define OFN_PATHMUSTEXIST            0x00000800
#define OFN_FILEMUSTEXIST            0x00001000

//#define IDC_ARROW           MAKEINTRESOURCE(32512)
#define IDC_ARROW           Qt::ArrowCursor
//#define IDC_WAIT            MAKEINTRESOURCE(32514)
#define IDC_WAIT            Qt::WaitCursor

#define SND_ASYNC           0x0001  /* play asynchronously */
#define SND_FILENAME    0x00020000L /* name is file name */

#define PS_SOLID            0
#define PS_DOT              2       /* .......  */

#define RGN_COPY            5

#define SS_ICON             0x00000003L
#define SS_REALSIZEIMAGE    0x00000800L
#define SS_CENTERIMAGE      0x00000200L

#define STM_SETICON         0x0170
#define STM_GETICON         0x0171

#define RDW_FRAME               0x0400
#define RDW_INTERNALPAINT       0x0002
#define RDW_INVALIDATE          0x0001

#define TRUETYPE_FONTTYPE   0x004
#define SRCINVERT           (unsigned long)0x00660046 /* dest = source XOR dest          */
#define OFN_OVERWRITEPROMPT          0x00000002
#define OFN_ALLOWMULTISELECT         0x00000200

#define CREATE_NEW_PROCESS_GROUP          0x00000200
#define STARTF_USESHOWWINDOW    0x00000001

#define BLACKONWHITE                 1
#define WHITEONBLACK                 2
#define WPF_SETMINPOSITION          0x0001
#define PHYSICALOFFSETX 112 /* Physical Printable Area x margin         */
#define PHYSICALOFFSETY 113 /* Physical Printable Area y margin         */
#define LOGPIXELSX    88    /* Logical pixels/inch in X                 */
#define LOGPIXELSY    90    /* Logical pixels/inch in Y                 */
#define ALTERNATE                    1

#define DT_RASPRINTER       2   /* Raster printer                   */
#define DT_RASDISPLAY       1   /* Raster display                   */
#define DT_PLOTTER          0   /* Vector plotter                   */

#define TECHNOLOGY    2     /* Device classification                    */
#define GETCOLORTABLE                5
#define R2_NOT              6   /* Dn       */
#define R2_COPYPEN          13  /* P        */
#define SRCPAINT            (DWORD)0x00EE0086 /* dest = source OR dest           */
#define SRCAND              (DWORD)0x008800C6 /* dest = source AND dest          */
#define DIB_RGB_COLORS      0 /* color table in RGBs */
#define BI_RGB        0L
#define PATINVERT           (DWORD)0x005A0049 /* dest = pattern XOR dest         */
#define WHITENESS           (DWORD)0x00FF0062 /* dest = WHITE                    */
#define OPAQUE              2
#define COLOR_BTNTEXT           18
#define COLOR_HIGHLIGHT         13
#define COLOR_HIGHLIGHTTEXT     14
#define COLOR_BTNFACE           15
#define COLOR_BTNSHADOW         16
#define COLOR_BTNHIGHLIGHT      20
#define COLOR_3DDKSHADOW        21
#define COLOR_3DLIGHT           22
#define COLOR_3DFACE            COLOR_BTNFACE
#define COLOR_3DSHADOW          COLOR_BTNSHADOW
#define COLOR_BTNHILIGHT        COLOR_BTNHIGHLIGHT
#define COLOR_3DHIGHLIGHT       COLOR_BTNHIGHLIGHT
#define COLOR_3DHILIGHT         COLOR_BTNHIGHLIGHT
#define TRANSPARENT         1
#define WHITE_BRUSH         0
#define LTGRAY_BRUSH        1
#define BLACK_BRUSH         4
#define NULL_BRUSH          5
#define WHITE_PEN           6
#define BLACK_PEN           7
#define NULL_PEN            8
#define GRAY_BRUSH          2
#define HORZSIZE      4     /* Horizontal size in millimeters           */
#define VERTSIZE      6     /* Vertical size in millimeters             */
#define HORZRES       8     /* Horizontal width in pixels               */
#define VERTRES       10    /* Vertical height in pixels                */
#define LF_FACESIZE 32
#define LF_FULLFACESIZE 64
#define OFS_MAXPATHNAME 128
#ifndef CCHDEVICENAME
#define CCHDEVICENAME 32
#endif
#define CCHFORMNAME 32
#define CONST const
#define RCWINAPI WINAPI
#define NEAR
#define FAR
#define VOID void
#define CALLBACK __stdcall
#define WINAPI __stdcall
#define PASCAL  __stdcall
#ifndef FARPROC
#if WINVER >= 0x0602
typedef INT_PTR (WINAPI *FARPROC)(void);
#else
typedef int (__stdcall *FARPROC)(void);
#endif
#endif
#define RCFARPROC FARPROC
#ifndef APIENTRY
#define APIENTRY FAR __stdcall
#endif

// basic data types
//typedef void NMHDR;
typedef QChar RCCHAR;
typedef QChar RCWCHAR;
typedef RCCHAR* RCPCHAR;
typedef QString RCSTRING;
typedef wchar_t WCHAR;
#ifdef UNICODE
 typedef WCHAR TCHAR;
#else
 typedef char TCHAR;
#endif
typedef void *LPVOID;
typedef LPVOID RCLPVOID;
typedef char CHAR;
typedef CHAR *PSTR;
typedef unsigned char UCHAR;
typedef unsigned char BYTE;
typedef BYTE FAR *LPBYTE;
#ifdef UNICODE
    typedef WCHAR BCHAR;
#else
    typedef BYTE BCHAR;
#endif
//
// ANSI (Multi-byte Character) types
//
typedef CHAR *PCHAR, *LPCH, *PCH;
typedef CONST CHAR *LPCCH, *PCCH;
typedef __nullterminated CHAR *NPSTR, *LPSTR, *PSTR;
typedef __nullterminated PSTR *PZPSTR;
typedef __nullterminated CONST PSTR *PCZPSTR;
typedef __nullterminated CONST CHAR *LPCSTR, *PCSTR;
typedef __nullterminated PCSTR *PZPCSTR;
typedef short SHORT;
typedef long LONG; // W 32 M 64
typedef unsigned long ULONG; // W 32 M 64
typedef LONG *PLONG;
typedef int32_t RCLONG;
typedef int32_t *RCPLONG;
typedef uint32_t RCULONG;
typedef int64_t RCLONGLONG;
typedef int64_t *RCPLONGLONG;
typedef uint64_t RCULONGLONG;
typedef unsigned int RCSIZE_T;
typedef unsigned int UINT;
typedef unsigned int RCUINT;
typedef unsigned int UINT32;
typedef USHORT RCUSHORT;
typedef unsigned short  u_short;
typedef unsigned char BYTE;
typedef unsigned char RCBYTE;
typedef unsigned short WORD; // W 16 M 16
typedef WORD* LPWORD;
typedef WORD  *PWORD;
typedef WORD RCWORD;
typedef int RCINT;
typedef int* RCLPINT;
typedef double RCDOUBLE;
typedef float RCFLOAT;
typedef char RCACHAR;
typedef int RCBOOL;
typedef int BOOL;
typedef unsigned long DWORD; // W 32  M 64
typedef DWORD *LPDWORD;
typedef DWORD RCDWORD;
typedef RCDWORD *RCLPDWORD;
#if defined(_WIN64)
 typedef _int64 LONG_PTR;
#else
 typedef long LONG_PTR;
#endif
typedef LONG_PTR LPARAM;
typedef LONG_PTR RCLPARAM;
typedef LONG_PTR RCLONG_PTR;
typedef RCLONG_PTR RCLRESULT;
typedef void *PVOID;
typedef void RCVOID;
typedef void* RCPVOID;
typedef void* RCLPVOID;
typedef PVOID HANDLE; // defined in Qt headers
typedef HANDLE  HLOCAL;
#ifdef _MAC
typedef HANDLE HWND;
//typedef HANDLE RCHFONT;
typedef HANDLE HICON;
typedef HANDLE HDC;
//typedef HANDLE RCHBITMAP;
typedef HANDLE HPALETTE;
typedef HANDLE RCHINSTANCE;
typedef HANDLE HMONITOR;
//typedef HANDLE RCHPEN;
//typedef HANDLE RCHRGN;
typedef HICON HCURSOR;
typedef HANDLE HDROP;
typedef HANDLE HACCEL;
typedef HANDLE HMENU;
#endif
//typedef HANDLE HDROP;
//typedef HANDLE HACCEL;
//typedef HANDLE HMENU;
typedef HANDLE RCHANDLE;
typedef HANDLE RCHWND;
typedef HANDLE RCHMENU;

typedef QIcon RCHICON;
typedef QIcon *RCPICON;
//#define HCURSOR QCursor
//typedef HCURSOR RCHCURSOR;
//typedef HCURSOR RCHCURSOR;
class RCCursor;
typedef RCCursor* RCHCURSOR;
typedef HANDLE RCHINSTANCE;
typedef HANDLE RCHDC;
//typedef QObject* RCHGDIOBJ;
typedef const RCCHAR* RCLPCSTR;
typedef RCCHAR* RCLPTSTR;
typedef const RCCHAR* RCLPCWSTR;
typedef WCHAR *LPWSTR;
typedef RCLPTSTR RCLPWSTR;
typedef __nullterminated CONST CHAR *LPCSTR;
typedef CONST WCHAR *LPCWSTR;
#ifdef UNICODE
typedef LPCWSTR LPCTSTR;
#else
typedef LPCSTR LPCTSTR;
#endif
#if defined(_WIN64)
 typedef __int64 INT_PTR;
#elif defined(_MAC)
typedef int64_t INT_PTR;
#else
 typedef int INT_PTR;
#endif
typedef INT_PTR RCINT_PTR;
#if defined(_WIN64)
 typedef unsigned __int64 UINT_PTR;
#elif defined(_MAC)
typedef uint64_t UINT_PTR;
#else
 typedef unsigned int UINT_PTR;
#endif
typedef UINT_PTR RCUNIT_PTR;
typedef UINT_PTR WPARAM;
typedef UINT_PTR RCUINT_PTR ;
typedef WPARAM RCWPARAM;
typedef CHAR *LPSTR;
typedef WCHAR *LPWSTR;
#ifdef UNICODE
 typedef LPWSTR LPTSTR;
#else
 typedef LPSTR LPTSTR;
#endif
typedef LONG_PTR LRESULT;
#ifdef _MAC
typedef HANDLE HHOOK;
#endif
typedef HHOOK RCHHOOK;

#ifdef _MAC
typedef struct _OSVERSIONINFO{
    DWORD dwOSVersionInfoSize;
  DWORD dwMajorVersion;
  DWORD dwMinorVersion;
  DWORD dwBuildNumber;
  DWORD dwPlatformId;
  TCHAR szCSDVersion[128];
} OSVERSIONINFO;
typedef OSVERSIONINFO *LPOSVERSIONINFO;
typedef struct tagSIZE {
  LONG cx;
  LONG cy;
} SIZE, *PSIZE, *LPSIZE;
typedef struct _POINTL {
  LONG x;
  LONG y;
} POINTL, *PPOINTL;
typedef struct tagPOINT {
  LONG x;
  LONG y;
} POINT, *PPOINT;
#endif
typedef LPSIZE RCLPSIZE;
typedef POINT* RCLPPOINT;
typedef POINT RCPOINT;
#ifdef _MAC
typedef struct tagMINMAXINFO {
    POINT ptReserved;
    POINT ptMaxSize;
    POINT ptMaxPosition;
    POINT ptMinTrackSize;
    POINT ptMaxTrackSize;
} MINMAXINFO, *PMINMAXINFO, *LPMINMAXINFO;
typedef struct _RECT {
  RCLONG left;
  RCLONG top;
  RCLONG right;
  RCLONG bottom;
} RECT, *PRECT, *LPRECT;
#endif
typedef LPRECT RCLPRECT;
typedef RECT RCRECT;

#ifdef _MAC
typedef struct tagRGBQUAD {
  BYTE    rgbBlue;
  BYTE    rgbGreen;
  BYTE    rgbRed;
  BYTE    rgbReserved;
} RGBQUAD;
typedef struct tagBITMAP {
  LONG   bmType;
  LONG   bmWidth;
  LONG   bmHeight;
  LONG   bmWidthBytes;
  WORD   bmPlanes;
  WORD   bmBitsPixel;
  LPVOID bmBits;
} BITMAP, *PBITMAP;
typedef struct tagBITMAPINFOHEADER{
  /*DWORD*/ unsigned int biSize; // W 32 M 64
  /*LONG*/ int biWidth; // W 32 M 64
  /*LONG*/ int biHeight;// W 32 M 64
  WORD biPlanes; // W 16 M 16
  WORD biBitCount; // W 16 M 16
  /*DWORD*/ unsigned int biCompression;// W 32 M 64
  /*DWORD*/ unsigned int biSizeImage; // W 32 M 64
  /*LONG*/ int biXPelsPerMeter;// W 32 M 64
  /*LONG*/ int biYPelsPerMeter;// W 32 M 64
  /*DWORD*/ unsigned int biClrUsed; // W 32 M 64
  /*DWORD*/ unsigned int biClrImportant;// W 32 M 64
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
typedef struct tagBITMAPINFO {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD          bmiColors[1];
} BITMAPINFO, *PBITMAPINFO, *LPBITMAPINFO;
typedef struct tagBITMAPFILEHEADER {
  WORD    bfType;
  DWORD   bfSize;
  WORD    bfReserved1;
  WORD    bfReserved2;
  DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;
#endif
typedef QImage RCIMAGE;
typedef RCIMAGE * RCPIMAGE;
typedef BITMAPINFO RCBITMAPINFO;
typedef RCBITMAPINFO * RCLPBITMAPINFO;
//typedef QImage RCBITMAPINFO;
//typedef RCBITMAPINFO * RCLPBITMAPINFO;
//typedef DWORD RCCOLORREF;
//typedef DWORD *LPRCCOLORREF;
typedef uint32_t *LPRCCOLORREF;
typedef uint32_t RCCOLORREF;
#ifdef _MAC
typedef struct tagPALETTEENTRY {
  BYTE peRed;
  BYTE peGreen;
  BYTE peBlue;
  BYTE peFlags;
} PALETTEENTRY, *PPALETTEENTRY, FAR *LPPALETTEENTRY;
typedef struct tagPANOSE {
  BYTE bFamilyType;
  BYTE bSerifStyle;
  BYTE bWeight;
  BYTE bProportion;
  BYTE bContrast;
  BYTE bStrokeVariation;
  BYTE bArmStyle;
  BYTE bLetterform;
  BYTE bMidline;
  BYTE bXHeight;
} PANOSE, *LPPANOSE;
typedef struct tagTEXTMETRIC {
  LONG tmHeight;
  LONG tmAscent;
  LONG tmDescent;
  LONG tmInternalLeading;
  LONG tmExternalLeading;
  LONG tmAveCharWidth;
  LONG tmMaxCharWidth;
  LONG tmWeight;
  LONG tmOverhang;
  LONG tmDigitizedAspectX;
  LONG tmDigitizedAspectY;
  TCHAR tmFirstChar;
  TCHAR tmLastChar;
  TCHAR tmDefaultChar;
  TCHAR tmBreakChar;
  BYTE tmItalic;
  BYTE tmUnderlined;
  BYTE tmStruckOut;
  BYTE tmPitchAndFamily;
  BYTE tmCharSet;
} TEXTMETRIC, *PTEXTMETRIC, NEAR *NPTEXTMETRIC, FAR *LPTEXTMETRIC,
NEAR *NPTEXTMETRICW, FAR *LPTEXTMETRICW;
typedef struct _OUTLINETEXTMETRIC {
  RCUINT   otmSize;
  TEXTMETRIC otmTextMetrics;
  BYTE   otmFiller;
  PANOSE otmPanoseNumber;
  UINT   otmfsSelection;
  UINT   otmfsType;
  int    otmsCharSlopeRise;
  int    otmsCharSlopeRun;
  int    otmItalicAngle;
  UINT   otmEMSquare;
  int    otmAscent;
  int    otmDescent;
  UINT   otmLineGap;
  UINT   otmsCapEmHeight;
  UINT   otmsXHeight;
  RECT   otmrcFontBox;
  int    otmMacAscent;
  int    otmMacDescent;
  UINT   otmMacLineGap;
  UINT   otmusMinimumPPEM;
  POINT  otmptSubscriptSize;
  POINT  otmptSubscriptOffset;
  POINT  otmptSuperscriptSize;
  POINT  otmptSuperscriptOffset;
  UINT   otmsStrikeoutSize;
  int    otmsStrikeoutPosition;
  int    otmsUnderscoreSize;
  int    otmsUnderscorePosition;
  PSTR   otmpFamilyName;
  PSTR   otmpFaceName;
  PSTR   otmpStyleName;
  PSTR   otmpFullName;
} OUTLINETEXTMETRIC, *POUTLINETEXTMETRIC, NEAR *NPOUTLINETEXTMETRICW, FAR *LPOUTLINETEXTMETRICW,
NEAR *NPOUTLINETEXTMETRIC, FAR *LPOUTLINETEXTMETRIC;
#endif
typedef LPOUTLINETEXTMETRIC RCLPOUTLINETEXTMETRIC;
typedef LPTEXTMETRIC RCLPTEXTMETRIC;
typedef LPTEXTMETRICW RCLPTEXTMETRICW;
typedef RCPCHAR RCLPCTSTR;
#ifdef _MAC
//typedef HANDLE RCHBRUSH;
#endif

typedef void (CALLBACK* TIMERPROC)(HWND, UINT, UINT_PTR, DWORD);
typedef TIMERPROC RCTIMERPROC;
#ifdef _MAC
typedef struct _WINDOWPLACEMENT {
    UINT length;
    UINT flags;
    UINT showCmd;
    POINT ptMinPosition;
    POINT ptMaxPosition;
    RECT rcNormalPosition;
} WINDOWPLACEMENT;
#endif
typedef WINDOWPLACEMENT RCWINDOWPLACEMENT;
typedef RCLONG RCHRESULT;
typedef void* RCHMODULE;
typedef unsigned short RCWORD;
typedef INT_PTR (CALLBACK *DLGPROC)(
        HWND hwndDlg,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam
);
typedef  DLGPROC RCDLGPROC;
#ifdef _MAC
typedef struct {
    DWORD style;
    DWORD dwExtendedStyle;
    WORD cdit;
    short x;
    short y;
    short cx;
    short cy;
} DLGTEMPLATE, *LPDLGTEMPLATE;
typedef const LPDLGTEMPLATE LPCDLGTEMPLATE;
#endif
typedef LPDLGTEMPLATE RCLPDLGTEMPLATE;
typedef LPCDLGTEMPLATE RCLPCDLGTEMPLATE;
//typedef FILE RCFILE;
typedef QFile RCFILE;
#ifdef _MAC
typedef dev_t _dev_t;
typedef wint_t _ino_t;
typedef off_t _off_t;
#endif
/*
struct RC_STAT {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };
*/
typedef struct stat RC_STAT;
typedef ULONG_PTR RCULONG_PTR;
typedef ULONG_PTR DWORD_PTR;

#ifdef _MAC
typedef struct tagMSG {
    HWND        hwnd;
    UINT        message;
    WPARAM      wParam;
    LPARAM      lParam;
    DWORD       time;
    POINT       pt;
#ifdef _MAC
    DWORD       lPrivate;
#endif
} MSG, *PMSG, NEAR *NPMSG, FAR *LPMSG;
#endif
typedef struct {
    RCHWND hwnd;
    RCUINT message;
    RCWPARAM wParam;
    RCLPARAM lParam;
    RCDWORD time;
    RCPOINT pt;
} RCMSG, *RCPMSG;
typedef RCPMSG RCLPMSG;

typedef short RCSHORT;
typedef struct _RCGLYPHMETRICS {
  RCUINT  gmBlackBoxX;
  RCUINT  gmBlackBoxY;
  RCPOINT gmptGlyphOrigin;
  RCSHORT gmCellIncX;
  RCSHORT gmCellIncY;
} RCGLYPHMETRICS, *RCLPGLYPHMETRICS;
#ifdef _MAC
typedef struct _FIXED {
  WORD  fract;
  short value;
} FIXED;
typedef struct _MAT2 {
  FIXED eM11;
  FIXED eM12;
  FIXED eM21;
  FIXED eM22;
} MAT2, *LPMAT2;
#endif
typedef MAT2 RCMAT2;
#ifdef _MAC
typedef struct tagLOGFONT {
  LONG lfHeight;
  LONG lfWidth;
  LONG lfEscapement;
  LONG lfOrientation;
  LONG lfWeight;
  BYTE lfItalic;
  BYTE lfUnderline;
  BYTE lfStrikeOut;
  BYTE lfCharSet;
  BYTE lfOutPrecision;
  BYTE lfClipPrecision;
  BYTE lfQuality;
  BYTE lfPitchAndFamily;
  TCHAR lfFaceName[LF_FACESIZE];
} LOGFONT, *PLOGFONT;
typedef LOGFONT LOGFONTA;
typedef PLOGFONT PLOGFONTA;
typedef struct {
    LONG lfHeight;
    LONG lfWidth;
    LONG lfEscapement;
    LONG lfOrientation;
    LONG lfWeight;
    BYTE lfItalic;
    BYTE lfUnderline;
    BYTE lfStrikeOut;
    BYTE lfCharSet;
    BYTE lfOutPrecision;
    BYTE lfClipPrecision;
    BYTE lfQuality;
    BYTE lfPitchAndFamily;
    WCHAR lfFaceName[LF_FACESIZE];
} LOGFONTW;
typedef struct tagENUMLOGFONT {
  LOGFONT elfLogFont;
  TCHAR   elfFullName[LF_FULLFACESIZE];
  TCHAR   elfStyle[LF_FACESIZE];
} ENUMLOGFONT, *LPENUMLOGFONT;
typedef struct tagNEWTEXTMETRIC {
  LONG   tmHeight;
  LONG   tmAscent;
  LONG   tmDescent;
  LONG   tmInternalLeading;
  LONG   tmExternalLeading;
  LONG   tmAveCharWidth;
  LONG   tmMaxCharWidth;
  LONG   tmWeight;
  LONG   tmOverhang;
  LONG   tmDigitizedAspectX;
  LONG   tmDigitizedAspectY;
  TCHAR  tmFirstChar;
  TCHAR  tmLastChar;
  TCHAR  tmDefaultChar;
  TCHAR  tmBreakChar;
  BYTE   tmItalic;
  BYTE   tmUnderlined;
  BYTE   tmStruckOut;
  BYTE   tmPitchAndFamily;
  BYTE   tmCharSet;
  DWORD  ntmFlags;
  UINT   ntmSizeEM;
  UINT   ntmCellHeight;
  UINT   ntmAvgWidth;
} NEWTEXTMETRIC, *PNEWTEXTMETRIC;
typedef TEXTMETRIC TEXTMETRICA;
typedef PTEXTMETRIC PTEXTMETRICA;
typedef TEXTMETRIC TEXTMETRICW;
#endif
typedef int (CALLBACK* OLDFONTENUMPROCA)(CONST LOGFONTA *, CONST TEXTMETRICA *, DWORD, LPARAM);
typedef int (CALLBACK* OLDFONTENUMPROCW)(CONST LOGFONTW *, CONST TEXTMETRICW *, DWORD, LPARAM);
#ifdef UNICODE
#define OLDFONTENUMPROC  OLDFONTENUMPROCW
#else
#define OLDFONTENUMPROC  OLDFONTENUMPROCA
#endif // !UNICODE
typedef OLDFONTENUMPROCA    FONTENUMPROCA;
typedef OLDFONTENUMPROCW    FONTENUMPROCW;
#ifdef UNICODE
typedef FONTENUMPROCW FONTENUMPROC;
#else
typedef FONTENUMPROCA FONTENUMPROC;
#endif // UNICODE
typedef FONTENUMPROC RCFONTENUMPROC;
typedef FONTENUMPROCW RCFONTENUMPROCW;
typedef void* RCWINOLEAPI;
typedef UINT_PTR (CALLBACK *OFNHookProc)(HWND,UINT,WPARAM,LPARAM);
#ifdef _MAC
typedef OFNHookProc *LPOFNHOOKPROC;
#endif
typedef LPOFNHOOKPROC RCLPOFNHOOKPROC;
typedef struct tagOFN {
  RCDWORD         lStructSize;
  RCHWND          hwndOwner;
  RCHINSTANCE     hInstance;
  RCLPCTSTR       lpstrFilter;
  RCLPTSTR        lpstrCustomFilter;
  RCDWORD         nMaxCustFilter;
  RCDWORD         nFilterIndex;
  RCLPTSTR        lpstrFile;
  RCDWORD         nMaxFile;
  RCLPTSTR        lpstrFileTitle;
  RCDWORD         nMaxFileTitle;
  RCLPCTSTR       lpstrInitialDir;
  RCLPCTSTR       lpstrTitle;
  RCDWORD         Flags;
  RCWORD          nFileOffset;
  RCWORD          nFileExtension;
  RCLPCTSTR       lpstrDefExt;
  RCLPARAM        lCustData;
  RCLPOFNHOOKPROC lpfnHook;
  RCLPCTSTR       lpTemplateName;
#if (_WIN32_WINNT >= 0x0500)
  void *        pvReserved;
  RCDWORD         dwReserved;
  RCDWORD         FlagsEx;
#endif // (_WIN32_WINNT >= 0x0500)
} RCOPENFILENAME, *RCLPOPENFILENAME;
#define OPENFILENAME RCOPENFILENAME
#define LPOPENFILENAME RCLPOPENFILENAME
#define LPOPENFILENAMEA RCLPOPENFILENAME
#define RCLPOPENFILENAMEA LPOPENFILENAMEA
typedef RCWORD* RCDWORD_PTR;
typedef RCCHAR* RCLPSTR;
#ifdef _MAC
typedef int errno_t;
#endif
typedef errno_t RCERRNO_T;
#ifdef _MAC
typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES,
*PSECURITY_ATTRIBUTES,
*LPSECURITY_ATTRIBUTES;
#endif
typedef LPSECURITY_ATTRIBUTES RCLPSECURITY_ATTRIBUTES;
#ifdef _MAC
typedef struct _STARTUPINFO {
    DWORD cb;
    LPTSTR lpReserved;
    LPTSTR lpDesktop;
    LPTSTR lpTitle;
    DWORD dwX;
    DWORD dwY;
    DWORD dwXSize;
    DWORD dwYSize;
    DWORD dwXCountChars;
    DWORD dwYCountChars;
    DWORD dwFillAttribute;
    DWORD dwFlags;
    WORD wShowWindow;
    WORD cbReserved2;
    LPBYTE lpReserved2;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
} STARTUPINFO,  *LPSTARTUPINFO;
#endif
typedef STARTUPINFO RCSTARTUPINFO;
typedef LPSTARTUPINFO RCLPSTARTUPINFO;
#ifdef _MAC
typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION,  *LPPROCESS_INFORMATION;
#endif
typedef PROCESS_INFORMATION RCPROCESS_INFORMATION;
typedef LPPROCESS_INFORMATION RCLPPROCESS_INFORMATION;

typedef intptr_t RCintptr_t;
#ifdef _MAC
typedef HANDLE HKEY;
#endif
typedef HKEY RCHKEY;
typedef DWORD ACCESS_MASK;
typedef ACCESS_MASK* PACCESS_MASK;
typedef ACCESS_MASK REGSAM;
typedef REGSAM RCREGSAM;
typedef HKEY *PHKEY;
typedef PHKEY RCPHKEY;
typedef time_t RCtime_t;

#define RCJPEG_CORE_PROPERTIES JPEG_CORE_PROPERTIES
typedef int HFILE;
typedef HFILE RCHFILE;
#ifdef _MAC
typedef struct _OFSTRUCT {
    BYTE cBytes;
    BYTE fFixedDisk;
    WORD nErrCode;
    WORD Reserved1;
    WORD Reserved2;
    CHAR szPathName[OFS_MAXPATHNAME];
} OFSTRUCT,  *POFSTRUCT, FAR *LPOFSTRUCT;
#endif
typedef LPOFSTRUCT RCLPOFSTRUCT;
typedef struct {
    DWORD fccType;
    DWORD fccHandler;
    DWORD dwFlags;
    DWORD dwCaps;
    WORD  wPriority;
    WORD  wLanguage;
    DWORD dwScale;
    DWORD dwRate;
    DWORD dwStart;
    DWORD dwLength;
    DWORD dwInitialFrames;
    DWORD dwSuggestedBufferSize;
    DWORD dwQuality;
    DWORD dwSampleSize;
    RECT  rcFrame;
    DWORD dwEditCount;
    DWORD dwFormatChangeCount;
    char  szName[64];
} AVISTREAMINFO;
#define IAVIStream void
typedef IAVIStream FAR* PAVISTREAM;

/*
#ifdef _USE_32BIT_TIME_T
#define _finddata_t     _finddata32_t
#define _finddatai64_t  _finddata32i64_t

#define _findfirst      _findfirst32
#define _findnext       _findnext32
#define _findfirsti64   _findfirst32i64
#define _findnexti64     _findnext32i64

#else
#define _finddata_t     _finddata64i32_t
#define _finddatai64_t  __finddata64_t

#define _findfirst      _findfirst64i32
#define _findnext       _findnext64i32
#define _findfirsti64   _findfirst64
#define _findnexti64    _findnext64
#endif
*/
//typedef struct _finddata_t RC_finddata_t;
//#define RC_finddata_t _finddata_t
// Our own version of _finddata_t
// a class for passing Qt filters to it
class RCfindfilter
{
public:
    QDir::Filter mfilter;
    RCfindfilter()
    {
        mfilter = QDir::NoFilter;
    }
    RCfindfilter(QDir::Filter filter)
    {
        mfilter = filter;
    }
};
struct RC_finddata_t {
        RCfindfilter filter; // in : for passing Qt filters
        unsigned    attrib; // out using Qt equivalents
        //__time64_t  time_create;    /* -1 for FAT file systems */
        //__time64_t  time_access;    /* -1 for FAT file systems */
        //__time64_t  time_write;
        __int64     size; // out
        RCCHAR     name[260]; // out
};
typedef HANDLE HGLOBAL;
typedef HGLOBAL RCHGLOBAL;
typedef UINT_PTR (CALLBACK *PrintHookProc)(
        HWND hdlg,
        UINT uiMsg,
        WPARAM wParam,
        LPARAM lParam
);
typedef PrintHookProc LPPRINTHOOKPROC;
typedef LPPRINTHOOKPROC RCLPPRINTHOOKPROC;
typedef UINT_PTR (CALLBACK *SetupHookProc)(
        HWND hdlg,
        UINT uiMsg,
        WPARAM wParam,
        LPARAM lParam
);
typedef SetupHookProc LPSETUPHOOKPROC;
typedef LPSETUPHOOKPROC RCLPSETUPHOOKPROC;
typedef struct RCtagPD {
    RCDWORD lStructSize;
    RCHWND hwndOwner;
    RCHGLOBAL hDevMode;
    RCHGLOBAL hDevNames;
    RCHDC hDC;
    RCDWORD Flags;
    RCWORD nFromPage;
    RCWORD nToPage;
    RCWORD nMinPage;
    RCWORD nMaxPage;
    RCWORD nCopies;
    RCHINSTANCE hInstance;
    RCLPARAM lCustData;
    RCLPPRINTHOOKPROC lpfnPrintHook;
    RCLPSETUPHOOKPROC lpfnSetupHook;
    RCLPCTSTR lpPrintTemplateName;
    RCLPCTSTR lpSetupTemplateName;
    RCHGLOBAL hPrintTemplate;
    RCHGLOBAL hSetupTemplate;
} RCPRINTDLG, *RCLPPRINTDLG;
#define PRINTDLG RCPRINTDLG
#define LPPRINTDLG RCLPPRINTDLG
#define FAR
#define NEAR
#define far
#define near

/*
typedef struct
{
     RCULONG ulVersion;
     RCULONG ulFlags;
} RCMAPIINIT_0, FAR *RCLPMAPIINIT_0;
#define MAPIINIT_0 RCMAPIINIT_0
#define LPMAPIINIT_0 RCLPMAPIINIT_0
typedef struct {
     RCULONG ulReserved;
     RCULONG ulRecipClass;
     RCLPTSTR lpszName;
     RCLPTSTR lpszAddress;
     RCULONG ulEIDSize;
     RCLPVOID lpEntryID;
} RCMapiRecipDesc, FAR *RClpMapiRecipDesc;
#define MapiRecipDesc RCMapiRecipDesc
#define lpMapiRecipDesc RClpMapiRecipDesc
typedef struct {
     RCULONG ulReserved;
     RCULONG flFlags;
     RCULONG nPosition;
     RCLPTSTR lpszPathName;
     RCLPTSTR lpszFileName;
     RCLPVOID lpFileType;
} RCMapiFileDesc, FAR *RClpMapiFileDesc;
#define MapiFileDesc RCMapiFileDesc
#define lpMapiFileDesc RClpMapiFileDesc
typedef struct {
     RCULONG ulReserved;
     RCLPTSTR lpszSubject;
     RCLPTSTR lpszNoteText;
     RCLPTSTR lpszMessageType;
     RCLPTSTR lpszDateReceived;
     RCLPTSTR lpszConversationID;
     RCFLAGS flFlags;
     RClpMapiRecipDesc lpOriginator;
     RCULONG nRecipCount;
     RClpMapiRecipDesc lpRecips;
     RCULONG nFileCount;
     RClpMapiFileDesc lpFiles;
} RCMapiMessage, FAR *RClpMapiMessage;
#define MapiMessage RCMapiMessage
#define RClpMapiMessage RCULONG_PTR
#define lpMapiMesssage RClpMapiMessage
*/
#define SYNCHRONIZE                      (0x00100000L)
#define KEY_QUERY_VALUE         (0x0001)
#define KEY_ENUMERATE_SUB_KEYS  (0x0008)
#define KEY_NOTIFY              (0x0010)
#define READ_CONTROL                     (0x00020000L)
#define STANDARD_RIGHTS_READ             (READ_CONTROL)
#define KEY_READ                ((STANDARD_RIGHTS_READ       |\
                                  KEY_QUERY_VALUE            |\
                                  KEY_ENUMERATE_SUB_KEYS     |\
                                  KEY_NOTIFY)                 \
                                  &                           \
                                 (~SYNCHRONIZE))

#define WSADESCRIPTION_LEN      256
#define WSASYS_STATUS_LEN       128
#ifdef _MAC
typedef struct WSAData {
    WORD wVersion;
    WORD wHighVersion;
    char szDescription[WSADESCRIPTION_LEN+1];
    char szSystemStatus[WSASYS_STATUS_LEN+1];
    unsigned short iMaxSockets;
    unsigned short iMaxUdpDg;
    char FAR* lpVendorInfo;
} WSADATA,  *LPWSADATA;
#endif
typedef WSADATA RCWSADATA;
typedef LPWSADATA RCLPWSADATA;

typedef struct
{
    ULONG			ulVersion;
    ULONG			ulFlags;
} MAPIINIT_0, FAR *LPMAPIINIT_0;
typedef MAPIINIT_0 MAPIINIT;
typedef MAPIINIT FAR *LPMAPIINIT;

typedef struct
{
    RCULONG			ulVersion;
    RCULONG			ulFlags;
} RCMAPIINIT_0, FAR *RCLPMAPIINIT_0;
typedef RCMAPIINIT_0 RCMAPIINIT;
typedef RCMAPIINIT FAR *RCLPMAPIINIT;

#ifndef __LHANDLE
#define __LHANDLE
typedef ULONG_PTR LHANDLE, *LPLHANDLE;
#endif
typedef ULONG_PTR SIZE_T, *PSIZE_T;
typedef LONG_PTR SSIZE_T, *PSSIZE_T;
typedef LHANDLE RCLHANDLE;
typedef unsigned long  FLAGS;
typedef unsigned long  RCFLAGS;
typedef struct
{
    ULONG ulReserved;            /* Reserved for future use (must be 0)     */
    ULONG flFlags;               /* Flags                                   */
    ULONG nPosition;             /* character in text to be replaced by attachment */
    LPSTR lpszPathName;          /* Full path name of attachment file       */
    LPSTR lpszFileName;          /* Original file name (optional)           */
    LPVOID lpFileType;           /* Attachment file type (can be lpMapiFileTagExt) */
} MapiFileDesc, FAR * lpMapiFileDesc;

typedef struct
{
    RCULONG ulReserved;            /* Reserved for future use (must be 0)     */
    RCULONG flFlags;               /* Flags                                   */
    RCULONG nPosition;             /* character in text to be replaced by attachment */
    RCLPSTR lpszPathName;          /* Full path name of attachment file       */
    RCLPSTR lpszFileName;          /* Original file name (optional)           */
    RCLPVOID lpFileType;           /* Attachment file type (can be lpMapiFileTagExt) */
} RCMapiFileDesc, FAR * RClpMapiFileDesc;

typedef struct
{
    ULONG ulReserved;           /* Reserved for future use                  */
    ULONG ulRecipClass;         /* Recipient class                          */
                                /* MAPI_TO, MAPI_CC, MAPI_BCC, MAPI_ORIG    */
    LPSTR lpszName;             /* Recipient name                           */
    LPSTR lpszAddress;          /* Recipient address (optional)             */
    ULONG ulEIDSize;            /* Count in bytes of size of pEntryID       */
    LPVOID lpEntryID;           /* System-specific recipient reference      */
} MapiRecipDesc, FAR * lpMapiRecipDesc;

typedef struct
{
    RCULONG ulReserved;           /* Reserved for future use                  */
    RCULONG ulRecipClass;         /* Recipient class                          */
                                /* MAPI_TO, MAPI_CC, MAPI_BCC, MAPI_ORIG    */
    RCLPSTR lpszName;             /* Recipient name                           */
    RCLPSTR lpszAddress;          /* Recipient address (optional)             */
    RCULONG ulEIDSize;            /* Count in bytes of size of pEntryID       */
    RCLPVOID lpEntryID;           /* System-specific recipient reference      */
} RCMapiRecipDesc, FAR * RClpMapiRecipDesc;

typedef struct
{
    ULONG ulReserved;             /* Reserved for future use (M.B. 0)       */
    LPSTR lpszSubject;            /* Message Subject                        */
    LPSTR lpszNoteText;           /* Message Text                           */
    LPSTR lpszMessageType;        /* Message Class                          */
    LPSTR lpszDateReceived;       /* in YYYY/MM/DD HH:MM format             */
    LPSTR lpszConversationID;     /* conversation thread ID                 */
    FLAGS flFlags;                /* unread,return receipt                  */
    lpMapiRecipDesc lpOriginator; /* Originator descriptor                  */
    ULONG nRecipCount;            /* Number of recipients                   */
    lpMapiRecipDesc lpRecips;     /* Recipient descriptors                  */
    ULONG nFileCount;             /* # of file attachments                  */
    lpMapiFileDesc lpFiles;       /* Attachment descriptors                 */
} MapiMessage, FAR * lpMapiMessage;

typedef struct
{
    RCULONG ulReserved;             /* Reserved for future use (M.B. 0)       */
    RCLPSTR lpszSubject;            /* Message Subject                        */
    RCLPSTR lpszNoteText;           /* Message Text                           */
    RCLPSTR lpszMessageType;        /* Message Class                          */
    RCLPSTR lpszDateReceived;       /* in YYYY/MM/DD HH:MM format             */
    RCLPSTR lpszConversationID;     /* conversation thread ID                 */
    RCFLAGS flFlags;                /* unread,return receipt                  */
    RClpMapiRecipDesc lpOriginator; /* Originator descriptor                  */
    RCULONG nRecipCount;            /* Number of recipients                   */
    RClpMapiRecipDesc lpRecips;     /* Recipient descriptors                  */
    RCULONG nFileCount;             /* # of file attachments                  */
    RClpMapiFileDesc lpFiles;       /* Attachment descriptors                 */
} RCMapiMessage, FAR * RClpMapiMessage;

typedef ULONG (FAR PASCAL MAPISENDMAIL)(
    LHANDLE lhSession,
    ULONG_PTR ulUIParam,
    lpMapiMessage lpMessage,
    FLAGS flFlags,
    ULONG ulReserved
);
typedef MAPISENDMAIL FAR *LPMAPISENDMAIL;
MAPISENDMAIL MAPISendMail;

#ifdef _MAC
typedef struct tagPAINTSTRUCT {
    HDC         hdc;
    BOOL        fErase;
    RECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;
#endif
typedef struct tagRCPAINTSTRUCT {
    RCHDC         hdc;
    RCBOOL        fErase;
    RCRECT        rcPaint;
    RCBOOL        fRestore;
    RCBOOL        fIncUpdate;
    RCBYTE        rgbReserved[32];
} RCPAINTSTRUCT, *RCPPAINTSTRUCT, *RCNPPAINTSTRUCT, *RCLPPAINTSTRUCT;
#ifdef _MAC
typedef struct tagWINDOWPOS {
    HWND    hwnd;
    HWND    hwndInsertAfter;
    int     x;
    int     y;
    int     cx;
    int     cy;
    UINT    flags;
} WINDOWPOS, *LPWINDOWPOS, *PWINDOWPOS;
#endif
typedef WINDOWPOS RCWINDOWPOS;
typedef LPWINDOWPOS RCLPWINDOWPOS;
typedef PWINDOWPOS RCPWINDOWPOS;
#ifdef _MAC
typedef struct tagDRAWITEMSTRUCT {
    UINT CtlType;
    UINT CtlID;
    UINT itemID;
    UINT itemAction;
    UINT itemState;
    HWND hwndItem;
    HDC hDC;
    RECT rcItem;
    ULONG_PTR itemData;
} DRAWITEMSTRUCT;
#endif
typedef DRAWITEMSTRUCT RCDRAWITEMSTRUCT;
#ifdef _MAC
typedef struct tagMONITORINFO
{
    DWORD   cbSize;
    RECT    rcMonitor;
    RECT    rcWork;
    DWORD   dwFlags;
} MONITORINFO, *LPMONITORINFO;
#endif
typedef MONITORINFO RCMONITORINFO;
typedef LPMONITORINFO RCLPMONITORINFO;
#ifdef _MAC
typedef struct tagNMHDR
{
    HWND      hwndFrom;
    UINT_PTR  idFrom;
    UINT      code;         // NM_ code
}   NMHDR;
typedef NMHDR FAR * LPNMHDR;
#endif
typedef LPNMHDR RCLPNMHDR;
typedef struct _NM_UPDOWN
{
    NMHDR hdr;
    int iPos;
    int iDelta;
} NMUPDOWN, *LPNMUPDOWN;

#ifndef _MCIERROR_              /* MCIERROR is defined in some post 3.1 apps */
#define _MCIERROR_
typedef DWORD   MCIERROR;       /* error return code, 0 means no error */
#endif
#ifndef _MCIDEVICEID_           /* Same with MCIDEVICEID */
#define _MCIDEVICEID_
typedef UINT    MCIDEVICEID;    /* MCI device ID type */
#endif
typedef struct {
    DWORD_PTR   dwCallback;
#ifdef MCI_USE_OFFEXT
    POINT   ptOffset;
    POINT   ptExtent;
#else
    RECT    rc;
#endif
    HDC     hDC;

#ifndef _WIN32
    WORD    wReserved0;
#endif
} MCI_DGV_UPDATE_PARMS;
typedef MCI_DGV_UPDATE_PARMS FAR * LPMCI_DGV_UPDATE_PARMS;
#ifdef _MAC
typedef struct tagMCI_PLAY_PARMS {
    DWORD_PTR   dwCallback;
    DWORD       dwFrom;
    DWORD       dwTo;
} MCI_PLAY_PARMS, *PMCI_PLAY_PARMS, FAR *LPMCI_PLAY_PARMS;
#endif
typedef MCI_PLAY_PARMS MCI_DGV_PLAY_PARMS;
typedef MCI_DGV_PLAY_PARMS FAR * LPMCI_DGV_PLAY_PARMS;

typedef struct {
    RCDWORD_PTR dwCallback;
    RCUINT      wDeviceID;
    RCLPSTR     lpstrDeviceType;
    RCLPSTR     lpstrElementName;
    RCLPSTR     lpstrAlias;
    RCDWORD     dwStyle;
    RCHWND      hWndParent;
} RCMCI_DGV_OPEN_PARMS;

typedef struct {
    RCDWORD_PTR dwCallback;
//    RCWORD      hWnd;
    RCHWND      hWnd;
    RCWORD      nCmdShow;
    RCLPSTR     lpstrText;
} RCMCI_DGV_WINDOW_PARMS;

typedef struct {
    RCDWORD_PTR dwCallback;
    RCRECT      rc;
} RCMCI_DGV_PUT_PARMS;

typedef struct {
    RCDWORD_PTR dwCallback;
    RCRECT      rc;
} RCMCI_DGV_RECT_PARMS;

typedef struct {
    RCDWORD_PTR dwCallback;
    RCDWORD     dwTimeFormat;
    RCDWORD     dwAudio;
} RCMCI_SET_PARMS;
#ifdef _MAC
typedef struct tagMCI_GENERIC_PARMS {
    DWORD_PTR   dwCallback;
} MCI_GENERIC_PARMS, *PMCI_GENERIC_PARMS, FAR *LPMCI_GENERIC_PARMS;
#endif
typedef MCI_GENERIC_PARMS MCI_DGV_STOP_PARMS;
typedef MCI_DGV_STOP_PARMS FAR * LPMCI_DGV_STOP_PARMS;
#ifdef _MAC
typedef struct tagMCI_SEEK_PARMS {
    DWORD_PTR   dwCallback;
    DWORD       dwTo;
} MCI_SEEK_PARMS, *PMCI_SEEK_PARMS, FAR *LPMCI_SEEK_PARMS;
#endif
typedef MCI_GENERIC_PARMS MCI_DGV_PAUSE_PARMS;
typedef MCI_DGV_PAUSE_PARMS FAR * LPMCI_DGV_PAUSE_PARMS;
typedef struct {
    DWORD_PTR dwCallback;
    DWORD_PTR dwReturn;
    DWORD   dwItem;
    DWORD   dwTrack;
    LPSTR   lpstrDrive;
    DWORD   dwReference;
} MCI_DGV_STATUS_PARMSA;
typedef struct {
    DWORD_PTR dwCallback;
    DWORD_PTR dwReturn;
    DWORD   dwItem;
    DWORD   dwTrack;
    LPWSTR  lpstrDrive;
    DWORD   dwReference;
} MCI_DGV_STATUS_PARMSW;
#ifdef UNICODE
typedef MCI_DGV_STATUS_PARMSW MCI_DGV_STATUS_PARMS;
#else
typedef MCI_DGV_STATUS_PARMSA MCI_DGV_STATUS_PARMS;
#endif // UNICODE
typedef MCI_DGV_STATUS_PARMSA FAR * LPMCI_DGV_STATUS_PARMSA;
typedef MCI_DGV_STATUS_PARMSW FAR * LPMCI_DGV_STATUS_PARMSW;
#ifdef UNICODE
typedef LPMCI_DGV_STATUS_PARMSW LPMCI_DGV_STATUS_PARMS;
#else
typedef LPMCI_DGV_STATUS_PARMSA LPMCI_DGV_STATUS_PARMS;
#endif // UNICODE
typedef struct {
    DWORD_PTR   dwCallback;
    DWORD   dwFrames;
} MCI_DGV_STEP_PARMS;
typedef MCI_DGV_STEP_PARMS FAR *LPMCI_DGV_STEP_PARMS;
#ifdef _MAC
typedef struct tagPOINTFX {
  FIXED x;
  FIXED y;
} POINTFX, *LPPOINTFX;
#endif
#ifdef _MAC
typedef struct _TTPOLYGONHEADER {
  DWORD   cb;
  DWORD   dwType;
  POINTFX pfxStart;
} TTPOLYGONHEADER, *LPTTPOLYGONHEADER;
#endif
#ifdef _MAC
typedef struct tagTTPOLYCURVE {
  WORD    wType;
  WORD    cpfx;
  POINTFX apfx[1];
} TTPOLYCURVE, *LPTTPOLYCURVE;
#endif
/* accelerator key stuct fVirt values */
#define FALT       0x10 /* The ALT key must be held down when the accelerator key is pressed */
#define FCONTROL   0x08 /* The CTRL key must be held down when the accelerator key is pressed */
#define FSHIFT     0x04 /* The SHIFT key must be held down when the accelerator key is pressed */
#define FNOINVERT  0x02 /* obsolete */
#define FVIRTKEY   TRUE /* The key member specifies a virtual-key code.
                        /* If this flag is not specified, key is assumed to specify a character code */
/* modifed version of the accelerator key (shortcut) struct for RealCAD v7
/*
typedef struct tagACCEL {
    BYTE fVirt;
    WORD key;
    WORD cmd;
} ACCEL, *LPACCEL;
*/
typedef class tagRCACCEL {
public:
    RCSTRING whatsThis; // help message for this shortcut
    RCSTRING kstring; // ascii version of the shotcut (QShortcut)
    BYTE fVirt; // see notes above
    int key; // key value depending on fVirt value (0bit)
    int cmd; // realcad command id
    tagRCACCEL()
    {
        whatsThis.clear();
        kstring.clear();
        fVirt = 0;
        key = 0;
        cmd = 0;
    }
    tagRCACCEL(const tagRCACCEL& o)
    {
        whatsThis = o.whatsThis;
        kstring = o.kstring; // new for v7
        fVirt = o.fVirt;
        key = o.key;
        cmd = o.cmd;
    }
    tagRCACCEL& operator=(const tagRCACCEL& o)
    {
        whatsThis = o.whatsThis;
        kstring = o.kstring; // new for v7
        fVirt = o.fVirt;
        key = o.key;
        cmd = o.cmd;
        return *this;
    }
} RCACCEL, *RCLPACCEL;
typedef std::vector<RCACCEL> RCACCELARRAY;
typedef RCACCELARRAY::iterator RCVACCELITER;
typedef RCACCELARRAY RCVACCEL;
typedef RCVACCEL *RCVPACCEL;
typedef RCVPACCEL RCHACCEL; // alias for compatibilty
//typedef RCLPACCEL RCHACCEL;
//typedef ACCEL RCACCEL;
//typedef LPACCEL RCLPACCEL;
typedef void *PAVIFILE;
typedef struct {
    DWORD	fccType;		    /* stream type, for consistency */
    DWORD       fccHandler;                 /* compressor */
    DWORD       dwKeyFrameEvery;            /* keyframe rate */
    DWORD       dwQuality;                  /* compress quality 0-10,000 */
    DWORD       dwBytesPerSecond;           /* bytes per second */
    DWORD       dwFlags;                    /* flags... see below */
    LPVOID      lpFormat;                   /* save format */
    DWORD       cbFormat;
    LPVOID      lpParms;                    /* compressor options */
    DWORD       cbParms;
    DWORD       dwInterleaveEvery;          /* for non-video streams only */
} AVICOMPRESSOPTIONS, FAR *LPAVICOMPRESSOPTIONS;

#ifdef _MAC
typedef struct _MEMORYSTATUS {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    SIZE_T dwTotalPhys;
    SIZE_T dwAvailPhys;
    SIZE_T dwTotalPageFile;
    SIZE_T dwAvailPageFile;
    SIZE_T dwTotalVirtual;
    SIZE_T dwAvailVirtual;
} MEMORYSTATUS, *LPMEMORYSTATUS;
#endif
enum DebugEventLevel
{
    DebugEventLevelFatal,
    DebugEventLevelWarning
};

#define RGB(r,g,b) ((RCCOLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define IN
#define OUT
typedef int INT;
typedef float REAL;
#ifdef _MAC
class CLSID {} ;
#define REAL_MAX            FLT_MAX
#define REAL_MIN            FLT_MIN
#define REAL_TOLERANCE     (FLT_MIN * 100)
#define REAL_EPSILON        1.192092896e-07F        /* FLT_EPSILON */
#define WINGDIPAPI __stdcall
typedef void (WINAPI *DebugEventProc)(DebugEventLevel level, CHAR *message);
#endif
#ifndef GUID_DEFINED
#define GUID_DEFINED
#if defined(__midl)
typedef struct {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    byte           Data4[ 8 ];
} GUID;
#else
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;
#endif
#endif
typedef DWORD ARGB;
extern "C" {
typedef BOOL (CALLBACK * ImageAbort)(VOID *);
typedef ImageAbort DrawImageAbort;
typedef ImageAbort GetThumbnailImageAbort;
}
typedef DWORD PROPID;
typedef struct IDirectDrawSurface7      FAR *LPDIRECTDRAWSURFACE7;

typedef INT PixelFormat;

#define    PixelFormatIndexed      0x00010000 // Indexes into a palette
#define    PixelFormatGDI          0x00020000 // Is a GDI-supported format
#define    PixelFormatAlpha        0x00040000 // Has an alpha component
#define    PixelFormatPAlpha       0x00080000 // Pre-multiplied alpha
#define    PixelFormatExtended     0x00100000 // Extended color 16 bits/channel
#define    PixelFormatCanonical    0x00200000

#define    PixelFormatUndefined       0
#define    PixelFormatDontCare        0

#define    PixelFormat1bppIndexed     (1 | ( 1 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define    PixelFormat4bppIndexed     (2 | ( 4 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define    PixelFormat8bppIndexed     (3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define    PixelFormat16bppGrayScale  (4 | (16 << 8) | PixelFormatExtended)
#define    PixelFormat16bppRGB555     (5 | (16 << 8) | PixelFormatGDI)
#define    PixelFormat16bppRGB565     (6 | (16 << 8) | PixelFormatGDI)
#define    PixelFormat16bppARGB1555   (7 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI)
#define    PixelFormat24bppRGB        (8 | (24 << 8) | PixelFormatGDI)
#define    PixelFormat32bppRGB        (9 | (32 << 8) | PixelFormatGDI)
#define    PixelFormat32bppARGB       (10 | (32 << 8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical)
#define    PixelFormat32bppPARGB      (11 | (32 << 8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatGDI)
#define    PixelFormat48bppRGB        (12 | (48 << 8) | PixelFormatExtended)
#define    PixelFormat64bppARGB       (13 | (64 << 8) | PixelFormatAlpha  | PixelFormatCanonical | PixelFormatExtended)
#define    PixelFormat64bppPARGB      (14 | (64 << 8) | PixelFormatAlpha  | PixelFormatPAlpha | PixelFormatExtended)
#define    PixelFormat32bppCMYK       (15 | (32 << 8))
#define    PixelFormatMax             16

#ifdef _MAC
class IStream {
public:
    void Release(){;}
    HRESULT Read( void* pv,   ULONG cb,   ULONG* pcbRead ){return 0;}
    HRESULT Write(   void const* pv,   ULONG cb,  ULONG* pcbWritten ){return 0;}
};
#endif
#ifdef _MAC
class IStorage {
public:
    void Release(){;}
    HRESULT CreateStream(
                       const WCHAR* pwcsName,
                        DWORD grfMode,
                       DWORD reserved1,
                        DWORD reserved2,
                       IStream** ppstm
    )
    {
        return 0;
    }

    HRESULT OpenStream(   const WCHAR* pwcsName,   void* reserved1,
      DWORD grfMode,   DWORD reserved2,   IStream** ppstm )
    {
        return 0;
    }
};
#endif
/*
class GpImage {};
class GpBitmap : public GpImage {};

namespace Gdiplus  {

enum ImageLockMode
{
    ImageLockModeRead        = 0x0001,
    ImageLockModeWrite       = 0x0002,
    ImageLockModeUserInputBuf= 0x0004
};

class PropertyItem
{
public:
    PROPID  id;                 // ID of this property
    ULONG   length;             // Length of the property value, in bytes
    WORD    type;               // Type of the value, as one of TAG_TYPE_XXX
                                // defined above
    VOID*   value;              // property value
};

enum RotateFlipType
{
    RotateNoneFlipNone = 0,
    Rotate90FlipNone   = 1,
    Rotate180FlipNone  = 2,
    Rotate270FlipNone  = 3,

    RotateNoneFlipX    = 4,
    Rotate90FlipX      = 5,
    Rotate180FlipX     = 6,
    Rotate270FlipX     = 7,

    RotateNoneFlipY    = Rotate180FlipX,
    Rotate90FlipY      = Rotate270FlipX,
    Rotate180FlipY     = RotateNoneFlipX,
    Rotate270FlipY     = Rotate90FlipX,

    RotateNoneFlipXY   = Rotate180FlipNone,
    Rotate90FlipXY     = Rotate270FlipNone,
    Rotate180FlipXY    = RotateNoneFlipNone,
    Rotate270FlipXY    = Rotate90FlipNone
};

struct ColorPalette
{
public:

    UINT Flags;             // Palette flags
    UINT Count;             // Number of color entries
    ARGB Entries[1];        // Palette color entries
};

enum Unit
{
    UnitWorld,      // 0 -- World coordinate (non-physical unit)
    UnitDisplay,    // 1 -- Variable -- for PageTransform only
    UnitPixel,      // 2 -- Each unit is one device pixel.
    UnitPoint,      // 3 -- Each unit is a printer's point, or 1/72 inch.
    UnitInch,       // 4 -- Each unit is 1 inch.
    UnitDocument,   // 5 -- Each unit is 1/300 inch.
    UnitMillimeter  // 6 -- Each unit is 1 millimeter.
};

typedef enum {
    Ok = 0,
    GenericError = 1,
    InvalidParameter = 2,
    OutOfMemory = 3,
    ObjectBusy = 4,
    InsufficientBuffer = 5,
    NotImplemented = 6,
    Win32Error = 7,
    WrongState = 8,
    Aborted = 9,
    FileNotFound = 10,
    ValueOverflow = 11,
    AccessDenied = 12,
    UnknownImageFormat = 13,
    FontFamilyNotFound = 14,
    FontStyleNotFound = 15,
    NotTrueTypeFont = 16,
    UnsupportedGdiplusVersion = 17,
    GdiplusNotInitialized = 18,
    PropertyNotFound = 19,
    PropertyNotSupported = 20,
    ProfileNotFound = 21
} Status;

enum ImageType
{
    ImageTypeUnknown,   // 0
    ImageTypeBitmap,    // 1
    ImageTypeMetafile   // 2
};

enum CombineMode
{
    CombineModeReplace,     // 0
    CombineModeIntersect,   // 1
    CombineModeUnion,       // 2
    CombineModeXor,         // 3
    CombineModeExclude,     // 4
    CombineModeComplement   // 5 (Exclude From)
};

typedef Status GpStatus;
typedef INT PixelFormat;

typedef Status (WINAPI *NotificationHookProc)(OUT ULONG_PTR *token);
typedef VOID (WINAPI *NotificationUnhookProc)(ULONG_PTR token);

namespace DllExports
{
    //#include "GdiplusMem.h"
    inline void* WINGDIPAPI GdipAlloc(size_t size){return 0;}

    inline void WINGDIPAPI GdipFree(void* ptr){;}
};

class EncoderParameter
{
public:
    GUID    Guid;               // GUID of the parameter
    ULONG   NumberOfValues;     // Number of the parameter values
    ULONG   Type;               // Value type, like ValueTypeLONG  etc.
    VOID*   Value;              // A pointer to the parameter values
};

class EncoderParameters
{
public:
    UINT Count;                      // Number of parameters in this structure
    EncoderParameter Parameter[1];   // Parameter values
};

class GdiplusBase
{
public:
    void (operator delete)(void* in_pVoid)
    {
       DllExports::GdipFree(in_pVoid);
    }
    void* (operator new)(size_t in_size)
    {
       return DllExports::GdipAlloc(in_size);
    }
    void (operator delete[])(void* in_pVoid)
    {
       DllExports::GdipFree(in_pVoid);
    }
    void* (operator new[])(size_t in_size)
    {
       return DllExports::GdipAlloc(in_size);
    }
};

struct GdiplusStartupInput
{
    UINT32 GdiplusVersion;
    DebugEventProc DebugEventCallback;
    BOOL SuppressBackgroundThread;
    BOOL SuppressExternalCodecs;

    GdiplusStartupInput(
        DebugEventProc debugEventCallback = NULL,
        BOOL suppressBackgroundThread = FALSE,
        BOOL suppressExternalCodecs = FALSE)
    {
        GdiplusVersion = 1;
        DebugEventCallback = debugEventCallback;
        SuppressBackgroundThread = suppressBackgroundThread;
        SuppressExternalCodecs = suppressExternalCodecs;
    }
};

struct GdiplusStartupOutput
{
    // The following 2 fields are NULL if SuppressBackgroundThread is FALSE.
    // Otherwise, they are functions which must be called appropriately to
    // replace the background thread.
    //
    // These should be called on the application's main message loop - i.e.
    // a message loop which is active for the lifetime of GDI+.
    // "NotificationHook" should be called before starting the loop,
    // and "NotificationUnhook" should be called after the loop ends.

    NotificationHookProc NotificationHook;
    NotificationUnhookProc NotificationUnhook;
};

inline Status WINAPI GdiplusStartup(
    OUT ULONG_PTR *token,
    const GdiplusStartupInput *input,
    OUT GdiplusStartupOutput *output)
{
    return Ok;
}

inline Status WINAPI GdiplusShutdown( ULONG_PTR token )
{
    return Ok;
}

class Color
{
public:

    Color()
    {
        Argb = (ARGB)Color::Black;
    }

    // Construct an opaque Color object with
    // the specified Red, Green, Blue values.
    //
    // Color values are not premultiplied.

    Color(IN BYTE r,
          IN BYTE g,
          IN BYTE b)
    {
        Argb = MakeARGB(255, r, g, b);
    }

    Color(IN BYTE a,
          IN BYTE r,
          IN BYTE g,
          IN BYTE b)
    {
        Argb = MakeARGB(a, r, g, b);
    }

    Color(IN ARGB argb)
    {
        Argb = argb;
    }

    BYTE GetAlpha() const
    {
        return (BYTE) (Argb >> AlphaShift);
    }

    BYTE GetA() const
    {
        return GetAlpha();
    }

    BYTE GetRed() const
    {
        return (BYTE) (Argb >> RedShift);
    }

    BYTE GetR() const
    {
        return GetRed();
    }

    BYTE GetGreen() const
    {
        return (BYTE) (Argb >> GreenShift);
    }

    BYTE GetG() const
    {
        return GetGreen();
    }

    BYTE GetBlue() const
    {
        return (BYTE) (Argb >> BlueShift);
    }

    BYTE GetB() const
    {
        return GetBlue();
    }

    ARGB GetValue() const
    {
        return Argb;
    }

    VOID SetValue(IN ARGB argb)
    {
        Argb = argb;
    }

    VOID SetFromRCCOLORREF(IN RCCOLORREF rgb)
    {
        Argb = MakeARGB(255, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
    }

    RCCOLORREF ToRCCOLORREF() const
    {
        return RGB(GetRed(), GetGreen(), GetBlue());
    }

public:

    // Common color constants

    enum
    {
        AliceBlue            = 0xFFF0F8FF,
        AntiqueWhite         = 0xFFFAEBD7,
        Aqua                 = 0xFF00FFFF,
        Aquamarine           = 0xFF7FFFD4,
        Azure                = 0xFFF0FFFF,
        Beige                = 0xFFF5F5DC,
        Bisque               = 0xFFFFE4C4,
        Black                = 0xFF000000,
        BlanchedAlmond       = 0xFFFFEBCD,
        Blue                 = 0xFF0000FF,
        BlueViolet           = 0xFF8A2BE2,
        Brown                = 0xFFA52A2A,
        BurlyWood            = 0xFFDEB887,
        CadetBlue            = 0xFF5F9EA0,
        Chartreuse           = 0xFF7FFF00,
        Chocolate            = 0xFFD2691E,
        Coral                = 0xFFFF7F50,
        CornflowerBlue       = 0xFF6495ED,
        Cornsilk             = 0xFFFFF8DC,
        Crimson              = 0xFFDC143C,
        Cyan                 = 0xFF00FFFF,
        DarkBlue             = 0xFF00008B,
        DarkCyan             = 0xFF008B8B,
        DarkGoldenrod        = 0xFFB8860B,
        DarkGray             = 0xFFA9A9A9,
        DarkGreen            = 0xFF006400,
        DarkKhaki            = 0xFFBDB76B,
        DarkMagenta          = 0xFF8B008B,
        DarkOliveGreen       = 0xFF556B2F,
        DarkOrange           = 0xFFFF8C00,
        DarkOrchid           = 0xFF9932CC,
        DarkRed              = 0xFF8B0000,
        DarkSalmon           = 0xFFE9967A,
        DarkSeaGreen         = 0xFF8FBC8B,
        DarkSlateBlue        = 0xFF483D8B,
        DarkSlateGray        = 0xFF2F4F4F,
        DarkTurquoise        = 0xFF00CED1,
        DarkViolet           = 0xFF9400D3,
        DeepPink             = 0xFFFF1493,
        DeepSkyBlue          = 0xFF00BFFF,
        DimGray              = 0xFF696969,
        DodgerBlue           = 0xFF1E90FF,
        Firebrick            = 0xFFB22222,
        FloralWhite          = 0xFFFFFAF0,
        ForestGreen          = 0xFF228B22,
        Fuchsia              = 0xFFFF00FF,
        Gainsboro            = 0xFFDCDCDC,
        GhostWhite           = 0xFFF8F8FF,
        Gold                 = 0xFFFFD700,
        Goldenrod            = 0xFFDAA520,
        Gray                 = 0xFF808080,
        Green                = 0xFF008000,
        GreenYellow          = 0xFFADFF2F,
        Honeydew             = 0xFFF0FFF0,
        HotPink              = 0xFFFF69B4,
        IndianRed            = 0xFFCD5C5C,
        Indigo               = 0xFF4B0082,
        Ivory                = 0xFFFFFFF0,
        Khaki                = 0xFFF0E68C,
        Lavender             = 0xFFE6E6FA,
        LavenderBlush        = 0xFFFFF0F5,
        LawnGreen            = 0xFF7CFC00,
        LemonChiffon         = 0xFFFFFACD,
        LightBlue            = 0xFFADD8E6,
        LightCoral           = 0xFFF08080,
        LightCyan            = 0xFFE0FFFF,
        LightGoldenrodYellow = 0xFFFAFAD2,
        LightGray            = 0xFFD3D3D3,
        LightGreen           = 0xFF90EE90,
        LightPink            = 0xFFFFB6C1,
        LightSalmon          = 0xFFFFA07A,
        LightSeaGreen        = 0xFF20B2AA,
        LightSkyBlue         = 0xFF87CEFA,
        LightSlateGray       = 0xFF778899,
        LightSteelBlue       = 0xFFB0C4DE,
        LightYellow          = 0xFFFFFFE0,
        Lime                 = 0xFF00FF00,
        LimeGreen            = 0xFF32CD32,
        Linen                = 0xFFFAF0E6,
        Magenta              = 0xFFFF00FF,
        Maroon               = 0xFF800000,
        MediumAquamarine     = 0xFF66CDAA,
        MediumBlue           = 0xFF0000CD,
        MediumOrchid         = 0xFFBA55D3,
        MediumPurple         = 0xFF9370DB,
        MediumSeaGreen       = 0xFF3CB371,
        MediumSlateBlue      = 0xFF7B68EE,
        MediumSpringGreen    = 0xFF00FA9A,
        MediumTurquoise      = 0xFF48D1CC,
        MediumVioletRed      = 0xFFC71585,
        MidnightBlue         = 0xFF191970,
        MintCream            = 0xFFF5FFFA,
        MistyRose            = 0xFFFFE4E1,
        Moccasin             = 0xFFFFE4B5,
        NavajoWhite          = 0xFFFFDEAD,
        Navy                 = 0xFF000080,
        OldLace              = 0xFFFDF5E6,
        Olive                = 0xFF808000,
        OliveDrab            = 0xFF6B8E23,
        Orange               = 0xFFFFA500,
        OrangeRed            = 0xFFFF4500,
        Orchid               = 0xFFDA70D6,
        PaleGoldenrod        = 0xFFEEE8AA,
        PaleGreen            = 0xFF98FB98,
        PaleTurquoise        = 0xFFAFEEEE,
        PaleVioletRed        = 0xFFDB7093,
        PapayaWhip           = 0xFFFFEFD5,
        PeachPuff            = 0xFFFFDAB9,
        Peru                 = 0xFFCD853F,
        Pink                 = 0xFFFFC0CB,
        Plum                 = 0xFFDDA0DD,
        PowderBlue           = 0xFFB0E0E6,
        Purple               = 0xFF800080,
        Red                  = 0xFFFF0000,
        RosyBrown            = 0xFFBC8F8F,
        RoyalBlue            = 0xFF4169E1,
        SaddleBrown          = 0xFF8B4513,
        Salmon               = 0xFFFA8072,
        SandyBrown           = 0xFFF4A460,
        SeaGreen             = 0xFF2E8B57,
        SeaShell             = 0xFFFFF5EE,
        Sienna               = 0xFFA0522D,
        Silver               = 0xFFC0C0C0,
        SkyBlue              = 0xFF87CEEB,
        SlateBlue            = 0xFF6A5ACD,
        SlateGray            = 0xFF708090,
        Snow                 = 0xFFFFFAFA,
        SpringGreen          = 0xFF00FF7F,
        SteelBlue            = 0xFF4682B4,
        Tan                  = 0xFFD2B48C,
        Teal                 = 0xFF008080,
        Thistle              = 0xFFD8BFD8,
        Tomato               = 0xFFFF6347,
        Transparent          = 0x00FFFFFF,
        Turquoise            = 0xFF40E0D0,
        Violet               = 0xFFEE82EE,
        Wheat                = 0xFFF5DEB3,
        White                = 0xFFFFFFFF,
        WhiteSmoke           = 0xFFF5F5F5,
        Yellow               = 0xFFFFFF00,
        YellowGreen          = 0xFF9ACD32
    };

    // Shift count and bit mask for A, R, G, B components

    enum
    {
        AlphaShift  = 24,
        RedShift    = 16,
        GreenShift  = 8,
        BlueShift   = 0
    };

    enum
    {
        AlphaMask   = 0xff000000,
        RedMask     = 0x00ff0000,
        GreenMask   = 0x0000ff00,
        BlueMask    = 0x000000ff
    };

    // Assemble A, R, G, B values into a 32-bit integer

    static ARGB MakeARGB(IN BYTE a,
                         IN BYTE r,
                         IN BYTE g,
                         IN BYTE b)
    {
        return (((ARGB) (b) <<  BlueShift) |
                ((ARGB) (g) << GreenShift) |
                ((ARGB) (r) <<   RedShift) |
                ((ARGB) (a) << AlphaShift));
    }

protected:

    ARGB Argb;
};

class Size
{
public:
    Size()
    {
        Width = Height = 0;
    }

    Size(IN const Size& size)
    {
        Width = size.Width;
        Height = size.Height;
    }

    Size(IN INT width,
         IN INT height)
    {
        Width = width;
        Height = height;
    }

    Size operator+(IN const Size& sz) const
    {
        return Size(Width + sz.Width,
                    Height + sz.Height);
    }

    Size operator-(IN const Size& sz) const
    {
        return Size(Width - sz.Width,
                    Height - sz.Height);
    }

    BOOL Equals(IN const Size& sz) const
    {
        return (Width == sz.Width) && (Height == sz.Height);
    }

    BOOL Empty() const
    {
        return (Width == 0 && Height == 0);
    }

public:

    INT Width;
    INT Height;
};

class SizeF
{
public:
    SizeF()
    {
        Width = Height = 0.0f;
    }

    SizeF(IN const SizeF& size)
    {
        Width = size.Width;
        Height = size.Height;
    }

    SizeF(IN REAL width,
          IN REAL height)
    {
        Width = width;
        Height = height;
    }

    SizeF operator+(IN const SizeF& sz) const
    {
        return SizeF(Width + sz.Width,
                     Height + sz.Height);
    }

    SizeF operator-(IN const SizeF& sz) const
    {
        return SizeF(Width - sz.Width,
                     Height - sz.Height);
    }

    BOOL Equals(IN const SizeF& sz) const
    {
        return (Width == sz.Width) && (Height == sz.Height);
    }

    BOOL Empty() const
    {
        return (Width == 0.0f && Height == 0.0f);
    }

public:

    REAL Width;
    REAL Height;
};

class Point
{
public:
   Point()
   {
       X = Y = 0;
   }

   Point(IN const Point &point)
   {
       X = point.X;
       Y = point.Y;
   }

   Point(IN const Size &size)
   {
       X = size.Width;
       Y = size.Height;
   }

   Point(IN INT x,
         IN INT y)
   {
       X = x;
       Y = y;
   }

   Point operator+(IN const Point& point) const
   {
       return Point(X + point.X,
                    Y + point.Y);
   }

   Point operator-(IN const Point& point) const
   {
       return Point(X - point.X,
                    Y - point.Y);
   }

   BOOL Equals(IN const Point& point)
   {
       return (X == point.X) && (Y == point.Y);
   }

public:

    INT X;
    INT Y;
};

class PointF
{
public:
   PointF()
   {
       X = Y = 0.0f;
   }

   PointF(IN const PointF &point)
   {
       X = point.X;
       Y = point.Y;
   }

   PointF(IN const SizeF &size)
   {
       X = size.Width;
       Y = size.Height;
   }

   PointF(IN REAL x,
          IN REAL y)
   {
       X = x;
       Y = y;
   }

   PointF operator+(IN const PointF& point) const
   {
       return PointF(X + point.X,
                     Y + point.Y);
   }

   PointF operator-(IN const PointF& point) const
   {
       return PointF(X - point.X,
                     Y - point.Y);
   }

   BOOL Equals(IN const PointF& point)
   {
       return (X == point.X) && (Y == point.Y);
   }

public:

    REAL X;
    REAL Y;
};

class Rect
{
public:

    Rect()
    {
        X = Y = Width = Height = 0;
    }

    Rect(INT x,
         INT y,
         INT width,
         INT height)
    {
        X = x;
        Y = y;
        Width = width;
        Height = height;
    }

    Rect(  const Point& location,
           const Size& size)
    {
        X = location.X;
        Y = location.Y;
        Width = size.Width;
        Height = size.Height;
    }

    Rect* Clone() const
    {
        return new Rect(X, Y, Width, Height);
    }

    VOID GetLocation(OUT Point* point) const
    {
        point->X = X;
        point->Y = Y;
    }

    VOID GetSize(OUT Size* size) const
    {
        size->Width = Width;
        size->Height = Height;
    }

    VOID GetBounds(OUT Rect* rect) const
    {
        rect->X = X;
        rect->Y = Y;
        rect->Width = Width;
        rect->Height = Height;
    }

    INT GetLeft() const
    {
        return X;
    }

    INT GetTop() const
    {
        return Y;
    }

    INT GetRight() const
    {
        return X+Width;
    }

    INT GetBottom() const
    {
        return Y+Height;
    }

    BOOL IsEmptyArea() const
    {
        return (Width <= 0) || (Height <= 0);
    }

    BOOL Equals(  const Rect & rect) const
    {
        return X == rect.X &&
               Y == rect.Y &&
               Width == rect.Width &&
               Height == rect.Height;
    }

    BOOL Contains(  INT x,
                    INT y) const
    {
        return x >= X && x < X+Width &&
               y >= Y && y < Y+Height;
    }

    BOOL Contains(IN const Point& pt) const
    {
        return Contains(pt.X, pt.Y);
    }

    BOOL Contains(IN Rect& rect) const
    {
        return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
               (Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
    }

    VOID Inflate(  INT dx,
                   INT dy)
    {
        X -= dx;
        Y -= dy;
        Width += 2*dx;
        Height += 2*dy;
    }

    VOID Inflate(  const Point& point)
    {
        Inflate(point.X, point.Y);
    }

    BOOL Intersect(  const Rect& rect)
    {
        return Intersect(*this, *this, rect);
    }

    static BOOL Intersect(  Rect& c,
                            const Rect& a,
                            const Rect& b)
    {
        INT right = min(a.GetRight(), b.GetRight());
        INT bottom = min(a.GetBottom(), b.GetBottom());
        INT left = max(a.GetLeft(), b.GetLeft());
        INT top = max(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    BOOL IntersectsWith(  const Rect& rect) const
    {
        return (GetLeft() < rect.GetRight() &&
                GetTop() < rect.GetBottom() &&
                GetRight() > rect.GetLeft() &&
                GetBottom() > rect.GetTop());
    }

    static BOOL Union(OUT Rect& c,
                      IN const Rect& a,
                      IN const Rect& b)
    {
        INT right = max(a.GetRight(), b.GetRight());
        INT bottom = max(a.GetBottom(), b.GetBottom());
        INT left = min(a.GetLeft(), b.GetLeft());
        INT top = min(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    VOID Offset(  const Point& point)
    {
        Offset(point.X, point.Y);
    }

    VOID Offset(IN INT dx,
                IN INT dy)
    {
        X += dx;
        Y += dy;
    }

public:

    INT X;
    INT Y;
    INT Width;
    INT Height;
};

class RectF
{
public:

    RectF()
    {
        X = Y = Width = Height = 0.0f;
    }

    RectF(IN REAL x,
          IN REAL y,
          IN REAL width,
          IN REAL height)
    {
        X = x;
        Y = y;
        Width = width;
        Height = height;
    }

    RectF(IN const PointF& location,
          IN const SizeF& size)
    {
        X = location.X;
        Y = location.Y;
        Width = size.Width;
        Height = size.Height;
    }

    RectF* Clone() const
    {
        return new RectF(X, Y, Width, Height);
    }

    VOID GetLocation(OUT PointF* point) const
    {
        point->X = X;
        point->Y = Y;
    }

    VOID GetSize(OUT SizeF* size) const
    {
        size->Width = Width;
        size->Height = Height;
    }

    VOID GetBounds(OUT RectF* rect) const
    {
        rect->X = X;
        rect->Y = Y;
        rect->Width = Width;
        rect->Height = Height;
    }

    REAL GetLeft() const
    {
        return X;
    }

    REAL GetTop() const
    {
        return Y;
    }

    REAL GetRight() const
    {
        return X+Width;
    }

    REAL GetBottom() const
    {
        return Y+Height;
    }

    BOOL IsEmptyArea() const
    {
        return (Width <= REAL_EPSILON) || (Height <= REAL_EPSILON);
    }

    BOOL Equals(IN const RectF & rect) const
    {
        return X == rect.X &&
               Y == rect.Y &&
               Width == rect.Width &&
               Height == rect.Height;
    }

    BOOL Contains(IN REAL x,
                  IN REAL y) const
    {
        return x >= X && x < X+Width &&
               y >= Y && y < Y+Height;
    }

    BOOL Contains(IN const PointF& pt) const
    {
        return Contains(pt.X, pt.Y);
    }

    BOOL Contains(IN const RectF& rect) const
    {
        return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
               (Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
    }

    VOID Inflate(IN REAL dx,
                 IN REAL dy)
    {
        X -= dx;
        Y -= dy;
        Width += 2*dx;
        Height += 2*dy;
    }

    VOID Inflate(IN const PointF& point)
    {
        Inflate(point.X, point.Y);
    }

    BOOL Intersect(IN const RectF& rect)
    {
        return Intersect(*this, *this, rect);
    }

    static BOOL Intersect(OUT RectF& c,
                          IN const RectF& a,
                          IN const RectF& b)
    {
        REAL right = min(a.GetRight(), b.GetRight());
        REAL bottom = min(a.GetBottom(), b.GetBottom());
        REAL left = max(a.GetLeft(), b.GetLeft());
        REAL top = max(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    BOOL IntersectsWith(IN const RectF& rect) const
    {
        return (GetLeft() < rect.GetRight() &&
                GetTop() < rect.GetBottom() &&
                GetRight() > rect.GetLeft() &&
                GetBottom() > rect.GetTop());
    }

    static BOOL Union(OUT RectF& c,
                      IN const RectF& a,
                      IN const RectF& b)
    {
        REAL right = max(a.GetRight(), b.GetRight());
        REAL bottom = max(a.GetBottom(), b.GetBottom());
        REAL left = min(a.GetLeft(), b.GetLeft());
        REAL top = min(a.GetTop(), b.GetTop());

        c.X = left;
        c.Y = top;
        c.Width = right - left;
        c.Height = bottom - top;
        return !c.IsEmptyArea();
    }

    VOID Offset(IN const PointF& point)
    {
        Offset(point.X, point.Y);
    }

    VOID Offset(IN REAL dx,
                IN REAL dy)
    {
        X += dx;
        Y += dy;
    }

public:

    REAL X;
    REAL Y;
    REAL Width;
    REAL Height;
};
class BitmapData
{
public:
    UINT Width;
    UINT Height;
    INT Stride;
    PixelFormat PixelFormat;
    VOID* Scan0;
    UINT_PTR Reserved;
};


enum BrushType
{
   BrushTypeSolidColor       = 0,
   BrushTypeHatchFill        = 1,
   BrushTypeTextureFill      = 2,
   BrushTypePathGradient     = 3,
   BrushTypeLinearGradient   = 4
};

class GpBrush {};

class Brush : public GdiplusBase
{
public:
    friend class Pen;
    friend class Graphics;

    virtual ~Brush()
    {
        //DllExports::GdipDeleteBrush(nativeBrush);
    }

    virtual Brush* Clone() const
    {

        GpBrush *brush = NULL;

        SetStatus(DllExports::GdipCloneBrush(nativeBrush, &brush));

        Brush *newBrush = new Brush(brush, lastResult);

        if (newBrush == NULL)
        {
            DllExports::GdipDeleteBrush(brush);
        }

        return newBrush;
    }

    BrushType GetType() const
    {
        BrushType type = static_cast<BrushType>(-1);

        //SetStatus(DllExports::GdipGetBrushType(nativeBrush, &type));

        return type;
    }

    Status GetLastStatus() const
    {
        Status lastStatus = lastResult;
        lastResult = Ok;

        return lastStatus;
    }

protected:

    Brush()
    {
        SetStatus(NotImplemented);
    }

private:
    Brush(const Brush& brush);
    Brush& operator=(const Brush& brush);
protected:

    Brush(GpBrush* nativeBrush, Status status)
    {
        lastResult = status;
        SetNativeBrush(nativeBrush);
    }

    VOID SetNativeBrush(GpBrush* nativeBrush)
    {
        this->nativeBrush = nativeBrush;
    }

    Status SetStatus(Status status) const
    {
        if (status != Ok)
            return (lastResult = status);
        else
            return status;
    }

    GpBrush* nativeBrush;
    mutable Status lastResult;
};

enum FlushIntention
{
    FlushIntentionFlush = 0,        // Flush all batched rendering operations
    FlushIntentionSync = 1          // Flush all batched rendering operations
                                    // and wait for them to complete
};

enum TextRenderingHint
{
    TextRenderingHintSystemDefault = 0,            // Glyph with system default rendering hint
    TextRenderingHintSingleBitPerPixelGridFit,     // Glyph bitmap with hinting
    TextRenderingHintSingleBitPerPixel,            // Glyph bitmap without hinting
    TextRenderingHintAntiAliasGridFit,             // Glyph anti-alias bitmap with hinting
    TextRenderingHintAntiAlias,                    // Glyph anti-alias bitmap without hinting
    TextRenderingHintClearTypeGridFit              // Glyph CT bitmap with hinting
};
enum CompositingMode
{
    CompositingModeSourceOver,    // 0
    CompositingModeSourceCopy     // 1
};

enum QualityMode
{
    QualityModeInvalid   = -1,
    QualityModeDefault   = 0,
    QualityModeLow       = 1, // Best performance
    QualityModeHigh      = 2  // Best rendering quality
};

enum CompositingQuality
{
    CompositingQualityInvalid          = QualityModeInvalid,
    CompositingQualityDefault          = QualityModeDefault,
    CompositingQualityHighSpeed        = QualityModeLow,
    CompositingQualityHighQuality      = QualityModeHigh,
    CompositingQualityGammaCorrected,
    CompositingQualityAssumeLinear
};

enum InterpolationMode
{
    InterpolationModeInvalid          = QualityModeInvalid,
    InterpolationModeDefault          = QualityModeDefault,
    InterpolationModeLowQuality       = QualityModeLow,
    InterpolationModeHighQuality      = QualityModeHigh,
    InterpolationModeBilinear,
    InterpolationModeBicubic,
    InterpolationModeNearestNeighbor,
    InterpolationModeHighQualityBilinear,
    InterpolationModeHighQualityBicubic
};

enum SmoothingMode
{
    SmoothingModeInvalid     = QualityModeInvalid,
    SmoothingModeDefault     = QualityModeDefault,
    SmoothingModeHighSpeed   = QualityModeLow,
    SmoothingModeHighQuality = QualityModeHigh,
    SmoothingModeNone,
    SmoothingModeAntiAlias,
#if (GDIPVER >= 0x0110)
    SmoothingModeAntiAlias8x4 = SmoothingModeAntiAlias,
    SmoothingModeAntiAlias8x8
#endif //(GDIPVER >= 0x0110)
};

enum PixelOffsetMode
{
    PixelOffsetModeInvalid     = QualityModeInvalid,
    PixelOffsetModeDefault     = QualityModeDefault,
    PixelOffsetModeHighSpeed   = QualityModeLow,
    PixelOffsetModeHighQuality = QualityModeHigh,
    PixelOffsetModeNone,    // No pixel offset
    PixelOffsetModeHalf     // Offset by -0.5, -0.5 for fast anti-alias perf
};


enum MatrixOrder
{
    MatrixOrderPrepend    = 0,
    MatrixOrderAppend     = 1
};

enum CoordinateSpace
{
    CoordinateSpaceWorld,     // 0
    CoordinateSpacePage,      // 1
    CoordinateSpaceDevice     // 2
};

enum FillMode
{
    FillModeAlternate,        // 0
    FillModeWinding           // 1
};

enum StringAlignment
{
    // Left edge for left-to-right text,
    // right for right-to-left text,
    // and top for vertical
    StringAlignmentNear   = 0,
    StringAlignmentCenter = 1,
    StringAlignmentFar    = 2
};

enum HotkeyPrefix
{
    HotkeyPrefixNone        = 0,
    HotkeyPrefixShow        = 1,
    HotkeyPrefixHide        = 2
};


enum StringDigitSubstitute
{
    StringDigitSubstituteUser        = 0,  // As NLS setting
    StringDigitSubstituteNone        = 1,
    StringDigitSubstituteNational    = 2,
    StringDigitSubstituteTraditional = 3
};

enum StringTrimming {
    StringTrimmingNone              = 0,
    StringTrimmingCharacter         = 1,
    StringTrimmingWord              = 2,
    StringTrimmingEllipsisCharacter = 3,
    StringTrimmingEllipsisWord      = 4,
    StringTrimmingEllipsisPath      = 5
};

extern "C" {
typedef BOOL (CALLBACK * EnumerateMetafileProc)(EmfPlusRecordType,UINT,UINT,const BYTE*,VOID*);
}

typedef WORD LANGID;

class Matrix;
typedef Matrix GpMatrix;
class GpStringFormat {};

class StringFormat : public GdiplusBase
{
public:
    friend class Graphics;
    friend class GraphicsPath;


    StringFormat(
        IN INT     formatFlags = 0,
        IN LANGID  language = LANG_NEUTRAL
    )
    {
        nativeFormat = NULL;
        lastError = DllExports::GdipCreateStringFormat(
            formatFlags,
            language,
            &nativeFormat
        );
    }

    static const StringFormat *GenericDefault();
    static const StringFormat *GenericTypographic();

    StringFormat(
        IN const StringFormat *format
    )
    {
        nativeFormat = NULL;
        lastError = DllExports::GdipCloneStringFormat(
            format ? format->nativeFormat : NULL,
            &nativeFormat
        );
    }

    StringFormat *Clone() const
    {
        GpStringFormat *clonedStringFormat = NULL;

        lastError = DllExports::GdipCloneStringFormat(
            nativeFormat,
            &clonedStringFormat
        );

        if (lastError == Ok)
            return new StringFormat(clonedStringFormat, lastError);
        else
            return NULL;
    }

    ~StringFormat()
    {
        DllExports::GdipDeleteStringFormat(nativeFormat);
    }

    Status SetFormatFlags(IN INT flags)
    {
        return SetStatus(DllExports::GdipSetStringFormatFlags(
            nativeFormat,
            flags
        ));
    }

    INT GetFormatFlags() const
    {
        INT flags;
        SetStatus(DllExports::GdipGetStringFormatFlags(nativeFormat, &flags));
        return flags;
    }

    Status SetAlignment(IN StringAlignment align)
    {
        return SetStatus(DllExports::GdipSetStringFormatAlign(
            nativeFormat,
            align
        ));
    }

    StringAlignment GetAlignment() const
    {
        StringAlignment alignment;
        SetStatus(DllExports::GdipGetStringFormatAlign(
            nativeFormat,
            &alignment
        ));
        return alignment;
    }

    Status SetLineAlignment(IN StringAlignment align)
    {
        return SetStatus(DllExports::GdipSetStringFormatLineAlign(
            nativeFormat,
            align
        ));
    }

    StringAlignment GetLineAlignment() const
    {
        StringAlignment alignment;
        SetStatus(DllExports::GdipGetStringFormatLineAlign(
            nativeFormat,
            &alignment
        ));
        return alignment;
    }

    Status SetHotkeyPrefix(IN HotkeyPrefix hotkeyPrefix)
    {
        return SetStatus(DllExports::GdipSetStringFormatHotkeyPrefix(
            nativeFormat,
            (INT)hotkeyPrefix
        ));
    }

    HotkeyPrefix GetHotkeyPrefix() const
    {
        HotkeyPrefix hotkeyPrefix;
        SetStatus(DllExports::GdipGetStringFormatHotkeyPrefix(
            nativeFormat,
            (INT*)&hotkeyPrefix
        ));
        return hotkeyPrefix;
    }

    Status SetTabStops(
        IN REAL    firstTabOffset,
        IN INT     count,
        IN const REAL    *tabStops
    )
    {
        return SetStatus(DllExports::GdipSetStringFormatTabStops(
            nativeFormat,
            firstTabOffset,
            count,
            tabStops
        ));
    }

    INT GetTabStopCount() const
    {
        INT count;
        SetStatus(DllExports::GdipGetStringFormatTabStopCount(nativeFormat, &count));
        return count;
    }

    Status GetTabStops(
        IN INT     count,
        OUT REAL   *firstTabOffset,
        OUT REAL   *tabStops
    ) const
    {
        return SetStatus(DllExports::GdipGetStringFormatTabStops(
            nativeFormat,
            count,
            firstTabOffset,
            tabStops
        ));
    }

    Status SetDigitSubstitution(
        IN LANGID                language,
        IN StringDigitSubstitute substitute
    )
    {
        return SetStatus(DllExports::GdipSetStringFormatDigitSubstitution(
            nativeFormat,
            language,
            substitute
        ));
    }

    LANGID GetDigitSubstitutionLanguage(
    ) const
    {
        LANGID language;
        SetStatus(DllExports::GdipGetStringFormatDigitSubstitution(
            nativeFormat,
            &language,
            NULL
        ));
        return language;
    }

    StringDigitSubstitute GetDigitSubstitutionMethod(
    ) const
    {
        StringDigitSubstitute substitute;
        SetStatus(DllExports::GdipGetStringFormatDigitSubstitution(
            nativeFormat,
            NULL,
            &substitute
        ));
        return substitute;
    }

    Status SetTrimming(IN StringTrimming trimming)
    {
        return SetStatus(DllExports::GdipSetStringFormatTrimming(
            nativeFormat,
            trimming
        ));
    }

    StringTrimming StringFormat::GetTrimming() const
    {
        StringTrimming trimming;
        SetStatus(DllExports::GdipGetStringFormatTrimming(
            nativeFormat,
            &trimming
        ));
        return trimming;
    }

    Status SetMeasurableCharacterRanges(
        IN INT                  rangeCount,
        IN const CharacterRange *ranges
    )
    {
        return SetStatus(DllExports::GdipSetStringFormatMeasurableCharacterRanges(
            nativeFormat,
            rangeCount,
            ranges
        ));
    }

    INT GetMeasurableCharacterRangeCount()
    {
        INT count;
        SetStatus(DllExports::GdipGetStringFormatMeasurableCharacterRangeCount(
            nativeFormat,
            &count
        ));
        return count;
    }

    Status GetLastStatus() const
    {
        Status lastStatus = lastError;
        lastError = Ok;

        return lastStatus;
    }

protected:

    Status SetStatus(GpStatus newStatus) const
    {
        if (newStatus == Ok)
        {
            return Ok;
        }
        else
        {
            return lastError = newStatus;
        }
    }

    StringFormat(const StringFormat &source)
    {
        nativeFormat = NULL;
        lastError = DllExports::GdipCloneStringFormat(
            source.nativeFormat,
            &nativeFormat
        );
    }

    StringFormat& operator=(const StringFormat &source)
    {
        DllExports::GdipDeleteStringFormat(nativeFormat);
        lastError = DllExports::GdipCloneStringFormat(
            source.nativeFormat,
            &nativeFormat
        );
        return *this;
    }

    StringFormat(GpStringFormat * clonedStringFormat, Status status)
    {
        lastError = status;
        nativeFormat = clonedStringFormat;

    }

    GpStringFormat *nativeFormat;
    mutable Status  lastError;
};

class Matrix : public GdiplusBase
{
public:
    friend class Graphics;
    friend class GraphicsPath;
    friend class TextureBrush;
    friend class LinearGradientBrush;
    friend class PathGradientBrush;
    friend class Pen;
    friend class Region;

    // Default constructor is set to identity matrix.

    Matrix()
    {
        GpMatrix *matrix = NULL;

        //lastResult = DllExports::GdipCreateMatrix(&matrix);

        SetNativeMatrix(matrix);
    }

    Matrix(IN REAL m11,
           IN REAL m12,
           IN REAL m21,
           IN REAL m22,
           IN REAL dx,
           IN REAL dy)
    {
        GpMatrix *matrix = NULL;

        ///lastResult = DllExports::GdipCreateMatrix2(m11, m12, m21, m22,
                      //                                dx, dy, &matrix);

        SetNativeMatrix(matrix);
    }

    Matrix(IN const RectF& rect,
           IN const PointF* dstplg)
    {
        GpMatrix *matrix = NULL;

        //lastResult = DllExports::GdipCreateMatrix3(&rect,
                 //                                  dstplg,
                 //                                  &matrix);

        SetNativeMatrix(matrix);
    }

    Matrix(IN const Rect& rect,
           IN const Point* dstplg)
    {
        GpMatrix *matrix = NULL;

        //lastResult = DllExports::GdipCreateMatrix3I(&rect,
                 ////                                   dstplg,
                 //                                   &matrix);

        SetNativeMatrix(matrix);
    }

    ~Matrix()
    {
        //DllExports::GdipDeleteMatrix(nativeMatrix);
    }

    Matrix *Clone() const
    {
        GpMatrix *cloneMatrix = NULL;

        //SetStatus(DllExports::GdipCloneMatrix(nativeMatrix,
                 //                                 &cloneMatrix));

        if (lastResult != Ok)
            return NULL;

        return new Matrix(cloneMatrix);
    }

    Status GetElements(OUT REAL *m) const
    {
        //return SetStatus(DllExports::GdipGetMatrixElements(nativeMatrix, m));
    }

    Status SetElements(IN REAL m11,
                       IN REAL m12,
                       IN REAL m21,
                       IN REAL m22,
                       IN REAL dx,
                       IN REAL dy)
    {
        //return SetStatus(DllExports::GdipSetMatrixElements(nativeMatrix,
               //             m11, m12, m21, m22, dx, dy));
    }

    REAL OffsetX() const
    {
        REAL elements[6];

        if (GetElements(&elements[0]) == Ok)
            return elements[4];
        else
            return 0.0f;
    }

    REAL OffsetY() const
    {
       REAL elements[6];

       if (GetElements(&elements[0]) == Ok)
           return elements[5];
       else
           return 0.0f;
    }

    Status Reset()
    {
        // set identity matrix elements
        //return SetStatus(DllExports::GdipSetMatrixElements(nativeMatrix,
                 //                            1.0, 0.0, 0.0, 1.0, 0.0, 0.0));
    }

    Status Multiply(IN const Matrix *matrix,
                    IN MatrixOrder order = MatrixOrderPrepend)
    {
        //return SetStatus(DllExports::GdipMultiplyMatrix(nativeMatrix,
                //                          matrix->nativeMatrix,
              //                            order));
    }

    Status Translate(IN REAL offsetX,
                     IN REAL offsetY,
                     IN MatrixOrder order = MatrixOrderPrepend)
    {
        //return SetStatus(DllExports::GdipTranslateMatrix(nativeMatrix, offsetX,
         //                                                offsetY, order));
    }

    Status Scale(IN REAL scaleX,
                 IN REAL scaleY,
                 IN MatrixOrder order = MatrixOrderPrepend)
    {
        //return SetStatus(DllExports::GdipScaleMatrix(nativeMatrix, scaleX,
         //                                            scaleY, order));
    }

    Status Rotate(IN REAL angle,
                  IN MatrixOrder order = MatrixOrderPrepend)
    {
        //return SetStatus(DllExports::GdipRotateMatrix(nativeMatrix, angle,
        //                                              order));
    }

    Status RotateAt(IN REAL angle,
                    IN const PointF& center,
                    IN MatrixOrder order = MatrixOrderPrepend)
    {
        if(order == MatrixOrderPrepend)
        {
           // SetStatus(DllExports::GdipTranslateMatrix(nativeMatrix, center.X,
            //                                          center.Y, order));
           // SetStatus(DllExports::GdipRotateMatrix(nativeMatrix, angle,
            //                                       order));
           // return SetStatus(DllExports::GdipTranslateMatrix(nativeMatrix,
           //                                                  -center.X,
            //                                                 -center.Y,
            //                                                 order));
        }
        else
        {
           // SetStatus(DllExports::GdipTranslateMatrix(nativeMatrix,
            //                                          - center.X,
            //                                          - center.Y,
            //                                          order));
           // SetStatus(DllExports::GdipRotateMatrix(nativeMatrix, angle,
             //                                      order));
           // return SetStatus(DllExports::GdipTranslateMatrix(nativeMatrix,
            //                                                 center.X,
            //                                                 center.Y,
            //                                                 order));
        }
    }

    Status Shear(IN REAL shearX,
                 IN REAL shearY,
                 IN MatrixOrder order = MatrixOrderPrepend)
    {
        //return SetStatus(DllExports::GdipShearMatrix(nativeMatrix, shearX,
          //                                           shearY, order));
    }

    Status Invert()
    {
        //return SetStatus(DllExports::GdipInvertMatrix(nativeMatrix));
    }

    // float version
    Status TransformPoints(IN OUT PointF* pts,
                           IN INT count = 1) const
    {
        //return SetStatus(DllExports::GdipTransformMatrixPoints(nativeMatrix,
          //                                                     pts, count));
    }

    Status TransformPoints(IN OUT Point* pts,
                           IN INT count = 1) const
    {
        //return SetStatus(DllExports::GdipTransformMatrixPointsI(nativeMatrix,
         //                                                       pts,
         //                                                       count));
    }

    Status TransformVectors(IN OUT PointF* pts,
                            IN INT count = 1) const
    {
        //return SetStatus(DllExports::GdipVectorTransformMatrixPoints(
         //                               nativeMatrix, pts, count));
    }

    Status TransformVectors(IN OUT Point* pts,
                            IN INT count = 1) const
    {
       //return SetStatus(DllExports::GdipVectorTransformMatrixPointsI(
       //                                 nativeMatrix,
       //                                 pts,
       //                                 count));
    }

    BOOL IsInvertible() const
    {
        BOOL result = FALSE;

       // SetStatus(DllExports::GdipIsMatrixInvertible(nativeMatrix, &result));

        return result;
    }

    BOOL IsIdentity() const
    {
       BOOL result = FALSE;

      // SetStatus(DllExports::GdipIsMatrixIdentity(nativeMatrix, &result));

       return result;
    }

    BOOL Equals(IN const Matrix *matrix) const
    {
       BOOL result = FALSE;

      // SetStatus(DllExports::GdipIsMatrixEqual(nativeMatrix,
                //                               matrix->nativeMatrix,
                   //                            &result));

       return result;
    }

    Status GetLastStatus() const
    {
        Status lastStatus = lastResult;
        lastResult = Ok;

        return lastStatus;
    }

private:
    Matrix(const Matrix &);
    Matrix& operator=(const Matrix &);

protected:
    Matrix(GpMatrix *nativeMatrix)
    {
        lastResult = Ok;
        SetNativeMatrix(nativeMatrix);
    }

    VOID SetNativeMatrix(GpMatrix *nativeMatrix)
    {
        this->nativeMatrix = nativeMatrix;
    }

    Status SetStatus(Status status) const
    {
        if (status != Ok)
            return (lastResult = status);
        else
            return status;
    }

protected:
    GpMatrix *nativeMatrix;
    mutable Status lastResult;

};


class Graphics : public GdiplusBase
{
public:
    friend class Region;
    friend class GraphicsPath;
    friend class Image;
    friend class Bitmap;
    friend class Metafile;
    friend class Font;
    friend class FontFamily;
    friend class FontCollection;
    friend class CachedBitmap;

    static Graphics* FromHDC(IN HDC hdc)
    {
        return new Graphics(hdc);
    }

    static Graphics* FromHDC(IN HDC hdc,
                             IN HANDLE hdevice)
    {
        return new Graphics(hdc, hdevice);
    }

    static Graphics* FromHWND(IN HWND hwnd,
                              IN BOOL icm = FALSE)
    {
        return new Graphics(hwnd, icm);
    }

    static Graphics* FromImage(IN Image *image)
    {
        return new Graphics(image);
    }

    Graphics(IN HDC hdc)
    {
        GpGraphics *graphics = NULL;

        lastResult = DllExports::GdipCreateFromHDC(hdc, &graphics);

        SetNativeGraphics(graphics);
    }

    Graphics(IN HDC hdc,
             IN HANDLE hdevice)
    {
        GpGraphics *graphics = NULL;

        lastResult = DllExports::GdipCreateFromHDC2(hdc, hdevice, &graphics);

        SetNativeGraphics(graphics);
    }

    Graphics(IN HWND hwnd,
             IN BOOL icm = FALSE)
    {
        GpGraphics *graphics = NULL;

        if (icm)
        {
            lastResult = DllExports::GdipCreateFromHWNDICM(hwnd, &graphics);
        }
        else
        {
            lastResult = DllExports::GdipCreateFromHWND(hwnd, &graphics);
        }

        SetNativeGraphics(graphics);
    }

    Graphics(IN Image* image)
    {
        GpGraphics *graphics = NULL;

        if (image != NULL)
        {
            lastResult = DllExports::GdipGetImageGraphicsContext(
                                                                image->nativeImage, &graphics);
        }
        SetNativeGraphics(graphics);
    }

    ~Graphics()
    {
        DllExports::GdipDeleteGraphics(nativeGraphics);
    }

    VOID Flush(IN FlushIntention intention = FlushIntentionFlush)
    {
        DllExports::GdipFlush(nativeGraphics, intention);
    }

    //------------------------------------------------------------------------
    // GDI Interop methods
    //------------------------------------------------------------------------

    // Locks the graphics until ReleaseDC is called

    HDC GetHDC()
    {
        HDC     hdc = NULL;

        SetStatus(DllExports::GdipGetDC(nativeGraphics, &hdc));

        return hdc;
    }

    VOID ReleaseHDC(IN HDC hdc)
    {
        SetStatus(DllExports::GdipReleaseDC(nativeGraphics, hdc));
    }

    //------------------------------------------------------------------------
    // Rendering modes
    //------------------------------------------------------------------------

    Status SetRenderingOrigin(IN INT x, IN INT y)
    {
        return SetStatus(
            DllExports::GdipSetRenderingOrigin(
                nativeGraphics, x, y
            )
        );
    }

    Status GetRenderingOrigin(OUT INT *x, OUT INT *y) const
    {
        return SetStatus(
            DllExports::GdipGetRenderingOrigin(
                nativeGraphics, x, y
            )
        );
    }

    Status SetCompositingMode(IN CompositingMode compositingMode)
    {
        return SetStatus(DllExports::GdipSetCompositingMode(nativeGraphics,
                                                            compositingMode));
    }

    CompositingMode GetCompositingMode() const
    {
        CompositingMode mode;

        SetStatus(DllExports::GdipGetCompositingMode(nativeGraphics,
                                                     &mode));

        return mode;
    }

    Status SetCompositingQuality(IN CompositingQuality compositingQuality)
    {
        return SetStatus(DllExports::GdipSetCompositingQuality(
            nativeGraphics,
            compositingQuality));
    }

    CompositingQuality GetCompositingQuality() const
    {
        CompositingQuality quality;

        SetStatus(DllExports::GdipGetCompositingQuality(
            nativeGraphics,
            &quality));

        return quality;
    }

    Status SetTextRenderingHint(IN TextRenderingHint newMode)
    {
        return SetStatus(DllExports::GdipSetTextRenderingHint(nativeGraphics,
                                                          newMode));
    }

    TextRenderingHint GetTextRenderingHint() const
    {
        TextRenderingHint hint;

        SetStatus(DllExports::GdipGetTextRenderingHint(nativeGraphics,
                                                   &hint));

        return hint;
    }

    Status SetTextContrast(IN UINT contrast)
    {
        return SetStatus(DllExports::GdipSetTextContrast(nativeGraphics,
                                                          contrast));
    }

    UINT GetTextContrast() const
    {
        UINT contrast;

        SetStatus(DllExports::GdipGetTextContrast(nativeGraphics,
                                                    &contrast));

        return contrast;
    }

    InterpolationMode GetInterpolationMode() const
    {
        InterpolationMode mode = InterpolationModeInvalid;

        SetStatus(DllExports::GdipGetInterpolationMode(nativeGraphics,
                                                           &mode));

        return mode;
    }

    Status SetInterpolationMode(IN InterpolationMode interpolationMode)
    {
        return SetStatus(DllExports::GdipSetInterpolationMode(nativeGraphics,
                                                           interpolationMode));
    }

#if (GDIPVER >= 0x0110)
    Status SetAbort(GdiplusAbort *pIAbort)
    {
        return SetStatus(DllExports::GdipGraphicsSetAbort(
            nativeGraphics,
            pIAbort
        ));
    }
#endif //(GDIPVER >= 0x0110)

    SmoothingMode GetSmoothingMode() const
    {
        SmoothingMode smoothingMode = SmoothingModeInvalid;

        SetStatus(DllExports::GdipGetSmoothingMode(nativeGraphics,
                                                   &smoothingMode));

        return smoothingMode;
    }

    Status SetSmoothingMode(IN SmoothingMode smoothingMode)
    {
        return SetStatus(DllExports::GdipSetSmoothingMode(nativeGraphics,
                                                          smoothingMode));
    }

    PixelOffsetMode GetPixelOffsetMode() const
    {
        PixelOffsetMode pixelOffsetMode = PixelOffsetModeInvalid;

        SetStatus(DllExports::GdipGetPixelOffsetMode(nativeGraphics,
                                                     &pixelOffsetMode));

        return pixelOffsetMode;
    }

    Status SetPixelOffsetMode(IN PixelOffsetMode pixelOffsetMode)
    {
        return SetStatus(DllExports::GdipSetPixelOffsetMode(nativeGraphics,
                                                            pixelOffsetMode));
    }

    //------------------------------------------------------------------------
    // Manipulate current world transform
    //------------------------------------------------------------------------

    Status SetTransform(IN const Matrix* matrix)
    {
        return SetStatus(DllExports::GdipSetWorldTransform(nativeGraphics,
                                                        matrix->nativeMatrix));
    }
    Status ResetTransform()
    {
        return SetStatus(DllExports::GdipResetWorldTransform(nativeGraphics));
    }

    Status MultiplyTransform(IN const Matrix* matrix,
                             IN MatrixOrder order = MatrixOrderPrepend)
    {
        return SetStatus(DllExports::GdipMultiplyWorldTransform(nativeGraphics,
                                                                matrix->nativeMatrix,
                                                                order));
    }

    Status TranslateTransform(IN REAL dx,
                              IN REAL dy,
                              IN MatrixOrder order = MatrixOrderPrepend)
    {
        return SetStatus(DllExports::GdipTranslateWorldTransform(nativeGraphics,
                                                               dx, dy, order));
    }

    Status ScaleTransform(IN REAL sx,
                          IN REAL sy,
                          IN MatrixOrder order = MatrixOrderPrepend)
    {
        return SetStatus(DllExports::GdipScaleWorldTransform(nativeGraphics,
                                                             sx, sy, order));
    }

    Status RotateTransform(IN REAL angle,
                           IN MatrixOrder order = MatrixOrderPrepend)
    {
        return SetStatus(DllExports::GdipRotateWorldTransform(nativeGraphics,
                                                              angle, order));
    }

    Status GetTransform(OUT Matrix* matrix) const
    {
        return SetStatus(DllExports::GdipGetWorldTransform(nativeGraphics,
                                                           matrix->nativeMatrix));
    }

    Status SetPageUnit(IN Unit unit)
    {
        return SetStatus(DllExports::GdipSetPageUnit(nativeGraphics,
                                                     unit));
    }

    Status SetPageScale(IN REAL scale)
    {
        return SetStatus(DllExports::GdipSetPageScale(nativeGraphics,
                                                      scale));
    }

    Unit GetPageUnit() const
    {
        Unit unit;

        SetStatus(DllExports::GdipGetPageUnit(nativeGraphics, &unit));

        return unit;
    }

    REAL GetPageScale() const
    {
        REAL scale;

        SetStatus(DllExports::GdipGetPageScale(nativeGraphics, &scale));

        return scale;
    }

    REAL GetDpiX() const
    {
        REAL dpi;

        SetStatus(DllExports::GdipGetDpiX(nativeGraphics, &dpi));

        return dpi;
    }

    REAL GetDpiY() const
    {
        REAL dpi;

        SetStatus(DllExports::GdipGetDpiY(nativeGraphics, &dpi));

        return dpi;
    }

    Status TransformPoints(IN CoordinateSpace destSpace,
                           IN CoordinateSpace srcSpace,
                           IN OUT PointF* pts,
                           IN INT count) const
    {
        return SetStatus(DllExports::GdipTransformPoints(nativeGraphics,
                                                         destSpace,
                                                         srcSpace,
                                                         pts,
                                                         count));
    }

    Status TransformPoints(IN CoordinateSpace destSpace,
                           IN CoordinateSpace srcSpace,
                           IN OUT Point* pts,
                           IN INT count) const
    {

        return SetStatus(DllExports::GdipTransformPointsI(nativeGraphics,
                                                          destSpace,
                                                          srcSpace,
                                                          pts,
                                                          count));
    }

    //------------------------------------------------------------------------
    // GetNearestColor (for <= 8bpp surfaces).  Note: Alpha is ignored.
    //------------------------------------------------------------------------

    Status GetNearestColor(IN OUT Color* color) const
    {
        if (color == NULL)
        {
            return SetStatus(InvalidParameter);
        }

        ARGB argb = color->GetValue();

        Status status = SetStatus(DllExports::GdipGetNearestColor(nativeGraphics, &argb));

        color->SetValue(argb);

        return status;
    }

    Status DrawLine(IN const Pen* pen,
                    IN REAL x1,
                    IN REAL y1,
                    IN REAL x2,
                    IN REAL y2)
    {
        return SetStatus(DllExports::GdipDrawLine(nativeGraphics,
                                                  pen->nativePen, x1, y1, x2,
                                                  y2));
    }

    Status DrawLine(IN const Pen* pen,
                    IN const PointF& pt1,
                    IN const PointF& pt2)
    {
        return DrawLine(pen, pt1.X, pt1.Y, pt2.X, pt2.Y);
    }

    Status DrawLines(IN const Pen* pen,
                     IN const PointF* points,
                     IN INT count)
    {
        return SetStatus(DllExports::GdipDrawLines(nativeGraphics,
                                                   pen->nativePen,
                                                   points, count));
    }

    Status DrawLine(IN const Pen* pen,
                    IN INT x1,
                    IN INT y1,
                    IN INT x2,
                    IN INT y2)
    {
        return SetStatus(DllExports::GdipDrawLineI(nativeGraphics,
                                                   pen->nativePen,
                                                   x1,
                                                   y1,
                                                   x2,
                                                   y2));
    }

    Status DrawLine(IN const Pen* pen,
                    IN const Point& pt1,
                    IN const Point& pt2)
    {
        return DrawLine(pen,
                        pt1.X,
                        pt1.Y,
                        pt2.X,
                        pt2.Y);
    }

    Status DrawLines(IN const Pen* pen,
                     IN const Point* points,
                     IN INT count)
    {
        return SetStatus(DllExports::GdipDrawLinesI(nativeGraphics,
                                                    pen->nativePen,
                                                    points,
                                                    count));
    }

    Status DrawArc(IN const Pen* pen,
                   IN REAL x,
                   IN REAL y,
                   IN REAL width,
                   IN REAL height,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return SetStatus(DllExports::GdipDrawArc(nativeGraphics,
                                                 pen->nativePen,
                                                 x,
                                                 y,
                                                 width,
                                                 height,
                                                 startAngle,
                                                 sweepAngle));
    }

    Status DrawArc(IN const Pen* pen,
                   IN const RectF& rect,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return DrawArc(pen, rect.X, rect.Y, rect.Width, rect.Height,
                       startAngle, sweepAngle);
    }

    Status DrawArc(IN const Pen* pen,
                   IN INT x,
                   IN INT y,
                   IN INT width,
                   IN INT height,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return SetStatus(DllExports::GdipDrawArcI(nativeGraphics,
                                                  pen->nativePen,
                                                  x,
                                                  y,
                                                  width,
                                                  height,
                                                  startAngle,
                                                  sweepAngle));
    }


    Status DrawArc(IN const Pen* pen,
                   IN const Rect& rect,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return DrawArc(pen,
                       rect.X,
                       rect.Y,
                       rect.Width,
                       rect.Height,
                       startAngle,
                       sweepAngle);
    }

    Status DrawBezier(IN const Pen* pen,
                      IN REAL x1,
                      IN REAL y1,
                      IN REAL x2,
                      IN REAL y2,
                      IN REAL x3,
                      IN REAL y3,
                      IN REAL x4,
                      IN REAL y4)
    {
        return SetStatus(DllExports::GdipDrawBezier(nativeGraphics,
                                                    pen->nativePen, x1, y1,
                                                    x2, y2, x3, y3, x4, y4));
    }

    Status DrawBezier(IN const Pen* pen,
                      IN const PointF& pt1,
                      IN const PointF& pt2,
                      IN const PointF& pt3,
                      IN const PointF& pt4)
    {
        return DrawBezier(pen,
                          pt1.X,
                          pt1.Y,
                          pt2.X,
                          pt2.Y,
                          pt3.X,
                          pt3.Y,
                          pt4.X,
                          pt4.Y);
    }

    Status DrawBeziers(IN const Pen* pen,
                       IN const PointF* points,
                       IN INT count)
    {
        return SetStatus(DllExports::GdipDrawBeziers(nativeGraphics,
                                                     pen->nativePen,
                                                     points,
                                                     count));
    }

    Status DrawBezier(IN const Pen* pen,
                      IN INT x1,
                      IN INT y1,
                      IN INT x2,
                      IN INT y2,
                      IN INT x3,
                      IN INT y3,
                      IN INT x4,
                      IN INT y4)
    {
        return SetStatus(DllExports::GdipDrawBezierI(nativeGraphics,
                                                     pen->nativePen,
                                                     x1,
                                                     y1,
                                                     x2,
                                                     y2,
                                                     x3,
                                                     y3,
                                                     x4,
                                                     y4));
    }

    Status DrawBezier(IN const Pen* pen,
                      IN const Point& pt1,
                      IN const Point& pt2,
                      IN const Point& pt3,
                      IN const Point& pt4)
    {
        return DrawBezier(pen,
                          pt1.X,
                          pt1.Y,
                          pt2.X,
                          pt2.Y,
                          pt3.X,
                          pt3.Y,
                          pt4.X,
                          pt4.Y);
    }

    Status DrawBeziers(IN const Pen* pen,
                       IN const Point* points,
                       IN INT count)
    {
        return SetStatus(DllExports::GdipDrawBeziersI(nativeGraphics,
                                                      pen->nativePen,
                                                      points,
                                                      count));
    }

    Status DrawRectangle(IN const Pen* pen,
                         IN const RectF& rect)
    {
        return DrawRectangle(pen, rect.X, rect.Y, rect.Width, rect.Height);
    }

    Status DrawRectangle(IN const Pen* pen,
                         IN REAL x,
                         IN REAL y,
                         IN REAL width,
                         IN REAL height)
    {
        return SetStatus(DllExports::GdipDrawRectangle(nativeGraphics,
                                                       pen->nativePen, x, y,
                                                       width, height));
    }

    Status DrawRectangles(IN const Pen* pen,
                          IN const RectF* rects,
                          IN INT count)
    {
        return SetStatus(DllExports::GdipDrawRectangles(nativeGraphics,
                                                        pen->nativePen,
                                                        rects, count));
    }

    Status DrawRectangle(IN const Pen* pen,
                         IN const Rect& rect)
    {
        return DrawRectangle(pen,
                             rect.X,
                             rect.Y,
                             rect.Width,
                             rect.Height);
    }

    Status DrawRectangle(IN const Pen* pen,
                         IN INT x,
                         IN INT y,
                         IN INT width,
                         IN INT height)
    {
        return SetStatus(DllExports::GdipDrawRectangleI(nativeGraphics,
                                                        pen->nativePen,
                                                        x,
                                                        y,
                                                        width,
                                                        height));
    }

    Status DrawRectangles(IN const Pen* pen,
                          IN const Rect* rects,
                          IN INT count)
    {
        return SetStatus(DllExports::GdipDrawRectanglesI(nativeGraphics,
                                                         pen->nativePen,
                                                         rects,
                                                         count));
    }

    Status DrawEllipse(IN const Pen* pen,
                       IN const RectF& rect)
    {
        return DrawEllipse(pen, rect.X, rect.Y, rect.Width, rect.Height);
    }

    Status DrawEllipse(IN const Pen* pen,
                       IN REAL x,
                       IN REAL y,
                       IN REAL width,
                       IN REAL height)
    {
        return SetStatus(DllExports::GdipDrawEllipse(nativeGraphics,
                                                     pen->nativePen,
                                                     x,
                                                     y,
                                                     width,
                                                     height));
    }

    Status DrawEllipse(IN const Pen* pen,
                       IN const Rect& rect)
    {
        return DrawEllipse(pen,
                           rect.X,
                           rect.Y,
                           rect.Width,
                           rect.Height);
    }

    Status DrawEllipse(IN const Pen* pen,
                       IN INT x,
                       IN INT y,
                       IN INT width,
                       IN INT height)
    {
        return SetStatus(DllExports::GdipDrawEllipseI(nativeGraphics,
                                                      pen->nativePen,
                                                      x,
                                                      y,
                                                      width,
                                                      height));
    }

    Status DrawPie(IN const Pen* pen,
                   IN const RectF& rect,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return DrawPie(pen,
                       rect.X,
                       rect.Y,
                       rect.Width,
                       rect.Height,
                       startAngle,
                       sweepAngle);
    }

    Status DrawPie(IN const Pen* pen,
                   IN REAL x,
                   IN REAL y,
                   IN REAL width,
                   IN REAL height,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return SetStatus(DllExports::GdipDrawPie(nativeGraphics,
                                                 pen->nativePen,
                                                 x,
                                                 y,
                                                 width,
                                                 height,
                                                 startAngle,
                                                 sweepAngle));
    }

    Status DrawPie(IN const Pen* pen,
                   IN const Rect& rect,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return DrawPie(pen,
                       rect.X,
                       rect.Y,
                       rect.Width,
                       rect.Height,
                       startAngle,
                       sweepAngle);
    }

    Status DrawPie(IN const Pen* pen,
                   IN INT x,
                   IN INT y,
                   IN INT width,
                   IN INT height,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return SetStatus(DllExports::GdipDrawPieI(nativeGraphics,
                                                  pen->nativePen,
                                                  x,
                                                  y,
                                                  width,
                                                  height,
                                                  startAngle,
                                                  sweepAngle));
    }

    Status DrawPolygon(IN const Pen* pen,
                       IN const PointF* points,
                       IN INT count)
    {
        return SetStatus(DllExports::GdipDrawPolygon(nativeGraphics,
                                                     pen->nativePen,
                                                     points,
                                                     count));
    }

    Status DrawPolygon(IN const Pen* pen,
                       IN const Point* points,
                       IN INT count)
    {
        return SetStatus(DllExports::GdipDrawPolygonI(nativeGraphics,
                                                      pen->nativePen,
                                                      points,
                                                      count));
    }

    Status DrawPath(IN const Pen* pen,
                    IN const GraphicsPath* path)
    {
        return SetStatus(DllExports::GdipDrawPath(nativeGraphics,
                                                  pen ? pen->nativePen : NULL,
                                                  path ? path->nativePath : NULL));
    }

    Status DrawCurve(IN const Pen* pen,
                     IN const PointF* points,
                     IN INT count)
    {
        return SetStatus(DllExports::GdipDrawCurve(nativeGraphics,
                                                   pen->nativePen, points,
                                                   count));
    }

    Status DrawCurve(IN const Pen* pen,
                     IN const PointF* points,
                     IN INT count,
                     IN REAL tension)
    {
        return SetStatus(DllExports::GdipDrawCurve2(nativeGraphics,
                                                    pen->nativePen, points,
                                                    count, tension));
    }

    Status DrawCurve(IN const Pen* pen,
                     IN const PointF* points,
                     IN INT count,
                     IN INT offset,
                     IN INT numberOfSegments,
                     IN REAL tension = 0.5f)
    {
        return SetStatus(DllExports::GdipDrawCurve3(nativeGraphics,
                                                    pen->nativePen, points,
                                                    count, offset,
                                                    numberOfSegments, tension));
    }

    Status DrawCurve(IN const Pen* pen,
                     IN const Point* points,
                     IN INT count)
    {
        return SetStatus(DllExports::GdipDrawCurveI(nativeGraphics,
                                                    pen->nativePen,
                                                    points,
                                                    count));
    }

    Status DrawCurve(IN const Pen* pen,
                     IN const Point* points,
                     IN INT count,
                     IN REAL tension)
    {
        return SetStatus(DllExports::GdipDrawCurve2I(nativeGraphics,
                                                     pen->nativePen,
                                                     points,
                                                     count,
                                                     tension));
    }

    Status DrawCurve(IN const Pen* pen,
                     IN const Point* points,
                     IN INT count,
                     IN INT offset,
                     IN INT numberOfSegments,
                     IN REAL tension = 0.5f)
    {
        return SetStatus(DllExports::GdipDrawCurve3I(nativeGraphics,
                                                     pen->nativePen,
                                                     points,
                                                     count,
                                                     offset,
                                                     numberOfSegments,
                                                     tension));
    }

    Status DrawClosedCurve(IN const Pen* pen,
                           IN const PointF* points,
                           IN INT count)
    {
        return SetStatus(DllExports::GdipDrawClosedCurve(nativeGraphics,
                                                         pen->nativePen,
                                                         points, count));
    }

    Status DrawClosedCurve(IN const Pen *pen,
                           IN const PointF* points,
                           IN INT count,
                           IN REAL tension)
    {
        return SetStatus(DllExports::GdipDrawClosedCurve2(nativeGraphics,
                                                          pen->nativePen,
                                                          points, count,
                                                          tension));
    }

    Status DrawClosedCurve(IN const Pen* pen,
                           IN const Point* points,
                           IN INT count)
    {
        return SetStatus(DllExports::GdipDrawClosedCurveI(nativeGraphics,
                                                          pen->nativePen,
                                                          points,
                                                          count));
    }

    Status DrawClosedCurve(IN const Pen *pen,
                           IN const Point* points,
                           IN INT count,
                           IN REAL tension)
    {
        return SetStatus(DllExports::GdipDrawClosedCurve2I(nativeGraphics,
                                                           pen->nativePen,
                                                           points,
                                                           count,
                                                           tension));
    }

    Status Clear(IN const Color &color)
    {
        return SetStatus(DllExports::GdipGraphicsClear(
            nativeGraphics,
            color.GetValue()));
    }

    Status FillRectangle(IN const Brush* brush,
                         IN const RectF& rect)
    {
        return FillRectangle(brush, rect.X, rect.Y, rect.Width, rect.Height);
    }

    Status FillRectangle(IN const Brush* brush,
                         IN REAL x,
                         IN REAL y,
                         IN REAL width,
                         IN REAL height)
    {
        return SetStatus(DllExports::GdipFillRectangle(nativeGraphics,
                                                       brush->nativeBrush, x, y,
                                                       width, height));
    }

    Status FillRectangles(IN const Brush* brush,
                          IN const RectF* rects,
                          IN INT count)
    {
        return SetStatus(DllExports::GdipFillRectangles(nativeGraphics,
                                                        brush->nativeBrush,
                                                        rects, count));
    }

    Status FillRectangle(IN const Brush* brush,
                         IN const Rect& rect)
    {
        return FillRectangle(brush,
                             rect.X,
                             rect.Y,
                             rect.Width,
                             rect.Height);
    }

    Status FillRectangle(IN const Brush* brush,
                         IN INT x,
                         IN INT y,
                         IN INT width,
                         IN INT height)
    {
        return SetStatus(DllExports::GdipFillRectangleI(nativeGraphics,
                                                        brush->nativeBrush,
                                                        x,
                                                        y,
                                                        width,
                                                        height));
    }

    Status FillRectangles(IN const Brush* brush,
                          IN const Rect* rects,
                          IN INT count)
    {
        return SetStatus(DllExports::GdipFillRectanglesI(nativeGraphics,
                                                         brush->nativeBrush,
                                                         rects,
                                                         count));
    }

    Status FillPolygon(IN const Brush* brush,
                       IN const PointF* points,
                       IN INT count)
    {
        return FillPolygon(brush, points, count, FillModeAlternate);
    }

    Status FillPolygon(IN const Brush* brush,
                       IN const PointF* points,
                       IN INT count,
                       IN FillMode fillMode)
    {
        return SetStatus(DllExports::GdipFillPolygon(nativeGraphics,
                                                     brush->nativeBrush,
                                                     points, count, fillMode));
    }

    Status FillPolygon(IN const Brush* brush,
                       IN const Point* points,
                       IN INT count)
    {
        return FillPolygon(brush, points, count, FillModeAlternate);
    }

    Status FillPolygon(IN const Brush* brush,
                       IN const Point* points,
                       IN INT count,
                       IN FillMode fillMode)
    {
        return SetStatus(DllExports::GdipFillPolygonI(nativeGraphics,
                                                      brush->nativeBrush,
                                                      points, count,
                                                      fillMode));
    }

    Status FillEllipse(IN const Brush* brush,
                       IN const RectF& rect)
    {
        return FillEllipse(brush, rect.X, rect.Y, rect.Width, rect.Height);
    }

    Status FillEllipse(IN const Brush* brush,
                       IN REAL x,
                       IN REAL y,
                       IN REAL width,
                       IN REAL height)
    {
        return SetStatus(DllExports::GdipFillEllipse(nativeGraphics,
                                                     brush->nativeBrush, x, y,
                                                     width, height));
    }

    Status FillEllipse(IN const Brush* brush,
                       IN const Rect& rect)
    {
        return FillEllipse(brush, rect.X, rect.Y, rect.Width, rect.Height);
    }

    Status FillEllipse(IN const Brush* brush,
                       IN INT x,
                       IN INT y,
                       IN INT width,
                       IN INT height)
    {
        return SetStatus(DllExports::GdipFillEllipseI(nativeGraphics,
                                                      brush->nativeBrush,
                                                      x,
                                                      y,
                                                      width,
                                                      height));
    }

    Status FillPie(IN const Brush* brush,
                   IN const RectF& rect,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return FillPie(brush, rect.X, rect.Y, rect.Width, rect.Height,
                       startAngle, sweepAngle);
    }

    Status FillPie(IN const Brush* brush,
                   IN REAL x,
                   IN REAL y,
                   IN REAL width,
                   IN REAL height,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return SetStatus(DllExports::GdipFillPie(nativeGraphics,
                                                 brush->nativeBrush, x, y,
                                                 width, height, startAngle,
                                                 sweepAngle));
    }

    Status FillPie(IN const Brush* brush,
                   IN const Rect& rect,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return FillPie(brush, rect.X, rect.Y, rect.Width, rect.Height,
                       startAngle, sweepAngle);
    }

    Status FillPie(IN const Brush* brush,
                   IN INT x,
                   IN INT y,
                   IN INT width,
                   IN INT height,
                   IN REAL startAngle,
                   IN REAL sweepAngle)
    {
        return SetStatus(DllExports::GdipFillPieI(nativeGraphics,
                                                  brush->nativeBrush,
                                                  x,
                                                  y,
                                                  width,
                                                  height,
                                                  startAngle,
                                                  sweepAngle));
    }

    Status FillPath(IN const Brush* brush,
                    IN const GraphicsPath* path)
    {
        return SetStatus(DllExports::GdipFillPath(nativeGraphics,
                                                  brush->nativeBrush,
                                                  path->nativePath));
    }

    Status FillClosedCurve(IN const Brush* brush,
                           IN const PointF* points,
                           IN INT count)
    {
        return SetStatus(DllExports::GdipFillClosedCurve(nativeGraphics,
                                                         brush->nativeBrush,
                                                         points, count));

    }

    Status FillClosedCurve(IN const Brush* brush,
                           IN const PointF* points,
                           IN INT count,
                           IN FillMode fillMode,
                           IN REAL tension = 0.5f)
    {
        return SetStatus(DllExports::GdipFillClosedCurve2(nativeGraphics,
                                                          brush->nativeBrush,
                                                          points, count,
                                                          tension, fillMode));
    }

    Status FillClosedCurve(IN const Brush* brush,
                           IN const Point* points,
                           IN INT count)
    {
        return SetStatus(DllExports::GdipFillClosedCurveI(nativeGraphics,
                                                          brush->nativeBrush,
                                                          points,
                                                          count));
    }

    Status FillClosedCurve(IN const Brush* brush,
                           IN const Point* points,
                           IN INT count,
                           IN FillMode fillMode,
                           IN REAL tension = 0.5f)
    {
        return SetStatus(DllExports::GdipFillClosedCurve2I(nativeGraphics,
                                                           brush->nativeBrush,
                                                           points, count,
                                                           tension, fillMode));
    }

    Status FillRegion(IN const Brush* brush,
                      IN const Region* region)
    {
        return SetStatus(DllExports::GdipFillRegion(nativeGraphics,
                                                    brush->nativeBrush,
                                                    region->nativeRegion));
    }

    Status
    DrawString(
        IN const WCHAR        *string,
        IN INT                 length,
        IN const Font         *font,
        IN const RectF        &layoutRect,
        IN const StringFormat *stringFormat,
        IN const Brush        *brush
    )
    {
        return SetStatus(DllExports::GdipDrawString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &layoutRect,
            stringFormat ? stringFormat->nativeFormat : NULL,
            brush ? brush->nativeBrush : NULL
        ));
    }

    Status
    DrawString(
        const WCHAR        *string,
        INT                 length,
        const Font         *font,
        const PointF       &origin,
        const Brush        *brush
    )
    {
        RectF rect(origin.X, origin.Y, 0.0f, 0.0f);

        return SetStatus(DllExports::GdipDrawString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &rect,
            NULL,
            brush ? brush->nativeBrush : NULL
        ));
    }

    Status
    DrawString(
        const WCHAR        *string,
        INT                 length,
        const Font         *font,
        const PointF       &origin,
        const StringFormat *stringFormat,
        const Brush        *brush
    )
    {
        RectF rect(origin.X, origin.Y, 0.0f, 0.0f);

        return SetStatus(DllExports::GdipDrawString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &rect,
            stringFormat ? stringFormat->nativeFormat : NULL,
            brush ? brush->nativeBrush : NULL
        ));
    }

    Status
    MeasureString(
        IN const WCHAR        *string,
        IN INT                 length,
        IN const Font         *font,
        IN const RectF        &layoutRect,
        IN const StringFormat *stringFormat,
        OUT RectF             *boundingBox,
        OUT INT               *codepointsFitted = 0,
        OUT INT               *linesFilled      = 0
    ) const
    {
        return SetStatus(DllExports::GdipMeasureString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &layoutRect,
            stringFormat ? stringFormat->nativeFormat : NULL,
            boundingBox,
            codepointsFitted,
            linesFilled
        ));
    }

    Status
    MeasureString(
        IN const WCHAR        *string,
        IN INT                 length,
        IN const Font         *font,
        IN const SizeF        &layoutRectSize,
        IN const StringFormat *stringFormat,
        OUT SizeF             *size,
        OUT INT               *codepointsFitted = 0,
        OUT INT               *linesFilled      = 0
    ) const
    {
        RectF   layoutRect(0, 0, layoutRectSize.Width, layoutRectSize.Height);
        RectF   boundingBox;
        Status  status;

        if (size == NULL)
        {
            return SetStatus(InvalidParameter);
        }

        status = SetStatus(DllExports::GdipMeasureString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &layoutRect,
            stringFormat ? stringFormat->nativeFormat : NULL,
            size ? &boundingBox : NULL,
            codepointsFitted,
            linesFilled
        ));

        if (size && status == Ok)
        {
            size->Width  = boundingBox.Width;
            size->Height = boundingBox.Height;
        }

        return status;
    }

    Status
    MeasureString(
        IN const WCHAR        *string,
        IN INT                 length,
        IN const Font         *font,
        IN const PointF       &origin,
        IN const StringFormat *stringFormat,
        OUT RectF             *boundingBox
    ) const
    {
        RectF rect(origin.X, origin.Y, 0.0f, 0.0f);

        return SetStatus(DllExports::GdipMeasureString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &rect,
            stringFormat ? stringFormat->nativeFormat : NULL,
            boundingBox,
            NULL,
            NULL
        ));
    }

    Status
    MeasureString(
        IN const WCHAR  *string,
        IN INT           length,
        IN const Font   *font,
        IN const RectF  &layoutRect,
        OUT RectF       *boundingBox
    ) const
    {
        return SetStatus(DllExports::GdipMeasureString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &layoutRect,
            NULL,
            boundingBox,
            NULL,
            NULL
        ));
    }

    Status
    MeasureString(
        IN const WCHAR  *string,
        IN INT           length,
        IN const Font   *font,
        IN const PointF &origin,
        OUT RectF       *boundingBox
    ) const
    {
        RectF rect(origin.X, origin.Y, 0.0f, 0.0f);

        return SetStatus(DllExports::GdipMeasureString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &rect,
            NULL,
            boundingBox,
            NULL,
            NULL
        ));
    }


    Status
    MeasureCharacterRanges(
        IN const WCHAR        *string,
        IN INT                 length,
        IN const Font         *font,
        IN const RectF        &layoutRect,
        IN const StringFormat *stringFormat,
        IN INT                 regionCount,
        OUT Region            *regions
    ) const
    {
        if (!regions || regionCount <= 0)
        {
            return InvalidParameter;
        }

        GpRegion **nativeRegions = new GpRegion* [regionCount];

        if (!nativeRegions)
        {
            return OutOfMemory;
        }

        for (INT i = 0; i < regionCount; i++)
        {
            nativeRegions[i] = regions[i].nativeRegion;
        }

        Status status = SetStatus(DllExports::GdipMeasureCharacterRanges(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            layoutRect,
            stringFormat ? stringFormat->nativeFormat : NULL,
            regionCount,
            nativeRegions
        ));

        delete [] nativeRegions;

        return status;
    }

    Status DrawDriverString(
        IN const UINT16  *text,
        IN INT            length,
        IN const Font    *font,
        IN const Brush   *brush,
        IN const PointF  *positions,
        IN INT            flags,
        IN const Matrix        *matrix
    )
    {
        return SetStatus(DllExports::GdipDrawDriverString(
            nativeGraphics,
            text,
            length,
            font ? font->nativeFont : NULL,
            brush ? brush->nativeBrush : NULL,
            positions,
            flags,
            matrix ? matrix->nativeMatrix : NULL
        ));
    }

    Status MeasureDriverString(
        IN const UINT16  *text,
        IN INT            length,
        IN const Font    *font,
        IN const PointF  *positions,
        IN INT            flags,
        IN const Matrix        *matrix,
        OUT RectF        *boundingBox
    ) const
    {
        return SetStatus(DllExports::GdipMeasureDriverString(
            nativeGraphics,
            text,
            length,
            font ? font->nativeFont : NULL,
            positions,
            flags,
            matrix ? matrix->nativeMatrix : NULL,
            boundingBox
        ));
    }

    // Draw a cached bitmap on this graphics destination offset by
    // x, y. Note this will fail with WrongState if the CachedBitmap
    // native format differs from this Graphics.

    Status DrawCachedBitmap(IN CachedBitmap *cb,
                            IN INT x,
                            IN INT y)
    {
        return SetStatus(DllExports::GdipDrawCachedBitmap(
            nativeGraphics,
            cb->nativeCachedBitmap,
            x, y
        ));
    }

    Status DrawImage(IN Image* image,
                     IN const PointF& point)
    {
        return DrawImage(image, point.X, point.Y);
    }

    Status DrawImage(IN Image* image,
                     IN REAL x,
                     IN REAL y)
    {
        return SetStatus(DllExports::GdipDrawImage(nativeGraphics,
                                                   image ? image->nativeImage
                                                         : NULL,
                                                   x,
                                                   y));
    }

    Status DrawImage(IN Image* image,
                     IN const RectF& rect)
    {
        return DrawImage(image, rect.X, rect.Y, rect.Width, rect.Height);
    }

    Status DrawImage(IN Image* image,
                     IN REAL x,
                     IN REAL y,
                     IN REAL width,
                     IN REAL height)
    {
        return SetStatus(DllExports::GdipDrawImageRect(nativeGraphics,
                                                       image ? image->nativeImage
                                                             : NULL,
                                                       x,
                                                       y,
                                                       width,
                                                       height));
    }

    Status DrawImage(IN Image* image,
                     IN const Point& point)
    {
        return DrawImage(image, point.X, point.Y);
    }

    Status DrawImage(IN Image* image,
                     IN INT x,
                     IN INT y)
    {
        return SetStatus(DllExports::GdipDrawImageI(nativeGraphics,
                                                    image ? image->nativeImage
                                                          : NULL,
                                                    x,
                                                    y));
    }

    Status DrawImage(IN Image* image,
                     IN const Rect& rect)
    {
        return DrawImage(image,
                         rect.X,
                         rect.Y,
                         rect.Width,
                         rect.Height);
    }

    Status DrawImage(IN Image* image,
                     IN INT x,
                     IN INT y,
                     IN INT width,
                     IN INT height) {
        return SetStatus(DllExports::GdipDrawImageRectI(nativeGraphics,
                                                        image ? image->nativeImage
                                                              : NULL,
                                                        x,
                                                        y,
                                                        width,
                                                        height));
    }


    Status DrawImage(IN Image* image,
                     IN const PointF* destPoints,
                     IN INT count)
    {
        if (count != 3 && count != 4)
            return SetStatus(InvalidParameter);

        return SetStatus(DllExports::GdipDrawImagePoints(nativeGraphics,
                                                         image ? image->nativeImage
                                                               : NULL,
                                                         destPoints, count));
    }

    Status DrawImage(IN Image* image,
                     IN const Point* destPoints,
                     IN INT count)
    {
        if (count != 3 && count != 4)
            return SetStatus(InvalidParameter);

        return SetStatus(DllExports::GdipDrawImagePointsI(nativeGraphics,
                                                          image ? image->nativeImage
                                                                : NULL,
                                                          destPoints,
                                                          count));
    }

    Status DrawImage(IN Image* image,
                     IN REAL x,
                     IN REAL y,
                     IN REAL srcx,
                     IN REAL srcy,
                     IN REAL srcwidth,
                     IN REAL srcheight,
                     IN Unit srcUnit)
    {
        return SetStatus(DllExports::GdipDrawImagePointRect(nativeGraphics,
                                                            image ? image->nativeImage
                                                                  : NULL,
                                                            x, y,
                                                            srcx, srcy,
                                                            srcwidth, srcheight, srcUnit));
    }

    Status DrawImage(IN Image* image,
                     IN const RectF& destRect,
                     IN REAL srcx,
                     IN REAL srcy,
                     IN REAL srcwidth,
                     IN REAL srcheight,
                     IN Unit srcUnit,
                     IN const ImageAttributes* imageAttributes = NULL,
                     IN DrawImageAbort callback = NULL,
                     IN VOID* callbackData = NULL)
    {
        return SetStatus(DllExports::GdipDrawImageRectRect(nativeGraphics,
                                                           image ? image->nativeImage
                                                                 : NULL,
                                                           destRect.X,
                                                           destRect.Y,
                                                           destRect.Width,
                                                           destRect.Height,
                                                           srcx, srcy,
                                                           srcwidth, srcheight,
                                                           srcUnit,
                                                           imageAttributes
                                                            ? imageAttributes->nativeImageAttr
                                                            : NULL,
                                                           callback,
                                                           callbackData));
    }

    Status DrawImage(IN Image* image,
                     IN const PointF* destPoints,
                     IN INT count,
                     IN REAL srcx,
                     IN REAL srcy,
                     IN REAL srcwidth,
                     IN REAL srcheight,
                     IN Unit srcUnit,
                     IN const ImageAttributes* imageAttributes = NULL,
                     IN DrawImageAbort callback = NULL,
                     IN VOID* callbackData = NULL)
    {
        return SetStatus(DllExports::GdipDrawImagePointsRect(nativeGraphics,
                                                             image ? image->nativeImage
                                                                   : NULL,
                                                             destPoints, count,
                                                             srcx, srcy,
                                                             srcwidth,
                                                             srcheight,
                                                             srcUnit,
                                                             imageAttributes
                                                              ? imageAttributes->nativeImageAttr
                                                              : NULL,
                                                             callback,
                                                             callbackData));
    }

    Status DrawImage(IN Image* image,
                     IN INT x,
                     IN INT y,
                     IN INT srcx,
                     IN INT srcy,
                     IN INT srcwidth,
                     IN INT srcheight,
                     IN Unit srcUnit)
    {
        return SetStatus(DllExports::GdipDrawImagePointRectI(nativeGraphics,
                                                             image ? image->nativeImage
                                                                   : NULL,
                                                             x,
                                                             y,
                                                             srcx,
                                                             srcy,
                                                             srcwidth,
                                                             srcheight,
                                                             srcUnit));
    }

    Status DrawImage(IN Image* image,
                     IN const Rect& destRect,
                     IN INT srcx,
                     IN INT srcy,
                     IN INT srcwidth,
                     IN INT srcheight,
                     IN Unit srcUnit,
                     IN const ImageAttributes* imageAttributes = NULL,
                     IN DrawImageAbort callback = NULL,
                     IN VOID* callbackData = NULL)
    {
        return SetStatus(DllExports::GdipDrawImageRectRectI(nativeGraphics,
                                                            image ? image->nativeImage
                                                                  : NULL,
                                                            destRect.X,
                                                            destRect.Y,
                                                            destRect.Width,
                                                            destRect.Height,
                                                            srcx,
                                                            srcy,
                                                            srcwidth,
                                                            srcheight,
                                                            srcUnit,
                                                            imageAttributes
                                                            ? imageAttributes->nativeImageAttr
                                                            : NULL,
                                                            callback,
                                                            callbackData));
    }

    Status DrawImage(IN Image* image,
                     IN const Point* destPoints,
                     IN INT count,
                     IN INT srcx,
                     IN INT srcy,
                     IN INT srcwidth,
                     IN INT srcheight,
                     IN Unit srcUnit,
                     IN const ImageAttributes* imageAttributes = NULL,
                     IN DrawImageAbort callback = NULL,
                     IN VOID* callbackData = NULL)
    {
        return SetStatus(DllExports::GdipDrawImagePointsRectI(nativeGraphics,
                                                              image ? image->nativeImage
                                                                    : NULL,
                                                              destPoints,
                                                              count,
                                                              srcx,
                                                              srcy,
                                                              srcwidth,
                                                              srcheight,
                                                              srcUnit,
                                                              imageAttributes
                                                               ? imageAttributes->nativeImageAttr
                                                               : NULL,
                                                              callback,
                                                              callbackData));
    }

#if (GDIPVER >= 0x0110)
    Status DrawImage(
        IN Image *image,
        IN const RectF &destRect,
        IN const RectF &sourceRect,
        IN Unit srcUnit,
        IN const ImageAttributes *imageAttributes = NULL
    )
    {
        return SetStatus(DllExports::GdipDrawImageRectRect(
            nativeGraphics,
            image->nativeImage,
            destRect.X,
            destRect.Y,
            destRect.Width,
            destRect.Height,
            sourceRect.X,
            sourceRect.Y,
            sourceRect.Width,
            sourceRect.Height,
            srcUnit,
            imageAttributes ? imageAttributes->nativeImageAttr : NULL,
            NULL,
            NULL
        ));
    }

    Status DrawImage(
        IN Image *image,
        IN RectF *sourceRect,
        IN Matrix *xForm,
        IN Effect *effect,
        IN ImageAttributes *imageAttributes,
        IN Unit srcUnit
    )
    {
        return SetStatus(DllExports::GdipDrawImageFX(
            nativeGraphics,
            image->nativeImage,
            sourceRect,
            xForm ? xForm->nativeMatrix : NULL,
            effect ? effect->nativeEffect : NULL,
            imageAttributes ? imageAttributes->nativeImageAttr : NULL,
            srcUnit
        ));
    }
#endif //(GDIPVER >= 0x0110)

    // The following methods are for playing an EMF+ to a graphics
    // via the enumeration interface.  Each record of the EMF+ is
    // sent to the callback (along with the callbackData).  Then
    // the callback can invoke the Metafile::PlayRecord method
    // to play the particular record.

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const PointF &          destPoint,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileDestPoint(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destPoint,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const Point &           destPoint,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileDestPointI(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destPoint,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const RectF &           destRect,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileDestRect(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destRect,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const Rect &            destRect,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileDestRectI(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destRect,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const PointF *          destPoints,
        IN INT                     count,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileDestPoints(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destPoints,
                    count,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const Point *           destPoints,
        IN INT                     count,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileDestPointsI(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destPoints,
                    count,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const PointF &          destPoint,
        IN const RectF &           srcRect,
        IN Unit                    srcUnit,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileSrcRectDestPoint(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destPoint,
                    srcRect,
                    srcUnit,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const Point &           destPoint,
        IN const Rect &            srcRect,
        IN Unit                    srcUnit,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileSrcRectDestPointI(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destPoint,
                    srcRect,
                    srcUnit,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const RectF &           destRect,
        IN const RectF &           srcRect,
        IN Unit                    srcUnit,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileSrcRectDestRect(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destRect,
                    srcRect,
                    srcUnit,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const Rect &            destRect,
        IN const Rect &            srcRect,
        IN Unit                    srcUnit,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileSrcRectDestRectI(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destRect,
                    srcRect,
                    srcUnit,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const PointF *          destPoints,
        IN INT                     count,
        IN const RectF &           srcRect,
        IN Unit                    srcUnit,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileSrcRectDestPoints(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destPoints,
                    count,
                    srcRect,
                    srcUnit,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status
    EnumerateMetafile(
        IN const Metafile *        metafile,
        IN const Point *           destPoints,
        IN INT                     count,
        IN const Rect &            srcRect,
        IN Unit                    srcUnit,
        IN EnumerateMetafileProc   callback,
        IN VOID *                  callbackData    = NULL,
        IN const ImageAttributes *       imageAttributes = NULL
        )
    {
        return SetStatus(DllExports::GdipEnumerateMetafileSrcRectDestPointsI(
                    nativeGraphics,
                    (const GpMetafile *)(metafile ? metafile->nativeImage:NULL),
                    destPoints,
                    count,
                    srcRect,
                    srcUnit,
                    callback,
                    callbackData,
                    imageAttributes ? imageAttributes->nativeImageAttr : NULL));
    }

    Status SetClip(IN const Graphics* g,
                   IN CombineMode combineMode = CombineModeReplace)
    {
        return SetStatus(DllExports::GdipSetClipGraphics(nativeGraphics,
                                                         g->nativeGraphics,
                                                         combineMode));
    }

    Status SetClip(IN const RectF& rect,
                   IN CombineMode combineMode = CombineModeReplace)
    {
        return SetStatus(DllExports::GdipSetClipRect(nativeGraphics,
                                                     rect.X, rect.Y,
                                                     rect.Width, rect.Height,
                                                     combineMode));
    }

    Status SetClip(IN const Rect& rect,
                   IN CombineMode combineMode = CombineModeReplace)
    {
        return SetStatus(DllExports::GdipSetClipRectI(nativeGraphics,
                                                      rect.X, rect.Y,
                                                      rect.Width, rect.Height,
                                                      combineMode));
    }

    Status SetClip(IN const GraphicsPath* path,
                   IN CombineMode combineMode = CombineModeReplace)
    {
        return SetStatus(DllExports::GdipSetClipPath(nativeGraphics,
                                                     path->nativePath,
                                                     combineMode));
    }

    Status SetClip(IN const Region* region,
                   IN CombineMode combineMode = CombineModeReplace)
    {
        return SetStatus(DllExports::GdipSetClipRegion(nativeGraphics,
                                                       region->nativeRegion,
                                                       combineMode));
    }

    // This is different than the other SetClip methods because it assumes
    // that the RCHRGN is already in device units, so it doesn't transform
    // the coordinates in the RCHRGN.

    Status SetClip(IN RCHRGN hRgn,
                   IN CombineMode combineMode = CombineModeReplace)
    {
        return SetStatus(DllExports::GdipSetClipHrgn(nativeGraphics, hRgn,
                                                     combineMode));
    }

    Status IntersectClip(IN const RectF& rect)
    {
        return SetStatus(DllExports::GdipSetClipRect(nativeGraphics,
                                                     rect.X, rect.Y,
                                                     rect.Width, rect.Height,
                                                     CombineModeIntersect));
    }

    Status IntersectClip(IN const Rect& rect)
    {
        return SetStatus(DllExports::GdipSetClipRectI(nativeGraphics,
                                                      rect.X, rect.Y,
                                                      rect.Width, rect.Height,
                                                      CombineModeIntersect));
    }

    Status IntersectClip(IN const Region* region)
    {
        return SetStatus(DllExports::GdipSetClipRegion(nativeGraphics,
                                                       region->nativeRegion,
                                                       CombineModeIntersect));
    }

    Status ExcludeClip(IN const RectF& rect)
    {
        return SetStatus(DllExports::GdipSetClipRect(nativeGraphics,
                                                     rect.X, rect.Y,
                                                     rect.Width, rect.Height,
                                                     CombineModeExclude));
    }

    Status ExcludeClip(IN const Rect& rect)
    {
        return SetStatus(DllExports::GdipSetClipRectI(nativeGraphics,
                                                      rect.X, rect.Y,
                                                      rect.Width, rect.Height,
                                                      CombineModeExclude));
    }

    Status ExcludeClip(IN const Region* region)
    {
        return SetStatus(DllExports::GdipSetClipRegion(nativeGraphics,
                                                       region->nativeRegion,
                                                       CombineModeExclude));
    }

    Status ResetClip()
    {
        return SetStatus(DllExports::GdipResetClip(nativeGraphics));
    }

    Status TranslateClip(IN REAL dx,
                         IN REAL dy)
    {
        return SetStatus(DllExports::GdipTranslateClip(nativeGraphics, dx, dy));
    }

    Status TranslateClip(IN INT dx,
                         IN INT dy)
    {
        return SetStatus(DllExports::GdipTranslateClipI(nativeGraphics,
                                                        dx, dy));
    }

    Status GetClip(OUT Region* region) const
    {
        return SetStatus(DllExports::GdipGetClip(nativeGraphics,
                                                 region->nativeRegion));
    }

    Status GetClipBounds(OUT RectF* rect) const
    {
        return SetStatus(DllExports::GdipGetClipBounds(nativeGraphics, rect));
    }

    Status GetClipBounds(OUT Rect* rect) const
    {
        return SetStatus(DllExports::GdipGetClipBoundsI(nativeGraphics, rect));
    }

    BOOL IsClipEmpty() const
    {
        BOOL booln = FALSE;

        SetStatus(DllExports::GdipIsClipEmpty(nativeGraphics, &booln));

        return booln;
    }

    Status GetVisibleClipBounds(OUT RectF *rect) const
    {

        return SetStatus(DllExports::GdipGetVisibleClipBounds(nativeGraphics,
                                                              rect));
    }

    Status GetVisibleClipBounds(OUT Rect *rect) const
    {
       return SetStatus(DllExports::GdipGetVisibleClipBoundsI(nativeGraphics,
                                                              rect));
    }

    BOOL IsVisibleClipEmpty() const
    {
        BOOL booln = FALSE;

        SetStatus(DllExports::GdipIsVisibleClipEmpty(nativeGraphics, &booln));

        return booln;
    }

    BOOL IsVisible(IN INT x,
                   IN INT y) const
    {
        return IsVisible(Point(x,y));
    }

    BOOL IsVisible(IN const Point& point) const
    {
        BOOL booln = FALSE;

        SetStatus(DllExports::GdipIsVisiblePointI(nativeGraphics,
                                                  point.X,
                                                  point.Y,
                                                  &booln));

        return booln;
    }

    BOOL IsVisible(IN INT x,
                   IN INT y,
                   IN INT width,
                   IN INT height) const
    {
        return IsVisible(Rect(x, y, width, height));
    }

    BOOL IsVisible(IN const Rect& rect) const
    {

        BOOL booln = TRUE;

        SetStatus(DllExports::GdipIsVisibleRectI(nativeGraphics,
                                                 rect.X,
                                                 rect.Y,
                                                 rect.Width,
                                                 rect.Height,
                                                 &booln));
        return booln;
    }

    BOOL IsVisible(IN REAL x,
                   IN REAL y) const
    {
        return IsVisible(PointF(x, y));
    }

    BOOL IsVisible(IN const PointF& point) const
    {
        BOOL booln = FALSE;

        SetStatus(DllExports::GdipIsVisiblePoint(nativeGraphics,
                                                 point.X,
                                                 point.Y,
                                                 &booln));

        return booln;
    }

    BOOL IsVisible(IN REAL x,
                   IN REAL y,
                   IN REAL width,
                   IN REAL height) const
    {
        return IsVisible(RectF(x, y, width, height));
    }

    BOOL IsVisible(IN const RectF& rect) const
    {
        BOOL booln = TRUE;

        SetStatus(DllExports::GdipIsVisibleRect(nativeGraphics,
                                                rect.X,
                                                rect.Y,
                                                rect.Width,
                                                rect.Height,
                                                &booln));
        return booln;
    }

    GraphicsState Save() const
    {
        GraphicsState gstate;

        SetStatus(DllExports::GdipSaveGraphics(nativeGraphics, &gstate));

        return gstate;
    }

    Status Restore(IN GraphicsState gstate)
    {
        return SetStatus(DllExports::GdipRestoreGraphics(nativeGraphics,
                                                         gstate));
    }

    GraphicsContainer BeginContainer(IN const RectF &dstrect,
                                     IN const RectF &srcrect,
                                     IN Unit         unit)
    {
        GraphicsContainer state;

        SetStatus(DllExports::GdipBeginContainer(nativeGraphics, &dstrect,
                                                 &srcrect, unit, &state));

        return state;
    }

    GraphicsContainer BeginContainer(IN const Rect    &dstrect,
                                     IN const Rect    &srcrect,
                                     IN Unit           unit)
    {
        GraphicsContainer state;

        SetStatus(DllExports::GdipBeginContainerI(nativeGraphics, &dstrect,
                                                  &srcrect, unit, &state));

        return state;
    }

    GraphicsContainer BeginContainer()
    {
        GraphicsContainer state;

        SetStatus(DllExports::GdipBeginContainer2(nativeGraphics, &state));

        return state;
    }

    Status EndContainer(IN GraphicsContainer state)
    {
        return SetStatus(DllExports::GdipEndContainer(nativeGraphics, state));
    }

    // Only valid when recording metafiles.

    Status AddMetafileComment(IN const BYTE * data,
                              IN UINT sizeData)
    {
        return SetStatus(DllExports::GdipComment(nativeGraphics, sizeData, data));
    }

    static HPALETTE GetHalftonePalette()
    {
        return DllExports::GdipCreateHalftonePalette();
    }

    Status GetLastStatus() const
    {
        Status lastStatus = lastResult;
        lastResult = Ok;

        return lastStatus;
    }

private:
    Graphics(const Graphics &);
    Graphics& operator=(const Graphics &);

protected:
    Graphics(GpGraphics* graphics)
    {
        lastResult = Ok;
        SetNativeGraphics(graphics);
    }

    VOID SetNativeGraphics(GpGraphics *graphics)
    {
        this->nativeGraphics = graphics;
    }

    Status SetStatus(Status status) const
    {
        if (status != Ok)
            return (lastResult = status);
        else
            return status;
    }

    GpGraphics* GetNativeGraphics() const
    {
        return this->nativeGraphics;
    }

    GpPen* GetNativePen(const Pen* pen)
    {
        return pen->nativePen;
    }

protected:
    GpGraphics* nativeGraphics;
    mutable Status lastResult;



class Image : public GdiplusBase
{
public:
    friend class Brush;
    friend class TextureBrush;
    friend class Graphics;

    Image(
        IN const WCHAR* filename,
        IN BOOL useEmbeddedColorManagement = FALSE
    ){;}

    Image(
        IN IStream* stream,
        IN BOOL useEmbeddedColorManagement = FALSE
    ){;}

    static Image* FromFile(
        IN const WCHAR* filename,
        IN BOOL useEmbeddedColorManagement = FALSE
    );

    static Image* FromStream(
        IN IStream* stream,
        IN BOOL useEmbeddedColorManagement = FALSE
    );

    virtual ~Image(){;}
    virtual Image* Clone(){return 0;}

    Status Save(IN const WCHAR* filename,
                IN const CLSID* clsidEncoder,
                IN const EncoderParameters *encoderParams = NULL);
    Status Save(IN IStream* stream,
                IN const CLSID* clsidEncoder,
                IN const EncoderParameters *encoderParams = NULL);
    Status SaveAdd(IN const EncoderParameters* encoderParams);
    Status SaveAdd(IN Image* newImage,
                   IN const EncoderParameters* encoderParams);

    ImageType GetType() const;
    Status GetPhysicalDimension(OUT SizeF* size);
    Status GetBounds(OUT RectF* srcRect,
                     OUT Unit* srcUnit);

    UINT GetWidth(){return 0;}
    UINT GetHeight(){return 0;}
    REAL GetHorizontalResolution();
    REAL GetVerticalResolution();
    UINT GetFlags();
    Status GetRawFormat(OUT GUID *format);
    PixelFormat GetPixelFormat();

    INT GetPaletteSize();
    Status GetPalette(OUT ColorPalette* palette,
                      IN INT size);
    Status SetPalette(IN const ColorPalette* palette);

    Image* GetThumbnailImage(IN UINT thumbWidth,
                             IN UINT thumbHeight,
                             IN GetThumbnailImageAbort callback = NULL,
                             IN VOID* callbackData = NULL);
    UINT GetFrameDimensionsCount();
    Status GetFrameDimensionsList(OUT GUID* dimensionIDs,
                                  IN UINT count);
    UINT GetFrameCount(IN const GUID* dimensionID);
    Status SelectActiveFrame(IN const GUID* dimensionID,
                             IN UINT frameIndex);
    Status RotateFlip(IN RotateFlipType rotateFlipType);
    UINT GetPropertyCount();
    Status GetPropertyIdList(IN UINT numOfProperty,
                             OUT PROPID* list);
    UINT GetPropertyItemSize(IN PROPID propId);
    Status GetPropertyItem(IN PROPID propId,
                           IN UINT propSize,
                           OUT PropertyItem* buffer);
    Status GetPropertySize(OUT UINT* totalBufferSize,
                           OUT UINT* numProperties);
    Status GetAllPropertyItems(IN UINT totalBufferSize,
                               IN UINT numProperties,
                               OUT PropertyItem* allItems);
    Status RemovePropertyItem(IN PROPID propId);
    Status SetPropertyItem(IN const PropertyItem* item);

    UINT  GetEncoderParameterListSize(IN const CLSID* clsidEncoder);
    Status GetEncoderParameterList(IN const CLSID* clsidEncoder,
                                   IN UINT size,
                                   OUT EncoderParameters* buffer);
#if (GDIPVER >= 0x0110)
    Status FindFirstItem(IN ImageItemData *item);
    Status FindNextItem(IN ImageItemData *item);
    Status GetItemData(IN ImageItemData *item);
    Status SetAbort(GdiplusAbort *pIAbort);
#endif //(GDIPVER >= 0x0110)

    Status GetLastStatus() const { return Ok;}

protected:

    Image() {}

    Image(GpImage *nativeImage, Status status);

    VOID SetNativeImage(GpImage* nativeImage);

    Status SetStatus(Status status) const
    {
        if (status != Ok)
            return (lastResult = status);
        else
            return status;
    }

    GpImage* nativeImage;
    mutable Status lastResult;
    mutable Status loadStatus;

private:
    Image(IN const Image& C);
    Image& operator=(IN const Image& C);
};

class Bitmap : public Image
{
public:
    friend class Image;
    friend class CachedBitmap;

    Bitmap(
        IN const WCHAR *filename,
        IN BOOL useEmbeddedColorManagement = FALSE
    ){ return;}

    Bitmap(
        IN IStream *stream,
        IN BOOL useEmbeddedColorManagement = FALSE
    ){return;}

    static Bitmap* FromFile(
        IN const WCHAR *filename,
        IN BOOL useEmbeddedColorManagement = FALSE
    ){return 0;}

    static Bitmap* FromStream(
        IN IStream *stream,
        IN BOOL useEmbeddedColorManagement = FALSE
    );

    Bitmap(IN INT width,
           IN INT height,
           IN INT stride, PixelFormat format,
           IN BYTE* scan0);
    Bitmap(IN INT width,
           IN INT height,
           IN PixelFormat format = PixelFormat32bppARGB);
    Bitmap(IN INT width,
           IN INT height,
           IN  Graphics* target);

    Bitmap* Clone(IN const Rect& rect,
                  IN PixelFormat format);
    Bitmap* Clone(IN INT x,
                  IN INT y,
                  IN INT width,
                  IN INT height,
                  IN PixelFormat format);
    Bitmap* Clone(IN const RectF& rect,
                  IN PixelFormat format);
    Bitmap* Clone(IN REAL x,
                  IN REAL y,
                  IN REAL width,
                  IN REAL height,
                  IN PixelFormat format);

    Status LockBits(IN const Rect* rect,
                    IN UINT flags,
                    IN PixelFormat format,
                    OUT BitmapData* lockedBitmapData){return Ok;}
    Status UnlockBits(IN BitmapData* lockedBitmapData){return Ok;}
    Status GetPixel(IN INT x,
                    IN INT y,
                    OUT Color *color);
    Status SetPixel(IN INT x,
                    IN INT y,
                    IN const Color &color);

#if (GDIPVER >= 0x0110)
    Status ConvertFormat(
        PixelFormat format,
        DitherType dithertype,
        PaletteType palettetype,
        ColorPalette *palette,
        REAL alphaThresholdPercent
        );

    // The palette must be allocated and count must be set to the number of
    // entries in the palette. If there are not enough, the API will fail.

    static Status InitializePalette(
        IN OUT ColorPalette *palette,  // Palette to initialize.
        PaletteType palettetype,       // palette enumeration type.
        INT optimalColors,             // how many optimal colors
        BOOL useTransparentColor,      // add a transparent color to the palette.
        Bitmap *bitmap                 // optional bitmap for median cut.
        );

    Status ApplyEffect(Effect *effect, RECT *ROI);

    static Status
    ApplyEffect(
        IN  Bitmap **inputs,
        IN  INT numInputs,
        IN  Effect *effect,
        IN  RECT *ROI,           // optional parameter.
        OUT RECT *outputRect,    // optional parameter.
        OUT Bitmap **output
    );

    Status GetHistogram(
        IN HistogramFormat format,
        IN UINT NumberOfEntries,
        __out_bcount(sizeof(UINT)*256) UINT *channel0,
        __out_bcount(sizeof(UINT)*256) UINT *channel1,
        __out_bcount(sizeof(UINT)*256) UINT *channel2,
        __out_bcount(sizeof(UINT)*256) UINT *channel3
    );

    static Status GetHistogramSize(
        IN HistogramFormat format,
        OUT UINT *NumberOfEntries
    );
#endif //(GDIPVER >= 0x0110)

    Status SetResolution(IN REAL xdpi,
                         IN REAL ydpi);

    Bitmap(IN IDirectDrawSurface7* surface);
    Bitmap(IN const BITMAPINFO* gdiBitmapInfo,
           IN VOID* gdiBitmapData);
    Bitmap(IN RCHBITMAP hbm,
           IN HPALETTE hpal);
    Bitmap(IN HICON hicon);
    Bitmap(IN RCHINSTANCE hInstance,
           IN const WCHAR * bitmapName);
    static Bitmap* FromDirectDrawSurface7(IN IDirectDrawSurface7* surface);
    static Bitmap* FromBITMAPINFO(IN const BITMAPINFO* gdiBitmapInfo,
                                  IN VOID* gdiBitmapData);
    static Bitmap* FromHBITMAP(IN RCHBITMAP hbm,
                               IN HPALETTE hpal);
    static Bitmap* FromHICON(IN HICON hicon);
    static Bitmap* FromResource(IN RCHINSTANCE hInstance,
                                IN const WCHAR * bitmapName);

    Status GetHBITMAP(IN const Color& colorBackground,
                      OUT RCHBITMAP *hbmReturn);
    Status GetHICON(HICON *hicon);

private:
    Bitmap(const Bitmap &);
    Bitmap& operator=(const Bitmap &);

protected:
    Bitmap(GpBitmap *nativeBitmap);
};

} // Gdiplus
*/


#define CDM_FIRST       (WM_USER + 100)
#define CDM_GETFILEPATH         (CDM_FIRST + 0x0001)
#define CommDlg_OpenSave_GetFilePathA(_hdlg, _psz, _cbmax) \
        (int)SNDMSG(_hdlg, CDM_GETFILEPATH, (LPARAM)(_cbmax), (LPARAM)(LPSTR)(_psz))
#define CommDlg_OpenSave_GetFilePathW(_hdlg, _psz, _cbmax) \
        (int)SNDMSG(_hdlg, CDM_GETFILEPATH, (LPARAM)(_cbmax), (LPARAM)(LPWSTR)(_psz))
#ifdef UNICODE
#define CommDlg_OpenSave_GetFilePath  CommDlg_OpenSave_GetFilePathW
#else
#define CommDlg_OpenSave_GetFilePath  CommDlg_OpenSave_GetFilePathA
#endif // !UNICODE

typedef LRESULT (CALLBACK* HOOKPROC)(int code, WPARAM wParam, LPARAM lParam);

#ifdef _MAC
#define WINUSERAPI
inline
WINUSERAPI
HHOOK
WINAPI
SetWindowsHookA(
    __in int nFilterType,
    __in HOOKPROC pfnFilterProc){return 0;}
inline
WINUSERAPI
HHOOK
WINAPI
SetWindowsHookW(
    __in int nFilterType,
    __in HOOKPROC pfnFilterProc){return 0;}
#ifdef UNICODE
#define SetWindowsHook  SetWindowsHookW
#else
#define SetWindowsHook  SetWindowsHookA
#endif // !UNICODE

#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////

#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define POINTSTOPOINT(pt, pts)                          \
        { (pt).x = (LONG)(SHORT)LOWORD(*(LONG*)&pts);   \
          (pt).y = (LONG)(SHORT)HIWORD(*(LONG*)&pts); }

#define POINTTOPOINTS(pt)      (MAKELONG((short)((pt).x), (short)((pt).y)))
#define MAKEWPARAM(l, h)      ((WPARAM)(DWORD)MAKELONG(l, h))
#define MAKELPARAM(l, h)      ((LPARAM)(DWORD)MAKELONG(l, h))
#define MAKELRESULT(l, h)     ((LRESULT)(DWORD)MAKELONG(l, h))

typedef LRESULT (CALLBACK* HOOKPROC)(int code, WPARAM wParam, LPARAM lParam);
inline UINT RCRegisterClipboardFormat( LPCTSTR lpszFormat)
{
    return 0;
}

inline UINT RCRegisterClipboardFormat( RCCHAR* lpszFormat)
{
    return 0;
}

inline UINT RCRegisterClipboardFormat( const char* lpszFormat)
{
    return 0;
}

extern RCSHORT RCGetAsyncKeyState( int vKey );

#ifdef _USING_QTCHAR
inline void printQChar( const QChar * out )
{
#ifdef _DEBUG_RCCHARS
    /*
    QTextStream(stdout) << endl;
    const QChar *op;
    QString msg;
    for(op=out;*op!=0;op++)
    {
        msg += *op;
        QTextStream(stdout) << *op;
    }
    */
#ifdef _WIN32
    //RCMessageBox(NULL,(_TCHAR*)qPrintable(msg),_T(""),MB_OK);
     //MessageBox(NULL,(LPCSTR)msg.data(),_T(""),MB_OK);
#endif
#endif
}
#endif

#ifdef _USING_QTCHAR
inline RCCHAR * LOCAL2RCHR( RCCHAR * qstr, TCHAR * lstr )
{
#ifdef UNICODE
   //QString str = QString::fromUtf16((RCUSHORT *)lstr, _tcslen(lstr));
   //QString str = QString::fromUtf16((RCUSHORT *)lstr, _tcsclen(lstr));
   QString str = QString::fromUtf16((RCUSHORT *)lstr, wcslen(lstr));
#else
    //QString str = QString::fromLocal8Bit(lstr, _tcslen(lstr));
    QString str = QString::fromLocal8Bit(lstr, strlen(lstr));
#endif
   QChar *cs = qstr, *out=qstr;
   for(int i=0; i<=str.size(); i++)
   {
       *cs = str[i];
       cs++;
   }
   return out;
}
#else
inline RCCHAR * LOCAL2RCHR( RCCHAR * qstr, TCHAR * lstr )
{
    return _tcscpy( qstr, lstr );
}
#endif

#ifdef _USING_QTCHAR
inline TCHAR * RCHR2LOCAL( TCHAR * lstr, RCCHAR * qstr  )
{
   QByteArray lbta = QString(qstr).toLocal8Bit();
   TCHAR *cs = lstr, *out=lstr;
   for(int i=0; i<=lbta.size(); i++){
       *cs = lbta[i];
       cs++;
   }
   return out;
}

inline char * RCHR2LOCAL( char * lstr, RCCHAR * qstr  )
{
   return QString(qstr).toLocal8Bit().data();
   /*
   QByteArray lbta = QString(qstr).toLocal8Bit();
   char *cs = lstr, *out=lstr;
   for(int i=0; i<=lbta.size(); i++){
       *cs = lbta[i];
       cs++;
   }
   return out;
   */
}

inline char * RCHR2LATIN1( char * lstr, RCCHAR * qstr  )
{
    return QString(qstr).toLatin1().data();
    /*
   QByteArray lbta = QString(qstr).toLatin1();
   char *cs = lstr, *out=lstr;
   for(int i=0; i<=lbta.size(); i++){
       *cs = lbta[i];
       cs++;
   }
   return out;
   */
}
#else
inline TCHAR * RCHR2LOCAL( RCCHAR * qstr, TCHAR * lstr )
{
    return _tcscpy( lstr, qstr );
}
#endif

#ifdef _USING_QTCHAR
//char * strcpy ( char * destination, const char * source );
inline RCCHAR * RCSTRCPY( RCCHAR * destination, const RCCHAR * source )
{
    const QChar *cs;
    QChar *cd;
    cd = destination;
    if(cd == 0)
        return destination;
    if(source !=0)
        for( cs=source; *cs != 0 ; cs++)
        {
            *cd = *cs;
            cd++;
        }
    *cd=0;
    return destination;
}

inline RCCHAR * RCSTRCPY( RCCHAR * destination, const TCHAR * source )
{
    const TCHAR *cs;
    QChar *cd=0;
    cd = destination;
    if(cd == 0)
        return destination;
    if(source !=0)
        for( cs=source; *cs != 0 ; cs++)
        {
            *cd = QChar(*cs);
             cd++;
        }
        *cd=0;
    return destination;
}

inline RCCHAR * RCSTRCPY( RCCHAR * destination, const char * source )
{
    const char *cs;
    QChar *cd=0;
    cd = destination;
    if(cd == 0)
        return destination;
    if(source != 0)
        for( cs=source; *cs != 0 ; cs++)
        {
            *cd = QChar(*cs);
             cd++;
        }
        *cd=0;
    return destination;
}

inline TCHAR * RCSTRCPY( TCHAR * destination, const RCCHAR * source )
{
    const RCCHAR *cs;
    TCHAR *cd;
    cd = destination;
    if(cd == 0)
        return destination;
    if(source != 0)
        for( cs=source; *cs != 0 ; cs++)
        {
             *cd = cs->toLatin1();
             cd++;
        }
        *cd=0;
    return destination;
}

inline char * RCSTRCPY( char * destination, const RCCHAR * source )
{
    const RCCHAR *cs;
    char *cd;
    cd = destination;
    if(cd == 0)
        return destination;
    if(source != 0)
        for( cs=source; *cs != 0 ; cs++)
        {
             *cd = cs->toLatin1();
             cd++;
        }
     *cd=0;
    return destination;
}

inline RCCHAR * RCSTRCPY( RCCHAR * destination, const RCSTRING * source )
{
    RCCHAR *cd;
    const RCCHAR *cs;
    cd = destination;
    cs = source->data();
    return RCSTRCPY(cd,cs);
}

inline RCCHAR * RCSTRCPY( RCCHAR * destination, const RCSTRING source )
{
    RCCHAR *cd;
    const RCCHAR *cs;
    cd = destination;
    cs = source.data();
    return RCSTRCPY(cd,cs);
}

inline TCHAR * RCSTRCPY( TCHAR * destination, const TCHAR * source )
{
#ifdef UNICODE
    return wcscpy( destination, source );
#else
    //return _tcscpy( destination, source );
    return strcpy( destination, source );
#endif
}
inline char * RCSTRCPY( char * destination, const char * source )
{
    return strcpy( destination, source );
}
#else
inline TCHAR * RCSTRCPY( TCHAR * destination, const TCHAR * source )
{
    return _tcscpy( destination, source );
}
#endif

#ifdef _USING_QTCHAR
//size_t strlen ( const char * str );
inline RCSIZE_T RCSTRLEN( const RCCHAR * str )
{
    QString qstr(str);
    return qstr.length();
    //return (RCSIZE_T) _tcsclen ( (const _TCHAR *)str );
}

inline RCSIZE_T RCSTRLEN( const TCHAR * str )
{
#ifdef UNICODE
    return wcslen ( (const TCHAR *)str );
#else
    //return _tcsclen ( (const _TCHAR *)str );
    return strlen ( (const TCHAR *)str );
#endif
}
#ifndef UNICODE
inline RCSIZE_T RCSTRLEN( const WCHAR * str )
{
    return wcslen ( (const WCHAR *)str );
}
#else
inline RCSIZE_T RCSTRLEN( const char * str )
{
    return strlen ( (const char *)str );
}
#endif
#else
inline size_t RCSTRLEN( const TCHAR * str )
{
    return _tcsclen ( (const TCHAR *)str );
}
#endif

#ifdef _USING_QTCHAR
//char * strcat ( char * destination, const char * source );
inline RCCHAR * RCSTRCAT ( RCCHAR * destination, const RCCHAR * source )
{
    //return (RCCHAR*) strcat ( (_TCHAR*)destination, (_TCHAR*)source );
    const QChar *cs;
    QChar *cd;
    cd = destination;
    for( cd=destination; *cd != 0 ; cd++)
    {
        //*cd = *cs;
        //cd++;
    }
    for( cs=source; *cs != 0 ; cs++)
    {
        *cd = 0;
        *cd = *cs;
        cd++;
    }
    *cd=0;
    return destination;

}

inline RCCHAR * RCSTRCAT ( RCCHAR * destination, const TCHAR * source )
{
#ifdef UNICODE
    return (RCCHAR*) wcscat ( (TCHAR*)destination, source );
#else
    //return (RCCHAR*) _tcscat ( (TCHAR*)destination, source );
    return (RCCHAR*) strcat ( (TCHAR*)destination, source );
#endif
}

inline TCHAR * RCSTRCAT ( TCHAR * destination , const RCCHAR * source )
{
    const QChar *cs;
    TCHAR *cd;
    cd = destination;
    for( cd=destination; *cd != 0 ; cd++)
    {
        //*cd = *cs;
        //cd++;
    }
    for( cs=source; *cs != 0 ; cs++)
    {
        *cd = 0;
        *cd = cs->toLatin1();
        cd++;
    }
    *cd=0;
    return destination;
}

inline RCCHAR * RCSTRCAT ( RCCHAR * destination, const char * source )
{
    const char *cs;
    RCCHAR *cd;
    cd = destination;
    for( cd=destination; *cd != 0 ; cd++)
    {
        //*cd = *cs;
        //cd++;
    }
    for( cs=source; *cs != 0 ; cs++)
    {
        *cd = 0;
        *cd = QChar(*cs);
        cd++;
    }
    *cd=0;
    return destination;
}

inline char * RCSTRCAT ( char * destination, RCCHAR * source )
{
    const QChar *cs;
    char *cd;
    cd = (char*)destination;
    for( cd=(char*)destination; *cd != 0 ; cd++)
    {
        //*cd = *cs;
        //cd++;
    }
    for( cs=source; *cs != 0 ; cs++)
    {
        *cd = 0;
        *cd = cs->toLatin1();
        cd++;
    }
    *cd=0;
    return (char*)destination;
}

inline char * RCSTRCAT ( char * destination, const RCCHAR * source )
{
    return RCSTRCAT( (char*)destination, (RCCHAR*)source);
}

inline TCHAR * RCSTRCAT ( TCHAR * destination, const TCHAR * source )
{
#ifdef UNICODE
    return (TCHAR*) wcscat ( (TCHAR*)destination, source );
    //return (TCHAR*) _tcscat ( (TCHAR*)destination, source );
#else
    return (TCHAR*) strcat ( (TCHAR*)destination, source );
#endif
}
#ifdef UNICODE
inline char * RCSTRCAT ( char * destination, const char * source )
{
    return strcat ( (char*)destination, source );
}
#endif
#else
inline _TCHAR * RCSTRCAT ( _TCHAR * destination, _TCHAR * source )
{
    return  _tcscat ( destination, source );
}
#endif

#ifdef _USING_QTCHAR
//char * strncat ( char * destination, char * source, size_t num );
inline RCCHAR * RCSTRNCAT( RCCHAR * destination, RCCHAR * source, RCSIZE_T num )
{
#ifdef UNICODE
    return (RCCHAR *) wcsncat ( (TCHAR*)destination, (TCHAR*)source, num );
#else
    //return (RCCHAR *) _tcsncat ( (TCHAR*)destination, (TCHAR*)source, num );
    return (RCCHAR *) strncat ( (TCHAR*)destination, (TCHAR*)source, num );
#endif
}
#endif

#ifdef _USING_QTCHAR
//char * strncpy ( char * destination, const char * source, size_t num );
inline RCCHAR * RCSTRNCPY ( RCCHAR * destination, const RCCHAR * source, RCSIZE_T num )
{
    //return (RCCHAR *) _tcsncpy ( (TCHAR*)destination, (TCHAR*)source, num );
    const RCCHAR *cs;
    RCCHAR *cd;
    cd = destination;
    int cnt=0;
    for( cs=source; *cs != 0 ; cs++)
    {
        *cd = *cs;
        cd++;
        cnt++;
    }
    for(int i=cnt; i<num; i++)
    {
        *cd = 0;
    }
    //*cd=0;
    return destination;
}

inline TCHAR * RCSTRNCPY ( LPTSTR destination, const char * source, RCSIZE_T num )
{
#ifdef UNICODE
    return wcsncpy ( destination,QString(source).toStdWString().c_str(), num);
    //return wcsncpy ( destination, source, num );
#else
    //return _tcsncpy ( destination, source, num );
    return strncpy ( destination, source, num );
#endif
}

inline TCHAR * RCSTRNCPY ( TCHAR * destination, const TCHAR * source, RCSIZE_T num )
{
#ifdef UNICODE
    return wcsncpy ( destination, source, num );
#else
    //return _tcsncpy ( destination, source, num );
    return strncpy ( destination, source, num );
#endif

}
#ifdef UNICODE
inline char * RCSTRNCPY ( char * destination, const char * source, RCSIZE_T num )
{

    return strncpy ( destination, source, num );
}
#endif
#else
inline TCHAR * RCSTRNCPY ( TCHAR * destination, const TCHAR * source, RCSIZE_T num )
{
    return _tcsncpy ( destination, source, num );
}
#endif

#ifdef _USING_QTCHAR
/*
inline RCCHAR * RCSTRCPY( RCCHAR * destination, const char * source )
{
    const char *cs;
    QChar *cd=0;
    cd = destination;
    for( cs=source; *cs != 0 ; cs++)
    {
        *cd = QChar(*cs);
         cd++;
    }
    *cd=0;

    return destination;
}
*/
#endif

#ifdef _USING_QTCHAR
/*
inline char * RCSTRCPY( char * destination, const RCCHAR * source )
{
    const RCCHAR *cs;
    char *cd;
    cd = destination;
    for( cs=source; *cs != 0 ; cs++)
    {
        *cd = cs->toLatin1();
         cd++;
    }
    *cd=0;

    return destination;
}
*/
#endif

#ifdef _USING_QTCHAR
//int strcmp ( const char * str1, const char * str2 );
inline RCINT RCSTRCMP ( const RCSTRING str1, const RCSTRING str2 )
{
    return RCSTRING::compare(str1,str2);
}

inline RCINT RCSTRCMP ( const char * str1, const RCSTRING str2 )
{
    return RCSTRING::compare(QString(str1),str2);
}

inline RCINT RCSTRCMP ( const RCSTRING str2, const char * str1 )
{
    return RCSTRING::compare(str1,QString(str2));
}

inline RCINT RCSTRCMP ( const RCCHAR * str1, const RCCHAR * str2 )
{
    RCSTRING qstr1(str1),qstr2(str2);
    return RCSTRING::compare(qstr1,qstr2);
}

inline RCINT RCSTRCMP ( const RCCHAR * str1, const RCSTRING * str2 )
{
    return RCSTRING::compare(RCSTRING(str1),str2);
}

inline RCINT RCSTRCMP ( const RCCHAR * str1, const char * str2 )
{
    return RCSTRING::compare(RCSTRING(str1),RCSTRING(str2));
}

inline RCINT RCSTRCMP ( const char * str1, const RCCHAR * str2 )
{
    return RCSTRING::compare(RCSTRING(str1),RCSTRING(str2));;
}

inline RCINT RCSTRCMP ( const char * str1, LPCTSTR str2 )
{
#ifdef UNICODE
    return RCSTRING::compare(RCSTRING(str1),QString::fromWCharArray((LPCWSTR)str2,wcslen((LPCWSTR)str2)));
#else
    return RCSTRING::compare(RCSTRING(str1),QString(str2);
#endif
}

inline RCINT RCSTRCMP ( const TCHAR * str1, const TCHAR * str2 )
{
#ifdef UNICODE
    return (RCINT) wcscmp ( (TCHAR*) str1, (TCHAR*) str2 );
#else
    return (RCINT) strcmp ( (TCHAR*) str1, (TCHAR*) str2 );
#endif
}
#ifdef UNICODE
inline RCINT RCSTRCMP ( const char * str1, const char * str2 )
{
    return (RCINT) strcmp ( str1, str2 );
}
#endif
#else
inline RCINT RCSTRCMP ( const TCHAR * str1, const TCHAR * str2 )
{
   return (RCINT) _tcscmp ( (TCHAR*) str1, (TCHAR*) str2 );
}
#endif


#ifdef _USING_QTCHAR
//int _stricmp( const char *string1, const char *string2 );
inline RCINT RCSTRICMP( const RCCHAR *string1, const RCCHAR *string2 )
{
    return RCSTRING::compare(RCSTRING(string1),RCSTRING(string2),Qt::CaseInsensitive);
}

inline RCINT RCSTRICMP( const RCCHAR *string1, const char *string2 )
{
    return RCSTRING::compare(RCSTRING(string1),RCSTRING(string2),Qt::CaseInsensitive);
}

inline RCINT RCSTRICMP( const TCHAR *string1, const TCHAR *string2 )
{
#ifdef UNICODE
    return RCSTRING::compare(QString::fromWCharArray((LPCWSTR)string1,wcslen((LPCWSTR)string1)),
                             QString::fromWCharArray((LPCWSTR)string2,wcslen((LPCWSTR)string2)),Qt::CaseInsensitive);
#else
    return RCSTRING::compare(RCSTRING(string1),RCSTRING(string2),Qt::CaseInsensitive);
#endif
}

inline RCINT RCSTRICMP( const char *string1, const TCHAR *string2 )
{
#ifdef UNICODE
    return RCSTRING::compare(RCSTRING(string1),
                             QString::fromWCharArray((LPCWSTR)string2,wcslen((LPCWSTR)string2)),Qt::CaseInsensitive);
#else
    return RCSTRING::compare(RCSTRING(string1),RCSTRING(string2),Qt::CaseInsensitive);
#endif
}

#ifdef UNICODE
inline RCINT RCSTRICMP( const char *string1, const char *string2 )
{
#ifdef _MAC
    return RCSTRING::compare(RCSTRING(string1),RCSTRING(string2),Qt::CaseInsensitive);
#else
    return (RCINT) stricmp( string1, string2 );
#endif
}
#endif
#else
inline RCINT RCSTRICMP( const _TCHAR *string1, const _TCHAR *string2 )
{
    return _tcsicmp(  string1,  string2 );
}
#endif


#ifdef _USING_QTCHAR
inline RCCHAR * RCSTRUPR( RCCHAR * source )
{
    RCCHAR *cs, *out;
    cs = out = source;
    for( cs=source; *cs != 0 ; cs++)
        *cs = cs->toUpper();

    return out;
}
inline TCHAR * RCSTRUPR( TCHAR * s )
{
#ifdef _MAC
    TCHAR* p = s;
    while (*p = towupper( *p )) p++;
    return s;
#else
    return _wcsupr (s);
#endif
}

inline char* RCSTRUPR( char* s )
{
  char* p = s;
  while (*p = toupper( *p )) p++;
  return s;
}

/* alternative method
#include <algorithm>
#include <cctype>
#include <functional>

std::string& stoupper( const std::string& s )
{
  std::string result( s );
  std::transform(
    s.begin(),
    s.end(),
    result.begin(),
    std::ptr_fun <int, int> ( std::toupper )
    );
  return result;
}
*/
#endif

#ifdef _USING_QTCHAR
inline RCCHAR * RCSTRLWR( RCCHAR * source )
{
    RCCHAR *cs, *out;
    cs = out = source;
    for( cs=source; *cs != 0 ; cs++)
        *cs = cs->toLower();

    return out;
}

inline char* RCSTRLWR( char* s )
{
  char* p = s;
  while (*p = tolower( *p )) p++;
  return s;
}

inline TCHAR * RCSTRLWR( TCHAR * s )
{
#ifdef WIN32
#ifdef UNICODE
     return _wcslwr ( s );
#else
    return strlwr( s );
#endif
#else
#ifdef UNICODE
    TCHAR* p = s;
    while (*p = tolower( *p )) p++;
    return s;
#else
  char* p = s;
  while (*p = tolower( *p )) p++;
  return s;
#endif
#endif
}
#else
inline TCHAR * RCSTRLWR( TCHAR * s )
{
#ifdef WIN32
    return _tcslwr( s );
//}
#else
//char* strlwr( char* s )
//{
  char* p = s;
  while (*p = tolower( *p )) p++;
  return s;
#endif
}
/* alternative method
#include <algorithm>
#include <cctype>
#include <functional>

std::string& stolower( const std::string& s )
{
  std::string result( s );
  std::transform(
    s.begin(),
    s.end(),
    result.begin(),
    std::ptr_fun <int, int> ( std::tolower )
    );
  return result;
}
*/
#endif

//double atof (const char * str);
#ifdef _USING_QTCHAR
inline RCDOUBLE RCATOF (const RCCHAR * str)
{
    //return (RCDOUBLE) _tstof ((TCHAR*)str);
    QString qstr = QString(str);
    return qstr.toDouble();
}

inline RCDOUBLE RCATOF (const TCHAR * str)
{
#ifdef UNICODE
    return wcstod (str,NULL);
#else
    //return _tstof (str);
    return atof (str);
#endif
}
#ifdef UNICODE
inline RCDOUBLE RCATOF (const char  * str)
{
    return atof (str);
}
#endif
#else
inline RCDOUBLE RCATOF (const _TCHAR * str)
{
    return _tstof (str);
}
#endif

#ifdef _USING_QT
//int tolower( int c );
inline RCCHAR RCTOLOWER( RCCHAR c )
{
    return c.toLower();
}

inline char RCTOLOWER( char c )
{
    return tolower(c);
}
#endif

#ifdef _USING_QTCHAR
//int atoi (const char * str);
inline RCINT RCATOI (const RCCHAR * str)
{
    bool ok=false;
    QString string1(str),string2("");
    for(int i=0; i<string1.length(); i++)
    {
        if(!string1.at(i).isNumber() && !string1.at(i).isSpace())
            break;
        else
            string2 += string1.at(i);
    }
    int ret = string2.toInt(&ok);
    if(ok)
        return ret;
    else
        return 0;
    //return (RCINT) wcstol ((TCHAR*)str,NULL,10);
    //return (RCINT) _tstoi ((TCHAR*)str);
    //return (RCINT) atoi ((TCHAR*)str);
}

inline RCINT RCATOI (const TCHAR * str)
{
#ifdef UNICODE
    return  wcstol (str,NULL,10);
#else
    //return  _tstoi (str);
    return  atoi (str);
#endif
}
#ifdef UNICODE
inline RCINT RCATOI (const char * str)
{
    return  atoi (str);
}
#endif
#else
inline RCINT RCATOI (const TCHAR * str)
{
    return  _tstoi (str);
}
#endif

#ifdef _USING_QTCHAR
//double strtod( const char *nptr, char **endptr );
inline double RCSTRTOD( QString num, QString end  )
{
    bool ok;
    double cv = num.toDouble(&ok);
    if(ok)
    {
        end = "";
        return cv;
    }
    else
    {
        end = num;
        return 0.0;
    }
}

inline double RCSTRTOD( const RCCHAR *nptr, RCCHAR **endptr  )
{
    //QString dbg(nptr);
    char cnptr[300];
    RCSTRCPY(cnptr,nptr);
    char *ceptr;
    double val=0.0;

    if(endptr)
    {
        val = ::strtod(cnptr,&ceptr);
        if(*ceptr != 0)
        {
            char *cp; RCCHAR *rc = (RCCHAR*)nptr;
            for(cp=cnptr; cp!=ceptr; cp++)
                rc++;
            *endptr=(RCCHAR*)rc;
        }
        else
            *endptr=(RCCHAR*)nptr+strlen(cnptr);
        return val;
    }
    else
    {
        val =strtod(cnptr,0);
        return val;
    }
}

inline double RCSTRTOD( const TCHAR *nptr, TCHAR **endptr  )
{
#ifdef UNICODE
    return ::wcstod ( nptr, endptr );
#else
    return ::strtod( nptr, endptr );
#endif
}

inline double RCSTRTOD( const RCCHAR *nptr, TCHAR **endptr  )
{
#ifdef UNICODE
    return ::wcstod ( (TCHAR*)nptr, endptr );
#else
    return ::strtod( (TCHAR*)nptr, endptr );
#endif
}

inline double RCSTRTOD( const char *nptr, char **endptr  )
{
    return ::strtod( nptr, endptr );
}

#else
inline double RCSTRTOD( const TCHAR *nptr, TCHAR **endptr  )
{
    return _tcstod( nptr, endptr );
}
#endif

#ifdef _USING_QTCHAR
#define RCPRINTABLE(a) ( qPrintable(RCSTRING(a)) )
#else
#define RCPRINTABLE(a) ( a )
#endif

#ifdef _USING_QTCHAR
inline RCCHAR * _Char2Rcchar( const char * _c )
{
    return (RCCHAR*)QString(_c).data();
}

inline RCCHAR * _Char2Rcchar( const TCHAR * _c )
{
    return QString().fromWCharArray(_c).data();
}

inline RCCHAR * _Char2Rcchar( const RCCHAR * _c )
{
    return (RCCHAR*)_c;
}

inline char* _RCchr2Char ( const RCCHAR * _c )
{
    return (char*)QString(_c).toStdString().c_str();
}

inline char _RCchr2Char (  RCCHAR * _c )
{
    return *QString(_c).toStdString().c_str();
}

inline char _RCPchr2PChar ( RCCHAR _c )
{
    return (char)*QString(_c).toStdString().c_str();
}

inline char* _RCPchr2PChar ( RCCHAR * _c )
{
    return (char*)QString(_c).toStdString().c_str();
}

inline char* _RCPchr2PChar ( const RCCHAR * _c )
{
    return (char*)QString(_c).toStdString().c_str();
}

//#define RCCHR2CHAR(a) ( _RCchr2Char(a) )
//#define RCPCHR2PCHAR(a) ( _RCPchr2PChar(a) )
//#define CHAR2RCCHR(a) ( _Char2Rcchar(a) )
#define CHAR2RCCHR(a)    ( QString(a).data() )
#define TCHAR2RCCHR(a)   ( QString().fromWCharArray(a).data() )
#define RCPCHR2TCHAR(a)  ( (TCHAR)*QString(a).toStdWString().c_str() )
#define RCPCHR2PTCHAR(a) ( (TCHAR*)QString(a).toStdWString().c_str() )
#define RCCHR2CHAR(a)    ( (char*)(QString(a).toStdString().c_str()) )
#define RC2C(a)          ( (char*)(QString(a).toStdString().c_str()) )
//#define RCPCHR2PCHAR(a) ( (TCHAR*)(a->toLatin1()) )
#else
#define CHAR2RCCHR(a) ( a )
#define TCHAR2RCCHR(a) ( a )
#define RCPCHR2TCHAR(a) ( a )
#define RCPCHR2PTCHAR(a) ( a )
#define RCCHR2CHAR(a) ( a )
#endif

#ifdef _USING_QTCHAR
#define RCCHAR2UINT(a) ( (RCUINT)(a.toLatin1()) )
#else
#define RCCHAR2UINT(a) ( a )
#endif

//long strtol( const char *nptr, char **endptr, int base );
#ifdef _USING_QTCHAR
inline RCLONG RCCHAR2LONG( const RCCHAR *nptr, RCCHAR **endptr, RCINT base )
{
#ifdef UNICODE
    return (RCLONG) wcstol( (TCHAR*)nptr, (TCHAR**)endptr,  base );
#else
    //return (RCLONG) _tcstol( (TCHAR*)nptr, (TCHAR**)endptr,  base );
    return (RCLONG) strtol( (TCHAR*)nptr, (TCHAR**)endptr,  base );
#endif
}
#else
inline RCLONG RCCHAR2LONG( const RCCHAR *nptr, RCCHAR **endptr, RCINT base )
{
    return _tcstol(   nptr,  endptr,  base );
}
#endif

#ifdef _USING_QTCHAR
#define _RCT(a) (QString(a).data())
#define _RCTT(a) (TCHAR2RCCHR(a))
#else
#define _RCT(a) ( a )
#endif

#ifdef _USING_QTCHAR

/*
int sscanf(
   const char *buffer,
   const char *format [,
      argument ] ...
);
*/
// possible solution for the lack of vscanf in VS C++
// the VS 2013 will have this function so we will use that one
/*
template < size_t _NumArgs >

int VSSCANF_S(LPCTSTR strSrc, LPCTSTR ptcFmt, INT_PTR (&arr)[_NumArgs]) {

class vaArgs
      {
      vaArgs() {}
      INT_PTR* m_args[_NumArgs];
      public:
         vaArgs(INT_PTR (&arr)[_NumArgs])
                {
                for(size_t nIndex=0;nIndex<_NumArgs;++nIndex)
                    m_args[nIndex] = &arr[nIndex];
                }
      };

   return sscanf_s(strSrc, ptcFmt, vaArgs(arr));
   }
*/
// this is a replacement for the one below which stopped working
// when built for the windows 64 bit platform
// this function is only used on Windows
inline int RCVSSCANF(const char *s, const char *fmt, va_list ap)
{
  void *a[30];
  int i;
  for (i=0; i<sizeof(a)/sizeof(a[0]); i++) a[i] = va_arg(ap, void *);
  return sscanf(s, fmt,
                a[0],  a[1],  a[2],  a[3],  a[4],
                a[5],  a[6],  a[7],  a[8],  a[9],
                a[10], a[11], a[12], a[13], a[14],
                a[15], a[16], a[17], a[18], a[19],
                a[20], a[21], a[22], a[23], a[24],
                a[25], a[26], a[27], a[28], a[29]
               );
}
/*
inline int RCVSSCANF(const char *str, const char *format, va_list ap)
{
    unsigned int f01 = va_arg(ap, long);
    unsigned int f02 = va_arg(ap, long);
    unsigned int f03 = va_arg(ap, long);
    unsigned int f04 = va_arg(ap, long);
    unsigned int f05 = va_arg(ap, long);
    unsigned int f06 = va_arg(ap, long);
    unsigned int f07 = va_arg(ap, long);
    unsigned int f08 = va_arg(ap, long);
    unsigned int f09 = va_arg(ap, long);
    unsigned int f10 = va_arg(ap, long);
    unsigned int f11 = va_arg(ap, long);
    unsigned int f12 = va_arg(ap, long);
    unsigned int f13 = va_arg(ap, long);
    unsigned int f14 = va_arg(ap, long);
    unsigned int f15 = va_arg(ap, long);
    unsigned int f16 = va_arg(ap, long);
    unsigned int f17 = va_arg(ap, long);
    unsigned int f18 = va_arg(ap, long);
    unsigned int f19 = va_arg(ap, long);
    unsigned int f20 = va_arg(ap, long);
    unsigned int f21 = va_arg(ap, long);
    unsigned int f22 = va_arg(ap, long);
    unsigned int f23 = va_arg(ap, long);
    unsigned int f24 = va_arg(ap, long);
    unsigned int f25 = va_arg(ap, long);
    unsigned int f26 = va_arg(ap, long);
    unsigned int f27 = va_arg(ap, long);
    unsigned int f28 = va_arg(ap, long);
    unsigned int f29 = va_arg(ap, long);
    unsigned int f30 = va_arg(ap, long);

    return sscanf(str, format, f01, f02, f03, f04, f05, f06, f07, f08, f09, f10,
            f11, f12, f13, f14, f15, f16, f17, f18, f19, f20,
            f21, f22, f23, f24, f25, f26, f27, f28, f29, f30);
}
*/
inline RCINT RCSSCANF( const RCCHAR *buffer, const RCCHAR *format, ... )
{
    QString qbuffer = QString(buffer);
    QString qformat = QString(format);
    char *cbuffer = (char*)malloc(strlen(qbuffer.toStdString().c_str())+1);
    memcpy(cbuffer,qbuffer.toStdString().c_str(),strlen(qbuffer.toStdString().c_str())+1);
    char *cformat = (char*)malloc(strlen(qformat.toStdString().c_str())+1);
    memcpy(cformat,qformat.toStdString().c_str(),strlen(qformat.toStdString().c_str())+1);
    va_list args;
    va_start(args, format);
#ifdef _MAC
    RCINT ret = vsscanf( cbuffer, cformat,args );
#else
    RCINT ret = RCVSSCANF( cbuffer, cformat, args );
#endif
    va_end(args);
    free(cbuffer);
    free(cformat);
    return ret;
}

inline RCINT RCSSCANF( const RCCHAR *buffer, const char *format, ... )
{
    QString qbuffer = QString(buffer).toStdString().c_str();
    char *cbuffer = (char*)malloc(strlen(qbuffer.toStdString().c_str())+1);
    memcpy(cbuffer,qbuffer.toStdString().c_str(),strlen(qbuffer.toStdString().c_str())+1);
    va_list args;
    va_start(args, format);
#ifdef _MAC
    RCINT ret = vsscanf( cbuffer, format, args );
#else
    RCINT ret = RCVSSCANF(  cbuffer, format, args);
#endif
    va_end(args);
    free(cbuffer);
    return ret;
}
inline int RCSSCANF( const char *buffer, const char *format, ... )
{
    va_list args;
    va_start(args, format);
#ifdef _MAC
    RCINT ret = vsscanf( buffer, format, args );
#else
    RCINT ret = RCVSSCANF(  buffer, format, args);
#endif
    va_end(args);
    return ret;
}
#endif

#ifdef _USING_QTCHAR
inline int RCFSCANF( FILE * stream, const RCCHAR *format, ... )
{
    return 0;
}

inline int RCFSCANF( FILE * stream, const char *format, ... )
{
    va_list args;
    va_start (args, format);
    int ret = vfscanf (stream, format, args);
    va_end (args);
    return ret;
}
#endif

#ifdef _USING_QTCHAR
//int sprintf( char *buffer, const char *format [,argument] ... );
inline RCINT RCSPRINTF( RCCHAR * buffer, const char * format , ... )
{
#if 0
    QString str;
    char buf[300];
    va_list args;
    va_start(args, format);
    vsnprintf(buf,300,format,args);
    str = QString::fromUtf8(buf);
    va_end(args);

    QChar *s=buffer;
    int i;
    for(i=0,s=buffer; i<=str.size(); i++,s++)
        *s = str[i];
    return i;
#endif
#if 1
    va_list args;
    va_start(args, format);
    QString str,qsformat(format);
    str.vsprintf(qsformat.toStdString().c_str(),args); // deprecated in qt
    //str.vasprintf(qsformat.toStdString().c_str(),args);// replacement for vsprint
    va_end(args);
    RCCHAR *s=buffer;
    int i;
    //for(i=0,s=buffer; i<=str.size(); i++,s++)
    for(i=0,s=buffer; i<str.size(); i++,s++)
    {
        *s = str[i];
    }
    *s++ = 0;
    return i;
#endif
}

inline RCINT RCSPRINTF( RCCHAR * buffer, const RCCHAR * format , ... )
{
#if 0
    QString str;
    QChar buf[300];
    va_list args;
    va_start(args, format);
    //_vsnprintf(buf,300,format,args);
    vswprintf((wchar_t*)buf,300,(wchar_t*)format,args);
    str = QString(buf);
    va_end(args);

    QChar *s=buffer;
    int i;
    for(i=0,s=buffer; i<=str.size(); i++,s++)
        *s = str[i];
    return i;
#endif
#if 1
    va_list args;
    //va_start(args, format);
    va_start(args, (const char*&)format);
    //va_start(args, (const char*)format);
    QString str(""),qsformat(format);
    str.vsprintf(qsformat.toStdString().c_str(),args); //deprecated in Qt
    //str.vasprintf(qsformat.toStdString().c_str(),args);
    va_end(args);
    QChar *s=buffer;
    int i;
    for(i=0,s=buffer; i<str.size(); i++,s++)
    {
        *s = str[i];
    }
    *s++ = 0;
    return i;
#endif
}
inline RCINT RCSPRINTF( TCHAR * buffer, const TCHAR * format , ... )
{
    va_list args;
    va_start(args, format);
#ifdef UNICODE
#ifdef _MAC
    RCINT ret = vswprintf ( buffer, wcslen(buffer)+1 ,format , args );
#else
    RCINT ret = vswprintf ( buffer, format , args );
#endif
#else
    RCINT ret = vsprintf( buffer, format , args );
#endif
    va_end(args);
    return ret;
}
#ifdef UNICODE
inline RCINT RCSPRINTF( char * buffer, const char * format , ... )
{
    va_list args;
    va_start(args, format);
    RCINT ret = vsprintf( buffer, format , args );
     va_end(args);
    return ret;
}
#endif
#else
inline RCINT RCSPRINTF( RCCHAR * buffer, const TCHAR * format , ... )
{
    va_list args;
    va_start(args, format);
    RCINT ret = _vstprintf( buffer, format , args );
    va_end(args);
    return ret;
}
#endif

#ifdef _USING_QTCHAR
// uses wchar_t version of vfwprintf on the Mac
RCINT RCfprintf( FILE *stream, const RCCHAR *format , ...);

inline RCINT RCfprintf( FILE *stream, const TCHAR *format , ...)
{
    va_list args;
    va_start(args, format);
#ifdef UNICODE
    RCINT ret = vfwprintf( stream, format , args );
#else
    RCINT ret = vfprintf( stream, format , args );
#endif
    va_end(args);
    return ret;
}

inline RCINT RCfprintf( FILE *stream, const char *format , ...)
{
    va_list args;
    va_start(args, format);
    //vRCINT ret = fprintf( stream, format , args );
    RCINT ret = vfprintf( stream, format , args );
    va_end(args);
    return ret;
}
#endif

#ifdef _USING_QTCHAR
// this is implemented in qdefs.ccp
extern RCCHAR * RCSTRTOK(register RCCHAR *s, register const RCCHAR *delim);

//char *strtok( char *strToken, const char *strDelimit );

inline TCHAR * RCSTRTOK( TCHAR * strToken, const TCHAR * strDelmit )
{
#ifdef UNICODE
#ifdef _MAC
    TCHAR *cptr;
    return wcstok( strToken, strDelmit, &cptr );
#else
     return wcstok( strToken, strDelmit );
#endif
#else
    //return _tcstok( strToken, strDelmit );
    return strtok( strToken, strDelmit );
#endif
}

#ifdef UNICODE
inline char * RCSTRTOK( char * strToken, const char * strDelmit )
{
    return strtok ( strToken, strDelmit );
}
#endif
#else
inline TCHAR * RCSTRTOK( TCHAR * strToken, const TCHAR * strDelmit )
{
    return _tcstok( strToken, strDelmit );
}
#endif

#ifdef _USING_QTCHAR
#ifdef UNICODE
inline RCINT RCGETPRIVATEPROFILESTRING( LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpDefault, LPWSTR lpReturnedString ,DWORD nSize , LPCWSTR lpFileName)
{
#ifdef _MAC
    return 0;
#else
    return GetPrivateProfileString(  lpAppName,   lpKeyName,   lpDefault,   lpReturnedString ,  nSize ,   lpFileName);
#endif
}
#endif
extern RCINT RCGETPRIVATEPROFILESTRING( QString App, QString Key, QString Default, QString &ReturnedString ,DWORD nSize , QString FileName);

inline RCINT RCGETPRIVATEPROFILESTRING( const RCCHAR * lpAppName, const RCCHAR *lpKeyName, const RCCHAR * lpDefault, RCCHAR * lpReturnedString ,DWORD nSize , const RCCHAR * lpFileName)
{
    // calls the QSting version
    QString qFileName(lpFileName),qAppName(lpAppName),qKeyName(lpKeyName),qDefault(lpDefault),qReturnString,qkey;

    int count = RCGETPRIVATEPROFILESTRING(qAppName,qKeyName,qDefault,qReturnString,nSize,qFileName);
    QChar *pChr = lpReturnedString;
    for(int i=0; i<count; i++)
    {
        *pChr = qReturnString[i];
        pChr++;
    }
    *pChr=0;
    return count;
    //return GetPrivateProfileStringA(  (LPCSTR)lpAppName, (LPCSTR)lpKeyName, (LPCSTR)lpDefault, (LPSTR)lpReturnedString ,  nSize , (LPCSTR)lpFileName);
}

inline RCINT RCGETPRIVATEPROFILESTRING( const RCCHAR * lpAppName, const RCCHAR *lpKeyName, const RCCHAR * lpDefault, char * lpReturnedString ,DWORD nSize , const RCCHAR * lpFileName)
{
    QString qFileName(lpFileName),qAppName(lpAppName),qKeyName(lpKeyName),qDefault(lpDefault), qReturnString,qkey;

    int count = RCGETPRIVATEPROFILESTRING( qAppName, qKeyName, qDefault, qReturnString ,  nSize , qFileName);
    strcpy(lpReturnedString,qReturnString.toStdString().c_str());
    return count;
}

inline RCINT RCGETPRIVATEPROFILESTRING( const char * lpAppName, const char *lpKeyName, const char * lpDefault, char * lpReturnedString ,DWORD nSize , const RCCHAR * lpFileName)
{
    QString qFileName(lpFileName),qAppName(lpAppName),qKeyName(lpKeyName),qDefault(lpDefault), qReturnString,qkey;

    int count = RCGETPRIVATEPROFILESTRING( qAppName, qKeyName, qDefault, qReturnString ,  nSize , qFileName);
    strcpy(lpReturnedString,qReturnString.toStdString().c_str());
    return count;
}

inline RCINT RCGETPRIVATEPROFILESTRING( const char * lpAppName, const char *lpKeyName, const char * lpDefault, RCCHAR * lpReturnedString ,DWORD nSize , const RCCHAR * lpFileName)
{
    QString qFileName(lpFileName),qAppName(lpAppName),qKeyName(lpKeyName),qDefault(lpDefault), qReturnString,qkey;

    int count = RCGETPRIVATEPROFILESTRING( qAppName, qKeyName, qDefault, qReturnString ,  nSize , qFileName);
    RCSTRCPY(lpReturnedString,qReturnString.constData());
    return count;
}

inline RCINT RCGETPRIVATEPROFILESTRING( const char * lpAppName, const char *lpKeyName, const char * lpDefault, char * lpReturnedString ,DWORD nSize , const char * lpFileName)
{
    QString qFileName(lpFileName),qAppName(lpAppName),qKeyName(lpKeyName),qDefault(lpDefault), qReturnString,qkey;

    int count = RCGETPRIVATEPROFILESTRING( qAppName, qKeyName, qDefault, qReturnString ,  nSize , qFileName);
    //strcpy(lpReturnedString,qReturnString.toStdString().c_str());
    for(int i=0; i<count; i++)
        lpReturnedString[i] = qReturnString.at(i).toLatin1();
    return count;
}
#else
inline DWORD RCGETPRIVATEPROFILESTRING( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString ,DWORD nSize , LPCTSTR lpFileName)
{

    return GetPrivateProfileString(  lpAppName,   lpKeyName,   lpDefault,   lpReturnedString ,  nSize ,   lpFileName);
}
#endif

#ifdef _USING_QTCHAR
#ifdef UNICODE
inline RCBOOL RCWRITEPRIVATEPROFILESTRING( LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpString, LPCWSTR lpFileName )
{
#ifdef _MAC
    return 0;
#else
    return WritePrivateProfileString(  lpAppName,  lpKeyName, lpString, lpFileName );
#endif
}
extern RCBOOL RCWRITEPRIVATEPROFILESTRING( const RCCHAR *  lpAppName, const RCCHAR *  lpKeyName, const RCCHAR *  lpString, const RCCHAR *  lpFileName );
#endif

inline RCBOOL RCWRITEPRIVATEPROFILESTRING( const RCCHAR *  lpAppName, const RCCHAR *  lpKeyName, const char *  lpString, const RCCHAR *  lpFileName )
{
    return RCWRITEPRIVATEPROFILESTRING( QString(lpAppName).data(), QString(lpKeyName).data(), QString(lpString).data(), lpFileName );
}

inline RCBOOL RCWRITEPRIVATEPROFILESTRING( const char *  lpAppName, const char *  lpKeyName, const char *  lpString, const RCCHAR *  lpFileName )
{
    return RCWRITEPRIVATEPROFILESTRING( QString(lpAppName).data(), QString(lpKeyName).data(), QString(lpString).data(), lpFileName );
}

inline RCBOOL RCWRITEPRIVATEPROFILESTRING( const char *  lpAppName, const char *  lpKeyName, const char *  lpString, const char *  lpFileName )
{
#ifdef _MAC
    return RCWRITEPRIVATEPROFILESTRING( QString(lpAppName).data(), QString(lpKeyName).data(), QString(lpString).data(), QString(lpFileName).data() );
#else
    return RCWRITEPRIVATEPROFILESTRING( lpAppName, lpKeyName, lpString, QString(lpFileName).data() );
#endif
}
#endif

#ifdef _USING_QTCHAR
#ifdef UNICODE
inline RCINT RCGETPRIVATEPROFILEINT( LPCTSTR lpAppName, LPCWSTR lpKeyName, INT nDefault, LPCWSTR lpFileName )
{
#ifdef _MAC
    return 0;
#else
    return GetPrivateProfileIntW( lpAppName, lpKeyName, nDefault, lpFileName );
#endif
}
#endif
inline RCINT RCGETPRIVATEPROFILEINT( QString AppName, QString KeyName, RCINT nDefault, QString FileName )
{
    QSettings settings(FileName,QSettings::IniFormat);
    QString qkey;

    if( AppName.length() != 0 && KeyName.length() != 0)
    {
        qkey = QString((AppName)) + "/" + QString(KeyName);
        return settings.value(qkey,nDefault).toInt();
    }
    return nDefault;
}

inline RCINT RCGETPRIVATEPROFILEINT( const RCCHAR * lpAppName, const RCCHAR *lpKeyName, RCINT nDefault, const RCCHAR * lpFileName )
{
    return RCGETPRIVATEPROFILEINT(QString(lpAppName),QString(lpKeyName),nDefault,QString(lpFileName));
}

inline RCINT RCGETPRIVATEPROFILEINT( const char * lpAppName, const char *lpKeyName, RCINT nDefault, const RCCHAR * lpFileName )
{
    return RCGETPRIVATEPROFILEINT(QString(lpAppName),QString(lpKeyName),nDefault,QString(lpFileName));
}

inline RCINT RCGETPRIVATEPROFILEINT( const char * lpAppName, const char *lpKeyName, RCINT nDefault, const char * lpFileName )
{
    return RCGETPRIVATEPROFILEINT(QString(lpAppName),QString(lpKeyName),nDefault,QString(lpFileName));
}

#endif

#ifdef _USING_QTCHAR
#ifdef UNICODE
inline RCINT RCGETPROFILESTRING( LPCTSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpDefault, LPWSTR lpReturnedString, DWORD nSize )
{
#ifdef _MAC
    return 0;
#else
    return GetProfileStringW(  lpAppName,  lpKeyName,  lpDefault,  lpReturnedString,  nSize );
#endif
}
#endif
inline RCINT RCGETPROFILESTRING( const RCCHAR * lpAppName, const RCCHAR *lpKeyName, const RCCHAR *lpDefault, RCCHAR *lpReturnedString, DWORD nSize )
{
    QSettings settings(QCoreApplication::organizationName(),"RealCAD");
    QString value = settings.value(QString(lpAppName)+"/"+QString(lpKeyName),QString(lpDefault)).toString();
    RCSTRCPY(lpReturnedString,value.data());
    return RCSTRLEN(lpReturnedString);
    //return GetProfileStringA(  (LPCSTR)lpAppName,  (LPCSTR)lpKeyName,  (LPCSTR)lpDefault,  (LPSTR)lpReturnedString,  nSize );
}

inline RCINT RCGETPROFILESTRING( const char * lpAppName, const char *lpKeyName, const char *lpDefault, RCCHAR *lpReturnedString, DWORD nSize )
{
    QSettings settings(QCoreApplication::organizationName(),"RealCAD");
    QString value = settings.value(QString(lpAppName)+"/"+QString(lpKeyName),QString(lpDefault)).toString();
    RCSTRCPY(lpReturnedString,value.data());
    return RCSTRLEN(lpReturnedString);
    //GetProfileStringA(  lpAppName,  lpKeyName,  lpDefault,  (LPSTR)lpReturnedString,  nSize );
}

inline RCINT RCGETPROFILESTRING( const char * lpAppName, const char *lpKeyName, const char *lpDefault, char *lpReturnedString, DWORD nSize )
{

    QSettings settings(QCoreApplication::organizationName(),"RealCAD");
    QString value = settings.value(QString(lpAppName)+"/"+QString(lpKeyName),QString(lpDefault)).toString();
    strcpy(lpReturnedString,value.toLatin1().data());
    return strlen(lpReturnedString);
    //GetProfileStringA(  lpAppName,  lpKeyName,  lpDefault,  lpReturnedString,  nSize );
}
#endif

#ifdef _USING_QTCHAR
/*
  __in  LPCTSTR lpAppName,
  __in  LPCTSTR lpKeyName,
  __in  LPCTSTR lpString

*/
inline RCUINT RCWRITEPROFILESTRING( RCLPCWSTR lpAppName, RCLPCWSTR lpKeyName, RCLPCWSTR lpString )
{
    QSettings settings(QCoreApplication::organizationName(),"RealCAD");
    settings.setValue(QString(lpAppName)+"/"+QString(lpKeyName),QString(lpString));
    return 1;//WriteProfileStringW( (LPCWSTR)lpAppName, (LPCWSTR)lpKeyName, (LPCWSTR)lpString );
}

inline RCUINT RCWRITEPROFILESTRING( RCLPCWSTR lpAppName, RCLPCWSTR lpKeyName, LPCWSTR lpString )
{
    return 0;//WriteProfileStringW( (LPCWSTR)lpAppName, (LPCWSTR)lpKeyName, lpString );
}

inline RCUINT RCWRITEPROFILESTRING( const char * lpAppName, const char *lpKeyName, const char *lpString )
{
    QSettings settings(QCoreApplication::organizationName(),"RealCAD");
    settings.setValue(QString(lpAppName)+"/"+QString(lpKeyName),QString(lpString));
    return 1;//WriteProfileStringA( lpAppName, lpKeyName, lpString );
}
#endif

#ifdef _USING_QTCHAR
extern RCUINT RCGETWINDOWSDIRECTORY( RCPCHAR lpBuffer, RCUINT uSize );
extern RCUINT RCGETWINDOWSDIRECTORY( LPWSTR lpBuffer, RCUINT uSize );
extern RCUINT RCGETWINDOWSDIRECTORY( char * lpBuffer, UINT uSize );
#endif


#ifdef _USING_QTCHAR
#ifdef UNICODE
inline RCBOOL RCGETVOLUMEINFORMATION(
    LPCWSTR lpRootPathName,
    LPWSTR lpVolumeNameBuffer,
    DWORD nVolumeNameSize,
    LPDWORD lpVolumeSerialNumber,
    LPDWORD lpMaximumComponentLength,
    LPDWORD lpFileSystemFlags,
    LPTSTR lpFileSystemNameBuffer,
    DWORD nFileSystemNameSize)
{
#ifdef _MAC
    return 0;
#else
    return GetVolumeInformationW(
        lpRootPathName,
        lpVolumeNameBuffer,
        nVolumeNameSize,
        lpVolumeSerialNumber,
        lpMaximumComponentLength,
        lpFileSystemFlags,
        lpFileSystemNameBuffer,
        nFileSystemNameSize);
#endif
}
#endif
typedef DWORD *LPDWORD;
#ifdef _MAC
BOOL WINAPI GetVolumeInformation(
   LPCTSTR lpRootPathName,
   LPTSTR lpVolumeNameBuffer,
   DWORD nVolumeNameSize,
   LPDWORD lpVolumeSerialNumber,
   LPDWORD lpMaximumComponentLength,
   LPDWORD lpFileSystemFlags,
   LPTSTR lpFileSystemNameBuffer,
   DWORD nFileSystemNameSize
);
#define GetVolumeInformationA GetVolumeInformation
#endif
inline RCBOOL RCGETVOLUMEINFORMATION(
    const char * lpRootPathName,
    char * lpVolumeNameBuffer,
    DWORD nVolumeNameSize,
    LPDWORD lpVolumeSerialNumber,
    LPDWORD lpMaximumComponentLength,
    LPDWORD lpFileSystemFlags,
    char * lpFileSystemNameBuffer,
    DWORD nFileSystemNameSize)
{
#ifndef _MAC
    return GetVolumeInformationA(
        lpRootPathName,
        lpVolumeNameBuffer,
        nVolumeNameSize,
        lpVolumeSerialNumber,
        lpMaximumComponentLength,
        lpFileSystemFlags,
        lpFileSystemNameBuffer,
        nFileSystemNameSize);
#else
  static DWORD serial=0;
  uuid_t id;
  struct timespec wait;
  wait.tv_nsec=0;
  wait.tv_sec=0;
  int ret;

  if((ret = gethostuuid(id,&wait)) == 0)
  {
      qDebug() << "got an id";
      serial=0;
      for(int i=0; i<sizeof(uuid_t); i++)
        serial += id[i];

      *lpVolumeSerialNumber = serial;

      return 1;
  }
  else
      qDebug() << "did no get an id";
  return 0;
#endif
}
#endif

#ifdef _USING_QTCHAR
#ifdef UNICODE
inline RCLONG RCREGQUERYVALUEEX(
    HKEY hKey,
    LPCWSTR lpValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE lpData,
    LPDWORD lpcbData)
{
    return 0;
    /*RegQueryValueExW(
        hKey,
        lpValueName,
        lpReserved,
        lpType,
        lpData,
        lpcbData);*/
}
#endif

inline RCLONG RCREGQUERYVALUEEX(
    HKEY hKey,
    const char * lpValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE lpData,
    LPDWORD lpcbData)
{
    return 0;
    /*RegQueryValueExA(
        hKey,
        lpValueName,
        lpReserved,
        lpType,
        lpData,
        lpcbData);*/
}

inline RCLONG RCREGQUERYVALUEEX(
    HKEY hKey,
    LPDWORD lpValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE lpData,
    LPDWORD lpcbData)
{
    return 0;
    /*RegQueryValueExA(
        hKey,
        lpValueName,
        lpReserved,
        lpType,
        lpData,
        lpcbData);*/
}


#endif


#ifdef _USING_QTCHAR
inline RCLONG RCREGQUERYVALUE( RCHKEY hKey, RCLPCSTR lpSubKey, RCLPSTR lpValue, RCPLONG lpcbValue)
{
    return 0;//RegQueryValue( hKey, (LPCSTR)lpSubKey, (LPSTR)lpValue, lpcbValue);
}

inline RCLONG RCREGQUERYVALUE( RCHKEY hKey, LPCWSTR lpSubKey, RCLPWSTR lpValue, RCPLONG lpcbValue)
{
    return 0;//RegQueryValueW( hKey, (LPCWSTR)lpSubKey, lpValue, lpcbValue);
}

inline RCLONG RCREGQUERYVALUE( HKEY hKey, const char * lpSubKey, char * lpValue, PLONG lpcbValue)
{
    return 0;//RegQueryValueA( hKey, lpSubKey, lpValue, lpcbValue);
}

#endif

#ifdef _USING_QTCHAR
inline RCINT RCMessagebox( RCHWND hWnd, const RCCHAR* lpText, const RCCHAR* lpCaption, RCUINT uType )
{
    int ret = 0;
    if(hWnd)
    {
        switch(uType)
        {
        case MB_YESNOCANCEL :
            //qDebug() << "in yesno cancel messagebox";
            ret = QMessageBox::question((QWidget*)hWnd, QString(lpCaption), QString(lpText),
                                        QMessageBox::Yes |
                                        QMessageBox::No |
                                        QMessageBox::Cancel );
            break;
        case MB_YESNO :
            //qDebug() << "in OK cancel messagebox";
            ret = QMessageBox::question((QWidget*)hWnd, QString(lpCaption), QString(lpText),
                                        QMessageBox::Yes |
                                        QMessageBox::No);
            break;
        case MB_OK :
        default :
            //qDebug() << "in OK cancel messagebox";
            ret = QMessageBox::question((QWidget*)hWnd, QString(lpCaption), QString(lpText),
                                        QMessageBox::Ok);
            break;
        }

        // convert to windows return values
        switch(ret)
        {
        case QMessageBox::Ok :
            return IDOK;
        case QMessageBox::Yes :
            return IDYES;
        case QMessageBox::No :
            return IDNO;
        case QMessageBox::Cancel :
        case QMessageBox::Escape :
            return IDCANCEL;
        }
    }
    return 0;
    //MessageBoxW( (HWND)hWnd, (LPCWSTR)lpText, (LPCWSTR)lpCaption, uType );
}

inline RCINT RCMessageBox( RCHWND hWnd, RCLPCWSTR lpText, RCLPCWSTR lpCaption, RCUINT uType )
{
    return RCMessagebox( hWnd, lpText, lpCaption, uType );
}

inline RCINT RCMessageBox( RCHWND hWnd, const char *  lpText, const char *  lpCaption, RCUINT uType )
{
    return RCMessageBox(hWnd, QString(lpText).data(),QString(lpCaption).data(),uType);
    //return MessageBoxW( (HWND)hWnd, (LPCWSTR)lpText, (LPCWSTR)lpCaption, uType );
}

inline RCINT RCMessageboxW( RCHWND hWnd, const RCCHAR* lpText, const RCCHAR* lpCaption, UINT uType )
{
    return 0;//MessageBoxW( (HWND)hWnd, (LPCWSTR)lpText, (LPCWSTR)lpCaption, uType );
}

inline RCINT RCMessageBoxW( RCHWND hWnd, const char* lpText, const char* lpCaption, RCUINT uType )
{
    return 0;//MessageBoxW( (HWND)hWnd, (LPCWSTR)lpText, (LPCWSTR)lpCaption, uType );
}
/*
inline RCINT RCMessageBox( HWND hWnd, const char * lpText, const char * lpCaption, UINT uType )
{
    return 0;//MessageBoxA( hWnd, lpText, lpCaption, uType );
}
*/
#endif

#ifdef _USING_QTCHAR
inline RCLONG RCREGOPENKEYEX( HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult )
{
    return 0;//RegOpenKeyExW( hKey, lpSubKey, ulOptions, samDesired, phkResult );
}

inline RCLONG RCREGOPENKEYEX( HKEY hKey, const char * lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult )
{
    return 0;//RegOpenKeyExA( hKey, lpSubKey, ulOptions, samDesired, phkResult );
}

inline RCLONG   RCREOPENKEYEX( RCHKEY hKey, RCLPCTSTR lpSubKey, RCDWORD ulOptions, RCREGSAM samDesired,  RCPHKEY phkResult  )
{
    return   0;//RegOpenKeyEx( hKey, lpSubKey, ulOptions, samDesired, phkResult );
}

#endif
#ifdef _USING_QT
inline RCCHAR * RCGETCOMMANDLINE(void)
{
#ifdef _MAC
    return 0;//GetCommandLine(void);
#else
    return 0;//GetCommandLine();
#endif
}

#endif

#ifdef _USING_QT
#ifdef __cplusplus
//char *strrchr( char *str, int c ); // C++ only
//const char *strrchr( const char *str, int c ); // C++ only
inline RCCHAR *RCSTRRCHR( RCCHAR *str, RCINT c )
{
    int i=0;
    // find the end of the input string
    RCCHAR *chr=str,cc(c);
    int cnt=0;
    for(chr=str; *chr != 0; chr++)
        cnt++;
    // now go back to find the char
    for( i=cnt; i>0; chr--,i--)
    {
        if(*chr == cc )
            break;
    }
    return chr;
    //return 0;//(RCCHAR*)_tcsrchr( (TCHAR*)str, (int)c );
}

inline RCCHAR *RCSTRRCHR( RCCHAR *str, RCCHAR *c )
{
    int i=0;
    // find the end of the input string
    RCCHAR *chr=str,*cc=c;
    int cnt=0;
    for(chr=str; *chr != 0; chr++)
        cnt++;
    // now go back to find the char
    for( i=cnt; i>0; chr--,i--)
    {
        if(*chr == *cc )
            break;
    }
    return chr;
    //return 0;//(RCCHAR*)_tcsrchr( (TCHAR*)str, (int)c );
}

inline char *RCSTRRCHR( TCHAR *str, int c )
{
    return 0;//_tcsrchr( str, c );
}

inline const RCCHAR *RCSTRRCHR( const RCCHAR *str, RCINT c )
{
    return RCSTRRCHR( str, c );
    //RCCHAR *)_tcsrchr( (TCHAR*)str, (int)c );
}

inline const char *RCSTRRCHR( const char *str, char c )
{
    return strrchr( str, c );
}

inline char *RCSTRRCHR( char *str, char c )
{
    return strrchr( str, c );
}

inline const char *RCSTRRCHR( const TCHAR *str, int c )
{
    return 0;//_tcsrchr( str, c );
}
#else
//char *strrchr( const char *str, int c ); // C only
char *RCSTRRCHR( const TCHAR *str, int c )
{
    return _tcsrchr( str, c );
}

#endif
#else
#ifdef __cplusplus
inline char *RCSTRRCHR( TCHAR *str, int c )
{
    return _tcsrchr( str, c );
}

inline const char *RCSTRRCHR( const TCHAR *str, int c )
{
    return _tcsrchr( str, c );
}
#else
inline char *RCSTRRCHR( const TCHAR *str, int c )
{
    return _tcsrchr( str, c );
}
#endif
#endif

#ifdef _USING_QT
//int _mbsnbicmp( const unsigned char *string1, const unsigned char *string2, size_t count );
//int _strnicmp( const unsigned char *string1, const unsigned char *string2, size_t count );
#if defined(_USING_STD)
    //std::compare (size_t pos, size_t len, const string& str,
                    size_t subpos, size_t sublen) const;
#endif
inline RCINT RCSTRNICMP(
            RCCHAR *string1,
            RCCHAR *string2,
            RCSIZE_T count
            )
{
#if defined(_USING_QTCHAR)
    QString qstring1(string1),qstring2(string2);
    return qstring1.compare(qstring2,Qt::CaseInsensitive);
    //return strncmp ( (TCHAR*)string1, (TCHAR*)string2, count );
#else
    // other char types
    return strncmp ( string1, string2, count );
#endif
    //return 0;//_tcsnicmp( (TCHAR*)string1, (TCHAR*)string2, count );
}

inline RCINT RCSTRNICMP(
            const unsigned char *string1,
            const unsigned char *string2,
            RCSIZE_T count
            )
{
    return 0;//_tcsnicmp( (TCHAR*)string1, (TCHAR*)string2, count );
}

inline RCINT RCSTRNICMP(
            const char *string1,
            const char *string2,
            RCSIZE_T count
            )
{
    return qstrnicmp ( string1, string2, count );
    //_tcsnicmp( (TCHAR*)string1, (TCHAR*)string2, count );
}
#endif

#ifdef _USING_QT
//char *_getcwd( char *buffer, int maxlen );
inline RCCHAR *RCGETCWD( RCCHAR *buffer, RCINT maxlen )
{
    QString cwd = QDir::currentPath();
    if(cwd.isEmpty() || cwd == QCoreApplication::applicationDirPath())
        cwd = QDir::homePath();
    return RCSTRNCPY(buffer,cwd.constData(),maxlen);
    //return (RCCHAR*)_tgetcwd( (TCHAR*)buffer, (int)maxlen );
}

inline TCHAR *RCGETCWD( TCHAR *buffer, int maxlen )
{
    return 0;//_tgetcwd( buffer, maxlen );
}
#else
inline TCHAR *RCGETCWD( TCHAR *buffer, int maxlen )
{
    return _tgetcwd( buffer, maxlen );
}
#endif

#ifdef _USING_QT
//int _mkdir( const char *dirname );
inline RCINT RCMKDIR( const RCCHAR *dirname )
{
    QDir mdir;
    return (RCINT)mdir.mkdir(QString(dirname));
    //return (RCINT)_tmkdir( (char*)dirname );
}

inline int RCMKDIR( const char *dirname )
{
#ifdef _MAC
    return mkdir(dirname, 755);
#else
    return _mkdir( dirname );
#endif
}
#endif

#ifdef _USING_QT
//char *_mktemp( char *template );
#ifdef _MAC
inline char * RCMKTEMP( char *_template )
{
    return mktemp( _template );
}
#endif
inline RCCHAR * RCMKTEMP( RCCHAR *_template )
{
#ifdef _MAC
    mktemp(QString(_template).toLocal8Bit().data());
#else
    return (RCCHAR*)_wmktemp( (TCHAR*)_template );
#endif
}

inline TCHAR * RCMKTEMP( TCHAR *_template )
{
#ifdef _MAC
#else
    return _wmktemp( _template );
#endif
}
#endif

#ifdef _USING_QT
#ifdef __cplusplus
//char *strchr( char * str, int c ); // C++ only
//const char *strchr( const char * str, int c ); // C++ only
inline RCCHAR *RCSTRCHR( RCCHAR * str, RCCHAR c )
{
#if 0
    int index=0;
    if(c == 0)
        return str + QString(str).length();
    index=QString(str).indexOf(c);
    if(index >= 0)
        return str + index;
    else
        return (RCCHAR*)0;
#else
    RCCHAR *chr=str,cc(c);
    int cnt=0;
    for(chr=str,cnt=0; *chr != 0; chr++,cnt++)
    {
        if(*chr == cc )
            break;
    }
    return *chr != 0 ? str + cnt : (RCCHAR*)0;
#endif
}

inline RCCHAR *RCSTRCHR( RCCHAR * str, RCINT c )
{
    return RCSTRCHR(str, RCCHAR(c) );
}

inline const RCCHAR *RCSTRCHR( const RCCHAR * str, RCINT c )
{
    //const RCCHAR *tmp;
    //tmp = RCSTRCHR((RCCHAR*)str, RCCHAR(c) );
    //return tmp;
    return RCSTRCHR((RCCHAR*)str, RCCHAR(c) );
}

inline TCHAR *RCSTRCHR( TCHAR* str, int c )
{
#ifdef UNICODE
    return wcschr  ( str, c );
#else
    return _tcschr ( str, c );
#endif
}

inline const TCHAR *RCSTRCHR( const TCHAR * str, int c )
{
#ifdef UNICODE
    return wcschr  ( str, c );
#else
    return _tcschr ( str, c );
#endif
}

inline const char *RCSTRCHR( const char * str, char c )
{
    return strchr ( str, c );
}

inline char *RCSTRCHR(  char * str, char c )
{
    return strchr ( str, c );
}

#else
//char *strchr( const char *str, int c );  // C only
inline char *RCSTRCHR( const char *str, int c )
{
    return _tcschr( str, c );
}
#endif
#endif

#ifdef _USING_QT
//int _stat( const char *path, struct _stat *buffer );
inline RCINT RCSTAT( const RCCHAR *path,  RC_STAT *buffer )
{
    //return _stat( (char*)QString(path).data(), (struct _stat *)buffer );
    return 1;
}
#endif

#ifdef _USING_QT
//long strtol( const char *nptr, char **endptr, int base );
inline RCLONG RCSTRTOL( const RCCHAR *nptr, RCCHAR **endptr, RCINT base )
{
    QString dbg(nptr);
    char cnptr[300];
    RCSTRCPY(cnptr,nptr);
    char *ceptr;
    long val=0;

    if(endptr)
    {
        val = ::strtol(cnptr,&ceptr,base);
        if(*ceptr != 0)
        {
            char *cp; RCCHAR *rc = (RCCHAR*)nptr;
            for(cp=cnptr; cp!=ceptr; cp++)
                rc++;
            *endptr=(RCCHAR*)rc;
        }
        else
            *endptr=(RCCHAR*)nptr+strlen(cnptr);
        return val;
    }
    else
    {
        val =strtol(cnptr,0,base);
        return val;
    }
    //return (RCLONG) wcstol( (TCHAR*)nptr, (TCHAR**)endptr,  base );
}
inline long RCSTRTOL( const char *nptr, char **endptr, int base )
{
    return ::strtol(nptr,endptr,base);
    //_tcstol(  nptr,  endptr, base );
}
#endif

#ifdef _USING_QT
//int isdigit( int c );
inline RCINT RCISDIGIT( RCCHAR c )
{
    return c.isDigit();
}
inline RCINT RCISDIGIT( int c )
{
    return isdigit(c);
}

#endif

#ifdef _USING_QT
/*
int _strnicmp(
   const char *string1,
   const char *string2,
   size_t count
);
*/
extern RCINT RCSTRNCMP( const RCCHAR *string1, const RCCHAR *string2, RCSIZE_T count );
/*
 * // now defined in qtdefs.cpp
inline RCINT RCSTRNCMP( const RCCHAR *string1, const RCCHAR *string2, RCSIZE_T count )
{
    return 0;
    //_tcsncicmp( (const char*)string1, (const char*)string2, count  );
}
*/
//extern RCINT RCSTRNCMP( const RCCHAR *string1, const char *string2, RCSIZE_T count );
inline RCINT RCSTRNCMP( const RCCHAR *string1, const char *string2, RCSIZE_T count )
{
    // debug
    //QString dbg(string1);
    QString ctmp(string2);
    return RCSTRNCMP(string1,ctmp.data(),count);
    //_tcsncicmp( (const char*)string1,  string2, count  );
}

//extern RCINT RCSTRNCMP( const char *string1, const RCCHAR *string2, RCSIZE_T count );
inline RCINT RCSTRNCMP( const char *string1, const RCCHAR *string2, RCSIZE_T count )
{
    // debug
    //QString dbg(string2);
    QString ctmp(string1);
    return RCSTRNCMP(ctmp.data(),string2,count);
    //_tcsncicmp( string1,  (const char*)string2, count  );
}

//extern RCINT RCSTRNCMP( const char *string1, const char *string2, RCSIZE_T count );
inline RCINT RCSTRNCMP( const char *string1, const char *string2, RCSIZE_T count )
{
    return qstrncmp(string1,string2,count);
    //_tcsncicmp( (const char*)string1,  string2, count  );
}
#endif

#ifdef _USING_QT
//int _unlink( const char *filename );
inline RCINT RCUNLINK( const RCCHAR *filename )
{
    return (RCINT) QFile::remove(QString(filename));
    //return 0;//_unlink( (char*)filename );
}
#endif

#ifdef _USING_QT
//FILE *fopen( const char *filename,const char *mode );
inline FILE *fopen( const RCCHAR *filename,const RCCHAR *mode )
{
    // KMJ TODO: use Qt streams
    char cfilename[600],cmode[10];
    strcpy(cfilename,QString(filename).toStdString().c_str());
    strcpy(cmode,QString(mode).toStdString().c_str());
    return fopen( cfilename, cmode );
}
inline FILE *fopen( const RCCHAR *filename,const char *mode )
{
    char cfilename[600];
    strcpy(cfilename,QString(filename).toStdString().c_str());
    return fopen( cfilename, mode );
}
#endif

#ifdef _USING_QT
//char *strstr( char *str, const char *strSearch ); // C++ only
inline int strstr( QString str, const QString & strSearch )
{
    if( strSearch.size() == 0 )
        return 0;
    int index = str.indexOf(strSearch);
    return index < 0 ? 0 : index;
}

inline RCCHAR *strstr( RCCHAR *str, const RCCHAR *strSearch )
{
    QString qstrSearch(strSearch);
    // return the search string if strSearch is empty
    if( qstrSearch.size() == 0 )
        return str;
    QString qstr((str));
    int index = qstr.indexOf(qstrSearch);
    return index < 0 ? 0 : (RCCHAR*)&str[index];
}

inline RCCHAR *strstr( RCCHAR *str, const char *strSearch )
{
    if( strSearch == 0)
        return str;
    QString qstrSearch(strSearch);
    return strstr(  str, qstrSearch.constData() );
}
#endif

#ifdef _USING_QT
//char *strncat( char *strDest, const char *strSource, size_t count );
inline RCCHAR *strncat( RCCHAR *strDest, const RCCHAR *strSource, RCSIZE_T count )
{
    return strncat( strDest, strSource, count );
}
#endif

#ifdef _USING_QT
//char *fgets(  char *str, int n, FILE *stream  );
inline RCCHAR *fgets(  RCCHAR *str, RCINT n, FILE *stream  )
{
    return 0;//fgets(  str, n,  stream  );
}

inline char *fgets(  char *str, RCINT n, char *stream  )
{
    return 0;//fgets(  str, n,  stream  );
}

#endif


#ifdef _USING_QT
//int _chdir(  const char *dirname  );
inline RCINT RCCHDIR(  const RCCHAR *dirname  )
{
    if(dirname)
        return QDir::setCurrent(QString(dirname));
    return 0;//_chdir(  (char*)dirname  );
}
#endif

#ifdef _USING_QT
/*
errno_t fopen_s(
   FILE** pFile,
   const char *filename,
   const char *mode
);
*/
inline RCERRNO_T RCFOPEN_S(
   FILE** pFile,
   const RCCHAR *filename,
   const char *mode
)
{
    //return fopen(pFile, QString(filename).data(),mode);
    return 0;
    /*
    return  fopen_s(
       pFile,
       (char*)filename,
        mode
    );
  */
}
#endif

#ifdef _USING_QT
/*
char *strpbrk(
   char *str,
   const char *strCharSet
); // C++ only
*/
inline RCCHAR *RCSTRPBRK( RCCHAR *str, const RCCHAR *strCharSet )
{
    return (RCCHAR*)strpbrk( (char*)str, (const char *)strCharSet );
}

/*
const char *strpbrk(
   const char *str,
   const char *strCharSet
); // C++ only
*/
inline const RCCHAR *RCSTRPBRK( const RCCHAR *str, const RCCHAR *strCharSet )
{
    return (RCCHAR*)strpbrk( (const char *)str, (const char *)strCharSet );
}

inline const RCCHAR *RCSTRPBRK( const RCCHAR *str, const char *strCharSet )
{
    return (RCCHAR*)strpbrk( (const char *)str, strCharSet );
}

inline const RCCHAR *RCSTRPBRK( const RCCHAR *str, const char *strCharSet, const char *strNew  )
{
    return 0;
}

inline const RCCHAR *RCSTRPBRK( const char *str, const char *strCharSet )
{
    return (RCCHAR*)strpbrk(  str, strCharSet );
}

#endif

//int fputc( int c, FILE *stream );
inline RCINT RCfputc( RCCHAR c, FILE *stream )
{
    return 0;//fputc( c, stream );
}

inline RCINT RCfputc( char c, FILE *stream )
{
    return fputc( c, stream );
}

RCBOOL RCGETCLIENTRECT( RCHWND hWnd, RCLPRECT lpRect);

extern RCHWND RCCREATEWINDOW(  RCPCHAR lpClassName,
                               RCPCHAR lpWindowName,
                               RCDWORD style,
                               RCINT x,
                               RCINT y,
                               RCINT w,
                               RCINT h,
                               RCHWND hParent,
                               RCHMENU hMenu,
                               RCHINSTANCE hInstance,
                               RCPVOID lpParam);

extern RCHWND RCCREATEWINDOWEX( RCDWORD dwExStyle,
                                RCPCHAR lpClassName,
                                RCPCHAR lpWindowName,
                                RCDWORD style,
                                RCINT x,
                                RCINT y,
                                RCINT w,
                                RCINT h,
                                RCHWND hParent,
                                RCHMENU hMenu,
                                RCHINSTANCE hInstance,
                                RCPVOID lpParam);

extern RCBOOL RCMOVEWINDOW( RCHWND hWnd,
                            RCINT X,
                            RCINT Y,
                            RCINT nWidth,
                            RCINT nHeight,
                            RCBOOL bRepaint);

extern RCBOOL RCSHOWWINDOW( RCHWND hWnd, RCINT show );

extern RCBOOL RCBRINGWINDOWTOTOP( RCHWND hWnd );

extern RCBOOL RCINVALIDATERECT( RCHWND hWnd, const RECT* lpRect, RCBOOL bErase );

extern RCBOOL RCDESTROYWINDOW( RCHWND Hwnd );

extern RCBOOL RCBITBLT( RCHDC hdcDest, // handle to destination DC
                        RCINT nXDest,  // x-coord of destination upper-left corner
                        RCINT nYDest,  // y-coord of destination upper-left corner
                        RCINT nWidth,  // width of destination rectangle
                        RCINT nHeight, // height of destination rectangle
                        RCHDC hdcSrc,  // handle to source DC
                        RCINT nXSrc,   // x-coordinate of source upper-left corner
                        RCINT nYSrc,   // y-coordinate of source upper-left corner
                        RCDWORD dwRop  // raster operation code);
                        );

extern RCHGDIOBJ RCSELECTOBJECT( RCHDC hdc, RCHGDIOBJ hPen);

extern RCINT RCSETROP2( RCHDC hdc,         // handle to DC
                 RCINT fnDrawMode   // drawing mode
                 );

extern RCBOOL RCMOVETOEX( RCHDC hDC ,RCINT xp, RCINT yp, RCLPPOINT plPoint);

extern RCBOOL RCLINETO( RCHDC hdc,    // device context handle
                 RCINT nXEnd,  // x-coordinate of ending point
                 RCINT nYEnd   // y-coordinate of ending point
                 );

extern RCINT RCESCAPE( RCHDC hdc,           // handle to DC
                RCINT nEscape,       // escape function
                RCINT cbInput,       // size of input structure
                RCLPCSTR lpvInData,  // input structure
                RCPVOID lpvOutData  // output structure
                );

extern RCUINT RCGETSYSTEMPALETTEENTRIES(  RCHDC hdc,              // handle to DC
                                 RCUINT iStartIndex,     // first entry to be retrieved
                                 RCUINT nEntries,        // number of entries to be retrieved
                                 LPPALETTEENTRY lppe     // array that receives entries
                                 );

extern RCINT RCGETDEVICECAPS( RCHDC hdc,     // handle to DC
                       RCINT nIndex   // index of capability
                       );

extern RCCOLORREF RCSETTEXTCOLOR( RCHDC hdc,           // handle to DC
                           RCCOLORREF crColor   // text color
                           );

extern RCINT RCSETBKMODE( RCHDC hdc,      // handle to DC
                   RCINT iBkMode   // background mode
                   );

extern RCCOLORREF RCSETBKCOLOR( RCHDC hdc,           // handle to DC
                         RCCOLORREF crColor   // background color value
                         );

extern RCUINT RCGETOUTLINETEXTMETRICS( RCHDC hdc,                    // handle to DC
                                       RCUINT cbData,                // size of metric data array
                                       RCLPOUTLINETEXTMETRIC lpOTM   // metric data array
                                       );

extern RCBOOL RCGETTEXTEXTENTPOINT32( RCHDC hdc,           // handle to DC
                                      RCLPCTSTR lpString,  // text string
                                      RCINT c,      // characters in string
                                      RCLPSIZE lpSize      // string size
                                      );

extern RCBOOL RCGETTEXTEXTENTPOINT32W( RCHDC hdc,           // handle to DC
                                       RCLPCWSTR lpString,  // text string
                                       RCINT c,      // characters in string
                                       RCLPSIZE lpSize      // string size
                                       );

extern RCBOOL RCGETTEXTEXTENTPOINT( RCHDC hdc,           // handle to DC
                                    RCLPCTSTR lpString,  // text string
                                    RCINT cbString,      // number of characters in string
                                    RCLPSIZE lpSize      // string size
                                    );

extern RCBOOL RCGETTEXTEXTENT( RCHDC hdc,           // handle to DC
                               const RCCHAR* lpString,  // text string
                               RCINT cbString      // number of characters in string
                                );

extern RCBOOL RCTEXTOUTW( RCHDC hdc,           // handle to DC
                  RCINT nXStart,       // x-coordinate of starting position
                  RCINT nYStart,       // y-coordinate of starting position
                  RCLPCTSTR lpString,  // character string
                  RCINT cbString       // number of characters
                  );

extern RCBOOL RCTEXTOUT( RCHDC hdc,           // handle to DC
                  RCINT nXStart,       // x-coordinate of starting position
                  RCINT nYStart,       // y-coordinate of starting position
                  RCLPCTSTR lpString,  // character string
                  RCINT cbString       // number of characters
                  );

extern RCINT RCSETPOLYFILLMODE( RCHDC hdc,            // handle to device context
                         RCINT iPolyFillMode   // polygon fill mode
                         );

extern RCBOOL RCPOLYPOLYGON( RCHDC hdc,                  // handle to DC
                      const RCPOINT *lpPoints,    // array of vertices
                      const RCINT *lpPolyCounts,  // array of count of vertices
                      RCINT nCount                // count of polygons
                      );

extern RCCOLORREF RCSETPIXEL( RCHDC hdc,           // handle to DC
                       RCINT X,             // x-coordinate of pixel
                       RCINT Y,             // y-coordinate of pixel
                       RCCOLORREF crColor   // pixel color
                       );

extern RCBOOL RCGETWINDOWRECT( RCHWND hWnd,
                        RCLPRECT lpRect
                        );

extern RCHWND RCGETPARENT( RCHWND hWnd );

extern RCHDC RCGETDC( RCHWND hWnd   // handle to window
              );

extern RCINT RCRELEASEDC( RCHWND hWnd,  // handle to window
                   RCHDC hDC     // handle to DC
                   );

extern RCHDC RCCREATECOMPATIBLEDC( RCHDC hdc   // handle to DC
                           );

extern RCHBITMAP RCCREATECOMPATIBLEBITMAP( RCHDC hdc,        // handle to DC
                                    RCINT nWidth,     // width of bitmap, in pixels
                                    RCINT nHeight     // height of bitmap, in pixels
                                    );

extern RCBOOL RCDELETEDC( RCHDC hdc   // handle to DC
                   );

extern RCINT RCGETDIBITS( RCHDC hdc,           // handle to DC
                   RCHBITMAP hbmp,      // handle to bitmap
                   RCUINT uStartScan,   // first scan line to set
                   RCUINT cScanLines,   // number of scan lines to copy
                   RCLPVOID lpvBits,    // array for bitmap bits
                   RCLPBITMAPINFO lpbi, // bitmap data buffer
                   RCUINT uUsage        // RGB or palette index
                   );

extern RCCOLORREF RCGETPIXEL( RCHDC hdc,    // handle to DC
                       RCINT nXPos,  // x-coordinate of pixel
                       RCINT nYPos   // y-coordinate of pixel
                       );

extern RCBOOL RCSETWINDOWTEXT( RCHWND hWnd,
                        RCLPCTSTR lpString
                        );

extern RCBOOL RCSETWINDOWTEXTW( RCHWND hWnd,
                        RCLPCTSTR lpString
                        );

extern RCBOOL RCSETMENU( RCHWND hWnd,
                  RCHMENU hMenu
                  );

extern RCHMENU RCGETMENU( RCHWND hWnd );

extern RCHMENU RCGETSUBMENU( RCHMENU hMenu, int nPos );

extern RCBOOL RCREMOVEMENU( RCHMENU hMenu,
                            RCUINT uPosition,
                            RCUINT uFlags );

extern RCBOOL RCAPPENDMENU( RCHMENU hMenu,
                            RCUINT uFlags,
                            RCUINT_PTR uIDNewItem,
                            RCLPCTSTR lpNewItem );

extern RCINT RCGETCLIPRGN( RCHDC hdc,    // handle to DC
                    RCHRGN hrgn   // handle to region
                   );

extern RCINT RCINTERSECTCLIPRGN( RCHDC hdc,         // handle to DC
                          RCINT nLeftRect,   // x-coord of upper-left corner
                          RCINT nTopRect,    // y-coord of upper-left corner
                          RCINT nRightRect,  // x-coord of lower-right corner
                          RCINT nBottomRect  // y-coord of lower-right corner
                          );


extern RCINT RCSELECTCLIPRGN( RCHDC hdc,    // handle to DC
                       RCHRGN hrgn   // handle to region
                       );

extern RCBOOL RCPTVISIBLE( RCHDC hdc,  // handle to DC
                           RCINT X,    // x-coordinate of point
                           RCINT Y     // y-coordinate of point
                           );

extern RCINT RCFILLRECT( RCHDC hDC,           // handle to DC
                         CONST RCRECT *lprc,  // rectangle
                         RCHBRUSH hbr         // handle to brush
                         );

RCINT RCFILLRECT( QPainter *hDC,       // handle to a QTpainter
                  CONST RCRECT *lprc,  // rectangle
                  RCHBRUSH hbr         // handle to a brush
                 );

extern RCBOOL RCGETTEXTMETRICS( RCHDC hdc,            // handle to DC
                                  RCLPTEXTMETRIC lptm   // text metrics
                                  );

extern RCBOOL RCGETTEXTMETRICSW( RCHDC hdc,            // handle to DC
                                  RCLPTEXTMETRICW lptm   // text metrics
                                  );

extern RCUINT_PTR RCSETTIMER( RCHWND hWnd,
                              RCUINT_PTR nIDEvent,
                              RCUINT uElapse,
                              RCTIMERPROC lpTimerFunc
                              );

extern RCHWND RCSETCLIPBOARDVIEWER( RCHWND hWndNewViewer );

extern RCVOID RCDRAGACCEPTFILES( RCHWND hWnd, RCBOOL fAccept );

extern RCDWORD RCSETCLASSLONG( RCHWND hWnd, RCINT nIndex, RCLONG dwNewLong );

extern RCBOOL RCKILLTIMER( RCHWND hWnd, RCUINT_PTR uIDEvent );

extern RCBOOL RCSETWINDOWPOS( RCHWND hWnd,
                              RCHWND hWndInsertAfter,
                              RCINT X,
                              RCINT Y,
                              RCINT cx,
                              RCINT cy,
                              RCUINT uFlags
                              );

extern RCUINT RCSETTEXTALIGN( RCHDC hdc,     // handle to DC
                              RCUINT fMode   // text-alignment option
                              );

extern RCBOOL RCOPENCLIPBOARD( RCHWND hWndNewOwner );

extern RCBOOL RCSETSCROLLRANGE( RCHWND hWnd,
                                RCINT nBar,
                                RCINT nMinPos,
                                RCINT nMaxPos,
                                RCBOOL bRedraw
                                );

extern RCBOOL RCINVERTRECT( RCHDC hDC,           // handle to DC
                            CONST RCRECT *lprc   // rectangle
                            );

extern RCBOOL RCPOLYGON( RCHDC hdc,                // handle to DC
                  CONST RCPOINT *lpPoints,  // polygon vertices
                  RCINT nCount              // count of polygon vertices
                  );

extern RCBOOL RCELLIPSE( RCHDC hdc,        // handle to DC
                  RCINT nLeftRect,  // x-coord of upper-left corner of rectangle
                  RCINT nTopRect,   // y-coord of upper-left corner of rectangle
                  RCINT nRightRect, // x-coord of lower-right corner of rectangle
                  RCINT nBottomRect // y-coord of lower-right corner of rectangle
                  );

extern RCHWND RCSETCAPTURE( RCHWND hWnd );

extern RCBOOL RCGETSCROLLRANGE( RCHWND hWnd,
                         RCINT nBar,
                         RCLPINT lpMinPos,
                         RCLPINT lpMaxPos
                         );

extern RCINT RCSETSCROLLPOS( RCHWND hWnd,
                      RCINT nBar,
                      RCINT nPos,
                      RCBOOL bRedraw
                      );

extern RCBOOL RCGETWINDOWPLACEMENT( RCHWND hWnd, RCWINDOWPLACEMENT *lpwndpl );

extern RCLONG RCSETWINDOWLONG( RCHWND hWnd, RCINT nIndex, RCLONG dwNewLong );

extern RCBOOL RCSETWINDOWPLACEMENT( RCHWND hWnd, RCWINDOWPLACEMENT *lpwndpl );

extern RCLONG RCGETWINDOWLONG( RCHWND hWnd, RCINT nIndex );

extern RCHWND RCGETWINDOW( RCHWND hWnd, RCUINT uCmd );

extern RCBOOL RCCLIENTTOSCREEN( RCHWND hWnd,       // handle to window
                                RCLPPOINT lpPoint  // screen coordinates
                                );

extern RCBOOL RCSCREENTOCLIENT( RCHWND hWnd,        // handle to window
                         RCLPPOINT lpPoint   // screen coordinates
                         );

extern RCLRESULT RCSENDMESSAGE( RCHWND hWnd,
                         RCUINT Msg,
                         RCWPARAM wParam,
                         RCLPARAM lParam
                         );

extern RCLRESULT RCSENDMESSAGEW( RCHWND hWnd,
                         RCUINT Msg,
                         RCWPARAM wParam,
                         RCLPARAM lParam
                         );

extern RCHWND RCSETFOCUS( RCHWND hWnd );

extern RCHWND RCGETFOCUS(void);

extern RCHWND RCCHILDWINDOWFROMPOINT( RCHWND hWndParent, RCPOINT Point );

extern RCBOOL RCPEEKMESSAGE( RCLPMSG lpMsg,
                      RCHWND hWnd,
                      RCUINT wMsgFilterMin,
                      RCUINT wMsgFilterMax,
                      RCUINT wRemoveMsg
                      );

extern RCBOOL RCISICONIC( RCHWND hWnd );

extern RCBOOL RCBEGINPATH( RCHDC hdc );

extern RCBOOL RCENDPATH( RCHDC hdc  );

extern RCINT RCSETSTRETCHBLTMODE(  RCHDC hdc,           // handle to DC
                                   RCINT iStretchMode   // bitmap stretching mode
                                   );

extern RCBOOL RCSTRETCHBLT(  RCHDC hdcDest,      // handle to destination DC
                             RCINT nXOriginDest, // x-coord of destination upper-left corner
                             RCINT nYOriginDest, // y-coord of destination upper-left corner
                             RCINT nWidthDest,   // width of destination rectangle
                             RCINT nHeightDest,  // height of destination rectangle
                             RCHDC hdcSrc,       // handle to source DC
                             RCINT nXOriginSrc,  // x-coord of source upper-left corner
                             RCINT nYOriginSrc,  // y-coord of source upper-left corner
                             RCINT nWidthSrc,    // width of source rectangle
                             RCINT nHeightSrc,   // height of source rectangle
                             RCDWORD dwRop       // raster operation code
                             );

extern RCBOOL RCSELECTCLIPPATH( RCHDC hdc,    // handle to DC
                                RCINT iMode   // clipping mode
                                );

extern RCINT RCEXTSELECTCLIPRGN( RCHDC hdc,  // handle to DC
                                 RCHRGN hrgn, // handle to region
                                 RCINT fnMode        // region-selection mode
                                 );

extern RCBOOL RCRECTANGLE( RCHDC hdc,         // handle to DC
                           RCINT nLeftRect,   // x-coord of upper-left corner of rectangle
                           RCINT nTopRect,    // y-coord of upper-left corner of rectangle
                           RCINT nRightRect,  // x-coord of lower-right corner of rectangle
                           RCINT nBottomRect  // y-coord of lower-right corner of rectangle
                           );

extern RCBOOL RCENABLEWINDOW( RCHWND hWnd, RCBOOL bEnable );

extern RCBOOL RCPOSETMESSAGE( RCHWND hWnd, RCUINT Msg, RCWPARAM wParam, RCLPARAM lParam );

extern RCINT RCGETWINDOWTEXT( RCHWND hWnd, RCLPTSTR lpString, RCINT nMaxCount );

extern RCINT RCGETWINDOWTEXTW( RCHWND hWnd, RCLPTSTR lpString, RCINT nMaxCount );

extern RCBOOL RCISWINDOW( RCHWND hWnd );

extern RCHWND RCSETPARENT( RCHWND hWndChild, RCHWND hWndNewParent );


extern RCBOOL RCREDRAWWINDOW( RCHWND hWnd, // handle to window
                              CONST RCRECT *lprcUpdate,  // update rectangle
                              RCHRGN hrgnUpdate,         // handle to update region
                              RCUINT flags               // array of redraw flags
                              );

extern RCBOOL RCENABLEMENUITEM( RCHMENU hMenu, RCUINT uIDEnableItem, RCUINT uEnable );

extern RCHWND RCHTMLHELPA(
                         RCHWND    hwndCaller,
                         RCLPCSTR  pszFile,
                         RCUINT    uCommand,
                         RCDWORD   dwData ) ;

extern RCBOOL RCWINHELPA( RCHWND hWndMain,
                         RCLPCTSTR lpszHelp,
                         RCUINT uCommand,
                         RCULONG_PTR dwData
                        );

extern RCINT RCTRANSLATEACCELERATOR( RCHWND hWnd, RCHACCEL hAccTable, RCLPMSG lpMsg );

extern RCBOOL RCTRANSLATEMESSAGE( const RCMSG *lpMsg );

extern RCLRESULT RCDISPATCHMESSAGE( const RCMSG *lpmsg );

extern RCBOOL RCGETMESSAGE( RCLPMSG lpMsg,
                            RCHWND hWnd,
                            RCUINT wMsgFilterMin,
                            RCUINT wMsgFilterMax
                            );

extern RCINT RCGETCLASSNAME( RCHWND hWnd,
                             RCCHAR* lpClassName,
                             RCINT nMaxCount
                             );

extern RCBOOL RCPATBLT( RCHDC hdc,      // handle to DC
                        RCINT nXLeft,   // x-coord of upper-left rectangle corner
                        RCINT nYLeft,   // y-coord of upper-left rectangle corner
                        RCINT nWidth,   // width of rectangle
                        RCINT nHeight,  // height of rectangle
                        RCDWORD dwRop   // raster operation code
                        );

extern RCINT RCSETDIBITS( RCHDC hdc,                  // handle to DC
                          RCHBITMAP hbmp,             // handle to bitmap
                          RCUINT uStartScan,          // starting scan line
                          RCUINT cScanLines,          // number of scan lines
                          CONST RCVOID *lpvBits,      // array of bitmap bits
                          CONST RCBITMAPINFO *lpbmi,  // bitmap data
                          RCUINT fuColorUse           // type of color indexes to use
                          );

extern RCINT RCFRAMERECT( RCHDC hDC,         // handle to DC
                          CONST RCRECT *lprc,  // rectangle
                          RCHBRUSH hbr         // handle to brush
                          );

extern HMONITOR RCMONITORFROMWINDOW( RCHWND hwnd,       // handle to a window
                                     RCDWORD dwFlags    // determine return value
                                     );

extern RCHRESULT RCPLAYSOUND( RCLPCWSTR pszSoundNamepqc );

extern RCBOOL RCPLAYSOUND( RCLPCSTR pszSound, RCHMODULE hmod, RCDWORD fdwSound );

extern RCDWORD RCGETMODULEFILENAME( RCHMODULE hModule, RCLPTSTR lpFilename, RCDWORD nSize );

extern RCBOOL RCSETDLGITEMTEXT( RCHWND hDlg, RCINT nIDDlgItem, RCLPCTSTR lpString );
extern RCBOOL RCSETDLGITEMTEXT( RCHWND hDlg, RCINT nIDDlgItem, char * lpString );

extern RCHWND RCGETDLGITEM( RCHWND hDlg, RCINT nIDDlgItem );

extern RCLRESULT RCSENDITEMMESSAGE( RCHWND hDlg,
                                     RCINT nIDDlgItem,
                                     RCUINT Msg,
                                     RCWPARAM wParam,
                                     RCLPARAM lParam
                                     );

extern RCLRESULT RCSENDITEMMESSAGEW( RCHWND hDlg,
                                     RCINT nIDDlgItem,
                                     RCUINT Msg,
                                     RCWPARAM wParam,
                                     RCLPARAM lParam
                                     );

extern RCBOOL RCENDDIALOG( RCHWND hDlg, RCINT_PTR nResult );

extern RCBOOL RCCHECKDLGBUTTON( RCHWND hDlg, RCINT nIDButton, RCUINT uCheck );

extern RCUINT RCISDLGBUTTONCHECKED( RCHWND hDlg, RCINT nIDButton );

extern RCUINT RCGETDLGITEMTEXT( RCHWND hDlg, RCINT nIDDlgItem, RCLPTSTR lpString, RCINT nMaxCount );

extern RCUINT RCGETDLGITEMTEXTW( RCHWND hDlg, RCINT nIDDlgItem, RCLPTSTR lpString, RCINT nMaxCount );

extern RCHWND RCCREATEDIALOG( RCHINSTANCE hInstance,
                              RCLPCTSTR lpTemplate,
                              RCHWND hWndParent,
                              RCDLGPROC lpDialogFunc
                              );

extern RCINT_PTR RCDIALOGBOXINDIRECT( RCHINSTANCE hInstance,
                                      RCLPCDLGTEMPLATE lpTemplate,
                                      RCHWND hWndParent,
                                      RCDLGPROC lpDialogFunc
                                      );

extern RCINT_PTR RCDIALOGBOX( RCHINSTANCE hInstance,
                              RCLPCTSTR lpTemplate,
                              RCHWND hWndParent,
                              RCDLGPROC lpDialogFunc
                              );


extern RCDWORD RCGETGLYPHOUTLINE( RCHDC hdc,             // handle to DC
                                  RCUINT uChar,          // character to query
                                  RCUINT uFormat,        // data format
                                  RCLPGLYPHMETRICS lpgm,   // glyph metrics
                                  RCDWORD cbBuffer,      // size of data buffer
                                  RCLPVOID lpvBuffer,    // data buffer
                                  CONST RCMAT2 *lpmat2   // transformation matrix
                                 );

extern RCDWORD RCGETGLYPHOUTLINEW( RCHDC hdc,             // handle to DC
                                   RCUINT uChar,          // character to query
                                   RCUINT uFormat,        // data format
                                   RCLPGLYPHMETRICS lpgm,   // glyph metrics
                                   RCDWORD cbBuffer,      // size of data buffer
                                   RCLPVOID lpvBuffer,    // data buffer
                                   CONST RCMAT2 *lpmat2   // transformation matrix
                                  );
extern RCBOOL RCEXTTEXTOUT( RCHDC hdc,          // handle to DC
                     RCINT X,            // x-coordinate of reference point
                     RCINT Y,            // y-coordinate of reference point
                     RCUINT fuOptions,   // text-output options
                     CONST RCRECT* lprc, // optional dimensions
                     RCLPCTSTR lpString, // string
                     RCUINT cbCount,     // number of characters in string
                     CONST RCINT* lpDx   // array of spacing values
                    );

extern RCINT RCENUMFONTFAMILIES( RCHDC hdc,                        // handle to DC
                                 RCLPCTSTR lpszFamily,             // font family
                                 RCFONTENUMPROC lpEnumFontFamProc, // callback function
                                 RCLPARAM lParam                   // additional data
                               );

extern RCINT RCENUMFONTFAMILIESW( RCHDC hdc,                        // handle to DC
                                 RCLPCTSTR lpszFamily,             // font family
                                 RCFONTENUMPROCW lpEnumFontFamProc, // callback function
                                 RCLPARAM lParam                   // additional data
                               );

extern RCWINOLEAPI RCSTGISSTORAGEFILE(  RCWCHAR* pwcsName );

extern RCUINT_PTR CALLBACK RCOFNHookProcOldStyle( RCHWND hdlg,
    RCUINT uiMsg,
    RCWPARAM wParam,
    RCLPARAM lParam
);

extern RCBOOL RCGETSAVEFILENAME( RCLPOPENFILENAME lpofn );

extern RCCHAR *RCITOA( RCINT value, RCCHAR *str, RCINT radix  );

extern RCINT  RCSTRETCHDIBITS(  RCHDC hdc,                      // handle to DC
  RCINT  XDest,                    // x-coord of destination upper-left corner
  RCINT  YDest,                    // y-coord of destination upper-left corner
  RCINT  nDestWidth,               // width of destination rectangle
  RCINT  nDestHeight,              // height of destination rectangle
  RCINT  XSrc,                     // x-coord of source upper-left corner
  RCINT  YSrc,                     // y-coord of source upper-left corner
  RCINT  nSrcWidth,                // width of source rectangle
  RCINT  nSrcHeight,               // height of source rectangle
  CONST RCVOID *lpBits,           // bitmap bits
  CONST RCBITMAPINFO *lpBitsInfo, // bitmap data
  RCUINT iUsage,                  // usage options
  RCDWORD dwRop                   // raster operation code
                    );

extern RCDWORD RCGETTEMPPATH( RCDWORD nBufferLength, RCLPTSTR lpBuffer );

extern RCUINT RCGETTEMPFILENAME( RCLPCTSTR lpPathName, RCLPCTSTR lpPrefixString,  RCUINT uUnique,  RCLPTSTR lpTempFileName);

extern RCBOOL   RCDELETEFILE( RCLPCTSTR lpFileName );

extern RCSHORT RCGETFILETITLE(  RCLPCTSTR lpszFile,
                     RCLPTSTR lpszTitle,
                     RCWORD cbBuf
                     );

extern RCBOOL  RCCOPYFILE( RCLPCTSTR lpExistingFileName, RCLPCTSTR lpNewFileName, RCBOOL bFailIfExists );

extern RCHMODULE RCLOADLIBRARY( RCLPCTSTR lpFileName );

extern RCFARPROC WINAPI RCGETPROCADDRESS( RCHMODULE hModule, RCLPCSTR lpProcName );
extern RCFARPROC WINAPI RCGETPROCADDRESS( RCHMODULE hModule, LPCSTR lpProcName );

extern RCBOOL RCCREATEPROCESS(
        RCLPCTSTR lpApplicationName,
        RCLPTSTR lpCommandLine,
        RCLPSECURITY_ATTRIBUTES lpProcessAttributes,
        RCLPSECURITY_ATTRIBUTES lpThreadAttributes,
        RCBOOL bInheritHandles,
        RCDWORD dwCreationFlags,
        RCLPVOID lpEnvironment,
        RCLPCTSTR lpCurrentDirectory,
        RCLPSTARTUPINFO lpStartupInfo,
        RCLPPROCESS_INFORMATION lpProcessInformation
        );

extern RCBOOL RCFREELIBRARY( RCHMODULE hModule );

extern RCINT RCGETSCROLLPOS( RCHWND hWnd, RCINT nBar );

extern RCintptr_t RCFINDFIRST(const RCCHAR *filespec,
            struct RC_finddata_t *fileinfo
            /*, QDir::Filters *filters=0*/);
/*
extern RCLONG RCREGOPENKEYEX( RCHKEY hKey,
                            RCLPCTSTR lpSubKey,
                            RCDWORD ulOptions,
                            RCREGSAM samDesired,
                            RCPHKEY phkResult
                           );
extern LONG RCREGOPENKEYEX( HKEY hKey,
                            LPCTSTR lpSubKey,
                            DWORD ulOptions,
                            REGSAM samDesired,
                            PHKEY phkResult
                           );
*/
extern RCUINT   RCWINEXEC( RCLPCSTR lpCmdLine, RCUINT uCmdShow );

extern RCSIZE_T RCSTRFTIME( RCCHAR *strDest,
                 RCSIZE_T maxsize,
                 const RCCHAR *format,
                 const struct tm *timeptr
                 );

extern RCHFILE   RCOPENFILE( RCLPCSTR lpFileName, RCLPOFSTRUCT lpReOpenBuff, RCUINT uStyle );

extern RCDWORD RCCHECKMENUITEM( RCHMENU hmenu, RCUINT uIDCheckItem, RCUINT uCheck );

extern RCINT RCGETMENUSTRING( RCHMENU hMenu,
                   RCUINT uIDItem,
                   RCLPTSTR lpString,
                   RCINT nMaxCount,
                   RCUINT uFlag
                   );

extern RCBOOL RCPrintDlg( RCLPPRINTDLG lppd );
#ifdef _MAC
typedef struct _devicemode {
  BCHAR  dmDeviceName[CCHDEVICENAME];
  WORD   dmSpecVersion;
  WORD   dmDriverVersion;
  WORD   dmSize;
  WORD   dmDriverExtra;
  DWORD  dmFields;
  union {
    struct {
      short dmOrientation;
      short dmPaperSize;
      short dmPaperLength;
      short dmPaperWidth;
      short dmScale;
      short dmCopies;
      short dmDefaultSource;
      short dmPrintQuality;
    };
    POINTL dmPosition;
    DWORD  dmDisplayOrientation;
    DWORD  dmDisplayFixedOutput;
  };
  short  dmColor;
  short  dmDuplex;
  short  dmYResolution;
  short  dmTTOption;
  short  dmCollate;
  BYTE  dmFormName[CCHFORMNAME];
  WORD  dmLogPixels;
  DWORD  dmBitsPerPel;
  DWORD  dmPelsWidth;
  DWORD  dmPelsHeight;
  union {
    DWORD  dmDisplayFlags;
    DWORD  dmNup;
  };
  DWORD  dmDisplayFrequency;
#if(WINVER >= 0x0400)
  DWORD  dmICMMethod;
  DWORD  dmICMIntent;
  DWORD  dmMediaType;
  DWORD  dmDitherType;
  DWORD  dmReserved1;
  DWORD  dmReserved2;
#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
  DWORD  dmPanningWidth;
  DWORD  dmPanningHeight;
#endif
#endif /* WINVER >= 0x0400 */
} DEVMODE;
typedef DEVMODE *LPDEVMODE;
#endif
typedef DEVMODE RCDEVMODE ;
#ifdef _MAC
typedef struct tagDEVNAMES {
   WORD wDriverOffset;
   WORD wDeviceOffset;
   WORD wOutputOffset;
   WORD wDefault;
} DEVNAMES;
typedef DEVNAMES *LPDEVNAMES;
#endif
typedef const DEVNAMES *PCDEVNAMES;
extern RCHDC RCCRETEIC( RCLPCTSTR lpszDriver,       // driver name
  RCLPCTSTR lpszDevice,       // device name
  RCLPCTSTR lpszOutput,       // port or file name
  CONST RCDEVMODE *lpdvmInit  // optional initialization data
               );

extern RCINT RCDOCABORT( RCHDC hdc );

extern RCINT RCEndPage( RCHDC hdc );

extern RCINT RCEndDoc( RCHDC hdc );
#ifdef _MAC
typedef struct {
  int     cbSize;
  LPCTSTR lpszDocName;
  LPCTSTR lpszOutput;
  LPCTSTR lpszDatatype;
  DWORD   fwType;
} DOCINFO, *LPDOCINFO;
#endif
typedef DOCINFO RCDOCINFO;
extern RCINT RCStartDoc( RCHDC hdc, // handle to DC
              CONST RCDOCINFO* lpdi // contains file names
              );
extern RCINT RCStartPage( RCHDC hDC );
extern RCHDC RCCreateEnhMetaFile( RCHDC hdcRef,            // handle to reference DC
  RCLPCTSTR lpFilename,    // file name
  CONST RCRECT* lpRect,    // bounding rectangle
  RCLPCTSTR lpDescription  // description string
                        );

typedef void* RCHENHMETAFILE;
typedef void* RCHMETAFILE;
extern RCHENHMETAFILE RCCloseEnhMetaFile( RCHDC hdc );

extern RCBOOL RCDeleteEnhMetaFile( RCHENHMETAFILE hemf );

extern RCBOOL RCSetWindowExtEx( RCHDC hdc, // handle to device context
                       RCINT nXExtent,  // new horizontal window extent
                       RCINT nYExtent,  // new vertical window extent
                       RCLPSIZE lpSize  // original window extent
                      );

extern RCBOOL RCCloseFigure( RCHDC hdc );

extern RCHBITMAP RCLoadBitmap( RCHINSTANCE hInstance,  // handle to application instance
                               RCLPCTSTR lpBitmapName  // name of bitmap resource
                               );

extern RCHMETAFILE RCGetMetaFile( RCLPCTSTR lpszMetaFile );

extern RCHENHMETAFILE RCGetEnhMetaFile( RCLPCTSTR lpszMetaFile );

#define CALLBACK __stdcall
//#define RCHANDLETABLE HANDLETABLE
//#define RCMETARECORD  METARECORD
#ifdef _MAC
typedef HANDLE HGDIOBJ;
typedef struct tagHANDLETABLE
  {
    HGDIOBJ     objectHandle[1];
  } HANDLETABLE, *PHANDLETABLE, FAR *LPHANDLETABLE;
#endif
typedef HANDLETABLE RCHANDLETABLE;
#ifdef _MAC
typedef struct tagMETARECORD
  {
    DWORD       rdSize;
    WORD        rdFunction;
    WORD        rdParm[1];
  } METARECORD;
#endif
typedef METARECORD RCMETARECORD;
typedef int (CALLBACK* MFENUMPROC)(
             HDC hdc,
             HANDLETABLE FAR* lpht,
             METARECORD FAR* lpMR,
             int nObj,
             LPARAM param);
typedef MFENUMPROC RCMFENUMPROC;
extern RCBOOL RCEnumMetaFile(  RCHDC hdc,                // handle to DC
  RCHMETAFILE hmf,          // handle to Windows-format metafile
  RCMFENUMPROC lpMetaFunc,  // callback function
  RCLPARAM lParam           // optional data
                    );

extern RCINT CALLBACK RCEnumMetaFileProc(  RCHDC hDC,                // handle to DC
  RCHANDLETABLE *lpHTable,  // metafile handle table
  RCMETARECORD *lpMFR,      // metafile record
  RCINT nObj,               // count of objects
  RCLPARAM lpClientData     // optional data
                                    );

extern RCBOOL RCDeleteMetaFile(  RCHMETAFILE hmf  );

extern RCBOOL RCSetDllDirectory( RCLPCTSTR lpPathName );

extern RCBOOL RCGetOpenFileName( RCLPOPENFILENAME lpofn );
extern RCBOOL RCGetOpenFileNameA( RCLPOPENFILENAMEA lpofn );

extern RCULONG RCinet_addr( const RCCHAR* cp);
#ifdef _MAC
typedef struct hostent {
    char FAR* h_name;
    char FAR  FAR** h_aliases;
    short h_addrtype;
    short h_length;
    char FAR  FAR** h_addr_list;
} HOSTENT,  *PHOSTENT,  FAR *LPHOSTENT;
struct sockaddr {
        ushort  sa_family;
        char    sa_data[14];
};
typedef struct in_addr {
        union {
                struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { USHORT s_w1,s_w2; } S_un_w;
                ULONG S_addr;
        } S_un;
#define s_addr  S_un.S_addr /* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2    // host on imp
#define s_net   S_un.S_un_b.s_b1    // network
#define s_imp   S_un.S_un_w.s_w2    // imp
#define s_impno S_un.S_un_b.s_b4    // imp #
#define s_lh    S_un.S_un_b.s_b3    // logical host
} IN_ADDR, *PIN_ADDR, FAR *LPIN_ADDR;
typedef struct sockaddr_in {
#if (_WIN32_WINNT < 0x0600)
    short   sin_family;
#else //(_WIN32_WINNT < 0x0600)
    ADDRESS_FAMILY sin_family;
#endif //(_WIN32_WINNT < 0x0600)

    USHORT sin_port;
    IN_ADDR sin_addr;
    CHAR sin_zero[8];
} SOCKADDR_IN, *PSOCKADDR_IN;
typedef struct sockaddr_in SOCKADDR_IN;
#endif
typedef struct hostent RChostent;
extern RChostent* RCgethostbyname( const RCCHAR* name );
typedef UINT_PTR SOCKET;
typedef SOCKET RCSOCKET;
extern RCINT send( RCSOCKET s,
            const RCCHAR* buf,
            RCINT len,
            RCINT flags
            );

extern RCINT RCrecv(
        RCSOCKET s,
        RCCHAR* buf,
        RCINT len,
        RCINT flags
        );

extern RCBOOL RCSHGetSpecialFolderPath( RCHWND hwndOwner,
                               RCLPSTR lpszPath,
                               RCINT nFolder,
                               RCBOOL fCreate
                               );
#define PASCAL
/*
typedef RCULONG RCLHANDLE;
typedef RCULONG (FAR PASCAL RCMAPISENDMAIL)(
    RCLHANDLE lhSession,
    RCULONG_PTR ulUIParam,
    RClpMapiMessage lpMessage,
    RCFLAGS flFlags,
    RCULONG ulReserved
);
typedef RCMAPISENDMAIL FAR *RCLPMAPISENDMAIL;
RCMAPISENDMAIL RCMAPISendMail;
*/
RCBOOL RCTrackPopupMenu( RCHMENU hMenu,
    RCUINT uFlags,
    RCINT x,
    RCINT y,
    RCINT nReserved,
    RCHWND hWnd,
    RCHWND prcRect
     );

typedef int RCPTR;
RCBOOL RCInsertMenu( RCHMENU hMenu,
    RCUINT uPosition,
    RCUINT uFlags,
    RCPTR uIDNewItem,
    RCLPCTSTR lpNewItem
);

RCBOOL RCInsertMenu( RCHMENU hMenu,
    RCUINT uPosition,
    RCUINT uFlags,
    RCPTR uIDNewItem,
    const char* lpNewItem
);

typedef void* RCHDROP;
RCUINT RCDragQueryFile( RCHDROP hDrop,
    RCUINT iFile,
    RCLPTSTR lpszFile,
    RCUINT cch
);

extern  RCHFONT RCCreateFont(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD fdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  LPCTSTR lpszFace           // typeface name
                   );

extern  RCHFONT RCCreateFont(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD fdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  const char * lpszFace           // typeface name
                   );

extern  RCHFONT RCCreateFont(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD fdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  RCCHAR* lpszFace           // typeface name
            );

extern  RCHFONT RCCreateFontW(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD fdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  const RCCHAR* lpszFace           // typeface name
            );

extern  RCHFONT RCCreateFontW(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD fdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  LPCTSTR lpszFace           // typeface name
                   );

extern RCBOOL RCDeleteObject(  RCHGDIOBJ hObject );

extern RCHGDIOBJ RCGetStockObject(  RCINT fnObject  );

extern RCHANDLE RCGetClipboardData( RCUINT uFormat );

extern RCLPVOID WINAPI RCGlobalLock( RCHGLOBAL hMem );

extern RCBOOL WINAPI RCGlobalUnlock( RCHGLOBAL hMem );

extern RCBOOL RCCloseClipboard(  );

extern RCHGLOBAL WINAPI RCGlobalAlloc( RCUINT uFlags, RCSIZE_T dwBytes );

extern RCHANDLE RCSetClipboardData( RCUINT uFormat, RCHANDLE hMem );

extern RCSHORT RCGetKeyState( RCINT nVirtKey );

extern RCDWORD WINAPI RCGetSysColor( RCINT nIndex );

extern RCBOOL RCReleaseCapture();

extern RCVOID RCPostQuitMessage( RCINT nExitCode );


extern RCHCURSOR RCLoadCursor( RCHINSTANCE hInstance, RCCHAR* lpCursorName );

extern RCHCURSOR RCLoadCursor( RCHINSTANCE hInstance, ULONG_PTR lpCursorName);

extern RCHCURSOR RCLoadCursor( RCHINSTANCE hInstance, char* lpCursorName );

extern RCHBRUSH RCCreateSolidBrush(  RCCOLORREF crColor );

extern RCHCURSOR RCSetCursor( RCHCURSOR hCursor );

extern RCHRESULT RCSleep( RCDWORD dwMilliseconds );

extern RCDWORD RCtimeGetTime();

//static RCtime_t time( RCtime_t * _Time);

extern RCBOOL RCEmptyClipboard();

extern RCSIZE_T WINAPI RCGlobalSize( HGLOBAL hMem );

extern u_short RChtons( u_short hostshort );

#define WINSOCK_API_LINKAGE
#define WSAAPI   FAR PASCAL
extern WINSOCK_API_LINKAGE SOCKET WSAAPI RCsocket( int af, int type, int protocol );

extern WINSOCK_API_LINKAGE int WSAAPI RCconnect( SOCKET s,
                                        const struct sockaddr FAR * name,
                                        int namelen
                                        );

extern RCBOOL RCGetCursorPos( RCLPPOINT lpPoint );

extern RCHWND RCWindowFromPoint( RCPOINT Point );

extern WINSOCK_API_LINKAGE int WSAAPI RCclosesocket( SOCKET s );

extern RCLONG WINAPI RCRegCloseKey( RCHKEY hKey );

extern RCDWORD WINAPI RCGetVersion(void);

extern RCHMODULE WINAPI RCGetModuleHandle( RCLPCTSTR lpModuleName );
extern RCHMODULE WINAPI RCGetModuleHandle( LPCTSTR lpModuleName );
extern RCHMODULE WINAPI RCGetModuleHandle( const char* lpModuleName );

extern RCBOOL RCGetUpdateRect(
        RCHWND hWnd,       // handle to window
        RCLPRECT lpRect,   // update rectangle coordinates
        RCBOOL bErase      // erase state
        );

extern RCHDC RCBeginPaint( RCHWND hwnd, RCLPPAINTSTRUCT lpPaint );

extern RCBOOL RCEndPaint( RCHWND hWnd, CONST RCPAINTSTRUCT *lpPaint );

extern RCLRESULT RCDefWindowProc(
        RCHWND hWnd,
        RCUINT Msg,
        RCWPARAM wParam,
        RCLPARAM lParam
        );

extern RCINT RCLoadString(
        RCHINSTANCE hInstance,
        RCUINT uID,
        RCLPTSTR lpBuffer,
        RCINT nBufferMax
            );

RCINT RCLoadString(
        RCHINSTANCE hInstance,
        RCUINT uID,
        LPTSTR lpBuffer,
        RCINT nBufferMax
        );

extern RCHBRUSH RCGetSysColorBrush(  RCINT nIndex  );

extern RCBOOL WINAPI RCGetUserName( RCLPTSTR lpBuffer, RCLPDWORD lpnSize );

extern RCHMENU RCLoadMenu( RCHINSTANCE hInstance, RCLPCTSTR lpMenuName );
extern RCHMENU RCLoadMenu( RCHINSTANCE hInstance, LPCTSTR lpMenuName );

extern RCBOOL RCDestroyMenu( RCHMENU hMenu );

extern RCBOOL RCSetCursorPos( RCINT X, RCINT Y );

extern RCBOOL RCDrawMenuBar( RCHWND hWnd );

extern RCBOOL RCIsZoomed( RCHWND hWnd );

extern RCVOID RCDragFinish( RCHDROP hDrop );

extern RCLRESULT RCCallNextHookEx(
        RCHHOOK hhk,
        RCINT nCode,
        RCWPARAM wParam,
        RCLPARAM lParam
            );

extern RCHACCEL RCLoadAccelerators( RCHINSTANCE hInstance, RCLPCTSTR lpTableName );

extern RCBOOL RCUnregisterClass( RCLPCTSTR lpClassName, RCHINSTANCE hInstance );

extern RCLONG WINAPI RCRegCreateKey(
        RCHKEY hKey,
        RCLPCTSTR lpSubKey,
        RCPHKEY phkResult );

extern RCLONG WINAPI RCRegDeleteKey( RCHKEY hKey, RCLPCTSTR lpSubKey );

extern RCINT RCWSAStartup(  RCWORD wVersionRequested, LPWSADATA lpWSAData );

extern RCLONG WINAPI RCRegSetValueEx(
        RCHKEY hKey,
        LPCTSTR lpValueName,
        RCDWORD Reserved,
        RCDWORD dwType,
        const RCBYTE* lpData,
        RCDWORD cbData
        );

extern RCLONG WINAPI RCRegDeleteValue(
        RCHKEY hKey,
        LPCTSTR lpValueName
        );

extern RCLONG WINAPI RCRegCreateKey(
        RCHKEY hKey,
        LPCTSTR lpSubKey,
        RCPHKEY phkResult
        );

extern RCLONG WINAPI RCRegDeleteKey(
        RCHKEY hKey,
        LPCTSTR lpSubKey
        );

extern RCDWORD WINAPI RCGetTickCount(void);

extern RCBOOL RCPtInRect(  CONST RCRECT *lprc,  // rectangle
                    RCPOINT pt           // point
                  );

extern RCINT RCGetDlgCtrlID( RCHWND hwndCtl );

extern RCBOOL RCGetMonitorInfo(  HMONITOR hMonitor,  LPMONITORINFO lpmi  );

extern RCHWND RCGetDesktopWindow(void);

extern HMONITOR RCMonitorFromRect(  const RECT *lprc, DWORD dwFlags );

extern HGLOBAL WINAPI RCGlobalFree( HGLOBAL hMem );

extern HHOOK RCSetWindowsHookEx(
        int idHook,
        HOOKPROC lpfn,
        RCHINSTANCE hMod,
        DWORD dwThreadId
        );

extern DWORD WINAPI RCGetCurrentThreadId(void);

extern BOOL RCUnhookWindowsHookEx( HHOOK hhk );

extern BYTE RCGetRValue(  DWORD rgb );
extern BYTE RCGetGValue(  DWORD rgb );
extern BYTE RCGetBValue(  DWORD rgb );

extern RCHPEN RCCreatePen(
        int fnPenStyle,    // pen style
        int nWidth,        // pen width
        RCCOLORREF crColor   // pen color
        );

extern BOOL RCGdiFlush(void);

extern int RCGetObject(
        HGDIOBJ hgdiobj,  // handle to graphics object
        int cbBuffer,     // size of buffer for object information
        LPVOID lpvObject  // buffer for object information
        );

extern int WINAPI RCGetSystemMetrics( int nIndex );

extern RCHICON RCLoadIcon( RCHINSTANCE hInstance, RCSTRING lpIconName );
extern RCHICON RCLoadIcon( RCHINSTANCE hInstance, ULONG_PTR lpIconName );

extern RCHFONT RCCreateFontIndirect(  CONST LOGFONT* lplf  );
extern RCHFONT RCCreateFontIndirectW(  CONST LOGFONTW* lplf  );

extern RCHRGN RCCreateRectRgn(  int nLeftRect,   // x-coordinate of upper-left corner
                     int nTopRect,    // y-coordinate of upper-left corner
                     int nRightRect,  // x-coordinate of lower-right corner
                     int nBottomRect  // y-coordinate of lower-right corner
                     );


extern MCIERROR RCmciSendCommand(
                                MCIDEVICEID IDDevice,
                                UINT        uMsg,
                                DWORD       fdwCommand,
                                DWORD_PTR   dwParam
                                );

typedef BOOL (CALLBACK* MONITORENUMPROC)(HMONITOR, HDC, LPRECT, LPARAM);
extern BOOL RCEnumDisplayMonitors(
        HDC hdc,                   // handle to display DC
        const RECT* lprcClip,          // clipping rectangle
        MONITORENUMPROC lpfnEnum,  // callback function
        LPARAM dwData              // data for callback function
        );

extern UINT WINAPI RCSetErrorMode( UINT uMode );

extern int RCFINDNEXT( intptr_t handle, struct RC_finddata_t *fileinfo );

extern int RCFINDCLOSE( intptr_t handle );

extern int RCCopyAcceleratorTable(
        RCHACCEL hAccelSrc,
        RCVPACCEL lpAccelDst,
        int cAccelEntries
        );

extern RCHACCEL RCCreateAcceleratorTable(
        RCVPACCEL lpaccl,
        int cEntries
        );

extern BOOL RCDestroyAcceleratorTable( RCHACCEL hAccel );

extern void WINAPI RCGlobalMemoryStatus( LPMEMORYSTATUS lpBuffer );
#ifdef _MAC
typedef char      OLECHAR;
typedef LPSTR     LPOLESTR;
typedef LPCSTR    LPCOLESTR;
typedef OLECHAR **SNB;
#endif
extern HRESULT RCStgOpenStorage(  const WCHAR* pwcsName,  IStorage* pstgPriority,
  DWORD grfMode,   SNB snbExclude,   DWORD reserved,   IStorage** ppstgOpen );

extern HRESULT RCStgCreateDocfile(
  const WCHAR* pwcsName,
  DWORD grfMode,
  DWORD reserved,
  IStorage** ppstgOpen
);

extern HDC RCCreateDC(  LPCTSTR lpszDriver,        // driver name
  LPCTSTR lpszDevice,        // device name
  LPCTSTR lpszOutput,        // not used; should be NULL
  CONST DEVMODE* lpInitData  // optional printer data
               );

extern BOOL RCGetVersionEx(  LPOSVERSIONINFO lpVersionInformation );

extern int RCMultiByteToWideChar(
        UINT CodePage,
        DWORD dwFlags,
        LPCSTR lpMultiByteStr,
        int cbMultiByte,
        LPWSTR lpWideCharStr,
        int cchWideChar
        );

extern int RCMultiByteToWideChar(
        UINT CodePage,
        DWORD dwFlags,
        LPCSTR lpMultiByteStr,
        int cbMultiByte,
        RCCHAR* lpWideCharStr,
        int cchWideChar
        );

extern UINT RCGetACP(void);

extern HLOCAL RCLocalAlloc(  UINT uFlags,   UINT uBytes );

extern DWORD WINAPI RCWaitForSingleObject( HANDLE hHandle, DWORD dwMilliseconds );

extern RCCHAR *RCgetenv( const RCCHAR *varname  );
extern char *RCgetenv( const char *varname  );

//extern IJLERR RCijlInit( JPEG_CORE_PROPERTIES* jcprops );
//extern IJLERR RCijlFree( JPEG_CORE_PROPERTIES* jcprops );
//extern IJLERR RCijlWrite( JPEG_CORE_PROPERTIES* jcprops, IJLIOTYPE iotype );

extern RCLPWSTR *RCCommandLineToArgvW( RCLPCWSTR lpCmdLine, RCINT *pNumArgs );

inline RCINT RCMAKEINTRESOURCE( WORD i )
{
    return i;
}
#define MAKEINTRESOURCE RCMAKEINTRESOURCE

extern RCSTRING RCFixupPathSeperator(RCCHAR *name);
extern RCSTRING RCFixupPathSeperator(RCSTRING name);

extern RCINT RCGetHDPIXscale(RCHWND hWnd=0);
extern RCINT RCGetHDPIYscale(RCHWND hWnd=0);

//////////////////////////////////////////////////////////////////////////////////////////

#ifdef _USING_QT
#ifdef _USING_QTCHAR
//#define long RCLONG
//#define localtime RCLOCALTIME
#define strpbrk RCSTRPBRK
#define strtok RCSTRTOK
#define _unlink RCUNLINK
#define strcpy RCSTRCPY
#define strncpy RCSTRNCPY
#define _tcscpy RCSTRCPY
#define _stricmp RCSTRICMP
#define stricmp RCSTRICMP
#define strncmp RCSTRNCMP
#define strcmp RCSTRCMP
#define _tcscmp RCSTRCMP
#define strcat RCSTRCAT
#define strlen RCSTRLEN
#define _tcslen RCSTRLEN
#define strlwr RCSTRLWR
#define _strlwr RCSTRLWR
#define _tcslwr RCSTRLWR
#define _strupr RCSTRUPR
#define strupr RCSTRUPR
#define strtok RCSTRTOK
#define sprintf RCSPRINTF
#define sscanf RCSSCANF
#define atoi RCATOI
#define atof RCATOF
#define strtod RCSTRTOD
#define strrchr RCSTRRCHR
#define _tcsrchr  RCSTRRCHR
#define _strnicmp RCSTRNICMP
#define _getcwd RCGETCWD
#define _tgetcwd RCGETCWD
#define _mkdir RCMKDIR
#define _mktemp RCMKTEMP
#define strchr RCSTRCHR
#define stat RCSTAT
#define _stat RCSTAT
#define strtol RCSTRTOL
#define tolower RCTOLOWER
#define isdigit RCISDIGIT
#define _tcsstr strstr
#define GetPrivateProfileString RCGETPRIVATEPROFILESTRING
#define WritePrivateProfileString RCWRITEPRIVATEPROFILESTRING
#define GetPrivateProfileInt RCGETPRIVATEPROFILEINT
#define GetProfileString RCGETPROFILESTRING
#define WriteProfileString RCWRITEPROFILESTRING
#define GetWindowsDirectory RCGETWINDOWSDIRECTORY
#define GetVolumeInformation RCGETVOLUMEINFORMATION
#define RegQueryValueEx RCREGQUERYVALUEEX
#define RegQueryValue RCREGQUERYVALUE
#define RegOpenKeyEx RCREGOPENKEYEX
#define CreateWindow RCCREATEWINDOW
#define CreateWindowEx RCCREATEWINDOWEX
#define GetClientRect RCGETCLIENTRECT
#define MoveWindow RCMOVEWINDOW
#define ShowWindow RCSHOWWINDOW
#define BringWindowToTop RCBRINGWINDOWTOTOP
#define InvalidateRect RCINVALIDATERECT
#define DestroyWindow RCDESTROYWINDOW
#define BitBlt RCBITBLT
#define SelectObject RCSELECTOBJECT
#define SetROP2 RCSETROP2
#define MoveToEx RCMOVETOEX
#define LineTo RCLINETO
#define Escape RCESCAPE
#define GetSystemPaletteEntries RCGETSYSTEMPALETTEENTRIES
#define GetDeviceCaps RCGETDEVICECAPS
#define SetTextColor RCSETTEXTCOLOR
#define SetBkMode RCSETBKMODE
#define SetBkColor RCSETBKCOLOR
#define GetOutlineTextMetrics RCGETOUTLINETEXTMETRICS
#define GetTextExtentPoint32 RCGETTEXTEXTENTPOINT32
#define GetTextExtentPoint32W RCGETTEXTEXTENTPOINT32W
#define GetTextExtentPoint RCGETTEXTEXTENTPOINT
#define GetTextExtent RCGETTEXTEXTENT
#define TextOutW RCTEXTOUTW
#define TextOut RCTEXTOUT
#define SetPolyFillMode RCSETPOLYFILLMODE
#define PolyPolygon RCPOLYPOLYGON
#define SetPixel RCSETPIXEL
#define GetPixel RCGETPIXEL
#define GetWindowRect RCGETWINDOWRECT
#define GetParent RCGETPARENT
#define GetDC RCGETDC
#define ReleaseDC RCRELEASEDC
#define CreateCompatibleDC RCCREATECOMPATIBLEDC
#define CreateCompatibleBitmap RCCREATECOMPATIBLEBITMAP
#define DeleteDC RCDELETEDC
#define GetDIBits RCGETDIBITS
#define SetWindowText RCSETWINDOWTEXT
#define SetWindowTextW RCSETWINDOWTEXT
#define SetMenu RCSETMENU
#define GetMenu RCGETMENU
#define GetSubMenu RCGETSUBMENU
#define RemoveMenu RCREMOVEMENU
#define AppendMenu RCAPPENDMENU
#define GetClipRgn RCGETCLIPRGN
#define IntersectClipRect RCINTERSECTCLIPRGN
#define SelectClipRgn RCSELECTCLIPRGN
#define PtVisible RCPTVISIBLE
#define FillRect RCFILLRECT
#define GetTextMetrics RCGETTEXTMETRICS
#define GetTextMetricsW RCGETTEXTMETRICSW
#define SetTimer RCSETTIMER
#define SetClipboardViewer RCSETCLIPBOARDVIEWER
#define DragAcceptFiles RCDRAGACCEPTFILES
#define SetClassLong RCSETCLASSLONG
#define KillTimer RCKILLTIMER
#define SetWindowPos RCSETWINDOWPOS
#define SetTextAlign RCSETTEXTALIGN
#define OpenClipboard RCOPENCLIPBOARD
#define SetScrollRange RCSETSCROLLRANGE
#define InvertRect RCINVERTRECT
#define Polygon RCPOLYGON
#define Ellipse RCELLIPSE
#define SetCapture RCSETCAPTURE
#define GetScrollRange RCGETSCROLLRANGE
#define SetScrollPos RCSETSCROLLPOS
#define GetWindowPlacement RCGETWINDOWPLACEMENT
#define SetWindowLong RCSETWINDOWLONG
#define SetWindowPlacement RCSETWINDOWPLACEMENT
#define GetWindowLong RCGETWINDOWLONG
#define GetWindow RCGETWINDOW
#define ClientToScreen RCCLIENTTOSCREEN
#define ScreenToClient RCSCREENTOCLIENT
#define SendMessage RCSENDMESSAGE
#define SendMessageW RCSENDMESSAGEW
#define SetFocus RCSETFOCUS
#define GetFocus RCGETFOCUS
#define ChildWindowFromPoint RCCHILDWINDOWFROMPOINT
#define PeekMessage RCPEEKMESSAGE
#define IsIconic RCISICONIC
#define BeginPath RCBEGINPATH
#define EndPath RCENDPATH
#define SetStretchBltMode RCSETSTRETCHBLTMODE
#define StretchBlt RCSTRETCHBLT
#define SelectClipPath RCSELECTCLIPPATH
#define ExtSelectClipRgn RCEXTSELECTCLIPRGN
#define Rectangle RCRECTANGLE
#define EnableWindow RCENABLEWINDOW
#define PostMessage RCPOSETMESSAGE
#define GetWindowText RCGETWINDOWTEXT
#define GetWindowTextW RCGETWINDOWTEXTW
#define IsWindow RCISWINDOW
#define SetParent RCSETPARENT
#define RedrawWindow RCREDRAWWINDOW
#define EnableMenuItem RCENABLEMENUITEM
#define HtmlHelp RCHTMLHELPA
#define HtmlHelpA RCHTMLHELPA
#define WinHelp RCWINHELPA
#define WinHelpA RCWINHELPA
#define TranslateAccelerator RCTRANSLATEACCELERATOR
#define TranslateMessage RCTRANSLATEMESSAGE
#define DispatchMessage RCDISPATCHMESSAGE
#define GetMessage RCGETMESSAGE
#define GetClassName RCGETCLASSNAME
#define PatBlt RCPATBLT
#define SetDIBits RCSETDIBITS
#define FrameRect RCFRAMERECT
#define MonitorFromWindow RCMONITORFROMWINDOW
#define PlaySound RCPLAYSOUND
#define GetModuleFileName RCGETMODULEFILENAME
#define SetDlgItemText RCSETDLGITEMTEXT
#define GetDlgItem RCGETDLGITEM
#define SendDlgItemMessage RCSENDITEMMESSAGE
#define SendDlgItemMessageW RCSENDITEMMESSAGEW
#define EndDialog RCENDDIALOG
#define CheckDlgButton RCCHECKDLGBUTTON
#define IsDlgButtonChecked RCISDLGBUTTONCHECKED
#define GetDlgItemText RCGETDLGITEMTEXT
#define GetDlgItemTextW RCGETDLGITEMTEXTW
#define CreateDialog RCCREATEDIALOG
#define DialogBoxIndirect RCDIALOGBOXINDIRECT
#define DialogBox RCDIALOGBOX
#define GetGlyphOutline RCGETGLYPHOUTLINE
#define GetGlyphOutlineW RCGETGLYPHOUTLINEW
#define ExtTextOut RCEXTTEXTOUT
#define ExtTextOutW RCEXTTEXTOUT
#define EnumFontFamilies RCENUMFONTFAMILIES
#define EnumFontFamiliesW RCENUMFONTFAMILIESW
#define StgIsStorageFile RCSTGISSTORAGEFILE
#define OFNHookProcOldStyle RCOFNHookProcOldStyle
#define GetSaveFileName RCGETSAVEFILENAME
#define GetOpenFileName RCGetOpenFileName
#define GetOpenFileNameA RCGetOpenFileNameA
#define itoa RCITOA
#define StretchDIBits RCSTRETCHDIBITS
#define GetTempPath RCGETTEMPPATH
#define GetTempFileName RCGETTEMPFILENAME
#define DeleteFile RCDELETEFILE
#define GetFileTitle RCGETFILETITLE
#define CopyFile RCCOPYFILE
#define _chdir RCCHDIR
#define fopen_s RCFOPEN_S
#define LoadLibrary RCLOADLIBRARY
#define GetProcAddress RCGETPROCADDRESS
#define CreateProcess RCCREATEPROCESS
#define FreeLibrary RCFREELIBRARY
#define GetScrollPos RCGETSCROLLPOS
#define _findfirst RCFINDFIRST
#define _findnext RCFINCDNEXT
#define _findclose RCFINDCLOSE
#define RegOpenKeyEx RCREOPENKEYEX
#define WinExec RCWINEXEC
#define strftime RCSTRFTIME
#define OpenFile RCOPENFILE
#define CheckMenuItem RCCHECKMENUITEM
#define GetMenuString RCGETMENUSTRING
#define PrintDlg RCPrintDlg
#define CreateIC RCCRETEIC
#define AbortDoc RCDOCABORT
#define EndPage RCEndPage
#define EndDoc RCEndDoc
#define StartDoc RCStartDoc
#define StartPage RCStartPage
#define CreateEnhMetaFile RCCreateEnhMetaFile
#define CloseEnhMetaFile RCCloseEnhMetaFile
#define DeleteEnhMetaFile RCDeleteEnhMetaFile
#define SetWindowExtEx RCSetWindowExtEx
#define CloseFigure RCCloseFigure
#define LoadBitmap RCLoadBitmap
#define GetEnhMetaFile RCGetEnhMetaFile
#define GetMetaFile RCGetMetaFile
#define EnumMetaFile RCEnumMetaFile
#define EnumMetaFileProc RCEnumMetaFileProc
#define DeleteMetaFile RCDeleteMetaFile
#define SetDllDirectory RCSetDllDirectory
#define inet_addr RCinet_addr
#define gethostbyname RCgethostbyname
#define recv RCrecv
#define SHGetSpecialFolderPath RCSHGetSpecialFolderPath
#define SHGetSpecialFolderPathA RCSHGetSpecialFolderPath
#define MapiSendMail RCMapiSendMail
#define TrackPopupMenu RCTrackPopupMenu
#define InsertMenu RCInsertMenu
#define DragQueryFile RCDragQueryFile
#define fprintf RCfprintf
#define fputc RCfputc
#define MessageBox RCMessageBox
#define MessageBoxW RCMessageBoxW
//#define RegisterClipboardFormat RCRegisterClipboardFormat
#define CreateFont RCCreateFont
#define CreateFontW RCCreateFontW
#define DeleteObject RCDeleteObject
#define GetStockObject RCGetStockObject
#define GetClipboardData RCGetClipboardData
#define GlobalLock RCGlobalLock
#define GlobalUnlock RCGlobalUnlock
#define CloseClipboard RCCloseClipboard
#define GlobalAlloc RCGlobalAlloc
#define SetClipboardData RCSetClipboardData
#define GetKeyState RCGetKeyState
#define GetSysColor RCGetSysColor
#define ReleaseCapture RCReleaseCapture
#define PostQuitMessage RCPostQuitMessage
#define LoadCursor RCLoadCursor
#define CreateSolidBrush RCCreateSolidBrush
#define SetCursor RCSetCursor
#define Sleep RCSleep
#define timeGetTime RCtimeGetTime
#define EmptyClipboard RCEmptyClipboard
#define GlobalSize RCGlobalSize
#define htons RChtons
#define socket RCsocket
//#define connect RCconnect
#define GetCursorPos RCGetCursorPos
#define WindowFromPoint RCWindowFromPoint
#define closesocket RCclosesocket
#define RegCloseKey RCRegCloseKey
#define GetVersion RCGetVersion
#define GetModuleHandle RCGetModuleHandle
#define GetUpdateRect RCGetUpdateRect
#define BeginPaint RCBeginPaint
#define EndPaint RCEndPaint
#define DefWindowProc RCDefWindowProc
#define LoadString RCLoadString
#define GetSysColorBrush RCGetSysColorBrush
#define GetUserName RCGetUserName
#define LoadMenu RCLoadMenu
#define DestroyMenu RCDestroyMenu
#define LoadMenu RCLoadMenu
#define SetCursorPos RCSetCursorPos
#define DrawMenuBar RCDrawMenuBar
#define IsZoomed RCIsZoomed
#define DragFinish RCDragFinish
#define CallNextHookEx RCCallNextHookEx
#define LoadAccelerators RCLoadAccelerators
#define UnregisterClass RCUnregisterClass
#define RegCreateKey RCRegCreateKey
#define RegDeleteKey RCRegDeleteKey
#define WSAStartup RCWSAStartup
#define RegSetValueEx RCRegSetValueEx
#define RegDeleteValue RCRegDeleteValue
#define RegCreateKey RCRegCreateKey
#define RegDeleteKey RCRegDeleteKey
#define GetTickCount RCGetTickCount
#define PtInRect RCPtInRect
#define GetDlgCtrlID RCGetDlgCtrlID
#define GetMonitorInfo RCGetMonitorInfo
#define GetDesktopWindow RCGetDesktopWindow
#define MonitorFromRect RCMonitorFromRect
#define GlobalFree RCGlobalFree
#define SetWindowsHookEx RCSetWindowsHookEx
#define GetCurrentThreadId RCGetCurrentThreadId
#define UnhookWindowsHookEx RCUnhookWindowsHookEx
#define GetRValue RCGetRValue
#define GetGValue RCGetGValue
#define GetBValue RCGetBValue
#define CreatePen RCCreatePen
#define GdiFlush RCGdiFlush
#define GetObject RCGetObject
#define GetSystemMetrics RCGetSystemMetrics
#define LoadIcon RCLoadIcon
#define CreateFontIndirect RCCreateFontIndirect
#define CreateFontIndirectW RCCreateFontIndirectW
#define CreateRectRgn RCCreateRectRgn
#define mciSendCommand RCmciSendCommand
#define EnumDisplayMonitors RCEnumDisplayMonitors
#define SetErrorMode RCSetErrorMode
#define _findnext RCFINDNEXT
#define _findclose RCFINDCLOSE
#define CopyAcceleratorTable RCCopyAcceleratorTable
#define CreateAcceleratorTable RCCreateAcceleratorTable
#define DestroyAcceleratorTable RCDestroyAcceleratorTable
#define GlobalMemoryStatus RCGlobalMemoryStatus
#define StgOpenStorage RCStgOpenStorage
#define StgCreateDocfile RCStgCreateDocfile
#define CreateDC RCCreateDC
#define GetVersionEx RCGetVersionEx
#define MultiByteToWideChar RCMultiByteToWideChar
#define GetACP RCGetACP
#define LocalAlloc RCLocalAlloc
#define WaitForSingleObject RCWaitForSingleObject
//#define getenv RCgetenv
#define fscanf RCFSCANF
//#define fscanf_s RCFSCANF
// intel jpeg library stuff
#define ijlInit RCijlInit
#define ijlFree RCijlFree
#define ijlWrite RCijlWrite
#define CommandLineToArgvW RCCommandLineToArgvW
#define GetAsyncKeyState  RCGetAsyncKeyState
#endif
#endif

#endif // QTDEFS_H
