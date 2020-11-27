#include "SwitchButton.h"
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

    _onBgColorDisabled = QAbstractButton::palette().color(QPalette::Dark);
}

void SwitchButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int offset = isChecked() ? width() - height() : 0;
    QRectF sRect(offset, 0, height(), height());

    if (!isEnabled() )
    {
        painter.setPen(QPen(isChecked() ? _onBgColorDisabled : _offBgColorDisabled));
        painter.setBrush(QBrush(isChecked() ? _onBgColorDisabled : _offBgColorDisabled));
        drawBackground(&painter);

        painter.setPen(QPen(isChecked() ? _onSliderColorDisabled : _offSliderColorDisabled));
        painter.setBrush(QBrush(isChecked() ? _onSliderColorDisabled : _offSliderColorDisabled));
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
    QRectF r = rect().adjusted(1,1,-1,-1); // Use pen size?
    painter->drawRoundedRect(r, r.height()/2.0, r.height()/2.0);
}

void SwitchButton::drawSlider(QPainter *painter, const QRectF &rect)
{
    float margin = 1 + _marginSliderRatio*height();
    QRectF r = rect.adjusted(margin, margin, -margin, -margin);
    painter->drawEllipse(r);
}
