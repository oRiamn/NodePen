#include <string>
#include "XinputWatcher.cpp"

using namespace std;

int main( int argc, const char* argv[] )
{
    XinputWatcher watcher = XinputWatcher();
	std::string inputId = "16";
    watcher.watch(inputId);

	int i = 0;
	while(true){
		printf( " %d %s \n", i, watcher.print_events().c_str());
		i++;
	}
}