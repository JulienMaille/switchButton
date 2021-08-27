#include "SwitchButton.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyle>
#include <QFontMetrics>
#include <QCoreApplication>

SwitchButton::SwitchButton(QWidget *parent) : QAbstractButton(parent)
{
    QAbstractButton::setCheckable(true);

    _animation = new QPropertyAnimation(this, "sliderOffset");
    _animation->setDuration(100);

    setText(tr("Off"));
    _onText = tr("On");
}

SwitchButton::~SwitchButton()
{
    delete _animation;
}

float SwitchButton::switchWidth() const
{
    return height() * _widthRatio;
}

int SwitchButton::textWidth() const
{
    return style()->itemTextRect(fontMetrics(), QRect(), Qt::TextShowMnemonic, false, text()).size().width();
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
        QColor onCol = _onBgColor.isValid() ? _onBgColor : QAbstractButton::palette().color(QPalette::Highlight);
        painter.setPen(Qt::NoPen);
        painter.setBrush(isEnabled() ? onCol : textCol);
    }
    else 
    {
        painter.setPen(QPen(textCol, _border));
        painter.setBrush(Qt::NoBrush);
    }

    drawBackground(&painter, _labelOnLeft ? textWidth() + fontMetrics().width(" ") : 0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(isChecked() && _sliderRatio < 1.0f ? sliderCol : textCol));
    drawSlider(&painter, _labelOnLeft ? textWidth() + fontMetrics().width(" ") : 0);

    painter.setPen(textCol);
    drawLabel(&painter, _labelOnLeft ? 0 : switchWidth() + fontMetrics().width(" "));

    return QWidget::paintEvent(event);
}

void SwitchButton::drawBackground(QPainter *painter, int hOff)
{
    QRectF r(hOff, 0, switchWidth(), height());
    if( _sliderRatio > 1.0f ) r.adjust(0, height() * (_sliderRatio - 1), 0, -height() * (_sliderRatio - 1));
    if( !isChecked() ) r.adjust(_border/2, _border/2, -_border/2, -_border/2);
    painter->drawRoundedRect(r, r.height()/2.0, r.height()/2.0);
}

void SwitchButton::drawSlider(QPainter *painter, int hOff)
{
    float margin = height();
    if( _sliderRatio < 1.0f ) margin *= _sliderRatio;
    QRectF r = QRect(hOff + _sliderOffset, 0, height(), height()).adjusted(margin, margin, -margin, -margin);
    painter->drawEllipse(r);
}

void SwitchButton::drawLabel(QPainter *painter, int hOff)
{
    QRect r = rect().adjusted(hOff, 0, 0, 0);
    painter->drawText(r, Qt::AlignVCenter|Qt::AlignLeft, isChecked() ? _onText : text());
}

void SwitchButton::slotClicked(bool on)
{
    while( _animation->state() == QAbstractAnimation::Running )
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents, 100);

    _animation->setStartValue(_sliderOffset);
    _animation->setEndValue(on ? switchWidth() - height() : 0);
    _animation->start();
}

void SwitchButton::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    if( !_connected )
    {
        connect(this, SIGNAL(toggled(bool)), this, SLOT(slotClicked(bool)));
        _connected = true;
    }
}

void SwitchButton::resizeEvent(QResizeEvent* event)
{
    _sliderOffset = isChecked() ? switchWidth() - height() : 0;
    QAbstractButton::resizeEvent(event);
}

QSize SwitchButton::sizeHint() const
{
    int sz = textWidth();
    if( !text().isEmpty() ) sz += fontMetrics().width(" ");
    return QSize(switchWidth() + sz, 20);
}
