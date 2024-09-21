-------请在以下空白处填写完成任务的语句，空白行可通过回车换行添加。
-------你也可以在命令行窗口完成任务，不过，在命令行键入的语句不会被保存。
drop database MyDb;
create database MyDb;
use MyDb;
create table products(
    pid char(10) primary key,
    name varchar(32),
    brand char(10) constraint CK_products_brand check(brand in ('A', 'B')),
    price int constraint CK_products_price check(price > 0)
);
--------结束 