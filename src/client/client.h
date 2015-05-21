#include "socket.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <string>
#include <chrono>

#define STANDARD_SERVER_PORT 1331
#define STANDARD_SERVER_ADDR "127.0.0.1"
#define SECONDS_PER_EVENT 5
#define EVENT_LIMITS 5

typedef struct
{
	std::string name;
	double weight;
} human_state;

class human
{
public:
	human_state state;
	bool empty;
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
	std::string dump(void);
};

human generate_random_actor(void);
Event generate_random_event(void);
bool send_event_on_server(Event ev);

void event_nothing(Event &tmp);
void event_lift_client(Event &tmp, sock_handle server);
