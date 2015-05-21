#include  "socket.h"
#include <map>
#include <vector>
#include <iterator>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <algorithm>
#include <chrono>

#ifdef NIX
#include <utility>
#include <functional>
#include <pthread.h>
#endif

#define STANDARD_SERVER_PORT 1338
#define STANDARD_SERVER_ADDR "127.0.0.1"
#define MAX_BUFFER_SIZE 65536

struct human_state
{
	std::string name;
	double weight;
	bool operator==(struct human_state rhv)
	{
		return (name == rhv.name && weight == rhv.weight);
	}
};

typedef human_state human_state;

class human
{
public:
	human_state state;
	bool empty;
	bool operator==(human rhv)
	{
		return (empty == rhv.empty && state == rhv.state);
	}
};

class Event
{
public:
	uint32_t type;
	std::string description;
	std::string history;
	uint32_t begin;
	uint32_t end;
	human actor;
	void show(void);
	bool operator==(Event rhv)
	{
		return (type = rhv.type && description == rhv.description && history == rhv.history\
			&& begin == rhv.begin && end == rhv.end && actor == rhv.actor);
	}
};

#define ELEVATOR_VELOCITY 0.5
#define MAX_LOADS_W 500
#define MAX_LOADS_P 5
#define MAX_FLOORS 7
#define MAIN_TIME_CONSTANT 60

typedef struct
{
	double velocity;
	double max_velocity;
	double max_weight;
	double weight;
	uint32_t max_people;
	double x_coord;
	uint32_t max_floors;
	uint32_t floor;
	uint32_t people;
	bool on_floor;
} elevator_state;

class elevator
{
public:
	elevator_state state;
	std::vector<human> *society;
	bool init_elevator(void);
	void destroy_elevator(void);
};

struct floor_ad
{
	uint32_t floor;
	std::string desc;
	bool operator<(struct floor_ad rhv)
	{
		return floor < rhv.floor;
	}
	bool operator==(struct floor_ad rhv)
	{
		return floor == rhv.floor;
	}
};

typedef struct floor_ad floor_ad;

Event parse_message(char *buffer);
Event get_event_from_client(sock_handle client);
Event build_error_event(std::string description);
static void * lift_func(/*elevator &lift*/ void *null);
void interpretate_event(uint32_t floor, elevator &lift);
uint32_t build_way(elevator &lift);
