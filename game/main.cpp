#include <stdio.h>
#include "core/base/log.h"

int main() {
	log::info("This is info");
	log::success("This is sucess");
	log::error("This is error");
	log::panic("This is panic");
	return 0;
}
