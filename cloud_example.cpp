#include "cloud.h"
#include <iostream>
#include <time.h>

int main() {
	CloudGrid cg("hello", 0.01, 24.0);
	cg.resize_cloud({{-400, -300, 400, 300}});
	cg.write_cloud("cloud.txt");
	return 0;
}
