elevator win/nix cross
учитывая случайную генерацию кину пример удачной цепочки событий
выхлоп сервера:

hstw@machine:~/code/elevator_linux$ ./server 
lift thread is detached: id# 139687653050112
in prepare event
elevator loop:> 29
	BUILDWAY:> 0
elevator loop:> 28
	BUILDWAY:> 0
in prepare event
event prepared
elevator loop:> 27
	BUILDWAY:> Y2MfwHN4 vizval lift na 6 etaje
6
poehali s 1 do 6
interpritate event floor is 1
	BUILDWAY:> 6
poehali s 2 do 6
interpritate event floor is 2
	BUILDWAY:> 6
poehali s 3 do 6
interpritate event floor is 3
	BUILDWAY:> 6
in prepare event
event prepared
poehali s 4 do 6
interpritate event floor is 4
	BUILDWAY:> up	0oxhD vizval lift na 1 etaje
6
poehali s 5 do 6
interpritate event floor is 5
	BUILDWAY:> 6
poehali s 6 do 6
interpritate event floor is 6
Y2MfwHN4 zahodit v lift na 6 etaje
	BUILDWAY:> 1
elevator loop:> 26
	BUILDWAY:> 1
poehali s 7 do 1
interpritate event floor is 7
	BUILDWAY:> 1
poehali s 6 do 1
interpritate event floor is 6
	BUILDWAY:> 1
poehali s 5 do 1
interpritate event floor is 5
	BUILDWAY:> 1
poehali s 4 do 1
interpritate event floor is 4
	BUILDWAY:> 1
poehali s 3 do 1
interpritate event floor is 3
	BUILDWAY:> 1
poehali s 2 do 1
interpritate event floor is 2
	BUILDWAY:> 1
poehali s 1 do 1
interpritate event floor is 1
up	0oxhD zahodit v lift na 1 etaje
Y2MfwHN4 pokidaet nash slavniuy lift na 1 etaje
	BUILDWAY:> 6
in prepare event
event prepared
elevator loop:> 25
	BUILDWAY:> o1	5	98V vizval lift na 2 etaje
6
poehali s 0 do 6
interpritate event floor is 0
	BUILDWAY:> 6
poehali s 1 do 6
interpritate event floor is 1
	BUILDWAY:> 6
in prepare event
main loop is aborted
lift thread is end
poehali s 2 do 6
interpritate event floor is 2
o1	5	98V zahodit v lift na 2 etaje
	BUILDWAY:> 7
poehali s 3 do 6
interpritate event floor is 3
	BUILDWAY:> 7
poehali s 4 do 6
interpritate event floor is 4
	BUILDWAY:> 7
poehali s 5 do 6
interpritate event floor is 5
	BUILDWAY:> 7
poehali s 6 do 6
interpritate event floor is 6
up	0oxhD pokidaet nash slavniuy lift na 6 etaje
	BUILDWAY:> 7
elevator loop:> 24
	BUILDWAY:> 7
stoim na 7
interpritate event floor is 7
o1	5	98V pokidaet nash slavniuy lift na 7 etaje
elevator loop:> 23
	BUILDWAY:> 0
elevator loop:> 22
	BUILDWAY:> 0
elevator loop:> 21
	BUILDWAY:> 0
elevator loop:> 20
	BUILDWAY:> 0
elevator loop:> 19
	BUILDWAY:> 0
elevator loop:> 18
	BUILDWAY:> 0
elevator loop:> 17
	BUILDWAY:> 0
elevator loop:> 16
	BUILDWAY:> 0
elevator loop:> 15
	BUILDWAY:> 0
elevator loop:> 14
	BUILDWAY:> 0
elevator loop:> 13
	BUILDWAY:> 0
elevator loop:> 12
	BUILDWAY:> 0
elevator loop:> 11
	BUILDWAY:> 0
elevator loop:> 10
	BUILDWAY:> 0
elevator loop:> 9
	BUILDWAY:> 0
elevator loop:> 8
	BUILDWAY:> 0
^C

выхлоп клиента:

hstw@machine:~/code/elevator_linux$ ./client 
HELLO CLIENT
generated: Thu May 21 15:20:35 2015

actor.empty?: 1
actor.name: empty
actor.weight: 0
type: 0
history: NICHEGO NE PROISHODIT
description: event_nothing
begin: 0
end: 0
generated: Thu May 21 15:20:40 2015

actor.empty?: 0
actor.name: Y2MfwHN4
actor.weight: 50.07
type: 1
history: CHELOVEK HOCHET POEHAT
description: lift_client
begin: 6
end: 1
generated: Thu May 21 15:20:45 2015

actor.empty?: 0
actor.name: up	0oxhD
actor.weight: 85.016
type: 1
history: CHELOVEK HOCHET POEHAT
description: lift_client
begin: 1
end: 6
generated: Thu May 21 15:20:50 2015

actor.empty?: 0
actor.name: o1	5	98V
actor.weight: 54.059
type: 1
history: CHELOVEK HOCHET POEHAT
description: lift_client
begin: 2
end: 7
generated: Thu May 21 15:20:55 2015

actor.empty?: 1
actor.name: empty
actor.weight: 0
type: 0
history: NICHEGO NE PROISHODIT
description: event_nothing
begin: 0
end: 0
generating ended
disconnect
connection destroyed
bye
