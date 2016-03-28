#ifndef ANIMATEDSTANDARDITEM_H
#define ANIMATEDSTANDARDITEM_H

#include <QStandardItem>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

class AnimatedStandardItem : public QObject, public QStandardItem
{

    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)

  public:
    AnimatedStandardItem(const QString text);
    ~AnimatedStandardItem();
    void setColor (QColor color){
        setBackground(color);
    }
    QColor color(){
        return background().color(); // getter is not really needed for now
    }
    void setAnimation(QColor color);
    void animate();
private:
    QSequentialAnimationGroup* myAnimatioGroup;
};

#endif // ANIMATEDSTANDARDITEM_H
