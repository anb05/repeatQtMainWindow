#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

namespace spr_sht {

class Application : public QApplication
{
	Q_OBJECT
public:
	explicit Application(int& argc, char** argv);
	~Application() override;

	explicit Application(const Application&)    = delete ;
	explicit Application(Application&&)         = delete ;

	Application& operator= (const Application&) = delete ;
	Application& operator= (Application&&)      = delete ;
};

} // namespace spr_sht

#endif // APPLICATION_H
