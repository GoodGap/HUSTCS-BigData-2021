-- 事务1:

set session transaction isolation level read committed;
start transaction;
insert into result
select *, 1 t from ticket where flight_no = 'MU2455';
select pg_sleep(2);
insert into result
select *,1 t from ticket where flight_no = 'MU2455';
commit;