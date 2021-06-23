#pragma once

#include <QAbstractButton>

class QPropertyAnimation;

class SwitchButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QIcon icon READ icon DESIGNABLE false)
    Q_PROPERTY(QSize iconSize READ iconSize DESIGNABLE false)
    Q_PROPERTY(int sliderOffset READ offset WRITE setOffset DESIGNABLE false)
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QString onText MEMBER _onText)
    Q_PROPERTY(double border MEMBER _border)
    Q_PROPERTY(double widthRatio READ widthRatio WRITE setWidthRatio)
    Q_PROPERTY(QColor onBgColor READ onBgColor WRITE setOnBgColor)

public:
    SwitchButton(QWidget *parent = NULL);
    ~SwitchButton();

    QColor onBgColor() const { return _onBgColor; }
    void setOnBgColor(const QColor &color) { _onBgColor = color; update(); }

    float widthRatio() { return _widthRatio; }
    void setWidthRatio(float ratio) { _widthRatio = qMax(1.1f, ratio); if( isChecked() ) _sliderOffset = switchWidth() - height(); update(); }


    virtual QSize sizeHint() const override;

    bool isCheckable() const = delete;
    void setCheckable(bool) = delete;
    void setIcon(const QIcon &icon) = delete;
    void setIconSize(const QSize &size) = delete;

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

    QString _onText;
    float _widthRatio = 2.2f;   // H/W ratio
    float _sliderRatio = 0.25f; // ratio of the "dot" handle over the height
    float _border = 2.0f;       // thickness of the border
    QColor _onBgColor = QColor();
};
