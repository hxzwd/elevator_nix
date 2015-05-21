#include "client.h"

#define MAX_NAME_LEN 8
#define MAX_EVENT_TYPES 2
#define MAX_FLOORS 7
#define SECONDS_ON_PACK 30

void Event::show(void)
{
	std::cout << "actor.empty?: " << this->actor.empty << std::endl;
	std::cout << "actor.name: " << (this->actor.state.name).c_str() << std::endl;
	std::cout << "actor.weight: " << this->actor.state.weight << std::endl;
	std::cout << "type: " << this->type << std::endl;
	std::cout << "history: " << this->history.c_str() << std::endl;
	std::cout << "description: " << this->description.c_str() << std::endl;
	std::cout << "begin: " << this->begin << std::endl;
	std::cout << "end: " << this->end << std::endl;
}

std::string Event::dump(void)
{
	return "(actorEmpty)" + std::to_string(this->actor.empty) + \
		"(actorName)" + this->actor.state.name + \
		"(actorWeight)" + std::to_string(this->actor.state.weight) + \
		"(type)" + std::to_string(this->type) + "(history)" + this->history + \
		"(description)" + this->description + \
		"(begin)" + std::to_string(this->begin) + "(end)" + std::to_string(this->end);
}

void event_nothing(Event &tmp)
{
	tmp.actor.empty = true;
	tmp.actor.state.name = "empty";
	tmp.actor.state.weight = 0.0f;
	tmp.history = "NICHEGO NE PROISHODIT";
	tmp.description = "event_nothing";
	tmp.begin = 0;
	tmp.end = 0;
}

void event_lift_client(Event &tmp)
{
	tmp.actor = generate_random_actor();
	tmp.history = "CHELOVEK HOCHET POEHAT";
	tmp.description = "lift_client";
	tmp.begin = 1 + (rand() % MAX_FLOORS);
	tmp.end = 1 + (rand() % MAX_FLOORS);
}

human generate_random_actor(void)
{
	human tmp;
	static const char alphanum[] =
		"0123456789\
		ABCDEFGHIJKLMNOPQRSTUVWXYZ\
		abcdefghijklmnopqrstuvwxyz";
	tmp.state.weight = 38.0 + (rand() % 65) + (double)(rand() % 100) / 1000.0;
	tmp.state.name = "";
	for (uint32_t counter = 0; counter < MAX_NAME_LEN; counter++)
	{
		tmp.state.name += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	tmp.empty = false;
	return tmp;
}



Event generate_random_event(void)
{
	Event tmp;
	tmp.type = rand() % MAX_EVENT_TYPES;
	if (tmp.type == 0)
		event_nothing(tmp);
	if (tmp.type == 1)
		event_lift_client(tmp);

	return tmp;
}

bool send_event_on_server(Event ev, sock_handle server)
{
	char *buffer;
	try
	{
		buffer = new char[ev.dump().length() + 1];
	}
	catch (...)
	{
		return false;
	}
	strcpy(buffer, ev.dump().c_str());
	if (send_msg(server, (int8_t *)buffer, ev.dump().length() + 1, 0) == SOCKET_ERROR)
	{
		return false;
	}
	if (buffer != NULL)
		delete buffer;
	return true;
}

int32_t main(void)
{
	sock_handle server;
	srand(time(0));
	if (!init_socket())
	{
		std::cout << "init_socket error" << std::endl;
		clear_network();
		return 1;
	}
	server = get_socket();
	if (!check_socket(server))
	{
		std::cout << "bad socket" << std::endl;
		close_socket(server);
		clear_network();
		return 1;
	}
	if (!connect_to_server(server, (int8_t *)STANDARD_SERVER_ADDR, STANDARD_SERVER_PORT))
	{
		std::cout << "connection error" << std::endl;
		close_socket(server);
		clear_network();
		return 1;
	}

	int8_t buffer[1024] = { 0 };
	get_msg(server, buffer, 1024, 0);
	std::cout << buffer << std::endl;
	std::chrono::time_point<std::chrono::system_clock> start_clock;



	uint32_t events_limit = EVENT_LIMITS;



	while (events_limit--)
	{
		uint64_t seconds_counter;
		start_clock = std::chrono::system_clock::now();
		Event ev = generate_random_event();
		std::time_t when_gen = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::cout << "generated: " << std::ctime(&when_gen) << std::endl;
		ev.show();
		if (!send_event_on_server(ev, server))
		{
			std::cout << "can not send event" << std::endl;
		}
		while (1)
		{
			seconds_counter = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start_clock).count();
			if (seconds_counter >= SECONDS_PER_EVENT)
				break;
		}
	}

	std::cout << "generating ended" << std::endl;

	std::string get_cmd;
	while (1)
	{
		std::cin >> get_cmd;
		if (get_cmd == "disconnect")
		{
			std::cout << "connection destroyed" << std::endl;
			break;
		}
		std::cout << "unknow command" << std::endl;
	}
	close_socket(server);
	clear_network();
	std::cout << "bye" << std::endl;
	return 0;
}
