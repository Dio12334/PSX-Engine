#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include "uiScreen.h"

namespace psx {
	class DialogBox: public UIScreen{
		public:
			DialogBox(class Engine* engine, const std::string& text, std::function<void()> onOK);
			~DialogBox();
	};
}

#endif

