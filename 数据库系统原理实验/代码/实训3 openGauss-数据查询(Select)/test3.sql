-- 3) 查询既买了保险又买了基金的客户的名称、邮箱和电话。结果依c_id排序
-- 请用一条SQL语句实现该查询：
select
    c_name,
    c_mail,
    c_phone
from client
where exists (
    select
        *
    from property
    where pro_c_id = c_id
    and pro_type = 2
)
and exists (
    select
        *
    from property
    where pro_c_id = c_id
    and pro_type = 3
)
order by c_id;
/*  end  of  your code  */
