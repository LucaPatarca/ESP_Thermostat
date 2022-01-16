#include <screen/screen.h>

class NotificationScreen : public Screen
{
public:
    using Screen::Screen;

    void setText(const String&);

    void draw() override;
    void refresh() override;
private:
    String m_text;
    bool m_needsUpdate;
};