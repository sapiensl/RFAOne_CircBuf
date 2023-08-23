#include "CircularBuffer.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

//the testing is done using the Catch2 framework which is included as a single header file.
//the test case below will run as many times as there are sections in it, calling one section after another in isolation.

TEST_CASE("base CircularBuffer functionality") {
	CircularBuffer<int, 3> cb;
	int elem;
	
	SECTION("check init condition") {
		REQUIRE(cb.count() == 0);
		REQUIRE(cb.pop(elem) == false);
	}

	SECTION("pushing and popping a single element") {
		cb.push(123);

		REQUIRE(cb.count() == 1);
		REQUIRE(cb.pop(elem) == true);
		REQUIRE(elem == 123);
	}

	SECTION("pushing and popping multiple elements to check order") {
		cb.push(123);
		cb.push(321);

		REQUIRE(cb.count() == 2);
		REQUIRE(cb.pop(elem) == true);
		REQUIRE(elem == 123);
		REQUIRE(cb.pop(elem) == true);
		REQUIRE(elem == 321);
	}

	SECTION("overwriting through overflow, check count and read position") {
		cb.push(1); // elem0
		cb.push(2); // elem1
		cb.push(3); // elem2
		REQUIRE(cb.count() == 3);

		cb.push(4); // elem0 overwritten, count still 3, read position on elem1 -> 2
		REQUIRE(cb.count() == 3);
		REQUIRE(cb.pop(elem) == true);
		REQUIRE(elem == 2);
		REQUIRE(cb.pop(elem) == true);
		REQUIRE(elem == 3);
		REQUIRE(cb.pop(elem) == true);
		REQUIRE(elem == 4);

		//check if considered empty now
		REQUIRE(cb.count() == 0);
		REQUIRE(cb.pop(elem) == false);
	}
}