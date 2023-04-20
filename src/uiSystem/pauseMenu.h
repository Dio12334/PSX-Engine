#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "uiScreen.h"

namespace psx {
	class PauseMenu: public UIScreen{
		public:
			PauseMenu(class Engine* engine);
			~PauseMenu();

			void HandleKeyPress(i32 key) override;
	};
}

#endif

