#include "SwitchButton.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyle>
#include <QFontMetrics>

SwitchButton::SwitchButton(QWidget *parent) : QAbstractButton(parent)
{
    QAbstractButton::setCheckable(true);

    _animation = new QPropertyAnimation(this, "sliderOffset");
    _animation->setDuration(100);

    _onBgColor = QAbstractButton::palette().color(QPalette::Highlight);
}

SwitchButton::~SwitchButton()
{
    delete _animation;
}

float SwitchButton::switchWidth() const
{
    return fontMetrics().height() * _backgroundRatio;
}

void SwitchButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPalette palette = QAbstractButton::palette();
    QColor textCol = palette.color(isEnabled() ?  QPalette::Normal : QPalette::Disabled, QPalette::Text);
    QColor sliderCol = palette.color(isEnabled() ?  QPalette::Normal : QPalette::Disabled, QPalette::BrightText);

    if( isChecked() )
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(isEnabled() ? _onBgColor : textCol);
    }
    else 
    {
        painter.setPen(QPen(textCol, 2.0));
        painter.setBrush(Qt::NoBrush);
    }
    drawBackground(&painter);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(isChecked() ? sliderCol : textCol));
    drawSlider(&painter);

    painter.setPen(textCol);
    QRect r = rect().adjusted(switchWidth() + fontMetrics().width(" "), 0, 0, 0);
    painter.drawText(r, Qt::AlignVCenter|Qt::AlignLeft, text());

    return QWidget::paintEvent(event);
}

void SwitchButton::drawBackground(QPainter *painter)
{
    QRectF r(0, 0, switchWidth(), height());
    if( _sliderRatio > 1.0f ) r.adjust(0, height() * (_sliderRatio - 1), 0, -height() * (_sliderRatio - 1));
    if( !isChecked() ) r.adjust(1, 1, -1, -1);
    painter->drawRoundedRect(r, r.height()/2.0, r.height()/2.0);
}

void SwitchButton::drawSlider(QPainter *painter)
{
    float margin = height();
    if( _sliderRatio < 1.0f ) margin *= _sliderRatio;
    QRectF r = QRect(_sliderOffset, 0, height(), height()).adjusted(margin, margin, -margin, -margin);
    painter->drawEllipse(r);
}

void SwitchButton::slotClicked(bool on)
{
    _animation->setStartValue(_sliderOffset);
    _animation->setEndValue(on ? switchWidth() - height() : 0);
    _animation->start();
}

void SwitchButton::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    if( !_connected )
        connect(this, SIGNAL(toggled(bool)), this, SLOT(slotClicked(bool)));
}

void SwitchButton::resizeEvent(QResizeEvent* event)
{
    _sliderOffset = isChecked() ? switchWidth() - height() : 0;
    QAbstractButton::resizeEvent(event);
}

QSize SwitchButton::sizeHint() const
{
    QFontMetrics fm = fontMetrics();
    QSize sz = style()->itemTextRect(fm, QRect(), Qt::TextShowMnemonic, false, text()).size();
    if( !text().isEmpty() ) sz += QSize(fontMetrics().width(" "), 0);
    return QSize(switchWidth() + sz.width(), fm.height() * 1.1);
}
