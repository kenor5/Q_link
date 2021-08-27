#include "imagebutton.h"

ImageButton::ImageButton()
{
    setDown(false);
    setFocusPolicy(Qt::NoFocus);
    // 空白图片
    QPixmap map(":/new/prefix1/image/group1/1-1 (0).png");
    clearState = QIcon(map);
}

void ImageButton::changeIcon() {
    if(currentIcon) {
        setIcon(normalState);
        currentIcon = 0;
    } else {
        setIcon(activeState);
        currentIcon = 1;
    }
}

void ImageButton::setButtonIcon(QIcon normal, QIcon active)
{
    normalState = normal;
    activeState = active;
    setIcon(normal);
    currentIcon = 0;
}

void ImageButton::setPlayer(QIcon icon) {
    setIcon(icon);
}

void ImageButton::clearIcon() {
    setIcon(clearState);
}

void ImageButton::exchangeIcon(ImageButton *m)
{
    QIcon tmp1 = activeState;
    QIcon tmp2 = normalState;
    activeState = m->activeState;
    normalState = m->normalState;
    m->activeState = tmp1;
    m->normalState = tmp2;
}

void ImageButton::flushIcon()
{
    setIcon(normalState);
}
