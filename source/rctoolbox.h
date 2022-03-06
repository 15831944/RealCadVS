#ifndef RCTOOLBOX_H
#define RCTOOLBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

class Button;
class ButtonMenu;
class RCToolButton;

/**
 * @brief The RCToolBox class
 * This is a toolbox which has a set of main command buttons
 * and a set of sub command buttons
 * the subcommand buttons may be floatable
 * the shape may be changable
 * the buttons can be added using an api
 * this api is similar to the current RealCAD button boxes
 */
class RCToolBox : public QDockWidget
{
    Q_OBJECT

public:
    enum layoutType { vBlock, vLine, hBlock, hLine };

    int BUTTON_SIZE;
    int BUTTON_MARGIN;
    int MIDDLE_GAP;

protected:
    ButtonMenu *mButtonMenu;
    QToolButton *lastMainButton;
    QToolButton *lastSubButton;
    QToolButton *lastButton;
    QToolButton *previousButton;
    layoutType lType;
    int mnMainButtons;
    int mnSubButtons;
    int mInitlType;
    int mInitCols;
    int mnCols;
    int mnRows;
    int mnextId; // next available index
    int mnextSid; // next available section index
    QString mTitle; // short name
    QString mRole; // MainMenu; MainInterrupt etc.
    QString mAppPath; // a place to store the full path to the app which owns this menu
    Qt::DockWidgetArea mArea;
    //bool mVerticalTitleBar;

    QWidget *dockWidgetContents;
    //
    QWidget *gridLayoutWidgetMain;
    QGridLayout *gridLayoutMain;
    //
    QWidget *gridLayoutWidgetSub;
    QGridLayout *gridLayoutSub;

    // main buttons list
    QList<QToolButton*> mainButtons;

    // sub buttons list
    QList<QToolButton*> subButtons;

public:
    explicit RCToolBox(QWidget *parent = 0);
    //explicit RCToolBox(layoutType type, int nCols, int nButtons, int nSubButtons=0, QString title="", QWidget *parent = 0);
    explicit RCToolBox(layoutType type, int nCols, int nButtons, int nSubButtons=0, QWidget *parent = 0);
    ~RCToolBox();

    //void setup();
    void setup(layoutType type, int nCols, int nButtons, int nSubButtons=0);
    void setButtonMenu(ButtonMenu *bm) { mButtonMenu = bm; }
    ButtonMenu* getButtonMenu() { return mButtonMenu; }
    void initId() { mnextId = 1; }
    int nextId() { return ++mnextId; }
    int nextSid() { return (mnextSid > 12 ? 1 : 0); }
    void resetLayout(layoutType type);
    RCToolBox::layoutType getLayoutType() { return lType; }
    void retranslateUi();
    QString getIconName( QString name );
    RCToolButton*  addButton(int bid, QString name, int comid );
    RCToolButton* addButton(int bid, int sid, QString cname, int comid, Button *rcparent=0);
    void addButtons(QList<QToolButton*> * mainButts , QList<QToolButton*> * subButts=0);
    void getButtons(QList<QToolButton*> * mainButts , QList<QToolButton*> * subButts=0);
    void initButtons(int sid);
    void hideButtons(int sid);
    void pushButton(RCToolButton *b);
    void releaseButton(RCToolButton *b);
    void enableButton(RCToolButton *b, bool onOff);
    void releasAllButtons();
    void enableAllButtons(bool onOff);
    void setVerticalTitleBar(bool onOff) { (onOff==true ? setFeatures(features()|QDockWidget::DockWidgetVerticalTitleBar) : setFeatures(features()&=~QDockWidget::DockWidgetVerticalTitleBar)); }
    bool verticalTitleBar() { return (features() & QDockWidget::DockWidgetVerticalTitleBar); }
    int getSubButtonCount() { return subButtons.count(); }
    void setTitle(const QString title) { mTitle = title; }
    QString getTitle() { return mTitle; }
    QChar*  getTitle(QChar *ctitle) { ctitle = mTitle.data(); return ctitle; }
    void setRole(const QString role)
    {
        mRole = role;
        if(mRole.length())
            setObjectName(mRole);
    }
    QString getRole() { return mRole; }
    QChar*  getRole(QChar *crole) { crole = mRole.data(); return crole; }
    void saveState();
    void restoreState();
    void replaceButton(RCToolButton *from,RCToolButton *to);

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    void moveEvent(QMoveEvent *e);
    void closeEvent(QCloseEvent *event);

public slots:
    void doTopLevelChanged(bool how);
    void doDockLocationChanged(Qt::DockWidgetArea area);
    void doMainButtonPressed();
    void doMainButtonReleased();
    void doSubButtonPressed();
    void doSubButtonReleased();
    void onVisibilityChanged(bool visibility);

};


#endif // RCTOOLBOX_H
