#pragma once

#include <QAbstractButton>

class SwitchButton : public QAbstractButton
{
    Q_OBJECT

public:
    SwitchButton(QWidget *parent = nullptr);

    QColor onBgColor() const { return _onBgColor; }
    void setOnBgColor(const QColor &color) { _onBgColor = color; repaint(); }
    QColor offBgColor() const { return _offBgColor; }
    void setOffBgColor(const QColor& color) { _offBgColor = color; _offBgColorDisabled = _offBgColor.lighter(150);  repaint(); }

    QColor onSliderColor() const { return _onSliderColor; }
    void setOnSliderColor(const QColor &color) { _onSliderColor = color; _onSliderColorDisabled = _onSliderColor.darker(120); repaint(); }
    QColor offSliderColor() const { return _offSliderColor; }
    void setOffSliderColor(const QColor& color) { _offSliderColor = color; _offSliderColorDisabled = _offSliderColor.darker(120); repaint(); }

    void setOffSliderColor(float ratio) { _marginSliderRatio = ratio; repaint(); }

    virtual QSize sizeHint() const override { return QSize(fontMetrics().height() * 3, fontMetrics().height() * 1.5); }

    bool isCheckable() const = delete;
    void setCheckable(bool) = delete;

    QString text() const = delete;
    void setText(const QString &text) = delete;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter *painter);
    void drawSlider(QPainter *painter, const QRectF &rect);

    float _marginSliderRatio = 0.20f;
    QColor _onBgColor, _offBgColor;
    QColor _onSliderColor, _offSliderColor;
    QColor _onBgColorDisabled, _offBgColorDisabled;
    QColor _onSliderColorDisabled, _offSliderColorDisabled;
};
