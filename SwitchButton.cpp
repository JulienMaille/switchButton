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

int SwitchButton::textWidth(const QString& text) const
{
    return style()->itemTextRect(fontMetrics(), QRect(), Qt::TextShowMnemonic, false, text).size().width();
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

    bool labelOnLeft = layoutDirection()==Qt::RightToLeft;
    int margin = fontMetrics().horizontalAdvance(" ");
    int textW = qMax(textWidth(text()), textWidth(_onText)) + margin;

    drawBackground(&painter, labelOnLeft ? textW : 0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(isChecked() && _sliderRatio < 1.0f ? sliderCol : textCol));
    drawSlider(&painter, labelOnLeft ? textW : 0);

    painter.setPen(textCol);
    drawLabel(&painter, labelOnLeft ? 0 : switchWidth() + margin);

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
    if( _hovered ) margin /= 1.15f;

    QRectF r = QRect(hOff + _sliderOffset, 0, height(), height()).adjusted(margin, margin, -margin, -margin);

    if( _pressed )
    {
        float stretch = 1.2f;
        float originalWidth = r.width();
        float newWidth = originalWidth * stretch;
        float delta = newWidth - originalWidth;
        r.setWidth(newWidth);

        // Move left to keep right side fixed
        if( isChecked() )
            r.translate(-delta, 0);

        painter->drawRoundedRect(r, r.height() / 2, r.height() / 2);
    }
    else
    {
        painter->drawEllipse(r);
    }
}

void SwitchButton::drawLabel(QPainter *painter, int hOff)
{
    int textW = qMax(textWidth(text()), textWidth(_onText));
    QRectF r = QRectF(hOff, 0, textW, height());
    int flags = Qt::AlignVCenter|Qt::AlignLeft;
    painter->drawText(r, flags, curText());
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
    int textW = qMax(textWidth(text()), textWidth(_onText)) + fontMetrics().horizontalAdvance(" ");
    return QSize(20 * _widthRatio + textW, 20);
}

void SwitchButton::enterEvent(QEvent *event)
{
    _hovered = true;
    QWidget::enterEvent(event);
}

void SwitchButton::leaveEvent(QEvent *event)
{
    _hovered = false;
    QWidget::leaveEvent(event);
}

void SwitchButton::mousePressEvent(QMouseEvent *event)
{
    _pressed = true;
    QAbstractButton::mousePressEvent(event);
}

void SwitchButton::mouseReleaseEvent(QMouseEvent *event)
{
    _pressed = false;
    QAbstractButton::mouseReleaseEvent(event);
}