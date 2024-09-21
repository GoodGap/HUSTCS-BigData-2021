-- 事务1: 
select  pg_sleep(1);
set session transaction isolation level read committed;
start transaction;
-- 第1次查询航班'MU2455'的余票

select pg_sleep(5);
-- 第2次查询航班'MU2455'的余票

commit;
-- 第3次查询所有航班的余票，发生在事务2提交后
select pg_sleep(1);
select * from ticket  order by flight_no;
