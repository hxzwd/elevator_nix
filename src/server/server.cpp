#include "server.h"
#define ERROR_TYPE 228


Event build_error_event(std::string description)
{
	Event tmp;
	tmp.type = ERROR_TYPE;
	tmp.description = description;
	return tmp;
}

Event parse_message(char *buffer)
{
	Event tmp;
	std::string str(buffer), sub_str = "";
	uint32_t pos = str.find("(actorEmpty)");
	pos += std::string("(actorEmpty)").length();
	while (str[pos] != '(')
	{
		sub_str += str[pos];
		pos++;
	}
	if (sub_str == "0")
		tmp.actor.empty = false;
	else
		tmp.actor.empty = true;
	pos = str.find("(actorName)");
	pos += std::string("(actorName)").length();
	sub_str = "";
	while (str[pos] != '(')
	{
		sub_str += str[pos];
		pos++;
	}
	tmp.actor.state.name = sub_str;
	pos = str.find("(actorWeight)");
	pos += std::string("(actorWeight)").length();
	sub_str = "";
	while (str[pos] != '(')
	{
		sub_str += str[pos];
		pos++;
	}
	tmp.actor.state.weight = std::stof(sub_str);
	pos = str.find("(type)");
	pos += std::string("(type)").length();
	sub_str = "";
	while (str[pos] != '(')
	{
		sub_str += str[pos];
		pos++;
	}
	tmp.type = std::stoi(sub_str);
	pos = str.find("(history)");
	pos += std::string("(history)").length();
	sub_str = "";
	while (str[pos] != '(')
	{
		sub_str += str[pos];
		pos++;
	}
	tmp.history = sub_str;
	pos = str.find("(description)");
	pos += std::string("(description)").length();
	sub_str = "";
	while (str[pos] != '(')
	{
		sub_str += str[pos];
		pos++;
	}
	tmp.description = sub_str;
	pos = str.find("(begin)");
	pos += std::string("(begin)").length();
	sub_str = "";
	while (str[pos] != '(')
	{
		sub_str += str[pos];
		pos++;
	}
	tmp.begin = std::stoi(sub_str);
	pos = str.find("(end)");
	pos += std::string("(end)").length();
	sub_str = "";
	while (str[pos] != '\0')
	{
		sub_str += str[pos];
		pos++;
	}
	tmp.end = std::stoi(sub_str);
	return tmp;
}

Event get_event_from_client(sock_handle client)
{
	Event tmp;
	char buffer[MAX_BUFFER_SIZE] = { 0 };
	uint32_t length = get_msg(client, (int8_t *)buffer, MAX_BUFFER_SIZE, 0);
	if (length <= 0)
	{
		return build_error_event("length of message is less or equal zero");
	}
	tmp = parse_message(buffer);
	return tmp;
}

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

bool elevator::init_elevator(void)
{
	this->state.floor =  1;
	this->state.max_floors = MAX_FLOORS;
	this->state.max_people = MAX_LOADS_P;
	this->state.max_weight = MAX_LOADS_W;
	this->state.on_floor = true;
	this->state.people = 0;
	this->state.velocity = 0.0f;
	this->state.weight = 0.0f;
	this->state.max_velocity = ELEVATOR_VELOCITY;
	this->state.x_coord = 1.0f;
	try
	{
		this->society = new std::vector<human>;
	}
	catch (...)
	{
		return false;
	}
	return true;
}

void elevator::destroy_elevator(void)
{
	if (this->society != NULL)
		delete this->society;
}


std::multimap<uint32_t, Event> *q_event = NULL;
std::vector<floor_ad> *q_floor = NULL;

std::mutex g_lock;

void prepare_event(Event ev)
{


	std::cout << "in prepare event" << std::endl;
	
	
	if (ev.type == 0)
		return;
	if (ev.type == 1)
	{
		floor_ad in;
		in.desc = ev.actor.state.name + " vizval lift na " + std::to_string(ev.begin) + " etaje";
		in.floor = ev.begin;
		ev.description = "lift_get_actor";
		ev.history = "zashel v lift";
		g_lock.lock();
		q_event->insert(std::pair<uint32_t, Event>(ev.begin, ev));
		q_floor->push_back(in);
		g_lock.unlock();
		
		std::cout << "event prepared" << std::endl;

	}
}

elevator lift;
bool thread_done = false;

int32_t main(void)
{
	sock_handle server, client;
	if (!init_socket())
	{
		std::cout << "init_socket error";
		clear_network();
		return 1;
	}
	server = get_socket();
	if (!check_socket(server))
	{
		std::cout << "bad socket";
		clear_network();
		return 1;
	}
	if (!bind_addres(server, (int8_t *)"", STANDARD_SERVER_PORT))
	{
		std::cout << "bind error";
		close_socket(server);
		clear_network();
		return 1;
	}

	if (!listen_for_clients(server, 1))
	{
		std::cout << "listen error";
		close_socket(server);
		clear_network();
		return 1;
	}
	client = accept_client(server);
	if (!check_socket(client))
	{
		std::cout << "client connection error";
		close_socket(server);
		close_socket(client);
		clear_network();
		return 1;
	}
	send_msg(client, (int8_t *)"HELLO CLIENT", std::string("HELLO CLIENT").length() + 1, 0);
	try
	{
		q_floor = new std::vector<floor_ad>;
		q_event = new std::multimap<uint32_t, Event>;
	}
	catch (...)
	{
		std::cout << "memory allocation error" << std::endl;
		close_socket(server);
		close_socket(client);
		clear_network();
		return 1;
	}
	if (!lift.init_elevator())
	{
		std::cout << "cannot init elevator" << std::endl;
		close_socket(server);
		close_socket(client);
		clear_network();
		if (q_event != NULL)
			delete q_event;
		if (q_floor != NULL)
			delete q_floor;
		return 1;
	}
	
#ifdef NIX
	pthread_t thr_lift;
	pthread_create(&thr_lift, NULL, lift_func, NULL);
	pthread_detach(thr_lift);
#endif

#ifdef WIN
	std::thread thr_lift(&lift_func, NULL);
	thr_lift.detach();
#endif

	uint32_t server_main_loop_counter = EVENTS_LIMIT;
	while (server_main_loop_counter--)
	{
		Event ev;
		ev = get_event_from_client(client);
		if (ev.type == ERROR_TYPE)
		{
			std::cout << "ERROR: " << ev.description << std::endl;
		}
		else
		{

//			ev.show();

			prepare_event(ev);
			
		}
		
	}





	/*

	char buffer[65536] = { 0 };
	uint32_t length =  get_msg(client, (int8_t *)buffer, 65536, 0);
	std::cout << "BUFFER:\n";
	std::cout << buffer << std::endl;

	*/
	
	std::cout << "main loop is aborted" << std::endl;

	uint32_t message_count_in_end = 1;
	int32_t super_delay = 10;
// 	while (1)
// 	{
// 		if(g_lock.try_lock())
// 		{
// 			if(thread_done)
// 			{
// 				g_lock.unlock();
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			std::cout << "cannot lock in parent thread wait for 1++ seconds" << std::endl;
// 			std::this_thread::sleep_for(std::chrono::seconds(super_delay++));
// 		}
// 		if (message_count_in_end)
// 		{
// 			std::cout << "potok eshe ne zakonchil raboty" << std::endl;
// 			message_count_in_end--;
// 		}
// 
// 	}

	std::cout << "lift thread is end" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(WORK_TIME));
	close_socket(server);
	close_socket(client);
	clear_network();
	if (q_event != NULL)
		delete q_event;
	if (q_floor != NULL)
		delete q_floor;
	lift.destroy_elevator();
	return 0;
}

static void * lift_func(/*elevator &lift*/ void *null)
{
	g_lock.lock();
	std::cout << "lift thread is detached: id# " << std::this_thread::get_id() << std::endl;
	g_lock.unlock();
	uint32_t loop_counter = THREAD2_LOOP_COUNTER;
	uint32_t flr, start_point, end_point;
	std::chrono::time_point<std::chrono::system_clock> start_time;
	while (loop_counter--)
	{
		uint64_t seconds_counter;
		start_time = std::chrono::system_clock::now();
		std::cout << "elevator loop:> " << loop_counter << std::endl;

		if(g_lock.try_lock())
		{
			flr = build_way(lift);
			g_lock.unlock();
		}
		else
		{
			std::cout << "cannot lock wait 2 seconds\n";
			std::this_thread::sleep_for(std::chrono::seconds(2));
			loop_counter++;
			continue;
		}

		if (flr != 0)
		{
			if (lift.state.floor < flr)
			{
				start_point = lift.state.floor;
				end_point = flr;
				for (; start_point <= end_point; start_point++)
				{
#ifdef FLOOR_TO_FLOOR_DELAY_ENABLE
std::this_thread::sleep_for(std::chrono::seconds(FLOOR_TO_FLOOR_TIME));					
#endif
					std::cout << "poehali s " << start_point << " do " << end_point << std::endl;
					g_lock.lock();
					interpretate_event(start_point, lift);
					build_way(lift);
					g_lock.unlock();
					lift.state.floor++;
					lift.state.x_coord += 1.0;
				}
			}
			else
			if (lift.state.floor > flr)
			{
#ifdef FLOOR_TO_FLOOR_DELAY_ENABLE
std::this_thread::sleep_for(std::chrono::seconds(FLOOR_TO_FLOOR_TIME));					
#endif
				start_point = lift.state.floor;
				end_point = flr;
				for (; start_point >= end_point; start_point--)
				{
					std::cout << "poehali s " << start_point << " do " << end_point << std::endl;
					g_lock.lock();
					interpretate_event(start_point, lift);
					build_way(lift);
					g_lock.unlock();
					lift.state.floor--;
					lift.state.x_coord -= 1.0;
				}
			}
			else if (lift.state.floor == flr)
			{
				std::cout << "stoim na " << flr << std::endl;
				g_lock.lock();
				interpretate_event(flr, lift);
				g_lock.unlock();
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::seconds(3));
			continue;
		}

		while (1)
		{
			seconds_counter = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start_time).count();
			if (seconds_counter >= MAIN_TIME_CONSTANT)
				break;
		}

	}
	g_lock.lock();
	thread_done = true;
	g_lock.unlock();
	return NULL;

}

uint32_t build_way(elevator &lift)
{
	std::cout << "\tBUILDWAY:> ";
	for (auto it = q_floor->begin(); it != q_floor->end(); ++it)
	{
		if ((*it).desc != "")
		{
			std::cout << (*it).desc << std::endl;
			(*it).desc = "";
		}
	}
	if (!q_floor->empty())
	{
		std::cout << q_floor->at(0).floor << std::endl;
		return q_floor->at(0).floor;
	}
	std::cout << 0 << std::endl;
	return 0;

	/*
	std::sort(q_floor->begin(), q_floor->end());
	if (!lift.state.on_floor)
	{
		g_lock.unlock();
		return;
	}
	uint32_t len = q_floor->size();
	for (uint32_t i = 0; i < len; i++)
	{
		if (q_floor->at(i).floor == lift.state.floor)
			break;
	}
	*/
}

void interpretate_event(uint32_t floor, elevator &lift)
{
	bool leave = false;
	std::cout << "interpritate event floor is " << floor << std::endl;
	std::pair<std::multimap<uint32_t, Event>::iterator, std::multimap<uint32_t, Event>::iterator> range;
	range = q_event->equal_range(floor);
	for (; range.first != range.second;)
	{
		if (leave)
			break;
		if (range.first == range.second) break;
		if ((*range.first).second.description == "lift_get_actor")
		{
			if (lift.state.people == lift.state.max_people)
			{
				std::cout << "slishkom mnogo ludey v lifte" << std::endl;
				std::cout << "gospodin " << (*range.first).second.actor.state.name << " jdet eshe" << std::endl;
			}
			if (lift.state.weight + (*range.first).second.actor.state.weight > lift.state.max_weight)
			{
				std::cout << "lift stol`ko ne viderzhit" << std::endl;
				std::cout << "gospodin " << (*range.first).second.actor.state.name << " s massoy " << (*range.first).second.actor.state.weight << " jdet eshe";
			}
			lift.society->push_back((*range.first).second.actor);
			std::cout << (*range.first).second.actor.state.name << " zahodit v lift";
			std::cout << " na " << (*range.first).second.begin << " etaje" << std::endl;
			lift.state.people++;
			lift.state.weight += (*range.first).second.actor.state.weight;
			floor_ad remove_it;
			remove_it.desc = "";
			remove_it.floor = floor;
			std::vector<floor_ad>::iterator it = std::find(q_floor->begin(), q_floor->end(), remove_it);
			q_floor->erase(it);
			remove_it.floor = (*range.first).second.end;


			
//			q_floor->push_back(remove_it);
			q_floor->insert(q_floor->begin(), remove_it);
			
			
			Event ev;
			ev = (*range.first).second;
			ev.description = "lift_drop_actor";
			q_event->insert(std::pair<uint32_t, Event>(((*range.first).second.end), ev));
//			std::multimap<uint32_t, Event>::iterator tmp = range.first;
			std::multimap<uint32_t, Event>::iterator tmp = range.first;
			++range.first;
			q_event->erase(tmp);
/*			auto ito = std::find(q_event->begin(), q_event->end(), std::pair<uint32_t, Event>(floor, (*range.first).second));
			q_event->erase(ito); */
		}
		else
		if ((*range.first).second.description == "lift_drop_actor")
		{
			if (floor != (*range.first).second.end)
			{
				++range.first;
				continue;
			}
			std::cout << (*range.first).second.actor.state.name << " pokidaet nash slavniuy lift na ";
			std::cout << floor << " etaje" << std::endl;
			lift.state.people--;
			lift.state.weight -= (*range.first).second.actor.state.weight;
			auto it0 = std::find(lift.society->begin(), lift.society->end(), (*range.first).second.actor);
			lift.society->erase(it0);
			floor_ad remove_it;
			remove_it.desc = "";
			remove_it.floor = floor;
			auto it1 = std::find(q_floor->begin(), q_floor->end(), remove_it);
			if (it1 != q_floor->end())
			{
				q_floor->erase(it1);
			}
			std::multimap<uint32_t, Event>::iterator tmp = range.first;
			++range.first;
			q_event->erase(tmp);
//			auto it = std::find(q_event->begin(), q_event->end(), std::pair<uint32_t, Event>(floor, (*range.first).second));
//			q_event->erase(it);
		}
	}
}
