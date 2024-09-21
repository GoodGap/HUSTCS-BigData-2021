-- 创建存储过程`sp_fibonacci(in m int)`，向表fibonacci插入斐波拉契数列的前m项，及其对应的斐波拉契数。fibonacci表初始值为一张空表。请保证你的存储过程可以多次运行而不出错。

create procedure sp_fibonacci(in m int)
as
    declare a int default 0;
    declare b int default 1;
    declare countunm int default 2;
    declare temp int default 0;
begin
    if m > 0 then
        insert into fibonacci values (0, 0);
    end if;

    if m > 1 then
        insert into fibonacci values (1, 1);
    end if;

    while m > countunm loop
        insert into fibonacci values (countunm, a + b);
        temp := a + b;
        a:= b;
        b:= temp;
        countunm:= countunm+1;
    end loop;
end;
/