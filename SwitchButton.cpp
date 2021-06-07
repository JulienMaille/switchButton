#include "SwitchButton.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyle>
#include <QFontMetrics>

SwitchButton::SwitchButton(QWidget *parent) : QAbstractButton(parent)
{
    QAbstractButton::setCheckable(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(this, SIGNAL(clicked(bool)), this, SLOT(slotClicked(bool)));

    _animation = new QPropertyAnimation(this, "sliderOffset");
    _animation->setDuration(100);

    QPalette palette = QAbstractButton::palette();
    _onBgColor = palette.color(QPalette::Highlight);
    _offBgColor = palette.color(QPalette::Mid).lighter(130);
    _onSliderColor = _offSliderColor = palette.color(QPalette::Base);
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
    painter.setPen(Qt::NoPen);

    if (!isEnabled() )
    {
        painter.setBrush(_offBgColor.darker(110));
        drawBackground(&painter);

        painter.setBrush(QAbstractButton::palette().color(QPalette::Button));
        drawSlider(&painter);
    }
    else
    {
        painter.setBrush(QBrush(isChecked() ? _onBgColor : _offBgColor));
        drawBackground(&painter);

        painter.setBrush(QBrush(isChecked() ? _onSliderColor : _offSliderColor));
        drawSlider(&painter);
    }

    painter.setPen(QAbstractButton::palette().color(isEnabled() ? QPalette::Normal : QPalette::Disabled, QPalette::Text));
    QRect r = rect().adjusted(switchWidth() + fontMetrics().width(" "), 0, 0, 0);
    painter.drawText(r, Qt::AlignVCenter|Qt::AlignLeft, text());

    return QWidget::paintEvent(event);
}

void SwitchButton::drawBackground(QPainter *painter)
{
    QRectF r(0, 0, switchWidth(), height());
    if( _sliderRatio > 1.0f ) r.adjust(0, height() * (_sliderRatio - 1), 0, -height() * (_sliderRatio - 1));
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
