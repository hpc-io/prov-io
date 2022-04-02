#include <stdlib.h>
#include <unistd.h>
#include "stat.h"


duration_ht* counts;

void function_1() {
	unsigned long start = get_time_usec();
	//Sleeping for 0.01 second
	sleep(0.01);
	unsigned long elapsed = (get_time_usec() - start);
	accumulate_duration(counts, __func__, elapsed);

}

void function_2() {
	unsigned long start = get_time_usec();
	//Sleeping for 0.02 second
	sleep(0.02);
	unsigned long elapsed = (get_time_usec() - start);
	accumulate_duration(counts, __func__, elapsed);
}

void function_3() {
	unsigned long start = get_time_usec();
	//Sleeping for 0.03 second
	sleep(0.03);
	unsigned long elapsed = (get_time_usec() - start);
	accumulate_duration(counts, __func__, elapsed);
}

int main() {

	counts = stat_create();
	for(int i=0; i< 100; i++) {
		int j = rand() % 4;
		switch(j) {
			case 1:
				function_1();
				break;
			case 2:
				function_2();
				break;
			case 3:
				function_3();
				break;
		}
	}
	stat_print(counts, "stat.txt");
	stat_destroy(counts);
	return 0;
}