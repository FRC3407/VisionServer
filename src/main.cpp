#include <vector>
#include <string>

#include "tools/src/resources.h"
#include "tools/src/sighandle.h"
#include "tools/src/timing.h"
#include "tools/src/server/server.h"

#include "core/weightedsubtraction.h"
#include "core/visioncamera.h"
#include "core/visionserver.h"
#include "core/processing.h"
#include "core/vision.h"

#include "2021/testing.h"
#include "2022/rapidreact.h"


StopWatch runtime("Runtime", &std::cout, 0);
void on_exit() { runtime.end(); }

// std::string resourceLookup(std::string&& root, const std::string& item) {
// 	std::cout << "RESOURCE LOOKUP: " << item << newline;
// 	return item;
// }

int main(int argc, char* argv[]) {
	runtime.setStart();
	SigHandle::get();
	atexit(on_exit);

	std::vector<VisionCamera> cameras;

	if(argc > 1 && readConfig(cameras, argv[1])) {}
	else if(readConfig(cameras)) {}
	else { return EXIT_FAILURE; }

	VisionServer vserver(std::move(cameras));
	HttpServer hserver(
		&std::cout,
		"/home/pi",
		HttpServer::findResource,
		HttpServer::supplyResource,
		nullptr,
		Version::HTTP_1_1,
		"81"	// the main WPILibPi page uses port 80
	);
	//server.runVision<SquareTargetPNP, TargetSolver<Test6x6, WeightedSubtraction<VThreshold::LED::BLUE> >, TargetSolver<Test6x6, WeightedSubtraction<VThreshold::LED::GREEN> > >(25);
	vserver.runVisionThread<CargoFinder, StripFinder<VThreshold::LED::GREEN> >(25);
	hserver.serve();
}

// LIST OF THINGS
/*	x = done, x? = kind of done
x Dynamic resizing/scaling
x Position math -> networktables
x Test communication with robot -> target positioning w/ drive program
x multiple cameras -> switching (find out what we want to do)
x compression/stay under bandwidth limit
x Modularize?
? MORE CUSTOM ASSEMBLY!!! :)		<--
x Target abstraction and generalization (pipeline template param)
x System for telling the robot when targeting info is outdated
x Toggle pipeline processing (processing or just streaming)
- Networktables continuity with multiple class instances
- Multiple VisionServer processing instances, data protection/management -> vector of threads?
- Robot-program mode where all settings are determined by robot program over ntables
- Automatically deduce nt-connection mode
*/