#include "SwitchButton.h"
#include <QPainter>
#include <QRectF>
#include <QPalette>
#include <QPropertyAnimation>

SwitchButton::SwitchButton(QWidget *parent) : QAbstractButton(parent)
{
    QAbstractButton::setCheckable(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(slotClicked(bool)));

    _animation = new QPropertyAnimation(this, "sliderOffset");
    _animation->setDuration(100);
    
    QPalette palette = QAbstractButton::palette();
    _onBgColor = palette.color(QPalette::Highlight);
    _offBgColor = palette.color(QPalette::Mid);
    _onSliderColor = palette.color(QPalette::Base);
    _offSliderColor = palette.color(QPalette::Base);

    _onBgColorDisabled = QAbstractButton::palette().color(QPalette::Dark);
}
SwitchButton::~SwitchButton()
{
    delete _animation;
}

void SwitchButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    if (!isEnabled() )
    {
        painter.setBrush(QBrush(isChecked() ? _onBgColorDisabled : _offBgColorDisabled));
        drawBackground(&painter);

        painter.setBrush(QBrush(isChecked() ? _onSliderColorDisabled : _offSliderColorDisabled));
        drawSlider(&painter);
    }
    else
    {
        painter.setBrush(QBrush(isChecked() ? _onBgColor : _offBgColor));
        drawBackground(&painter);

        painter.setBrush(QBrush(isChecked() ? _onSliderColor : _offSliderColor));
        drawSlider(&painter);
    }

    return QWidget::paintEvent(event);
}

void SwitchButton::drawBackground(QPainter *painter)
{
    QRectF r = rect(); // Use pen size?
    if( _sliderRatio > 1.0f ) r.adjust(0, height() * (_sliderRatio - 1), 0, -height() * (_sliderRatio - 1));
    painter->drawRoundedRect(r, r.height()/2.0, r.height()/2.0);
}

void SwitchButton::drawSlider(QPainter *painter)
{
    float margin = height();
    if( _sliderRatio < 1.0f )margin *= _sliderRatio;
    QRectF r = QRect(_sliderOffset, 0, height(), height()).adjusted(margin, margin, -margin, -margin);
    painter->drawEllipse(r);
}

void SwitchButton::slotClicked(bool on)
{
    _animation->setStartValue(_sliderOffset);
    _animation->setEndValue(on ? width() - height() : 0);
    _animation->start();
}

void SwitchButton::resizeEvent(QResizeEvent* event)
{
    _sliderOffset = isChecked() ? width() - height() : 0;
    QAbstractButton::resizeEvent(event);
}
