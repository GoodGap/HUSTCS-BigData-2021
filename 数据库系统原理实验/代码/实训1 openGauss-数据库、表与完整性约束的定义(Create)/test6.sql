---请在以下空白处填写完成任务的语句，空白行可通过回车换行添加。
---你也可以在命令行窗口完成任务，不过，在命令行键入的语句不会被保存。
drop database MyDb;
create database MyDb;
use MyDb;
create table s(
    sno char(10) primary key,
    name varchar(32) not null,
    ID char(18) unique
);
---结束 