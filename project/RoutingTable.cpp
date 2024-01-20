#include "machine.h"


RoutingTable::RoutingTable(Machine* m1)
{
	data = m1;
	next = 0;
	prev = 0;
}