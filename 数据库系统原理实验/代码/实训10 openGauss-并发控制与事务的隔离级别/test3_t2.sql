-- 事务2:
set session transaction isolation level read uncommitted;
start transaction;
select pg_sleep(2);
--在事务1的第1，2次查询之间，试图出票1张(航班MU2455)：

commit;
