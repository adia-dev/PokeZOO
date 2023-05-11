#include "application.h"

#include <ctime>

#ifdef __EMSCRIPTEN__
extern "C" EMSCRIPTEN_KEEPALIVE int mainf() {
	srand(time(NULL));
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
	srand(time(NULL));
	if (Application::run() != 0) {
		printf("Application failed to run\n");
		return 1;
	}

	return 0;
}
#endif