#include "pch.h"

#include <Kore/Log.h>
#include <Kore/System.h>
#include <Kore/Network/Connection.h>

//#define ECHO_TEST

using namespace Kore;

namespace {
	const int port = 7734;
	const char* url = "localhost";

	Connection *conn;
	unsigned char buff[256];

	void update() {
		#ifndef ECHO_TEST
		const unsigned char data[] = "ping";
		conn->send(data, 4);
		#endif

		int got;
		while ((got = conn->receive(buff)) > 0) {
			#ifdef ECHO_TEST
			buff[got] = '!';
			conn->send(buff, got + 1);
			#endif

			buff[got] = '\0';
			log(LogLevel::Info, "Received %i bytes: %s", got, buff);
		}
	}
}

int kore(int argc, char** argv) {
	char* name = "UDP Test";

	Kore::System::setName(name);
	Kore::System::setup();
	Kore::WindowOptions options;
	options.title = name;
	options.width = 1024;
	options.height = 768;
	options.x = 100;
	options.y = 100;
	options.targetDisplay = -1;
	options.mode = WindowModeWindow;
	options.rendererOptions.depthBufferBits = 16;
	options.rendererOptions.stencilBufferBits = 8;
	options.rendererOptions.textureFormat = 0;
	options.rendererOptions.antialiasing = 0;
	Kore::System::initWindow(options);

	#ifdef ECHO_TEST
	conn = new Connection(url, port + 1, port);
	#else
	conn = new Connection(url, port, port + 1);
	#endif

	Kore::System::setCallback(update);
	Kore::System::start();

	return 0;
}
