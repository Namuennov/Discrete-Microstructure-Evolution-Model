#include "dyskrecjaForm.h"

using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	dyskrecjaGUI::dyskrecjaForm form;
	Application::Run(% form);
}