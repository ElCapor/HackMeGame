#ifndef WIDGET_HPP
#define WIDGET_HPP

class rWidget
{
public:
    virtual void Init() = 0;
    virtual void Render() = 0;

    bool isInit = false;
};

namespace ui {
    void RegisterWidget(rWidget* widget);
}

#endif /* WIDGET_HPP */
