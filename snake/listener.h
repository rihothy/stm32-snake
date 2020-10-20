#ifndef _LISTENER_H_
#define _LISTENER_H_

#include <XPT2046_Touchscreen.h>
#include <tuple>

class
{
public:
	
	std::tuple<bool, int, int> listen(XPT2046_Touchscreen& ts)
	{
		int x = 0, y = 0;
		
		auto pnt = ts.getPoint();
		
		if (pnt.z)
		{
			x = map(pnt.y, 3900, 200, 0, 320);
			y = map(pnt.x, 200, 3900, 0, 480);
		}
		
		return std::make_tuple(bool(pnt.z), x, y);
	}
} Listener;

#endif