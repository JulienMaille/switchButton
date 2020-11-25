#include "SwitchButton.hpp"
#include <QPainter>
#include <QRectF>
#include <QPalette>

SwitchButton::SwitchButton(QWidget *parent) : QAbstractButton(parent)
{
    QAbstractButton::setCheckable(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    QPalette palette = QAbstractButton::palette();
    _onBgColor = palette.color(QPalette::Highlight);
    _offBgColor = palette.color(QPalette::Mid);
    _onSliderColor = palette.color(QPalette::Base);
    _offSliderColor = palette.color(QPalette::Base);
}

void SwitchButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    const float marginRatio = 0.20f;

    int offset = isChecked() ? width() - height() : 0;
    QRectF sRect(offset + height() * marginRatio, height() * marginRatio, height() * (1-2*marginRatio), height() * (1-2*marginRatio));

    if (!isEnabled() )
    {
        const QColor DISABLED_ON_BG_COLOR = QAbstractButton::palette().color(QPalette::Dark);
        const QColor DISABLED_OFF_BG_COLOR = _offBgColor.lighter(150);
        const QColor DISABLED_ON_SLIDER_COLOR = _onSliderColor.darker(120);
        const QColor DISABLED_OFF_SLIDER_COLOR = _offSliderColor.darker(120);
    
        painter.setPen(QPen(isChecked() ? DISABLED_ON_BG_COLOR : DISABLED_OFF_BG_COLOR));
        painter.setBrush(QBrush(isChecked() ? DISABLED_ON_BG_COLOR : DISABLED_OFF_BG_COLOR));
        drawBackground(&painter);

        painter.setPen(QPen(isChecked() ? DISABLED_ON_SLIDER_COLOR : DISABLED_OFF_SLIDER_COLOR));
        painter.setBrush(QBrush(isChecked() ? DISABLED_ON_SLIDER_COLOR : DISABLED_OFF_SLIDER_COLOR));
        drawSlider(&painter, sRect);
    }
    else
    {
        painter.setPen(QPen(isChecked() ? _onBgColor : _offBgColor));
        painter.setBrush(QBrush(isChecked() ? _onBgColor : _offBgColor));
        drawBackground(&painter);

        painter.setPen(QPen(isChecked() ? _onSliderColor : _offSliderColor));
        painter.setBrush(QBrush(isChecked() ? _onSliderColor : _offSliderColor));
        drawSlider(&painter, sRect);
    }

    return QWidget::paintEvent(event);
}

void SwitchButton::drawBackground(QPainter *painter)
{
    QRect r = rect().adjusted(1,1,-1,-1); // Use pen size?
    painter->drawRoundedRect(r, r.height()/2.0, r.height()/2.0);
}

void SwitchButton::drawSlider(QPainter *painter, const QRectF &rect)
{
    painter->drawEllipse(rect);
}
