#pragma once
#include "_components.hh"

namespace UI {
	class Alert {
		public:
			Alert();
			string  text;
			int16_t time;
			void    DoTime();
			void    Render();
	};
};