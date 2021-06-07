#pragma once

#include <QAbstractButton>

class QPropertyAnimation;

class SwitchButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(int sliderOffset READ offset WRITE setOffset DESIGNABLE false)
    Q_PROPERTY(double sliderRatio READ sliderRatio WRITE setSliderRatio)
    Q_PROPERTY(double backgroundRatio READ backgroundRatio WRITE setBackgroundRatio)
    Q_PROPERTY(QColor onBgColor READ onBgColor WRITE setOnBgColor)
    Q_PROPERTY(QColor onSliderColor READ onSliderColor WRITE setOnSliderColor)
    Q_PROPERTY(QColor offBgColor READ offBgColor WRITE setOffBgColor)
    Q_PROPERTY(QColor offSliderColor READ offSliderColor WRITE setOffSliderColor)

public:
    SwitchButton(QWidget *parent = NULL);
    ~SwitchButton();

    QColor onBgColor() const { return _onBgColor; }
    void setOnBgColor(const QColor &color) { _onBgColor = color; repaint(); }
    QColor offBgColor() const { return _offBgColor; }
    void setOffBgColor(const QColor& color) { _offBgColor = color; repaint(); }

    QColor onSliderColor() const { return _onSliderColor; }
    void setOnSliderColor(const QColor &color) { _onSliderColor = color; repaint(); }
    QColor offSliderColor() const { return _offSliderColor; }
    void setOffSliderColor(const QColor& color) { _offSliderColor = color; repaint(); }

    float backgroundRatio() { return _backgroundRatio; }
    void setBackgroundRatio(float ratio) { _backgroundRatio = ratio; repaint(); }
    float sliderRatio() { return _sliderRatio; }
    void setSliderRatio(float ratio) { _sliderRatio = ratio; repaint(); }

    virtual QSize sizeHint() const override;

    bool isCheckable() const = delete;
    void setCheckable(bool) = delete;
    QIcon icon() const = delete;
    void setIcon(const QIcon &icon) = delete;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private slots:
    void slotClicked(bool on);

private:
    void drawBackground(QPainter *painter);
    void drawSlider(QPainter *painter);
    float switchWidth() const;

    int offset() const { return _sliderOffset; }
    void setOffset(int o) { _sliderOffset = o; update(); }
    QPropertyAnimation* _animation;
    int _sliderOffset = 0;
    bool _connected = false;

    float _backgroundRatio = 2.3f;
    float _sliderRatio = 0.25f;

    QColor _onBgColor, _offBgColor;
    QColor _onSliderColor, _offSliderColor;
};
