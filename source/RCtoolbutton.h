#ifndef RCTOOLBUTTON_H
#define RCTOOLBUTTON_H

//#include <QWidget>
#include <QToolButton>

class QToolButton;
class Button;
class RCToolBox;

/**! RCToolButton class
 * This reimplements QToolBuon and is used for the tool button in the ButtonMenus
 *
 */
class RCToolButton : public QToolButton
{
    Q_OBJECT
public:
    RCToolBox *mToolBox;
    Button *rcparent;
    int mcomId;
    QString mcName;
    bool mIsSubButton;
protected:
    bool isOn;
    bool isOver;

protected:
    bool event(QEvent * event);
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
    void mouseReleaseEvent ( QMouseEvent * e );
    void mousePressEvent ( QMouseEvent * e );
    void paintEvent(QPaintEvent * event);
public:
    explicit RCToolButton(QWidget * parent=0);
    explicit RCToolButton(QString cname, int comid, QWidget * parent=0);
    ~RCToolButton();

    bool isPressed() { return isOn; }
    void setPressed(bool onOff) { isOn = onOff; }
signals:
    //void released();
};

class RCStyleButton : public RCToolButton
//class RCStyleButton : public QToolButton
{
    Q_OBJECT

protected:
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
    void mouseReleaseEvent ( QMouseEvent * e );
    void mousePressEvent ( QMouseEvent * e );
    void paintEvent(QPaintEvent * event);

public:
    explicit RCStyleButton(QWidget * parent=0);
    explicit RCStyleButton(QString cname, int comid, QWidget * parent=0);


};

#endif // RCTOOLBUTTON_H
