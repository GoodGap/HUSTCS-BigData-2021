-- 2) 查询客户表(client)中邮箱信息为null的客户的编号、名称、身份证号、手机号。
--    请用一条SQL语句实现该查询：
select
    c_id,
    c_name,
    c_id_card,
    c_phone
from client
where c_mail is null;
/*  end  of  your code  */