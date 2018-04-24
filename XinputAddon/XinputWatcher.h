#include <iostream>
#include <string.h>
#include <X11/extensions/XInput.h>

#ifndef XINPUTWATCHER_H
#define XINPUTWATCHER_H

using namespace std;


class XinputWatcher
{
    public:
        XinputWatcher();
        void watch(std::string inputID);
        std::string print_events();

    protected:

    private:
        static int motion_type;
        static int button_press_type;
        static int button_release_type;
        static int key_press_type;
        static int key_release_type;
        static int proximity_in_type;
        static int proximity_out_type;

        Display *dpy;

        XDeviceInfo * find_device_info(char *name, Bool only_extended);
        int register_events(XDeviceInfo *info, char *dev_name,	Bool handle_proximity);
};

#endif // XINPUTWATCHER_H
