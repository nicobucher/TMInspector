#include "animatedstandarditem.h"

AnimatedStandardItem::AnimatedStandardItem(const QString text) : QStandardItem(text)
{
    myAnimatioGroup = new QSequentialAnimationGroup(this);
}

AnimatedStandardItem::~AnimatedStandardItem()
{
    delete myAnimatioGroup;
}

void
AnimatedStandardItem::setAnimation(QColor color)
{
    this->setBackground(color);

    QPropertyAnimation *animation_1 = new QPropertyAnimation(this, "color");
    QPropertyAnimation *animation_2 = new QPropertyAnimation(this, "color");
    QPropertyAnimation *animation_3 = new QPropertyAnimation(this, "color");
    QPropertyAnimation *animation_4 = new QPropertyAnimation(this, "color");
    QPropertyAnimation *animation_5 = new QPropertyAnimation(this, "color");
    QPropertyAnimation *animation_6 = new QPropertyAnimation(this, "color");
    QPropertyAnimation *animation_7 = new QPropertyAnimation(this, "color");

    animation_1->setDuration(400);
    animation_1->setStartValue(QColor(250, 250, 250));
    animation_1->setEndValue(color);
    animation_2->setDuration(400);
    animation_2->setStartValue(color);
    animation_2->setEndValue(QColor(250, 250, 250));
    animation_3->setDuration(400);
    animation_3->setStartValue(QColor(250, 250, 250));
    animation_3->setEndValue(color);
    animation_4->setDuration(400);
    animation_4->setStartValue(color);
    animation_4->setEndValue(QColor(250, 250, 250));
    animation_5->setDuration(400);
    animation_5->setStartValue(QColor(250, 250, 250));
    animation_5->setEndValue(color);
    animation_6->setDuration(400);
    animation_6->setStartValue(color);
    animation_6->setEndValue(QColor(250, 250, 250));
    animation_7->setDuration(400);
    animation_7->setStartValue(QColor(250, 250, 250));
    animation_7->setEndValue(color);

    myAnimatioGroup->addAnimation(animation_1);
    myAnimatioGroup->addAnimation(animation_2);
    myAnimatioGroup->addAnimation(animation_3);
    myAnimatioGroup->addAnimation(animation_4);
    myAnimatioGroup->addAnimation(animation_5);
    myAnimatioGroup->addAnimation(animation_6);
    myAnimatioGroup->addAnimation(animation_7);
}

void
AnimatedStandardItem::animate()
{
    myAnimatioGroup->start();
}

void
AnimatedStandardItem::text_ouput(QAbstractAnimation::State new_state, QAbstractAnimation::State old_state)
{
//    qDebug() << "anim_state changed from " << old_state << " to " << new_state;
}



