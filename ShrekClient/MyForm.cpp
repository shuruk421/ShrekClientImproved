#include "MyForm.h"
using namespace ShrekClient;
int UIMain()
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	// Create the main window and run it
	Application::Run(gcnew MyForm()); //Form is the name of the form created. 
	return 0;
}