#include "ShowText.h"

int main(int argc, const char* argv[])
{
	Initialize("ledLib32.dll", argc, argv);
	for (;;) {
		CString text(_T("オレの名前は内田だ！"));
		ShowText(text);
		exit(0);
    }
}
