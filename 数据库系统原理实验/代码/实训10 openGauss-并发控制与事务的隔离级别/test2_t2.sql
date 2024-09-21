-- 事务2（采用事务隔离级别- read committed）:
set session transaction isolation level read committed;

-- 开启事务
start transaction;

select  pg_sleep(1);
insert into ticket values('MU5111','A330-200',311);
commit;
