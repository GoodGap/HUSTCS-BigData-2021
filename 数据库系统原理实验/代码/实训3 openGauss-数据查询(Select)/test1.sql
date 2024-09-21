-- 1) 查询所有客户的名称、手机号和邮箱信息。查询结果按照客户编号排序。
--    请用一条SQL语句实现该查询：
select
    c_name,
    c_phone,
    c_mail
from client
order by c_id;
/*  end  of  your code  */