 # 请将你实现flight_booking数据库的语句写在下方：

drop table if exists "user" cascade;
create table "user"
(
	user_id int primary key,
	firstname varchar(50) not null,
	lastname varchar(50) not null,
	dob date not null,
	sex char(1) not null,
	email varchar(50) default '',
	phone varchar(30) default '',
	username varchar(20) not null,
	password char(32) not null,
	admin_tag tinyint not null default 0
);
create unique index username_unq on "user" using btree(username);

drop table if exists passenger cascade;
create table passenger
(
	passenger_id int primary key,
	id char(18) not null,
	firstname varchar(50) not null,
	lastname varchar(50) not null,
	mail varchar(50) default '',
	phone varchar(20) not null,
	sex char(1) not null,
	dob timestamp
);
create unique index id_unq on passenger using btree(id);

drop table if exists airport cascade;
create table airport
(
	airport_id smallint primary key,
	iata char(3) not null,
	icao char(4) not null,
	name varchar(50) not null,
	city varchar(50) default '',
	country varchar(50) default '',
	latitude decimal(11,8) default 0,
	longitude decimal(11,8) default 0
);
create index in_name on airport(name);
create unique index uni_in_iata on airport using btree(iata);
create unique index uni_in_icao on airport using btree(icao);

drop table if exists airline cascade;
create table airline
(
	airline_id int primary key,
	name varchar(30) not null,
	iata char(2) not null,
	airport_id smallint not null,
	constraint FK_airline_airport foreign key (airport_id) references airport(airport_id)
);
create unique index uni_in_iata_l on airline using btree(iata);
create index in_airportid_l on airline using btree(airport_id);

drop table if exists airplane cascade;
create table airplane
(
	airplane_id int primary key,
	type varchar(50) not null,
	capacity smallint not null,
	identifier varchar(50) not null,
	airline_id int not null,
	constraint FK_airplane_airline foreign key (airline_id) references airline(airline_id)
);
create index in_airlineid on airplane using btree(airline_id);

drop table if exists flightschedule cascade;
create table flightschedule(
	flight_no char(8) primary key,
	departure timestamp not null,
	arrival timestamp not null,
	duration smallint not null,
	monday tinyint default 0,
	tuesday tinyint default 0,
	wednesday tinyint default 0,
	thursday tinyint default 0,
	friday tinyint default 0,
	saturday tinyint default 0,
	sunday tinyint default 0,
	airline_id int not null,
	"from" smallint not null,
	"to" smallint not null,
	constraint FK_flightschedule_airline foreign key (airline_id) references airline(airline_id),
	constraint FK_flightschedule_airport1 foreign key ("from") references airport(airport_id),
	constraint FK_flightschedule_airport2 foreign key ("to") references airport(airport_id)
);
create index in_flsc_airlineid on flightschedule using btree(airline_id);
create index in_flsc_from on flightschedule using btree("from");
create index in_flsc_to on flightschedule using btree("to");

drop table if exists flight cascade;
create table flight
(
	flight_id int primary key,
	departure timestamp not null,
	arrivals timestamp not null,
	duration smallint not null,
	airline_id int not null,
	airplane_id int not null,
	flight_no char(8) not null,
	"from" smallint not null,
	"to" smallint not null,
	constraint FK_flight_airline foreign key (airline_id) references airline(airline_id),
	constraint FK_flight_airplane foreign key (airplane_id) references airplane(airplane_id),
	constraint FK_flight_flightschedule foreign key (flight_no) references flightschedule(flight_no),
	constraint FK_flight_airport1 foreign key ("from") references airport(airport_id),
	constraint FK_flight_airport2 foreign key ("to") references airport(airport_id)
);
create index in_fl_arrivals on flight using btree(arrivals);
create index in_fl_departure on flight using btree(departure);
create index in_fl_airlineid on flight using btree(airline_id);
create index in_fl_flightno on flight using btree(flight_no);
create index in_fl_from on flight using btree("from");
create index in_fl_to on flight using btree("to");

drop table if exists ticket cascade;
create table ticket
(
	ticket_id int primary key,
	seat char(4) default '',
	price decimal(10,2) not null,
	flight_id int not null,
	passenger_id int not null,
	user_id int not null,
	constraint FK_ticket_flight foreign key (flight_id) references flight(flight_id),
	constraint FK_ticket_passenger foreign key (passenger_id) references passenger(passenger_id),
	constraint FK_ticket_user foreign key (user_id) references "user"(user_id)
);
create index in_ti_userid on ticket using btree(user_id);
create index in_ti_passengerid on ticket using btree(passenger_id);
create index in_ti_flightid on ticket using btree(flight_id);