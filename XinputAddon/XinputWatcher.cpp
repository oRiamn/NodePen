#include "XinputWatcher.h"

#define INVALID_EVENT_TYPE -1

int XinputWatcher::motion_type = INVALID_EVENT_TYPE;
int XinputWatcher::button_press_type = INVALID_EVENT_TYPE;
int XinputWatcher::button_release_type = INVALID_EVENT_TYPE;
int XinputWatcher::key_press_type = INVALID_EVENT_TYPE;
int XinputWatcher::key_release_type = INVALID_EVENT_TYPE;
int XinputWatcher::proximity_in_type = INVALID_EVENT_TYPE;
int XinputWatcher::proximity_out_type = INVALID_EVENT_TYPE;

Display XinputWatcher::*dpy;
XDeviceInfo *info = NULL;

XinputWatcher::XinputWatcher()
{
	dpy = XOpenDisplay(NULL);
}

void XinputWatcher::watch(std::string inputID)
{

	// cast std:string to char*
	char *inputIdChar = new char[inputID.length() + 1];
	strcpy(inputIdChar, inputID.c_str());

	info = find_device_info(inputIdChar, True);

	if (!info)
	{
		fprintf(stderr, "unable to find device '%s'\n", inputIdChar);
	}
	else
	{
		if (!register_events(info, inputIdChar, false))
		{
			fprintf(stderr, "no event registered...\n");
		}
	}
}

XDeviceInfo *
XinputWatcher::find_device_info(char *name,
								Bool only_extended)
{
	XDeviceInfo *devices;
	XDeviceInfo *found = NULL;
	int loop;
	int num_devices;
	int len = strlen(name);
	Bool is_id = True;
	XID id = (XID)-1;

	for (loop = 0; loop < len; loop++)
	{
		if (!isdigit(name[loop]))
		{
			is_id = False;
			break;
		}
	}

	if (is_id)
	{
		id = atoi(name);
	}

	devices = XListInputDevices(dpy, &num_devices);

	for (loop = 0; loop < num_devices; loop++)
	{
		if ((!only_extended || (devices[loop].use >= IsXExtensionDevice)) &&
			((!is_id && strcmp(devices[loop].name, name) == 0) ||
			 (is_id && devices[loop].id == id)))
		{
			if (found)
			{
				fprintf(stderr,
						"Warning: There are multiple devices named '%s'.\n"
						"To ensure the correct one is selected, please use "
						"the device ID instead.\n\n",
						name);
				return NULL;
			}
			else
			{
				found = &devices[loop];
			}
		}
	}
	return found;
}

std::string
XinputWatcher::print_events()
{
	XEvent Event;

	setvbuf(stdout, NULL, _IOLBF, 0);

	XNextEvent(dpy, &Event);

	std::string msg = "";

	int loop;
	if (Event.type == motion_type)
	{
		XDeviceMotionEvent *motion = (XDeviceMotionEvent *)&Event;

		msg += "motion ";

		for (loop = 0; loop < motion->axes_count; loop++)
		{
			char buffer[50];
			sprintf(buffer, "a[%d]=%d ", motion->first_axis + loop, motion->axis_data[loop]);
			msg += buffer;
		}
	}
	else if ((Event.type == button_press_type) ||
			 (Event.type == button_release_type))
	{
		int loop;
		XDeviceButtonEvent *button = (XDeviceButtonEvent *)&Event;
		char buffer[50];
		sprintf(buffer, "button %s %d ", (Event.type == button_release_type) ? "release" : "press  ",
				button->button);
		msg += buffer;

		for (loop = 0; loop < button->axes_count; loop++)
		{
			sprintf(buffer, "a[%d]=%d ", button->first_axis + loop, button->axis_data[loop]);
			msg += buffer;
		}
	}
	else if ((Event.type == key_press_type) ||
			 (Event.type == key_release_type))
	{
		int loop;
		XDeviceKeyEvent *key = (XDeviceKeyEvent *)&Event;

		char buffer[50];
		sprintf(buffer, "key %s %d ", (Event.type == key_release_type) ? "release" : "press  ",
				key->keycode);
		msg += buffer;

		for (loop = 0; loop < key->axes_count; loop++)
		{
			sprintf(buffer, "a[%d]=%d ", key->first_axis + loop, key->axis_data[loop]);
			msg += buffer;
		}
	}
	else if ((Event.type == proximity_out_type) ||
			 (Event.type == proximity_in_type))
	{
		int loop;
		XProximityNotifyEvent *prox = (XProximityNotifyEvent *)&Event;

		char buffer[50];
		sprintf(buffer, "proximity %s ", (Event.type == proximity_in_type) ? "in " : "out");
		msg += buffer;

		for (loop = 0; loop < prox->axes_count; loop++)
		{
			sprintf(buffer, "a[%d]=%d ", prox->first_axis + loop, prox->axis_data[loop]);
			msg += buffer;
		}
	}
	else
	{
		msg += printf("what's that %d\n", Event.type);
	}

	return msg;
}

int XinputWatcher::register_events(XDeviceInfo *info,
								   char *dev_name,
								   Bool handle_proximity)
{
	int number = 0; /* number of events registered */
	XEventClass event_list[7];
	int i;
	XDevice *device;
	Window root_win;
	unsigned long screen;
	XInputClassInfo *ip;

	screen = DefaultScreen(dpy);
	root_win = RootWindow(dpy, screen);

	device = XOpenDevice(dpy, info->id);

	if (!device)
	{
		fprintf(stderr, "unable to open device '%s'\n", dev_name);
		return 0;
	}

	if (device->num_classes > 0)
	{
		for (ip = device->classes, i = 0; i < info->num_classes; ip++, i++)
		{
			switch (ip->input_class)
			{
			case KeyClass:
				DeviceKeyPress(device, key_press_type, event_list[number]);
				number++;
				DeviceKeyRelease(device, key_release_type, event_list[number]);
				number++;
				break;

			case ButtonClass:
				DeviceButtonPress(device, button_press_type, event_list[number]);
				number++;
				DeviceButtonRelease(device, button_release_type, event_list[number]);
				number++;
				break;

			case ValuatorClass:
				DeviceMotionNotify(device, motion_type, event_list[number]);
				number++;
				if (handle_proximity)
				{
					ProximityIn(device, proximity_in_type, event_list[number]);
					number++;
					ProximityOut(device, proximity_out_type, event_list[number]);
					number++;
				}
				break;

			default:
				fprintf(stderr, "unknown class\n");
				break;
			}
		}

		if (XSelectExtensionEvent(dpy, root_win, event_list, number))
		{
			fprintf(stderr, "error selecting extended events\n");
			return 0;
		}
	}
	return number;
}
