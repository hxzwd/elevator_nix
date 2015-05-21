#include "socket.h"

#ifdef WIN

bool init_socket(void)
{
	int8_t buffer[1024];
	if (WSAStartup(0x202, (WSADATA *)buffer))
	{
		return false;
	}
	return true;
}

void close_socket(sock_handle close_it)
{
	closesocket(close_it);
}

void clear_network(void)
{
	WSACleanup();
}

bool check_socket(sock_handle check_it)
{
	if (check_it < 0)
		return false;
	else
		return true;
}

sock_handle  get_socket(void)
{
	sock_handle tmp = socket(AF_INET, SOCK_STREAM, 0);
	return tmp;
}

bool bind_addres(sock_handle server, int8_t *addr, uint32_t port)
{
	sockaddr_in server_addr;
	server_addr.sin_port = htons(port);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = 0;
	if (bind(server, (sockaddr *)&server_addr, sizeof(server_addr)))
	{
		return false;
	}
	return true;
}

bool listen_for_clients(sock_handle server, uint32_t clients)
{
	if (listen(server, clients))
		return false;
	else
		return true;
}

sock_handle  accept_client(sock_handle server)
{
	sockaddr_in client_addr;
	int32_t client_size = sizeof(client_addr);
	sock_handle tmp;
	tmp = accept(server, (sockaddr *)&client_addr, &client_size);
	return tmp;
}

bool connect_to_server(sock_handle server, int8_t *addr, uint32_t port)
{
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.S_un.S_addr = inet_addr((char *)addr);
	if (connect(server, (sockaddr *)&server_addr, sizeof(server_addr)))
	{
		return false;
	}
	else
		return true;
}

int32_t send_msg(sock_handle destination, int8_t* buffer, uint32_t buffer_size, uint32_t flags)
{
	return send(destination, (char *)buffer, buffer_size, flags);
}

int32_t get_msg(sock_handle destination, int8_t* buffer, uint32_t buffer_size, uint32_t flags)
{
	return recv(destination, (char *)buffer, buffer_size, flags);
}

#endif

#ifdef NIX

int32_t get_msg(sock_handle destination, int8_t *buffer, uint32_t buffer_size, uint32_t flags)
{
	return recv(destination, (char *)buffer, buffer_size, flags);
}

int32_t send_msg(sock_handle destination, int8_t *buffer, uint32_t buffer_size, uint32_t flags)
{
	return send(destination, (char *)buffer, buffer_size, flags);
}

bool init_socket(void)
{
	return true;
}

void close_socket(sock_handle close_it)
{
	close(close_it);
}

void clear_network(void)
{

}

bool check_socket(sock_handle check_it)
{
	if(check_it < 0)
		return false;
	else
		return true;
}

sock_handle get_socket(void)
{
	sock_handle tmp = socket(AF_INET, SOCK_STREAM, 0);
}

bool bind_addres(sock_handle server, int8_t *addr, uint32_t port)
{
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = 0;
	if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		return false;
	}
	return true;
}

bool listen_for_clients(sock_handle server, uint32_t clients)
{
	if (listen(server, clients))
		return false;
	else
		return true;
}

sock_handle  accept_client(sock_handle server)
{
	struct sockaddr_in client_addr;
	int32_t client_size = sizeof(client_addr);
	sock_handle tmp;
	tmp = accept(server, NULL, NULL);
	return tmp;
}

bool connect_to_server(sock_handle server, int8_t *addr, uint32_t port)
{
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr((char *)addr);
	if (connect(server, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		return false;
	}
	else
		return true;
}




#endif
