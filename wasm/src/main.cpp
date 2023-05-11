#include "application.h"

#ifdef __EMSCRIPTEN__
extern "C" EMSCRIPTEN_KEEPALIVE int mainf() {
	if (Application::run() != 0) {
		printf("Application failed to run\n");
		return 1;
	}

	return 0;
}

extern "C" EMSCRIPTEN_KEEPALIVE void quit() {
	Application::instance()->quit();
}

#else

int main() {
	if (Application::run() != 0) {
		printf("Application failed to run\n");
		return 1;
	}

	return 0;
}
#endif