#include "application.h"

namespace spr_sht {

Application::Application(int& argc, char** argv)
	: QApplication(argc, argv)
{  }

Application::~Application() = default;

} // namespace spr_sht
